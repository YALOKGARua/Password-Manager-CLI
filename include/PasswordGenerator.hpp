#pragma once
#include <string>
#include <random>
#include <vector>
#include <stdexcept>

class PasswordGenerator {
public:
    struct Options { size_t length = 16; bool upper = true; bool lower = true; bool digits = true; bool symbols = true; };
    explicit PasswordGenerator(const Options& opts = {});
    std::string generate() const;
private:
    void buildCharset();
    Options opts_;
    std::mt19937_64 gen_;
    std::string charset_;
}; 