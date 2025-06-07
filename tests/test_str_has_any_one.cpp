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

TEST(test_str, has_any_one) {
    SECTION("字符集") {
        ASSERT_EQ(str::has_any_one("A", str::charset("ABC")), true);
        ASSERT_EQ(str::has_any_one("a", str::charset("ABC")), false);
        ASSERT_EQ(str::has_any_one("0", str::charset("012ABC")), true);
        ASSERT_EQ(str::has_any_one("ABC123", str::charset("0123456789ABCDEFG")), true);
        ASSERT_EQ(str::has_any_one("", str::charset("012ABC")), false);
        ASSERT_EQ(str::has_any_one("abc", str::charset("")), false);
        ASSERT_EQ(str::has_any_one("", str::charset("")), false);
    }
    SECTION("proc") {
        // clang-format off
        ASSERT_EQ(str::has_any_one("123456", [](str::value_type ch) -> bool {return std::isalpha(ch);}), false);
        ASSERT_EQ(str::has_any_one("", [](str::value_type ch) -> bool {return std::isalpha(ch);}), false);
        ASSERT_EQ(str::has_any_one("Abc 123DEF", [](str::value_type ch) -> bool {return std::isalpha(ch);}), true);
        ASSERT_EQ(str::has_any_one(" abc 123DEF", [](str::value_type ch) -> bool {return std::isxdigit(ch);}), true);
        ASSERT_EQ(str::has_any_one("  #$%^&", [](str::value_type ch) -> bool {return std::isalpha(ch);}), false);
        // clang-format on
    }
}
