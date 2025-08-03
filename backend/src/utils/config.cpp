#include "utils/Config.h"
#include "utils/Logger.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace Crypto {
    namespace Utils {

        std::unique_ptr<Config> Config::instance = nullptr;
        std::mutex Config::mutex_;

        Config::Config() {
            Logger::getInstance()->log(LogLevel::INFO, "Config initialized.");
        }

        Config::~Config() {
            Logger::getInstance()->log(LogLevel::INFO, "Config destroyed.");
        }

        Config* Config::getInstance() {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!instance) {
                instance = std::unique_ptr<Config>(new Config());
            }
            return instance.get();
        }

        bool Config::loadConfig(const std::string& filePath) {
            std::ifstream file(filePath);
            if (!file.is_open()) {
                Logger::getInstance()->log(LogLevel::ERROR, "Failed to open config file: " + filePath);
                return false;
            }

            try {
                json parsed;
                file >> parsed;
                getInstance()->configData = parsed;
                Logger::getInstance()->log(LogLevel::INFO, "Config loaded from " + filePath);
                return true;
            } catch (const std::exception& e) {
                Logger::getInstance()->log(LogLevel::ERROR, std::string("Config parse error: ") + e.what());
                return false;
            }
        }

        json Config::getValueByKey(const std::string& key) {
            json* current = &getInstance()->configData;
            std::istringstream ss(key);
            std::string part;

            while (std::getline(ss, part, '.')) {
                if (!current->contains(part)) return nullptr;
                current = &(*current)[part];
            }
            return *current;
        }

        std::string Config::getString(const std::string& key) {
            json val = getValueByKey(key);
            return val.is_string() ? val.get<std::string>() : "";
        }

        int Config::getInt(const std::string& key) {
            json val = getValueByKey(key);
            return val.is_number_integer() ? val.get<int>() : 0;
        }

        bool Config::getBool(const std::string& key) {
            json val = getValueByKey(key);
            return val.is_boolean() ? val.get<bool>() : false;
        }

    } // namespace Utils
} // namespace Crypto
