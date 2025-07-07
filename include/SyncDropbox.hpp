#pragma once
#include "Sync.hpp"
#include <string>

class SyncDropbox : public Sync {
public:
    SyncDropbox(const std::string& token);
    void upload(const std::string& localPath) override;
    void download(const std::string& localPath) override;
private:
    std::string token_;
}; 