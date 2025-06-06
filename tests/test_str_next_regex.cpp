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

TEST(test_str, next_regex) {
    SECTION("不同起始位置:从0开始连续查找") {
        str::size_type pos = 0;
        ASSERT_EQ(str::next_regex("abc1234def", pos, "[0-9]+"), std::string("1234"));
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(str::next_regex("abc1234def", pos, "[0-9]+"), "");
        ASSERT_EQ(pos, 10);
    }
    SECTION("不同起始位置:从指定位置开始连续查找") {
        size_t pos = 3;
        ASSERT_EQ(str::next_regex("abc1234def", pos, "[0-9]+"), std::string("1234"));
        ASSERT_EQ(pos, 7);
        ASSERT_EQ(str::next_regex("abc1234def", pos, "[0-9]+"), "");
        ASSERT_EQ(pos, 10);
    }
    SECTION("不同起始位置:能找到多个") {
        size_t pos = 3;
        ASSERT_EQ(str::next_regex("1234def789", pos, "[0-9]+"), std::string("4"));
        ASSERT_EQ(pos, 4);
        ASSERT_EQ(str::next_regex("1234def789", pos, "[0-9]+"), std::string("789"));
        ASSERT_EQ(pos, 10);
        ASSERT_EQ(str::next_regex("1234def789", pos, "[0-9]+"), "");
        ASSERT_EQ(pos, 10);
    }
    SECTION("空串") {
        str::size_type pos = 0;
        ASSERT_EQ(str::next_regex("", pos, "[0-9]+"), "");
        ASSERT_EQ(pos, 0);
        ASSERT_EQ(str::next_regex("", pos, "[0-9]+"), "");
        ASSERT_EQ(pos, 0);
    }
    SECTION("字符串中没有任何匹配子串") {
        str::size_type pos = 0;
        ASSERT_EQ(str::next_regex("abc----def", pos, "[0-9]+"), "");
        ASSERT_EQ(pos, 10);
        ASSERT_EQ(str::next_regex("abc----def", pos, "[0-9]+"), "");
        ASSERT_EQ(pos, 10);
    }
    SECTION("字符串中没有任何匹配子串:指定位置超出范围") {
        size_t pos = 10;
        ASSERT_EQ(str::next_regex("abc----def", pos, "[0-9]+"), "");
        ASSERT_EQ(pos, 10);
        ASSERT_EQ(str::next_regex("abc----def", pos, "[0-9]+"), "");
        ASSERT_EQ(pos, 10);
    }
    SECTION("字符串中没有任何匹配子串:指定位置超出范围-npos") {
        size_t pos = str::npos;
        ASSERT_EQ(str::next_regex("abc----def", pos, "[0-9]+"), "");
        ASSERT_EQ(pos, 10);
        ASSERT_EQ(str::next_regex("abc----def", pos, "[0-9]+"), "");
        ASSERT_EQ(pos, 10);
    }
}