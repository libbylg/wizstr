#include "testing.hpp"

#include "str.hpp"

TEST(test_str, remove_eol_suffix_range) {
    SECTION("一般情况:LF") {
        ASSERT_EQ(str::remove_eol_suffix_range("abc\n"), (str::range_type{0, 3}));
        ASSERT_EQ(str::remove_eol_suffix_range("\n"), (str::range_type{0, 0}));
    }
    SECTION("一般情况:CR") {
        ASSERT_EQ(str::remove_eol_suffix_range("abc\r"), (str::range_type{0, 3}));
        ASSERT_EQ(str::remove_eol_suffix_range("\r"), (str::range_type{0, 0}));
    }
    SECTION("一般情况:CR_LF") {
        ASSERT_EQ(str::remove_eol_suffix_range("abc\r\n"), (str::range_type{0, 3}));
        ASSERT_EQ(str::remove_eol_suffix_range("\r\n"), (str::range_type{0, 0}));
    }
    SECTION("一般情况:无尾部换行") {
        ASSERT_EQ(str::remove_eol_suffix_range("abc"), (str::range_type{0, 3}));
    }
    SECTION("一般情况:空串") {
        ASSERT_EQ(str::remove_eol_suffix_range(""), (str::range_type{0, 0}));
    }
    SECTION("一般情况:\\0") {
        ASSERT_EQ(str::remove_eol_suffix_range(std::string_view{"\r\0", 2}), (str::range_type{0, 2}));
    }
    SECTION("尾部多个换行") {
        ASSERT_EQ(str::remove_eol_suffix_range("\r\r\r\n"), (str::range_type{0, 2}));
        ASSERT_EQ(str::remove_eol_suffix_range("\r\r\n\n"), (str::range_type{0, 3}));
        ASSERT_EQ(str::remove_eol_suffix_range("\n\n\r"), (str::range_type{0, 2}));
    }
}