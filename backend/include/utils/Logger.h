#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <mutex>
#include <string>

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
            static std::unique_ptr<Logger> instance;
            static std::mutex mutex_;
            static std::string logLevelToString(LogLevel level);
            static std::string getCurrentTime();

            Logger();

        public: 
            Logger(const Logger&) = delete;
            Logger& operator=(const Logger&) = delete;

            void debug(const std::string& message);
            void info(const std::string& message);
            void warning(const std::string& message);
            void error(const std::string& message);
            void critical(const std::string& message);


            static Logger* getInstance();
            void log(LogLevel level, const std::string& message);
            ~Logger();
        };

        #define LOG_DEBUG(msg) Crypto::Utils::Logger::getInstance()->debug(msg)
        #define LOG_INFO(msg) Crypto::Utils::Logger::getInstance()->info(msg)
        #define LOG_WARNING(msg) Crypto::Utils::Logger::getInstance()->warning(msg)
        #define LOG_ERROR(msg) Crypto::Utils::Logger::getInstance()->error(msg)
        #define LOG_CRITICAL(msg) Crypto::Utils::Logger::getInstance()->critical(msg)
    }
    
}

#endif // LOGGER_H
