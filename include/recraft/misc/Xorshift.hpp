#pragma once

typedef u32 Xorshift32;
typedef u64 Xorshift64;

static inline Xorshift32 Xorshift32_New() { return (Xorshift32)314159265; }
static inline Xorshift64 Xorshift64_New() { return (Xorshift64)88172645463325252ull; }

static inline u32 Xorshift32_Next(Xorshift32* gen) {
    *gen ^= *gen << 13;
    *gen ^= *gen >> 17;
    *gen ^= *gen << 5;
    return *gen;
}

static inline u64 Xorshift64_Next(Xorshift64* gen) {
    *gen ^= *gen << 13;
    *gen ^= *gen >> 7;
    *gen ^= *gen << 17;
    return *gen;
}
