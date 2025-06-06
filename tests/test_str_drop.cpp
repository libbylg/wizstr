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

TEST(test_str, drop) {
    SECTION("pos+n模式") {
        // 左边界
        ASSERT_EQ(str::drop("abc1234", 0, str::npos), "");
        ASSERT_EQ(str::drop("abc1234", 0, 999), "");
        ASSERT_EQ(str::drop("abc1234", 0, 7), "");
        ASSERT_EQ(str::drop("abc1234", 0, 3), "1234");
        ASSERT_EQ(str::drop("abc1234", 0, 1), "bc1234");
        ASSERT_EQ(str::drop("abc1234", 0, 0), "abc1234");

        // 中间位置
        ASSERT_EQ(str::drop("abc1234", 3, str::npos), "abc");
        ASSERT_EQ(str::drop("abc1234", 3, 999), "abc");
        ASSERT_EQ(str::drop("abc1234", 3, 5), "abc");
        ASSERT_EQ(str::drop("abc1234", 3, 4), "abc");
        ASSERT_EQ(str::drop("abc1234", 3, 2), "abc34");
        ASSERT_EQ(str::drop("abc1234", 3, 1), "abc234");
        ASSERT_EQ(str::drop("abc1234", 3, 0), "abc1234");

        // 右边界
        ASSERT_EQ(str::drop("abc1234", 7, str::npos), "abc1234");
        ASSERT_EQ(str::drop("abc1234", 7, 999), "abc1234");
        ASSERT_EQ(str::drop("abc1234", 7, 1), "abc1234");
        ASSERT_EQ(str::drop("abc1234", 7, 0), "abc1234");

        // 超过右边界
        ASSERT_EQ(str::drop("abc1234", 8, str::npos), "abc1234");
        ASSERT_EQ(str::drop("abc1234", 8, 999), "abc1234");
        ASSERT_EQ(str::drop("abc1234", 8, 1), "abc1234");
        ASSERT_EQ(str::drop("abc1234", 8, 0), "abc1234");

        // 空串
        ASSERT_EQ(str::drop("", 0, 0), "");
        ASSERT_EQ(str::drop("", 0, 1), "");
        ASSERT_EQ(str::drop("", 0, 999), "");
        ASSERT_EQ(str::drop("", 0, str::npos), "");
        ASSERT_EQ(str::drop("", 1, 0), "");
        ASSERT_EQ(str::drop("", 1, 1), "");
        ASSERT_EQ(str::drop("", 1, 999), "");
        ASSERT_EQ(str::drop("", 1, str::npos), "");
        ASSERT_EQ(str::drop("", str::npos, 0), "");
        ASSERT_EQ(str::drop("", str::npos, 1), "");
        ASSERT_EQ(str::drop("", str::npos, 999), "");
        ASSERT_EQ(str::drop("", str::npos, str::npos), "");
    }
    SECTION("pos模式") {
        // 一般情况
        ASSERT_EQ(str::drop("3bc1233", 0), "");
        ASSERT_EQ(str::drop("3bc1233", 3), "3bc");
        ASSERT_EQ(str::drop("3bc1239", 6), "3bc123");
        ASSERT_EQ(str::drop("3bc1234", 7), "3bc1234");
        ASSERT_EQ(str::drop("3bc1234", 8), "3bc1234");
        ASSERT_EQ(str::drop("3bc1234", str::npos), "3bc1234");

        // 空串
        ASSERT_EQ(str::drop("", 0), "");
        ASSERT_EQ(str::drop("", 1), "");
        ASSERT_EQ(str::drop("", str::npos), "");
    }
    SECTION("range模式") {
        // 左边界
        ASSERT_EQ(str::drop("abc1234", str::range(0, str::npos)), "");
        ASSERT_EQ(str::drop("abc1234", str::range(0, 999)), "");
        ASSERT_EQ(str::drop("abc1234", str::range(0, 7)), "");
        ASSERT_EQ(str::drop("abc1234", str::range(0, 3)), "1234");
        ASSERT_EQ(str::drop("abc1234", str::range(0, 1)), "bc1234");
        ASSERT_EQ(str::drop("abc1234", str::range(0, 0)), "abc1234");

        // 中间位置
        ASSERT_EQ(str::drop("abc1234", str::range(3, str::npos)), "abc");
        ASSERT_EQ(str::drop("abc1234", str::range(3, 999)), "abc");
        ASSERT_EQ(str::drop("abc1234", str::range(3, 5)), "abc");
        ASSERT_EQ(str::drop("abc1234", str::range(3, 4)), "abc");
        ASSERT_EQ(str::drop("abc1234", str::range(3, 2)), "abc34");
        ASSERT_EQ(str::drop("abc1234", str::range(3, 1)), "abc234");
        ASSERT_EQ(str::drop("abc1234", str::range(3, 0)), "abc1234");

        // 右边界
        ASSERT_EQ(str::drop("abc1234", str::range(7, str::npos)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::range(7, 999)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::range(7, 1)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::range(7, 0)), "abc1234");

        // 超过右边界
        ASSERT_EQ(str::drop("abc1234", str::range(8, str::npos)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::range(8, 999)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::range(8, 1)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::range(8, 0)), "abc1234");

        // 空串
        ASSERT_EQ(str::drop("", str::range(0, 0)), "");
        ASSERT_EQ(str::drop("", str::range(0, 1)), "");
        ASSERT_EQ(str::drop("", str::range(0, 999)), "");
        ASSERT_EQ(str::drop("", str::range(0, str::npos)), "");
        ASSERT_EQ(str::drop("", str::range(1, 0)), "");
        ASSERT_EQ(str::drop("", str::range(1, 1)), "");
        ASSERT_EQ(str::drop("", str::range(1, 999)), "");
        ASSERT_EQ(str::drop("", str::range(1, str::npos)), "");
        ASSERT_EQ(str::drop("", str::range(str::npos, 0)), "");
        ASSERT_EQ(str::drop("", str::range(str::npos, 1)), "");
        ASSERT_EQ(str::drop("", str::range(str::npos, 999)), "");
        ASSERT_EQ(str::drop("", str::range(str::npos, str::npos)), "");
    }
    SECTION("interval模式") {
        // 左边界
        ASSERT_EQ(str::drop("abc1234", str::interval(0, 0)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::interval(0, 1)), "bc1234");
        ASSERT_EQ(str::drop("abc1234", str::interval(0, 3)), "1234");
        ASSERT_EQ(str::drop("abc1234", str::interval(0, 6)), "4");
        ASSERT_EQ(str::drop("abc1234", str::interval(0, 7)), "");
        ASSERT_EQ(str::drop("abc1234", str::interval(0, 999)), "");
        ASSERT_EQ(str::drop("abc1234", str::interval(0, str::npos)), "");

        // 中间位置
        ASSERT_EQ(str::drop("abc1234", str::interval(3, 0)), "1234");
        ASSERT_EQ(str::drop("abc1234", str::interval(3, 1)), "a1234");
        ASSERT_EQ(str::drop("abc1234", str::interval(3, 3)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::interval(3, 6)), "abc4");
        ASSERT_EQ(str::drop("abc1234", str::interval(3, 7)), "abc");
        ASSERT_EQ(str::drop("abc1234", str::interval(3, 999)), "abc");
        ASSERT_EQ(str::drop("abc1234", str::interval(3, str::npos)), "abc");

        // 右边界
        ASSERT_EQ(str::drop("abc1234", str::interval(7, 0)), "");
        ASSERT_EQ(str::drop("abc1234", str::interval(7, 1)), "a");
        ASSERT_EQ(str::drop("abc1234", str::interval(7, 3)), "abc");
        ASSERT_EQ(str::drop("abc1234", str::interval(7, 6)), "abc123");
        ASSERT_EQ(str::drop("abc1234", str::interval(7, 7)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::interval(7, 999)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::interval(7, str::npos)), "abc1234");

        // 超过右边界
        ASSERT_EQ(str::drop("abc1234", str::interval(8, 0)), "");
        ASSERT_EQ(str::drop("abc1234", str::interval(8, 1)), "a");
        ASSERT_EQ(str::drop("abc1234", str::interval(8, 3)), "abc");
        ASSERT_EQ(str::drop("abc1234", str::interval(8, 6)), "abc123");
        ASSERT_EQ(str::drop("abc1234", str::interval(8, 7)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::interval(8, 999)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::interval(8, str::npos)), "abc1234");

        // 空串
        ASSERT_EQ(str::drop("", str::interval(0, 0)), "");
        ASSERT_EQ(str::drop("", str::interval(0, 1)), "");
        ASSERT_EQ(str::drop("", str::interval(0, 999)), "");
        ASSERT_EQ(str::drop("", str::interval(0, str::npos)), "");
        ASSERT_EQ(str::drop("", str::interval(1, 0)), "");
        ASSERT_EQ(str::drop("", str::interval(1, 1)), "");
        ASSERT_EQ(str::drop("", str::interval(1, 999)), "");
        ASSERT_EQ(str::drop("", str::interval(1, str::npos)), "");
        ASSERT_EQ(str::drop("", str::interval(str::npos, 0)), "");
        ASSERT_EQ(str::drop("", str::interval(str::npos, 1)), "");
        ASSERT_EQ(str::drop("", str::interval(str::npos, 999)), "");
        ASSERT_EQ(str::drop("", str::interval(str::npos, str::npos)), "");
    }
    SECTION("shifter模式") {
        // 左边界
        ASSERT_EQ(str::drop("abc1234", str::shifter(0, 999)), "");
        ASSERT_EQ(str::drop("abc1234", str::shifter(0, 7)), "");
        ASSERT_EQ(str::drop("abc1234", str::shifter(0, 3)), "1234");
        ASSERT_EQ(str::drop("abc1234", str::shifter(0, 1)), "bc1234");
        ASSERT_EQ(str::drop("abc1234", str::shifter(0, 0)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::shifter(0, -1)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::shifter(0, -999)), "abc1234");

        // 中间位置
        ASSERT_EQ(str::drop("abc1234", str::shifter(3, 999)), "abc");
        ASSERT_EQ(str::drop("abc1234", str::shifter(3, 5)), "abc");
        ASSERT_EQ(str::drop("abc1234", str::shifter(3, 4)), "abc");
        ASSERT_EQ(str::drop("abc1234", str::shifter(3, 2)), "abc34");
        ASSERT_EQ(str::drop("abc1234", str::shifter(3, 1)), "abc234");
        ASSERT_EQ(str::drop("abc1234", str::shifter(3, 0)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::shifter(3, -2)), "a1234");
        ASSERT_EQ(str::drop("abc1234", str::shifter(3, -3)), "1234");
        ASSERT_EQ(str::drop("abc1234", str::shifter(3, -999)), "1234");

        // 右边界
        ASSERT_EQ(str::drop("abc1234", str::shifter(7, 999)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::shifter(7, 1)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::shifter(7, 0)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::shifter(7, -1)), "abc123");
        ASSERT_EQ(str::drop("abc1234", str::shifter(7, -3)), "abc1");
        ASSERT_EQ(str::drop("abc1234", str::shifter(7, -7)), "");
        ASSERT_EQ(str::drop("abc1234", str::shifter(7, -8)), "");
        ASSERT_EQ(str::drop("abc1234", str::shifter(7, -999)), "");

        // 超过右边界
        ASSERT_EQ(str::drop("abc1234", str::shifter(8, 999)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::shifter(8, 1)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::shifter(8, 0)), "abc1234");
        ASSERT_EQ(str::drop("abc1234", str::shifter(8, -1)), "abc123");
        ASSERT_EQ(str::drop("abc1234", str::shifter(8, -3)), "abc1");
        ASSERT_EQ(str::drop("abc1234", str::shifter(8, -7)), "");
        ASSERT_EQ(str::drop("abc1234", str::shifter(8, -8)), "");
        ASSERT_EQ(str::drop("abc1234", str::shifter(8, -999)), "");

        // 空串
        ASSERT_EQ(str::drop("", str::shifter(1, 999)), "");
        ASSERT_EQ(str::drop("", str::shifter(1, 1)), "");
        ASSERT_EQ(str::drop("", str::shifter(1, 0)), "");
        ASSERT_EQ(str::drop("", str::shifter(1, -1)), "");
        ASSERT_EQ(str::drop("", str::shifter(1, -999)), "");
        ASSERT_EQ(str::drop("", str::shifter(0, 999)), "");
        ASSERT_EQ(str::drop("", str::shifter(0, 1)), "");
        ASSERT_EQ(str::drop("", str::shifter(0, 0)), "");
        ASSERT_EQ(str::drop("", str::shifter(0, -1)), "");
        ASSERT_EQ(str::drop("", str::shifter(0, -999)), "");
    }
    SECTION("proc模式") {
        ASSERT_EQ(str::drop("abc1de2f3h4", [](str::value_type ch) -> bool {
            return std::isdigit(ch);
        }),
            "abcdefh");
        ASSERT_EQ(str::drop("", [](str::value_type ch) -> bool {
            return std::isdigit(ch);
        }),
            "");
    }
    SECTION("charset模式") {
        ASSERT_EQ(str::drop("abc1de2f3h4", str::charset(str::all_digits)), "abcdefh");
        ASSERT_EQ(str::drop("", str::charset(str::all_digits)), "");
        ASSERT_EQ(str::drop("abc1de2f3h4", str::charset("")), "abc1de2f3h4");
    }
}