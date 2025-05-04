#include "testing.hpp"

#include "str.hpp"

TEST(test_str, next_string) {
    SECTION("一般场景") {
        std::string_view s{"AAabcAAdefAAAA"};
        str::size_type pos = 0;
        std::optional<std::string> result;

        ASSERT_TRUE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 1) && (*result == "AA"));

        ASSERT_TRUE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 6) && (*result == "AA"));

        ASSERT_TRUE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 11) && (*result == "AA"));

        ASSERT_TRUE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 12) && (*result == "AA"));

        ASSERT_TRUE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 13) && (*result == "AA"));

        ASSERT_FALSE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 14));

        ASSERT_FALSE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 14));
    }
    SECTION("pos超过范围:1") {
        std::string_view s{"AAabcAAdefAAAA"};
        size_t pos = 14;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 14));
    }
    SECTION("pos超过范围:npos") {
        std::string_view s{"AAabcAAdefAAAA"};
        size_t pos = str::npos;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 14));
    }
    SECTION("在空串中查找") {
        std::string_view s{""};
        size_t pos = 14;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("查找空串") {
        std::string_view s{"abc"};
        str::size_type pos = 0;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::next_string(s, pos, ""));
        ASSERT_TRUE((pos == 3));

        ASSERT_FALSE(result = str::next_string(s, pos, ""));
        ASSERT_TRUE((pos == 3));
    }
}