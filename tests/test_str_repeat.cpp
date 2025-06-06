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

TEST(test_str, repeat) {
    GROUP("字符串重复场景") {
        SECTION("简单字符串重复多次") {
            ASSERT_EQ(str::repeat("0123456789", 5), "01234567890123456789012345678901234567890123456789");
            ASSERT_EQ(str::repeat(" ", 5), "     ");
        }
        SECTION("对空字符串重复：仍然得到的是空字符串") {
            ASSERT_EQ(str::repeat("", 5), "");
            ASSERT_EQ(str::repeat("", 4096), "");
        }
        SECTION("对任意字符串重复0次：仍然是空字符串") {
            ASSERT_EQ(str::repeat("", 0), "");
            ASSERT_EQ(str::repeat("abc", 0), "");
            ASSERT_EQ(str::repeat('A', 0), "");
        }
    }
    GROUP("单字符重复场景") {
        SECTION("简单字符串重复多次") {
            ASSERT_EQ(str::repeat('A', 5), "AAAAA");
            ASSERT_EQ(str::repeat('\0', 5), (std::string_view{"\0\0\0\0\0", 5}));
        }
        SECTION("对任意字符串重复0次：仍然是空字符串") {
            ASSERT_EQ(str::repeat('Q', 0), "");
            ASSERT_EQ(str::repeat('\0', 0), "");
        }
    }
}
