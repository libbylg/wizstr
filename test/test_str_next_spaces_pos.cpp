#include "testing.hpp"

#include "str.hpp"

TEST(test_str, next_spaces_pos) {
    SECTION("一般场景") {
        std::string_view s{"\r\nAA abc  AAd\tef \v AAA \rA\n\n"};
        str::size_type pos = 0;

        ASSERT_EQ(str::next_spaces_pos(s, pos), 0);
        ASSERT_TRUE((pos == 2));

        ASSERT_EQ(str::next_spaces_pos(s, pos), 4);
        ASSERT_TRUE((pos == 5));

        ASSERT_EQ(str::next_spaces_pos(s, pos), 8);
        ASSERT_TRUE((pos == 10));

        ASSERT_EQ(str::next_spaces_pos(s, pos), 13);
        ASSERT_TRUE((pos == 14));

        ASSERT_EQ(str::next_spaces_pos(s, pos), 16);
        ASSERT_TRUE((pos == 19));

        ASSERT_EQ(str::next_spaces_pos(s, pos), 22);
        ASSERT_TRUE((pos == 24));

        ASSERT_EQ(str::next_spaces_pos(s, pos), 25);
        ASSERT_TRUE((pos == 27));

        ASSERT_EQ(str::next_spaces_pos(s, pos), str::npos);
        ASSERT_TRUE((pos == 27));

        ASSERT_EQ(str::next_spaces_pos(s, pos), str::npos);
        ASSERT_TRUE((pos == 27));
    }
    SECTION("起始位置") {
        std::string_view s{"  aa  "};
        str::size_type pos = 0;

        ASSERT_EQ(str::next_spaces_pos(s, pos = 6), str::npos);
        ASSERT_TRUE((pos == 6));

        ASSERT_EQ(str::next_spaces_pos(s, pos = 7), str::npos);
        ASSERT_TRUE((pos == 6));

        ASSERT_EQ(str::next_spaces_pos(s, pos = str::npos), str::npos);
        ASSERT_TRUE((pos == 6));
    }
    SECTION("空串场景") {
        std::string_view s;
        str::size_type pos = 0;

        ASSERT_EQ(str::next_spaces_pos(s, pos = 0), str::npos);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::next_spaces_pos(s, pos = 1), str::npos);
        ASSERT_TRUE((pos == 0));

        ASSERT_EQ(str::next_spaces_pos(s, pos = str::npos), str::npos);
        ASSERT_TRUE((pos == 0));
    }
    SECTION("全空白场景") {
        std::string_view s{" \t\r\n  \r\v  "};
        str::size_type pos = 0;

        ASSERT_EQ(str::next_spaces_pos(s, pos = 0), 0);
        ASSERT_TRUE((pos == 10));

        ASSERT_EQ(str::next_spaces_pos(s, pos = 1), 1);
        ASSERT_TRUE((pos == 10));

        ASSERT_EQ(str::next_spaces_pos(s, pos = 5), 5);
        ASSERT_TRUE((pos == 10));

        ASSERT_EQ(str::next_spaces_pos(s, pos = 10), str::npos);
        ASSERT_TRUE((pos == 10));

        ASSERT_EQ(str::next_spaces_pos(s, pos = 11), str::npos);
        ASSERT_TRUE((pos == 10));

        ASSERT_EQ(str::next_spaces_pos(s, pos = str::npos), str::npos);
        ASSERT_TRUE((pos == 10));
    }
    SECTION("无空白场景") {
        std::string_view s{"ABC"};
        str::size_type pos = 0;

        ASSERT_EQ(str::next_spaces_pos(s, pos = 0), str::npos);
        ASSERT_TRUE((pos == 3));

        ASSERT_EQ(str::next_spaces_pos(s, pos = 1), str::npos);
        ASSERT_TRUE((pos == 3));

        ASSERT_EQ(str::next_spaces_pos(s, pos = 2), str::npos);
        ASSERT_TRUE((pos == 3));

        ASSERT_EQ(str::next_spaces_pos(s, pos = 3), str::npos);
        ASSERT_TRUE((pos == 3));

        ASSERT_EQ(str::next_spaces_pos(s, pos = 4), str::npos);
        ASSERT_TRUE((pos == 3));

        ASSERT_EQ(str::next_spaces_pos(s, pos = str::npos), str::npos);
        ASSERT_TRUE((pos == 3));
    }
}