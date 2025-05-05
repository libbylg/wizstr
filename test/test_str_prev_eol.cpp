#include "testing.hpp"

#include "str.hpp"

TEST(test_str, prev_eol) {
    SECTION("一般情况:LF") {
        std::string_view s{"a\nb\nc"};
        str::size_type pos = str::npos;
        std::optional<std::string> result;

        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 3) && (*result == "\n"));
        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 1) && (*result == "\n"));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("\\r+\\n") {
        std::string_view s{"a\r\nb\nc"};
        str::size_type pos = str::npos;
        std::optional<std::string> result;

        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 4) && (*result == "\n"));
        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 1) && (*result == "\r\n"));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("\\r+\\r") {
        std::string_view s{"a\r\rb\nc"};
        str::size_type pos = str::npos;
        std::optional<std::string> result;

        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 4) && (*result == "\n"));
        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 2) && (*result == "\r"));
        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 1) && (*result == "\r"));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("\\n+\\n") {
        std::string_view s{"a\n\nb\nc"};
        str::size_type pos = str::npos;
        std::optional<std::string> result;

        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 4) && (*result == "\n"));
        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 2) && (*result == "\n"));
        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 1) && (*result == "\n"));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("\\n+\\r") {
        std::string_view s{"a\n\rb\nc"};
        str::size_type pos = str::npos;
        std::optional<std::string> result;

        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 4) && (*result == "\n"));
        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 2) && (*result == "\r"));
        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 1) && (*result == "\n"));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("空串") {
        std::string_view s{""};
        str::size_type pos = str::npos;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("无任何换行符号") {
        std::string_view s{"abcdef"};
        str::size_type pos = str::npos;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("单一\\r") {
        std::string_view s{"\r"};
        str::size_type pos = str::npos;
        std::optional<std::string> result;

        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0) && (*result == "\r"));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("单一\\n") {
        std::string_view s{"\n"};
        str::size_type pos = str::npos;
        std::optional<std::string> result;

        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0) && (*result == "\n"));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("综合") {
        std::string_view s{"\n\r\n\r\r\r\n"};
        str::size_type pos = str::npos;
        std::optional<std::string> result;

        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 5) && (*result == "\r\n"));
        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 4) && (*result == "\r"));
        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 3) && (*result == "\r"));
        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 1) && (*result == "\r\n"));
        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0) && (*result == "\n"));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("pos超出字符串范围[1]") {
        std::string_view s{"aKb\nc"};
        str::size_type pos = 5;
        std::optional<std::string> result;

        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 3)&& (*result == "\n"));
        ASSERT_FALSE(result = str::prev_eol(s, pos = 1));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("pos超出字符串范围[2]") {
        std::string_view s{"aaKbb\nc"};
        str::size_type pos = 10;
        std::optional<std::string> result;

        ASSERT_TRUE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 5) && (*result == "\n"));
        ASSERT_FALSE(result = str::prev_eol(s, pos));
        ASSERT_TRUE((pos == 0));
    }
}