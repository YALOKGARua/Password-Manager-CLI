#include "Crypto.hpp"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>

std::vector<unsigned char> Crypto::generateSalt(size_t length) {
    std::vector<unsigned char> salt(length);
    RAND_bytes(salt.data(), static_cast<int>(length));
    return salt;
}

std::vector<unsigned char> Crypto::deriveKey(const std::string& password, const std::vector<unsigned char>& salt, int iterations) {
    std::vector<unsigned char> key(32);
    PKCS5_PBKDF2_HMAC(password.data(), static_cast<int>(password.size()), salt.data(), static_cast<int>(salt.size()), iterations, EVP_sha256(), static_cast<int>(key.size()), key.data());
    return key;
}

std::vector<unsigned char> Crypto::encrypt(const std::vector<unsigned char>& data, const std::string& password, const std::vector<unsigned char>& salt, int iterations) {
    auto key = deriveKey(password, salt, iterations);
    std::vector<unsigned char> iv(12);
    RAND_bytes(iv.data(), static_cast<int>(iv.size()));
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(iv.size()), nullptr);
    EVP_EncryptInit_ex(ctx, nullptr, nullptr, key.data(), iv.data());
    std::vector<unsigned char> ciphertext(data.size());
    int len;
    EVP_EncryptUpdate(ctx, ciphertext.data(), &len, data.data(), static_cast<int>(data.size()));
    int ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len);
    ciphertext_len += len;
    std::vector<unsigned char> tag(16);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, static_cast<int>(tag.size()), tag.data());
    EVP_CIPHER_CTX_free(ctx);
    std::vector<unsigned char> result;
    result.reserve(salt.size() + iv.size() + ciphertext_len + tag.size());
    result.insert(result.end(), salt.begin(), salt.end());
    result.insert(result.end(), iv.begin(), iv.end());
    result.insert(result.end(), ciphertext.begin(), ciphertext.begin() + ciphertext_len);
    result.insert(result.end(), tag.begin(), tag.end());
    return result;
}

std::vector<unsigned char> Crypto::decrypt(const std::vector<unsigned char>& token, const std::string& password, int iterations) {
    const size_t salt_len = 16;
    const size_t iv_len = 12;
    const size_t tag_len = 16;
    std::vector<unsigned char> salt(token.begin(), token.begin() + salt_len);
    std::vector<unsigned char> iv(token.begin() + salt_len, token.begin() + salt_len + iv_len);
    std::vector<unsigned char> tag(token.end() - tag_len, token.end());
    std::vector<unsigned char> ciphertext(token.begin() + salt_len + iv_len, token.end() - tag_len);
    auto key = deriveKey(password, salt, iterations);
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(iv.size()), nullptr);
    EVP_DecryptInit_ex(ctx, nullptr, nullptr, key.data(), iv.data());
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, static_cast<int>(tag.size()), tag.data());
    std::vector<unsigned char> plaintext(ciphertext.size());
    int len;
    EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), static_cast<int>(ciphertext.size()));
    EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
} 