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

TEST(test_str, is_literal_integer) {
    SECTION("空串") {
        ASSERT_EQ(str::is_literal_integer(""), false);
    }
    SECTION("结单个数字") {
        ASSERT_EQ(str::is_literal_integer("0"), true);
        ASSERT_EQ(str::is_literal_integer("1"), true);
        ASSERT_EQ(str::is_literal_integer("2"), true);
        ASSERT_EQ(str::is_literal_integer("3"), true);
        ASSERT_EQ(str::is_literal_integer("4"), true);
        ASSERT_EQ(str::is_literal_integer("5"), true);
        ASSERT_EQ(str::is_literal_integer("6"), true);
        ASSERT_EQ(str::is_literal_integer("7"), true);
        ASSERT_EQ(str::is_literal_integer("8"), true);
        ASSERT_EQ(str::is_literal_integer("9"), true);
        ASSERT_EQ(str::is_literal_integer("A"), false);
        ASSERT_EQ(str::is_literal_integer(" "), false);
    }
    SECTION("多位数字") {
        ASSERT_EQ(str::is_literal_integer("00000"), true);
        ASSERT_EQ(str::is_literal_integer("32768"), true);
        ASSERT_EQ(str::is_literal_integer("99999999999999000000000000009999999999999900000000000000"), true);
    }
    SECTION("带符号") {
        ASSERT_EQ(str::is_literal_integer("+0"), true);
        ASSERT_EQ(str::is_literal_integer("-32768"), true);
        ASSERT_EQ(str::is_literal_integer("+"), false);
    }
}
