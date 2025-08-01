#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>
#include <memory>
#include <mutex>
#include <vector>
#include <nlohmann/json.hpp>

namespace Crypto {
    namespace Utils {
        using json = nlohmann::json;

        class Config {
        private:
            static std::unique_ptr<Config> instance;
            static std::mutex mutex_;
            
            json configData;
            std::string configFilePath;
            std::mutex configMutex;

            // Private constructor
            Config();

            // Helper method to validate JSON structure
            bool validateConfig(const json& config);

        public:
            // Singleton pattern
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;

            // Get singleton instance
            static Config* getInstance();

            // Load configuration from file
            bool loadFromFile(const std::string& filePath);

            // Save configuration to file
            bool saveToFile(const std::string& filePath = "");

            // Get configuration values with different types
            std::string getString(const std::string& key, const std::string& defaultValue = "");
            int getInt(const std::string& key, int defaultValue = 0);
            double getDouble(const std::string& key, double defaultValue = 0.0);
            bool getBool(const std::string& key, bool defaultValue = false);
            std::vector<std::string> getStringArray(const std::string& key);

            // Set configuration values
            void setString(const std::string& key, const std::string& value);
            void setInt(const std::string& key, int value);
            void setDouble(const std::string& key, double value);
            void setBool(const std::string& key, bool value);
            void setStringArray(const std::string& key, const std::vector<std::string>& value);

            // Check if key exists
            bool hasKey(const std::string& key);

            // Get nested values using dot notation (e.g., "network.port")
            json getValue(const std::string& key);
            void setValue(const std::string& key, const json& value);

            // Get all configuration as JSON
            json getAllConfig();

            // Reload configuration from file
            bool reload();

            // Create default configuration
            void createDefaultConfig();

            // Validate current configuration
            bool validate();

            // Get configuration file path
            std::string getConfigFilePath() const;

            // Destructor
            ~Config() = default;
        };

        // Configuration structure for blockchain parameters
        struct BlockchainConfig {
            std::string networkName;
            int targetBlockTime;          // in seconds
            int difficultyAdjustmentPeriod; // in blocks
            int maxBlockSize;             // in bytes
            double initialDifficulty;
            std::string genesisBlockHash;
            int blockReward;              // in satoshis
            int halvingPeriod;            // in blocks
        };

        // Configuration structure for network parameters
        struct NetworkConfig {
            std::string bindAddress;
            int port;
            int maxConnections;
            int connectionTimeout;        // in seconds
            int heartbeatInterval;        // in seconds
            std::vector<std::string> seedNodes;
            bool enableUpnp;
            std::string userAgent;
        };

        // Configuration structure for mining parameters
        struct MiningConfig {
            bool enableMining;
            int threadCount;
            std::string minerAddress;
            int hashrateCheckInterval;    // in seconds
            bool logMiningStats;
            double minFeeRate;            // in satoshis per byte
        };

        // Helper functions to load specific configurations
        BlockchainConfig loadBlockchainConfig();
        NetworkConfig loadNetworkConfig();
        MiningConfig loadMiningConfig();

    }
}

#endif