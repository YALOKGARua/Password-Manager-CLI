#include "Logger.hpp"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>

std::shared_ptr<spdlog::logger> g_logger;

void Logger::init(const std::string& pattern, bool async) {
    if(async) spdlog::init_thread_pool(8192, 1);
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("pwdmgr.log");
    if(async) g_logger = std::make_shared<spdlog::async_logger>("pwdmgr", sink, spdlog::thread_pool());
    else g_logger = std::make_shared<spdlog::logger>("pwdmgr", sink);
    spdlog::set_default_logger(g_logger);
    spdlog::set_pattern(pattern);
}

std::shared_ptr<spdlog::logger>& Logger::instance() { return g_logger; } 