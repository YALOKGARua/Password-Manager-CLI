#include <gtest/gtest.h>
#include "Logger.hpp"

TEST(LoggerTest, Init) {
    Logger::init("%+", true);
    auto& log = Logger::instance();
    ASSERT_TRUE(log != nullptr);
} 