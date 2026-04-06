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
    return this->_vendor_id;
}
