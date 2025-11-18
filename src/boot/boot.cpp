#include <boot.hpp>
#include <serial.hpp>
#include <string.hpp>
#include <crc.hpp>

typedef void (*boot_func)(void*, uint64_t, uint64_t, uint64_t);

Kernel::Kernel(void* kernel_addr, void* device_tree_addr) {
    this->kh = reinterpret_cast<KernelHeader*>(kernel_addr);
    this->dt = device_tree_addr;
}

static void* align(void* kh) {
    uint64_t int_kh = reinterpret_cast<uint64_t>(kh);
    int_kh &= ~(0x1FFFFF);
    int_kh += 0x200000;
    return reinterpret_cast<void*>(int_kh);
}

void Kernel::boot() const {
    Serial::kprintf("=== Kernel Header found at %X ===\n", (uint64_t) this->kh);
    uint32_t crc = CRC32::digest(this->kh, IMAGE_SIZE);

    if (crc != IMAGE_CRC32) {
        Serial::puts("   Invalid CRC32 Checksum!\n");
        Serial::kprintf("   Got %x, expected %x\n", crc, IMAGE_CRC32);
        return;
    }

    Serial::kprintf("=== CRC32 Checksum %x ===\n", crc);
    Serial::kprintf("   code0 = %x\n", this->kh->code0);
    Serial::kprintf("   code1 = %x\n", this->kh->code1);
    Serial::kprintf("   text_offset = %X\n", this->kh->text_offset);
    Serial::kprintf("   image_size = %X\n", this->kh->image_size);
    Serial::kprintf("   flags = %X\n", this->kh->flags);
    Serial::kprintf("   reserved2 = %X\n", this->kh->reserved2);
    Serial::kprintf("   reserved3 = %X\n", this->kh->reserved3);
    Serial::kprintf("   reserved4 = %X\n", this->kh->reserved4);
    Serial::kprintf("   magic = %x\n", this->kh->magic);
    Serial::kprintf("   reserved5 = %x\n", this->kh->reserved5);

    void* aligned_kh = align(this->kh);
    memmove(aligned_kh, this->kh, this->kh->image_size);

    Serial::kprintf("=== Aligning image to %X ===\n", reinterpret_cast<uint64_t>(aligned_kh));

    Serial::puts("=== Jumping to kernel code... ===\n");

    // kernel(&device_tree: x0, 0: x1, 0: x2, 0: x3);
    reinterpret_cast<boot_func>(aligned_kh)(this->dt, 0, 0, 0);
}
