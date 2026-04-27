#ifndef PCIE_HPP
#define PCIE_HPP

#include <cstdint>

class pci_device {
    public:
        pci_device();
        pci_device(uint16_t bus, uint8_t device, uint16_t function, uint8_t* cfg_space);
        uint16_t vendor();
        uint16_t device();
        bool valid();

        void* get_bar_addr(uint8_t bar);
        bool set_bar_addr(uint8_t bar, uint64_t addr);
        bool has_caps();
        uint8_t cap_ptr();
        uint8_t* get_cap(uint8_t cap_id);
        uint32_t* cmd_reg();

    protected:
        uint16_t _bus;
        uint8_t _device;
        uint8_t _function;
        uint8_t* _cfg_space;
        uint16_t _vendor_id;
        uint16_t _device_id;

};

class PCIe {
    public:
        static void init(uint64_t pci_base);
        static void enumerate();
        static pci_device get(uint16_t target_vendor_id, uint16_t target_device_id);

    private:
        static bool _init;
        static uint64_t _pci_base;
};

#endif /* PCIE_HPP */
