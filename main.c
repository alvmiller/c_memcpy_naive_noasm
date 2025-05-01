#include <stdio.h>
#include <stdlib.h>

void *memcpy_naive(void *restrict dst, const void *restrict src, size_t size)
{
    const size_t BASIC_SIZE = sizeof(size_t);

    if ((dst == NULL || src == NULL) && size != 0) {
        printf("\tError: Bad args\n");
        return NULL;
    }

    if (size == 0 || dst == NULL || src == NULL) {
        return dst;
    }
    
    if ((size_t)dst % BASIC_SIZE != 0 || (size_t)src % BASIC_SIZE != 0) {
        printf("\tError: Not aligned\n");
        return NULL;
    }

    size_t tmp = 0;
    if (__builtin_add_overflow((size_t)src, size, &tmp)) {
        printf("\tError: Overflow\n");
        return NULL;
    }
    if (__builtin_add_overflow((size_t)dst, size, &tmp)) {
        printf("\tError: Overflow\n");
        return NULL;
    }

    size_t *wdst = dst;
    const size_t *wsrc = src;
    for(size_t i = 0, m = size / BASIC_SIZE; i < m; ++i) {
        *(wdst++) = *(wsrc++);
    }
 
    unsigned char *cdst = (unsigned char *)wdst;
    unsigned char *csrc = (unsigned char *)wsrc;
    for(size_t i = 0, m = size % BASIC_SIZE; i < m; ++i) {
        *(cdst++) = *(csrc++);
    }

    return dst;
}

int main()
{
    size_t src[10] __attribute__((aligned(sizeof(size_t)))) = 
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    size_t *dst = aligned_alloc(sizeof(size_t), 10 * sizeof *dst);

    void *ret = memcpy_naive(dst, src, 10 * sizeof(size_t));
    if (ret != NULL) {
        for(size_t i = 0; i < 10; ++i) {
            printf("%zu ", dst[i]);
        }
        printf("\n");
    }

    free (dst);
    return 0;
}
