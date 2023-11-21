#include "rng.h"

#define R(x, n) (x<<n | x>>(32-n))

// internal state of the RNG
static uint32 rngState[4];

// quarter round function from the ChaCha20 algorithm
static void quarterRound(uint32* a, uint32* b, uint32* c, uint32* d)
{
    *a += *b; *d ^= *a; *d = R(*d, 16);
    *c += *d; *b ^= *c; *b = R(*b, 12);
    *a += *b; *d ^= *a; *d = R(*d, 8);
    *c += *d; *b ^= *c; *b = R(*b, 7);
}
// copy the seed into the internal state
void rng_init(uint32 seed)
{
    rngState[0] = rngState[1] = rngState[2] = rngState[3] = seed;
}
// perform two quarterrounds and yield the last part of the internal state
uint32 rng_rand()
{
    quarterRound(rngState  , rngState+1, rngState+2, rngState+3);
    quarterRound(rngState  , rngState+1, rngState+2, rngState+3);
    return rngState[3];
}