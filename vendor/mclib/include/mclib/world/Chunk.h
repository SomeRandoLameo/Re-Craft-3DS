#pragma once

#include "../block/Block.h"
#include "../block/BlockEntity.h"
#include "../common/Types.h"
#include "../nbt/NBT.h"

#include <array>
#include <map>
#include <memory>

namespace mc {

    class DataBuffer;

    namespace world {

        struct ChunkColumnMetadata {
            s32 x;
            s32 z;
            u16 sectionmask;
            bool continuous;
            bool skylight;
        };


        /**
         * A 16x16x16 area. A ChunkColumn is made up of 16 of these
         */
        class Chunk {
        private:
            std::vector<u32> m_Palette;
            std::vector<u64> m_Data;
            u8 m_BitsPerBlock;

        public:
            Chunk();

            Chunk(const Chunk& other);
            Chunk& operator=(const Chunk& other);
            /**
             * Position is relative to this chunk position
             */
            block::BlockPtr GetBlock(Vector3i chunkPosition) const;

            /**
            * Position is relative to this chunk position
            */
            void SetBlock(Vector3i chunkPosition, block::BlockPtr block);

            /**
             * chunkIndex is the index (0-16) of this chunk in the ChunkColumn
             */
            void Load(DataBuffer& in, ChunkColumnMetadata* meta, s32 chunkIndex);
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
            std::map<Vector3i, block::BlockEntityPtr> m_BlockEntities;

        public:
            ChunkColumn(ChunkColumnMetadata metadata);

            ChunkColumn(const ChunkColumn& rhs) = default;
            ChunkColumn& operator=(const ChunkColumn& rhs) = default;
            ChunkColumn(ChunkColumn&& rhs) = default;
            ChunkColumn& operator=(ChunkColumn&& rhs) = default;

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

            void AddBlockEntity(block::BlockEntityPtr blockEntity) {
                m_BlockEntities.insert(std::make_pair(blockEntity->GetPosition(), blockEntity));
            }

            void RemoveBlockEntity(Vector3i pos) {
                m_BlockEntities.erase(pos);
            }

            /**
             * Position is relative to this ChunkColumn position.
             */
            block::BlockPtr GetBlock(Vector3i position);
            const ChunkColumnMetadata& GetMetadata() const { return m_Metadata; }

            block::BlockEntityPtr GetBlockEntity(Vector3i worldPos);
            std::vector<block::BlockEntityPtr> GetBlockEntities();

            friend DataBuffer& operator>>(DataBuffer& in, ChunkColumn& column);
        };

        typedef std::shared_ptr<ChunkColumn> ChunkColumnPtr;

        DataBuffer& operator>>(DataBuffer& in, ChunkColumn& column);

    } // ns world
} // ns mc