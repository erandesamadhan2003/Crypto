#include "utils/Logger.h"
#include "utils/Config.h"
#include <string>
#include <iostream>
#include "crypto/hash.h"
int main() {
    using namespace Crypto::Utils;

    Logger* logger = Logger::getInstance();
    LOG_INFO("STARTING CONFIG TEST");

    const std::string configPath = "../config/blockchain_config.json";

    if (!Config::loadConfig(configPath)) {
        LOG_ERROR("Failed to load configuration from " + configPath);
        return 1;
    }

    std::string networkName = Config::getString("blockchain.networkName");
    int targetBlockTime = Config::getInt("blockchain.targetBlockTime");
    int maxBlockSize = Config::getInt("blockchain.maxBlockSize");

    int port = Config::getInt("network.port");
    std::string bindAddress = Config::getString("network.bindAddress");
    int maxConnections = Config::getInt("network.maxConnections");

    bool enableMining = Config::getBool("mining.enableMining");
    int threadCount = Config::getInt("mining.threadCount");

    LOG_INFO("Blockchain Network Name: " + networkName);
    LOG_INFO("Target Block Time: " + std::to_string(targetBlockTime));
    LOG_INFO("Max Block Size: " + std::to_string(maxBlockSize));

    LOG_INFO("Network Port: " + std::to_string(port));
    LOG_INFO("Bind Address: " + bindAddress);
    LOG_INFO("Max Connections: " + std::to_string(maxConnections));

    LOG_INFO("Mining Enabled: " + std::string(enableMining ? "Yes" : "No"));
    LOG_INFO("Mining Threads: " + std::to_string(threadCount));

    std::string data = "Hello Blockchain!";
    std::string hash = Crypto::SHA256::Hash::sha256(data);
    LOG_INFO("Hashed Data: " + hash);

    return 0;
}
