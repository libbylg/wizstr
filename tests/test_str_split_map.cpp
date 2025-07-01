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

#include "test-utils.hpp"

#include <map>

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, split_map) {
    SECTION("一般情况") {
        auto result = str::split_map("a:1,b:2,c:3");
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}, {"c", "3"}};
        ASSERT_EQ(result, expect);
    }
    SECTION("指定数量较小") {
        auto result = str::split_map("a:1,b:2,c:3,d:4,e:5", ",", ":", 3);
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}, {"c", "3"}};
        ASSERT_EQ(result, expect);
    }
    SECTION("指定数量较大1") {
        auto result = str::split_map("a:1,b:2,c:3,d:4,e:5", ",", ":", 5);
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}, {"c", "3"}, {"d", "4"}, {"e", "5"}};
        ASSERT_EQ(result, expect);
    }
    SECTION("指定数量较大2") {
        auto result = str::split_map("a:1,b:2,c:3,d:4,e:5", ",", ":", 6);
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}, {"c", "3"}, {"d", "4"}, {"e", "5"}};
        ASSERT_EQ(result, expect);
    }
    SECTION("指定数量较大3") {
        auto result = str::split_map("a:1,b:2,c:3,d:4,e:5", ",", ":", str::npos);
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}, {"c", "3"}, {"d", "4"}, {"e", "5"}};
        ASSERT_EQ(result, expect);
    }
    SECTION("指定数量为0") {
        auto result = str::split_map("a:1,b:2,c:3,d:4,e:5", ",", ":", 0);
        auto expect = std::map<std::string, std::string>{};
        ASSERT_EQ(result, expect);
    }
    SECTION("空串") {
        auto result = str::split_map("");
        auto expect = std::map<std::string, std::string>{};
        ASSERT_EQ(result, expect);
    }
    SECTION("key为空") {
        auto result = str::split_map("a:1,:2,c:3");
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"", "2"}, {"c", "3"}};
        ASSERT_EQ(result, expect);
    }
    SECTION("val为空") {
        auto result = str::split_map("a:1,b:2,c:");
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}, {"c", ""}};
        ASSERT_EQ(result, expect);
    }
    SECTION("key,val均为空") {
        auto result = str::split_map(",,c:");
        auto expect = std::map<std::string, std::string>{{"", ""}, {"", ""}, {"c", ""}};
        ASSERT_EQ(result, expect);
    }
    SECTION("无分隔符") {
        auto result = str::split_map("aa,bb,c:");
        auto expect = std::map<std::string, std::string>{{"aa", ""}, {"bb", ""}, {"c", ""}};
        ASSERT_EQ(result, expect);
    }
    SECTION("分隔空串") {
        auto result = str::split_map(",,,");
        auto expect = std::map<std::string, std::string>{{"", ""}, {"", ""}, {"", ""}};
        ASSERT_EQ(result, expect);
    }
    SECTION("指定分隔为空") {
        auto result = str::split_map("a:1,:2,c:3", "", "");
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"", "2"}, {"c", "3"}};
        ASSERT_EQ(result, expect);
    }
}