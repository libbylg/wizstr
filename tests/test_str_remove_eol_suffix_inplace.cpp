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

TEST(test_str, remove_eol_suffix_inplace) {
    SECTION("一般情况:LF") {
        std::string s;
        ASSERT_EQ(str::remove_eol_suffix_inplace(s = "abc\n"), "abc");
        ASSERT_EQ(str::remove_eol_suffix_inplace(s = "\n"), "");
    }
    SECTION("一般情况:CR") {
        std::string s;
        ASSERT_EQ(str::remove_eol_suffix_inplace(s = "abc\r"), "abc");
        ASSERT_EQ(str::remove_eol_suffix_inplace(s = "\r"), "");
    }
    SECTION("一般情况:CR_LF") {
        std::string s;
        ASSERT_EQ(str::remove_eol_suffix_inplace(s = "abc\r\n"), "abc");
        ASSERT_EQ(str::remove_eol_suffix_inplace(s = "\r\n"), "");
    }
    SECTION("一般情况:无尾部换行") {
        std::string s;
        ASSERT_EQ(str::remove_eol_suffix_inplace(s = "abc"), "abc");
    }
    SECTION("一般情况:空串") {
        std::string s;
        ASSERT_EQ(str::remove_eol_suffix_inplace(s = ""), "");
    }
    SECTION("一般情况:\\0") {
        std::string s;
        ASSERT_EQ(str::remove_eol_suffix_inplace(s = std::string_view{"\r\0", 2}), (std::string_view{"\r\0", 2}));
    }
    SECTION("尾部多个换行") {
        std::string s;
        ASSERT_EQ(str::remove_eol_suffix_inplace(s = "\r\r\r\n"), "\r\r");
        ASSERT_EQ(str::remove_eol_suffix_inplace(s = "\r\r\n\n"), "\r\r\n");
        ASSERT_EQ(str::remove_eol_suffix_inplace(s = "\n\n\r"), "\n\n");
    }
}