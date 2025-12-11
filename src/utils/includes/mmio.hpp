#ifndef MMIO_HPP
#define MMIO_HPP

#include <stdint.h>

constexpr uint32_t PL011_BASE = 0x09000000;

class MMIO {
    public:
        static void mmio_write(uint32_t reg, uint32_t data);
        static uint32_t mmio_read(uint32_t reg);
};

#endif /* MMIO_HPP */
