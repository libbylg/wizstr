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

TEST(test_str, windowed) {
    SECTION("max_n") {
        ASSERT_EQ(str::windowed("abc-def-ghi", 0, 3), (std::vector{"abc-def-ghi", "-def-ghi", "f-ghi", "hi"}));
        ASSERT_EQ(str::windowed("abc-def-ghi", 3, 3), (std::vector{"abc", "-de", "f-g", "hi"}));
        ASSERT_EQ(str::windowed("abc-def-ghi", 4, 3), (std::vector{"abc-", "-def", "f-gh", "hi"}));
        ASSERT_EQ(str::windowed("abc-def-ghi", 10, 3), (std::vector{"abc-def-gh", "-def-ghi", "f-ghi", "hi"}));
        ASSERT_EQ(str::windowed("abc-def-ghi", 11, 3), (std::vector{"abc-def-ghi", "-def-ghi", "f-ghi", "hi"}));
        ASSERT_EQ(str::windowed("abc-def-ghi", 12, 3), (std::vector{"abc-def-ghi", "-def-ghi", "f-ghi", "hi"}));
        ASSERT_EQ(str::windowed("abc-def-ghi", str::npos, 3), (std::vector{"abc-def-ghi", "-def-ghi", "f-ghi", "hi"}));
    }
    SECTION("step") {
        ASSERT_EQ(str::windowed("abc-def-ghi", 4, 0), (std::vector{"abc-", "bc-d", "c-de", "-def", "def-", "ef-g", "f-gh", "-ghi", "ghi", "hi", "i"}));
        ASSERT_EQ(str::windowed("abc-def-ghi", 4, 1), (std::vector{"abc-", "bc-d", "c-de", "-def", "def-", "ef-g", "f-gh", "-ghi", "ghi", "hi", "i"}));
        ASSERT_EQ(str::windowed("abc-def-ghi", 4, 4), (std::vector{"abc-", "def-", "ghi"}));
        ASSERT_EQ(str::windowed("abc-def-ghi", 4, 5), (std::vector{"abc-", "ef-g", "i"}));
        ASSERT_EQ(str::windowed("abc-def-ghi", 4, 11), (std::vector{"abc-"}));
        ASSERT_EQ(str::windowed("abc-def-ghi", 4, 12), (std::vector{"abc-"}));
        ASSERT_EQ(str::windowed("abc-def-ghi", 4, str::npos), (std::vector{"abc-"}));
    }
    SECTION("空串") {
        ASSERT_EQ(str::windowed("", 0, 0), (std::vector<std::string>{}));
        ASSERT_EQ(str::windowed("", 1, 0), (std::vector<std::string>{}));
        ASSERT_EQ(str::windowed("", 0, 1), (std::vector<std::string>{}));
        ASSERT_EQ(str::windowed("", 1, 1), (std::vector<std::string>{}));
        ASSERT_EQ(str::windowed("", 4, 4), (std::vector<std::string>{}));
        ASSERT_EQ(str::windowed("", str::npos, 4), (std::vector<std::string>{}));
        ASSERT_EQ(str::windowed("", 4, str::npos), (std::vector<std::string>{}));
        ASSERT_EQ(str::windowed("", str::npos, str::npos), (std::vector<std::string>{}));
    }
    SECTION("proc") {
        std::vector<std::string_view> result;

        str::windowed("abc-def-ghi", 4, 3, to_consumer(result = {}));
        ASSERT_EQ(result, (std::vector{"abc-", "-def", "f-gh", "hi"}));

        result = {};
        str::windowed("abc-def-ghi", 4, 3, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return -1;
        });
        ASSERT_EQ(result, (std::vector{"abc-"}));
    }
}