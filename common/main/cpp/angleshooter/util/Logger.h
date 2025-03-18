#pragma once
#include <fstream>

enum class Severity : std::uint8_t {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger {
    long creationTime;
    std::ofstream logFile;
    std::hash<std::string> hasher;
    std::unordered_set<size_t> loggedMessages;
    Logger();
    void log(Severity level, const std::string& message);
    void logOnce(Severity level, const std::string& message);
public:
    static Logger& getInstance();
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warn(const std::string& message);
    static void error(const std::string& message);
    static void debugOnce(const std::string& message);
    static void infoOnce(const std::string& message);
    static void warnOnce(const std::string& message);
    static void errorOnce(const std::string& message);
    [[nodiscard]] std::string severityToString(Severity level) const;
    [[nodiscard]] std::string severityToColour(Severity level) const;
};