#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "Logger.h"

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(Severity level, const std::string& message) {
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    tm localTime;
    std::stringstream timeStream;
    if (localtime_s(&localTime, &time) != 0) {
        timeStream << "[??:??:??]";
    } else {
        timeStream << std::put_time(&localTime, "[%H:%M:%S]");
    }
    const auto string = severityToColour(level) + timeStream.str() + " [" + severityToString(level) + "] \033[38;5;252m" + message + '\n';
    std::cout << string;
    std::cout.flush();
}

void Logger::logOnce(Severity level, const std::string& message) {
    const auto messageHash = hasher(message);
    if (!loggedMessages.empty() && loggedMessages.contains(messageHash)) return;
    loggedMessages.insert(messageHash);
    log(level, message);
}

void Logger::debug(const std::string& message) {
    getInstance().log(Severity::DEBUG, message);
}

void Logger::info(const std::string& message) {
    getInstance().log(Severity::INFO, message);
}

void Logger::warn(const std::string& message) {
    getInstance().log(Severity::WARN, message);
}

void Logger::error(const std::string& message) {
    getInstance().log(Severity::ERROR, message);
}

void Logger::debugOnce(const std::string& message) {
    getInstance().logOnce(Severity::DEBUG, message);
}

void Logger::infoOnce(const std::string& message) {
    getInstance().logOnce(Severity::INFO, message);
}

void Logger::warnOnce(const std::string& message) {
    getInstance().logOnce(Severity::WARN, message);
}

void Logger::errorOnce(const std::string& message) {
    getInstance().logOnce(Severity::ERROR, message);
}

std::string Logger::severityToString(Severity level) const {
    switch (level) {
        case Severity::DEBUG: return "DEBUG";
        case Severity::INFO: return "INFO";
        case Severity::WARN: return "WARN";
        case Severity::ERROR: return "ERROR";
    }
    return "UNKNOWN";
}

std::string Logger::severityToColour(Severity level) const {
    switch (level) {
        case Severity::DEBUG: return "\033[38;5;247m";
        case Severity::INFO: return "\033[38;5;110m";
        case Severity::WARN: return "\033[38;5;215m";
        case Severity::ERROR: return "\033[38;5;210m";
    }
    return "\033[0;39m";
}