#include <dtb.hpp>
#include <serial.hpp>
#include <string.hpp>

uint8_t* DTB::struct_block() {
    return (uint8_t*)this + __builtin_bswap32(hdr.off_dt_struct);
}

uint8_t* DTB::strings_block() {
    return (uint8_t*)this + __builtin_bswap32(hdr.off_dt_strings);
}

void DTB::dump_header() {
    Serial::puts("=== Device Tree Blob ===\n");
    DTB::verify_header();
    Serial::kprintf("  magic = %x\n", __builtin_bswap32(this->hdr.magic));
    Serial::kprintf("  totalsize = %x\n", __builtin_bswap32(this->hdr.totalsize));
    Serial::kprintf("  off_dt_struct = %x\n", __builtin_bswap32(this->hdr.off_dt_struct));
    Serial::kprintf("  off_dt_strings = %x\n", __builtin_bswap32(this->hdr.off_dt_strings));
    Serial::kprintf("  off_mem_rsvmap = %x\n", __builtin_bswap32(this->hdr.off_mem_rsvmap));
    Serial::kprintf("  version = %d\n", __builtin_bswap32(this->hdr.version));
    Serial::kprintf("  last_comp_version = %d\n", __builtin_bswap32(this->hdr.last_comp_version));
    Serial::kprintf("  boot_cpuid_phys = %x\n", __builtin_bswap32(this->hdr.boot_cpuid_phys));
    Serial::kprintf("  size_dt_strings = %x\n", __builtin_bswap32(this->hdr.size_dt_strings));
    Serial::kprintf("  size_dt_struct = %x\n", __builtin_bswap32(this->hdr.size_dt_struct));
    Serial::puts("\n");
}

void DTB::verify_header() {
    uint32_t magic = __builtin_bswap32(this->hdr.magic);
    if (magic != FDT_MAGIC)  {
        Serial::puts("  WARNING: magic value is invalid!\n");
    }

    uint32_t version = __builtin_bswap32(this->hdr.version);
    uint32_t last_comp_version = __builtin_bswap32(this->hdr.last_comp_version);

    if (version > DTB_VERSION_MAX || version < DTB_VERSION_MIN || last_comp_version > version) {
        Serial::puts("  WARNING: DTB Version is invalid!\n");
    }
}

void DTB::dump_nodes() {
    uint32_t* ptr = reinterpret_cast<uint32_t*>(this->struct_block());
    uint8_t* strings = strings_block();

    int spaces = 0;

    while (true) {
        uint32_t tag = __builtin_bswap32(*(ptr++));
        if (tag == FDT_BEGIN_NODE) {
            const char* name = (char*)ptr;
            size_t name_len = strlen(name);

            for (int i = 0; i < spaces; i++) {
                Serial::putchar(' ');
            }

            if (name_len > 0) {
                Serial::kprintf("FDT_BEGIN_NODE: %s\n", name);
            }

            size_t skip_len = (name_len) / 4 + (name_len % 4 != 0);
            ptr += skip_len;
            spaces += 2;

        } else if (tag == FDT_PROP) {
            uint32_t len = __builtin_bswap32(*(ptr++));
            uint32_t nameoff = __builtin_bswap32(*(ptr++));
            const char* prop_name = (char*)(strings + nameoff);

            for (int i = 0; i < spaces; i++) {
                Serial::putchar(' ');
            }

            if (!strcmp(prop_name, "reg")) {
                uint64_t offset = (((uint64_t) __builtin_bswap32(ptr[0])) << 32) | ((uint64_t) __builtin_bswap32(ptr[1]));
                uint64_t size = (((uint64_t) __builtin_bswap32(ptr[2])) << 32) | ((uint64_t) __builtin_bswap32(ptr[3]));
                (void) size;
                Serial::kprintf("FDT_PROP: reg = %X [%X]\n", offset, size);
            } else {
                Serial::kprintf("FDT_PROP: %s\n", prop_name);
            }

            ptr += (len / 4) + (len % 4 != 0);

        } else if (tag == FDT_NOP) {
            for (int i = 0; i < spaces; i++) {
                Serial::putchar(' ');
            }

            Serial::kprintf("FDT_NOP\n");
        } else if (tag == FDT_END_NODE) {
            spaces -= 2;

            for (int i = 0; i < spaces; i++) {
                Serial::putchar(' ');
            }

            Serial::kprintf("FDT_END_NODE\n");
        } else if (tag == FDT_END) {

            for (int i = 0; i < spaces; i++) {
                Serial::putchar(' ');
            }

            Serial::kprintf("FDT_END\n");
            break;
        }
    }
}
