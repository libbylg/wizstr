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

TEST(test_str, remove_eol_suffix) {
    SECTION("一般情况:LF") {
        ASSERT_EQ(str::remove_eol_suffix("abc\n"), "abc");
        ASSERT_EQ(str::remove_eol_suffix("\n"), "");
    }
    SECTION("一般情况:CR") {
        ASSERT_EQ(str::remove_eol_suffix("abc\r"), "abc");
        ASSERT_EQ(str::remove_eol_suffix("\r"), "");
    }
    SECTION("一般情况:CR_LF") {
        ASSERT_EQ(str::remove_eol_suffix("abc\r\n"), "abc");
        ASSERT_EQ(str::remove_eol_suffix("\r\n"), "");
    }
    SECTION("一般情况:无尾部换行") {
        ASSERT_EQ(str::remove_eol_suffix("abc"), "abc");
    }
    SECTION("一般情况:空串") {
        ASSERT_EQ(str::remove_eol_suffix(""), "");
    }
    SECTION("一般情况:\\0") {
        ASSERT_EQ(str::remove_eol_suffix(std::string_view{"\r\0", 2}), (std::string_view{"\r\0", 2}));
    }
    SECTION("尾部多个换行") {
        ASSERT_EQ(str::remove_eol_suffix("\r\r\r\n"), "\r\r");
        ASSERT_EQ(str::remove_eol_suffix("\r\r\n\n"), "\r\r\n");
        ASSERT_EQ(str::remove_eol_suffix("\n\n\r"), "\n\n");
    }
}