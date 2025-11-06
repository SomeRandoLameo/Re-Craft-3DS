#ifndef TERRACOTTA_CHUNK_H_
#define TERRACOTTA_CHUNK_H_

#include <mclib/block/Block.h>
#include <mclib/block/BlockEntity.h>
#include <mclib/common/Types.h>
#include <mclib/nbt/NBT.h>
#include <mclib/world/Chunk.h>

#include <array>
#include <map>
#include <memory>

namespace terra {

struct ChunkColumnMetadata {
    s32 x;
    s32 z;
    u16 sectionmask;
    bool continuous;
    bool skylight;

    ChunkColumnMetadata(const mc::world::ChunkColumnMetadata& metadata) 
        : x(metadata.x),
          z(metadata.z),
          sectionmask(metadata.sectionmask),
          continuous(metadata.continuous),
          skylight(metadata.skylight)
    {

    }
};

/**
 * A 16x16x16 area. A ChunkColumn is made up of 16 of these
 */
class Chunk {
private:
    std::array<mc::block::BlockPtr, 16 * 16 * 16> m_Blocks;

public:
    Chunk();

    Chunk(const Chunk& other) = delete;
    Chunk& operator=(const Chunk& other) = delete;

    Chunk(const mc::world::Chunk& other);

    /**
     * Position is relative to this chunk position
     */
    mc::block::BlockPtr GetBlock(const mc::Vector3i& chunkPosition) const;

    /**
    * Position is relative to this chunk position
    */
    void SetBlock(mc::Vector3i chunkPosition, mc::block::BlockPtr block);
};

typedef std::shared_ptr<Chunk> ChunkPtr;

/**
 * Stores a 16x256x16 area. Uses chunks (16x16x16) to store the data vertically.
 * A null chunk is fully air.
 */
class ChunkColumn {
public:
    enum { ChunksPerColumn = 16 };

    typedef std::array<ChunkPtr, ChunksPerColumn>::iterator iterator;
    typedef std::array<ChunkPtr, ChunksPerColumn>::reference reference;
    typedef std::array<ChunkPtr, ChunksPerColumn>::const_iterator const_iterator;
    typedef std::array<ChunkPtr, ChunksPerColumn>::const_reference const_reference;

private:
    std::array<ChunkPtr, ChunksPerColumn> m_Chunks;
    ChunkColumnMetadata m_Metadata;
    std::map<mc::Vector3i, mc::block::BlockEntityPtr> m_BlockEntities;

public:
    ChunkColumn(ChunkColumnMetadata metadata);

    ChunkColumn(const ChunkColumn& rhs) = default;
    ChunkColumn& operator=(const ChunkColumn& rhs) = default;
    ChunkColumn(ChunkColumn&& rhs) = default;
    ChunkColumn& operator=(ChunkColumn&& rhs) = default;

    ChunkColumn(const mc::world::ChunkColumn& rhs);

    iterator begin() {
        return m_Chunks.begin();
    }

    iterator end() {
        return m_Chunks.end();
    }

    reference operator[](std::size_t index) {
        return m_Chunks[index];
    }

    const_iterator begin() const {
        return m_Chunks.begin();
    }

    const_iterator end() const {
        return m_Chunks.end();
    }

    const_reference operator[](std::size_t index) const {
        return m_Chunks[index];
    }

    void AddBlockEntity(mc::block::BlockEntityPtr blockEntity) {
        m_BlockEntities.insert(std::make_pair(blockEntity->GetPosition(), blockEntity));
    }

    void RemoveBlockEntity(mc::Vector3i pos) {
        m_BlockEntities.erase(pos);
    }

    /**
     * Position is relative to this ChunkColumn position.
     */
    mc::block::BlockPtr GetBlock(const mc::Vector3i& position);
    const ChunkColumnMetadata& GetMetadata() const { return m_Metadata; }

    mc::block::BlockEntityPtr GetBlockEntity(mc::Vector3i worldPos);
    std::vector<mc::block::BlockEntityPtr> GetBlockEntities();
};

typedef std::shared_ptr<ChunkColumn> ChunkColumnPtr;

} // ns terra

#endif
