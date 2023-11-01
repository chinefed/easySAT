#include "bitmap.h"

bitmap *initBitmap(int32_t nBits)
{
    /* Allocate a new bitmap. */
    bitmap *newBitmap;
    newBitmap = (bitmap *)malloc(sizeof(bitmap));
    newBitmap->nBits = nBits;
    newBitmap->nWords = getWordIdx(nBits-1)+1;
    newBitmap->map = (uint64_t *)allocate1DArray(newBitmap->nWords, sizeof(uint64_t));
    return newBitmap;
}

void freeBitmap(bitmap *myBitmap)
{
    /* Free a bitmap. */
    free1DArray((void *)(myBitmap->map));
    free(myBitmap);
}

int32_t getWordIdx(int32_t i)
{
    /*  Returns the word index in a bitmap with 64-bit words. */
    int32_t wordIdx = i >> 6;

    return wordIdx;
}

char getBitIdx(int32_t i) 
{
    /*  Returns the bit index in a bitmap with 64-bit words. */
    char bitIdx = i % 64;

    return bitIdx;
}

void switchBit(bitmap *myBitmap, int32_t i)
{
    /* Toggles a bit within a bitmap. */
    int32_t wordIdx = getWordIdx(i);
    char bitIdx = getBitIdx(i);
    (myBitmap->map)[wordIdx] ^= (uint64_t)1 << bitIdx;
}

char getBit(bitmap *myBitmap, int32_t i)
{
    /* Reads the value of a bit in a bitmap. */
    int32_t wordIdx = getWordIdx(i);
    char bitIdx = getBitIdx(i);
    uint64_t word = (myBitmap->map)[wordIdx];
    
    return (word >> bitIdx) & 1;
}

char readBitmap(bitmap *myBitmap)
{
    /* Sequentially reads bits in a bitmap from bit 0 to limit-1 */
    static uint64_t currentWord = 0;
    static int32_t call = 0;
    static int32_t word = 0;
    static char bit = 0;

    if (bit == 0) {
         currentWord = (myBitmap->map)[word];
    }

    char outBit = currentWord & 1;

    call++;
    if (call == myBitmap->nBits) {
        currentWord = 0;
        call = 0;
        word = 0;
        bit = 0;
        return outBit;
    }

    if (bit == 63) {
        word++;
        bit = 0;
    } else {
        currentWord >>= 1;
        bit++;
    }

    return outBit;
}

char countSetBitsInWord(uint64_t word)
{
    /* Brian Kernighan's algorithm to count 
    set bits in a number. */
    char nSetBits = 0;
    while (word) {
        word &= word - 1;
        nSetBits++;
    }

    return nSetBits;
}

int32_t countSetBits(bitmap *myBitmap)
{
    /* Counts the number of set bits in a bitmap. */
    int32_t nSetBits = 0;
    for (int64_t w = 0; w < myBitmap->nWords; w++) {
        nSetBits += countSetBitsInWord((myBitmap->map)[w]);
    }

    return nSetBits;
}

void printBitmap(bitmap *myBitmap)
{
    /* Prints the frist N bits of a bitmap up to the limit. */
    for (int32_t i = 0; i < myBitmap->nBits; i++) {
        char bitValue = readBitmap(myBitmap);
        printf("Item %" PRIu32 " --> %d\n", i, bitValue);
    }
}