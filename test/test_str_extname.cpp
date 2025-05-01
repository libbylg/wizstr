#include "testing.hpp"

#include "str.hpp"


TEST(test_str, extname) {
    SECTION("简单裸名字") {
        ASSERT_EQ(str::extname("abc"), std::string(""));
    }
    SECTION("简单裸名字带扩展名") {
        ASSERT_EQ(str::extname("abc.ext"), std::string(".ext"));
    }
    SECTION("裸名字带路径") {
        ASSERT_EQ(str::extname("abc/def"), std::string(""));
    }
    SECTION("只有路径无basename") {
        ASSERT_EQ(str::extname("abc/"), std::string(""));
    }
    SECTION("裸名字带路径路径中有点") {
        ASSERT_EQ(str::extname("/abc.def/basename.ext"), std::string(".ext"));
    }
    SECTION("裸名字带路径路径中有点+") {
        ASSERT_EQ(str::extname("/abc.def/"), std::string(""));
    }
    SECTION("basename为隐藏文件，隐藏文件点开头不是只有扩展名") {
        ASSERT_EQ(str::extname("abc/.hidefile"), std::string(""));
    }
    SECTION("basename为隐藏文件，隐藏文件点开头，但是还有额外扩展名") {
        ASSERT_EQ(str::extname("abc/.hidefile.ext"), std::string(".ext"));
    }
    SECTION("basename带多个点") {
        ASSERT_EQ(str::extname("abc/normal-file.xxx.ext"), std::string(".ext"));
    }
    SECTION("basename位置只有点1") {
        ASSERT_EQ(str::extname("abc/."), std::string(""));
    }
    SECTION("basename位置只有点2") {
        ASSERT_EQ(str::extname("abc/.."), std::string("."));
    }
    SECTION("basename位置只有点3") {
        ASSERT_EQ(str::extname("..."), std::string("."));
    }
    SECTION("basename位置,点在最后1") {
        ASSERT_EQ(str::extname(".abc."), std::string("."));
    }
    SECTION("basename位置,点在最后2") {
        ASSERT_EQ(str::extname(".abc.."), std::string("."));
    }
    SECTION("basename位置,多点前缀1") {
        ASSERT_EQ(str::extname("..abc"), std::string(".abc"));
    }
    SECTION("basename位置,多点前缀1") {
        ASSERT_EQ(str::extname("...abc"), std::string(".abc"));
    }
    SECTION("空串") {
        ASSERT_EQ(str::extname(""), std::string(""));
    }
    SECTION("全空白字符") {
        ASSERT_EQ(str::extname(" \t "), std::string(""));
    }
}
