#include "tester.hpp"

#include "str.hpp"
#include "test-compares.h"


TEST(test_str, split_list_vector) {
    SECTION("一般情况") {
        ASSERT_EQ(str::split_list("a,b,c", ","), (std::vector<std::string_view>{"a", "b", "c"}));
    }
    SECTION("被拆分字符串全部是分隔符") {
        ASSERT_EQ(str::split_list(",,", ","), (std::vector<std::string_view>{"", "", ""}));
    }
    SECTION("没有替换分个符") {
        ASSERT_EQ(str::split_list("abc", ","), (std::vector<std::string_view>{"abc"}));
    }
    SECTION("分隔符号在开头") {
        ASSERT_EQ(str::split_list(",abc", ","), (std::vector<std::string_view>{"", "abc"}));
    }
    SECTION("分隔符号在结尾") {
        ASSERT_EQ(str::split_list("abc,", ","), (std::vector<std::string_view>{"abc", ""}));
    }
    SECTION("拆分空串") {
        ASSERT_EQ(str::split_list("", ","), (std::vector<std::string_view>{""}));
    }
    SECTION("被拆分字符串无对应分隔符") {
        ASSERT_EQ(str::split_list("abcdef#@#$", ","), (std::vector<std::string_view>{"abcdef#@#$"}));
    }
    SECTION("空串作为分隔符") {
        ASSERT_EQ(str::split_list("ab,cdef", ""), (std::vector<std::string_view>{"ab", "cdef"}));
    }
    SECTION("限制值为0") {
        ASSERT_EQ(str::split_list(":aa::bb:cc:", ":", 0), (std::vector<std::string_view>{":aa::bb:cc:"}));
    }
    SECTION("限制值合适") {
        ASSERT_EQ(str::split_list(":aa::bb:cc:", ":", 4), (std::vector<std::string_view>{"", "aa", "", "bb", "cc:"}));
    }
    SECTION("限制值超过总段数") {
        ASSERT_EQ(str::split_list(":aa::bb:cc:", ":", 100), (std::vector<std::string_view>{"", "aa", "", "bb", "cc", ""}));
    }
}

