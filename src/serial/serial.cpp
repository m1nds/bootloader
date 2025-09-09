#include <serial.hpp>
#include <stddef.h>
#include <stdint.h>

uint32_t MMIO_BASE = 0x09000000;
uint32_t UART0_DR = 0x0;
uint32_t UART0_FR = 0x18;

static inline void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t*)(MMIO_BASE + reg) = data;
}

static inline uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t*)(MMIO_BASE + reg);
}

char Serial::getchar() {
    while ( mmio_read(UART0_FR) & (1 << 4) ) { }
    return mmio_read(UART0_DR);
}

void Serial::putchar(char c) {
    while ( mmio_read(UART0_FR) & (1 << 5) ) { }
    mmio_write(UART0_DR, c);
}

void Serial::puts(const char* s) {
    size_t i = 0;
    while (s[i]) {
        putchar(s[i++]);
    }
}
