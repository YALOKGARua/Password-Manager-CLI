#include "Storage.hpp"
#include "Crypto.hpp"
#include <fstream>
#include <vector>
#include <filesystem>

Storage::Storage(const std::string& path, const std::string& masterPassword)
    : path_(path), masterPassword_(masterPassword) {}

bool Storage::exists() const noexcept {
    return std::filesystem::exists(path_);
}

void Storage::initialize() {
    if (exists())
        throw StorageError("Database already exists: " + path_);
    auto salt = Crypto::generateSalt(kSaltLength);
    auto payload = Crypto::encrypt(std::vector<unsigned char>{'{','}'}, masterPassword_, salt);
    std::ofstream ofs(path_, std::ios::binary);
    if (!ofs)
        throw StorageError("Failed to create database file: " + path_);
    ofs.write(reinterpret_cast<const char*>(payload.data()), payload.size());
}

void Storage::load() {
    if (!exists())
        throw StorageError("Database file not found: " + path_);
    std::ifstream ifs(path_, std::ios::binary);
    if (!ifs)
        throw StorageError("Failed to open database file: " + path_);
    std::vector<unsigned char> token((std::istreambuf_iterator<char>(ifs)), {});
    try {
        auto plaintext = Crypto::decrypt(token, masterPassword_);
        data_ = nlohmann::json::parse(plaintext.begin(), plaintext.end());
    } catch (const std::exception& e) {
        throw StorageError(std::string("Load error: ") + e.what());
    }
}

void Storage::save() {
    auto salt = Crypto::generateSalt(kSaltLength);
    std::string dumped = data_.dump();
    auto payload = Crypto::encrypt(
        std::vector<unsigned char>(dumped.begin(), dumped.end()), masterPassword_, salt);
    std::ofstream ofs(path_, std::ios::binary);
    if (!ofs)
        throw StorageError("Failed to open database for writing: " + path_);
    ofs.write(reinterpret_cast<const char*>(payload.data()), payload.size());
}

void Storage::add(const std::string& name, const StorageEntry& entry) {
    data_[name] = {{"username", entry.username}, {"password", entry.password}};
    save();
}

std::optional<StorageEntry> Storage::get(const std::string& name) const noexcept {
    if (!data_.contains(name))
        return std::nullopt;
    auto j = data_.at(name);
    return StorageEntry{j.value("username", ""), j.value("password", "")};
}

void Storage::remove(const std::string& name) {
    if (!data_.contains(name))
        throw StorageError("Entry not found: " + name);
    data_.erase(name);
    save();
}

std::vector<std::string> Storage::listNames() const noexcept {
    std::vector<std::string> names;
    for (const auto& el : data_.items())
        names.push_back(el.key());
    return names;
}

void Storage::update(const std::string& name, const std::optional<std::string>& username,
                     const std::optional<std::string>& password) {
    if (!data_.contains(name))
        throw StorageError("Entry not found: " + name);
    if (username)
        data_[name]["username"] = *username;
    if (password)
        data_[name]["password"] = *password;
    save();
} 