#ifndef HASH_H
#define HASH_H

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>

namespace Crypto {
    namespace SHA256 {
        class Hash {
        public:
            Hash();

            // SHA-256 SIBGLE-HASH
            static std::string sha256(const std::string& data);
            static std::string sha256(const std::vector<uint8_t>& data);
            
            // SHA-256 DOUBLE-HASH
            static std::string sha256d(const std::string& data);
            static std::string sha256d(const std::vector<uint8_t>& data);
            
            // RIPEMD-160HASH
            static std::string ripemd160(const std::string& data);
            static std::string ripemd160(const std::vector<uint8_t>& data);
            
            // Hash160 (SHA-256 + RIPEMD-160) - Used for addresses
            static std::string hash160(const std::string& data);
            
            // MERKLE TREE FOR ROOT HASH
            static std::string merkleRoot(const std::vector<std::string>& hashes);
            
            // UTILS
            static std::string bytesToHex(const std::vector<uint8_t>& bytes);
            static std::vector<uint8_t> hexToBytes(const std::string& hex);

            ~Hash();
        private:
            static std::vector<uint8_t> sha256Raw(const std::vector<uint8_t>& data);
            static std::vector<uint8_t> ripemd160Raw(const std::vector<uint8_t>& data);

        };
    } // namespace SHA256
} // namespace Crypto


#endif