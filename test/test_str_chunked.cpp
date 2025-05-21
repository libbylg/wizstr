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
#include "test-compares.hpp"

TEST(test_str, chunked) {
    GROUP("返回容器") {
        ASSERT_EQ(str::chunked("abcdefghi1234567", 0), (std::vector{"abcdefghi1234567"}));
        ASSERT_EQ(str::chunked("abcdefghi1234567", 3), (std::vector{"abc", "def", "ghi", "123", "456", "7"}));
        ASSERT_EQ(str::chunked("abcdefghi1234567", 5), (std::vector{"abcde", "fghi1", "23456", "7"}));
        ASSERT_EQ(str::chunked("abcdefghi1234567", 16), (std::vector{"abcdefghi1234567"}));
        ASSERT_EQ(str::chunked("abcdefghi1234567", 17), (std::vector{"abcdefghi1234567"}));
        ASSERT_EQ(str::chunked("abcdefghi1234567", str::npos), (std::vector{"abcdefghi1234567"}));
    }
    GROUP("proc") {
        std::vector<std::string> result;
        str::chunked("abcdefghi1234567", 3, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return -1;
        });
        ASSERT_EQ(result, (std::vector<std::string>{"abc"}));
    }
}