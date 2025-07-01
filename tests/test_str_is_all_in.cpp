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

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, is_all_in) {
    SECTION("字符集") {
        ASSERT_EQ(str::is_all_in("A", str::charset("ABC")), true);
        ASSERT_EQ(str::is_all_in("ACAACABCBBCB", str::charset("ABC")), true);
        ASSERT_EQ(str::is_all_in("ACM", str::charset("ABC")), false);
        ASSERT_EQ(str::is_all_in("", str::charset("ABC")), true);
        ASSERT_EQ(str::is_all_in("ABC", str::charset("")), false);
        ASSERT_EQ(str::is_all_in("", str::charset("")), true);
    }
    SECTION("proc") {
        // clang-format off
        ASSERT_EQ(str::is_all_in("A", [](str::value_type ch) -> bool {return std::isupper(ch);}),true);
        ASSERT_EQ(str::is_all_in("ACAACABCBBCB", [](str::value_type ch) -> bool {return std::isupper(ch);}), true);
        ASSERT_EQ(str::is_all_in("Acm", [](str::value_type ch) -> bool {return std::isupper(ch);}), false);
        ASSERT_EQ(str::is_all_in("123", [](str::value_type ch) -> bool {return std::isupper(ch);}), false);
        ASSERT_EQ(str::is_all_in("1A2B3", [](str::value_type ch) -> bool {return std::isupper(ch);}), false);
        ASSERT_EQ(str::is_all_in("", [](str::value_type ch) -> bool {return std::isupper(ch);}), true);
        // clang-format on
    }
}
