#include "matrix.h"

int main()
{
    size_t size[6] = {4, 16, 128, 1000, 8000, 64000};
    int range = 6;
    for (int i = 0; i < range - 2; i++)
    {
        printf("matmul_plain() size = %ld * %ld, time = %.9f s\n", size[i], size[i], testTime(size[i], 0));
    }
    for (int i = 0; i < range - 1; i++)
    {
        printf("matmul_improved_SIMD() size = %ld * %ld, time = %.9f s\n", size[i], size[i], testTime(size[i], 1));
    }
    for (int i = 0; i < range; i++)
    {
        printf("matmul_improved_OPENMP() size = %ld * %ld, time = %.9f s\n", size[i], size[i], testTime(size[i], 2));
    }
    for (int i = 0; i < range; i++)
    {
        printf("testOpenBlas() size = %ld * %ld, time = %.9f s\n", size[i], size[i], testTime(size[i], 3));
    }
}