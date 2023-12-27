#include "utils.h"

void *allocate1DArray(uint32_t n, uint32_t typeSize)
{
    /* Allocates a 1-dimensional array of size n, where each element has a size of typeSize. */
    void *array = calloc(n, typeSize);
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    return array;
}

void **allocate2DArray(uint32_t n, uint32_t m, uint32_t typeSize) 
{
    /* Allocates a 2-dimensional array of size n x m, where each element has a size of typeSize. */
    void **array = (void **)calloc(m, sizeof(void *));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    for (uint32_t i = 0; i < n; i++) {
        array[i] = calloc(m, typeSize);
        if (array[i] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(1);
        }
    }

    return array;
}

void free1DArray(void *array)
{
    /* Frees a 1D array of uint64_t. */
    free(array);
}

void free2DArray(void **array, uint32_t n)
{
    /* Frees a 2D array of uint64_t. */
    for (uint32_t i = 0; i < n; i++) {
        free(array[i]);
    }
    free(array);
}

void updateStatusBar(int x, int tot)
{
    /* Updates a status bar. */
    int progress = 100 * (float)x / tot;
    int numHashes = STATUS_BAR_WIDTH * x / tot;
    printf("\033[2K\r");
    printf("%% satisfied clauses: [");
    for (int i = 0; i < STATUS_BAR_WIDTH; i++) {
        if (i < numHashes) {
            printf("#");
        }
        else {
            printf(" ");
        }
    }
    printf("] %d%%", progress);
    fflush(stdout);
}