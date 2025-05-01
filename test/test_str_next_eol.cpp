#include "testing.hpp"

#include "str.hpp"

TEST(test_str, xxx_next_eol) {
    SECTION("一般情况:LF") {
        std::string_view s{"a\nb\nc"};
        str::size_type pos = 0;
        ASSERT_EQ(str::next_eol(s, pos), "\n");
        ASSERT_EQ(pos, 2);
        ASSERT_EQ(str::next_eol(s, pos), "\n");
        ASSERT_EQ(pos, 4);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 5);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 5);
    }
    SECTION("\\r+\\n") {
        std::string_view s{"a\r\nb\nc"};
        str::size_type pos = 0;
        ASSERT_EQ(str::next_eol(s, pos), "\r\n");
        ASSERT_EQ(pos, 3);
        ASSERT_EQ(str::next_eol(s, pos), "\n");
        ASSERT_EQ(pos, 5);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 6);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 6);
    }
    SECTION("\\r+\\r") {
        std::string_view s{"a\r\rb\nc"};
        str::size_type pos = 0;
        ASSERT_EQ(str::next_eol(s, pos), "\r");
        ASSERT_EQ(pos, 2);
        ASSERT_EQ(str::next_eol(s, pos), "\r");
        ASSERT_EQ(pos, 3);
        ASSERT_EQ(str::next_eol(s, pos), "\n");
        ASSERT_EQ(pos, 5);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 6);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 6);
    }
    SECTION("\\n+\\n") {
        std::string_view s{"a\n\nb\nc"};
        str::size_type pos = 0;
        ASSERT_EQ(str::next_eol(s, pos), "\n");
        ASSERT_EQ(pos, 2);
        ASSERT_EQ(str::next_eol(s, pos), "\n");
        ASSERT_EQ(pos, 3);
        ASSERT_EQ(str::next_eol(s, pos), "\n");
        ASSERT_EQ(pos, 5);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 6);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 6);
    }
    SECTION("\\n+\\r") {
        std::string_view s{"a\n\rb\nc"};
        str::size_type pos = 0;
        ASSERT_EQ(str::next_eol(s, pos), "\n");
        ASSERT_EQ(pos, 2);
        ASSERT_EQ(str::next_eol(s, pos), "\r");
        ASSERT_EQ(pos, 3);
        ASSERT_EQ(str::next_eol(s, pos), "\n");
        ASSERT_EQ(pos, 5);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 6);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 6);
    }
    SECTION("空串") {
        std::string_view s{""};
        str::size_type pos = 0;
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 0);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 0);
    }
    SECTION("无任何换行符号") {
        std::string_view s{"abcdef"};

        str::size_type pos = 0;
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 6);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 6);
    }
    SECTION("单一\\r") {
        std::string_view s{"\r"};

        str::size_type pos = 0;
        ASSERT_EQ(str::next_eol(s, pos), "\r");
        ASSERT_EQ(pos, 1);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 1);
    }
    SECTION("单一\\n") {
        std::string_view s{"\n"};
        str::size_type pos = 0;
        ASSERT_EQ(str::next_eol(s, pos), "\n");
        ASSERT_EQ(pos, 1);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 1);
    }
    SECTION("综合") {
        std::string_view s{"\n\r\n\r\r\r\n"};

        str::size_type pos = 0;
        ASSERT_EQ(str::next_eol(s, pos), "\n");
        ASSERT_EQ(pos, 1);
        ASSERT_EQ(str::next_eol(s, pos), "\r\n");
        ASSERT_EQ(pos, 3);
        ASSERT_EQ(str::next_eol(s, pos), "\r");
        ASSERT_EQ(pos, 4);
        ASSERT_EQ(str::next_eol(s, pos), "\r");
        ASSERT_EQ(pos, 5);
        ASSERT_EQ(str::next_eol(s, pos), "\r\n");
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 7);
    }
    SECTION("pos超出字符串范围[1]") {
        std::string_view s{"a\nb\nc"};
        str::size_type pos = 5;
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 5);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 5);
    }
    SECTION("pos超出字符串范围[2]") {
        std::string_view s{"a\nb\nc"};
        str::size_type pos = 10;
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 5);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 5);
    }
    SECTION("pos超出字符串范围[3]") {
        std::string_view s{"a\nb\nc"};
        str::size_type pos = str::npos;
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 5);
        ASSERT_EQ(str::next_eol(s, pos), "");
        ASSERT_EQ(pos, 5);
    }
}