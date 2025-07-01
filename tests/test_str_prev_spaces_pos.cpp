#include "testing.hpp"

#include "str.hpp"

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, prev_spaces_pos) {
    SECTION("一般场景") {
        std::string_view s{"\r\nAA abc  AAd\tef \v AAA \rA\n\n"};
        str::size_type pos = 0;

        ASSERT_EQ((str::prev_spaces_pos(s, pos = str::npos)), 27);
        ASSERT_TRUE((pos == 25));

        ASSERT_EQ((str::prev_spaces_pos(s, pos)), 24);
        ASSERT_TRUE((pos == 22));

        ASSERT_EQ(str::prev_spaces_pos(s, pos), 19);
        ASSERT_TRUE((pos == 16));

        ASSERT_EQ(str::prev_spaces_pos(s, pos), 14);
        ASSERT_TRUE((pos == 13));

        ASSERT_EQ(str::prev_spaces_pos(s, pos), 10);
        ASSERT_TRUE((pos == 8));

        ASSERT_EQ(str::prev_spaces_pos(s, pos), 5);
        ASSERT_TRUE((pos == 4));

        ASSERT_EQ(str::prev_spaces_pos(s, pos), 2);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::prev_spaces_pos(s, pos), 0);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::prev_spaces_pos(s, pos), 0);
        ASSERT_TRUE((pos == 0));
    }
    SECTION("起始位置") {
        std::string_view s{"  aa  "};
        str::size_type pos = 0;

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 6), 6);
        ASSERT_TRUE((pos == 4));

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 7), 6);
        ASSERT_TRUE((pos == 4));

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 0), 0);
        ASSERT_TRUE((pos == 0));
    }
    SECTION("空串场景") {
        std::string_view s;
        str::size_type pos = 0;

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 0), 0);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 1), 0);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 0), 0);
        ASSERT_TRUE((pos == 0));
    }
    SECTION("全空白场景") {
        std::string_view s{" \t\r\n  \r\v  "};
        str::size_type pos = 0;

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 0), 0);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 1), 1);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 5), 5);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 10), 10);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 11), 10);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::prev_spaces_pos(s, pos = str::npos), 10);
        ASSERT_TRUE((pos == 0));
    }
    SECTION("无空白场景") {
        std::string_view s{"ABC"};
        str::size_type pos = 0;

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 0), 0);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 1), 0);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 2), 0);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 3), 0);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::prev_spaces_pos(s, pos = 4), 0);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::prev_spaces_pos(s, pos = str::npos), 0);
        ASSERT_TRUE((pos == 0));
    }
}