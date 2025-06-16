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

TEST(test_str, to_title) {
    ASSERT_EQ(str::to_title("abc def"), "Abc Def");
    ASSERT_EQ(str::to_title("   abc def  \r\t\n\v 123"), "   Abc Def  \r\t\n\v 123");
    ASSERT_EQ(str::to_title("123abc def"), "123Abc Def");
    ASSERT_EQ(str::to_title(""), "");
    ASSERT_EQ(str::to_title("z"), "Z");
    ASSERT_EQ(str::to_title(" q"), " Q");
    ASSERT_EQ(str::to_title("123456"), "123456");
    ASSERT_EQ(str::to_title("123Abc Def"), "123Abc Def");

    ASSERT_EQ(str::is_title("123456"), true);
    ASSERT_EQ(str::is_title(""), false);
    ASSERT_EQ(str::is_title("Abc 123DEF"), false);
    ASSERT_EQ(str::is_title("Abc 123dEF"), false);
    ASSERT_EQ(str::is_title(" abc 123DEF"), false);
    ASSERT_EQ(str::is_title(" Abc Def 123"), true);
    ASSERT_EQ(str::is_title("  A"), true);
    ASSERT_EQ(str::is_title("A"), true);
    ASSERT_EQ(str::is_title("  a"), false);
}
