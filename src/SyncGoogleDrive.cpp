#include "SyncGoogleDrive.hpp"
#include <curl/curl.h>

SyncGoogleDrive::SyncGoogleDrive(const std::string& credentials): credentials_(credentials) {}
void SyncGoogleDrive::upload(const std::string& localPath) {}
void SyncGoogleDrive::download(const std::string& localPath) {} 