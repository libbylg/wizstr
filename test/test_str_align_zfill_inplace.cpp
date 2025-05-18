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

TEST(test_str, align_zfill_inplace) {
    std::string s;
    SECTION("一般情况：字符串右对齐，左边填充") {
        ASSERT_EQ(str::align_zfill_inplace(s = "123", 8), "00000123");
        ASSERT_EQ(str::align_zfill_inplace(s = "+123", 8), "+0000123");
        ASSERT_EQ(str::align_zfill_inplace(s = "-123", 8), "-0000123");
    }

    SECTION("宽度和原字符串相同：字符串不变") {
        ASSERT_EQ(str::align_zfill_inplace(s = "123", 3), "123");
        ASSERT_EQ(str::align_zfill_inplace(s = "+123", 3), "+123");
        ASSERT_EQ(str::align_zfill_inplace(s = "-123", 3), "-123");

        ASSERT_EQ(str::align_zfill_inplace(s = "123", 4), "0123");
        ASSERT_EQ(str::align_zfill_inplace(s = "+123", 4), "+123");
        ASSERT_EQ(str::align_zfill_inplace(s = "-123", 4), "-123");

        ASSERT_EQ(str::align_zfill_inplace(s = "123", 5), "00123");
        ASSERT_EQ(str::align_zfill_inplace(s = "+123", 5), "+0123");
        ASSERT_EQ(str::align_zfill_inplace(s = "-123", 5), "-0123");
    }

    SECTION("宽度太窄时保持原样") {
        ASSERT_EQ(str::align_zfill_inplace(s = "1234567890", 9), "1234567890");
    }

    SECTION("空字符串") {
        ASSERT_EQ(str::align_zfill_inplace(s = "", 5), "00000");
    }

    SECTION("对齐宽度为0") {
        ASSERT_EQ(str::align_zfill_inplace(s = "123", 0), "123");
        ASSERT_EQ(str::align_zfill_inplace(s = "", 0), "");
    }

    SECTION("孤立正负号") {
        ASSERT_EQ(str::align_zfill_inplace(s = "+", 4), "+000");
        ASSERT_EQ(str::align_zfill_inplace(s = "-", 4), "-000");
    }
}
