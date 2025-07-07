#pragma once
#include <string>
#include <random>

class PasswordGenerator {
public:
    PasswordGenerator();
    std::string generate(size_t length, bool upper, bool lower, bool digits, bool symbols);
private:
    std::random_device rd;
    std::mt19937 gen;
}; 