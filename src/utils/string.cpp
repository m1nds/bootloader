#include <string.hpp>
#include <stddef.h>
#include <stdint.h>

void *memcpy(void* dest, const void* src, size_t n) {
    uint8_t* c_dest = reinterpret_cast<uint8_t*>(dest);
    const uint8_t* c_src = reinterpret_cast<const uint8_t*>(src);

    for (size_t i = 0; i < n; i++) {
        c_dest[i] = c_src[i];
    }

    return dest;
}

char *strcpy(char* dest, const char* src) {
    uint8_t* c_dest = reinterpret_cast<uint8_t*>(dest);
    const uint8_t* c_src = reinterpret_cast<const uint8_t*>(src);

    size_t i = 0;
    while (c_src[i]) {
        c_dest[i] = c_src[i];
        i++;
    }

    return dest;
}

int memcmp(const void* one, const void* two, size_t len) {
    const uint8_t* c_one = reinterpret_cast<const uint8_t*>(one);
    const uint8_t* c_two = reinterpret_cast<const uint8_t*>(two);


    for (size_t i = 0; i < len; i++) {
        if (c_one[i] != c_two[i]) {
            return c_one[i] - c_two[i];
        }
    }

    return 0;
}

int strcmp(const char* one, const char* two) {
    size_t i = 0;

    while (one[i] && two[i]) {
        if (one[i] != two[i]) {
            return one[i] - two[i];
        }
        i++;
    }
    return 0;
}

int strncmp(const char* one, const char* two, size_t len) {
    size_t i = 0;

    while (one[i] && two[i] && i < len) {
        if (one[i] != two[i]) {
            return one[i] - two[i];
        }
        i++;
    }
    return 0;
}

void* memmove(void* dstptr, const void* srcptr, size_t size) {
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;
    if (dst < src) {
            for (size_t i = 0; i < size; i++)
                    dst[i] = src[i];
    } else {
            for (size_t i = size; i != 0; i--)
                    dst[i-1] = src[i-1];
    }
    return dstptr;
}

size_t strlen(const char *s) {
    size_t len = 0;
    while (s[len] != '\x00') {
        len++;
    }
    return len;
}

