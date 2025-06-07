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

TEST(test_str, trim_surrounding_view) {
    SECTION("左右都没有空白") {
        ASSERT_EQ(str::trim_surrounding_view("3bc1233"), "3bc1233");
    }

    SECTION("左边有空白") {
        ASSERT_EQ(str::trim_surrounding_view("\r  \t\nc1233"), "c1233");
    }

    SECTION("右边有空白") {
        ASSERT_EQ(str::trim_surrounding_view("c1233\r  \t\n"), "c1233");
    }

    SECTION("左右两边都有空白") {
        ASSERT_EQ(str::trim_surrounding_view("\r  \t\nc1233\r  \t\n"), "c1233");
    }

    SECTION("中间有空白") {
        ASSERT_EQ(str::trim_surrounding_view("\r  \t\nc12\r  \t\n33\r  \t\n"), "c12\r  \t\n33");
    }

    SECTION("空串") {
        ASSERT_EQ(str::trim_surrounding_view(""), "");
    }

    SECTION("全空白串") {
        ASSERT_EQ(str::trim_surrounding_view("\r\n \t"), "");
    }

    SECTION("指定字符") {
        ASSERT_TRUE(str::trim_surrounding_view("33c1233", [](std::string::value_type ch) -> bool {
            return ch == '3';
        }) == "c12");
    }

    SECTION("指定字符集") {
        ASSERT_EQ(str::trim_surrounding_view("1233ABC000", str::charset(str::all_digits)), "ABC");
        ASSERT_EQ(str::trim_surrounding_view("1233ABC000", str::all_digits), "ABC");
        ASSERT_EQ(str::trim_surrounding_view("1233ABC000", ""), "1233ABC000");
        ASSERT_EQ(str::trim_surrounding_view("", ""), "");
    }
}
