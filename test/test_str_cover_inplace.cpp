#include "testing.hpp"

#include "str.hpp"

#include <array>

TEST(test_str, cover_inplace) {
    std::string s;
    SECTION("mask") {
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk"), "a***ijk");
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "#", 1, 3), "a#ijk");
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "??????", 1, 3), "a??????ijk");
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "", 1, 3), "aijk");
    }
    SECTION("left&right") {
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk"), "a***ijk");
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", 0, 3), "###ijk");
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", 1, 3), "a###ijk");
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", 3, 3), "abc###ijk");

        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", 3, 0), "abc###");
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", 3, 1), "abc###k");
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", 3, 3), "abc###ijk");

        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", 5, 6), "abcde###fghijk");
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", 11, 0), "abcdefghijk###");
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", 0, 11), "###abcdefghijk");
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", 0, 11), "###abcdefghijk");

        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", 11, 1), "");
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", 1, 11), "");

        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", str::npos, 0), "");
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", 0, str::npos), "");
        ASSERT_EQ(str::cover_inplace(s = "abcdefghijk", "###", str::npos, str::npos), "");
    }
    SECTION("empty") {
        ASSERT_EQ(str::cover_inplace(s = "", "###", 0, 0), "");
        ASSERT_EQ(str::cover_inplace(s = "", "", 0, 0), "");
    }
}