#ifndef STRING_HPP
#define STRING_HPP

#include <stddef.h>

void *memcpy(void* dest, const void* src, size_t n);
char *strcpy(char* dest, const char* src);
void* memmove(void* dstptr, const void* srcptr, size_t size);

#endif /* STRING_HPP */
