#include <crc.hpp>
#include <stdint.h>
#include <stddef.h>

bool CRC32::_init = false;
uint32_t CRC32::lookup[256] = { 0 };

void CRC32::fill_lookup() {
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t c = i;
        for (int j = 0; j < 8; ++j) {
            c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
        }
        CRC32::lookup[i] = c;
    }
}

uint32_t CRC32::digest(const void* ptr, size_t len)
{
    if (!CRC32::_init) {
        CRC32::_init = true;
        CRC32::fill_lookup();
    }

    uint32_t crc = 0xffffffff;
    const uint8_t* ptr_c = reinterpret_cast<const uint8_t*>(ptr);

    while (len-- !=0) {
        crc = CRC32::lookup[((uint8_t) crc ^ *(ptr_c++))] ^ (crc >> 8);
    }

    return (~crc);
}
