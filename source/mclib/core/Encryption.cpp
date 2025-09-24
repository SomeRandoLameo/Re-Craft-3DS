#include "Encryption.h"

#include "../common/DataBuffer.h"

#include <algorithm>
#include <random>
#include <functional>
#include <cstring>

// mbedTLS headers instead of OpenSSL
#include "mbedtls/aes.h"
#include "mbedtls/rsa.h"
#include "mbedtls/pk.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/base64.h"

namespace mc {
    namespace core {

        class RandomGenerator {
        private:
            std::random_device m_RandomDevice;
            std::mt19937 m_Generator;

        public:
            RandomGenerator() : m_RandomDevice(), m_Generator(m_RandomDevice())
            {
            }

            RandomGenerator(unsigned int seed) : m_RandomDevice(), m_Generator(seed)
            {
            }

            unsigned int GetInt(unsigned int min, unsigned int max) {
                std::uniform_int_distribution<unsigned int> distr(min, max);
                return distr(m_Generator);
            }
        };

        // CFB8 implementation for mbedTLS (since mbedTLS doesn't have CFB8, only CFB128)
        class AES_CFB8 {
        private:
            mbedtls_aes_context m_aes_ctx;
            unsigned char m_iv[16];

        public:
            AES_CFB8() {
                mbedtls_aes_init(&m_aes_ctx);
            }

            ~AES_CFB8() {
                mbedtls_aes_free(&m_aes_ctx);
            }

            int setkey_enc(const unsigned char* key, unsigned int keybits) {
                return mbedtls_aes_setkey_enc(&m_aes_ctx, key, keybits);
            }

            int setkey_dec(const unsigned char* key, unsigned int keybits) {
                return mbedtls_aes_setkey_dec(&m_aes_ctx, key, keybits);
            }

            void set_iv(const unsigned char* iv) {
                memcpy(m_iv, iv, 16);
            }

            int crypt_cfb8(int mode, size_t length, unsigned char* output, const unsigned char* input) {
                unsigned char keystream[16];
                size_t n;

                for (n = 0; n < length; n++) {
                    // Encrypt the IV to get keystream
                    int ret = mbedtls_aes_crypt_ecb(&m_aes_ctx, MBEDTLS_AES_ENCRYPT, m_iv, keystream);
                    if (ret != 0) return ret;

                    if (mode == MBEDTLS_AES_DECRYPT) {
                        // For decryption: shift IV first, then XOR
                        memmove(m_iv, m_iv + 1, 15);
                        m_iv[15] = input[n];
                        output[n] = input[n] ^ keystream[0];
                    } else {
                        // For encryption: XOR first, then shift IV
                        output[n] = input[n] ^ keystream[0];
                        memmove(m_iv, m_iv + 1, 15);
                        m_iv[15] = output[n];
                    }
                }
                return 0;
            }
        };

        DataBuffer EncryptionStrategyNone::Encrypt(const DataBuffer& buffer) {
            return buffer;
        }

        DataBuffer EncryptionStrategyNone::Decrypt(const DataBuffer& buffer) {
            return buffer;
        }

        class EncryptionStrategyAES::Impl {
        private:
            RandomGenerator m_RNG;
            AES_CFB8 m_EncryptCTX;
            AES_CFB8 m_DecryptCTX;

            // mbedTLS random number generation
            mbedtls_entropy_context m_entropy;
            mbedtls_ctr_drbg_context m_ctr_drbg;

            protocol::packets::out::EncryptionResponsePacket* m_ResponsePacket;

            struct {
                unsigned char* key;
                unsigned int len;
            } m_PublicKey;

            struct {
                unsigned char key[16]; // AES-128 = 16 bytes
                unsigned int len;
            } m_SharedSecret;

