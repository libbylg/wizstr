#include "testing.hpp"

#include "str.hpp"

#include <array>

TEST(test_str, count_words) {
    SECTION("无pos+view模式:简单场景") {
        ASSERT_EQ(str::count_words("\r\n\t A\r\n\t B\r\n\t C\r\n\t "), 3);
    }
    SECTION("无pos+view模式:就一个单词1") {
        ASSERT_EQ(str::count_words("DEF"), 1);
    }
    SECTION("无pos+view模式:就一个单词2") {
        ASSERT_EQ(str::count_words(" \r\n\t DEF"), 1);
    }
    SECTION("无pos+view模式:就一个单词2") {
        ASSERT_EQ(str::count_words("DEF \r\n\t "), 1);
    }
    SECTION("无pos+view模式:多个单词1") {
        ASSERT_EQ(str::count_words("ABC \r\nDEF MPQ  "), 3);
    }
    SECTION("无pos+view模式:多个单词2") {
        ASSERT_EQ(str::count_words("   ABC   DEF GHI"), 3);
    }
    SECTION("无pos+view模式:空串") {
        ASSERT_EQ(str::count_words(""), 0);
    }
    SECTION("无pos+view模式:全空白") {
        ASSERT_EQ(str::count_words("\r\n\t \n\t "), 0);
    }
}