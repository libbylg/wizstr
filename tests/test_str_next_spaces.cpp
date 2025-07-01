#include "testing.hpp"

#include "str.hpp"

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, next_spaces) {
    SECTION("一般场景") {
        std::string_view s{"\r\nAA abc  AAd\tef \v AAA \rA\n\n"};
        str::size_type pos = 0;
        std::optional<str::size_type> start;

        ASSERT_TRUE((start = str::next_spaces(s, pos)));
        ASSERT_TRUE((pos == 2) && (*start == 0));

        ASSERT_TRUE((start = str::next_spaces(s, pos)));
        ASSERT_TRUE((pos == 5) && (*start == 4));

        ASSERT_TRUE((start = str::next_spaces(s, pos)));
        ASSERT_TRUE((pos == 10) && (*start == 8));

        ASSERT_TRUE((start = str::next_spaces(s, pos)));
        ASSERT_TRUE((pos == 14) && (*start == 13));

        ASSERT_TRUE((start = str::next_spaces(s, pos)));
        ASSERT_TRUE((pos == 19) && (*start == 16));

        ASSERT_TRUE((start = str::next_spaces(s, pos)));
        ASSERT_TRUE((pos == 24) && (*start == 22));

        ASSERT_TRUE((start = str::next_spaces(s, pos)));
        ASSERT_TRUE((pos == 27) && (*start == 25));

        ASSERT_FALSE((start = str::next_spaces(s, pos)));
        ASSERT_TRUE((pos == 27));

        ASSERT_FALSE((start = str::next_spaces(s, pos)));
        ASSERT_TRUE((pos == 27));
    }
    SECTION("起始位置") {
        std::string_view s{"  aa  "};
        str::size_type pos = 0;
        std::optional<str::size_type> start;

        ASSERT_FALSE((start = str::next_spaces(s, pos = 6)));
        ASSERT_TRUE((pos == 6));

        ASSERT_FALSE((start = str::next_spaces(s, pos = 7)));
        ASSERT_TRUE((pos == 6));

        ASSERT_FALSE((start = str::next_spaces(s, pos = str::npos)));
        ASSERT_TRUE((pos == 6));
    }
    SECTION("空串场景") {
        std::string_view s;
        str::size_type pos = 0;
        std::optional<str::size_type> start;

        ASSERT_FALSE((start = str::next_spaces(s, pos = 0)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((start = str::next_spaces(s, pos = 1)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((start = str::next_spaces(s, pos = str::npos)));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("全空白场景") {
        std::string_view s{" \t\r\n  \r\v  "};
        str::size_type pos = 0;
        std::optional<str::size_type> start;

        ASSERT_TRUE((start = str::next_spaces(s, pos = 0)));
        ASSERT_TRUE((pos == 10) && (*start == 0));

        ASSERT_TRUE((start = str::next_spaces(s, pos = 1)));
        ASSERT_TRUE((pos == 10) && (*start == 1));

        ASSERT_TRUE((start = str::next_spaces(s, pos = 5)));
        ASSERT_TRUE((pos == 10) && (*start == 5));

        ASSERT_FALSE((start = str::next_spaces(s, pos = 10)));
        ASSERT_TRUE((pos == 10));

        ASSERT_FALSE((start = str::next_spaces(s, pos = 11)));
        ASSERT_TRUE((pos == 10));

        ASSERT_FALSE((start = str::next_spaces(s, pos = str::npos)));
        ASSERT_TRUE((pos == 10));
    }
    SECTION("无空白场景") {
        std::string_view s{"ABC"};
        str::size_type pos = 0;
        std::optional<str::size_type> start;

        ASSERT_FALSE((start = str::next_spaces(s, pos = 0)));
        ASSERT_TRUE((pos == 3));

        ASSERT_FALSE((start = str::next_spaces(s, pos = 1)));
        ASSERT_TRUE((pos == 3));

        ASSERT_FALSE((start = str::next_spaces(s, pos = 2)));
        ASSERT_TRUE((pos == 3));

        ASSERT_FALSE((start = str::next_spaces(s, pos = 3)));
        ASSERT_TRUE((pos == 3));

        ASSERT_FALSE((start = str::next_spaces(s, pos = 4)));
        ASSERT_TRUE((pos == 3));

        ASSERT_FALSE((start = str::next_spaces(s, pos = str::npos)));
        ASSERT_TRUE((pos == 3));
    }
}