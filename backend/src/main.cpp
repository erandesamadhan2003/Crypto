#include "utils/Logger.h"
#include "utils/Config.h"
#include <string>
#include <iostream>

int main() {
    using namespace Crypto::Utils;

    Logger* logger = Logger::getInstance();
    logger->log(LogLevel::INFO, "Starting Config Test");

    const std::string configPath = "../config/sample_config.json";

    if (!Config::loadConfig(configPath)) {
        logger->log(LogLevel::CRITICAL, "Failed to load configuration. Exiting.");
        return 1;
    }

    int answer = Config::getInt("answer.everything");
    std::string greeting = Config::getString("greeting.message");
    bool debug = Config::getBool("debug.enabled");

    logger->log(LogLevel::INFO, "Answer to Everything: " + std::to_string(answer));
    logger->log(LogLevel::INFO, "Greeting: " + greeting);
    logger->log(LogLevel::INFO, std::string("Debug Mode: ") + (debug ? "ON" : "OFF"));

    return 0;
}
