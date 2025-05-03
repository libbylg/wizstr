#include "testing.hpp"

#include "str.hpp"

TEST(test_str, next_char) {
    SECTION("查找下一个字符") {
        size_t pos = 0;
        ASSERT_EQ(str::next_char("abc def", pos = 0, 'd'), 4);
        ASSERT_EQ(str::next_char("abc def", pos = 1, 'd'), 4);
        ASSERT_EQ(str::next_char("abc def", pos = 2, 'd'), 4);
        ASSERT_EQ(str::next_char("abc def", pos = 3, 'd'), 4);
        ASSERT_EQ(str::next_char("abc def", pos = 4, 'd'), 4);
        ASSERT_EQ(str::next_char("abc def", pos = 5, 'd'), str::npos);
        ASSERT_EQ(str::next_char("abc def", pos = 6, 'd'), str::npos);
        ASSERT_EQ(str::next_char("abc def", pos = 7, 'd'), str::npos);
        ASSERT_EQ(str::next_char("abc def", pos = str::npos, 'd'), str::npos);
        ASSERT_EQ(str::next_char("abc def", pos = 0, 'K'), str::npos);
        ASSERT_EQ(str::next_char("", pos = 0, '\0'), str::npos);
        ASSERT_EQ(str::next_char("", pos = str::npos, '\0'), str::npos);
    }
    SECTION("查找下一个字符集") {
        size_t pos = 0;
        ASSERT_EQ(str::next_char("abc def", pos = 0, "#db"), 1);
        ASSERT_EQ(str::next_char("abc def", pos = 1, "#db"), 1);
        ASSERT_EQ(str::next_char("abc def", pos = 2, "#db"), 4);
        ASSERT_EQ(str::next_char("abc def", pos = 3, "#db"), 4);
        ASSERT_EQ(str::next_char("abc def", pos = 4, "#db"), 4);
        ASSERT_EQ(str::next_char("abc def", pos = 5, "#db"), str::npos);
        ASSERT_EQ(str::next_char("abc def", pos = 6, "#db"), str::npos);
        ASSERT_EQ(str::next_char("abc def", pos = 7, "#db"), str::npos);
        ASSERT_EQ(str::next_char("abc def", pos = str::npos, "#bd"), str::npos);
        ASSERT_EQ(str::next_char("abc def", pos = 0, "#K"), str::npos);
        ASSERT_EQ(str::next_char("", pos = 0, std::string_view{"\0", 1}), str::npos);
        ASSERT_EQ(str::next_char("", pos = str::npos, std::string_view{"\0", 1}), str::npos);
    }
    SECTION("查找满足下一个proc的字符的位置") {
        size_t pos = 0;
        ASSERT_EQ(str::next_char("a12 e3", pos = 0, [](str::value_type ch) -> bool { return std::isdigit(ch); }), 1);
        ASSERT_EQ(str::next_char("a12 e3", pos = 1, [](str::value_type ch) -> bool { return std::isdigit(ch); }), 1);
        ASSERT_EQ(str::next_char("a12 e3", pos = 2, [](str::value_type ch) -> bool { return std::isdigit(ch); }), 2);
        ASSERT_EQ(str::next_char("a12 e3", pos = 3, [](str::value_type ch) -> bool { return std::isdigit(ch); }), 5);
        ASSERT_EQ(str::next_char("a12 e3", pos = 4, [](str::value_type ch) -> bool { return std::isdigit(ch); }), 5);
        ASSERT_EQ(str::next_char("a12 e3", pos = 5, [](str::value_type ch) -> bool { return std::isdigit(ch); }), 5);
        ASSERT_EQ(str::next_char("a12 e3", pos = 6, [](str::value_type ch) -> bool { return std::isdigit(ch); }), str::npos);
        ASSERT_EQ(str::next_char("a12 e3", pos = 7, [](str::value_type ch) -> bool { return std::isdigit(ch); }), str::npos);
        ASSERT_EQ(str::next_char("a12 e3", pos = str::npos, [](str::value_type ch) -> bool { return std::isdigit(ch); }), str::npos);
        ASSERT_EQ(str::next_char("a12 e3", pos = str::npos, [](str::value_type ch) -> bool { return false; }), str::npos);
    }
}
