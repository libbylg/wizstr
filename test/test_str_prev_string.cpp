#include "testing.hpp"

#include "str.hpp"

TEST(test_str, prev_string) {
    SECTION("不同起始位置") {
        str::size_type pos = 0;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::prev_string("abc def", pos = 0, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string("abc def", pos = 1, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string("abc def", pos = 2, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string("abc def", pos = 3, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string("abc def", pos = 4, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_TRUE(result = str::prev_string("abc def", pos = 5, "de"));
        ASSERT_TRUE((pos == 4) && (*result == "de"));

        ASSERT_TRUE(result = str::prev_string("abc def", pos = 6, "de"));
        ASSERT_TRUE((pos == 4) && (*result == "de"));

        ASSERT_TRUE(result = str::prev_string("abc def", pos = 7, "de"));
        ASSERT_TRUE((pos == 4) && (*result == "de"));

        ASSERT_TRUE(result = str::prev_string("abc def", pos = str::npos, "de"));
        ASSERT_TRUE((pos == 4) && (*result == "de"));
    }
    SECTION("无法匹配") {
        str::size_type pos = 0;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::prev_string("abc def", pos = str::npos, "KK"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string("ab", pos = str::npos, "abc"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string("ab", pos = str::npos, "bcd"));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("在空串中查找") {
        str::size_type pos = 0;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::prev_string("", pos = 0, "a"));
        ASSERT_TRUE(pos == 0);

        ASSERT_FALSE(result = str::prev_string("", pos = str::npos, ""));
        ASSERT_TRUE(pos == 0);
    }
    SECTION("查找空串") {
        str::size_type pos = 0;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::prev_string("abc", pos = 0, ""));
        ASSERT_TRUE(pos == 0);

        ASSERT_FALSE(result = str::prev_string("abcdef", pos = str::npos, ""));
        ASSERT_TRUE(pos == 0);
    }
}
