#ifndef RNG_H
#define RNG_H

typedef unsigned int uint32;


void rng_init(uint32 seed);
uint32 rng_rand();

#endif