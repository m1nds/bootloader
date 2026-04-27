#include <pcie.hpp>

pci_device::pci_device() {
    _bus = 0;
    _device = 0;
    _function = 0;
    _cfg_space = nullptr;
    _vendor_id = 0xFFFF;
    _device_id = 0xFFFF;
}

pci_device::pci_device(uint16_t bus, uint8_t device, uint16_t function, uint8_t* cfg_space) {
    this->_bus = bus;
    this->_device = device;
    this->_function = function;
    this->_cfg_space = cfg_space;

    uint32_t id_reg = *(reinterpret_cast<uint32_t*>(cfg_space));
    this->_vendor_id = id_reg & 0xFFFF;
    this->_device_id = (id_reg >> 16) & 0xFFFF;
}

bool pci_device::valid() {
    return _cfg_space != nullptr && this->_vendor_id != 0xFFFF && this->_device_id != 0xFFFF;
}

uint16_t pci_device::vendor() {
    return this->_vendor_id;
}

uint16_t pci_device::device() {
    return this->_device_id;
}

void* pci_device::get_bar_addr(uint8_t bar) {
    if (bar > 5) {
        return nullptr;
    }

    uint32_t* bar_ptr_low = reinterpret_cast<uint32_t*>(this->_cfg_space + 0x10 + 4 * bar);
    uint32_t* bar_ptr_high = reinterpret_cast<uint32_t*>(this->_cfg_space + 0x10 + 4 * (bar + 1));

    uint32_t bar_value_low = bar_ptr_low[0];

    uint64_t addr = bar_value_low & ~0xF;

    if ((bar_value_low & 0x6) == 0x4) {
        uint32_t bar_value_high = bar_ptr_high[0];
        addr |= ((uint64_t) bar_value_high << 32);
    }

    return reinterpret_cast<void*>(addr);
}

bool pci_device::set_bar_addr(uint8_t bar, uint64_t addr) {
    if (bar > 5) {
        return false;
    }

    uint32_t* bar_ptr = reinterpret_cast<uint32_t*>(this->_cfg_space + 0x10 + 4 * bar);
    uint32_t bar_value = *(bar_ptr);

    bar_ptr[0] = addr & 0xFFFFFFFF;

    if ((bar_value & 0x6) == 0x4) {
        bar_ptr[1] = (addr >> 32) & 0xFFFFFFFF;
    }

    return true;
}

uint32_t* pci_device::cmd_reg() {
    return reinterpret_cast<uint32_t*>(_cfg_space + 0x04);
}

bool pci_device::has_caps() {
    return *(reinterpret_cast<uint16_t*>(_cfg_space + 0x6)) & (1 << 4);
}

uint8_t pci_device::cap_ptr() {
    return *(_cfg_space + 0x34);
}

uint8_t* pci_device::get_cap(uint8_t cap_id) {
    if (!this->has_caps()) {
        return nullptr;
    }

    uint8_t ptr = this->cap_ptr();
    while (ptr) {
        uint8_t id =_cfg_space[ptr];
        if (id == cap_id) {
            return _cfg_space + ptr;
        }

        ptr = *(_cfg_space + ptr + 1);
    }

    return nullptr;
}
