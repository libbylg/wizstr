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

TEST(test_str, next_opt2) {
    SECTION("场景：-key value") {
        str::size_type pos = 0;
        auto argv = std::vector{"-a", "va"};
        ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-a", "va"}));
        ASSERT_EQ(pos, 2);
        ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"", ""}));
        ASSERT_EQ(pos, 2);
        ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"", ""}));
        ASSERT_EQ(pos, 2);
    }
    SECTION("场景：-key -- key2") {
        int pos = 0;
        const char* argv[5] = {"-a", "--", "-va"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", "-va"}));
        ASSERT_EQ(pos, 3);
        ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", ""}));
        ASSERT_EQ(pos, 3);
    }
}
