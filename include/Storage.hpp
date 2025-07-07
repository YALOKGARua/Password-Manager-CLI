#pragma once
#include <string>
#include <vector>
#include <optional>
#include <stdexcept>
#include <nlohmann/json.hpp>

class StorageError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

struct StorageEntry {
    std::string username;
    std::string password;
};

class Storage {
public:
    Storage(const std::string& path, const std::string& masterPassword);

    bool exists() const noexcept;
    void initialize();
    void load();
    void save();

    void add(const std::string& name, const StorageEntry& entry);
    std::optional<StorageEntry> get(const std::string& name) const noexcept;
    void remove(const std::string& name);
    std::vector<std::string> listNames() const noexcept;
    void update(const std::string& name, const std::optional<std::string>& username, const std::optional<std::string>& password);

private:
    std::string path_;
    std::string masterPassword_;
    nlohmann::json data_;
    static constexpr int kSaltLength{16};
}; 