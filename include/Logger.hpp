#pragma once
#include <spdlog/logger.h>
#include <memory>
#include <string>

class Logger {
public:
    static void init(const std::string& pattern, bool async = true, const std::string& logFile = "logs/pwdmgr.log");
    static std::shared_ptr<spdlog::logger>& instance();
private:
    static std::shared_ptr<spdlog::logger> logger_;
}; 