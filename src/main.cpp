#include <serial.hpp>
#include <stddef.h>

void* strcpy(char *dst, const char *src) {
    size_t i = 0;
    while (src[i]) {
        dst[i] = src[i];
        i++;
    }
    return dst;
}

extern "C" void kmain() {
    Serial::puts("pikaboot> ");

    while (true) {
        Serial::puts("pikaboot> ");
        Serial::putchar(Serial::getchar());
        Serial::putchar('\n');
    }
}
