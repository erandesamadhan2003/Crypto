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

            static Logger* getInstance();
            void log(LogLevel level, const std::string& message);
            ~Logger();
        };
    }
}

#endif // LOGGER_H
