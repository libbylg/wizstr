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

TEST(test_str, is_literal_real) {
    SECTION("空串") {
        ASSERT_EQ(str::is_literal_real(""), false);
    }
    SECTION("作为整数:简单形式") {
        ASSERT_EQ(str::is_literal_real("0"), true);
        ASSERT_EQ(str::is_literal_real("1"), true);
        ASSERT_EQ(str::is_literal_real("2"), true);
        ASSERT_EQ(str::is_literal_real("3"), true);
        ASSERT_EQ(str::is_literal_real("4"), true);
        ASSERT_EQ(str::is_literal_real("5"), true);
        ASSERT_EQ(str::is_literal_real("6"), true);
        ASSERT_EQ(str::is_literal_real("7"), true);
        ASSERT_EQ(str::is_literal_real("8"), true);
        ASSERT_EQ(str::is_literal_real("9"), true);

        ASSERT_EQ(str::is_literal_real("00000"), true);
        ASSERT_EQ(str::is_literal_real("32768"), true);
        ASSERT_EQ(str::is_literal_real("99999999999999000000000000009999999999999900000000000000"), true);
    }
    SECTION("作为整数:带符号") {
        ASSERT_EQ(str::is_literal_real("+0"), true);
        ASSERT_EQ(str::is_literal_real("-32768"), true);
    }
    SECTION("作为小数:整数小数部分齐全") {
        ASSERT_EQ(str::is_literal_real("0.1"), true);
        ASSERT_EQ(str::is_literal_real("0.12"), true);
        ASSERT_EQ(str::is_literal_real("123.4567890"), true);
        ASSERT_EQ(str::is_literal_real("+0.1"), true);
        ASSERT_EQ(str::is_literal_real("-123.4567890"), true);
    }
    SECTION("作为小数:整数部分残缺") {
        ASSERT_EQ(str::is_literal_real(".1"), true);
        ASSERT_EQ(str::is_literal_real(".12"), true);
        ASSERT_EQ(str::is_literal_real(".4567890"), true);
        ASSERT_EQ(str::is_literal_real("+.1"), true);
        ASSERT_EQ(str::is_literal_real("-.12"), true);
        ASSERT_EQ(str::is_literal_real(".4567890"), true);
    }
    SECTION("作为小数:小数部分残缺") {
        ASSERT_EQ(str::is_literal_real("1."), true);
        ASSERT_EQ(str::is_literal_real("12."), true);
        ASSERT_EQ(str::is_literal_real("4567890."), true);
        ASSERT_EQ(str::is_literal_real("+1."), true);
        ASSERT_EQ(str::is_literal_real("-12."), true);
        ASSERT_EQ(str::is_literal_real("4567890."), true);
    }
    SECTION("作为指数:作为整数:简单形式") {
        ASSERT_EQ(str::is_literal_real("0e0"), true);
        ASSERT_EQ(str::is_literal_real("1e+10"), true);
        ASSERT_EQ(str::is_literal_real("22e-18"), true);

        ASSERT_EQ(str::is_literal_real("+0E0"), true);
        ASSERT_EQ(str::is_literal_real("+1E+10"), true);
        ASSERT_EQ(str::is_literal_real("+22E-18"), true);
    }
    SECTION("作为指数:作为小数:整数小数部分齐全") {
        ASSERT_EQ(str::is_literal_real("0.1e1"), true);
        ASSERT_EQ(str::is_literal_real("0.12e+10"), true);
        ASSERT_EQ(str::is_literal_real("123.4567890e+19"), true);
        ASSERT_EQ(str::is_literal_real("+0.1e-31"), true);
        ASSERT_EQ(str::is_literal_real("-123.4567890e-6"), true);
    }
    SECTION("作为指数:作为小数:整数部分残缺") {
        ASSERT_EQ(str::is_literal_real(".1e10"), true);
        ASSERT_EQ(str::is_literal_real(".12e-9"), true);
        ASSERT_EQ(str::is_literal_real(".4567890e+3"), true);
        ASSERT_EQ(str::is_literal_real("+.1e2"), true);
        ASSERT_EQ(str::is_literal_real("-.12e-2"), true);
        ASSERT_EQ(str::is_literal_real(".4567890e+30"), true);

        ASSERT_EQ(str::is_literal_real(".1E11"), true);
        ASSERT_EQ(str::is_literal_real(".12E-9"), true);
        ASSERT_EQ(str::is_literal_real(".4567890E+3"), true);
        ASSERT_EQ(str::is_literal_real("+.1E2"), true);
        ASSERT_EQ(str::is_literal_real("-.12E-2"), true);
        ASSERT_EQ(str::is_literal_real(".4567890E+39"), true);
    }
    SECTION("作为指数:作为小数:小数部分残缺") {
        ASSERT_EQ(str::is_literal_real("1.e0"), true);
        ASSERT_EQ(str::is_literal_real("12.e-22"), true);
        ASSERT_EQ(str::is_literal_real("4567890.e32"), true);
        ASSERT_EQ(str::is_literal_real("+1.e-19"), true);
        ASSERT_EQ(str::is_literal_real("-12.e03"), true);
        ASSERT_EQ(str::is_literal_real("4567890.e-9"), true);

        ASSERT_EQ(str::is_literal_real("1.E30"), true);
        ASSERT_EQ(str::is_literal_real("12.E+24"), true);
        ASSERT_EQ(str::is_literal_real("4567890.E-45"), true);
        ASSERT_EQ(str::is_literal_real("+1.E30"), true);
        ASSERT_EQ(str::is_literal_real("-12.E+24"), true);
        ASSERT_EQ(str::is_literal_real("4567890.E-45"), true);
    }
    SECTION("异常场景") {
        ASSERT_EQ(str::is_literal_real("A"), false);
        ASSERT_EQ(str::is_literal_real(" "), false);
        ASSERT_EQ(str::is_literal_real("+"), false);
        ASSERT_EQ(str::is_literal_real("-"), false);
        ASSERT_EQ(str::is_literal_real("+."), false);
        ASSERT_EQ(str::is_literal_real("."), false);
        ASSERT_EQ(str::is_literal_real(".e"), false);
        ASSERT_EQ(str::is_literal_real("+e"), false);
        ASSERT_EQ(str::is_literal_real("+.e"), false);
        ASSERT_EQ(str::is_literal_real("-E-18"), false);
        ASSERT_EQ(str::is_literal_real("-e+"), false);
        ASSERT_EQ(str::is_literal_real("22E"), false);
        ASSERT_EQ(str::is_literal_real("22e"), false);
        ASSERT_EQ(str::is_literal_real("22e+"), false);
        ASSERT_EQ(str::is_literal_real("e+2"), false);
        ASSERT_EQ(str::is_literal_real("22eQ"), false);
        ASSERT_EQ(str::is_literal_real("22e10Q"), false);
        ASSERT_EQ(str::is_literal_real("22Xe+10"), false);
        ASSERT_EQ(str::is_literal_real("22e+10Q"), false);
        ASSERT_EQ(str::is_literal_real("22e-Q"), false);
    }
}