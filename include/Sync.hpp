#pragma once
#include <string>
#include <stdexcept>

class SyncError: public std::runtime_error { public: using std::runtime_error::runtime_error; };

class Sync {
public:
    virtual ~Sync() = default;
    virtual void upload(const std::string& localPath) = 0;
    virtual void download(const std::string& localPath) = 0;
}; 