#ifndef CRC_HPP
#define CRC_HPP

#include <stdint.h>
#include <stddef.h>

class CRC32 {
    public:
        static uint32_t digest(const void* ptr, size_t len);
    private:
        static void fill_lookup();
        static uint32_t lookup[256];
        static bool _init;

};

#endif /* CRC_HPP */
