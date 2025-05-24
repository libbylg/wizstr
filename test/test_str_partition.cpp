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

TEST(test_str, partition) {
    SECTION("") {
        ASSERT_EQ(str::partition("aaa::bbb", "::"), (std::array{"aaa", "::", "bbb"}));
        ASSERT_EQ(str::partition("::bbb", "::"), (std::array{"", "::", "bbb"}));
        ASSERT_EQ(str::partition("aaa::", "::"), (std::array{"aaa", "::", ""}));
        ASSERT_EQ(str::partition("::", "::"), (std::array{"", "::", ""}));
        ASSERT_EQ(str::partition("abc", "::"), (std::array{"abc", "", ""}));
        ASSERT_EQ(str::partition("", "::"), (std::array{"", "", ""}));
        ASSERT_EQ(str::partition("aaa::bbb", ""), (std::array{"aaa::bbb", "", ""}));
        ASSERT_EQ(str::partition("", ""), (std::array{"", "", ""}));
    }
}
