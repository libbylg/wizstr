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

TEST(test_str, encode_base64) {
    SECTION("一般情况") {
        ASSERT_EQ(str::encode_base64(""), "");
        ASSERT_EQ(str::encode_base64("a"), "YQ==");
        ASSERT_EQ(str::encode_base64("ab"), "YWI=");
        ASSERT_EQ(str::encode_base64("abc"), "YWJj");
        ASSERT_EQ(str::encode_base64("abcdef"), "YWJjZGVm");
    }
    GROUP("proc") {
        SECTION("proc:一般情况") {
            std::string s;
            str::encode_base64("abcdef", [&s](std::string_view item) -> int {
                s.append(item);
                return 0;
            });
            ASSERT_EQ(s, "YWJjZGVm");
        }
        SECTION("proc:提起终止") {
            std::string s;
            str::encode_base64("abcdef", [&s](std::string_view item) -> int {
                s.append(item);
                if (s.size() >= 3) {
                    return -1;
                }
                return 0;
            });
            ASSERT_EQ(s, "YWJj");
        }
    }
}
