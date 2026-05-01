#include <serial.hpp>
#include <pcie.hpp>

#include <cstdint>

bool PCIe::_init = false;
uint64_t PCIe::_pci_base = 0x0;

void PCIe::init(uint64_t pci_base) {
    PCIe::_init = true;
    PCIe::_pci_base = pci_base;
}

static inline uint64_t offset(uint16_t bus, uint8_t device, uint8_t function) {
    return (bus << 20) | (device << 15) | (function << 12);
}

void PCIe::enumerate() {
    if (!PCIe::_init) {
        Serial::puts("[PCIe] Get before init!\n");
        return;
    }

    for (uint16_t bus = 0; bus <= 255; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t function = 0; function < 8; function++) {

            uint64_t off = offset(bus, device, function);
            uint8_t* cfg_space = reinterpret_cast<uint8_t*>(PCIe::_pci_base + off);
            uint32_t id_reg = *(reinterpret_cast<uint32_t*>(cfg_space));

            uint16_t vendor_id = id_reg & 0xFFFF;
            uint16_t device_id = (id_reg >> 16) & 0xFFFF;

            if (vendor_id == 0xFFFF || device_id == 0xFFFF) {
                continue;
            }

            Serial::kprintf("[PCIe] %d:%d.%d | Vendor ID: %x | Device ID: %x\n",
                         bus, device, function, vendor_id, device_id);
            }
        }
    }
}

pci_device PCIe::get(uint16_t target_vendor_id, uint16_t target_device_id) {
    if (!PCIe::_init) {
        Serial::puts("[PCIe] Get before init!\n");
        return pci_device();
    }

    for (uint16_t bus = 0; bus <= 255; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t function = 0; function < 8; function++) {

                uint64_t off = offset(bus, device, function);
                uint8_t* cfg_space = reinterpret_cast<uint8_t*>(PCIe::_pci_base + off);
                uint32_t id_reg = *(reinterpret_cast<uint32_t*>(cfg_space));

                uint16_t vendor_id = id_reg & 0xFFFF;
                uint16_t device_id = (id_reg >> 16) & 0xFFFF;

                if (vendor_id != target_vendor_id || device_id != target_device_id) {
                    continue;
                }

                return pci_device((uint16_t) bus, (uint8_t) device, (uint8_t) function, cfg_space);
            }
        }
    }
    return pci_device();
}
