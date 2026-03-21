#include <serial.hpp>
#include <pcie.hpp>

#include <cstdint>

bool PCIE::_init = false;
uint64_t PCIE::_pci_base = 0x0;

void PCIE::init(uint64_t pci_base) {
    PCIE::_init = true;
    PCIE::_pci_base = pci_base;
}

void PCIE::enumerate() {
    if (!PCIE::_init) {
        return;
    }

    Serial::puts("[PCIE] Enumerating PCIe devices...\n");
    for (uint16_t bus = 0; bus <= 255; bus++) {
        for (uint8_t device = 0; device < 32; device++) {

            uint64_t offset = ((uint64_t) bus << 20) | ((uint64_t) device << 15);
            uint32_t* config_space = (uint32_t*) (PCIE::_pci_base + offset);

            uint32_t id_reg = *config_space;
            uint16_t vendor_id = id_reg & 0xFFFF;

            if (vendor_id == 0xFFFF) {
                continue;
            }

            uint16_t device_id = (id_reg >> 16) & 0xFFFF;

            Serial::kprintf("[PCIE] %d:%d | Vendor ID: 0x%x | Device ID: 0x%x\n",
                         bus, device, vendor_id, device_id);
        }
    }
}
