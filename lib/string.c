#include "jabulos.h"

void* memset(void* destination, int value, size_t count) {
    u8* bytes = (u8*)destination;
    while (count-- > 0) {
        *bytes++ = (u8)value;
    }
    return destination;
}

void* memcpy(void* destination, const void* source, size_t count) {
    u8* dst = (u8*)destination;
    const u8* src = (const u8*)source;
    while (count-- > 0) {
        *dst++ = *src++;
    }
    return destination;
}

void* memmove(void* destination, const void* source, size_t count) {
    u8* dst = (u8*)destination;
    const u8* src = (const u8*)source;

    if (dst == src || count == 0) {
        return destination;
    }

    if (dst < src) {
        while (count-- > 0) {
            *dst++ = *src++;
        }
    } else {
        dst += count;
        src += count;
        while (count-- > 0) {
            *--dst = *--src;
        }
    }

    return destination;
}

int memcmp(const void* left, const void* right, size_t count) {
    const u8* lhs = (const u8*)left;
    const u8* rhs = (const u8*)right;

    while (count-- > 0) {
        if (*lhs != *rhs) {
            return (int)*lhs - (int)*rhs;
        }
        ++lhs;
        ++rhs;
    }

    return 0;
}

size_t strlen(const char* string) {
    size_t length = 0;
    while (string[length] != '\0') {
        ++length;
    }
    return length;
}

char* strcpy(char* destination, const char* source) {
    char* out = destination;
    while ((*destination++ = *source++) != '\0') {
    }
    return out;
}

int strcmp(const char* left, const char* right) {
    while (*left != '\0' && *left == *right) {
        ++left;
        ++right;
    }
    return (int)(unsigned char)*left - (int)(unsigned char)*right;
}

int strncmp(const char* left, const char* right, size_t count) {
    while (count > 0 && *left != '\0' && *left == *right) {
        ++left;
        ++right;
        --count;
    }

    if (count == 0) {
        return 0;
    }

    return (int)(unsigned char)*left - (int)(unsigned char)*right;
}
