#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * Allocates a 1-dimensional array of size n, where each element has a size of typeSize.
 * 
 * @param n The number of elements in the array.
 * @param typeSize The size of each element in the array.
 * @return A pointer to the allocated 1-dimensional array.
 * @note The returned array must be freed using free1DArray() function to avoid memory leaks.
 */
void *allocate1DArray(uint32_t n, uint32_t typeSize);

/**
 * Allocates a 2-dimensional array of size n x m, where each element has a size of typeSize.
 * 
 * @param n The number of rows in the array.
 * @param m The number of columns in the array.
 * @param typeSize The size of each element in the array.
 * @return A pointer to the allocated 2-dimensional array.
 * @note The returned array must be freed using free2DArray() function to avoid memory leaks.
 */
void **allocate2DArray(uint32_t n, uint32_t m, uint32_t typeSize);

/**
 * Frees a 1D array of uint64_t.
 * 
 * @param array The 1D array to be freed.
 * @note Cast pointer to void * before passing it to the function
 */
void free1DArray(void *array);

/**
 * Frees a 2D array of uint64_t.
 * 
 * @param array The 2D array to be freed.
 * @param n The number of rows in the array.
 * @note Cast pointer to void ** before passing it to the function
 */
void free2DArray(void **array, uint32_t n);

#endif