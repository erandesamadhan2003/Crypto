#include "utils/Logger.h"
#include <filesystem>
#include <iostream>  // for std::cout, std::cerr

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
            std::cout << "Log directory: " << logDir << std::endl;
            if(!logDir.empty() && !std::filesystem::exists(logDir)) {
                try {
                    std::filesystem::create_directories(logDir);
                } catch (const std::exception& e) {
                    std::cerr << "Failed to create log directory: " << e.what() << std::endl;
                    return false;
                } 
            }

            logFile.open(filePath, std::ios::app); 
            
            if (!logFile.is_open()) {
                std::cerr << "Failed to open log file: " << filePath << std::endl;
                return false;
            }

            // Now it's safe to log since the file is open
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
        // Early return if log level is too low
        if (level < currentLogLevel) return;
        std::lock_guard<std::mutex> lock(fileMutex);
        
        std::cout<< "log fuction called" << std::endl;
        std::string timestamp = "getCurrentTimeStamp()";
        std::string levelStr = logLevelToString(level);
        std::string logMessage = "[" + timestamp + "] [" + levelStr + "] " + message;
        std::cout << "Log message: " << logMessage << std::endl;
        std::cout << "Log file path: " << logFilePath << std::endl;
        // Write to file if it's opent
        if (logFile.is_open()) {
            std::cout << "Writing to log file: " << logFilePath << std::endl;
            logFile << logMessage << std::endl;
            // Flush only for critical errors or when explicitly needed
            if (level >= LogLevel::ERROR) {
                logFile.flush();
            }
        } else {
            std::cerr << "Log file is not open. Cannot write log message." << std::endl;
        }
        // Console output
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