#include "testing.hpp"

#include "str.hpp"

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, prev_spaces_range) {
    SECTION("一般场景") {
        std::string_view s{"\r\nAA abc  AAd\tef \v AAA \rA\n\n"};
        str::size_type pos = 0;
        std::optional<str::range_type> range;

        ASSERT_TRUE((range = str::prev_spaces_range(s, pos = str::npos)));
        ASSERT_TRUE((pos == 25) && (range ==  (str::range(25, 2))));

        ASSERT_TRUE((range = str::prev_spaces_range(s, pos)));
        ASSERT_TRUE((pos == 22) && (range ==  (str::range(22, 2))));

        ASSERT_TRUE((range = str::prev_spaces_range(s, pos)));
        ASSERT_TRUE((pos == 16) && (range ==  (str::range(16, 3))));

        ASSERT_TRUE((range = str::prev_spaces_range(s, pos)));
        ASSERT_TRUE((pos == 13) && (range ==  (str::range(13, 1))));

        ASSERT_TRUE((range = str::prev_spaces_range(s, pos)));
        ASSERT_TRUE((pos == 8) && (range ==  (str::range(8, 2))));

        ASSERT_TRUE((range = str::prev_spaces_range(s, pos)));
        ASSERT_TRUE((pos == 4) && (range ==  (str::range(4, 1))));

        ASSERT_TRUE((range = str::prev_spaces_range(s, pos)));
        ASSERT_TRUE((pos == 0) && (range ==  (str::range(0, 2))));

        ASSERT_FALSE((range = str::prev_spaces_range(s, pos)));
        ASSERT_TRUE(pos == 0);

        ASSERT_FALSE((range = str::prev_spaces_range(s, pos)));
        ASSERT_TRUE(pos == 0);
    }
    SECTION("起始位置") {
        std::string_view s{"  aa  "};
        str::size_type pos = 0;

        ASSERT_FALSE(str::prev_spaces_range(s, pos = 0));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("空串场景") {
        std::string_view s;
        str::size_type pos = 0;
        std::optional<str::range_type> range;

        ASSERT_FALSE((range = str::prev_spaces_range(s, pos = 0)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((range = str::prev_spaces_range(s, pos = 1u)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((range = str::prev_spaces_range(s, pos = str::npos)));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("全空白场景") {
        std::string_view s{" \t\r\n  \r\v  "};
        str::size_type pos = 0;
        std::optional<str::range_type> range;

        ASSERT_FALSE((range = str::prev_spaces_range(s, pos = 0)));
        ASSERT_TRUE((pos == 0));

        ASSERT_TRUE((range = str::prev_spaces_range(s, pos = 1)));
        ASSERT_TRUE((pos == 0) && (range == str::range(0, 1)));

        ASSERT_TRUE((range = str::prev_spaces_range(s, pos = 5)));
        ASSERT_TRUE((pos == 0) && (range == str::range(0, 5)));

        ASSERT_TRUE((range =str::prev_spaces_range(s, pos = 10)));
        ASSERT_TRUE((pos == 0) && (range == str::range(0, 10)));

        ASSERT_TRUE((range = str::prev_spaces_range(s, pos = 11)));
        ASSERT_TRUE((pos == 0) && (range == str::range(0, 10)));

        ASSERT_TRUE((range = str::prev_spaces_range(s, pos = str::npos)));
        ASSERT_TRUE((pos == 0) && (range == str::range(0, 10)));
    }
    SECTION("无空白场景") {
        std::string_view s{"ABC"};
        str::size_type pos = 0;
        std::optional<str::range_type> range;

        ASSERT_FALSE((range = str::prev_spaces_range(s, pos = 0)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((range = str::prev_spaces_range(s, pos = 1)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((range = str::prev_spaces_range(s, pos = 2)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((range = str::prev_spaces_range(s, pos = 3)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((range = str::prev_spaces_range(s, pos = 4)));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE((range = str::prev_spaces_range(s, pos = str::npos)));
        ASSERT_TRUE((pos == 0));
    }
}