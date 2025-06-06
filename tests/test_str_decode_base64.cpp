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

TEST(test_str, decode_base64) {
    SECTION("一般情况") {
        ASSERT_EQ(str::decode_base64(""), "");
        ASSERT_EQ(str::decode_base64("YQ=="), "a");
        ASSERT_EQ(str::decode_base64("YWI="), "ab");
        ASSERT_EQ(str::decode_base64("YWJj"), "abc");
        ASSERT_EQ(str::decode_base64("YWJjZGVm"), "abcdef");
        ASSERT_EQ(str::decode_base64("VGhpcyBpcyBiYXNlNjQgZm9ybWF0IQ=="), "This is base64 format!");
    }
    GROUP("proc") {
        SECTION("proc:一般情况") {
            std::string s;
            str::decode_base64("YWJjZGVm", [&s](std::string_view item) -> int {
                s.append(item);
                return 0;
            });
            ASSERT_EQ(s, "abcdef");
        }
        SECTION("proc:提起终止") {
            std::string s;
            str::decode_base64("YWJj", [&s](std::string_view item) -> int {
                s.append(item);
                if (s.size() >= 3) {
                    return -1;
                }
                return 0;
            });
            ASSERT_EQ(s, "abc");
        }
    }
}
