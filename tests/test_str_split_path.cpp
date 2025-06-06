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

#include "test-utils.hpp"

TEST(test_str, split_path) {
    SECTION("绝对路径") {
        ASSERT_EQ(str::split_path("/"), (std::vector<std::string_view>{"/"}));
        ASSERT_EQ(str::split_path("///"), (std::vector<std::string_view>{"/"}));
        ASSERT_EQ(str::split_path("//abc/def"), (std::vector<std::string_view>{"/", "abc", "def"}));
        ASSERT_EQ(str::split_path("//abc/def/"), (std::vector<std::string_view>{"/", "abc", "def"}));
    }
    SECTION("空串路径") {
        ASSERT_EQ(str::split_path(""), (std::vector<std::string_view>{}));
    }
    SECTION("相对路径") {
        ASSERT_EQ(str::split_path("abc/mpq/def.txt"), (std::vector<std::string_view>{"abc", "mpq", "def.txt"}));
        ASSERT_EQ(str::split_path("abc/mpq/def.txt/"), (std::vector<std::string_view>{"abc", "mpq", "def.txt"}));
        ASSERT_EQ(str::split_path("abc//mpq//def.txt//"), (std::vector<std::string_view>{"abc", "mpq", "def.txt"}));
    }
    SECTION("proc:1") {
        std::vector<std::string_view> result;
        str::split_path("abc/def/ghi/mpq//def.txt//", [&result](std::string_view item) -> int {
            result.emplace_back(item);
            if (result.size() >= 3) {
                return -1;
            }

            return 0;
        });
        ASSERT_EQ(result, (std::vector<std::string_view>{"abc", "def", "ghi"}));
    }
    SECTION("proc:2") {
        std::vector<std::string_view> result;
        str::split_path("/abc/def/def.txt", [&result](std::string_view item) -> int {
            result.emplace_back(item);
            if (result.size() >= 1) {
                return -1;
            }

            return 0;
        });
        ASSERT_EQ(result, (std::vector<std::string_view>{"/"}));
    }
}