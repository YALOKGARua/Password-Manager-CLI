#pragma once
#include <string>
#include <optional>
#include <memory>
#include "Storage.hpp"
#include "Logger.hpp"
#include "PasswordGenerator.hpp"

class PasswordManager {
public:
    struct Config { std::string dbPath; bool asyncLogging = true; std::string logPattern = "[%Y-%m-%d %H:%M:%S] [%l] %v"; std::string logFile = "logs/pwdmgr.log"; };
    PasswordManager(const Config& cfg, const std::string& masterPassword);
    void initDb();
    void add(const std::string& name, const std::string& username, const std::string& password);
    void list() const;
    void get(const std::string& name) const;
    void remove(const std::string& name);
    void update(const std::string& name, const std::optional<std::string>& username, const std::optional<std::string>& password);
private:
    Storage storage_;
    std::shared_ptr<spdlog::logger> logger_;
}; 