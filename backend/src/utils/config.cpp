#include "utils/config.h"
#include "utils/Logger.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace Crypto {
    namespace Utils {

        std::unique_ptr<Config> Config::instance = nullptr;
        std::mutex Config::mutex_;

        Config::Config() {};

        Config* Config::getInstance() {
            std::lock_guard<std::mutex> lock(mutex_);

            if(!instance) 
                instance = std::unique_ptr<Config>(new Config());
            
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

                if(!validateConfig(configData)) {
                    LOG_ERROR("Invalid JSON structure in config file: " + filePath);
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

                if(!file.is_open()) {
                    LOG_ERROR("Failed to open config file for writing: " + targetPath);
                    return false;
                }

                file << configData.dump(5);
                LOG_INFO("Configuration saved successfully to: " + targetPath);
                return true; 
            } catch(const std::exception& e) {
                LOG_ERROR("Error saving config file: " + std::string(e.what()));
                return false;
            }
        }

    }
}
