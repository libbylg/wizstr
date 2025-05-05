#include "testing.hpp"

#include "str.hpp"

TEST(test_str, ends_with_eol) {
    SECTION("一般情况:LF") {
        ASSERT_TRUE(str::ends_with_eol("abc\n"));
        ASSERT_TRUE(str::ends_with_eol("\n"));
    }
    SECTION("一般情况:CR") {
        ASSERT_TRUE(str::ends_with_eol("abc\r"));
        ASSERT_TRUE(str::ends_with_eol("\r"));
    }
    SECTION("一般情况:CR_LF") {
        ASSERT_TRUE(str::ends_with_eol("abc\r\n"));
        ASSERT_TRUE(str::ends_with_eol("\r\n"));
    }
    SECTION("一般情况:无尾部换行") {
        ASSERT_FALSE(str::ends_with_eol("abc"));
    }
    SECTION("一般情况:空串") {
        ASSERT_FALSE(str::ends_with_eol(""));
    }
    SECTION("一般情况:\\0") {
        ASSERT_FALSE(str::ends_with_eol(std::string_view{"\r\0", 2}));
    }
}