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

TEST(test_str, split_view) {
    GROUP("按空白拆分(缺省场景") {
        SECTION("按空白拆分:返回容器:无次数限制") {
            // space 分隔符
            ASSERT_EQ(str::split_view("a b c"), (std::vector<std::string>{"a", "b", "c"}));

            // tab 分隔符
            ASSERT_EQ(str::split_view("a\tb\tc"), (std::vector<std::string>{"a", "b", "c"}));

            // 回车分隔符
            ASSERT_EQ(str::split_view("a\rb\rc"), (std::vector<std::string>{"a", "b", "c"}));

            // 换行分隔符
            ASSERT_EQ(str::split_view("a\nb\nc"), (std::vector<std::string>{"a", "b", "c"}));

            // 垂直制表做分隔符
            ASSERT_EQ(str::split_view("a\vb\vc"), (std::vector<std::string>{"a", "b", "c"}));

            // 混合多分隔符1
            ASSERT_EQ(str::split_view("a\r\n\t\vb\r\n\t\vc"), (std::vector<std::string>{"a", "b", "c"}));

            // 混合多分隔符2
            ASSERT_EQ(str::split_view("\r\t\n\v a\r\t\n\v b\r\t\n\v c\r\t\n\v "), (std::vector<std::string>{"", "a", "b", "c", ""}));

            // 全分隔符
            ASSERT_EQ(str::split_view(" \t\r\n\v"), (std::vector<std::string>{"", ""}));

            // 无分隔符
            ASSERT_EQ(str::split_view("abc"), (std::vector<std::string>{"abc"}));

            // 分隔符在前
            ASSERT_EQ(str::split_view(" abc"), (std::vector<std::string>{"", "abc"}));
            ASSERT_EQ(str::split_view("\tabc"), (std::vector<std::string>{"", "abc"}));
            ASSERT_EQ(str::split_view("\rabc"), (std::vector<std::string>{"", "abc"}));
            ASSERT_EQ(str::split_view("\nabc"), (std::vector<std::string>{"", "abc"}));
            ASSERT_EQ(str::split_view("\vabc"), (std::vector<std::string>{"", "abc"}));

            // 分隔符在后
            ASSERT_EQ(str::split_view("abc "), (std::vector<std::string>{"abc", ""}));
            ASSERT_EQ(str::split_view("abc\t"), (std::vector<std::string>{"abc", ""}));
            ASSERT_EQ(str::split_view("abc\r"), (std::vector<std::string>{"abc", ""}));
            ASSERT_EQ(str::split_view("abc\n"), (std::vector<std::string>{"abc", ""}));
            ASSERT_EQ(str::split_view("abc\v"), (std::vector<std::string>{"abc", ""}));

            // 空串
            ASSERT_EQ(str::split_view(""), (std::vector<std::string>{""}));
        }
        SECTION("按空白拆分:返回容器:限制次数") {
            // 一般串限制次数
            ASSERT_EQ(str::split_view(" a\rb\tc\vd\n", 0), (std::vector<std::string>{" a\rb\tc\vd\n"}));
            ASSERT_EQ(str::split_view(" a\rb\tc\vd\n", 1), (std::vector<std::string>{"", "a\rb\tc\vd\n"}));
            ASSERT_EQ(str::split_view(" a\rb\tc\vd\n", 2), (std::vector<std::string>{"", "a", "b\tc\vd\n"}));
            ASSERT_EQ(str::split_view(" a\rb\tc\vd\n", 5), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            ASSERT_EQ(str::split_view(" a\rb\tc\vd\n", 6), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            ASSERT_EQ(str::split_view(" a\rb\tc\vd\n", str::npos), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));

            // 空串限制次数
            ASSERT_EQ(str::split_view("", 0), (std::vector<std::string>{""}));
            ASSERT_EQ(str::split_view("", 1), (std::vector<std::string>{""}));
            ASSERT_EQ(str::split_view("", str::npos), (std::vector<std::string>{""}));
        }
    }

    GROUP("按指定字符串拆分") {
        SECTION("按指定字符串拆分:返回容器:无次数限制") {
            ASSERT_EQ(str::split_view("a||b||c|| ", "||"), (std::vector<std::string>{"a", "b", "c", " "}));
            ASSERT_EQ(str::split_view("abbbbbcbb", "bb"), (std::vector<std::string>{"a", "", "bc", ""}));
            ASSERT_EQ(str::split_view("bbbbbb", "bb"), (std::vector<std::string>{"", "", "", ""}));
            ASSERT_EQ(str::split_view("bbbbbb", "#"), (std::vector<std::string>{"bbbbbb"}));
            ASSERT_EQ(str::split_view("::", "::"), (std::vector<std::string>{"", ""}));
            ASSERT_EQ(str::split_view("", "::"), (std::vector<std::string>{""}));
            ASSERT_EQ(str::split_view("", ""), (std::vector<std::string>{""}));
            ASSERT_EQ(str::split_view("K", ""), (std::vector<std::string>{"K"}));
            ASSERT_EQ(str::split_view("abcd", ""), (std::vector<std::string>{"abcd"}));
        }
        SECTION("按指定字符串拆分:返回容器:限制次数") {
            ASSERT_EQ(str::split_view(",a,b,c,d,", ",", 0), (std::vector<std::string>{",a,b,c,d,"}));
            ASSERT_EQ(str::split_view(",a,b,c,d,", ",", 1), (std::vector<std::string>{"", "a,b,c,d,"}));
            ASSERT_EQ(str::split_view(",a,b,c,d,", ",", 2), (std::vector<std::string>{"", "a", "b,c,d,"}));
            ASSERT_EQ(str::split_view(",a,b,c,d,", ",", 5), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            ASSERT_EQ(str::split_view(",a,b,c,d,", ",", 6), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            ASSERT_EQ(str::split_view(",a,b,c,d,", ",", str::npos), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
        }
    }

    GROUP("按指定字符集拆分") {
        SECTION("按指定字符集拆分:返回容器:无次数限制") {
            ASSERT_EQ(str::split_view(",a;b:c,d,", str::charset(",;:")), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            ASSERT_EQ(str::split_view(",a;b:c,d,", str::charset(";:")), (std::vector<std::string>{",a", "b", "c,d,"}));
            ASSERT_EQ(str::split_view("abcd", str::charset(";:")), (std::vector<std::string>{"abcd"}));
            ASSERT_EQ(str::split_view("abbKbbd", str::charset("b")), (std::vector<std::string>{"a", "", "K", "", "d"}));
            ASSERT_EQ(str::split_view("", str::charset("b")), (std::vector<std::string>{""}));
            ASSERT_EQ(str::split_view("abcd", str::charset("")), (std::vector<std::string>{"abcd"}));
        }
        SECTION("按指定字符集拆分:返回容器:限制次数") {
            ASSERT_EQ(str::split_view(",a;b:c,d,", str::charset(",;:"), 0), (std::vector<std::string>{",a;b:c,d,"}));
            ASSERT_EQ(str::split_view(",a;b:c,d,", str::charset(",;:"), 1), (std::vector<std::string>{"", "a;b:c,d,"}));
            ASSERT_EQ(str::split_view(",a;b:c,d,", str::charset(",;:"), 3), (std::vector<std::string>{"", "a", "b", "c,d,"}));
            ASSERT_EQ(str::split_view(",a;b:c,d,", str::charset(",;:"), 5), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            ASSERT_EQ(str::split_view(",a;b:c,d,", str::charset(",;:"), 6), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
            ASSERT_EQ(str::split_view(",a;b:c,d,", str::charset(",;:"), str::npos), (std::vector<std::string>{"", "a", "b", "c", "d", ""}));
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
}