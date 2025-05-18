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

TEST(test_str, remove_eol_suffix_range) {
    SECTION("一般情况:LF") {
        ASSERT_EQ(str::remove_eol_suffix_range("abc\n"), (str::range_type{0, 3}));
        ASSERT_EQ(str::remove_eol_suffix_range("\n"), (str::range_type{0, 0}));
    }
    SECTION("一般情况:CR") {
        ASSERT_EQ(str::remove_eol_suffix_range("abc\r"), (str::range_type{0, 3}));
        ASSERT_EQ(str::remove_eol_suffix_range("\r"), (str::range_type{0, 0}));
    }
    SECTION("一般情况:CR_LF") {
        ASSERT_EQ(str::remove_eol_suffix_range("abc\r\n"), (str::range_type{0, 3}));
        ASSERT_EQ(str::remove_eol_suffix_range("\r\n"), (str::range_type{0, 0}));
    }
    SECTION("一般情况:无尾部换行") {
        ASSERT_EQ(str::remove_eol_suffix_range("abc"), (str::range_type{0, 3}));
    }
    SECTION("一般情况:空串") {
        ASSERT_EQ(str::remove_eol_suffix_range(""), (str::range_type{0, 0}));
    }
    SECTION("一般情况:\\0") {
        ASSERT_EQ(str::remove_eol_suffix_range(std::string_view{"\r\0", 2}), (str::range_type{0, 2}));
    }
    SECTION("尾部多个换行") {
        ASSERT_EQ(str::remove_eol_suffix_range("\r\r\r\n"), (str::range_type{0, 2}));
        ASSERT_EQ(str::remove_eol_suffix_range("\r\r\n\n"), (str::range_type{0, 3}));
        ASSERT_EQ(str::remove_eol_suffix_range("\n\n\r"), (str::range_type{0, 2}));
    }
}