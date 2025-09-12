#ifndef BOOT_HPP
#define BOOT_HPP

#include <stdint.h>

struct KernelHeader {
    uint32_t code0;
    uint32_t code1;
    uint64_t text_offset;
    uint64_t image_size;
    uint64_t flags;
    uint64_t reserved2;
    uint64_t reserved3;
    uint64_t reserved4;
    uint32_t magic;
    uint32_t reserved5;
} __attribute__((packed));

class Kernel {
    public:
        Kernel(void* kernel_addr, void* device_tree_addr);
        void boot() const;

    private:
        struct KernelHeader* kh;
        void* dt;
} __attribute__((packed));

#endif /* BOOT_HPP */
