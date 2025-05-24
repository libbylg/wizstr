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

TEST(test_str, partition_range) {
    SECTION("") {
        ASSERT_EQ(str::partition_range("aaa::bbb", "::"), (std::array{str::range(0, 3), str::range(3, 2), str::range(5, 3)}));
        ASSERT_EQ(str::partition_range("::bbb", "::"), (std::array{str::range(0, 0), str::range(0, 2), str::range(2, 3)}));
        ASSERT_EQ(str::partition_range("aaa::", "::"), (std::array{str::range(0, 3), str::range(3, 2), str::range(5, 0)}));
        ASSERT_EQ(str::partition_range("::", "::"), (std::array{str::range(0, 0), str::range(0, 2), str::range(2, 0)}));
        ASSERT_EQ(str::partition_range("abc", "::"), (std::array{str::range(0, 3), str::range(3, 0), str::range(3, 0)}));
        ASSERT_EQ(str::partition_range("", "::"), (std::array{str::range(0, 0), str::range(0, 0), str::range(0, 0)}));
        ASSERT_EQ(str::partition_range("aaa::bbb", ""), (std::array{str::range(0, 8), str::range(8, 0), str::range(8, 0)}));
        ASSERT_EQ(str::partition_range("", ""), (std::array{str::range(0, 0), str::range(0, 0), str::range(0, 0)}));
    }
}
