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

TEST(test_str, ends_with_eol) {
    SECTION("一般情况:LF") {
        ASSERT_TRUE(str::ends_with_eol("abc\n"));
        ASSERT_TRUE(str::ends_with_eol("\n"));
    }
    SECTION("一般情况:CR") {
        ASSERT_TRUE(str::ends_with_eol("abc\r"));
        ASSERT_TRUE(str::ends_with_eol("\r"));
    }
    SECTION("一般情况:CR_LF") {
        ASSERT_TRUE(str::ends_with_eol("abc\r\n"));
        ASSERT_TRUE(str::ends_with_eol("\r\n"));
    }
    SECTION("一般情况:无尾部换行") {
        ASSERT_FALSE(str::ends_with_eol("abc"));
    }
    SECTION("一般情况:空串") {
        ASSERT_FALSE(str::ends_with_eol(""));
    }
    SECTION("一般情况:\\0") {
        ASSERT_FALSE(str::ends_with_eol(std::string_view{"\r\0", 2}));
    }
}