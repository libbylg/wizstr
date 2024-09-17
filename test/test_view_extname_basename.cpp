#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::extname") {
    SECTION("简单裸名字") {
        REQUIRE(view::extname("abc") == std::string(""));
    }
    SECTION("简单裸名字带扩展名") {
        REQUIRE(view::extname("abc.ext") == std::string(".ext"));
    }
    SECTION("裸名字带路径") {
        REQUIRE(view::extname("abc/def") == std::string(""));
    }
    SECTION("只有路径无basename") {
        REQUIRE(view::extname("abc/") == std::string(""));
    }
    SECTION("裸名字带路径路径中有点") {
        REQUIRE(view::extname("/abc.def/basename.ext") == std::string(".ext"));
    }
    SECTION("裸名字带路径路径中有点+") {
        REQUIRE(view::extname("/abc.def/") == std::string(""));
    }
    SECTION("basename为隐藏文件，隐藏文件点开头不是只有扩展名") {
        REQUIRE(view::extname("abc/.hidefile") == std::string(""));
    }
    SECTION("basename为隐藏文件，隐藏文件点开头，但是还有额外扩展名") {
        REQUIRE(view::extname("abc/.hidefile.ext") == std::string(".ext"));
    }
    SECTION("basename带多个点") {
        REQUIRE(view::extname("abc/normal-file.xxx.ext") == std::string(".ext"));
    }
    SECTION("basename位置只有点1") {
        REQUIRE(view::extname("abc/.") == std::string(""));
    }
    SECTION("basename位置只有点2") {
        REQUIRE(view::extname("abc/..") == std::string("."));
    }
    SECTION("basename位置只有点3") {
        REQUIRE(view::extname("...") == std::string("."));
    }
    SECTION("basename位置,点在最后1") {
        REQUIRE(view::extname(".abc.") == std::string("."));
    }
    SECTION("basename位置,点在最后2") {
        REQUIRE(view::extname(".abc..") == std::string("."));
    }
    SECTION("basename位置,多点前缀1") {
        REQUIRE(view::extname("..abc") == std::string(".abc"));
    }
    SECTION("basename位置,多点前缀1") {
        REQUIRE(view::extname("...abc") == std::string(".abc"));
    }
    SECTION("空串") {
        REQUIRE(view::extname("") == std::string(""));
    }
    SECTION("全空白字符") {
        REQUIRE(view::extname(" \t ") == std::string(""));
    }
}
