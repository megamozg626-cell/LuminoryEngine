#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

namespace Luminary {

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

class Logger {
public:
    static Logger& GetInstance() {
        static Logger instance;
        return instance;
    }

    void Initialize(const std::string& logFile = "luminary.log") {
        m_LogFile.open(logFile, std::ios::app);
        m_LogLevel = LogLevel::Debug;
    }

    void Shutdown() {
        if (m_LogFile.is_open()) {
            m_LogFile.close();
        }
    }

    void SetLogLevel(LogLevel level) {
        m_LogLevel = level;
    }

    template<typename... Args>
    void Log(LogLevel level, const std::string& message, Args... args) {
        if (level < m_LogLevel) return;

        std::string formattedMessage = FormatMessage(message, args...);
        std::string logEntry = FormatLogEntry(level, formattedMessage);

        std::cout << logEntry << std::endl;

        if (m_LogFile.is_open()) {
            m_LogFile << logEntry << std::endl;
            m_LogFile.flush();
        }
    }

private:
    Logger() = default;
    ~Logger() { Shutdown(); }

    LogLevel m_LogLevel = LogLevel::Debug;
    std::ofstream m_LogFile;

    template<typename... Args>
    std::string FormatMessage(const std::string& message, Args... args) {
        // Simple formatting - can be enhanced with proper format library
        return message;
    }

    std::string FormatLogEntry(LogLevel level, const std::string& message) {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::stringstream ss;
        ss << "[" << std::put_time(std::localtime(&time), "%H:%M:%S")
           << "." << std::setfill('0') << std::setw(3) << ms.count() << "] "
           << "[" << GetLevelString(level) << "] "
           << message;

        return ss.str();
    }

    std::string GetLevelString(LogLevel level) const {
        switch (level) {
            case LogLevel::Trace:    return "TRACE";
            case LogLevel::Debug:    return "DEBUG";
            case LogLevel::Info:     return "INFO";
            case LogLevel::Warning:  return "WARN";
            case LogLevel::Error:    return "ERROR";
            case LogLevel::Critical: return "CRITICAL";
            default:                 return "UNKNOWN";
        }
    }
};

} // namespace Luminary

// Convenience macros
#define LOG_TRACE(msg, ...) Luminary::Logger::GetInstance().Log(Luminary::LogLevel::Trace, msg, ##__VA_ARGS__)
#define LOG_DEBUG(msg, ...) Luminary::Logger::GetInstance().Log(Luminary::LogLevel::Debug, msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) Luminary::Logger::GetInstance().Log(Luminary::LogLevel::Info, msg, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...) Luminary::Logger::GetInstance().Log(Luminary::LogLevel::Warning, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) Luminary::Logger::GetInstance().Log(Luminary::LogLevel::Error, msg, ##__VA_ARGS__)
#define LOG_CRITICAL(msg, ...) Luminary::Logger::GetInstance().Log(Luminary::LogLevel::Critical, msg, ##__VA_ARGS__)
