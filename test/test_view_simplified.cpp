#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::simplified") {
    SECTION("简单测试") {
        std::string a("\t  A abbCC中华人   民共  和 \t国   ");
        view::simplified(a);
        REQUIRE(a == "A abbCC中华人 民共 和 国");
    }

    SECTION("没什么可以替代的") {
        std::string a("AabbCC中华人民共和国");
        REQUIRE(view::simplified(a) == "AabbCC中华人民共和国");
    }

    SECTION("空串") {
        std::string a("");
        REQUIRE(view::simplified(a) == "");
    }

    SECTION("全空白") {
        std::string a("   \t  \n \r \v");
        REQUIRE(view::simplified(a) == "");
    }

    SECTION("简单测试") {
        REQUIRE(view::simplified("\t  A abbCC中华人   民共  和 \t国   ") == "A abbCC中华人 民共 和 国");
    }

    SECTION("没什么可以替代的") {
        REQUIRE(view::simplified("AabbCC中华人民共和国") == "AabbCC中华人民共和国");
    }

    SECTION("空串") {
        REQUIRE(view::simplified("") == "");
    }

    SECTION("全空白") {
        REQUIRE(view::simplified("   \t  \n \r \v") == "");
    }
}