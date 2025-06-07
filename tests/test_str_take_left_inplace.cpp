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

TEST(test_str, take_left_inplace) {
    std::string s;
    SECTION("一般场景") {
        ASSERT_EQ(str::take_left_inplace(s = "3bc1233", 0), "");
        ASSERT_EQ(str::take_left_inplace(s = "3bc1233", 3), "3bc");
        ASSERT_EQ(str::take_left_inplace(s = "3bc1233", 7), "3bc1233");
        ASSERT_EQ(str::take_left_inplace(s = "3bc1233", 8), "3bc1233");
        ASSERT_EQ(str::take_left_inplace(s = "3bc1233", str::npos), "3bc1233");
    }
    SECTION("空串") {
        ASSERT_EQ(str::take_left_inplace(s = "", 0), "");
        ASSERT_EQ(str::take_left_inplace(s = "", 3), "");
        ASSERT_EQ(str::take_left_inplace(s = "", str::npos), "");
    }
}
