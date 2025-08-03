#ifndef CONFIG_H
#define CONFIG_H

#include <nlohmann/json.hpp>
#include <mutex>
#include <memory>
#include <string>

namespace Crypto {
    namespace Utils {
        using json = nlohmann::json;

        class Config {
        private:
            static std::unique_ptr<Config> instance;
            static std::mutex mutex_;

            json configData;
            Config();

        public:
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;

            static Config* getInstance();
            static bool loadConfig(const std::string& filePath);

            static std::string getString(const std::string& key);
            static int getInt(const std::string& key);
            static bool getBool(const std::string& key);

            static json getValueByKey(const std::string& key);
            ~Config();
        };
    }
}

#endif
