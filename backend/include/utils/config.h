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

            Config();

            // helper fucntion to validate the JSON Structure
            bool validateConfig(const json& config);

        public: 
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;

            // Singleton Instance 
            static Config* getInstance();

            // Load configuration from a file
            bool loadFromFile(const std::string& filePath);

            // Save configuration to a file
            bool saveToFile(const std::string& filePath);

            // Get configuration values with different types
            std::string getString(const std::string& key, const std::string& defaultValue = "");
            int getInt(const std::string& key, int defaultValue = 0);
            double getDouble(const std::string& key, double defaultValue = 0.0);
            bool getBool(const std::string& key, bool defaultValue = false);
            std::vector<std::string> getStringArray(const std::string& key);

            // check key existence 
            bool hasKey(const std::string& key);

            // Get nested values using dot notation
            json getValue(const std::string& key);
            void setValue(const std::string& key, const json& value);

            // Get all configuration as JSON
            json getAllConfig();

            // reload configuration from the file
            bool reload();

            // validate current configuration
            bool validate();

            // Get configuration file path
            std::string getConfigFilePath() const;  

            ~Config() = default;

        };

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