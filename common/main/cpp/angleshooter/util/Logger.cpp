#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "Logger.h"

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : creationTime(static_cast<long>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count())) {
    const std::filesystem::path runDir = "run";
    const auto createdRun = create_directories(runDir);
    const auto logPath = runDir / "latest.log";
    if (exists(logPath)) try {
        auto fileTime = last_write_time(logPath);
        auto unixTime = static_cast<std::time_t>([&]{
            #ifdef _WIN32
                return (fileTime.time_since_epoch().count() - 116444736000000000ULL) / 10000000;
            #else
                return std::chrono::system_clock::to_time_t(fileTime + (std::chrono::system_clock::now() - std::filesystem::file_time_type::clock::now()));
            #endif
        }());
        std::tm timeInfo;
        #ifdef _WIN32
            localtime_s(&timeInfo, &unixTime);
        #else
            localtime_r(&unixTime, &timeInfo);
        #endif
        std::stringstream stringBuilder;
        stringBuilder << std::put_time(&timeInfo, "%Y-%m-%d_%H-%M-%S");
        std::filesystem::rename(logPath, runDir / (stringBuilder.str() + ".log"));
    } catch (const std::exception& e) { std::cerr << "Log rotation failed: " << e.what() << "\n"; }
    if (std::ofstream logFile(logPath, std::ios::out | std::ios::app); !logFile.is_open()) std::cerr << "Failed to open log file\n";
    if (createdRun) log(Severity::INFO, "Created Run Directory");
}

void Logger::log(Severity level, const std::string& message) {
    const auto string = severityToColour(level) + "[" + severityToString(level) + "] \033[38;5;252m" + message + '\n';
    std::cout << string;
    std::cout.flush();
    if (logFile.is_open()) {
        const auto cleanString = "[" + severityToString(level) + "] " + message + '\n';
        logFile << cleanString;
    }
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