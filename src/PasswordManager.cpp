#include "PasswordManager.hpp"
#include <iostream>

PasswordManager::PasswordManager(const std::string& dbPath, const std::string& masterPassword): storage_(dbPath, masterPassword) {}

void PasswordManager::initDb() {
    if(!storage_.exists()) storage_.initialize();
}

void PasswordManager::add(const std::string& name, const std::string& username, const std::string& password) {
    storage_.load();
    storage_.add(name, username, password);
}

void PasswordManager::list() {
    storage_.load();
    auto items = storage_.list();
    for(auto& el: items.items()) {
        std::cout<<el.key()<<": "<<el.value()["username"]<<" / "<<el.value()["password"]<<"\n";
    }
}

void PasswordManager::get(const std::string& name) {
    storage_.load();
    auto entry = storage_.get(name);
    if(!entry.is_null()) {
        std::cout<<entry["username"]<<" / "<<entry["password"]<<"\n";
    }
}

void PasswordManager::remove(const std::string& name) {
    storage_.load();
    storage_.remove(name);
}

void PasswordManager::update(const std::string& name, const std::string* username, const std::string* password) {
    storage_.load();
    storage_.update(name, username, password);
} 