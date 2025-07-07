#include "SyncDropbox.hpp"
#include <curl/curl.h>

SyncDropbox::SyncDropbox(const std::string& token): token_(token) {}
void SyncDropbox::upload(const std::string& localPath) {}
void SyncDropbox::download(const std::string& localPath) {} 