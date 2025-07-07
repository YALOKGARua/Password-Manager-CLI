#pragma once
#include <string>
#include <vector>
#include <stdexcept>

namespace detail {
class CipherContext {
public:
    struct Impl;
    CipherContext();
    ~CipherContext();
    void initEncrypt(const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv);
    void initDecrypt(const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv, const std::vector<unsigned char>& tag);
    std::vector<unsigned char> update(const std::vector<unsigned char>& data);
    std::vector<unsigned char> final();
    std::vector<unsigned char> getTag(size_t len = 16);
private:
    Impl* pImpl;
};
}

class Crypto {
public:
    static std::vector<unsigned char> generateSalt(size_t length = 16);
    static std::vector<unsigned char> deriveKey(const std::string& password, const std::vector<unsigned char>& salt, int iterations = 100000);
    static std::vector<unsigned char> encrypt(const std::vector<unsigned char>& data,
                                              const std::string& password,
                                              const std::vector<unsigned char>& salt,
                                              int iterations = 100000);
    static std::vector<unsigned char> decrypt(const std::vector<unsigned char>& token,
                                              const std::string& password,
                                              int iterations = 100000);
}; 