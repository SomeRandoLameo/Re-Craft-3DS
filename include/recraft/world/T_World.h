#pragma once

#include "../../mclib/protocol/packets/PacketHandler.h"
#include "../../mclib/protocol/packets/PacketDispatcher.h"
#include "../../mclib/util/ObserverSubject.h"

#include "T_Chunk.h"

#include <unordered_map>

namespace terra {

    using ChunkCoord = std::pair<s32, s32>;

}

namespace std {
    template <> struct hash<terra::ChunkCoord> {
        std::size_t operator()(const terra::ChunkCoord& s) const noexcept {
            std::size_t seed = 3;

            seed ^= s.first + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= s.second + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
}

namespace terra {

    class WorldListener {
    public:
        // yIndex is the chunk section index of the column, 0 means bottom chunk, 15 means top
        virtual void OnChunkLoad(ChunkPtr chunk, const ChunkColumnMetadata& meta, u16 yIndex) { }
        virtual void OnChunkUnload(ChunkColumnPtr chunk) { }
        virtual void OnBlockChange(mc::Vector3i position, mc::block::BlockPtr newBlock, mc::block::BlockPtr oldBlock) { }
    };

    class T_World : public mc::protocol::packets::PacketHandler, public mc::util::ObserverSubject<WorldListener> {
    public:
        T_World(mc::protocol::packets::PacketDispatcher* dispatcher);
        ~T_World();

        T_World(const T_World& rhs) = delete;
        T_World& operator=(const T_World& rhs) = delete;
        T_World(T_World&& rhs) = delete;
        T_World& operator=(T_World&& rhs) = delete;

        void HandlePacket(mc::protocol::packets::in::ChunkDataPacket* packet);
        void HandlePacket(mc::protocol::packets::in::UnloadChunkPacket* packet);
        void HandlePacket(mc::protocol::packets::in::MultiBlockChangePacket* packet);
        void HandlePacket(mc::protocol::packets::in::BlockChangePacket* packet);
        void HandlePacket(mc::protocol::packets::in::ExplosionPacket* packet);
        void HandlePacket(mc::protocol::packets::in::UpdateBlockEntityPacket* packet);
        void HandlePacket(mc::protocol::packets::in::RespawnPacket* packet);

        /**
         * Pos can be any world position inside of the chunk
         */
        ChunkColumnPtr GetChunk(const mc::Vector3i& pos) const;

        mc::block::BlockPtr GetBlock(const mc::Vector3i& pos) const;

        mc::block::BlockEntityPtr GetBlockEntity(mc::Vector3i pos) const;
        // Gets all of the known block entities in loaded chunks
        std::vector<mc::block::BlockEntityPtr> GetBlockEntities() const;

        const std::unordered_map<ChunkCoord, ChunkColumnPtr>::const_iterator begin() const { return m_Chunks.begin(); }
        const std::unordered_map<ChunkCoord, ChunkColumnPtr>::const_iterator end() const { return m_Chunks.end(); }

    private:
        std::unordered_map<ChunkCoord, ChunkColumnPtr> m_Chunks;

        bool SetBlock(mc::Vector3i position, u32 blockData);
    };

}