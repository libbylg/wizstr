/**
 * Copyright (c) 2021-2025  libbylg@126.com
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

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, split) {
    GROUP("按空白拆分(缺省场景") {
        SECTION("按空白拆分:返回容器:无次数限制") {
            // space 分隔符
            ASSERT_EQ(str::split("a b c"), (std::vector<std::string>{"a", "b", "c"}));

            // tab 分隔符
            ASSERT_EQ(str::split("a\tb\tc"), (std::vector<std::string>{"a", "b", "c"}));

            // 回车分隔符
            ASSERT_EQ(str::split("a\rb\rc"), (std::vector<std::string>{"a", "b", "c"}));

            // 换行分隔符
            ASSERT_EQ(str::split("a\nb\nc"), (std::vector<std::string>{"a", "b", "c"}));

            // 垂直制表做分隔符
            ASSERT_EQ(str::split("a\vb\vc"), (std::vector<std::string>{"a", "b", "c"}));

            // 混合多分隔符1
            ASSERT_EQ(str::split("a\r\n\t\vb\r\n\t\vc"), (std::vector<std::string>{"a", "b", "c"}));

            // 混合多分隔符2
            ASSERT_EQ(str::split("\r\t\n\v a\r\t\n\v b\r\t\n\v c\r\t\n\v "), (std::vector<std::string>{"", "a", "b", "c", ""}));

            // 全分隔符
            ASSERT_EQ(str::split(" \t\r\n\v"), (std::vector<std::string>{"", ""}));

            // 无分隔符
            ASSERT_EQ(str::split("abc"), (std::vector<std::string>{"abc"}));

            // 分隔符在前
            ASSERT_EQ(str::split(" abc"), (std::vector<std::string>{"", "abc"}));
            ASSERT_EQ(str::split("\tabc"), (std::vector<std::string>{"", "abc"}));
            ASSERT_EQ(str::split("\rabc"), (std::vector<std::string>{"", "abc"}));
            ASSERT_EQ(str::split("\nabc"), (std::vector<std::string>{"", "abc"}));
            ASSERT_EQ(str::split("\vabc"), (std::vector<std::string>{"", "abc"}));

            // 分隔符在后
            ASSERT_EQ(str::split("abc "), (std::vector<std::string>{"abc", ""}));
            ASSERT_EQ(str::split("abc\t"), (std::vector<std::string>{"abc", ""}));
            ASSERT_EQ(str::split("abc\r"), (std::vector<std::string>{"abc", ""}));
            ASSERT_EQ(str::split("abc\n"), (std::vector<std::string>{"abc", ""}));
            ASSERT_EQ(str::split("abc\v"), (std::vector<std::string>{"abc", ""}));

            // 空串
            ASSERT_EQ(str::split(""), (std::vector<std::string>{""}));
        }
        SECTION("按空白拆分:返回容器:限制次数") {
            // 一般串限制次数
            ASSERT_EQ(str::split(" a\rb\tc\vd\n", 0), (std::vector<std::string>{" a\rb\tc\vd\n"}));
            ASSERT_EQ(str::split(" a\rb\tc\vd\n", 1), (std::vector<std::string>{"", "a\rb\tc\vd\n"}));
            ASSERT_EQ(str::split(" a\rb\tc\vd\n", 2), (std::vector<std::string>{"", "a", "b\tc\vd\n"}));
            ASSERT_EQ(str::split(" a\rb\tc\vd\n", 5), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            ASSERT_EQ(str::split(" a\rb\tc\vd\n", 6), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            ASSERT_EQ(str::split(" a\rb\tc\vd\n", str::npos), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));

            // 空串限制次数
            ASSERT_EQ(str::split("", 0), (std::vector<std::string>{""}));
            ASSERT_EQ(str::split("", 1), (std::vector<std::string>{""}));
            ASSERT_EQ(str::split("", str::npos), (std::vector<std::string>{""}));
        }
        SECTION("按空白拆分:用proc接收:无次数限制") {
            std::vector<std::string_view> result;

            // space 分隔符
            str::split("a b c", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"a", "b", "c"}));

            // tab 分隔符
            str::split("a\tb\tc", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"a", "b", "c"}));

            // 回车分隔符
            str::split("a\rb\rc", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"a", "b", "c"}));

            // 换行分隔符
            str::split("a\nb\nc", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"a", "b", "c"}));

            // 垂直制表做分隔符
            str::split("a\vb\vc", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"a", "b", "c"}));

            // 混合多分隔符1
            str::split("a\r\n\t\vb\r\n\t\vc", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"a", "b", "c"}));

            // 混合多分隔符2
            str::split("\r\t\n\v a\r\t\n\v b\r\t\n\v c\r\t\n\v ", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b", "c", ""}));

            // 全分隔符
            str::split(" \t\r\n\v", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"", ""}));

            // 无分隔符
            str::split("abc", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"abc"}));

            // 分隔符在前
            str::split(" abc", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"", "abc"}));
            str::split("\tabc", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"", "abc"}));
            str::split("\rabc", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"", "abc"}));
            str::split("\nabc", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"", "abc"}));
            str::split("\vabc", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"", "abc"}));

            // 分隔符在后
            str::split("abc ", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"abc", ""}));
            str::split("abc\t", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"abc", ""}));
            str::split("abc\r", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"abc", ""}));
            str::split("abc\n", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"abc", ""}));
            str::split("abc\v", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"abc", ""}));

            // 空串
            str::split("", to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{""}));
        }
        SECTION("用proc接收:限制次数") {
            std::vector<std::string_view> result;

            // 一般串限制次数
            str::split(" a\rb\tc\vd\n", 0, to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{" a\rb\tc\vd\n"}));
            str::split(" a\rb\tc\vd\n", 1, to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"", "a\rb\tc\vd\n"}));
            str::split(" a\rb\tc\vd\n", 2, to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b\tc\vd\n"}));
            str::split(" a\rb\tc\vd\n", 5, to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            str::split(" a\rb\tc\vd\n", 6, to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            str::split(" a\rb\tc\vd\n", str::npos, to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            str::split(" a\rb\tc\vd\n", 4, to_consumer{result = {}, 3});
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b"}));

            // 空串限制次数
            str::split("", 0, to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{""}));
            str::split("", 1, to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{""}));
            str::split("", str::npos, to_consumer{result = {}});
            ASSERT_EQ(result, (std::vector<std::string>{""}));
        }
    }
    GROUP("按指定字符串拆分") {
        SECTION("按指定字符串拆分:返回容器:无次数限制") {
            ASSERT_EQ(str::split("a||b||c|| ", "||"), (std::vector<std::string>{"a", "b", "c", " "}));
            ASSERT_EQ(str::split("abbbbbcbb", "bb"), (std::vector<std::string>{"a", "", "bc", ""}));
            ASSERT_EQ(str::split("bbbbbb", "bb"), (std::vector<std::string>{"", "", "", ""}));
            ASSERT_EQ(str::split("bbbbbb", "#"), (std::vector<std::string>{"bbbbbb"}));
            ASSERT_EQ(str::split("::", "::"), (std::vector<std::string>{"", ""}));
            ASSERT_EQ(str::split("", "::"), (std::vector<std::string>{""}));
            ASSERT_EQ(str::split("", ""), (std::vector<std::string>{""}));
            ASSERT_EQ(str::split("K", ""), (std::vector<std::string>{"K"}));
            ASSERT_EQ(str::split("abcd", ""), (std::vector<std::string>{"abcd"}));
        }
        SECTION("按指定字符串拆分:返回容器:限制次数") {
            ASSERT_EQ(str::split(",a,b,c,d,", ",", 0), (std::vector<std::string>{",a,b,c,d,"}));
            ASSERT_EQ(str::split(",a,b,c,d,", ",", 1), (std::vector<std::string>{"", "a,b,c,d,"}));
            ASSERT_EQ(str::split(",a,b,c,d,", ",", 2), (std::vector<std::string>{"", "a", "b,c,d,"}));
            ASSERT_EQ(str::split(",a,b,c,d,", ",", 5), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            ASSERT_EQ(str::split(",a,b,c,d,", ",", 6), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            ASSERT_EQ(str::split(",a,b,c,d,", ",", str::npos), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
        }
        SECTION("按指定字符串拆分:用proc接收:无次数限制") {
            std::vector<std::string_view> result;
            result.clear();
            str::split(",a,b,c,d,", ",", to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            result.clear();
            str::split(",a,b,c,d,", ",", to_consumer(result, 2));
            ASSERT_EQ(result, (std::vector<std::string>{"", "a"}));
        }
        SECTION("按指定字符串拆分:用proc接收:限制次数") {
            std::vector<std::string_view> result;

            result.clear();
            str::split(",a,b,c,d,", ",", 0, to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string>{",a,b,c,d,"}));

            result.clear();
            str::split(",a,b,c,d,", ",", 1, to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string>{"", "a,b,c,d,"}));

            result.clear();
            str::split(",a,b,c,d,", ",", 2, to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b,c,d,"}));

            result.clear();
            str::split(",a,b,c,d,", ",", 5, to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b", "c", "d", ""}));

            result.clear();
            str::split(",a,b,c,d,", ",", 6, to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b", "c", "d", ""}));

            result.clear();
            str::split(",a,b,c,d,", ",", str::npos, to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b", "c", "d", ""}));

            result.clear();
            str::split(",a,b,c,d,", ",", 3, to_consumer(result, 2));
            ASSERT_EQ(result, (std::vector<std::string>{"", "a"}));
        }
    }
    GROUP("按指定字符集拆分") {
        SECTION("按指定字符集拆分:返回容器:无次数限制") {
            ASSERT_EQ(str::split(",a;b:c,d,", str::charset(",;:")), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            ASSERT_EQ(str::split(",a;b:c,d,", str::charset(";:")), (std::vector<std::string>{",a", "b", "c,d,"}));
            ASSERT_EQ(str::split("abcd", str::charset(";:")), (std::vector<std::string>{"abcd"}));
            ASSERT_EQ(str::split("abbKbbd", str::charset("b")), (std::vector<std::string>{"a", "", "K", "", "d"}));
            ASSERT_EQ(str::split("", str::charset("b")), (std::vector<std::string>{""}));
            ASSERT_EQ(str::split("abcd", str::charset("")), (std::vector<std::string>{"abcd"}));
        }
        SECTION("按指定字符集拆分:返回容器:限制次数") {
            ASSERT_EQ(str::split(",a;b:c,d,", str::charset(",;:"), 0), (std::vector<std::string>{",a;b:c,d,"}));
            ASSERT_EQ(str::split(",a;b:c,d,", str::charset(",;:"), 1), (std::vector<std::string>{"", "a;b:c,d,"}));
            ASSERT_EQ(str::split(",a;b:c,d,", str::charset(",;:"), 3), (std::vector<std::string>{"", "a", "b", "c,d,"}));
            ASSERT_EQ(str::split(",a;b:c,d,", str::charset(",;:"), 5), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            ASSERT_EQ(str::split(",a;b:c,d,", str::charset(",;:"), 6), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            ASSERT_EQ(str::split(",a;b:c,d,", str::charset(",;:"), str::npos), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
        }
        SECTION("按指定字符集拆分:用proc接收:无次数限制") {
            std::vector<std::string_view> result;

            str::split(",a;b:c,d,", str::charset(",;:"), to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
        }
        SECTION("按指定字符集拆分:用proc接收:限制次数") {
            std::vector<std::string_view> result;

            result.clear();
            str::split(",a;b:c,d,", str::charset(",;:"), 0, to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string>{",a;b:c,d,"}));

            result.clear();
            str::split(",a;b:c,d,", str::charset(",;:"), 1, to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string>{"", "a;b:c,d,"}));

            result.clear();
            str::split(",a;b:c,d,", str::charset(",;:"), 3, to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b", "c,d,"}));

            result.clear();
            str::split(",a;b:c,d,", str::charset(",;:"), 5, to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b", "c", "d", ""}));

            result.clear();
            str::split(",a;b:c,d,", str::charset(",;:"), 6, to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b", "c", "d", ""}));

            result.clear();
            str::split(",a;b:c,d,", str::charset(",;:"), str::npos, to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
        }
    }
    GROUP("按指定正则表达式拆分") {
        SECTION("按指定正则表达式拆分:返回容器:无次数限制") {
        }
        SECTION("按指定正则表达式拆分:返回容器:限制次数") {
        }
        SECTION("按指定正则表达式拆分:用proc接收:无次数限制") {
        }
        SECTION("按指定正则表达式拆分:用proc接收:限制次数") {
        }
    }

    // GROUP("proc场景") {
    //     SECTION("简单场景1") {
    //         std::vector<std::string_view> result;
    //         str::split("HelloWorld,HelloWorld", to_consumer{result});
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"HelloWorld", "HelloWorld"}));
    //     }
    //     SECTION("简单场景2") {
    //         std::vector<std::string_view> result;
    //         str::split("a,d,ee,", to_consumer(result));
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"a", "d", "ee", ""}));
    //     }
    //
    //     SECTION("刚好匹配一个") {
    //         std::vector<std::string_view> result;
    //         str::split(std::string(","), to_consumer(result));
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"", ""}));
    //     }
    //
    //     SECTION("前面字段") {
    //         std::vector<std::string_view> result;
    //         str::split("AAA,", to_consumer(result));
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"AAA", ""}));
    //     }
    //
    //     SECTION("后面字段") {
    //         std::vector<std::string_view> result;
    //         str::split(",AAA", to_consumer(result));
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"", "AAA"}));
    //     }
    //
    //     SECTION("拆分空串") {
    //         std::vector<std::string_view> result;
    //         str::split("", to_consumer(result));
    //         ASSERT_EQ(result, (std::vector<std::string_view>{""}));
    //     }
    //
    //     SECTION("连续匹配") {
    //         std::vector<std::string_view> result;
    //         str::split("AA,,BB", to_consumer(result));
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"AA", "", "BB"}));
    //     }
    //
    //     SECTION("连续匹配2") {
    //         std::vector<std::string_view> result;
    //         str::split(",,", to_consumer(result));
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"", "", ""}));
    //     }
    //
    //     SECTION("无法匹配") {
    //         std::vector<std::string_view> result;
    //         str::split("defgui", to_consumer(result));
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"defgui"}));
    //     }
    //
    //     SECTION("提前结束:一般场景") {
    //         std::vector<std::string_view> result;
    //         str::split(",123,456,,def,ghi,", [&result](std::string_view item) -> int {
    //             result.emplace_back(item);
    //             if (result.size() == 4) {
    //                 return -1;
    //             }
    //             return 0;
    //         });
    //
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"", "123", "456", ""}));
    //     }
    //
    //     SECTION("提前结束:但总数量并不够") {
    //         std::vector<std::string_view> result;
    //         str::split(",123,456,defghi", [&result](std::string_view item) -> int {
    //             result.emplace_back(item);
    //             if (result.size() == 4) {
    //                 return -1;
    //             }
    //             return 0;
    //         });
    //
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"", "123", "456", "defghi"}));
    //     }
    // }

    // GROUP("regex场景") {
    //     SECTION("一般场景") {
    //         std::regex sep("[ \t]*(:|,|;)[ \t]*");
    //         ASSERT_EQ(str::split("aaa, bbb:ccc ;ddd ", sep), (std::vector<std::string>{"aaa", "bbb", "ccc", "ddd "}));
    //         ASSERT_EQ(str::split("aaa, bbb:ccc ;ddd ", sep, 0), (std::vector<std::string>{"aaa, bbb:ccc ;ddd "}));
    //         ASSERT_EQ(str::split("aaa, bbb:ccc ;ddd ", sep, 1), (std::vector<std::string>{"aaa", "bbb:ccc ;ddd "}));
    //         ASSERT_EQ(str::split("aaa, bbb:ccc ;ddd ", sep, 2), (std::vector<std::string>{"aaa", "bbb", "ccc ;ddd "}));
    //         ASSERT_EQ(str::split("aaa, bbb:ccc ;ddd ", sep, 3), (std::vector<std::string>{"aaa", "bbb", "ccc", "ddd "}));
    //         ASSERT_EQ(str::split("aaa, bbb:ccc ;ddd ", sep, 4), (std::vector<std::string>{"aaa", "bbb", "ccc", "ddd "}));
    //         ASSERT_EQ(str::split("aaa, bbb:ccc ;ddd ", sep, str::npos), (std::vector<std::string>{"aaa", "bbb", "ccc", "ddd "}));
    //     }
    //     SECTION("空串") {
    //         std::regex sep(":|,|;");
    //         ASSERT_EQ(str::split("", sep), (std::vector<std::string>{""}));
    //         ASSERT_EQ(str::split("", sep, 0), (std::vector<std::string>{""}));
    //         ASSERT_EQ(str::split("", sep, 3), (std::vector<std::string>{""}));
    //     }
    //     SECTION("proc:不限拆分次数") {
    //         std::regex sep("[ \t]*(:|,|;)[ \t]*");
    //         std::vector<std::string_view> result;
    //         str::split("aaa, bbb:ccc ;ddd ", sep, [&result](std::string_view item) -> int {
    //             result.emplace_back(item);
    //             return 0;
    //         });
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"aaa", "bbb", "ccc", "ddd "}));
    //     }
    //     SECTION("proc:限制拆分次数+提前返回") {
    //         std::regex sep("[ \t]*(:|,|;)[ \t]*");
    //         std::vector<std::string_view> result;
    //         str::split("aaa, bbb:ccc ;ddd ", sep, 2, [&result](std::string_view item) -> int {
    //             result.emplace_back(item);
    //             if (result.size() == 2) { // max_n 限制不起作用,这里的约束条件会提前触发,导致提前结束,所以只存两个,尾部的数据直接丢弃
    //                 return -1;
    //             }
    //             return 0;
    //         });
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"aaa", "bbb"}));
    //     }
    //     SECTION("proc:限制拆分次数+提前返回+限制次数较小") {
    //         std::regex sep("[ \t]*(:|,|;)[ \t]*");
    //         std::vector<std::string_view> result;
    //         str::split("aaa, bbb:ccc ;ddd,MNP", sep, 2, [&result](std::string_view item) -> int {
    //             result.emplace_back(item);
    //             if (result.size() == 4) { // max_n 提前返回,限制起作用，实际能存三个
    //                 return -1;
    //             }
    //             return 0;
    //         });
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"aaa", "bbb", "ccc ;ddd,MNP"}));
    //     }
    //     SECTION("全都是分隔符") {
    //         std::regex sep(":|,|;");
    //         ASSERT_EQ(str::split("::,,;;", sep), (std::vector<std::string>{"", "", "", "", "", "", ""}));
    //     }
    //     SECTION("分隔符刚好在尾部") {
    //         std::regex sep(":|,|;");
    //         ASSERT_EQ(str::split("aaa:bbb,ccc;", sep), (std::vector<std::string>{"aaa", "bbb", "ccc", ""}));
    //     }
    //     SECTION("分隔符刚好在头部") {
    //         std::regex sep(":|,|;");
    //         ASSERT_EQ(str::split(":aaa:bbb,ccc", sep), (std::vector<std::string>{"", "aaa", "bbb", "ccc"}));
    //     }
    // }
}