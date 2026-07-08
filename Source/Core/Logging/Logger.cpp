#include "Logger.hpp"
#include <fmt/format.h>
#include <iostream>
#include <iomanip>

namespace Luminary {

std::unique_ptr<Logger> Logger::s_Instance;
std::mutex Logger::s_Mutex;
bool Logger::s_ConsoleOutput = true;
bool Logger::s_FileOutput = true;
Logger::LogLevel Logger::s_MinLogLevel = LogLevel::Debug;

Logger::Logger(const std::string& filename) : m_Filename(filename) {
    if (s_FileOutput) {
        m_FileStream.open(filename, std::ios::app);
    }
}

Logger::~Logger() {
    if (m_FileStream.is_open()) {
        m_FileStream.close();
    }
}

void Logger::Initialize(const std::string& filename) {
    std::lock_guard<std::mutex> lock(s_Mutex);
    if (!s_Instance) {
        s_Instance = std::make_unique<Logger>(filename);
    }
}

void Logger::Shutdown() {
    std::lock_guard<std::mutex> lock(s_Mutex);
    s_Instance.reset();
}

std::string Logger::GetLevelString(LogLevel level) const {
    switch (level) {
        case LogLevel::Debug: return "[DEBUG]";
        case LogLevel::Info: return "[INFO]";
        case LogLevel::Warning: return "[WARNING]";
        case LogLevel::Error: return "[ERROR]";
        case LogLevel::Critical: return "[CRITICAL]";
        default: return "[UNKNOWN]";
    }
}

std::string Logger::GetTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%H:%M:%S")
       << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

void Logger::LogInternal(LogLevel level, const std::string& message) {
    if (level < s_MinLogLevel) return;

    std::lock_guard<std::mutex> lock(s_Mutex);

    std::string logMessage = fmt::format("{} {} {}",
        GetTimestamp(),
        GetLevelString(level),
        message);

    if (s_ConsoleOutput) {
        std::cout << logMessage << std::endl;
    }

    if (s_FileOutput && m_FileStream.is_open()) {
        m_FileStream << logMessage << std::endl;
        m_FileStream.flush();
    }
}

} // namespace Luminary
