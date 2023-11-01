#ifndef BITMAP_H
#define BITMAP_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "utils.h"

/**
 * @brief A struct representing a bitmap.
 * 
 * This struct contains the number of relevant bits, 
 * the number of words, and a pointer to the bitmap array.
 */
typedef struct Bitmap {
    int32_t nBits;  /**< The number of relevant bits in the bitmap. */
    int32_t nWords; /**< The number of words in the bitmap. */
    uint64_t *map; /**< A pointer to the bitmap array. */
} bitmap;

/**
 * Initializes a new bitmap with the given number of relevant bits.
 * 
 * @param nBits The number of relevant bits in the bitmap.
 * @return A pointer to the newly allocated bitmap.
 */
bitmap *initBitmap(int32_t nBits);

/**
 * Frees the memory allocated for a bitmap.
 * 
 * @param myBitmap The bitmap to be freed.
 */
void freeBitmap(bitmap *myBitmap);

/**
 * Returns the word index in a bitmap with 64-bit words.
 * 
 * @param i The index of the bit to retrieve.
 * @return The word index.
 */
int32_t getWordIdx(int32_t i);

/**
 * Returns the bit index in a bitmap with 64-bit words.
 * 
 * @param i The index of the bit to retrieve.
 * @return The bit index.
 */
char getBitIdx(int32_t i);

/**
 * Toggles a bit within a bitmap.
 *
 * @param myBitmap The bitmap to modify.
 * @param i The index of the bit to toggle.
 */
void switchBit(bitmap *myBitmap, int32_t i);

/**
 * Reads the value of a bit in a bitmap.
 *
 * @param myBitmap The bitmap to read from.
 * @param i The index of the bit to read.
 * @return The value of the bit (0 or 1).
 */
char getBit(bitmap *myBitmap, int32_t i);

/**
 * Sequentially reads bits in a bitmap from bit 0 to nBits-1
 * 
 * @param myBitmap The bitmap to read from.
 * @return A bit value.
 */
char readBitmap(bitmap *myBitmap);

/**
 * Counts the number of set bits in a 64-bit word using Brian Kernighan's algorithm.
 * 
 * @param word The 64-bit word to count the set bits in.
 * @return The number of set bits in the word.
 */
char countSetBitsInWord(uint64_t word);

/**
 * Counts the number of set bits in a bitmap.
 * 
 * @param myBitmap Pointer to the bitmap to count set bits in.
 * @return The number of set bits in the bitmap.
 */
int32_t countSetBits(bitmap *myBitmap);

/**
 * Prints bits from 0 up to nBits-1.
 *
 * @param myBitmap The bitmap to print.
 */
void printBitmap(bitmap *myBitmap);

#endif