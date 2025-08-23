#include "testing.hpp"

#include "str.hpp"

#if defined(WIZSTR_NAMESPACE)
using str = WIZSTR_NAMESPACE::str;
#endif

TEST(test_str, next_spaces_view) {
    SECTION("一般场景") {
        std::string_view s{"\r\nAA abc  AAd\tef \v AAA \rA\n\n"};
        str::size_type pos = 0;
        std::optional<std::string_view> range;

        ASSERT_TRUE((range = str::next_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 2 )&& (*range == ("\r\n")));

        ASSERT_TRUE((range = str::next_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 5) && (*range == " "));

        ASSERT_TRUE((range = str::next_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 10) && (*range == "  "));

        ASSERT_TRUE((range = str::next_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 14) && (*range == "\t"));

        ASSERT_TRUE((range = str::next_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 19) && (*range == " \v "));

        ASSERT_TRUE((range = str::next_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 24) && (*range == " \r"));

        ASSERT_TRUE((range = str::next_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 27) && (*range == "\n\n"));

        ASSERT_FALSE((range = str::next_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 27));

        ASSERT_FALSE((range = str::next_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 27));

    }
    SECTION("起始位置") {
        std::string_view s{"  aa  "};
        str::size_type pos = 0;
        std::optional<std::string_view> range;

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = 6)));
        ASSERT_TRUE((pos == 6));

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = 7)));
        ASSERT_TRUE((pos == 6));

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = str::npos)));
        ASSERT_TRUE((pos == 6));
    }
    SECTION("空串场景") {
        std::string_view s;
        str::size_type pos = 0;
        std::optional<std::string_view> range;

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = 0)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = 1)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = str::npos)));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("全空白场景") {
        std::string_view s{" \t\r\n  \r\v  "};
        str::size_type pos = 0;
        std::optional<std::string_view> range;

        ASSERT_TRUE((range = str::next_spaces_view(s, pos = 0)));
        ASSERT_TRUE((pos == 10) && (*range == " \t\r\n  \r\v  "));

        ASSERT_TRUE((range = str::next_spaces_view(s, pos = 1)));
        ASSERT_TRUE((pos == 10) && (*range == "\t\r\n  \r\v  "));

        ASSERT_TRUE((range = str::next_spaces_view(s, pos = 5)));
        ASSERT_TRUE((pos == 10) && (*range == " \r\v  "));

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = 10)));
        ASSERT_TRUE((pos == 10));

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = 11)));
        ASSERT_TRUE((pos == 10));

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = str::npos)));
        ASSERT_TRUE((pos == 10));
    }
    SECTION("无空白场景") {
        std::string_view s{"ABC"};
        str::size_type pos = 0;
        std::optional<std::string_view> range;

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = 0)));
        ASSERT_TRUE((pos == 3));

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = 1)));
        ASSERT_TRUE((pos == 3));

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = 2)));
        ASSERT_TRUE((pos == 3));

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = 3)));
        ASSERT_TRUE((pos == 3));

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = 4)));
        ASSERT_TRUE((pos == 3));

        ASSERT_FALSE((range = str::next_spaces_view(s, pos = str::npos)));
        ASSERT_TRUE((pos == 3));
    }
}