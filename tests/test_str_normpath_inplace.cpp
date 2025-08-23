/**
 * Copyright (c) 2021-2025  libbylg@126.com
 * wizstr is licensed under Mulan PSL v2.
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

#include <list>

#if defined(WIZSTR_NAMESPACE)
using str = WIZSTR_NAMESPACE::str;
#endif

TEST(test_str, normpath_inplace) {
    SECTION("一般场景") {
        std::string s;
        ASSERT_EQ(str::normpath_inplace(s = "a/b/c/d"), "a/b/c/d");
    }
    SECTION("当前目录") {
        std::string s;
        ASSERT_EQ(str::normpath_inplace(s = "."), ".");
        ASSERT_EQ(str::normpath_inplace(s = "././."), ".");
        ASSERT_EQ(str::normpath_inplace(s = "./b/c/d"), "b/c/d");
        ASSERT_EQ(str::normpath_inplace(s = "b/././c/./d/."), "b/c/d");
    }
    SECTION("多分隔符处理") {
        std::string s;
        ASSERT_EQ(str::normpath_inplace(s = "/"), "/");
        ASSERT_EQ(str::normpath_inplace(s = "//"), "/");
        ASSERT_EQ(str::normpath_inplace(s = "////"), "/");
        ASSERT_EQ(str::normpath_inplace(s = "////b/c/d"), "/b/c/d");
        ASSERT_EQ(str::normpath_inplace(s = "////b////c//d//"), "/b/c/d");
    }
    SECTION("父目录处理") {
        std::string s;
        ASSERT_EQ(str::normpath_inplace(s = "a/b/../../../m/../n"), "../n");
        ASSERT_EQ(str::normpath_inplace(s = "/../"), "/");
        ASSERT_EQ(str::normpath_inplace(s = "/a/../../"), "/");
        ASSERT_EQ(str::normpath_inplace(s = "/a/../../../"), "/");
        ASSERT_EQ(str::normpath_inplace(s = "a/b/c/d/.."), "a/b/c");
        ASSERT_EQ(str::normpath_inplace(s = "a/b/c/d/../m/n"), "a/b/c/m/n");
        ASSERT_EQ(str::normpath_inplace(s = "a/b/../../"), ".");
        ASSERT_EQ(str::normpath_inplace(s = "a/b/../../"), ".");
    }
    SECTION("空串") {
        std::string s;
        ASSERT_EQ(str::normpath_inplace(s = ""), ".");
    }
}