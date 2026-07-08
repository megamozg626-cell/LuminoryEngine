#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <memory>
#include <mutex>

namespace Luminary {

class Logger {
public:
    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    static void Initialize(const std::string& filename = "engine.log");
    static void Shutdown();

    template<typename... Args>
    static void Log(LogLevel level, const std::string& format, Args&&... args) {
        if (!s_Instance) return;
        s_Instance->LogInternal(level, format, std::forward<Args>(args)...);
    }

    static void SetConsoleOutput(bool enabled) { s_ConsoleOutput = enabled; }
    static void SetFileOutput(bool enabled) { s_FileOutput = enabled; }
    static void SetMinLogLevel(LogLevel level) { s_MinLogLevel = level; }

private:
    static std::unique_ptr<Logger> s_Instance;
    static std::mutex s_Mutex;
    static bool s_ConsoleOutput;
    static bool s_FileOutput;
    static LogLevel s_MinLogLevel;

    std::string m_Filename;
    std::ofstream m_FileStream;

    Logger(const std::string& filename);
    ~Logger();

    void LogInternal(LogLevel level, const std::string& message);

    std::string GetLevelString(LogLevel level) const;
    std::string GetTimestamp() const;
};

} // namespace Luminary

// Convenient logging macros
#define LOG_DEBUG(fmt, ...) Luminary::Logger::Log(Luminary::Logger::LogLevel::Debug, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) Luminary::Logger::Log(Luminary::Logger::LogLevel::Info, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) Luminary::Logger::Log(Luminary::Logger::LogLevel::Warning, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Luminary::Logger::Log(Luminary::Logger::LogLevel::Error, fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) Luminary::Logger::Log(Luminary::Logger::LogLevel::Critical, fmt, ##__VA_ARGS__)
