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
    SECTION("mask") {
        ASSERT_EQ(str::encode_cstr(R"()"), R"()");
        ASSERT_EQ(str::encode_cstr("\a\b\r\n\xf3"), R"(\a\b\r\n\xf3)");
        ASSERT_EQ(str::encode_cstr(R"(abc\"'?)"), R"(abc\\\"\'?)");
    }
}