#include "testing.hpp"

#include "str.hpp"

#if defined(WIZSTR_NAMESPACE)
using str = WIZSTR_NAMESPACE::str;
#endif

TEST(test_str, prev_spaces_view) {
    SECTION("一般场景") {
        std::string_view s{"\r\nAA abc  AAd\tef \v AAA \rA\n\n"};
        str::size_type pos = 0;
        std::optional<std::string_view> item;

        ASSERT_TRUE((item = str::prev_spaces_view(s, pos = str::npos)));
        ASSERT_TRUE((pos == 25) && (*item ==  "\n\n"));

        ASSERT_TRUE((item = str::prev_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 22) && (item ==  " \r"));

        ASSERT_TRUE((item = str::prev_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 16) && (item == " \v "));

        ASSERT_TRUE((item = str::prev_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 13) && (item == "\t"));

        ASSERT_TRUE((item = str::prev_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 8) && (item ==  "  "));

        ASSERT_TRUE((item = str::prev_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 4) && (item ==  " "));

        ASSERT_TRUE((item = str::prev_spaces_view(s, pos)));
        ASSERT_TRUE((pos == 0) && (item ==  "\r\n"));

        ASSERT_FALSE((item = str::prev_spaces_view(s, pos)));
        ASSERT_TRUE(pos == 0);

        ASSERT_FALSE((item = str::prev_spaces_view(s, pos)));
        ASSERT_TRUE(pos == 0);
    }
    SECTION("起始位置") {
        std::string_view s{"  aa  "};
        str::size_type pos = 0;

        ASSERT_FALSE(str::prev_spaces_view(s, pos = 0));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("空串场景") {
        std::string_view s;
        str::size_type pos = 0;
        std::optional<std::string_view> item;

        ASSERT_FALSE((item = str::prev_spaces_view(s, pos = 0)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((item = str::prev_spaces_view(s, pos = 1u)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((item = str::prev_spaces_view(s, pos = str::npos)));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("全空白场景") {
        std::string_view s{" \t\r\n  \r\v  "};
        str::size_type pos = 0;
        std::optional<std::string_view> item;

        ASSERT_FALSE((item = str::prev_spaces_view(s, pos = 0)));
        ASSERT_TRUE((pos == 0));

        ASSERT_TRUE((item = str::prev_spaces_view(s, pos = 1)));
        ASSERT_TRUE((pos == 0) && (item == " "));

        ASSERT_TRUE((item = str::prev_spaces_view(s, pos = 5)));
        ASSERT_TRUE((pos == 0) && (item == " \t\r\n "));

        ASSERT_TRUE((item =str::prev_spaces_view(s, pos = 10)));
        ASSERT_TRUE((pos == 0) && (item == " \t\r\n  \r\v  "));

        ASSERT_TRUE((item = str::prev_spaces_view(s, pos = 11)));
        ASSERT_TRUE((pos == 0) && (item == " \t\r\n  \r\v  "));

        ASSERT_TRUE((item = str::prev_spaces_view(s, pos = str::npos)));
        ASSERT_TRUE((pos == 0) && (item == " \t\r\n  \r\v  "));
    }
    SECTION("无空白场景") {
        std::string_view s{"ABC"};
        str::size_type pos = 0;
        std::optional<std::string_view> item;

        ASSERT_FALSE((item = str::prev_spaces_view(s, pos = 0)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((item = str::prev_spaces_view(s, pos = 1)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((item = str::prev_spaces_view(s, pos = 2)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((item = str::prev_spaces_view(s, pos = 3)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((item = str::prev_spaces_view(s, pos = 4)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((item = str::prev_spaces_view(s, pos = str::npos)));
        ASSERT_TRUE((pos == 0));
    }
}