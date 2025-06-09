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

TEST(test_str, all_from_customers) {
    std::string_view s = "..ab;c.d., ff.yw..:d;fg..d.";
    SECTION("按单分隔符拆分") {
        auto result = str::split(s, str::charset("."));
        ASSERT_EQ(result, (std::vector{"", "", "ab;c", "d", ", ff", "yw", "", ":d;fg", "", "d", ""}));
    }
    SECTION("按多分隔符拆分") {
        auto result = str::split(s, str::charset(".,:;"));
        ASSERT_EQ(result, (std::vector{"", "", "ab", "c", "d", "", " ff", "yw", "", "", "d", "fg", "", "d", ""}));
    }
    SECTION("按多分隔符拆分+指定拆分次数上限") {
        auto result = str::split(s, str::charset(".,:;"), 5);
        ASSERT_EQ(result, (std::vector{"", "", "ab", "c", "d", ", ff.yw..:d;fg..d."}));
    }
    SECTION("按多分隔符拆分+丢弃空串") {
        std::vector<std::string> result;
        str::split(s, str::charset(".,:;"), [&result](std::string_view item) {
            if (!item.empty()) {
                result.emplace_back(item);
            }

            return 0;
        });
        ASSERT_EQ(result, (std::vector{"ab", "c", "d", " ff", "yw", "d", "fg", "d"}));
    }
}

