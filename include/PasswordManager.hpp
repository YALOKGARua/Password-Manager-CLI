#pragma once
#include <string>
#include "Storage.hpp"

class PasswordManager {
public:
    PasswordManager(const std::string& dbPath, const std::string& masterPassword);
    void initDb();
    void add(const std::string& name, const std::string& username, const std::string& password);
    void list();
    void get(const std::string& name);
    void remove(const std::string& name);
    void update(const std::string& name, const std::string* username, const std::string* password);
private:
    Storage storage_;
}; 