#include "testing.hpp"

#include "str.hpp"

TEST(test_str, next_word_range) {
    SECTION("不同起始位置") {
        str::size_type pos = 0;
        ASSERT_EQ(str::next_word_range("abc def", pos = 0), str::range(0, 3));
        ASSERT_EQ(pos, 3);
        ASSERT_EQ(str::next_word_range("abc def", pos = 1), str::range(1, 2));
        ASSERT_EQ(pos, 3);
        ASSERT_EQ(str::next_word_range("abc def", pos = 2), str::range(2, 1));
        ASSERT_EQ(pos, 3);
        ASSERT_EQ(str::next_word_range("abc def", pos = 3), str::range(4, 3));
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(str::next_word_range("abc def", pos = 4), str::range(4, 3));
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(str::next_word_range("abc def", pos = 5), str::range(5, 2));
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(str::next_word_range("abc def", pos = 6), str::range(6, 1));
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(str::next_word_range("abc def", pos = 7), str::range(0, 0));
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(str::next_word_range("abc def", pos = str::npos), str::range(0, 0));
        ASSERT_EQ(pos, 7);
    }
    SECTION("不同空格和多段字符串组合") {
        str::size_type pos = 0;

        ASSERT_EQ(str::next_word_range(" \r\t\n abc \r\t\n def \r\t\n ", pos = 0), str::range(5, 3));
        ASSERT_EQ(pos, 8);
        ASSERT_EQ(str::next_word_range(" \r\t\n abc \r\t\n def \r\t\n ", pos = 8), str::range(13, 3));
        ASSERT_EQ(pos, 16);
        ASSERT_EQ(str::next_word_range(" \r\t\n abc \r\t\n def \r\t\n ", pos = 16), str::range(0, 0));
        ASSERT_EQ(pos, 21);
    }
    SECTION("无任何空白的串") {
        str::size_type pos = 0;
        ASSERT_EQ(str::next_word_range("abc", pos = 0), str::range(0, 3));
        ASSERT_EQ(pos, 3);
        ASSERT_EQ(str::next_word_range("abcdef", pos = str::npos), str::range(0, 0));
        ASSERT_EQ(pos, 6);
    }
    SECTION("空串") {
        str::size_type pos = 0;
        ASSERT_EQ(str::next_word_range("", pos = 0), str::range(0, 0));
        ASSERT_EQ(pos, 0);
        ASSERT_EQ(str::next_word_range("", pos = str::npos), str::range(0, 0));
        ASSERT_EQ(pos, 0);
    }
    SECTION("全空白串") {
        str::size_type pos = 0;
        ASSERT_EQ(str::next_word_range(" \r\t\n ", pos = 1), str::range(0, 0));
        ASSERT_EQ(pos, 5);
    }
}
