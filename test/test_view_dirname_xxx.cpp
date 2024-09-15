#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::dirname&view::basename") {
    SECTION("全路径") {
        REQUIRE(view::dirname("/aaa/bbb/ccc") == "/aaa/bbb");
        REQUIRE(view::basename("/aaa/bbb/ccc") == "ccc");
    }
    SECTION("相对路径[1]") {
        REQUIRE(view::dirname("aaa/bbb/ccc") == "aaa/bbb");
        REQUIRE(view::basename("aaa/bbb/ccc") == "ccc");
    }
    SECTION("相对路径[2]") {
        REQUIRE(view::dirname("../bbb/ccc") == "../bbb");
        REQUIRE(view::basename("../bbb/ccc") == "ccc");
    }
    SECTION("相对路径[3]") {
        REQUIRE(view::dirname("../ccc") == "..");
        REQUIRE(view::basename("../ccc") == "ccc");
    }
    SECTION("相对路径[4]") {
        REQUIRE(view::dirname("./ccc") == ".");
        REQUIRE(view::basename("./ccc") == "ccc");
    }
    SECTION(".和..[1]") {
        REQUIRE(view::dirname(".") == "");
        REQUIRE(view::basename(".") == "");
    }
    SECTION(".和..[2]") {
        REQUIRE(view::dirname("..") == "");
        REQUIRE(view::basename("..") == "");
    }
    SECTION("无路径分隔符") {
        REQUIRE(view::dirname("abc") == ".");
        REQUIRE(view::basename("abc") == ".");
    }
    SECTION("./和../[1]") {
        REQUIRE(view::dirname("./") == ".");
        REQUIRE(view::basename("./") == ".");
    }
    SECTION("./和../[2]") {
        REQUIRE(view::dirname("../") == ".");
        REQUIRE(view::basename("../") == ".");
    }
    SECTION("./和../[3]") {
        REQUIRE(view::dirname("./aa") == ".");
        REQUIRE(view::basename("./aa") == "aa");
    }
    SECTION("./和../[4]") {
        REQUIRE(view::dirname("../aa") == "..");
        REQUIRE(view::basename("../aa") == "aa");
    }
    SECTION("绝对路径 /") {
        REQUIRE(view::dirname("/") == "/");
        REQUIRE(view::basename("/") == "/");
    }
    SECTION("空串") {
        REQUIRE(view::dirname("") == "");
        REQUIRE(view::basename("") == "");
    }
    SECTION("多余的路径元素[1]") {
        REQUIRE(view::dirname("///ccc") == "/");
        REQUIRE(view::basename("///ccc") == "/");
    }
    SECTION("多余的路径元素[2]") {
        REQUIRE(view::dirname(".///ccc") == ".");
        REQUIRE(view::basename(".///ccc") == ".");
    }
}