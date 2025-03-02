#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, is_literal_bool) {
    SECTION("标准值") {
        ASSERT_TRUE(view::is_literal_bool("1"));
        ASSERT_TRUE(view::is_literal_bool("0"));
        ASSERT_TRUE(view::is_literal_bool("on"));
        ASSERT_TRUE(view::is_literal_bool("off"));
        ASSERT_TRUE(view::is_literal_bool("ON"));
        ASSERT_TRUE(view::is_literal_bool("OFF"));
        ASSERT_TRUE(view::is_literal_bool("On"));
        ASSERT_TRUE(view::is_literal_bool("Off"));
        ASSERT_TRUE(view::is_literal_bool("Yes"));
        ASSERT_TRUE(view::is_literal_bool("No"));
        ASSERT_TRUE(view::is_literal_bool("yes"));
        ASSERT_TRUE(view::is_literal_bool("no"));
        ASSERT_TRUE(view::is_literal_bool("YES"));
        ASSERT_TRUE(view::is_literal_bool("NO"));
        ASSERT_TRUE(view::is_literal_bool("True"));
        ASSERT_TRUE(view::is_literal_bool("False"));
        ASSERT_TRUE(view::is_literal_bool("true"));
        ASSERT_TRUE(view::is_literal_bool("false"));
        ASSERT_TRUE(view::is_literal_bool("TRUE"));
        ASSERT_TRUE(view::is_literal_bool("FALSE"));
    }
    SECTION("空串") {
        ASSERT_EQ(view::is_literal_bool(""), false);
    }
    SECTION("非标值") {
        ASSERT_EQ(view::is_literal_bool("FalsE"), false);
        ASSERT_EQ(view::is_literal_bool("tRUe"), false);
        ASSERT_EQ(view::is_literal_bool("true "), false);
    }
    SECTION("超长值") {
        ASSERT_EQ(view::is_literal_bool("hello-workd"), false);
    }
}

TEST(test_view, is_literal_true) {
    SECTION("标准值") {
        ASSERT_EQ(view::is_literal_true("1"), true);
        ASSERT_EQ(view::is_literal_true("0"), false);
        ASSERT_EQ(view::is_literal_true("on"), true);
        ASSERT_EQ(view::is_literal_true("off"), false);
        ASSERT_EQ(view::is_literal_true("ON"), true);
        ASSERT_EQ(view::is_literal_true("OFF"), false);
        ASSERT_EQ(view::is_literal_true("On"), true);
        ASSERT_EQ(view::is_literal_true("Off"), false);
        ASSERT_EQ(view::is_literal_true("Yes"), true);
        ASSERT_EQ(view::is_literal_true("No"), false);
        ASSERT_EQ(view::is_literal_true("yes"), true);
        ASSERT_EQ(view::is_literal_true("no"), false);
        ASSERT_EQ(view::is_literal_true("YES"), true);
        ASSERT_EQ(view::is_literal_true("NO"), false);
        ASSERT_EQ(view::is_literal_true("True"), true);
        ASSERT_EQ(view::is_literal_true("False"), false);
        ASSERT_EQ(view::is_literal_true("true"), true);
        ASSERT_EQ(view::is_literal_true("false"), false);
        ASSERT_EQ(view::is_literal_true("TRUE"), true);
        ASSERT_EQ(view::is_literal_true("FALSE"), false);
    }
    SECTION("空串") {
        ASSERT_EQ(view::is_literal_true(""), false);
    }
    SECTION("非标值") {
        ASSERT_EQ(view::is_literal_true("FalsE"), false);
        ASSERT_EQ(view::is_literal_true("tRUe"), false);
        ASSERT_EQ(view::is_literal_true("true "), false);
    }
    SECTION("超长值") {
        ASSERT_EQ(view::is_literal_true("hello-workd"), false);
    }
}

TEST(test_view, is_literal_false) {
    SECTION("标准值") {
        ASSERT_EQ(view::is_literal_false("1"), false);
        ASSERT_EQ(view::is_literal_false("0"), true);
        ASSERT_EQ(view::is_literal_false("on"), false);
        ASSERT_EQ(view::is_literal_false("off"), true);
        ASSERT_EQ(view::is_literal_false("ON"), false);
        ASSERT_EQ(view::is_literal_false("OFF"), true);
        ASSERT_EQ(view::is_literal_false("On"), false);
        ASSERT_EQ(view::is_literal_false("Off"), true);
        ASSERT_EQ(view::is_literal_false("Yes"), false);
        ASSERT_EQ(view::is_literal_false("No"), true);
        ASSERT_EQ(view::is_literal_false("yes"), false);
        ASSERT_EQ(view::is_literal_false("no"), true);
        ASSERT_EQ(view::is_literal_false("YES"), false);
        ASSERT_EQ(view::is_literal_false("NO"), true);
        ASSERT_EQ(view::is_literal_false("True"), false);
        ASSERT_EQ(view::is_literal_false("False"), true);
        ASSERT_EQ(view::is_literal_false("true"), false);
        ASSERT_EQ(view::is_literal_false("false"), true);
        ASSERT_EQ(view::is_literal_false("TRUE"), false);
        ASSERT_EQ(view::is_literal_false("FALSE"), true);
    }
    SECTION("空串") {
        ASSERT_EQ(view::is_literal_false(""), false);
    }
    SECTION("非标值") {
        ASSERT_EQ(view::is_literal_false("FalsE"), false);
        ASSERT_EQ(view::is_literal_false("tRUe"), false);
        ASSERT_EQ(view::is_literal_false("true "), false);
    }
    SECTION("超长值") {
        ASSERT_EQ(view::is_literal_false("hello-workd"), false);
    }
}

