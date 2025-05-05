#include "testing.hpp"

#include "str.hpp"

TEST(test_str, remove_eol_suffix_view) {
    SECTION("一般情况:LF") {
        ASSERT_EQ(str::remove_eol_suffix_view("abc\n"), "abc");
        ASSERT_EQ(str::remove_eol_suffix_view("\n"), "");
    }
    SECTION("一般情况:CR") {
        ASSERT_EQ(str::remove_eol_suffix_view("abc\r"), "abc");
        ASSERT_EQ(str::remove_eol_suffix_view("\r"), "");
    }
    SECTION("一般情况:CR_LF") {
        ASSERT_EQ(str::remove_eol_suffix_view("abc\r\n"), "abc");
        ASSERT_EQ(str::remove_eol_suffix_view("\r\n"), "");
    }
    SECTION("一般情况:无尾部换行") {
        ASSERT_EQ(str::remove_eol_suffix_view("abc"), "abc");
    }
    SECTION("一般情况:空串") {
        ASSERT_EQ(str::remove_eol_suffix_view(""), "");
    }
    SECTION("一般情况:\\0") {
        ASSERT_EQ(str::remove_eol_suffix_view(std::string_view{"\r\0", 2}), (std::string_view{"\r\0", 2}));
    }
    SECTION("尾部多个换行") {
        ASSERT_EQ(str::remove_eol_suffix_view("\r\r\r\n"), "\r\r");
        ASSERT_EQ(str::remove_eol_suffix_view("\r\r\n\n"), "\r\r\n");
        ASSERT_EQ(str::remove_eol_suffix_view("\n\n\r"), "\n\n");
    }
}