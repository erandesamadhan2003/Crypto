#include "crypto/hash.h"
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "utils/Logger.h"

namespace Crypto {
    namespace SHA256 {

        // Constructor
        Hash::Hash() {
            Crypto::Utils::Logger::getInstance()->log(Crypto::Utils::LogLevel::INFO, "Hash initialized.");
        }

        // Destructor
        Hash::~Hash() {
            Crypto::Utils::Logger::getInstance()->log(Crypto::Utils::LogLevel::INFO, "Hash destroyed.");
        }

        // SHA-256 single hash (string input)
        std::string Hash::sha256(const std::string& data) {
            try {
                std::vector<uint8_t> bytes(data.begin(), data.end());
                std::string result = bytesToHex(sha256Raw(bytes));
                Crypto::Utils::Logger::getInstance()->debug("SHA-256 hash computed for " + std::to_string(data.length()) + " bytes");
                return result;
            } catch (const std::exception& e) {
                Crypto::Utils::Logger::getInstance()->error("SHA-256 hash failed: " + std::string(e.what()));
                throw;
            }
        }

        // SHA-256 single hash (bytes input)  
        std::string Hash::sha256(const std::vector<uint8_t>& data) {
            try {
                std::string result = bytesToHex(sha256Raw(data));
                Crypto::Utils::Logger::getInstance()->debug("SHA-256 hash computed for " + std::to_string(data.size()) + " bytes");
                return result;
            } catch (const std::exception& e) {
                Crypto::Utils::Logger::getInstance()->error("SHA-256 hash failed: " + std::string(e.what()));
                throw;
            }
        }

        // SHA-256 double hash (Bitcoin standard)
        std::string Hash::sha256d(const std::string& data) {
            try {
                std::vector<uint8_t> bytes(data.begin(), data.end());
                std::string result = sha256d(bytes);
                Crypto::Utils::Logger::getInstance()->debug("SHA-256d hash computed for string data");
                return result;
            } catch (const std::exception& e) {
                Crypto::Utils::Logger::getInstance()->error("SHA-256d hash failed: " + std::string(e.what()));
                throw;
            }
        }

        std::string Hash::sha256d(const std::vector<uint8_t>& data) {
            try {
                auto firstHash = sha256Raw(data);
                auto secondHash = sha256Raw(firstHash);
                std::string result = bytesToHex(secondHash);
                Crypto::Utils::Logger::getInstance()->debug("SHA-256d double hash computed");
                return result;
            } catch (const std::exception& e) {
                Crypto::Utils::Logger::getInstance()->error("SHA-256d double hash failed: " + std::string(e.what()));
                throw;
            }
        }

        // RIPEMD-160 hash (string input)
        std::string Hash::ripemd160(const std::string& data) {
            try {
                std::vector<uint8_t> bytes(data.begin(), data.end());
                std::string result = bytesToHex(ripemd160Raw(bytes));
                Crypto::Utils::Logger::getInstance()->debug("RIPEMD-160 hash computed for string data");
                return result;
            } catch (const std::exception& e) {
                Crypto::Utils::Logger::getInstance()->error("RIPEMD-160 hash failed: " + std::string(e.what()));
                throw;
            }
        }

        // RIPEMD-160 hash (bytes input)
        std::string Hash::ripemd160(const std::vector<uint8_t>& data) {
            try {
                std::string result = bytesToHex(ripemd160Raw(data));
                Crypto::Utils::Logger::getInstance()->debug("RIPEMD-160 hash computed for " + std::to_string(data.size()) + " bytes");
                return result;
            } catch (const std::exception& e) {
                Crypto::Utils::Logger::getInstance()->error("RIPEMD-160 hash failed: " + std::string(e.what()));
                throw;
            }
        }

        // Hash160: SHA-256 then RIPEMD-160 (for Bitcoin addresses)
        std::string Hash::hash160(const std::string& data) {
            try {
                std::vector<uint8_t> bytes(data.begin(), data.end());
                auto sha256Hash = sha256Raw(bytes);
                auto ripemdHash = ripemd160Raw(sha256Hash);
                std::string result = bytesToHex(ripemdHash);
                Crypto::Utils::Logger::getInstance()->debug("Hash160 computed for address generation");
                return result;
            } catch (const std::exception& e) {
                Crypto::Utils::Logger::getInstance()->error("Hash160 computation failed: " + std::string(e.what()));
                throw;
            }
        }

