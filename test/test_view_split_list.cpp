#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"
#include "test-compares.h"

TEST_CASE("view::split_list:vector") {
    SECTION("一般情况") {
        REQUIRE(view::split_list("a,b,c", ",") == std::vector<std::string_view>{"a", "b", "c"});
    }
    SECTION("被拆分字符串全部是分隔符") {
        REQUIRE(view::split_list(",,", ",") == std::vector<std::string_view>{"", "", ""});
    }
    SECTION("没有替换分个符") {
        REQUIRE(view::split_list("abc", ",") == std::vector<std::string_view>{"abc"});
    }
    SECTION("分隔符号在开头") {
        REQUIRE(view::split_list(",abc", ",") == std::vector<std::string_view>{"", "abc"});
    }
    SECTION("分隔符号在结尾") {
        REQUIRE(view::split_list("abc,", ",") == std::vector<std::string_view>{"abc", ""});
    }
    SECTION("拆分空串") {
        REQUIRE(view::split_list("", ",") == std::vector<std::string_view>{""});
    }
    SECTION("被拆分字符串无对应分隔符") {
        REQUIRE(view::split_list("abcdef#@#$", ",") == std::vector<std::string_view>{"abcdef#@#$"});
    }
    // SECTION("空串作为分隔符") {
    //     REQUIRE(view::split_list("abcdef", "") == std::vector<std::string_view>{"a", "b", "c", "d", "e", "f"});
    // }
    SECTION("限制值为0") {
        REQUIRE(view::split_list(":aa::bb:cc:", ":", 0) == std::vector<std::string_view>{":aa::bb:cc:"});
    }
    SECTION("限制值合适") {
        REQUIRE(view::split_list(":aa::bb:cc:", ":", 4) == std::vector<std::string_view>{"", "aa", "", "bb", "cc:"});
    }
    SECTION("限制值超过总段数") {
        REQUIRE(view::split_list(":aa::bb:cc:", ":", 100) == std::vector<std::string_view>{"", "aa", "", "bb", "cc", ""});
    }
}

TEST_CASE("view::split_list:proc") {
    SECTION("简单场景1") {
        std::vector<std::string_view> result;
        view::split_list("HelloWorld-HelloWorld", "-", [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });
        REQUIRE(result == std::vector<std::string_view>{"HelloWorld", "HelloWorld"});
    }
    SECTION("简单场景2") {
        std::vector<std::string_view> result;
        view::split_list("abcdbceebc", std::string("bc"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == std::vector<std::string_view>{"a", "d", "ee", ""});
    }

    SECTION("刚好匹配一个") {
        std::vector<std::string_view> result;
        view::split_list(std::string("bc"), "bc", [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });
        REQUIRE(result == std::vector<std::string_view>{"", ""});
    }

    SECTION("前面字段") {
        std::vector<std::string_view> result;
        view::split_list("AAAbc", std::string("bc"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == std::vector<std::string_view>{"AAA", ""});
    }

    SECTION("后面字段") {
        std::vector<std::string_view> result;
        view::split_list("bcAAA", std::string("bc"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == std::vector<std::string_view>{"", "AAA"});
    }

    SECTION("拆分空串") {
        std::vector<std::string_view> result;
        view::split_list("", std::string("bc"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == std::vector<std::string_view>{""});
    }

    SECTION("连续匹配") {
        std::vector<std::string_view> result;
        view::split_list("AAbcbcBB", std::string("bc"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == std::vector<std::string_view>{"AA", "", "BB"});
    }

    SECTION("连续匹配2") {
        std::vector<std::string_view> result;
        view::split_list("bcbc", std::string("bc"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == std::vector<std::string_view>{"", "", ""});
    }

    SECTION("无法匹配") {
        std::vector<std::string_view> result;
        view::split_list("defgui", std::string("bc"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == std::vector<std::string_view>{"defgui"});
    }

    SECTION("提前结束:一般场景") {
        std::vector<std::string_view> result;
        view::split_list("|123|456||def|ghi|", std::string("|"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            if (result.size() == 4) {
                return -1;
            }
            return 0;
        });

        REQUIRE(result == std::vector<std::string_view>{"", "123", "456", ""});
    }
}