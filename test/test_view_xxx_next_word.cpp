#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, find_next_word) {
    SECTION("不同起始位置") {
        ASSERT_EQ(view::find_next_word("abc def", 0), std::string("abc"));
        ASSERT_EQ(view::find_next_word("abc def", 1), std::string("bc"));
        ASSERT_EQ(view::find_next_word("abc def", 2), std::string("c"));
        ASSERT_EQ(view::find_next_word("abc def", 3), std::string("def"));
        ASSERT_EQ(view::find_next_word("abc def", 4), std::string("def"));
        ASSERT_EQ(view::find_next_word("abc def", 5), std::string("ef"));
        ASSERT_EQ(view::find_next_word("abc def", 6), std::string("f"));
        ASSERT_EQ(view::find_next_word("abc def", 7), std::string(""));
        ASSERT_EQ(view::find_next_word("abc def", view::npos), std::string(""));
    }
    SECTION("不同空格和多段字符串组合") {
        ASSERT_EQ(view::find_next_word(" \r\t\n abc \r\t\n def \r\t\n ", 0), std::string("abc"));
        ASSERT_EQ(view::find_next_word(" \r\t\n abc \r\t\n def \r\t\n ", 8), std::string("def"));
        ASSERT_EQ(view::find_next_word(" \r\t\n abc \r\t\n def \r\t\n ", 16), std::string(""));
    }
    SECTION("无任何空白的串") {
        ASSERT_EQ(view::find_next_word("abc", 0), std::string("abc"));
        ASSERT_EQ(view::find_next_word("abcdef", view::npos), std::string(""));
    }
    SECTION("空串") {
        ASSERT_EQ(view::find_next_word("", 0), std::string(""));
        ASSERT_EQ(view::find_next_word("", view::npos), std::string(""));
    }
    SECTION("全空白串") {
        ASSERT_EQ(view::find_next_word(" \r\t\n ", 1), std::string(""));
    }
}

TEST(test_view, iter_next_word) {
    SECTION("不同起始位置") {
        view::size_type pos = 0;
        ASSERT_EQ(view::iter_next_word("abc def", pos), std::string("abc"));
        ASSERT_EQ(pos, 3);
        ASSERT_EQ(view::iter_next_word("abc def", pos), std::string("def"));
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(view::iter_next_word("abc def", pos), std::string(""));
        ASSERT_EQ(pos, 7);

        pos = view::npos;
        ASSERT_EQ(view::iter_next_word("abc def", pos), std::string(""));
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(view::iter_next_word("abc def", pos), std::string(""));
        ASSERT_EQ(pos, 7);
    }
    SECTION("不同空格和多段字符串组合") {
        view::size_type pos = 0;
        ASSERT_EQ(view::iter_next_word(" \r\t\n abc \r\t\n def \r\t\n ", pos), std::string("abc"));
        ASSERT_EQ(pos, 8);
        ASSERT_EQ(view::iter_next_word(" \r\t\n abc \r\t\n def \r\t\n ", pos), std::string("def"));
        ASSERT_EQ(pos, 16);
        ASSERT_EQ(view::iter_next_word(" \r\t\n abc \r\t\n def \r\t\n ", pos), std::string(""));
        ASSERT_EQ(pos, 21);
    }
    SECTION("无任何空白的串") {
        view::size_type pos = 0;
        ASSERT_EQ(view::iter_next_word("abc", pos), std::string("abc"));
        ASSERT_EQ(pos, 3);

        pos = view::npos;
        ASSERT_EQ(view::iter_next_word("abcdef", pos), std::string(""));
        ASSERT_EQ(pos, 6);
    }
    SECTION("空串") {
        view::size_type pos = 0;
        ASSERT_EQ(view::iter_next_word("", pos), std::string(""));
        ASSERT_EQ(pos, 0);

        pos = view::npos;
        ASSERT_EQ(view::iter_next_word("", pos), std::string(""));
        ASSERT_EQ(pos, 0);
    }
    SECTION("全空白串") {
        view::size_type pos = 0;
        ASSERT_EQ(view::iter_next_word(" \r\t\n ", pos), std::string(""));
        ASSERT_EQ(pos, 5);
    }
}