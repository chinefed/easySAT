#include <assert.h>
#include "../src/bitmap.h"

int main()
{
    // Test getWordIdx
    assert(getWordIdx(0) == 0);
    assert(getWordIdx(63) == 0);
    assert(getWordIdx(64) == 1);
    assert(getWordIdx(127) == 1);
    assert(getWordIdx(128) == 2);

    // Test getBitIdx
    assert(getBitIdx(0) == 0);
    assert(getBitIdx(63) == 63);
    assert(getBitIdx(64) == 0);
    assert(getBitIdx(127) == 63);
    assert(getBitIdx(128) == 0);

    // Test switchBit
    bitmap *myBitmap;
    myBitmap = initBitmap(129);
    switchBit(myBitmap, 0);
    assert((myBitmap->map)[0] == 1);
    switchBit(myBitmap, 63);
    assert((myBitmap->map)[0] == 0x8000000000000001);
    switchBit(myBitmap, 64);
    assert((myBitmap->map)[1] == 1);
    switchBit(myBitmap, 64);
    assert((myBitmap->map)[1] == 0);
    switchBit(myBitmap, 127);
    assert((myBitmap->map)[1] == 0x8000000000000000);
    switchBit(myBitmap, 128);
    assert((myBitmap->map)[2] == 1);

    // Test getBit
    assert(getBit(myBitmap, 0) == 1);
    assert(getBit(myBitmap, 63) == 1);
    assert(getBit(myBitmap, 64) == 0);
    assert(getBit(myBitmap, 127) == 1);
    assert(getBit(myBitmap, 128) == 1);

    // Test readBitmap and printBitmap
    printBitmap(myBitmap);

    // Test countSetBits
    assert(countSetBits(myBitmap) == 4);

    return 0;
}