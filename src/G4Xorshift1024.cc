#include "G4Xorshift1024.hh"

#include <iostream>
#include <cmath>

G4Xorshift1024::G4Xorshift1024(uint64_t seed)
{
    state64 = seed;
    for(int i=0; i < 16; i++)
    {
        state[i] = shoot64();
    }
    p = 0;
    maxVal = 1.0/static_cast<double>(UINT64_MAX);
}

G4Xorshift1024::G4Xorshift1024()
{
    state64 = 1024;
    for(int i=0; i < 16; i++)
    {
        state[i] = shoot64();
    }
    p = 0;
    maxVal = 1.0/static_cast<double>(UINT64_MAX);
}

G4Xorshift1024::~G4Xorshift1024()
{}

void G4Xorshift1024::seed(uint64_t seed)
{
    state64 = seed;
    for(int i=0; i < 16; i++)
    {
        state[i] = shoot64();
    }
    p = 0;
}



// XORshift* 1024 generator - the one which should be used for generating random numbers
uint64_t G4Xorshift1024::shoot()
{
    uint64_t s0 = state[p];
    uint64_t s1 = state[p = (p + 1) & 15 ];
    s1 ^= s1 << 31;
    s1 ^= s1 >> 11;
    s0 ^= s0 >> 30;
    return (state[p] = s0 ^ s1) * 1181783497276652981LL;
}


double G4Xorshift1024::flat()
{
    return static_cast<double>(shoot())*maxVal;
}

// Xorshift 64 generator - used to bootstrap a single seed into the array for the 1024 generator

uint64_t G4Xorshift1024::shoot64() 
{
	state64 ^= state64 >> 12; // a
    state64 ^= state64 << 25; // b
	state64 ^= state64 >> 27; // c
	return state64 * 2685821657736338717LL;
}