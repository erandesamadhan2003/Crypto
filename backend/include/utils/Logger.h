#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace Crypto {
    namespace Utils {

        enum class LogLevel {
            DEBUG = 0,
            INFO = 1,
            WARNING = 2,
            ERROR = 3,
            CRITICAL = 4
        };

        class Logger {
        private:
            static std::unique_ptr<Logger> instance;
            static std::mutex mutex_;

            std::ofstream logFile;
            LogLevel currentLogLevel;
            std::mutex fileMutex;
            bool consoleOutput;
            std::string logFilePath;
            
            Logger();

            std::string getCurrentTimeStamp();
            std::string logLevelToString(LogLevel level);

        public:     
            Logger(const Logger&) = delete;
            Logger& operator=(const Logger&) = delete;

            static Logger* getInstance();

            bool initialize(const std::string& filePath, LogLevel level = LogLevel::INFO, bool enableConsole = true);

            void setLogLevel(LogLevel level);

            void debug(const std::string& message);
            void info(const std::string& message);
            void warning(const std::string& message);
            void error(const std::string& message);
            void critical(const std::string& message);

            void log(LogLevel level, const std::string& message);

            void close();

            ~Logger();
        };

    #define LOG_DEBUG(msg)    Crypto::Utils::Logger::getInstance()->debug(msg)
    #define LOG_INFO(msg)     Crypto::Utils::Logger::getInstance()->info(msg)
    #define LOG_WARNING(msg)  Crypto::Utils::Logger::getInstance()->warning(msg)
    #define LOG_ERROR(msg)    Crypto::Utils::Logger::getInstance()->error(msg)

} // namespace Utils
} // namespace Crypto

#endif // LOGGER_H