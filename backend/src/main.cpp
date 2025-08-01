#include "utils/config.h"
#include "utils/Logger.h"
#include <iostream>

// using namespace Crypto::Utils;

#include <filesystem>

int main() {

    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    
    // Initialize Logger
    Crypto::Utils::Logger* logger = Crypto::Utils::Logger::getInstance();
    logger->initialize("/run/media/samadhan/sda3/Projects/crypto/backend/logs/blockchain.log", 
                  Crypto::Utils::LogLevel::INFO, true);

    if (!logger->initialize("logs/blockchain.log", Crypto::Utils::LogLevel::INFO, true)) {
        std::cerr << "Failed to initialize logger!" << std::endl;
        return 1;
    }
       
    // Initialize Config
    Crypto::Utils::Config* config = Crypto::Utils::Config::getInstance();
    config->createDefaultConfig();
    config->saveToFile("config/blockchain_config.json");
    
    // Use logging
    LOG_INFO("Blockchain node starting...");
    LOG_DEBUG("Debug information");
    LOG_ERROR("Error occurred");
    
    // Use configuration
    int port = config->getInt("network.port", 8333);
    std::string networkName = config->getString("blockchain.networkName");
    
    return 0;
}