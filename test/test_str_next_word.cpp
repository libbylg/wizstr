#include "tester.hpp"

#include "str.hpp"

TEST(test_str, next_words1) {
    SECTION("不同起始位置") {
        size_t pos = 0;
        ASSERT_EQ(str::next_word("abc def", pos = 0), std::string("abc"));
        ASSERT_EQ(str::next_word("abc def", pos = 1), std::string("bc"));
        ASSERT_EQ(str::next_word("abc def", pos = 2), std::string("c"));
        ASSERT_EQ(str::next_word("abc def", pos = 3), std::string("def"));
        ASSERT_EQ(str::next_word("abc def", pos = 4), std::string("def"));
        ASSERT_EQ(str::next_word("abc def", pos = 5), std::string("ef"));
        ASSERT_EQ(str::next_word("abc def", pos = 6), std::string("f"));
        ASSERT_EQ(str::next_word("abc def", pos = 7), std::string(""));
        ASSERT_EQ(str::next_word("abc def", pos = str::npos), std::string(""));
    }
    SECTION("不同空格和多段字符串组合") {
        size_t pos = 0;

        ASSERT_EQ(str::next_word(" \r\t\n abc \r\t\n def \r\t\n ", pos = 0), std::string("abc"));
        ASSERT_EQ(str::next_word(" \r\t\n abc \r\t\n def \r\t\n ", pos = 8), std::string("def"));
        ASSERT_EQ(str::next_word(" \r\t\n abc \r\t\n def \r\t\n ", pos = 16), std::string(""));
    }
    SECTION("无任何空白的串") {
        size_t pos = 0;
        ASSERT_EQ(str::next_word("abc", pos = 0), std::string("abc"));
        ASSERT_EQ(str::next_word("abcdef", pos = str::npos), std::string(""));
    }
    SECTION("空串") {
        size_t pos = 0;
        ASSERT_EQ(str::next_word("", pos = 0), std::string(""));
        ASSERT_EQ(str::next_word("", pos = str::npos), std::string(""));
    }
    SECTION("全空白串") {
        size_t pos = 0;
        ASSERT_EQ(str::next_word(" \r\t\n ", pos = 1), std::string(""));
    }
}

TEST(test_str, next_words2) {
    SECTION("不同起始位置") {
        str::size_type pos = 0;
        ASSERT_EQ(str::next_word("abc def", pos), std::string("abc"));
        ASSERT_EQ(pos, 3);
        ASSERT_EQ(str::next_word("abc def", pos), std::string("def"));
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(str::next_word("abc def", pos), std::string(""));
        ASSERT_EQ(pos, 7);

        pos = str::npos;
        ASSERT_EQ(str::next_word("abc def", pos), std::string(""));
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(str::next_word("abc def", pos), std::string(""));
        ASSERT_EQ(pos, 7);
    }
    SECTION("不同空格和多段字符串组合") {
        str::size_type pos = 0;
        ASSERT_EQ(str::next_word(" \r\t\n abc \r\t\n def \r\t\n ", pos), std::string("abc"));
        ASSERT_EQ(pos, 8);
        ASSERT_EQ(str::next_word(" \r\t\n abc \r\t\n def \r\t\n ", pos), std::string("def"));
        ASSERT_EQ(pos, 16);
        ASSERT_EQ(str::next_word(" \r\t\n abc \r\t\n def \r\t\n ", pos), std::string(""));
        ASSERT_EQ(pos, 21);
    }
    SECTION("无任何空白的串") {
        str::size_type pos = 0;
        ASSERT_EQ(str::next_word("abc", pos), std::string("abc"));
        ASSERT_EQ(pos, 3);

        pos = str::npos;
        ASSERT_EQ(str::next_word("abcdef", pos), std::string(""));
        ASSERT_EQ(pos, 6);
    }
    SECTION("空串") {
        str::size_type pos = 0;
        ASSERT_EQ(str::next_word("", pos), std::string(""));
        ASSERT_EQ(pos, 0);

        pos = str::npos;
        ASSERT_EQ(str::next_word("", pos), std::string(""));
        ASSERT_EQ(pos, 0);
    }
    SECTION("全空白串") {
        str::size_type pos = 0;
        ASSERT_EQ(str::next_word(" \r\t\n ", pos), std::string(""));
        ASSERT_EQ(pos, 5);
    }
}