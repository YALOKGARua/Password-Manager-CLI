#include "Logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>
#include <filesystem>

std::shared_ptr<spdlog::logger> Logger::logger_ = nullptr;

void Logger::init(const std::string& pattern, bool async, const std::string& logFile) {
    if(async) spdlog::init_thread_pool(8192, 1);
    std::filesystem::create_directories("logs");
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_level(spdlog::level::info);
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile, true);
    fileSink->set_level(spdlog::level::debug);
    if(async)
        logger_ = std::make_shared<spdlog::async_logger>("pwdmgr", spdlog::sinks_init_list{consoleSink, fileSink}, spdlog::thread_pool());
    else
        logger_ = std::make_shared<spdlog::logger>("pwdmgr", spdlog::sinks_init_list{consoleSink, fileSink});
    spdlog::register_logger(logger_);
    logger_->set_level(spdlog::level::debug);
    logger_->set_pattern(pattern);
}

std::shared_ptr<spdlog::logger>& Logger::instance() { return logger_; } 