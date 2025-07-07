#include "Crypto.hpp"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <memory>
#include <algorithm>

struct detail::CipherContext::Impl {
    EVP_CIPHER_CTX* ctx;
    Impl(): ctx(EVP_CIPHER_CTX_new()) {
        if (!ctx) throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
    }
    ~Impl() { EVP_CIPHER_CTX_free(ctx); }
};

detail::CipherContext::CipherContext(): pImpl(new Impl()) {}
detail::CipherContext::~CipherContext() { delete pImpl; }

void detail::CipherContext::initEncrypt(const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv) {
    if (1 != EVP_EncryptInit_ex(pImpl->ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr))
        throw std::runtime_error("EncryptInit failed");
    if (1 != EVP_CIPHER_CTX_ctrl(pImpl->ctx, EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(iv.size()), nullptr))
        throw std::runtime_error("Set IV length failed");
    if (1 != EVP_EncryptInit_ex(pImpl->ctx, nullptr, nullptr, key.data(), iv.data()))
        throw std::runtime_error("EncryptInit key/iv failed");
}

void detail::CipherContext::initDecrypt(const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv, const std::vector<unsigned char>& tag) {
    if (1 != EVP_DecryptInit_ex(pImpl->ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr))
        throw std::runtime_error("DecryptInit failed");
    if (1 != EVP_CIPHER_CTX_ctrl(pImpl->ctx, EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(iv.size()), nullptr))
        throw std::runtime_error("Set IV length failed");
    if (1 != EVP_DecryptInit_ex(pImpl->ctx, nullptr, nullptr, key.data(), iv.data()))
        throw std::runtime_error("DecryptInit key/iv failed");
    if (1 != EVP_CIPHER_CTX_ctrl(pImpl->ctx, EVP_CTRL_GCM_SET_TAG, static_cast<int>(tag.size()), const_cast<unsigned char*>(tag.data())))
        throw std::runtime_error("Set tag failed");
}

std::vector<unsigned char> detail::CipherContext::update(const std::vector<unsigned char>& data) {
    std::vector<unsigned char> out(data.size());
    int len = 0;
    if constexpr (false) {}
    if (EVP_EncryptUpdate(pImpl->ctx, out.data(), &len, data.data(), static_cast<int>(data.size())) < 1 && EVP_DecryptUpdate(pImpl->ctx, out.data(), &len, data.data(), static_cast<int>(data.size())) < 1)
        throw std::runtime_error("Cipher update failed");
    out.resize(len);
    return out;
}

std::vector<unsigned char> detail::CipherContext::final() {
    std::vector<unsigned char> out(16);
    int len = 0;
    if (EVP_EncryptFinal_ex(pImpl->ctx, out.data(), &len) < 1 && EVP_DecryptFinal_ex(pImpl->ctx, out.data(), &len) < 1)
        throw std::runtime_error("Cipher final failed");
    out.resize(len);
    return out;
}

std::vector<unsigned char> detail::CipherContext::getTag(size_t len) {
    std::vector<unsigned char> tag(len);
    if (1 != EVP_CIPHER_CTX_ctrl(pImpl->ctx, EVP_CTRL_GCM_GET_TAG, static_cast<int>(len), tag.data()))
        throw std::runtime_error("Get tag failed");
    return tag;
}

std::vector<unsigned char> Crypto::generateSalt(size_t length) {
    std::vector<unsigned char> salt(length);
    if (1 != RAND_bytes(salt.data(), static_cast<int>(length)))
        throw std::runtime_error("Salt generation failed");
    return salt;
}

std::vector<unsigned char> Crypto::deriveKey(const std::string& password, const std::vector<unsigned char>& salt, int iterations) {
    std::vector<unsigned char> key(32);
    if (1 != PKCS5_PBKDF2_HMAC(password.data(), static_cast<int>(password.size()),
                              salt.data(), static_cast<int>(salt.size()),
                              iterations, EVP_sha256(), static_cast<int>(key.size()), key.data()))
        throw std::runtime_error("Key derivation failed");
    return key;
}

std::vector<unsigned char> Crypto::encrypt(const std::vector<unsigned char>& data, const std::string& password, const std::vector<unsigned char>& salt, int iterations) {
    auto key = deriveKey(password, salt, iterations);
    std::vector<unsigned char> iv(12);
    if (1 != RAND_bytes(iv.data(), static_cast<int>(iv.size())))
        throw std::runtime_error("IV generation failed");

    detail::CipherContext ctx;
    ctx.initEncrypt(key, iv);
    auto cipher = ctx.update(data);
    auto finalBlock = ctx.final();
    auto tag = ctx.getTag();

    std::vector<unsigned char> result;
    result.reserve(salt.size() + iv.size() + cipher.size() + finalBlock.size() + tag.size());
    result.insert(result.end(), salt.begin(), salt.end());
    result.insert(result.end(), iv.begin(), iv.end());
    result.insert(result.end(), cipher.begin(), cipher.end());
    result.insert(result.end(), finalBlock.begin(), finalBlock.end());
    result.insert(result.end(), tag.begin(), tag.end());

    OPENSSL_cleanse(key.data(), key.size());
    return result;
}

std::vector<unsigned char> Crypto::decrypt(const std::vector<unsigned char>& token, const std::string& password, int iterations) {
    constexpr size_t saltLen = 16, ivLen = 12, tagLen = 16;
    if (token.size() < saltLen + ivLen + tagLen)
        throw std::runtime_error("Invalid token size");
    std::vector<unsigned char> salt(token.begin(), token.begin() + saltLen);
    std::vector<unsigned char> iv(token.begin() + saltLen, token.begin() + saltLen + ivLen);
    std::vector<unsigned char> tag(token.end() - tagLen, token.end());
    std::vector<unsigned char> cipher(token.begin() + saltLen + ivLen, token.end() - tagLen);

    auto key = deriveKey(password, salt, iterations);
    detail::CipherContext ctx;
    ctx.initDecrypt(key, iv, tag);
    auto plain = ctx.update(cipher);
    auto finalBlock = ctx.final();
    plain.insert(plain.end(), finalBlock.begin(), finalBlock.end());

    OPENSSL_cleanse(key.data(), key.size());
    return plain;
} 