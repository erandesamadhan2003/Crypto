#ifndef JSONHELPER_H
#define JSONHELPER_H
#include <nlohmann/json.hpp>
namespace Crypto {
    namespace Utils {

        // Custom exception for JSON errors
        class JSONException : public std::runtime_error {
        public:
            explicit JSONException(const std::string& message) 
                : std::runtime_error("JSON Error: " + message) {}
        };

        class JSONHelper {
            using json = nlohmann::json;
        public:
            // CONVERT JSON FROM STRING
            static json parseFromString(const std::string& jsonString);

            // CONVERT JSON TO PRETTY STRING
            static std::string toString(const json& j, int indent = 4);

            // CONVERT JSON TO COMPACT STRING
            static std::string toCompactString(const json& j);

            // VALIDATE JSON STRUCTURE
            static bool isValidJSON(const std::string& jsonString);

            // FILE OPERATIONS
            static json loadFromFile(const std::string& filePath);
            static void saveToFile(const json& j, const std::string& filePath, int indent = 4);

            // GETTERS FOR COMMON TYPES
            static std::string getString(const json& jsonObj, const std::string& key, const std::string& defaultValue = "");
            static int getInt(const json& jsonObj, const std::string& key, int defaultValue = 0);
            static bool getBool(const json& jsonObj, const std::string& key, bool defaultValue = false);

            // SAFE NESTED ACCESS
            static json getNestedValue(const json& jsonObj, const std::string& path);

            static bool hasKey(const json& jsonObj, const std::string& key);
            static bool hasNestedKey(const json& jsonObj, const std::string& path);

            // REQUIRED FIELDS VALIDATION
            static void validateRequiredFields(const json& jsonObj, const std::vector<std::string>& requiredFields);

            // BLOCKCHAIN-SPECIFIC HELPER
            static json createErrorResponse(int code, const std::string& message, const std::string& details = "");
            static json createSuccessResponse(const json& data, const std::string& message = "Success");

            static json createTransactionJSON(const std::string& txid, const std::string& from, const std::string& to, double amount, uint64_t timestamp);

            static json createBlockJSON(const std::string& hash, const std::string& previousHash, const std::vector<std::string>& transactions, uint64_t timestamp, uint32_t nonce, double difficulty);

            // UTILITY FUNCTIONS
            static std::string generateUUID();
            static uint64_t getCurrentTimestamp();

        private:
            // HELPER
            static std::vector<std::string> splitPath(const std::string& path);
            static json navigateToPath(const json& jsonObj, const std::vector<std::string>& pathParts);

        };
    } // namespace Utils
} // namespace Crypto

#endif