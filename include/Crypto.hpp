#pragma once
#include <string>
#include <vector>

class Crypto {
public:
    static std::vector<unsigned char> generateSalt(size_t length = 16);
    static std::vector<unsigned char> deriveKey(const std::string& password, const std::vector<unsigned char>& salt, int iterations = 100000);
    static std::vector<unsigned char> encrypt(const std::vector<unsigned char>& data, const std::string& password, const std::vector<unsigned char>& salt, int iterations = 100000);
    static std::vector<unsigned char> decrypt(const std::vector<unsigned char>& token, const std::string& password, int iterations = 100000);
}; 