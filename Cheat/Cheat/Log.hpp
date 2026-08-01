#pragma once

#include <Windows.h>
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "Brand.hpp"
REVERS_STAMP(Log_hpp)

namespace CheatLogger {
    enum class LogLevel {
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR,
        LOG_SUCCESS
    };

    inline std::string GetTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        std::tm local_time;
        localtime_s(&local_time, &time_t_now);

        std::stringstream ss;
        ss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    inline std::string GetLevelString(LogLevel level) {
        switch (level) {
            case LogLevel::LOG_INFO: return "[INFO]";
            case LogLevel::LOG_WARNING: return "[WARNING]";
            case LogLevel::LOG_ERROR: return "[ERROR]";
            case LogLevel::LOG_SUCCESS: return "[SUCCESS]";
            default: return "[UNKNOWN]";
        }
    }

    inline void WriteLog(const std::string& message, LogLevel level = LogLevel::LOG_INFO) {
        try {
            std::ofstream logFile("cheat_log.txt", std::ios::app);
            if (logFile.is_open()) {
                logFile << GetTimestamp() << " [" << globals::brand::logPrefix << "] "
                    << globals::brand::createdBy << " "
                    << GetLevelString(level) << " " << message << std::endl;
                logFile.close();
            }
        } catch (...) {

        }
    }
}
