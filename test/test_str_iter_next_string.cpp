#include "tester.hpp"

#include "str.hpp"


TEST(test_str, iter_next_string) {
    SECTION("一般场景") {
        std::string_view s{"AAabcAAdefAAAA"};
        size_t pos = 0;

        ASSERT_EQ(str::iter_next_string(s, pos, "AA"), 0);
        ASSERT_EQ(pos, 2);

        ASSERT_EQ(str::iter_next_string(s, pos, "AA"), 5);
        ASSERT_EQ(pos, 7);

        ASSERT_EQ(str::iter_next_string(s, pos, "AA"), 10);
        ASSERT_EQ(pos, 12);

        ASSERT_EQ(str::iter_next_string(s, pos, "AA"), 12);
        ASSERT_EQ(pos, 14);

        ASSERT_EQ(str::iter_next_string(s, pos, "AA"), str::npos);
        ASSERT_EQ(pos, 14);

        ASSERT_EQ(str::iter_next_string(s, pos, "AA"), str::npos);
        ASSERT_EQ(pos, 14);
    }
    SECTION("pos超过范围:1") {
        std::string_view s{"AAabcAAdefAAAA"};
        size_t pos = 14;

        ASSERT_EQ(str::iter_next_string(s, pos, "AA"), str::npos);
        ASSERT_EQ(pos, 14);
    }
    SECTION("pos超过范围:npos") {
        std::string_view s{"AAabcAAdefAAAA"};
        size_t pos = str::npos;

        ASSERT_EQ(str::iter_next_string(s, pos, "AA"), str::npos);
        ASSERT_EQ(pos, 14);
    }
    SECTION("空串") {
        std::string_view s{""};
        size_t pos = 14;

        ASSERT_EQ(str::iter_next_string(s, pos, "AA"), str::npos);
        ASSERT_EQ(pos, 0);
    }
    SECTION("空串") {
        std::string_view s{"AACDEF"};
        size_t pos = 0;

        ASSERT_EQ(str::iter_next_string(s, pos, "AA"), 0);
        ASSERT_EQ(pos, 2);

        ASSERT_EQ(str::iter_next_string(s, pos, "AA"), str::npos);
        ASSERT_EQ(pos, 6);
    }
}