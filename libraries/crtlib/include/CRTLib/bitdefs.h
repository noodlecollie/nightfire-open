#pragma once

#define BIT(n) (1U << (n))
#define SetBits(iBitVector, bits) ((iBitVector) = (iBitVector) | (bits))
#define ClearBits(iBitVector, bits) ((iBitVector) = (iBitVector) & ~(bits))
#define FBitSet(iBitVector, bit) ((iBitVector) & (bit))
