#include <serial.hpp>
#include <boot.hpp>
#include <string.hpp>
#include <dtb.hpp>

#include <stddef.h>

extern uint64_t TEXT_START[];

constexpr uint64_t PFLASH_BIN_OFFSET = 50 * 1024 * 1024;

static char command_buffer[256];

void help() {
    Serial::puts("my_pikaboot - by m1nds\n");
    Serial::puts("   - boot : boots the image using Linux Boot Protocol.\n");
    Serial::puts("   - help : display all the commands available.\n");
}

extern "C" void kmain(void* device_tree) {
    DTB* dtb = reinterpret_cast<DTB*>(device_tree);
    dtb->dump_header();

    void* kernel_image_addr = reinterpret_cast<void*>((uint64_t) &TEXT_START + PFLASH_BIN_OFFSET);

    Serial::puts("my_pikaboot - by m1nds\n\n");
    Serial::puts("=== Starting the bootloader... ===\n");

    while (true) {
        Serial::puts("my_pikaboot> ");

        int read = Serial::get_command_line(command_buffer, 256);
        if (read > 0 && strcmp(command_buffer, "boot") == 0) {
            Kernel kernel = Kernel(kernel_image_addr, device_tree);
            kernel.boot();
        } else if (read > 0 && strcmp(command_buffer, "help") == 0) {
            help();
        } else {
            Serial::puts("Invalid command!\n");
            help();
        }
    }
}
