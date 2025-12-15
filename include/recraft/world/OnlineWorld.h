#pragma once

#include "CT_World.h"
#include "ChunkWorker.h"
#include "mclib/world/Chunk.h"
#include "mclib/world/World.h"

class OnlineWorld : public mc::world::WorldListener {
private:
   int m_chunkCount = 0;

public:
    OnlineWorld();
    ~OnlineWorld();

    void OnChunkUnload(mc::world::ChunkColumnPtr chunk) override;
    void OnChunkLoad(mc::world::ChunkPtr chunk, const mc::world::ChunkColumnMetadata& meta, u16 yIndex) override;
    void OnBlockChange(mc::Vector3i position, mc::block::BlockPtr newBlock, mc::block::BlockPtr oldBlock) override;
};