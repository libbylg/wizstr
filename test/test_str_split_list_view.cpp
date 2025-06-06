/**
 * Copyright (c) 2021-2024 libbylg@126.com
 * str is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "testing.hpp"

#include "str.hpp"

#include "test-utils.hpp"

TEST(test_str, split_list_view) {
    GROUP("container场景") {
        SECTION("一般情况") {
            ASSERT_EQ(str::split_list_view("a,b,c"), (std::vector<std::string>{"a", "b", "c"}));
        }
        SECTION("被拆分字符串全部是分隔符") {
            ASSERT_EQ(str::split_list_view(",,"), (std::vector<std::string>{"", "", ""}));
        }
        SECTION("没有替换分个符") {
            ASSERT_EQ(str::split_list_view("abc"), (std::vector<std::string>{"abc"}));
        }
        SECTION("分隔符号在开头") {
            ASSERT_EQ(str::split_list_view(",abc"), (std::vector<std::string>{"", "abc"}));
        }
        SECTION("分隔符号在结尾") {
            ASSERT_EQ(str::split_list_view("abc,"), (std::vector<std::string>{"abc", ""}));
        }
        SECTION("拆分空串") {
            ASSERT_EQ(str::split_list_view(""), (std::vector<std::string>{""}));
        }
        SECTION("被拆分字符串无对应分隔符") {
            ASSERT_EQ(str::split_list_view("abcdef#@#$"), (std::vector<std::string>{"abcdef#@#$"}));
        }
        SECTION("空串作为分隔符") {
            ASSERT_EQ(str::split_list_view("ab,cdef"), (std::vector<std::string>{"ab", "cdef"}));
        }
        SECTION("限制值为0") {
            ASSERT_EQ(str::split_list_view(",aa,,bb,cc,", 0), (std::vector<std::string>{",aa,,bb,cc,"}));
        }
        SECTION("限制值合适") {
            ASSERT_EQ(str::split_list_view(",aa,,bb,cc,", 4), (std::vector<std::string>{"", "aa", "", "bb", "cc,"}));
        }
        SECTION("限制值超过总段数") {
            ASSERT_EQ(str::split_list_view(",aa,,bb,cc,", 100), (std::vector<std::string>{"", "aa", "", "bb", "cc", ""}));
        }
    }
    //
    //    GROUP("proc场景") {
    //        SECTION("简单场景1") {
    //            std::vector<std::string_view> result;
    //            str::split_list_view("HelloWorld,HelloWorld", [&result](std::string_view item) -> int {
    //                result.emplace_back(item);
    //                return 0;
    //            });
    //            ASSERT_EQ(result, (std::vector<std::string_view>{"HelloWorld", "HelloWorld"}));
    //        }
    //        SECTION("简单场景2") {
    //            std::vector<std::string_view> result;
    //            str::split_list_view("abcdbceebc", to_consumer(result));
    //
    //            ASSERT_EQ(result, (std::vector<std::string_view>{"a", "d", "ee", ""}));
    //        }
    //
    //        SECTION("刚好匹配一个") {
    //            std::vector<std::string_view> result;
    //            str::split_list_view(std::string(","), to_consumer(result));
    //            ASSERT_EQ(result, (std::vector<std::string_view>{"", ""}));
    //        }
    //
    //        SECTION("前面字段") {
    //            std::vector<std::string_view> result;
    //            str::split_list_view("AAAbc", std::string("bc"), [&result](std::string_view item) -> int {
    //                result.emplace_back(item);
    //                return 0;
    //            });
    //
    //            ASSERT_EQ(result, (std::vector<std::string_view>{"AAA", ""}));
    //        }
    //
    //        SECTION("后面字段") {
    //            std::vector<std::string_view> result;
    //            str::split_list_view("bcAAA", std::string("bc"), [&result](std::string_view item) -> int {
    //                result.emplace_back(item);
    //                return 0;
    //            });
    //
    //            ASSERT_EQ(result, (std::vector<std::string_view>{"", "AAA"}));
    //        }
    //
    //        SECTION("拆分空串") {
    //            std::vector<std::string_view> result;
    //            str::split_list_view("", std::string("bc"), [&result](std::string_view item) -> int {
    //                result.emplace_back(item);
    //                return 0;
    //            });
    //
    //            ASSERT_EQ(result, (std::vector<std::string_view>{""}));
    //        }
    //
    //        SECTION("连续匹配") {
    //            std::vector<std::string_view> result;
    //            str::split_list_view("AAbcbcBB", std::string("bc"), [&result](std::string_view item) -> int {
    //                result.emplace_back(item);
    //                return 0;
    //            });
    //
    //            ASSERT_EQ(result, (std::vector<std::string_view>{"AA", "", "BB"}));
    //        }
    //
    //        SECTION("连续匹配2") {
    //            std::vector<std::string_view> result;
    //            str::split_list_view("bcbc", std::string("bc"), [&result](std::string_view item) -> int {
    //                result.emplace_back(item);
    //                return 0;
    //            });
    //
    //            ASSERT_EQ(result, (std::vector<std::string_view>{"", "", ""}));
    //        }
    //
    //        SECTION("无法匹配") {
    //            std::vector<std::string_view> result;
    //            str::split_list_view("defgui", std::string("bc"), [&result](std::string_view item) -> int {
    //                result.emplace_back(item);
    //                return 0;
    //            });
    //
    //            ASSERT_EQ(result, (std::vector<std::string_view>{"defgui"}));
    //        }
    //
    //        SECTION("提前结束:一般场景") {
    //            std::vector<std::string_view> result;
    //            str::split_list_view("|123|456||def|ghi|", std::string("|"), [&result](std::string_view item) -> int {
    //                result.emplace_back(item);
    //                if (result.size() == 4) {
    //                    return -1;
    //                }
    //                return 0;
    //            });
    //
    //            ASSERT_EQ(result, (std::vector<std::string_view>{"", "123", "456", ""}));
    //        }
    //    }
    //
    //    GROUP("regex场景") {
    //        SECTION("一般场景") {
    //            std::regex sep("[ \t]*(:|,|;)[ \t]*");
    //            ASSERT_EQ(str::split_list_view("aaa, bbb:ccc ;ddd ", sep), (std::vector<std::string>{"aaa", "bbb", "ccc", "ddd "}));
    //            ASSERT_EQ(str::split_list_view("aaa, bbb:ccc ;ddd ", sep, 0), (std::vector<std::string>{"aaa, bbb:ccc ;ddd "}));
    //            ASSERT_EQ(str::split_list_view("aaa, bbb:ccc ;ddd ", sep, 1), (std::vector<std::string>{"aaa", "bbb:ccc ;ddd "}));
    //            ASSERT_EQ(str::split_list_view("aaa, bbb:ccc ;ddd ", sep, 2), (std::vector<std::string>{"aaa", "bbb", "ccc ;ddd "}));
    //            ASSERT_EQ(str::split_list_view("aaa, bbb:ccc ;ddd ", sep, 3), (std::vector<std::string>{"aaa", "bbb", "ccc", "ddd "}));
    //            ASSERT_EQ(str::split_list_view("aaa, bbb:ccc ;ddd ", sep, 4), (std::vector<std::string>{"aaa", "bbb", "ccc", "ddd "}));
    //            ASSERT_EQ(str::split_list_view("aaa, bbb:ccc ;ddd ", sep, str::npos), (std::vector<std::string>{"aaa", "bbb", "ccc", "ddd "}));
    //        }
    //        SECTION("空串") {
    //            std::regex sep(":|,|;");
    //            ASSERT_EQ(str::split_list_view("", sep), (std::vector<std::string>{""}));
    //            ASSERT_EQ(str::split_list_view("", sep, 0), (std::vector<std::string>{""}));
    //            ASSERT_EQ(str::split_list_view("", sep, 3), (std::vector<std::string>{""}));
    //        }
    //        SECTION("proc:不限拆分次数") {
    //            std::regex sep("[ \t]*(:|,|;)[ \t]*");
    //            std::vector<std::string_view> result;
    //            str::split_list_view("aaa, bbb:ccc ;ddd ", sep, [&result](std::string_view item) -> int {
    //                result.emplace_back(item);
    //                return 0;
    //            });
    //            ASSERT_EQ(result, (std::vector<std::string_view>{"aaa", "bbb", "ccc", "ddd "}));
    //        }
    //        SECTION("proc:限制拆分次数+提前返回") {
    //            std::regex sep("[ \t]*(:|,|;)[ \t]*");
    //            std::vector<std::string_view> result;
    //            str::split_list_view("aaa, bbb:ccc ;ddd ", sep, 2, [&result](std::string_view item) -> int {
    //                result.emplace_back(item);
    //                if (result.size() == 2) { // max_n 限制不起作用,这里的约束条件会提前触发,导致提前结束,所以只存两个,尾部的数据直接丢弃
    //                    return -1;
    //                }
    //                return 0;
    //            });
    //            ASSERT_EQ(result, (std::vector<std::string_view>{"aaa", "bbb"}));
    //        }
    //        SECTION("proc:限制拆分次数+提前返回+限制次数较小") {
    //            std::regex sep("[ \t]*(:|,|;)[ \t]*");
    //            std::vector<std::string_view> result;
    //            str::split_list_view("aaa, bbb:ccc ;ddd,MNP", sep, 2, [&result](std::string_view item) -> int {
    //                result.emplace_back(item);
    //                if (result.size() == 4) { // max_n 提前返回,限制起作用，实际能存三个
    //                    return -1;
    //                }
    //                return 0;
    //            });
    //            ASSERT_EQ(result, (std::vector<std::string_view>{"aaa", "bbb", "ccc ;ddd,MNP"}));
    //        }
    //        SECTION("全都是分隔符") {
    //            std::regex sep(":|,|;");
    //            ASSERT_EQ(str::split_list_view("::,,;;", sep), (std::vector<std::string>{"", "", "", "", "", "", ""}));
    //        }
    //        SECTION("分隔符刚好在尾部") {
    //            std::regex sep(":|,|;");
    //            ASSERT_EQ(str::split_list_view("aaa:bbb,ccc;", sep), (std::vector<std::string>{"aaa", "bbb", "ccc", ""}));
    //        }
    //        SECTION("分隔符刚好在头部") {
    //            std::regex sep(":|,|;");
    //            ASSERT_EQ(str::split_list_view(":aaa:bbb,ccc", sep), (std::vector<std::string>{"", "aaa", "bbb", "ccc"}));
    //        }
    //    }
}