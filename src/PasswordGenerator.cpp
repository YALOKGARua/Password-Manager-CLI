#include "PasswordGenerator.hpp"

PasswordGenerator::PasswordGenerator(): gen(rd()) {}

std::string PasswordGenerator::generate(size_t length, bool upper, bool lower, bool digits, bool symbols) {
    std::string pool;
    if(upper) pool += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if(lower) pool += "abcdefghijklmnopqrstuvwxyz";
    if(digits) pool += "0123456789";
    if(symbols) pool += "!@#$%^&*()[]";
    std::uniform_int_distribution<> dist(0, static_cast<int>(pool.size() - 1));
    std::string result;
    for(size_t i = 0; i < length; ++i) result += pool[dist(gen)];
    return result;
} 