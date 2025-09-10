#include <serial.hpp>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

uint32_t MMIO_BASE = 0x09000000;
uint32_t UART0_DR = 0x0;
uint32_t UART0_FR = 0x18;

unsigned char Serial::stack[256] = { 0 };
size_t Serial::stack_idx = 0;

void Serial::mmio_write(uint32_t reg, uint32_t data) {
    *(reinterpret_cast<volatile uint32_t*>(MMIO_BASE + reg)) = data;
}

uint32_t Serial::mmio_read(uint32_t reg) {
    return *(reinterpret_cast<volatile uint32_t*>(MMIO_BASE + reg));
}

char Serial::getchar() {
    while (Serial::mmio_read(UART0_FR) & (1 << 4));
    return Serial::mmio_read(UART0_DR);
}

void Serial::putchar(char c) {
    while (Serial::mmio_read(UART0_FR) & (1 << 5));
    Serial::mmio_write(UART0_DR, c);
}

void Serial::puts(const char* s) {
    size_t i = 0;
    while (s[i]) {
        Serial::putchar(s[i++]);
    }
}

void Serial::print_hex(uint64_t value, uint32_t bits)
{
    uint32_t digits = 0;
    char hex[] = "0123456789ABCDEF";

    while (value)
    {
        Serial::stack[Serial::stack_idx++] = value % 16;
        digits++;
        value /= 16;
    }

    Serial::puts("0x");

    uint32_t len = bits / 4;
    for (uint8_t i = digits; i < len; i++) {
        Serial::putchar('0');
    }

    for (uint8_t i = 0; i < digits; i++)
    {
        Serial::putchar(hex[Serial::stack[--Serial::stack_idx]]);
    }
}

void Serial::print_dec(uint32_t value)
{
    uint32_t digits = 0;

    if (value == 0) {
        Serial::putchar('0');
        return;
    }

    while (value)
    {
        Serial::stack[Serial::stack_idx++] = value % 10;
        digits++;
        value /= 10;
    }

    for (uint8_t i = 0; i < digits; i++)
    {
        Serial::putchar('0' + Serial::stack[--Serial::stack_idx]);
    }
}

void Serial::kprintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    size_t i = 0;
    while (fmt[i]) {
        if (fmt[i] != '%') {
            Serial::putchar(fmt[i]);
        } else if (fmt[i + 1] == '%') {
            Serial::putchar('%');
            i++;
        } else if (fmt[i + 1] == 'c') {
            Serial::putchar(va_arg(args, int));
            i++;
        } else if (fmt[i + 1] == 's') {
            Serial::puts(va_arg(args, const char*));
            i++;
        } else if (fmt[i + 1] == 'd') {
            Serial::print_dec(va_arg(args, uint32_t));
            i++;
        } else if (fmt[i + 1] == 'x') {
            Serial::print_hex(va_arg(args, uint32_t), 32);
            i++;
        } else if (fmt[i + 1] == 'X') {
            Serial::print_hex(va_arg(args, uint64_t), 64);
            i++;
        } else {
            Serial::puts("[WARNING] Invalid printing fmt!\n");
        }
        i++;
    }

    va_end(args);
}
