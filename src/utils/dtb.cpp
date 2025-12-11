#include <dtb.hpp>
#include <serial.hpp>

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
