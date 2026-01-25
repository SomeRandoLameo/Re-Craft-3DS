#pragma once

#include "world/CT_Chunk.hpp"

#define SUPERCHUNK_SIZE 8
#define SUPERCHUNK_BLOCKSIZE (SUPERCHUNK_SIZE * CHUNK_SIZE)

// TODO: REMOVE AFTER REWRITE!!! // que
#define vec_unpack_(v) (char**)&(v)->data, &(v)->length, &(v)->capacity, sizeof(*(v)->data)


#define vec_t(T)                                                                                                       \
    struct {                                                                                                           \
        T*  data;                                                                                                      \
        int length, capacity;                                                                                          \
    }


#define vec_init(v) memset((v), 0, sizeof(*(v)))


#define vec_deinit(v) (free((v)->data), vec_init(v))


#define vec_push(v, val) (vec_expand_(vec_unpack_(v)) ? -1 : ((v)->data[(v)->length++] = (val), 0), 0)


int vec_expand_(char** data, int* length, int* capacity, int memsz);

struct ChunkInfo {
    u32 position;
    u32 compressedSize;
    u32 actualSize;
    u8  blockSize;
    u32 revision;
};

// TODO: Turn this into a class
struct SuperChunk {
    int       x, z;
    FILE*     dataFile;
    ChunkInfo grid[SUPERCHUNK_SIZE][SUPERCHUNK_SIZE];
    vec_t(u8) sectors;
};

inline static int ChunkToSuperChunkCoord(int x) {
    return (x + (int)(x < 0)) / SUPERCHUNK_SIZE - (int)(x < 0);
}
inline static int ChunkToLocalSuperChunkCoord(int x) {
    return x - ChunkToSuperChunkCoord(x) * SUPERCHUNK_SIZE;
}

void SuperChunk_InitPools();
void SuperChunk_DeinitPools();

void SuperChunk_Init(SuperChunk* superchunk, int x, int z);
void SuperChunk_Deinit(SuperChunk* superchunk);
void SuperChunk_SaveIndex(SuperChunk* superchunk);

void SuperChunk_SaveChunk(SuperChunk* superchunk, ChunkColumnPtr column);
void SuperChunk_LoadChunk(SuperChunk* superchunk, ChunkColumnPtr column);
