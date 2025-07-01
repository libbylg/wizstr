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

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, split_words) {
    SECTION("一般情况") {
        ASSERT_EQ(str::split_words("Hello World patppy boy"), (std::vector<std::string>{"Hello", "World", "patppy", "boy"}));
        ASSERT_EQ(str::split_words("Hello\r World\v patppy \t \n boy"), (std::vector<std::string>{"Hello", "World", "patppy", "boy"}));
    }
    SECTION("无任何空白情况") {
        ASSERT_EQ(str::split_words("Hello-World-patppy-boy"), (std::vector<std::string>{"Hello-World-patppy-boy"}));
    }
    SECTION("首尾有空白") {
        ASSERT_EQ(str::split_words("Hello \rWorld\n "), (std::vector<std::string>{"Hello", "World"}));
        ASSERT_EQ(str::split_words(" \r\v\t\n Hello \rWorld\n "), (std::vector<std::string>{"Hello", "World"}));
        ASSERT_EQ(str::split_words(" \r\v\t\n Hello \rWorld"), (std::vector<std::string>{"Hello", "World"}));
    }
    SECTION("空串") {
        ASSERT_EQ(str::split_words(""), (std::vector<std::string>{}));
        ASSERT_EQ(str::split_words("", 0), (std::vector<std::string>{}));
        ASSERT_EQ(str::split_words("", 1), (std::vector<std::string>{}));
        ASSERT_EQ(str::split_words("", str::npos), (std::vector<std::string>{}));
    }
    SECTION("全空白串") {
        ASSERT_EQ(str::split_words(" \t\v\r\n "), (std::vector<std::string>{}));
        ASSERT_EQ(str::split_words(" \t\v\r\n ", 0), (std::vector<std::string>{}));
        ASSERT_EQ(str::split_words(" \t\v\r\n ", 1), (std::vector<std::string>{}));
        ASSERT_EQ(str::split_words(" \t\v\r\n ", str::npos), (std::vector<std::string>{}));
    }
    SECTION("限制数据量") {
        ASSERT_EQ(str::split_words("Hello World patppy boy", 0), (std::vector<std::string>{}));
        ASSERT_EQ(str::split_words("Hello World patppy boy", 1), (std::vector<std::string>{"Hello"}));
        ASSERT_EQ(str::split_words("Hello World patppy boy", 2), (std::vector<std::string>{"Hello", "World"}));
        ASSERT_EQ(str::split_words("Hello World patppy boy", 3), (std::vector<std::string>{"Hello", "World", "patppy"}));
        ASSERT_EQ(str::split_words("Hello World patppy boy", 4), (std::vector<std::string>{"Hello", "World", "patppy", "boy"}));
        ASSERT_EQ(str::split_words("Hello World patppy boy", 5), (std::vector<std::string>{"Hello", "World", "patppy", "boy"}));
        ASSERT_EQ(str::split_words("Hello World patppy boy", str::npos), (std::vector<std::string>{"Hello", "World", "patppy", "boy"}));
    }
}
