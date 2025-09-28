#pragma once

#include <string>

#include "../mclib.h"
#include "../protocol/packets/Packet.h"

namespace mc {

    class DataBuffer;

    namespace core {

        struct EncryptionStrategy {
            virtual ~EncryptionStrategy() { }
            virtual DataBuffer Encrypt(const DataBuffer& buffer) = 0;
            virtual DataBuffer Decrypt(const DataBuffer& buffer) = 0;
        };

        class EncryptionStrategyNone : public EncryptionStrategy {
        public:
            DataBuffer Encrypt(const DataBuffer& buffer);
            DataBuffer Decrypt(const DataBuffer& buffer);
        };

        class EncryptionStrategyAES : public EncryptionStrategy {
        private:
            class Impl;
            Impl* m_Impl;

        public:
            EncryptionStrategyAES(const std::string& publicKey, const std::string& verifyToken);
            ~EncryptionStrategyAES();

            EncryptionStrategyAES(const EncryptionStrategyAES& other) = delete;
            EncryptionStrategyAES& operator=(const EncryptionStrategyAES& other) = delete;
            EncryptionStrategyAES(EncryptionStrategyAES&& other) = delete;
            EncryptionStrategyAES& operator=(EncryptionStrategyAES&& other) = delete;

            DataBuffer Encrypt(const DataBuffer& buffer);
            DataBuffer Decrypt(const DataBuffer& buffer);

            std::string GetSharedSecret() const;
            protocol::packets::out::EncryptionResponsePacket* GenerateResponsePacket() const;
        };

    } // ns core
} // ns mc
