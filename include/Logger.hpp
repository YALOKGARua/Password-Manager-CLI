#pragma once
#include <spdlog/spdlog.h>
#include <memory>
#include <string>

class Logger {
public:
    static void init(const std::string& pattern, bool async = true);
    static std::shared_ptr<spdlog::logger>& instance();
}; 