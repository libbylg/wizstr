#include "testing.hpp"

#include "str.hpp"

TEST(test_str, eol_suffix) {
    SECTION("一般情况:LF") {
        ASSERT_EQ(str::eol_suffix("abc\n"), 1);
        ASSERT_EQ(str::eol_suffix("\n"), 1);
    }
    SECTION("一般情况:CR") {
        ASSERT_EQ(str::eol_suffix("abc\r"), 1);
        ASSERT_EQ(str::eol_suffix("\r"), 1);
    }
    SECTION("一般情况:CR_LF") {
        ASSERT_EQ(str::eol_suffix("abc\r\n"), 2);
        ASSERT_EQ(str::eol_suffix("\r\n"), 2);
    }
    SECTION("一般情况:无尾部换行") {
        ASSERT_EQ(str::eol_suffix("abc"), 0);
    }
    SECTION("一般情况:空串") {
        ASSERT_EQ(str::eol_suffix(""), 0);
    }
    SECTION("一般情况:\\0") {
        ASSERT_EQ(str::eol_suffix(std::string_view{"\r\0", 2}), 0);
    }
}