            bool Initialize(const std::string& publicKey, const std::string& verifyToken) {
                // Initialize random number generator
                mbedtls_entropy_init(&m_entropy);
                mbedtls_ctr_drbg_init(&m_ctr_drbg);

                const char* pers = "minecraft_aes";
                int ret = mbedtls_ctr_drbg_seed(&m_ctr_drbg, mbedtls_entropy_func, &m_entropy,
                                                (const unsigned char*)pers, strlen(pers));
                if (ret != 0) return false;

                // Parse the public key (assuming DER format)
                mbedtls_pk_context pk;
                mbedtls_pk_init(&pk);

                ret = mbedtls_pk_parse_public_key(&pk, (const unsigned char*)publicKey.c_str(),
                                                  publicKey.length() + 1);
                if (ret != 0) {
                    mbedtls_pk_free(&pk);
                    return false;
                }

                // Generate random shared secret
                m_SharedSecret.len = 16; // AES-128
                ret = mbedtls_ctr_drbg_random(&m_ctr_drbg, m_SharedSecret.key, m_SharedSecret.len);
                if (ret != 0) {
                    mbedtls_pk_free(&pk);
                    return false;
                }

                // Get RSA context from pk context
                mbedtls_rsa_context* rsa = mbedtls_pk_rsa(pk);
                size_t rsa_size = mbedtls_rsa_get_len(rsa);

                std::string encryptedSS;
                std::string encryptedToken;
                encryptedSS.resize(rsa_size);
                encryptedToken.resize(rsa_size);

                size_t olen;

                // Encrypt the shared secret with public key
                ret = mbedtls_rsa_pkcs1_encrypt(rsa, mbedtls_ctr_drbg_random, &m_ctr_drbg,
                                                MBEDTLS_RSA_PUBLIC, m_SharedSecret.len,
                                                m_SharedSecret.key, (unsigned char*)&encryptedSS[0]);
                if (ret != 0) {
                    mbedtls_pk_free(&pk);
                    return false;
                }

                // Encrypt the verify token with public key
                ret = mbedtls_rsa_pkcs1_encrypt(rsa, mbedtls_ctr_drbg_random, &m_ctr_drbg,
                                                MBEDTLS_RSA_PUBLIC, verifyToken.length(),
                                                (const unsigned char*)verifyToken.c_str(),
                                                (unsigned char*)&encryptedToken[0]);

                mbedtls_pk_free(&pk);
                if (ret != 0) return false;

                // Initialize AES-128-CFB8 encryption and decryption
                if (m_EncryptCTX.setkey_enc(m_SharedSecret.key, 128) != 0)
                    return false;

                if (m_DecryptCTX.setkey_enc(m_SharedSecret.key, 128) != 0) // CFB8 uses encryption key for both
                    return false;

                // Set IV (same as key for Minecraft)
                m_EncryptCTX.set_iv(m_SharedSecret.key);
                m_DecryptCTX.set_iv(m_SharedSecret.key);

                m_ResponsePacket = new protocol::packets::out::EncryptionResponsePacket(encryptedSS, encryptedToken);
                return true;
            }

        public:
            Impl(const std::string& publicKey, const std::string& verifyToken)
                    : m_ResponsePacket(nullptr)
            {
                m_PublicKey.key = nullptr;
                Initialize(publicKey, verifyToken);
            }

            ~Impl() {
                if (m_ResponsePacket)
                    delete m_ResponsePacket;

                mbedtls_entropy_free(&m_entropy);
                mbedtls_ctr_drbg_free(&m_ctr_drbg);
            }

            DataBuffer encrypt(const DataBuffer& buffer) {
                DataBuffer result;
                result.Resize(buffer.GetSize());

                int ret = m_EncryptCTX.crypt_cfb8(MBEDTLS_AES_ENCRYPT, buffer.GetSize(),
                                                  &result[0], &buffer[0]);
                if (ret != 0) {
                    result.Resize(0);
                }

                return result;
            }

            DataBuffer decrypt(const DataBuffer& buffer) {
                DataBuffer result;
                result.Resize(buffer.GetSize());

                int ret = m_DecryptCTX.crypt_cfb8(MBEDTLS_AES_DECRYPT, buffer.GetSize(),
                                                  &result[0], &buffer[0]);
                if (ret != 0) {
                    result.Resize(0);
                }

                return result;
            }

            std::string GetSharedSecret() const {
                return std::string((char*)m_SharedSecret.key, m_SharedSecret.len);
            }

            protocol::packets::out::EncryptionResponsePacket* GenerateResponsePacket() const {
                return m_ResponsePacket;
            }
        };

        EncryptionStrategyAES::EncryptionStrategyAES(const std::string& publicKey, const std::string& verifyToken) {
            m_Impl = new Impl(publicKey, verifyToken);
        }

        EncryptionStrategyAES::~EncryptionStrategyAES() {
            delete m_Impl;
        }

        DataBuffer EncryptionStrategyAES::Encrypt(const DataBuffer& buffer) {
            return m_Impl->encrypt(buffer);
        }

        DataBuffer EncryptionStrategyAES::Decrypt(const DataBuffer& buffer) {
            return m_Impl->decrypt(buffer);
        }

        std::string EncryptionStrategyAES::GetSharedSecret() const {
            return m_Impl->GetSharedSecret();
        }

        protocol::packets::out::EncryptionResponsePacket* EncryptionStrategyAES::GenerateResponsePacket() const {
            return m_Impl->GenerateResponsePacket();
        }

    } // ns core
} // ns mc