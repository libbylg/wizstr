/**
 * Copyright (c) 2021-2025  libbylg@126.com
 * wizstr is licensed under Mulan PSL v2.
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

#if defined(WIZSTR_NAMESPACE)
using str = WIZSTR_NAMESPACE::str;
#endif

TEST(test_str, simplified_inplace) {
    std::string s;
    SECTION("一般场景") {
        ASSERT_EQ(str::simplified_inplace(s = "\r\n\t   A abbCC中华人\r\n\t   民共\r\n\t   和 \t国\r\n\t   "), "A abbCC中华人 民共 和 国");
        ASSERT_TRUE(str::simplified_inplace(s = "\r\n\t   A abbCC中华人\r\n\t   民共\r\n\t   和 \t国\r\n\t   ", "|", [](str::value_type ch) -> bool {
            return std::isspace(ch);
        }) == "A|abbCC中华人|民共|和|国");
    }

    SECTION("没什么可化简的") {
        ASSERT_EQ(str::simplified_inplace(s = "AabbCC中华人民共和国"), "AabbCC中华人民共和国");
    }

    SECTION("空串") {
        ASSERT_EQ(str::simplified_inplace(s = ""), "");
    }

    SECTION("全空白") {
        ASSERT_EQ(str::simplified_inplace(s = "   \t  \n \r \v"), "");
    }

    SECTION("proc模式") {
        ASSERT_EQ(str::simplified_inplace(s = "   \t  \n \r \v"), "");
    }
}