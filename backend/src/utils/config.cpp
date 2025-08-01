#include "utils/config.h"
#include "utils/Logger.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace Crypto {
    namespace Utils {

        // Static member definitions
        std::unique_ptr<Config> Config::instance = nullptr;
        std::mutex Config::mutex_;

        Config::Config() {}

        Config* Config::getInstance() {
            std::lock_guard<std::mutex> lock(mutex_);
            if (instance == nullptr) {
                instance = std::unique_ptr<Config>(new Config());
            }
            return instance.get();
        }

        bool Config::loadFromFile(const std::string& filePath) {
            std::lock_guard<std::mutex> lock(configMutex);
            
            try {
                std::ifstream file(filePath);
                if (!file.is_open()) {
                    LOG_ERROR("Failed to open config file: " + filePath);
                    return false;
                }

                file >> configData;
                configFilePath = filePath;
                
                if (!validateConfig(configData)) {
                    LOG_ERROR("Invalid configuration structure in file: " + filePath);
                    return false;
                }

                LOG_INFO("Configuration loaded successfully from: " + filePath);
                return true;

            } catch (const json::exception& e) {
                LOG_ERROR("JSON parsing error in config file: " + std::string(e.what()));
                return false;
            } catch (const std::exception& e) {
                LOG_ERROR("Error loading config file: " + std::string(e.what()));
                return false;
            }
        }

        bool Config::saveToFile(const std::string& filePath) {
            std::lock_guard<std::mutex> lock(configMutex);
            
            std::string targetPath = filePath.empty() ? configFilePath : filePath;
            
            try {
                std::ofstream file(targetPath);
                if (!file.is_open()) {
                    LOG_ERROR("Failed to open config file for writing: " + targetPath);
                    return false;
                }

                file << configData.dump(4); // Pretty print with 4 spaces
                LOG_INFO("Configuration saved to: " + targetPath);
                return true;

            } catch (const std::exception& e) {
                LOG_ERROR("Error saving config file: " + std::string(e.what()));
                return false;
            }
        }

        json Config::getValue(const std::string& key) {
            std::lock_guard<std::mutex> lock(configMutex);
            
            try {
                // Split key by dots for nested access
                std::vector<std::string> keys;
                std::stringstream ss(key);
                std::string item;
                
                while (std::getline(ss, item, '.')) {
                    keys.push_back(item);
                }

                json current = configData;
                for (const auto& k : keys) {
                    if (current.contains(k)) {
                        current = current[k];
                    } else {
                        return json();
                    }
                }
                
                return current;
            } catch (const json::exception& e) {
                LOG_ERROR("Error accessing config key '" + key + "': " + e.what());
                return json();
            }
        }

        void Config::setValue(const std::string& key, const json& value) {
            std::lock_guard<std::mutex> lock(configMutex);
            
            try {
                // Split key by dots for nested access
                std::vector<std::string> keys;
                std::stringstream ss(key);
                std::string item;
                
                while (std::getline(ss, item, '.')) {
                    keys.push_back(item);
                }

                json* current = &configData;
                for (size_t i = 0; i < keys.size() - 1; ++i) {
                    if (!current->contains(keys[i])) {
                        (*current)[keys[i]] = json::object();
                    }
                    current = &(*current)[keys[i]];
                }
                
                (*current)[keys.back()] = value;
                
            } catch (const json::exception& e) {
                LOG_ERROR("Error setting config key '" + key + "': " + e.what());
            }
        }

        std::string Config::getString(const std::string& key, const std::string& defaultValue) {
            json value = getValue(key);
            return value.is_string() ? value.get<std::string>() : defaultValue;
        }

        int Config::getInt(const std::string& key, int defaultValue) {
            json value = getValue(key);
            return value.is_number_integer() ? value.get<int>() : defaultValue;
        }

        double Config::getDouble(const std::string& key, double defaultValue) {
            json value = getValue(key);
            return value.is_number() ? value.get<double>() : defaultValue;
        }

        bool Config::getBool(const std::string& key, bool defaultValue) {
            json value = getValue(key);
            return value.is_boolean() ? value.get<bool>() : defaultValue;
        }

        std::vector<std::string> Config::getStringArray(const std::string& key) {
            json value = getValue(key);
            std::vector<std::string> result;
            
            if (value.is_array()) {
                for (const auto& item : value) {
                    if (item.is_string()) {
                        result.push_back(item.get<std::string>());
                    }
                }
            }
            
            return result;
        }

        void Config::setString(const std::string& key, const std::string& value) {
            setValue(key, json(value));
        }

        void Config::setInt(const std::string& key, int value) {
            setValue(key, json(value));
        }

        void Config::setDouble(const std::string& key, double value) {
            setValue(key, json(value));
        }

        void Config::setBool(const std::string& key, bool value) {
            setValue(key, json(value));
        }

        void Config::setStringArray(const std::string& key, const std::vector<std::string>& value) {
            setValue(key, json(value));
        }

        bool Config::hasKey(const std::string& key) {
            return !getValue(key).is_null();
        }

        json Config::getAllConfig() {
            std::lock_guard<std::mutex> lock(configMutex);
            return configData;
        }

        bool Config::reload() {
            if (configFilePath.empty()) {
                LOG_ERROR("No config file path set for reload");
                return false;
            }
            return loadFromFile(configFilePath);
        }

        void Config::createDefaultConfig() {
            std::lock_guard<std::mutex> lock(configMutex);
            
            configData = json::object();
            
            // Blockchain configuration
            configData["blockchain"]["networkName"] = "MyBlockchain";
            configData["blockchain"]["targetBlockTime"] = 600; // 10 minutes
            configData["blockchain"]["difficultyAdjustmentPeriod"] = 2016;
            configData["blockchain"]["maxBlockSize"] = 1048576; // 1MB
            configData["blockchain"]["initialDifficulty"] = 1.0;
            configData["blockchain"]["blockReward"] = 5000000000; // 50 coins
            configData["blockchain"]["halvingPeriod"] = 210000;
            
            // Network configuration
            configData["network"]["bindAddress"] = "0.0.0.0";
            configData["network"]["port"] = 8333;
            configData["network"]["maxConnections"] = 125;
            configData["network"]["connectionTimeout"] = 30;
            configData["network"]["heartbeatInterval"] = 60;
            configData["network"]["seedNodes"] = json::array();
            configData["network"]["enableUpnp"] = true;
            configData["network"]["userAgent"] = "MyBlockchain/1.0.0";
            
            // Mining configuration
            configData["mining"]["enableMining"] = false;
            configData["mining"]["threadCount"] = 1;
            configData["mining"]["minerAddress"] = "";
            configData["mining"]["hashrateCheckInterval"] = 5;
            configData["mining"]["logMiningStats"] = true;
            configData["mining"]["minFeeRate"] = 1.0;
            
            // API configuration
            configData["api"]["enableAPI"] = true;
            configData["api"]["port"] = 8334;
            configData["api"]["bindAddress"] = "127.0.0.1";
            configData["api"]["enableCORS"] = true;
            
            // Logging configuration
            configData["logging"]["level"] = "INFO";
            configData["logging"]["file"] = "logs/blockchain.log";
            configData["logging"]["maxFileSize"] = 10485760; // 10MB
            configData["logging"]["enableConsole"] = true;
            
            LOG_INFO("Default configuration created");
        }

        bool Config::validateConfig(const json& config) {
            try {
                // Check required sections
                if (!config.contains("blockchain") || !config.contains("network") || 
                    !config.contains("mining")) {
                    return false;
                }
                
                // Validate blockchain section
                const auto& blockchain = config["blockchain"];
                if (!blockchain.contains("networkName") || !blockchain.contains("targetBlockTime")) {
                    return false;
                }
                
                // Validate network section
                const auto& network = config["network"];
                if (!network.contains("port") || !network.contains("bindAddress")) {
                    return false;
                }
                
                return true;
            } catch (const json::exception&) {
                return false;
            }
        }

        bool Config::validate() {
            std::lock_guard<std::mutex> lock(configMutex);
            return validateConfig(configData);
        }

        std::string Config::getConfigFilePath() const {
            return configFilePath;
        }

        // Helper functions implementation
        BlockchainConfig loadBlockchainConfig() {
            Config* config = Config::getInstance();
            BlockchainConfig bc;
            
            bc.networkName = config->getString("blockchain.networkName", "MyBlockchain");
            bc.targetBlockTime = config->getInt("blockchain.targetBlockTime", 600);
            bc.difficultyAdjustmentPeriod = config->getInt("blockchain.difficultyAdjustmentPeriod", 2016);
            bc.maxBlockSize = config->getInt("blockchain.maxBlockSize", 1048576);
            bc.initialDifficulty = config->getDouble("blockchain.initialDifficulty", 1.0);
            bc.genesisBlockHash = config->getString("blockchain.genesisBlockHash", "");
            bc.blockReward = config->getInt("blockchain.blockReward", 5000000000LL);
            bc.halvingPeriod = config->getInt("blockchain.halvingPeriod", 210000);
            
            return bc;
        }

        NetworkConfig loadNetworkConfig() {
            Config* config = Config::getInstance();
            NetworkConfig nc;
            
            nc.bindAddress = config->getString("network.bindAddress", "0.0.0.0");
            nc.port = config->getInt("network.port", 8333);
            nc.maxConnections = config->getInt("network.maxConnections", 125);
            nc.connectionTimeout = config->getInt("network.connectionTimeout", 30);
            nc.heartbeatInterval = config->getInt("network.heartbeatInterval", 60);
            nc.seedNodes = config->getStringArray("network.seedNodes");
            nc.enableUpnp = config->getBool("network.enableUpnp", true);
            nc.userAgent = config->getString("network.userAgent", "MyBlockchain/1.0.0");
            
            return nc;
        }

        MiningConfig loadMiningConfig() {
            Config* config = Config::getInstance();
            MiningConfig mc;
            
            mc.enableMining = config->getBool("mining.enableMining", false);
            mc.threadCount = config->getInt("mining.threadCount", 1);
            mc.minerAddress = config->getString("mining.minerAddress", "");
            mc.hashrateCheckInterval = config->getInt("mining.hashrateCheckInterval", 5);
            mc.logMiningStats = config->getBool("mining.logMiningStats", true);
            mc.minFeeRate = config->getDouble("mining.minFeeRate", 1.0);
            
            return mc;
        }

    } // namespace Utils
} // namespace MyBlockchain