#include <serial.hpp>
#include <boot.hpp>
#include <string.hpp>

#include <stddef.h>

extern uint64_t TEXT_START[];

constexpr uint64_t PFLASH_BIN_OFFSET = 50 * 1024 * 1024;

extern "C" void kmain(void* device_tree) {
    Serial::puts("my_pikaboot - by m1nds\n\n");


    Serial::puts("=== Starting the bootloader... ===\n");
    void* kernel_image_addr = reinterpret_cast<void*>((uint64_t) &TEXT_START + PFLASH_BIN_OFFSET);
    Kernel kernel = Kernel(kernel_image_addr, device_tree);
    kernel.boot();
}