TEST(test_view, is_literal_integer) {
    SECTION("空串") {
        ASSERT_EQ(view::is_literal_integer(""), false);
    }
    SECTION("结单个数字") {
        ASSERT_EQ(view::is_literal_integer("0"), true);
        ASSERT_EQ(view::is_literal_integer("1"), true);
        ASSERT_EQ(view::is_literal_integer("2"), true);
        ASSERT_EQ(view::is_literal_integer("3"), true);
        ASSERT_EQ(view::is_literal_integer("4"), true);
        ASSERT_EQ(view::is_literal_integer("5"), true);
        ASSERT_EQ(view::is_literal_integer("6"), true);
        ASSERT_EQ(view::is_literal_integer("7"), true);
        ASSERT_EQ(view::is_literal_integer("8"), true);
        ASSERT_EQ(view::is_literal_integer("9"), true);
        ASSERT_EQ(view::is_literal_integer("A"), false);
        ASSERT_EQ(view::is_literal_integer(" "), false);
    }
    SECTION("多位数字") {
        ASSERT_EQ(view::is_literal_integer("00000"), true);
        ASSERT_EQ(view::is_literal_integer("32768"), true);
        ASSERT_EQ(view::is_literal_integer("99999999999999000000000000009999999999999900000000000000"), true);
    }
    SECTION("带符号") {
        ASSERT_EQ(view::is_literal_integer("+0"), true);
        ASSERT_EQ(view::is_literal_integer("-32768"), true);
        ASSERT_EQ(view::is_literal_integer("+"), false);
    }
}

