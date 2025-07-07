#include "SyncDropbox.hpp"
#include "Logger.hpp"
#include <curl/curl.h>
#include <fstream>

SyncDropbox::SyncDropbox(const std::string& token): token_(token) {}

void SyncDropbox::upload(const std::string& localPath) {
    auto logger = Logger::instance();
    logger->info("Uploading {} to Dropbox", localPath);
    std::ifstream ifs(localPath, std::ios::binary);
    if(!ifs) throw SyncError("Cannot open file: " + localPath);
    std::vector<char> data((std::istreambuf_iterator<char>(ifs)), {});
    CURL* curl = curl_easy_init();
    if(!curl) throw SyncError("CURL init failed");
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + token_).c_str());
    headers = curl_slist_append(headers, ("Dropbox-API-Arg: {\"path\":\"/" + localPath + "\",\"mode\":\"overwrite\"}").c_str());
    curl_easy_setopt(curl, CURLOPT_URL, "https://content.dropboxapi.com/2/files/upload");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.data());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    if(res != CURLE_OK) throw SyncError(std::string("Dropbox upload failed: ") + curl_easy_strerror(res));
}

void SyncDropbox::download(const std::string&) {
    throw SyncError("Dropbox download not implemented");
} 