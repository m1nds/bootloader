#include <serial.hpp>
#include <mmio.hpp>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

unsigned char Serial::stack[256] = { 0 };
size_t Serial::stack_idx = 0;

char Serial::getchar() {
    while (MMIO::mmio_read(PL011_BASE + UART0_FR) & (1 << 4));
    return MMIO::mmio_read(PL011_BASE + UART0_DR);
}

int Serial::get_command_line(char *buf, int maxlen) {
    int i = 0;
    while (true) {

        if (i >= maxlen) {
            break;
        }

        char c = Serial::getchar();
        if (c != '\n' && c != '\r' &&  (c < 0x20 || c > 0x7f)) {
            continue;
        }

        switch (c) {
            case '\r':
            case '\n':
                Serial::putchar('\n');
                buf[i] = '\0';
                return i;
            case '\b':
            case '\x7f':
                if (i > 0) {
                    Serial::putchar('\b');
                    Serial::putchar(' ');
                    Serial::putchar('\b');
                    i--;
                }
                continue;
            default:
                buf[i++] = c;
                Serial::putchar(c);
                break;
        }
    }
    return -1;
}


void Serial::putchar(char c) {
    while (MMIO::mmio_read(UART0_FR) & (1 << 5));
    MMIO::mmio_write(PL011_BASE + UART0_DR, c);
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
