#pragma once

#include "Chunk.h"
#include "../protocol/packets/PacketHandler.h"
#include "../protocol/packets/PacketDispatcher.h"
#include "../util/ObserverSubject.h"

#include <map>

namespace mc {
    namespace world {

        class  WorldListener {
        public:
            // yIndex is the chunk section index of the column, 0 means bottom chunk, 15 means top
            virtual void OnChunkLoad(ChunkPtr chunk, const ChunkColumnMetadata& meta, u16 yIndex) { }
            virtual void OnChunkUnload(ChunkColumnPtr chunk) { }
            virtual void OnBlockChange(Vector3i position, block::BlockPtr newBlock, block::BlockPtr oldBlock) { }
        };

        class World : public protocol::packets::PacketHandler, public util::ObserverSubject<WorldListener> {
        private:
            typedef std::pair<s32, s32> ChunkCoord;

            std::map<ChunkCoord, ChunkColumnPtr> m_Chunks;

            bool  SetBlock(Vector3i position, u32 blockData);

        public:
             World(protocol::packets::PacketDispatcher* dispatcher);
             ~World();

            World(const World& rhs) = delete;
            World& operator=(const World& rhs) = delete;
            World(World&& rhs) = delete;
            World& operator=(World&& rhs) = delete;

            void  HandlePacket(protocol::packets::in::ChunkDataPacket* packet);
            void  HandlePacket(protocol::packets::in::UnloadChunkPacket* packet);
            void  HandlePacket(protocol::packets::in::MultiBlockChangePacket* packet);
            void  HandlePacket(protocol::packets::in::BlockChangePacket* packet);
            void  HandlePacket(protocol::packets::in::ExplosionPacket* packet);
            void  HandlePacket(protocol::packets::in::UpdateBlockEntityPacket* packet);
            void  HandlePacket(protocol::packets::in::RespawnPacket* packet);

            /**
             * Pos can be any world position inside of the chunk
             */
            ChunkColumnPtr  GetChunk(Vector3i pos) const;

            block::BlockPtr  GetBlock(Vector3d pos) const;
            block::BlockPtr  GetBlock(Vector3f pos) const;
            block::BlockPtr  GetBlock(Vector3i pos) const;

             block::BlockEntityPtr GetBlockEntity(Vector3i pos) const;
            // Gets all of the known block entities in loaded chunks
             std::vector<block::BlockEntityPtr> GetBlockEntities() const;

            const std::map<ChunkCoord, ChunkColumnPtr>::const_iterator begin() const { return m_Chunks.begin(); }
            const std::map<ChunkCoord, ChunkColumnPtr>::const_iterator end() const { return m_Chunks.end(); }
        };

    } // ns world
} // ns mc
