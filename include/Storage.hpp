#pragma once
#include <string>
#include <nlohmann/json.hpp>

class Storage {
public:
    Storage(const std::string& path, const std::string& masterPassword);
    bool exists() const;
    void initialize();
    void load();
    void save();
    void add(const std::string& name, const std::string& username, const std::string& password);
    nlohmann::json get(const std::string& name) const;
    void remove(const std::string& name);
    nlohmann::json list() const;
    void update(const std::string& name, const std::string* username = nullptr, const std::string* password = nullptr);
private:
    std::string path_;
    std::string masterPassword_;
    nlohmann::json data_;
}; 