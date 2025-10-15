#pragma once

#include <stdint.h>

typedef uint32_t Xorshift32;
typedef uint64_t Xorshift64;

static inline Xorshift32 Xorshift32_New() { return (Xorshift32)314159265; }
static inline Xorshift64 Xorshift64_New() { return (Xorshift64)88172645463325252ull; }

static inline uint32_t Xorshift32_Next(Xorshift32* gen) {
	*gen ^= *gen << 13;
	*gen ^= *gen >> 17;
	*gen ^= *gen << 5;
	return *gen;
}

static inline uint64_t Xorshift64_Next(Xorshift64* gen) {
	*gen ^= *gen << 13;
	*gen ^= *gen >> 7;
	*gen ^= *gen << 17;
	return *gen;
}