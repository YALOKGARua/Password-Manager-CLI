#include "Storage.hpp"
#include "Crypto.hpp"
#include <fstream>
#include <vector>
#include <filesystem>

Storage::Storage(const std::string& path, const std::string& masterPassword): path_(path), masterPassword_(masterPassword) {}

bool Storage::exists() const {
    return std::filesystem::exists(path_);
}

void Storage::initialize() {
    auto salt = Crypto::generateSalt();
    auto payload = Crypto::encrypt(std::vector<unsigned char>{'{','}'}, masterPassword_, salt);
    std::ofstream ofs(path_, std::ios::binary);
    ofs.write(reinterpret_cast<const char*>(payload.data()), payload.size());
}

void Storage::load() {
    std::ifstream ifs(path_, std::ios::binary);
    std::vector<unsigned char> token((std::istreambuf_iterator<char>(ifs)), {});
    auto plaintext = Crypto::decrypt(token, masterPassword_);
    data_ = nlohmann::json::parse(plaintext.begin(), plaintext.end());
}

void Storage::save() {
    auto salt = Crypto::generateSalt();
    std::string dumped = data_.dump();
    auto payload = Crypto::encrypt(std::vector<unsigned char>(dumped.begin(), dumped.end()), masterPassword_, salt);
    std::ofstream ofs(path_, std::ios::binary);
    ofs.write(reinterpret_cast<const char*>(payload.data()), payload.size());
}

void Storage::add(const std::string& name, const std::string& username, const std::string& password) {
    data_[name] = {{"username", username}, {"password", password}};
    save();
}

nlohmann::json Storage::get(const std::string& name) const {
    return data_.value(name, nlohmann::json());
}

void Storage::remove(const std::string& name) {
    if(data_.contains(name)) {
        data_.erase(name);
        save();
    }
}

nlohmann::json Storage::list() const {
    return data_;
}

void Storage::update(const std::string& name, const std::string* username, const std::string* password) {
    if(data_.contains(name)) {
        if(username) data_[name]["username"] = *username;
        if(password) data_[name]["password"] = *password;
        save();
    }
} 