TEST(test_str, split_list_proc) {
    SECTION("简单场景1") {
        std::vector<std::string_view> result;
        str::split_list("HelloWorld-HelloWorld", "-", [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });
        ASSERT_EQ(result, (std::vector<std::string_view>{"HelloWorld", "HelloWorld"}));
    }
    SECTION("简单场景2") {
        std::vector<std::string_view> result;
        str::split_list("abcdbceebc", std::string("bc"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        ASSERT_EQ(result, (std::vector<std::string_view>{"a", "d", "ee", ""}));
    }

    SECTION("刚好匹配一个") {
        std::vector<std::string_view> result;
        str::split_list(std::string("bc"), "bc", [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });
        ASSERT_EQ(result, (std::vector<std::string_view>{"", ""}));
    }

    SECTION("前面字段") {
        std::vector<std::string_view> result;
        str::split_list("AAAbc", std::string("bc"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        ASSERT_EQ(result, (std::vector<std::string_view>{"AAA", ""}));
    }

    SECTION("后面字段") {
        std::vector<std::string_view> result;
        str::split_list("bcAAA", std::string("bc"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        ASSERT_EQ(result, (std::vector<std::string_view>{"", "AAA"}));
    }

    SECTION("拆分空串") {
        std::vector<std::string_view> result;
        str::split_list("", std::string("bc"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        ASSERT_EQ(result, (std::vector<std::string_view>{""}));
    }

    SECTION("连续匹配") {
        std::vector<std::string_view> result;
        str::split_list("AAbcbcBB", std::string("bc"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        ASSERT_EQ(result, (std::vector<std::string_view>{"AA", "", "BB"}));
    }

    SECTION("连续匹配2") {
        std::vector<std::string_view> result;
        str::split_list("bcbc", std::string("bc"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        ASSERT_EQ(result, (std::vector<std::string_view>{"", "", ""}));
    }

    SECTION("无法匹配") {
        std::vector<std::string_view> result;
        str::split_list("defgui", std::string("bc"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        ASSERT_EQ(result, (std::vector<std::string_view>{"defgui"}));
    }

    SECTION("提前结束:一般场景") {
        std::vector<std::string_view> result;
        str::split_list("|123|456||def|ghi|", std::string("|"), [&result](std::string_view item) -> int {
            result.emplace_back(item);
            if (result.size() == 4) {
                return -1;
            }
            return 0;
        });

        ASSERT_EQ(result, (std::vector<std::string_view>{"", "123", "456", ""}));
    }
}

TEST(test_str, split_list_regex) {
    SECTION("一般场景") {
        std::regex sep("[ \t]*(:|,|;)[ \t]*");
        ASSERT_EQ(str::split_list("aaa, bbb:ccc ;ddd ", sep), (std::vector<std::string_view>{"aaa", "bbb", "ccc", "ddd "}));
        ASSERT_EQ(str::split_list("aaa, bbb:ccc ;ddd ", sep, 0), (std::vector<std::string_view>{"aaa, bbb:ccc ;ddd "}));
        ASSERT_EQ(str::split_list("aaa, bbb:ccc ;ddd ", sep, 1), (std::vector<std::string_view>{"aaa", "bbb:ccc ;ddd "}));
        ASSERT_EQ(str::split_list("aaa, bbb:ccc ;ddd ", sep, 2), (std::vector<std::string_view>{"aaa", "bbb", "ccc ;ddd "}));
        ASSERT_EQ(str::split_list("aaa, bbb:ccc ;ddd ", sep, 3), (std::vector<std::string_view>{"aaa", "bbb", "ccc", "ddd "}));
        ASSERT_EQ(str::split_list("aaa, bbb:ccc ;ddd ", sep, 4), (std::vector<std::string_view>{"aaa", "bbb", "ccc", "ddd "}));
        ASSERT_EQ(str::split_list("aaa, bbb:ccc ;ddd ", sep, str::npos), (std::vector<std::string_view>{"aaa", "bbb", "ccc", "ddd "}));
    }
    SECTION("空串") {
        std::regex sep(":|,|;");
        ASSERT_EQ(str::split_list("", sep), (std::vector<std::string_view>{""}));
        ASSERT_EQ(str::split_list("", sep, 0), (std::vector<std::string_view>{""}));
        ASSERT_EQ(str::split_list("", sep, 3), (std::vector<std::string_view>{""}));
    }
    SECTION("proc:不限拆分次数") {
        std::regex sep("[ \t]*(:|,|;)[ \t]*");
        std::vector<std::string_view> result;
        str::split_list("aaa, bbb:ccc ;ddd ", sep, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });
        ASSERT_EQ(result, (std::vector<std::string_view>{"aaa", "bbb", "ccc", "ddd "}));
    }
    SECTION("proc:限制拆分次数+提前返回") {
        std::regex sep("[ \t]*(:|,|;)[ \t]*");
        std::vector<std::string_view> result;
        str::split_list("aaa, bbb:ccc ;ddd ", sep, 2, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            if (result.size() == 2) { // max_n 限制不起作用,这里的约束条件会提前触发,导致提前结束,所以只存两个,尾部的数据直接丢弃
                return -1;
            }
            return 0;
        });
        ASSERT_EQ(result, (std::vector<std::string_view>{"aaa", "bbb"}));
    }
    SECTION("proc:限制拆分次数+提前返回+限制次数较小") {
        std::regex sep("[ \t]*(:|,|;)[ \t]*");
        std::vector<std::string_view> result;
        str::split_list("aaa, bbb:ccc ;ddd,MNP", sep, 2, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            if (result.size() == 4) { // max_n 提前返回,限制起作用，实际能存三个
                return -1;
            }
            return 0;
        });
        ASSERT_EQ(result, (std::vector<std::string_view>{"aaa", "bbb", "ccc ;ddd,MNP"}));
    }
    SECTION("全都是分隔符") {
        std::regex sep(":|,|;");
        ASSERT_EQ(str::split_list("::,,;;", sep), (std::vector<std::string_view>{"", "", "", "", "", "", ""}));
    }
    SECTION("分隔符刚好在尾部") {
        std::regex sep(":|,|;");
        ASSERT_EQ(str::split_list("aaa:bbb,ccc;", sep), (std::vector<std::string_view>{"aaa", "bbb", "ccc", ""}));
    }
    SECTION("分隔符刚好在头部") {
        std::regex sep(":|,|;");
        ASSERT_EQ(str::split_list(":aaa:bbb,ccc", sep), (std::vector<std::string_view>{"", "aaa", "bbb", "ccc"}));
    }
}