#include "tester.hpp"

#include "str.hpp"


TEST(test_str, find_next_words) {
    SECTION("不同起始位置") {
        ASSERT_EQ(str::find_next_words("abc def", 0), std::string("abc"));
        ASSERT_EQ(str::find_next_words("abc def", 1), std::string("bc"));
        ASSERT_EQ(str::find_next_words("abc def", 2), std::string("c"));
        ASSERT_EQ(str::find_next_words("abc def", 3), std::string("def"));
        ASSERT_EQ(str::find_next_words("abc def", 4), std::string("def"));
        ASSERT_EQ(str::find_next_words("abc def", 5), std::string("ef"));
        ASSERT_EQ(str::find_next_words("abc def", 6), std::string("f"));
        ASSERT_EQ(str::find_next_words("abc def", 7), std::string(""));
        ASSERT_EQ(str::find_next_words("abc def", str::npos), std::string(""));
    }
    SECTION("不同空格和多段字符串组合") {
        ASSERT_EQ(str::find_next_words(" \r\t\n abc \r\t\n def \r\t\n ", 0), std::string("abc"));
        ASSERT_EQ(str::find_next_words(" \r\t\n abc \r\t\n def \r\t\n ", 8), std::string("def"));
        ASSERT_EQ(str::find_next_words(" \r\t\n abc \r\t\n def \r\t\n ", 16), std::string(""));
    }
    SECTION("无任何空白的串") {
        ASSERT_EQ(str::find_next_words("abc", 0), std::string("abc"));
        ASSERT_EQ(str::find_next_words("abcdef", str::npos), std::string(""));
    }
    SECTION("空串") {
        ASSERT_EQ(str::find_next_words("", 0), std::string(""));
        ASSERT_EQ(str::find_next_words("", str::npos), std::string(""));
    }
    SECTION("全空白串") {
        ASSERT_EQ(str::find_next_words(" \r\t\n ", 1), std::string(""));
    }
}

TEST(test_str, iter_next_words) {
    SECTION("不同起始位置") {
        str::size_type pos = 0;
        ASSERT_EQ(str::iter_next_words("abc def", pos), std::string("abc"));
        ASSERT_EQ(pos, 3);
        ASSERT_EQ(str::iter_next_words("abc def", pos), std::string("def"));
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(str::iter_next_words("abc def", pos), std::string(""));
        ASSERT_EQ(pos, 7);

        pos = str::npos;
        ASSERT_EQ(str::iter_next_words("abc def", pos), std::string(""));
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(str::iter_next_words("abc def", pos), std::string(""));
        ASSERT_EQ(pos, 7);
    }
    SECTION("不同空格和多段字符串组合") {
        str::size_type pos = 0;
        ASSERT_EQ(str::iter_next_words(" \r\t\n abc \r\t\n def \r\t\n ", pos), std::string("abc"));
        ASSERT_EQ(pos, 8);
        ASSERT_EQ(str::iter_next_words(" \r\t\n abc \r\t\n def \r\t\n ", pos), std::string("def"));
        ASSERT_EQ(pos, 16);
        ASSERT_EQ(str::iter_next_words(" \r\t\n abc \r\t\n def \r\t\n ", pos), std::string(""));
        ASSERT_EQ(pos, 21);
    }
    SECTION("无任何空白的串") {
        str::size_type pos = 0;
        ASSERT_EQ(str::iter_next_words("abc", pos), std::string("abc"));
        ASSERT_EQ(pos, 3);

        pos = str::npos;
        ASSERT_EQ(str::iter_next_words("abcdef", pos), std::string(""));
        ASSERT_EQ(pos, 6);
    }
    SECTION("空串") {
        str::size_type pos = 0;
        ASSERT_EQ(str::iter_next_words("", pos), std::string(""));
        ASSERT_EQ(pos, 0);

        pos = str::npos;
        ASSERT_EQ(str::iter_next_words("", pos), std::string(""));
        ASSERT_EQ(pos, 0);
    }
    SECTION("全空白串") {
        str::size_type pos = 0;
        ASSERT_EQ(str::iter_next_words(" \r\t\n ", pos), std::string(""));
        ASSERT_EQ(pos, 5);
    }
}