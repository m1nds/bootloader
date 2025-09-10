#include <serial.hpp>
#include <stddef.h>

extern "C" void kmain() {
    Serial::puts("bootloader - v0.1\n");
    Serial::kprintf("Printing deadbeef: %x\n", 0xdeadbeef);
}
