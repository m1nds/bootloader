#include <serial.hpp>
#include <pcie.hpp>
#include <boot.hpp>
#include <string.hpp>
#include <dtb.hpp>

#include <stddef.h>
#include <stdint.h>

extern uint64_t TEXT_START[];

constexpr uint64_t PFLASH_BIN_OFFSET = 50 * 1024 * 1024;

static char command_buffer[256];

void help() {
    Serial::puts("my_pikaboot - by m1nds\n");
    Serial::puts("   - boot : Boots the image using Linux Boot Protocol.\n");
    Serial::puts("   - dtb  : Dumps the Device Tree Blob.\n");
    Serial::puts("   - help : Display all the commands available.\n");
}

extern "C" void kmain(void* device_tree) {
    DTB* dtb = reinterpret_cast<DTB*>(device_tree);

    const char* nodes[] = {"pl011", "pcie"};
    size_t nodesLen[] = {strlen("pl011"), strlen("pcie")};
    uint64_t nodesValues[2] = {0, 0};

    dtb->parse_nodes(2, nodes, nodesLen, nodesValues);

    Serial::init(nodesValues[0]);
    PCIe::init(nodesValues[1]);

    void* kernel_image_addr = reinterpret_cast<void*>((uint64_t) &TEXT_START + PFLASH_BIN_OFFSET);

    Serial::puts("my_pikaboot - by m1nds\n\n");
    PCIe::enumerate();
    Serial::puts("=== Starting the bootloader... ===\n");

    while (true) {
        Serial::puts("my_pikaboot> ");

        int read = Serial::get_command_line(command_buffer, 256);
        if (read > 0 && strcmp(command_buffer, "boot") == 0) {
            Kernel kernel = Kernel(kernel_image_addr, device_tree);
            kernel.boot();
        } else if (read > 0 && strcmp(command_buffer, "dtb") == 0) {
            dtb->dump_header();
            dtb->dump_nodes();
        } else if (read > 0 && strcmp(command_buffer, "help") == 0) {
            help();
        } else {
            Serial::puts("Invalid command!\n");
            help();
        }
    }
}
