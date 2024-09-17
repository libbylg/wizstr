#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::align_left") {
    SECTION("一般情况：字符串左对齐，右边填充") {
        REQUIRE(view::align_left("HelloWorld", 13, '*') == "HelloWorld***");
    }

    SECTION("宽度和原字符串相同：字符串不变") {
        REQUIRE(view::align_left("HelloWorld", 10, '*') == "HelloWorld");
    }

    SECTION("宽度太窄时保持原样") {
        REQUIRE(view::align_left("HelloWorld", 9, '*') == "HelloWorld");
    }

    SECTION("空字符串") {
        REQUIRE(view::align_left("", 9, '*') == "*********");
    }

    SECTION("对齐宽度为0") {
        REQUIRE(view::align_left("HelloWorld", 0, '*') == "HelloWorld");
        REQUIRE(view::align_left("", 0, '*') == "");
    }
}

TEST_CASE("view::align_right") {
    SECTION("一般情况：字符串右对齐，左边填充") {
        REQUIRE(view::align_right("HelloWorld", 13, '*') == "***HelloWorld");
    }

    SECTION("宽度和原字符串相同：字符串不变") {
        REQUIRE(view::align_right("HelloWorld", 10, '*') == "HelloWorld");
    }

    SECTION("宽度太窄时保持原样") {
        REQUIRE(view::align_right("HelloWorld", 9, '*') == "HelloWorld");
    }

    SECTION("空字符串") {
        REQUIRE(view::align_right("", 9, '*') == "*********");
    }

    SECTION("对齐宽度为0") {
        REQUIRE(view::align_right("HelloWorld", 0, '*') == "HelloWorld");
        REQUIRE(view::align_right("", 0, '*') == "");
    }
}

TEST_CASE("view::align_center") {
    SECTION("一般情况：字符串居中对齐对齐，两边填充") {
        REQUIRE(view::align_center("HelloWorld", 13, '*') == "*HelloWorld**");
        REQUIRE(view::align_center("HelloWorld", 14, '*') == "**HelloWorld**");
        REQUIRE(view::align_center("Hello", 13, '*') == "****Hello****");
        REQUIRE(view::align_center("Hello", 14, '*') == "****Hello*****");
    }

    SECTION("宽度和原字符串相同：字符串不变") {
        REQUIRE(view::align_center("HelloWorld", 10, '*') == "HelloWorld");
    }

    SECTION("宽度太窄时保持原样") {
        REQUIRE(view::align_center("HelloWorld", 9, '*') == "HelloWorld");
    }

    SECTION("空字符串") {
        REQUIRE(view::align_center("", 9, '*') == "*********");
    }

    SECTION("对齐宽度为0") {
        REQUIRE(view::align_center("HelloWorld", 0, '*') == "HelloWorld");
        REQUIRE(view::align_center("", 0, '*') == "");
    }
}

TEST_CASE("view::align_zfill") {
    SECTION("一般情况：字符串右对齐，左边填充") {
        REQUIRE(view::align_zfill("123", 8) == "00000123");
        REQUIRE(view::align_zfill("+123", 8) == "+0000123");
        REQUIRE(view::align_zfill("-123", 8) == "-0000123");
    }

    SECTION("宽度和原字符串相同：字符串不变") {
        REQUIRE(view::align_zfill("123", 3) == "123");
        REQUIRE(view::align_zfill("+123", 3) == "+123");
        REQUIRE(view::align_zfill("-123", 3) == "-123");

        REQUIRE(view::align_zfill("123", 4) == "0123");
        REQUIRE(view::align_zfill("+123", 4) == "+123");
        REQUIRE(view::align_zfill("-123", 4) == "-123");

        REQUIRE(view::align_zfill("123", 5) == "00123");
        REQUIRE(view::align_zfill("+123", 5) == "+0123");
        REQUIRE(view::align_zfill("-123", 5) == "-0123");
    }

    SECTION("宽度太窄时保持原样") {
        REQUIRE(view::align_zfill("1234567890", 9) == "1234567890");
    }

    SECTION("空字符串") {
        REQUIRE(view::align_zfill("", 5) == "00000");
    }

    SECTION("对齐宽度为0") {
        REQUIRE(view::align_zfill("123", 0) == "123");
        REQUIRE(view::align_zfill("", 0) == "");
    }

    SECTION("孤立正负号") {
        REQUIRE(view::align_zfill("+", 4) == "+000");
        REQUIRE(view::align_zfill("-", 4) == "-000");
    }
}
