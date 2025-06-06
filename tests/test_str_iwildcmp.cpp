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

TEST(test_str, iwildcmp_cstr) {
    SECTION("检查是否匹配通配符") {
        ASSERT_TRUE(str::iwildcmp("3bc12def33", "3*"));
        ASSERT_TRUE(str::iwildcmp("3abc", "3abc"));
        ASSERT_TRUE(str::iwildcmp("3", "?"));
        ASSERT_TRUE(str::iwildcmp("3abc", "????"));
        ASSERT_FALSE(str::iwildcmp("3abc", ""));
        ASSERT_TRUE(str::iwildcmp("", ""));
        ASSERT_TRUE(str::iwildcmp("3b33KK33", "*33"));
        ASSERT_TRUE(str::iwildcmp("3bc12def33", "*3?"));
        ASSERT_TRUE(str::iwildcmp("3bc12def33", "*?3"));
        ASSERT_TRUE(str::iwildcmp("3bc12def33", "3bc12def33"));
        ASSERT_TRUE(str::iwildcmp("3bc12def33", "3*de?33"));
        ASSERT_TRUE(str::iwildcmp("3bc12def33", "*"));
        ASSERT_TRUE(str::iwildcmp("", "*"));
        ASSERT_FALSE(str::iwildcmp("abc", "3bc12def33"));
        ASSERT_TRUE(str::iwildcmp("3bc12def33", "*3**3***3"));
        ASSERT_TRUE(str::iwildcmp("3bc12def33", "*3**3***3***"));
        ASSERT_TRUE(str::iwildcmp("abc", "abc***"));
        ASSERT_FALSE(str::iwildcmp("abc", "abc***d"));
    }
    SECTION("关于大小写的匹配") {
        ASSERT_TRUE(str::iwildcmp("345", "345"));
        ASSERT_TRUE(str::iwildcmp("345abc", "345abc"));
        ASSERT_TRUE(str::iwildcmp("ABC", "abc"));
        ASSERT_TRUE(str::iwildcmp("abc", "abc"));
        ASSERT_TRUE(str::iwildcmp("abc", "A??"));
        ASSERT_TRUE(str::iwildcmp("ABC", "A??"));
        ASSERT_TRUE(str::iwildcmp("ABC", "A*"));
        ASSERT_TRUE(str::iwildcmp("ABC", "a*"));
    }
}

TEST(test_str, iwildcmp_string_view) {
    SECTION("检查是否匹配通配符") {
        ASSERT_TRUE(str::iwildcmp(std::string_view{"3bc12def33"}, std::string_view{"3*"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"3abc"}, std::string_view{"3abc"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"3"}, std::string_view{"?"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"3abc"}, std::string_view{"????"}));
        ASSERT_FALSE(str::iwildcmp(std::string_view{"3abc"}, std::string_view{""}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{""}, std::string_view{""}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"3bc12def33"}, std::string_view{"*3"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"3bc12def33"}, std::string_view{"*3?"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"3bc12def33"}, std::string_view{"*?3"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"3bc12def33"}, std::string_view{"3bc12def33"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"3bc12def33"}, std::string_view{"3*de?33"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"3bc12def33"}, std::string_view{"*"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{""}, std::string_view{"*"}));
        ASSERT_FALSE(str::iwildcmp(std::string_view{"abc"}, std::string_view{"3bc12def33"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"3bc12def33"}, std::string_view{"*3**3***3"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"3bc12def33"}, std::string_view{"*3**3***3***"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"abc"}, std::string_view{"abc***"}));
        ASSERT_FALSE(str::iwildcmp(std::string_view{"abc"}, std::string_view{"abc***d"}));
    }
    SECTION("关于大小写的匹配") {
        ASSERT_TRUE(str::iwildcmp(std::string_view{"345"}, std::string_view{"345"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"345abc"}, std::string_view{"345abc"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"ABC"}, std::string_view{"abc"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"abc"}, std::string_view{"abc"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"abc"}, std::string_view{"A??"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"ABC"}, std::string_view{"A??"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"ABC"}, std::string_view{"A*"}));
        ASSERT_TRUE(str::iwildcmp(std::string_view{"ABC"}, std::string_view{"a*"}));
    }
}