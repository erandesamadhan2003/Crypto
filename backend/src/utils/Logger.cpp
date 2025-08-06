    #include "utils/Logger.h"
    #include <memory>
    #include <chrono>
    #include <iomanip>
    #include <sstream>
    #include <iostream>

    namespace Crypto {
        namespace Utils {

            std::unique_ptr<Logger> Logger::instance = nullptr;
            std::mutex Logger::mutex_;

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

            std::string Logger::getCurrentTime() {
                auto now = std::chrono::system_clock::now();
                std::time_t now_time = std::chrono::system_clock::to_time_t(now);
                std::tm* tm = std::localtime(&now_time);
                std::ostringstream oss;
                oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
                return oss.str();   
            }

            Logger::Logger() {
                std::cout << "[Logger initialized]" << std::endl;
            }

            Logger* Logger::getInstance() {
                std::lock_guard<std::mutex> lock(mutex_);
                if (!instance)
                    instance = std::unique_ptr<Logger>(new Logger());
                return instance.get();
            }

            void Logger::log(LogLevel level, const std::string& message) {
                std::lock_guard<std::mutex> lock(mutex_);
                std::string logMessage = getCurrentTime() + " [" + logLevelToString(level) + "] " + message;

                // OUTPUT TO CONSOLE
                if (level >= LogLevel::DEBUG)
                    std::cout << logMessage << std::endl;
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
            
            // DESTRUCTOR
            Logger::~Logger() {
                std::cout << "[Logger destroyed]" << std::endl;
            }



        } // namespace Utils
    } // namespace Crypto
