#include "utils/Logger.h"
#include "utils/Config.h"
#include <string>
#include <iostream>

int main() {
    using namespace Crypto::Utils;

    Logger* logger = Logger::getInstance();
    LOG_INFO("STARTING CONFIG TEST");

    const std::string configPath = "../config/sample_config.json";

    if (!Config::loadConfig(configPath)) {
        LOG_ERROR("Failed to load configuration from " + configPath);
        return 1;
    }

    int answer = Config::getInt("answer.everything");
    std::string greeting = Config::getString("greeting.message");
    bool debug = Config::getBool("debug.enabled");

    LOG_INFO( "Answer to Everything: " + std::to_string(answer));
    LOG_INFO("Greeting: " + greeting);
    LOG_INFO(std::string("Debug Mode: ") + (debug ? "ON" : "OFF"));

    return 0;
}
