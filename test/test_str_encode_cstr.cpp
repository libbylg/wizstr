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

#include <array>

TEST(test_str, encode_cstr) {
    SECTION("一般情况") {
        ASSERT_EQ(str::encode_cstr(R"()"), R"()");
        ASSERT_EQ(str::encode_cstr("\a\b\r\n\xf3"), R"(\a\b\r\n\xf3)");
        ASSERT_EQ(str::encode_cstr(R"(abc\"'?)"), R"(abc\\\"\'?)");
        ASSERT_EQ(str::encode_cstr(std::string_view{"abc\0def", 7}), R"(abc\x00def)");
    }
    SECTION("proc形式:提前结束") {
        std::string s;
        str::encode_cstr(R"(abc\"'?)", [&s](std::string_view seg) -> int {
            s.append(seg);
            if (s.size() >= 2) {
                return -1;
            }

            return 0;
        });
        ASSERT_EQ(s, R"(ab)");
    }
    SECTION("proc形式:提前结束2") {
        std::string s;
        str::encode_cstr(R"(abc\"'?)", [&s](std::string_view seg) -> int {
            s.append(seg);
            if (s.size() >= 6) {
                return -1;
            }

            return 0;
        });
        ASSERT_EQ(s, R"(abc\\\")");
    }
}