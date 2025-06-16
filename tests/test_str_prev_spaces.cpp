#include "testing.hpp"

#include "str.hpp"

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, prev_spaces) {
    SECTION("一般场景") {
        std::string_view s{"\r\nAA abc  AAd\tef \v AAA \rA\n\n"};
        str::size_type pos = 0;
        std::optional<str::size_type> end;

        ASSERT_TRUE((end = str::prev_spaces(s, pos = str::npos)));
        ASSERT_TRUE((pos == 25) && (*end == 27));

        ASSERT_TRUE((end = str::prev_spaces(s, pos)));
        ASSERT_TRUE((pos == 22) && (*end == 24));

        ASSERT_TRUE((end = str::prev_spaces(s, pos)));
        ASSERT_TRUE((pos == 16) && (*end == 19));

        ASSERT_TRUE((end = str::prev_spaces(s, pos)));
        ASSERT_TRUE((pos == 13) && (*end == 14));

        ASSERT_TRUE((end = str::prev_spaces(s, pos)));
        ASSERT_TRUE((pos == 8) && (*end == 10));

        ASSERT_TRUE((end = str::prev_spaces(s, pos)));
        ASSERT_TRUE((pos == 4) && (*end == 5));

        ASSERT_TRUE((end = str::prev_spaces(s, pos)));
        ASSERT_TRUE((pos == 0) && (*end == 2));

        ASSERT_FALSE((end = str::prev_spaces(s, pos)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((end = str::prev_spaces(s, pos)));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("起始位置") {
        std::string_view s{"  aa  "};
        str::size_type pos = 0;
        std::optional<str::size_type> end;

        ASSERT_TRUE((end = str::prev_spaces(s, pos = 6)));
        ASSERT_TRUE((pos == 4)&& (*end == 6));

        ASSERT_TRUE((end = str::prev_spaces(s, pos = 7)));
        ASSERT_TRUE((pos == 4)&& (*end == 6));

        ASSERT_FALSE((end = str::prev_spaces(s, pos = 0)));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("空串场景") {
        std::string_view s;
        str::size_type pos = 0;
        std::optional<str::size_type> end;

        ASSERT_FALSE((end = str::prev_spaces(s, pos = 0)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((end = str::prev_spaces(s, pos = 1)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((end = str::prev_spaces(s, pos = str::npos)));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("全空白场景") {
        std::string_view s{" \t\r\n  \r\v  "};
        str::size_type pos = 0;
        std::optional<str::size_type> end;

        ASSERT_FALSE((end = str::prev_spaces(s, pos = 0)));
        ASSERT_TRUE((pos == 0));

        ASSERT_TRUE((end = str::prev_spaces(s, pos = 1)));
        ASSERT_TRUE((pos == 0)&& (*end == 1));

        ASSERT_TRUE((end = str::prev_spaces(s, pos = 5)));
        ASSERT_TRUE((pos == 0)&& (*end == 5));

        ASSERT_TRUE((end = str::prev_spaces(s, pos = 10)));
        ASSERT_TRUE((pos == 0)&& (*end == 10));

        ASSERT_TRUE((end = str::prev_spaces(s, pos = 11)));
        ASSERT_TRUE((pos == 0)&& (*end == 10));

        ASSERT_TRUE((end = str::prev_spaces(s, pos = str::npos)));
        ASSERT_TRUE((pos == 0)&& (*end == 10));
    }
    SECTION("无空白场景") {
        std::string_view s{"ABC"};
        str::size_type pos = 0;
        std::optional<str::size_type> end;

        ASSERT_FALSE((end = str::prev_spaces(s, pos = 0)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((end = str::prev_spaces(s, pos = 1)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((end = str::prev_spaces(s, pos = 2)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((end = str::prev_spaces(s, pos = 3)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((end = str::prev_spaces(s, pos = 4)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((end = str::prev_spaces(s, pos = str::npos)));
        ASSERT_TRUE((pos == 0));
    }
}