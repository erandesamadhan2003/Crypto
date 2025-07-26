#include "utils/Logger.h"
#include <filesystem>

namespace Crypto {
    namespace Utils {
        
        std::unique_ptr<Logger> Logger::instance = nullptr;
        std::mutex Logger::mutex_;

        Logger::Logger() : currentLogLevel(LogLevel::INFO), consoleOutput(true) {}

        Logger* Logger::getInstance() {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!instance) {
                instance = std::unique_ptr<Logger>(new Logger());
            }
            return instance.get();
        }

        bool Logger::initialize(const std::string& filePath, LogLevel level, bool enableConsole) {
            std::lock_guard<std::mutex> lock(fileMutex);

            logFilePath = filePath; 
            currentLogLevel = level;
            consoleOutput = enableConsole;

            std::filesystem::path logPath(filePath);
            std::filesystem::path logDir = logPath.parent_path();

            if(!logDir.empty() && !std::filesystem::exists(logDir)) {
                try {
                    std::filesystem::create_directories(logDir);
                } catch (const std::exception& e) {
                    std::cerr << "failed to create log directory: " << e.what() << std::endl;
                    return false;
                } 
            }

            logFile.open(filePath, std::ios::app); 
            if (!logFile.is_open()) {
                std::cerr << "Failed to open log file: " << filePath << std::endl;
                return false;log(LogLevel::INFO, "Logger initialized with log level: " + logLevelToString(currentLogLevel));
            }

            log(LogLevel::INFO, "Logger initialized - File: " + filePath + ", Level: " + logLevelToString(level));
            return true;
        }

        void Logger::setLogLevel(LogLevel level) {
            std::lock_guard<std::mutex> lock(fileMutex);
            currentLogLevel = level;
            log(LogLevel::INFO, "Log level changed to: " + logLevelToString(level));
        }

        std::string Logger::getCurrentTimeStamp() {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

            std::stringstream ss;
            ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
            ss << '.' << std::setfill('0') << std::setw(3) << ms.count();

            return ss.str();
        }

        std::string Logger::logLevelToString(LogLevel level) {
            switch (level) {
                case LogLevel::DEBUG: return "DEBUG";
                case LogLevel::INFO: return "INFO";
                case LogLevel::WARNING: return "WARNING";
                case LogLevel::ERROR: return "ERROR";
                case LogLevel::CRITICAL: return "CRITICAL";
                default: return "UNKNOWN";
            }
        }

        void Logger::log(LogLevel level, const std::string& message) {
            if (level < currentLogLevel) return;

            std::lock_guard<std::mutex> lock(fileMutex);
            std::string timestamp = getCurrentTimeStamp();
            std::string levelStr = logLevelToString(level);
            
            std::string logMessage = "[" + timestamp + "] [" + levelStr + "] " + message;

            if (logFile.is_open()) {
                logFile << logMessage << std::endl;
                logFile.flush(); 
            }

            if (consoleOutput) {
                if (level >= LogLevel::ERROR) {
                    std::cerr << logMessage << std::endl;
                } else {
                    std::cout << logMessage << std::endl;
                }
            }
        }

        void Logger::debug(const std::string& message) {
            log(LogLevel::DEBUG, message);
        }

        void Logger::info(const std::string& message) {
            log(LogLevel::INFO, message);
        }

        void Logger::warning(const std::string& message) {
            log(LogLevel::WARNING, message);
        }

        void Logger::error(const std::string& message) {
            log(LogLevel::ERROR, message);
        }

        void Logger::critical(const std::string& message) {
            log(LogLevel::CRITICAL, message);
        }

        void Logger::close() {
            std::lock_guard<std::mutex> lock(fileMutex);
            if (logFile.is_open()) {
                log(LogLevel::INFO, "Logger shutting down");
                logFile.close();
            }
        }

        Logger::~Logger() {
            close();
        }

    } // namespace Utils
} // namespace Crypto