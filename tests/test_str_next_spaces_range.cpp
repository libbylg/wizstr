#include "testing.hpp"

#include "str.hpp"

#if defined(WIZSTR_NAMESPACE)
using str = WIZSTR_NAMESPACE::str;
#endif

TEST(test_str, next_spaces_range) {
    SECTION("一般场景") {
        std::string_view s{"\r\nAA abc  AAd\tef \v AAA \rA\n\n"};
        str::size_type pos = 0;
        std::optional<str::range_type> range;

        ASSERT_TRUE((range = str::next_spaces_range(s, pos)));
        ASSERT_EQ(range, (str::range(0, 2)));

        ASSERT_TRUE((range = str::next_spaces_range(s, pos)));
        ASSERT_EQ(range, (str::range(4,  1)));

        ASSERT_TRUE((range = str::next_spaces_range(s, pos)));
        ASSERT_EQ(range, (str::range(8, 2)));

        ASSERT_TRUE((range = str::next_spaces_range(s, pos)));
        ASSERT_EQ(range, (str::range(13, 1)));

        ASSERT_TRUE((range = str::next_spaces_range(s, pos)));
        ASSERT_EQ(range, (str::range(16, 3)));

        ASSERT_TRUE((range = str::next_spaces_range(s, pos)));
        ASSERT_EQ(range, (str::range(22, 2)));

        ASSERT_TRUE((range = str::next_spaces_range(s, pos)));
        ASSERT_EQ(range, (str::range(25, 2)));

        ASSERT_FALSE((range = str::next_spaces_range(s, pos)));
        //ASSERT_EQ(range, (str::range(str::npos, 0)));

        ASSERT_FALSE((range = str::next_spaces_range(s, pos)));
        //ASSERT_EQ(range, (str::range(str::npos, 0)));
    }
    SECTION("起始位置") {
        std::string_view s{"  aa  "};
        str::size_type pos = 0;
        std::optional<str::range_type> range;

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = 6)));
        //ASSERT_EQ(range, (str::range(str::npos, 0)));

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = 7)));
        //ASSERT_EQ(range, (str::range(str::npos, 0)));

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = str::npos)));
        //ASSERT_EQ(range, (str::range(str::npos, 0)));
    }
    SECTION("空串场景") {
        std::string_view s;
        str::size_type pos = 0;
        std::optional<str::range_type> range;

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = 0)));
        // ASSERT_EQ(range, (str::range(str::npos, 0)));

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = 1)));
        // ASSERT_EQ(range, (str::range(str::npos, 0)));

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = str::npos)));
        // ASSERT_EQ(range, (str::range(str::npos, 0)));
    }
    SECTION("全空白场景") {
        std::string_view s{" \t\r\n  \r\v  "};
        str::size_type pos = 0;
        std::optional<str::range_type> range;

        ASSERT_TRUE((range = str::next_spaces_range(s, pos = 0)));
        ASSERT_EQ(range, (str::range(0, 10)));

        ASSERT_TRUE((range = str::next_spaces_range(s, pos = 1)));
        ASSERT_EQ(range, (str::range(1, 9)));

        ASSERT_TRUE((range = str::next_spaces_range(s, pos = 5)));
        ASSERT_EQ(range, (str::range(5, 5)));

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = 10)));
        // ASSERT_EQ(range, (str::range(str::npos, 0)));

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = 11)));
        // ASSERT_EQ(range, (str::range(str::npos, 0)));

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = str::npos)));
        // ASSERT_EQ(range, (str::range(str::npos, 0)));
    }
    SECTION("无空白场景") {
        std::string_view s{"ABC"};
        str::size_type pos = 0;
        std::optional<str::range_type> range;

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = 0)));
        // ASSERT_EQ(range, (str::range(str::npos, 0)));

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = 1)));
        // ASSERT_EQ(range, (str::range(str::npos, 0)));

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = 2)));
        // ASSERT_EQ(range, (str::range(str::npos, 0)));

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = 3)));
        // ASSERT_EQ(range, (str::range(str::npos, 0)));

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = 4)));
        // ASSERT_EQ(range, (str::range(str::npos, 0)));

        ASSERT_FALSE((range = str::next_spaces_range(s, pos = str::npos)));
        // ASSERT_EQ(range, (str::range(str::npos, 0)));
    }
}