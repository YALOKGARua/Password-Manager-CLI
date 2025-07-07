#include "SyncGoogleDrive.hpp"
#include "Logger.hpp"

SyncGoogleDrive::SyncGoogleDrive(const std::string& credentials): credentials_(credentials) {}

void SyncGoogleDrive::upload(const std::string&) {
    throw SyncError("Google Drive upload not implemented");
}

void SyncGoogleDrive::download(const std::string&) {
    throw SyncError("Google Drive download not implemented");
} 