        // Merkle tree root calculation
        std::string Hash::merkleRoot(const std::vector<std::string>& hashes) {
            try {
                if (hashes.empty()) {
                    Crypto::Utils::Logger::getInstance()->warning("Empty hash list provided for Merkle root");
                    return "";
                }
                
                if (hashes.size() == 1) {
                    Crypto::Utils::Logger::getInstance()->debug("Single hash Merkle root: " + hashes[0]);
                    return hashes[0];
                }
                
                std::vector<std::string> nextLevel;
                
                // Process pairs of hashes
                for (size_t i = 0; i < hashes.size(); i += 2) {
                    std::string left = hashes[i];
                    std::string right;
                    
                    // If odd number of hashes, duplicate the last one
                    if (i + 1 < hashes.size()) {
                        right = hashes[i + 1];
                    } else {
                        right = left;
                        Crypto::Utils::Logger::getInstance()->debug("Duplicating last hash for Merkle tree");
                    }
                    
                    // Combine left and right hashes and hash them
                    std::string combined = left + right;
                    std::string hashedPair = sha256d(combined);
                    nextLevel.push_back(hashedPair);
                }
                
                Crypto::Utils::Logger::getInstance()->debug("Merkle tree level processed: " + 
                    std::to_string(hashes.size()) + " -> " + std::to_string(nextLevel.size()));
                
                // Recursive call until we get single root
                return merkleRoot(nextLevel);
            } catch (const std::exception& e) {
                Crypto::Utils::Logger::getInstance()->error("Merkle root calculation failed: " + std::string(e.what()));
                throw;
            }
        }

        // Convert bytes to hex string
        std::string Hash::bytesToHex(const std::vector<uint8_t>& bytes) {
            try {
                std::stringstream ss;
                ss << std::hex << std::setfill('0');
                
                for (const auto& byte : bytes) {
                    ss << std::setw(2) << static_cast<int>(byte);
                }
                
                return ss.str();
            } catch (const std::exception& e) {
                Crypto::Utils::Logger::getInstance()->error("Bytes to hex conversion failed: " + std::string(e.what()));
                throw;
            }
        }

        // Convert hex string to bytes
        std::vector<uint8_t> Hash::hexToBytes(const std::string& hex) {
            try {
                if (hex.length() % 2 != 0) {
                    throw std::invalid_argument("Hex string must have even length");
                }
                
                std::vector<uint8_t> bytes;
                bytes.reserve(hex.length() / 2);
                
                for (size_t i = 0; i < hex.length(); i += 2) {
                    std::string byteString = hex.substr(i, 2);
                    auto byte = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
                    bytes.push_back(byte);
                }
                
                Crypto::Utils::Logger::getInstance()->debug("Converted hex string to " + std::to_string(bytes.size()) + " bytes");
                return bytes;
            } catch (const std::exception& e) {
                Crypto::Utils::Logger::getInstance()->error("Hex to bytes conversion failed: " + std::string(e.what()));
                throw;
            }
        }

        // Private: Raw SHA-256 implementation using OpenSSL
        std::vector<uint8_t> Hash::sha256Raw(const std::vector<uint8_t>& data) {
            std::vector<uint8_t> hash(SHA256_DIGEST_LENGTH);
            
            SHA256_CTX sha256Context;
            if (SHA256_Init(&sha256Context) != 1) {
                Crypto::Utils::Logger::getInstance()->critical("Failed to initialize SHA256 context");
                throw std::runtime_error("Failed to initialize SHA256 context");
            }
            
            if (SHA256_Update(&sha256Context, data.data(), data.size()) != 1) {
                Crypto::Utils::Logger::getInstance()->critical("Failed to update SHA256 hash");
                throw std::runtime_error("Failed to update SHA256 hash");
            }
            
            if (SHA256_Final(hash.data(), &sha256Context) != 1) {
                Crypto::Utils::Logger::getInstance()->critical("Failed to finalize SHA256 hash");
                throw std::runtime_error("Failed to finalize SHA256 hash");
            }
            
            return hash;
        }

        // Private: Raw RIPEMD-160 implementation using OpenSSL
        std::vector<uint8_t> Hash::ripemd160Raw(const std::vector<uint8_t>& data) {
            std::vector<uint8_t> hash(RIPEMD160_DIGEST_LENGTH);
            
            RIPEMD160_CTX ripemdContext;
            if (RIPEMD160_Init(&ripemdContext) != 1) {
                Crypto::Utils::Logger::getInstance()->critical("Failed to initialize RIPEMD160 context");
                throw std::runtime_error("Failed to initialize RIPEMD160 context");
            }
            
            if (RIPEMD160_Update(&ripemdContext, data.data(), data.size()) != 1) {
                Crypto::Utils::Logger::getInstance()->critical("Failed to update RIPEMD160 hash");
                throw std::runtime_error("Failed to update RIPEMD160 hash");
            }
            
            if (RIPEMD160_Final(hash.data(), &ripemdContext) != 1) {
                Crypto::Utils::Logger::getInstance()->critical("Failed to finalize RIPEMD160 hash");
                throw std::runtime_error("Failed to finalize RIPEMD160 hash");
            }
            
            return hash;
        }

    } // namespace SHA256
} // namespace Crypto