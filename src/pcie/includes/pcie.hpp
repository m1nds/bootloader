#ifndef PCIE_HPP
#define PCIE_HPP

#include <cstdint>

class PCIE {
    public:
        static void init(uint64_t pci_base);
        static void enumerate();

    private:
        static bool _init;
        static uint64_t _pci_base;
};

#endif /* PCIE_HPP */
