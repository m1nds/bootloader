#include <boot.hpp>
#include <serial.hpp>

typedef void (*boot_func)(void*, uint64_t, uint64_t, uint64_t);

Kernel::Kernel(void* kernel_addr, void* device_tree_addr) {
    this->kh = reinterpret_cast<KernelHeader*>(kernel_addr);
    this->dt = device_tree_addr;
}

void Kernel::boot() const {
    Serial::kprintf("=== Kernel Header found at %X ===\n", (uint64_t) this->kh);
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

    Serial::puts("=== Jumping to kernel code... ===\n");

    // kernel(&device_tree: x0, 0: x1, 0: x2, 0: x3);
    reinterpret_cast<boot_func>(this->kh)(this->dt, 0, 0, 0);
}
