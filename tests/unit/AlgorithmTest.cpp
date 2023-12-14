#include <gtest/gtest.h>

#include "Algorithm.hpp"

TEST(HelloTest, BasicAssertions)
{
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);
}

TEST(AlgorithmTest, extractFileNameTest)
{
    std::string result;
    const std::string sourcePath { "workspace/data/file.yaml " };
    hibiscus::algo::extractFileName(sourcePath, result);
    EXPECT_TRUE(result == "file.yaml");
}