TEST(test_view, is_literal_real) {
    SECTION("空串") {
        ASSERT_EQ(view::is_literal_real(""), false);
    }
    SECTION("作为整数:简单形式") {
        ASSERT_EQ(view::is_literal_real("0"), true);
        ASSERT_EQ(view::is_literal_real("1"), true);
        ASSERT_EQ(view::is_literal_real("2"), true);
        ASSERT_EQ(view::is_literal_real("3"), true);
        ASSERT_EQ(view::is_literal_real("4"), true);
        ASSERT_EQ(view::is_literal_real("5"), true);
        ASSERT_EQ(view::is_literal_real("6"), true);
        ASSERT_EQ(view::is_literal_real("7"), true);
        ASSERT_EQ(view::is_literal_real("8"), true);
        ASSERT_EQ(view::is_literal_real("9"), true);

        ASSERT_EQ(view::is_literal_real("00000"), true);
        ASSERT_EQ(view::is_literal_real("32768"), true);
        ASSERT_EQ(view::is_literal_real("99999999999999000000000000009999999999999900000000000000"), true);
    }
    SECTION("作为整数:带符号") {
        ASSERT_EQ(view::is_literal_real("+0"), true);
        ASSERT_EQ(view::is_literal_real("-32768"), true);
    }
    SECTION("作为小数:整数小数部分齐全") {
        ASSERT_EQ(view::is_literal_real("0.1"), true);
        ASSERT_EQ(view::is_literal_real("0.12"), true);
        ASSERT_EQ(view::is_literal_real("123.4567890"), true);
        ASSERT_EQ(view::is_literal_real("+0.1"), true);
        ASSERT_EQ(view::is_literal_real("-123.4567890"), true);
    }
    SECTION("作为小数:整数部分残缺") {
        ASSERT_EQ(view::is_literal_real(".1"), true);
        ASSERT_EQ(view::is_literal_real(".12"), true);
        ASSERT_EQ(view::is_literal_real(".4567890"), true);
        ASSERT_EQ(view::is_literal_real("+.1"), true);
        ASSERT_EQ(view::is_literal_real("-.12"), true);
        ASSERT_EQ(view::is_literal_real(".4567890"), true);
    }
    SECTION("作为小数:小数部分残缺") {
        ASSERT_EQ(view::is_literal_real("1."), true);
        ASSERT_EQ(view::is_literal_real("12."), true);
        ASSERT_EQ(view::is_literal_real("4567890."), true);
        ASSERT_EQ(view::is_literal_real("+1."), true);
        ASSERT_EQ(view::is_literal_real("-12."), true);
        ASSERT_EQ(view::is_literal_real("4567890."), true);
    }
    SECTION("作为指数:作为整数:简单形式") {
        ASSERT_EQ(view::is_literal_real("0e0"), true);
        ASSERT_EQ(view::is_literal_real("1e+10"), true);
        ASSERT_EQ(view::is_literal_real("22e-18"), true);

        ASSERT_EQ(view::is_literal_real("+0E0"), true);
        ASSERT_EQ(view::is_literal_real("+1E+10"), true);
        ASSERT_EQ(view::is_literal_real("+22E-18"), true);
    }
    SECTION("作为指数:作为小数:整数小数部分齐全") {
        ASSERT_EQ(view::is_literal_real("0.1e1"), true);
        ASSERT_EQ(view::is_literal_real("0.12e+10"), true);
        ASSERT_EQ(view::is_literal_real("123.4567890e+19"), true);
        ASSERT_EQ(view::is_literal_real("+0.1e-31"), true);
        ASSERT_EQ(view::is_literal_real("-123.4567890e-6"), true);
    }
    SECTION("作为指数:作为小数:整数部分残缺") {
        ASSERT_EQ(view::is_literal_real(".1e10"), true);
        ASSERT_EQ(view::is_literal_real(".12e-9"), true);
        ASSERT_EQ(view::is_literal_real(".4567890e+3"), true);
        ASSERT_EQ(view::is_literal_real("+.1e2"), true);
        ASSERT_EQ(view::is_literal_real("-.12e-2"), true);
        ASSERT_EQ(view::is_literal_real(".4567890e+30"), true);

        ASSERT_EQ(view::is_literal_real(".1E11"), true);
        ASSERT_EQ(view::is_literal_real(".12E-9"), true);
        ASSERT_EQ(view::is_literal_real(".4567890E+3"), true);
        ASSERT_EQ(view::is_literal_real("+.1E2"), true);
        ASSERT_EQ(view::is_literal_real("-.12E-2"), true);
        ASSERT_EQ(view::is_literal_real(".4567890E+39"), true);
    }
    SECTION("作为指数:作为小数:小数部分残缺") {
        ASSERT_EQ(view::is_literal_real("1.e0"), true);
        ASSERT_EQ(view::is_literal_real("12.e-22"), true);
        ASSERT_EQ(view::is_literal_real("4567890.e32"), true);
        ASSERT_EQ(view::is_literal_real("+1.e-19"), true);
        ASSERT_EQ(view::is_literal_real("-12.e03"), true);
        ASSERT_EQ(view::is_literal_real("4567890.e-9"), true);

        ASSERT_EQ(view::is_literal_real("1.E30"), true);
        ASSERT_EQ(view::is_literal_real("12.E+24"), true);
        ASSERT_EQ(view::is_literal_real("4567890.E-45"), true);
        ASSERT_EQ(view::is_literal_real("+1.E30"), true);
        ASSERT_EQ(view::is_literal_real("-12.E+24"), true);
        ASSERT_EQ(view::is_literal_real("4567890.E-45"), true);
    }
    SECTION("异常场景") {
        ASSERT_EQ(view::is_literal_real("A"), false);
        ASSERT_EQ(view::is_literal_real(" "), false);
        ASSERT_EQ(view::is_literal_real("+"), false);
        ASSERT_EQ(view::is_literal_real("-"), false);
        ASSERT_EQ(view::is_literal_real("+."), false);
        ASSERT_EQ(view::is_literal_real("."), false);
        ASSERT_EQ(view::is_literal_real(".e"), false);
        ASSERT_EQ(view::is_literal_real("+e"), false);
        ASSERT_EQ(view::is_literal_real("+.e"), false);
        ASSERT_EQ(view::is_literal_real("-E-18"), false);
        ASSERT_EQ(view::is_literal_real("-e+"), false);
        ASSERT_EQ(view::is_literal_real("22E"), false);
        ASSERT_EQ(view::is_literal_real("22e"), false);
        ASSERT_EQ(view::is_literal_real("22e+"), false);
        ASSERT_EQ(view::is_literal_real("e+2"), false);
        ASSERT_EQ(view::is_literal_real("22eQ"), false);
        ASSERT_EQ(view::is_literal_real("22e10Q"), false);
        ASSERT_EQ(view::is_literal_real("22Xe+10"), false);
        ASSERT_EQ(view::is_literal_real("22e+10Q"), false);
        ASSERT_EQ(view::is_literal_real("22e-Q"), false);
    }
}