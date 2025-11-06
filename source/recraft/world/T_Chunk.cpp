#include "T_Chunk.h"

#include <algorithm>

namespace terra {

    T_Chunk::T_Chunk() {
        auto block = mc::block::BlockRegistry::GetInstance()->GetBlock(0);

        for (int y = 0; y < 16; ++y) {
            for (int z = 0; z < 16; ++z) {
                for (int x = 0; x < 16; ++x) {
                    m_Blocks[y * 16 * 16 + z * 16 + x] = block;
                }
            }
        }
    }

    T_Chunk::T_Chunk(const mc::world::Chunk& other) {
        static const mc::block::BlockPtr air = mc::block::BlockRegistry::GetInstance()->GetBlock(0);

        for (int y = 0; y < 16; ++y) {
            for (int z = 0; z < 16; ++z) {
                for (int x = 0; x < 16; ++x) {
                    mc::block::BlockPtr block = other.GetBlock(mc::Vector3i(x, y, z));

                    if (block == nullptr) {
                        block = air;
                    }

                    m_Blocks[y * 16 * 16 + z * 16 + x] = block;
                }
            }
        }
    }

    mc::block::BlockPtr T_Chunk::GetBlock(const mc::Vector3i& chunkPosition) const {
        std::size_t index = chunkPosition.y * 16 * 16 + chunkPosition.z * 16 + chunkPosition.x;

        if (chunkPosition.x < 0 || chunkPosition.x > 15 || chunkPosition.y < 0 || chunkPosition.y > 15 || chunkPosition.z < 0 || chunkPosition.z > 15) {
            return nullptr;
        }

        return m_Blocks[index];
    }

    void T_Chunk::SetBlock(mc::Vector3i chunkPosition, mc::block::BlockPtr block) {
        m_Blocks[chunkPosition.y * 16 * 16 + chunkPosition.z * 16 + chunkPosition.x] = block;
    }

    ChunkColumn::ChunkColumn(const mc::world::ChunkColumn& rhs)
        : m_Metadata(terra::ChunkColumnMetadata(rhs.GetMetadata()))
    {
        for (int i = 0; i < ChunksPerColumn; ++i) {
            m_Chunks[i] = nullptr;

            if (rhs[i] != nullptr) {
                m_Chunks[i] = std::make_shared<T_Chunk>(*rhs[i]);
            }
        }
    }

    ChunkColumn::ChunkColumn(ChunkColumnMetadata metadata)
        : m_Metadata(metadata)
    {
        for (std::size_t i = 0; i < m_Chunks.size(); ++i)
            m_Chunks[i] = nullptr;
    }

    mc::block::BlockPtr ChunkColumn::GetBlock(const mc::Vector3i& position) {
        s32 chunkIndex = (s64)std::floor(position.y / 16.0);
        mc::Vector3i relativePosition(position.x, position.y % 16, position.z);

        static const mc::block::BlockPtr air = mc::block::BlockRegistry::GetInstance()->GetBlock(0);

        if (chunkIndex < 0 || chunkIndex > 15 || !m_Chunks[chunkIndex]) return air;

        return m_Chunks[chunkIndex]->GetBlock(relativePosition);
    }

    mc::block::BlockEntityPtr ChunkColumn::GetBlockEntity(mc::Vector3i worldPos) {
        auto iter = m_BlockEntities.find(worldPos);
        if (iter == m_BlockEntities.end()) return nullptr;
        return iter->second;
    }

    std::vector<mc::block::BlockEntityPtr> ChunkColumn::GetBlockEntities() {
        std::vector<mc::block::BlockEntityPtr> blockEntities;

        for (auto iter = m_BlockEntities.begin(); iter != m_BlockEntities.end(); ++iter)
            blockEntities.push_back(iter->second);

        return blockEntities;
    }

}
