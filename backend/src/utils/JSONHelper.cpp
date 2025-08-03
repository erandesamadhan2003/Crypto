#include "utils/JSONHelper.h"
#include "utils/Logger.h"

#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <iomanip>

namespace Crypto {
    namespace Utils {

        using json = nlohmann::json;

        json JSONHelper::parseFromString(const std::string& jsonString) {
            try {
                return json::parse(jsonString);
            } catch (const json::parse_error& e) {
                std::string errorMsg = "Failed to parse JSON: " + std::string(e.what());
                LOG_ERROR(errorMsg);
                throw JSONException(errorMsg);
            }
        }

        std::string JSONHelper::toString(const json& j, int indent) {
            try {
                return j.dump(indent);
            } catch (const json::exception& e) {
                std::string errorMsg = "Failed to convert JSON to string: " + std::string(e.what());
                LOG_ERROR(errorMsg);
                throw JSONException(errorMsg);
            }
        }

        std::string JSONHelper::toCompactString(const json& j) {
            return toString(j, -1); // Compact, no indentation
        }

        bool JSONHelper::isValidJSON(const std::string& jsonString) {
            try {
                json::parse(jsonString);
                return true;
            } catch (...) {
                return false;
            }
        }

        json JSONHelper::loadFromFile(const std::string& filePath) {
            try {
                std::ifstream file(filePath);
                if (!file.is_open()) {
                    throw JSONException("Cannot open file: " + filePath);
                }
                json result;
                file >> result;
                return result;
            } catch (const std::exception& e) {
                throw JSONException("Error loading JSON from file: " + std::string(e.what()));
            }
        }

        void JSONHelper::saveToFile(const json& j, const std::string& filePath, int indent) {
            try {
                std::ofstream file(filePath);
                if (!file.is_open()) {
                    throw JSONException("Cannot open file for writing: " + filePath);
                }
                file << j.dump(indent);
            } catch (const std::exception& e) {
                throw JSONException("Error saving JSON to file: " + std::string(e.what()));
            }
        }

        std::string JSONHelper::getString(const json& jsonObj, const std::string& key, const std::string& defaultValue) {
            try {
                if (jsonObj.contains(key) && jsonObj[key].is_string()) {
                    return jsonObj[key].get<std::string>();
                }
                return defaultValue;
            } catch (...) {
                return defaultValue;
            }
        }

        int JSONHelper::getInt(const json& jsonObj, const std::string& key, int defaultValue) {
            try {
                if (jsonObj.contains(key) && jsonObj[key].is_number_integer()) {
                    return jsonObj[key].get<int>();
                }
                return defaultValue;
            } catch (...) {
                return defaultValue;
            }
        }

        bool JSONHelper::getBool(const json& jsonObj, const std::string& key, bool defaultValue) {
            try {
                if (jsonObj.contains(key) && jsonObj[key].is_boolean()) {
                    return jsonObj[key].get<bool>();
                }
                return defaultValue;
            } catch (...) {
                return defaultValue;
            }
        }

        std::vector<std::string> JSONHelper::splitPath(const std::string& path) {
            std::vector<std::string> parts;
            std::stringstream ss(path);
            std::string token;
            while (std::getline(ss, token, '.')) {
                if (!token.empty()) {
                    parts.push_back(token);
                }
            }
            return parts;
        }

        json JSONHelper::navigateToPath(const json& jsonObj, const std::vector<std::string>& pathParts) {
            json current = jsonObj;
            for (const auto& part : pathParts) {
                if (!current.contains(part)) return nullptr;
                current = current[part];
            }
            return current;
        }

        json JSONHelper::getNestedValue(const json& jsonObj, const std::string& path) {
            try {
                auto parts = splitPath(path);
                return navigateToPath(jsonObj, parts);
            } catch (...) {
                return nullptr;
            }
        }

        bool JSONHelper::hasKey(const json& jsonObj, const std::string& key) {
            return jsonObj.contains(key);
        }

        bool JSONHelper::hasNestedKey(const json& jsonObj, const std::string& path) {
            return !getNestedValue(jsonObj, path).is_null();
        }

        void JSONHelper::validateRequiredFields(const json& jsonObj, const std::vector<std::string>& requiredFields) {
            for (const auto& field : requiredFields) {
                if (!jsonObj.contains(field)) {
                    throw JSONException("Missing required field: " + field);
                }
            }
        }

        json JSONHelper::createErrorResponse(int code, const std::string& message, const std::string& details) {
            json res = {
                {"success", false},
                {"error", {
                    {"code", code},
                    {"message", message}
                }},
                {"timestamp", getCurrentTimestamp()}
            };
            if (!details.empty()) {
                res["error"]["details"] = details;
            }
            return res;
        }

        json JSONHelper::createSuccessResponse(const json& data, const std::string& message) {
            return {
                {"success", true},
                {"message", message},
                {"data", data},
                {"timestamp", getCurrentTimestamp()}
            };
        }

        json JSONHelper::createTransactionJSON(const std::string& txid, const std::string& from, const std::string& to, double amount, uint64_t timestamp) {
            return {
                {"txid", txid},
                {"from", from},
                {"to", to},
                {"amount", amount},
                {"timestamp", timestamp}
            };
        }

        json JSONHelper::createBlockJSON(const std::string& hash, const std::string& previousHash, const std::vector<std::string>& transactions, uint64_t timestamp, uint32_t nonce, double difficulty) {
            return {
                {"hash", hash},
                {"previousHash", previousHash},
                {"transactions", transactions},
                {"timestamp", timestamp},
                {"nonce", nonce},
                {"difficulty", difficulty}
            };
        }

        std::string JSONHelper::generateUUID() {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<> dis(0, 15);
            static std::uniform_int_distribution<> dis2(8, 11);
            
            std::stringstream ss;
            ss << std::hex;
            
            for (int i = 0; i < 8; i++) ss << dis(gen);
            ss << "-";
            for (int i = 0; i < 4; i++) ss << dis(gen);
            ss << "-4";
            for (int i = 0; i < 3; i++) ss << dis(gen);
            ss << "-";
            ss << dis2(gen);
            for (int i = 0; i < 3; i++) ss << dis(gen);
            ss << "-";
            for (int i = 0; i < 12; i++) ss << dis(gen);
            
            return ss.str();
        }

        uint64_t JSONHelper::getCurrentTimestamp() {
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        }

    } // namespace Utils
} // namespace Crypto
