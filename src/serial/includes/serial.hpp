#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <stdint.h>
#include <stddef.h>

constexpr uint32_t UART0_DR = 0x0;
constexpr uint32_t UART0_FR = 0x18;
constexpr uint32_t STACK_SIZE = 0x1000;

class Serial {
    public:
        static void init(uint64_t base);
        static char getchar(void);
        static int get_command_line(char *buf, int maxlen);
        static void putchar(char c);
        static void puts(const char* s);
        static void kprintf(const char* fmt, ...);
        static void print_dec(uint32_t value);
        static void print_hex(uint64_t value, uint32_t bits);

    private:
        static bool _init;
        static uint64_t PL011_BASE;
        static void mmio_write(uint32_t reg, uint32_t data);
        static uint32_t mmio_read(uint32_t reg);

        static unsigned char stack[STACK_SIZE];
        static size_t stack_idx;
};

#endif /* SERIAL_HPP */
