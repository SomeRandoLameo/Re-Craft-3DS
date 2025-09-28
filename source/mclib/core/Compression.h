#pragma once

#include "../mclib.h"
#include "../common/Types.h"

namespace mc {

    class DataBuffer;

    namespace core {

        class CompressionStrategy {
        public:
            virtual ~CompressionStrategy() { }
            virtual DataBuffer Compress(DataBuffer& buffer) = 0;
            virtual DataBuffer Decompress(DataBuffer& buffer, std::size_t packetLength) = 0;
        };

        class CompressionNone : public CompressionStrategy {
        public:
            DataBuffer Compress(DataBuffer& buffer);
            DataBuffer Decompress(DataBuffer& buffer, std::size_t packetLength);
        };

        class CompressionZ : public CompressionStrategy {
        private:
            // How large a packet needs to be before it's compressed.
            // Don't compress packets smaller than this.
            // Received in SetCompressionPacket.
            u64 m_CompressionThreshold;

        public:
            CompressionZ(u64 threshold) : m_CompressionThreshold(threshold) { }

            DataBuffer Compress(DataBuffer& buffer);
            DataBuffer Decompress(DataBuffer& buffer, std::size_t packetLength);
        };

    } // ns core
} // ns mc
