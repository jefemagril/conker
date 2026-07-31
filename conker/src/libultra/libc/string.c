#include <string.h>

void *memcpy(void *s1, const void *s2, size_t n)
{
    unsigned char *su1 = (unsigned char *)s1;
    const unsigned char *su2 = (const unsigned char *)s2;

    while (n > 0) {
        *su1++ = *su2++;
        n--;
    }

    return (void *)s1;
}

size_t strlen(const char *s)
{
    const unsigned char *sc = (const unsigned char *)s;

    while (*sc) {
        sc++;
    }

    return sc - (const unsigned char *)s;
}

char *strchr(const char *s, s32 c)
{
    const unsigned char ch = c;

    while (*(unsigned char *)s != ch) {
        if (*(unsigned char *)s == 0) {
            return NULL;
        }

        s++;
    }

    return (char *)s;
}
