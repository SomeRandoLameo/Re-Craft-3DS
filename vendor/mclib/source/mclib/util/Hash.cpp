#include "Hash.h"

#include <vector>
#include <sstream>
#include <limits>
#include <iostream>
#include <iomanip>
#include <cstring>

// mbedTLS headers instead of OpenSSL
#include "mbedtls/sha1.h"
#include "mbedtls/base64.h"

// Define SHA_DIGEST_LENGTH for compatibility
#define SHA_DIGEST_LENGTH 20

namespace mc {
    namespace util {

        std::string Base64Decode(const std::string& message) {
            size_t output_len;

            // First call to get required output buffer size
            int ret = mbedtls_base64_decode(nullptr, 0, &output_len,
                                            (const unsigned char*)message.c_str(), message.length());

            if (ret != MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL) {
                return ""; // Error in input
            }

            std::string output;
            output.resize(output_len);

            ret = mbedtls_base64_decode((unsigned char*)&output[0], output_len, &output_len,
                                        (const unsigned char*)message.c_str(), message.length());

            if (ret == 0) {
                output.resize(output_len);
                return output;
            }

            return ""; // Error occurred
        }

        std::array<unsigned char, SHA_DIGEST_LENGTH> Sha1TwosComplement(const unsigned char* digest) {
            std::array<unsigned char, SHA_DIGEST_LENGTH> ret;

            bool carry = false;
            auto end = ret.end();
            auto last = end - 1;

            for (auto begin = ret.begin(); begin != end; ++begin, ++digest) {
                unsigned char& current = *begin;
                current = ~*digest;
                if ((begin == last) || carry) {
                    carry = (current == std::numeric_limits<unsigned char>::max());
                    if (carry)
                        current = 0;
                    else
                        ++current;
                }
            }

            return ret;
        }

        std::string Sha1HexDigest(const unsigned char* digest) {
            std::string new_digest((char*)digest, SHA_DIGEST_LENGTH);
            std::stringstream ss;

            bool negative = (new_digest[0] & (1 << 7)) != 0;

            if (negative) {
                auto arr = Sha1TwosComplement(digest);
                for (std::size_t i = 0; i < SHA_DIGEST_LENGTH; ++i)
                    new_digest[i] = arr[i];
            }

            for (std::size_t i = 0; i < SHA_DIGEST_LENGTH; ++i)
                ss << std::hex << std::setfill('0') << std::setw(2) << (int)(new_digest[i] & 0xFF);

            std::string result = ss.str();
            std::size_t pos = 0;
            while (result[pos] == '0')
                pos++;
            if (result[0] == '0')
                result = result.substr(pos);

            if (negative)
                result = '-' + result;
            return result;
        }

        // Helper function to compute SHA1 using mbedTLS
        void ComputeSha1(const unsigned char* input, size_t length, unsigned char* output) {
            mbedtls_sha1_context ctx;
            mbedtls_sha1_init(&ctx);

            mbedtls_sha1_starts(&ctx);
            mbedtls_sha1_update(&ctx, input, length);
            mbedtls_sha1_finish(&ctx, output);

            mbedtls_sha1_free(&ctx);
        }

        // Alternative one-shot SHA1 function (simpler interface)
        void Sha1(const unsigned char* input, size_t length, unsigned char* output) {
            mbedtls_sha1(input, length, output);
        }

        bool Sha1DigestTest() {
            std::vector<std::string> inputs = { "Notch", "jeb_", "simon" };
            std::vector<std::string> outputs = {
                    "4ed1f46bbe04bc756bcb17c0c7ce3e4632f06a48",
                    "-7c9d5b0044c130109a5d7b5fb5c317c02b4e28c1",
                    "88e16a1019277b15d58faf0541e11910eb756f6"
            };

            bool pass = true;

            for (std::size_t i = 0; i < inputs.size(); ++i) {
                std::string input = inputs[i];
                unsigned char digest[SHA_DIGEST_LENGTH] = { 0 };

                // Use mbedTLS SHA1 function instead of OpenSSL's SHA1
                Sha1((const unsigned char*)input.c_str(), input.length(), digest);
                std::string result = Sha1HexDigest(digest);

                if (result.compare(outputs[i]) != 0) {
                    std::cerr << "Hex digest not a match. Expected " << outputs[i] << " got " << result << std::endl;
                    pass = false;
                }
            }

            return pass;
        }

    } // ns util
} // ns mc