#include <virtio.hpp>
#include <virtio_queues.hpp>
#include <mmio.hpp>
#include <pcie.hpp>
#include <serial.hpp>

struct virtq virtio_net::rx;
struct virtq virtio_net::tx;

uint16_t* virtio_net::rx_reg;
uint16_t* virtio_net::tx_reg;

uint8_t* virtio_net::get_virtio_cap(uint8_t cfg_type) {
    if (!this->has_caps()) {
        return nullptr;
    }

    uint8_t ptr = this->cap_ptr();
    while (ptr) {
        auto cap = reinterpret_cast<struct virtio_pci_cap*>(_cfg_space + ptr);
        if (cap->cap_vndr == 0x09 && cap->cfg_type == cfg_type) {
            return _cfg_space + ptr;
        }

        ptr = cap->cap_next;
    }

    return nullptr;
}

struct virtio_pci_common_cfg* virtio_net::config(int virtio_config) {
    uint8_t* cap_ptr = get_virtio_cap(virtio_config);
    if (!cap_ptr) {
        return nullptr;
    }

    auto vcap = reinterpret_cast<struct virtio_pci_cap*>(cap_ptr);
    uintptr_t bar_addr = reinterpret_cast<uintptr_t>(this->get_bar_addr(vcap->bar));

    if (!bar_addr) {
        return nullptr;
    }

    return reinterpret_cast<struct virtio_pci_common_cfg*>(bar_addr + vcap->offset);
}

void virtio_net::setup_virtq() {
    auto cfg = this->config(VIRTIO_PCI_CAP_COMMON_CFG);

    uint8_t* ncap_ptr = get_virtio_cap(VIRTIO_PCI_CAP_NOTIFY_CFG);
    auto ncap = reinterpret_cast<struct virtio_pci_notify_cap*>(ncap_ptr);

    auto notify_bar_base = reinterpret_cast<uintptr_t>(this->get_bar_addr(ncap->cap.bar)) + ncap->cap.offset;
    uint32_t multiplier = ncap->notify_off_multiplier;

    for (int i = 0; i < 2; i++) {
        cfg->queue_select = i;
        MMIO::memory_barrier();

        auto notify = notify_bar_base + (cfg->queue_notify_off * multiplier);

        struct virtq* vq;
        if (i == 0) {
            vq = &virtio_net::rx;
            virtio_net::rx_reg = reinterpret_cast<uint16_t*>(notify);
        } else {
            vq = &virtio_net::tx;
            virtio_net::tx_reg = reinterpret_cast<uint16_t*>(notify);
        }

        vq->num = 256;
        cfg->queue_desc  = reinterpret_cast<uint64_t>(vq->desc);
        cfg->queue_avail = reinterpret_cast<uint64_t>(&vq->avail);
        cfg->queue_used  = reinterpret_cast<uint64_t>(&vq->used);

        cfg->queue_enable = 1;
        MMIO::memory_barrier();
    }
}

void virtio_net::init_virtio_net() {
    auto dev = PCIe::get(0x1af4, 0x1041);
    if (!dev.valid()) {
        Serial::puts("virtio-net not found !\n");
        return;
    }

    dev.set_bar_addr(4, 0x10000000);
    Serial::kprintf("Device get bar addr : %X\n", dev.get_bar_addr(4));
    uint32_t* cmd_reg = dev.cmd_reg();

    *cmd_reg |= (1 << 1) | (1 << 2);

    auto vio = reinterpret_cast<virtio_net*>(&dev);

    auto virtio_cfg = vio->config(VIRTIO_PCI_CAP_COMMON_CFG);

    // RESET
    virtio_cfg->device_status = 0;
    MMIO::memory_barrier();

    // ACKNOWLEDGE
    virtio_cfg->device_status |= VIRTIO_CONFIG_S_ACKNOWLEDGE;
    MMIO::memory_barrier();

    // DRIVER
    virtio_cfg->device_status |= VIRTIO_CONFIG_S_DRIVER;
    MMIO::memory_barrier();

    virtio_cfg->device_feature_select = 0;
    MMIO::memory_barrier();

    int wanted_feature = VIRTIO_NET_F_MAC;
    int device_feature = virtio_cfg->device_feature;
    virtio_cfg->driver_feature = device_feature & wanted_feature;

    virtio_cfg->device_status |= VIRTIO_CONFIG_S_FEATURES_OK;
    MMIO::memory_barrier();

    Serial::kprintf("Device status: %x\n", virtio_cfg->device_status);
    Serial::kprintf("Device feature: %x\n", virtio_cfg->device_feature);
    Serial::kprintf("Num queues: %d\n", virtio_cfg->num_queues);
    Serial::kprintf("Queue size: %d\n", virtio_cfg->queue_size);

    vio->setup_virtq();
    Serial::kprintf("Finished setup_virtq\n");

    virtio_cfg->device_status |= VIRTIO_CONFIG_S_DRIVER_OK;
    MMIO::memory_barrier();

    Serial::kprintf("Finished setup\n");
    Serial::kprintf("Device status: %x\n", virtio_cfg->device_status);
}
