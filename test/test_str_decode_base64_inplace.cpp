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

TEST(test_str, decode_base64_inplace) {
    SECTION("一般情况") {
        std::string s;
        ASSERT_EQ(str::decode_base64_inplace(s = ""), "");
        ASSERT_EQ(str::decode_base64_inplace(s = "YQ=="), "a");
        ASSERT_EQ(str::decode_base64_inplace(s = "YWI="), "ab");
        ASSERT_EQ(str::decode_base64_inplace(s = "YWJj"), "abc");
        ASSERT_EQ(str::decode_base64_inplace(s = "YWJjZGVm"), "abcdef");
    }
}
