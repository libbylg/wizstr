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

TEST(test_str, simplified) {
    SECTION("一般场景") {
        ASSERT_EQ(str::simplified("\r\n\t   A abbCC中华人\r\n\t   民共\r\n\t   和 \t国\r\n\t   "), "A abbCC中华人 民共 和 国");
        ASSERT_TRUE(str::simplified("\r\n\t   A abbCC中华人\r\n\t   民共\r\n\t   和 \t国\r\n\t   ", "|", [](str::value_type ch) -> bool {
            return std::isspace(ch);
        }) == "A|abbCC中华人|民共|和|国");
    }

    SECTION("没什么可化简的") {
        ASSERT_EQ(str::simplified("AabbCC中华人民共和国"), "AabbCC中华人民共和国");
    }

    SECTION("空串") {
        ASSERT_EQ(str::simplified(""), "");
    }

    SECTION("全空白") {
        ASSERT_EQ(str::simplified("   \t  \n \r \v"), "");
    }

    SECTION("proc模式") {
        ASSERT_EQ(str::simplified("   \t  \n \r \v"), "");
    }
}