#pragma once
#include "Sync.hpp"
#include <string>

class SyncGoogleDrive : public Sync {
public:
    SyncGoogleDrive(const std::string& credentials);
    void upload(const std::string& localPath) override;
    void download(const std::string& localPath) override;
private:
    std::string credentials_;
}; 