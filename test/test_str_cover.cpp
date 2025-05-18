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

TEST(test_str, cover) {
    SECTION("mask") {
        ASSERT_EQ(str::cover("abcdefghijk"), "a***ijk");
        ASSERT_EQ(str::cover("abcdefghijk", "#", 1, 3), "a#ijk");
        ASSERT_EQ(str::cover("abcdefghijk", "??????", 1, 3), "a??????ijk");
        ASSERT_EQ(str::cover("abcdefghijk", "", 1, 3), "aijk");
    }
    SECTION("left&right") {
        ASSERT_EQ(str::cover("abcdefghijk"), "a***ijk");
        ASSERT_EQ(str::cover("abcdefghijk", "###", 0, 3), "###ijk");
        ASSERT_EQ(str::cover("abcdefghijk", "###", 1, 3), "a###ijk");
        ASSERT_EQ(str::cover("abcdefghijk", "###", 3, 3), "abc###ijk");

        ASSERT_EQ(str::cover("abcdefghijk", "###", 3, 0), "abc###");
        ASSERT_EQ(str::cover("abcdefghijk", "###", 3, 1), "abc###k");
        ASSERT_EQ(str::cover("abcdefghijk", "###", 3, 3), "abc###ijk");

        ASSERT_EQ(str::cover("abcdefghijk", "###", 5, 6), "abcde###fghijk");
        ASSERT_EQ(str::cover("abcdefghijk", "###", 11, 0), "abcdefghijk###");
        ASSERT_EQ(str::cover("abcdefghijk", "###", 0, 11), "###abcdefghijk");
        ASSERT_EQ(str::cover("abcdefghijk", "###", 0, 11), "###abcdefghijk");

        ASSERT_EQ(str::cover("abcdefghijk", "###", 11, 1), "");
        ASSERT_EQ(str::cover("abcdefghijk", "###", 1, 11), "");

        ASSERT_EQ(str::cover("abcdefghijk", "###", str::npos, 0), "");
        ASSERT_EQ(str::cover("abcdefghijk", "###", 0, str::npos), "");
        ASSERT_EQ(str::cover("abcdefghijk", "###", str::npos, str::npos), "");
    }
    SECTION("empty") {
        ASSERT_EQ(str::cover("", "###", 0, 0), "");
        ASSERT_EQ(str::cover("", "", 0, 0), "");
    }
}