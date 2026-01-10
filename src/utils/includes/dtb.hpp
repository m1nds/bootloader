#ifndef DTB_HPP
#define DTB_HPP

#include <stdint.h>

constexpr uint32_t DTB_VERSION_MIN = 1;
constexpr uint32_t DTB_VERSION_MAX = 17;
constexpr uint32_t FDT_MAGIC  = 0xd00dfeed;
constexpr uint32_t FDT_BEGIN_NODE = 0x1;
constexpr uint32_t FDT_END_NODE = 0x2;
constexpr uint32_t FDT_PROP = 0x3;
constexpr uint32_t FDT_NOP = 0x4;
constexpr uint32_t FDT_END = 0x9;

struct fdt_header {
    uint32_t magic;
    uint32_t totalsize;
    uint32_t off_dt_struct;
    uint32_t off_dt_strings;
    uint32_t off_mem_rsvmap;
    uint32_t version;
    uint32_t last_comp_version;
    uint32_t boot_cpuid_phys;
    uint32_t size_dt_strings;
    uint32_t size_dt_struct;
};

class DTB {
    public:
        void dump_header();
        void verify_header();
        void dump_nodes();
    private:
        uint8_t* struct_block();
        uint8_t* strings_block();
        struct fdt_header hdr;
};

#endif /* DTB_HPP */
