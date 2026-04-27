#include <virtio.hpp>
#include <pcie.hpp>
#include <serial.hpp>

struct virtio_pci_common_cfg* virtio_net::config(int virtio_config) {
    if (!this->has_caps()) {
        return nullptr;
    }

    uint8_t ptr = this->cap_ptr();
    while (ptr) {
        if (_cfg_space[ptr] == 0x09) {
            auto vcap = reinterpret_cast<struct virtio_pci_cap*>(_cfg_space + ptr);
            if (vcap->cfg_type == virtio_config) {
                uintptr_t bar_addr = reinterpret_cast<uintptr_t>(this->get_bar_addr(vcap->bar));
                if (bar_addr == 0) {
                    return nullptr;
                }

                return reinterpret_cast<struct virtio_pci_common_cfg*>(bar_addr + vcap->offset);
            }
        }
        ptr = _cfg_space[ptr + 1];
    }
    return nullptr;
}
