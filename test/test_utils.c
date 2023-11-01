#include "../src/utils.h"

int main()
{
    // Test allocate1DArray
    uint32_t n = 5;
    uint32_t typeSize = sizeof(int);
    int *array1D = (int *)allocate1DArray(n, typeSize);
    for (uint32_t i = 0; i < n; i++) {
        array1D[i] = i;
    }
    for (uint32_t i = 0; i < n; i++) {
        printf("%d ", array1D[i]);
    }
    printf("\n");
    free1DArray((void *)array1D);

    printf("\n");

    // Test allocate2DArray
    uint32_t rows = 3;
    uint32_t cols = 4;
    int **array2D = (int **)allocate2DArray(rows, cols, typeSize);
    for (uint32_t i = 0; i < rows; i++) {
        for (uint32_t j = 0; j < cols; j++) {
            array2D[i][j] = i * j;
        }
    }
    for (uint32_t i = 0; i < rows; i++) {
        for (uint32_t j = 0; j < cols; j++) {
            printf("%d ", array2D[i][j]);
        }
        printf("\n");
    }
    free2DArray((void **)array2D, rows);

    return 0;
}