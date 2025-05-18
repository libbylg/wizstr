/**
 * Copyright (c) 2021-2024 libbylg@126.com
 * tiny is licensed under Mulan PSL v2.
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

TEST(test_str, take_view) {
    SECTION("pos+n模式") {
        // 左边界
        ASSERT_EQ(str::take_view("abc1234", 0, str::npos), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", 0, 999), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", 0, 7), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", 0, 3), "abc");
        ASSERT_EQ(str::take_view("abc1234", 0, 1), "a");
        ASSERT_EQ(str::take_view("abc1234", 0, 0), "");

        // 中间位置
        ASSERT_EQ(str::take_view("abc1234", 3, str::npos), "1234");
        ASSERT_EQ(str::take_view("abc1234", 3, 999), "1234");
        ASSERT_EQ(str::take_view("abc1234", 3, 5), "1234");
        ASSERT_EQ(str::take_view("abc1234", 3, 4), "1234");
        ASSERT_EQ(str::take_view("abc1234", 3, 2), "12");
        ASSERT_EQ(str::take_view("abc1234", 3, 1), "1");
        ASSERT_EQ(str::take_view("abc1234", 3, 0), "");

        // 右边界
        ASSERT_EQ(str::take_view("abc1234", 7, str::npos), "");
        ASSERT_EQ(str::take_view("abc1234", 7, 999), "");
        ASSERT_EQ(str::take_view("abc1234", 7, 1), "");
        ASSERT_EQ(str::take_view("abc1234", 7, 0), "");

        // 超过右边界
        ASSERT_EQ(str::take_view("abc1234", 8, str::npos), "");
        ASSERT_EQ(str::take_view("abc1234", 8, 999), "");
        ASSERT_EQ(str::take_view("abc1234", 8, 1), "");
        ASSERT_EQ(str::take_view("abc1234", 8, 0), "");

        // 空串
        ASSERT_EQ(str::take_view("", 0, 0), "");
        ASSERT_EQ(str::take_view("", 0, 1), "");
        ASSERT_EQ(str::take_view("", 0, 999), "");
        ASSERT_EQ(str::take_view("", 0, str::npos), "");
        ASSERT_EQ(str::take_view("", 1, 0), "");
        ASSERT_EQ(str::take_view("", 1, 1), "");
        ASSERT_EQ(str::take_view("", 1, 999), "");
        ASSERT_EQ(str::take_view("", 1, str::npos), "");
        ASSERT_EQ(str::take_view("", str::npos, 0), "");
        ASSERT_EQ(str::take_view("", str::npos, 1), "");
        ASSERT_EQ(str::take_view("", str::npos, 999), "");
        ASSERT_EQ(str::take_view("", str::npos, str::npos), "");
    }
    SECTION("pos模式") {
        // 一般情况
        ASSERT_EQ(str::take_view("3bc1233", 0), "3bc1233");
        ASSERT_EQ(str::take_view("3bc1233", 3), "1233");
        ASSERT_EQ(str::take_view("3bc1239", 6), "9");
        ASSERT_EQ(str::take_view("3bc1234", 7), "");
        ASSERT_EQ(str::take_view("3bc1234", 8), "");
        ASSERT_EQ(str::take_view("3bc1234", str::npos), "");

        // 空串
        ASSERT_EQ(str::take_view("", 0), "");
        ASSERT_EQ(str::take_view("", 1), "");
        ASSERT_EQ(str::take_view("", str::npos), "");
    }
    SECTION("range模式") {
        // 左边界
        ASSERT_EQ(str::take_view("abc1234", str::range(0, str::npos)), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", str::range(0, 999)), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", str::range(0, 7)), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", str::range(0, 3)), "abc");
        ASSERT_EQ(str::take_view("abc1234", str::range(0, 1)), "a");
        ASSERT_EQ(str::take_view("abc1234", str::range(0, 0)), "");

        // 中间位置
        ASSERT_EQ(str::take_view("abc1234", str::range(3, str::npos)), "1234");
        ASSERT_EQ(str::take_view("abc1234", str::range(3, 999)), "1234");
        ASSERT_EQ(str::take_view("abc1234", str::range(3, 5)), "1234");
        ASSERT_EQ(str::take_view("abc1234", str::range(3, 4)), "1234");
        ASSERT_EQ(str::take_view("abc1234", str::range(3, 2)), "12");
        ASSERT_EQ(str::take_view("abc1234", str::range(3, 1)), "1");
        ASSERT_EQ(str::take_view("abc1234", str::range(3, 0)), "");

        // 右边界
        ASSERT_EQ(str::take_view("abc1234", str::range(7, str::npos)), "");
        ASSERT_EQ(str::take_view("abc1234", str::range(7, 999)), "");
        ASSERT_EQ(str::take_view("abc1234", str::range(7, 1)), "");
        ASSERT_EQ(str::take_view("abc1234", str::range(7, 0)), "");

        // 超过右边界
        ASSERT_EQ(str::take_view("abc1234", str::range(8, str::npos)), "");
        ASSERT_EQ(str::take_view("abc1234", str::range(8, 999)), "");
        ASSERT_EQ(str::take_view("abc1234", str::range(8, 1)), "");
        ASSERT_EQ(str::take_view("abc1234", str::range(8, 0)), "");

        // 空串
        ASSERT_EQ(str::take_view("", str::range(0, 0)), "");
        ASSERT_EQ(str::take_view("", str::range(0, 1)), "");
        ASSERT_EQ(str::take_view("", str::range(0, 999)), "");
        ASSERT_EQ(str::take_view("", str::range(0, str::npos)), "");
        ASSERT_EQ(str::take_view("", str::range(1, 0)), "");
        ASSERT_EQ(str::take_view("", str::range(1, 1)), "");
        ASSERT_EQ(str::take_view("", str::range(1, 999)), "");
        ASSERT_EQ(str::take_view("", str::range(1, str::npos)), "");
        ASSERT_EQ(str::take_view("", str::range(str::npos, 0)), "");
        ASSERT_EQ(str::take_view("", str::range(str::npos, 1)), "");
        ASSERT_EQ(str::take_view("", str::range(str::npos, 999)), "");
        ASSERT_EQ(str::take_view("", str::range(str::npos, str::npos)), "");
    }
    SECTION("interval模式") {
        // 左边界
        ASSERT_EQ(str::take_view("abc1234", str::interval(0, 0)), "");
        ASSERT_EQ(str::take_view("abc1234", str::interval(0, 1)), "a");
        ASSERT_EQ(str::take_view("abc1234", str::interval(0, 3)), "abc");
        ASSERT_EQ(str::take_view("abc1234", str::interval(0, 6)), "abc123");
        ASSERT_EQ(str::take_view("abc1234", str::interval(0, 7)), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", str::interval(0, 999)), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", str::interval(0, str::npos)), "abc1234");

        // 中间位置
        ASSERT_EQ(str::take_view("abc1234", str::interval(3, 0)), "abc");
        ASSERT_EQ(str::take_view("abc1234", str::interval(3, 1)), "bc");
        ASSERT_EQ(str::take_view("abc1234", str::interval(3, 3)), "");
        ASSERT_EQ(str::take_view("abc1234", str::interval(3, 6)), "123");
        ASSERT_EQ(str::take_view("abc1234", str::interval(3, 7)), "1234");
        ASSERT_EQ(str::take_view("abc1234", str::interval(3, 999)), "1234");
        ASSERT_EQ(str::take_view("abc1234", str::interval(3, str::npos)), "1234");

        // 右边界
        ASSERT_EQ(str::take_view("abc1234", str::interval(7, 0)), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", str::interval(7, 1)), "bc1234");
        ASSERT_EQ(str::take_view("abc1234", str::interval(7, 3)), "1234");
        ASSERT_EQ(str::take_view("abc1234", str::interval(7, 6)), "4");
        ASSERT_EQ(str::take_view("abc1234", str::interval(7, 7)), "");
        ASSERT_EQ(str::take_view("abc1234", str::interval(7, 999)), "");
        ASSERT_EQ(str::take_view("abc1234", str::interval(7, str::npos)), "");

        // 超过右边界
        ASSERT_EQ(str::take_view("abc1234", str::interval(8, 0)), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", str::interval(8, 1)), "bc1234");
        ASSERT_EQ(str::take_view("abc1234", str::interval(8, 3)), "1234");
        ASSERT_EQ(str::take_view("abc1234", str::interval(8, 6)), "4");
        ASSERT_EQ(str::take_view("abc1234", str::interval(8, 7)), "");
        ASSERT_EQ(str::take_view("abc1234", str::interval(8, 999)), "");
        ASSERT_EQ(str::take_view("abc1234", str::interval(8, str::npos)), "");

        // 空串
        ASSERT_EQ(str::take_view("", str::interval(0, 0)), "");
        ASSERT_EQ(str::take_view("", str::interval(0, 1)), "");
        ASSERT_EQ(str::take_view("", str::interval(0, 999)), "");
        ASSERT_EQ(str::take_view("", str::interval(0, str::npos)), "");
        ASSERT_EQ(str::take_view("", str::interval(1, 0)), "");
        ASSERT_EQ(str::take_view("", str::interval(1, 1)), "");
        ASSERT_EQ(str::take_view("", str::interval(1, 999)), "");
        ASSERT_EQ(str::take_view("", str::interval(1, str::npos)), "");
        ASSERT_EQ(str::take_view("", str::interval(str::npos, 0)), "");
        ASSERT_EQ(str::take_view("", str::interval(str::npos, 1)), "");
        ASSERT_EQ(str::take_view("", str::interval(str::npos, 999)), "");
        ASSERT_EQ(str::take_view("", str::interval(str::npos, str::npos)), "");
    }
    SECTION("shifter模式") {
        // 左边界
        ASSERT_EQ(str::take_view("abc1234", str::shifter(0, 999)), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(0, 7)), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(0, 3)), "abc");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(0, 1)), "a");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(0, 0)), "");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(0, -1)), "");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(0, -999)), "");

        // 中间位置
        ASSERT_EQ(str::take_view("abc1234", str::shifter(3, 999)), "1234");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(3, 5)), "1234");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(3, 4)), "1234");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(3, 2)), "12");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(3, 1)), "1");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(3, 0)), "");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(3, -2)), "bc");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(3, -3)), "abc");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(3, -999)), "abc");

        // 右边界
        ASSERT_EQ(str::take_view("abc1234", str::shifter(7, 999)), "");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(7, 1)), "");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(7, 0)), "");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(7, -1)), "4");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(7, -3)), "234");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(7, -7)), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(7, -8)), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(7, -999)), "abc1234");

        // 超过右边界
        ASSERT_EQ(str::take_view("abc1234", str::shifter(8, 999)), "");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(8, 1)), "");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(8, 0)), "");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(8, -1)), "4");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(8, -3)), "234");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(8, -7)), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(8, -8)), "abc1234");
        ASSERT_EQ(str::take_view("abc1234", str::shifter(8, -999)), "abc1234");

        // 空串
        ASSERT_EQ(str::take_view("", str::shifter(1, 999)), "");
        ASSERT_EQ(str::take_view("", str::shifter(1, 1)), "");
        ASSERT_EQ(str::take_view("", str::shifter(1, 0)), "");
        ASSERT_EQ(str::take_view("", str::shifter(1, -1)), "");
        ASSERT_EQ(str::take_view("", str::shifter(1, -999)), "");
        ASSERT_EQ(str::take_view("", str::shifter(0, 999)), "");
        ASSERT_EQ(str::take_view("", str::shifter(0, 1)), "");
        ASSERT_EQ(str::take_view("", str::shifter(0, 0)), "");
        ASSERT_EQ(str::take_view("", str::shifter(0, -1)), "");
        ASSERT_EQ(str::take_view("", str::shifter(0, -999)), "");
    }
}