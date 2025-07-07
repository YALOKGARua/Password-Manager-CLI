#pragma once
#include <string>

class Sync {
public:
    virtual ~Sync() = default;
    virtual void upload(const std::string& localPath) = 0;
    virtual void download(const std::string& localPath) = 0;
}; 