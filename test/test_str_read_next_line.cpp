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
#include "test-utils.hpp"

TEST(test_str, read_next_line) {
    SECTION("一般情况") {
        std::string filename{str::dirname(__FILE__) + "/data/test-general.txt"};
        FILE* file = fopen(filename.c_str(), "r");
        assert(file != nullptr);
        scope_guard file_guard([file] { fclose(file); });
        std::optional<std::string> line;
        ASSERT_TRUE((line = str::read_next_line(file, true)));
        ASSERT_EQ(line, "ABCDE12345\n");
        ASSERT_TRUE((line = str::read_next_line(file, true)));
        ASSERT_EQ(line, "ABCDE\n");
        ASSERT_TRUE((line = str::read_next_line(file, true)));
        ASSERT_EQ(line, "12345");
        ASSERT_FALSE((line = str::read_next_line(file, true)));
        ASSERT_FALSE((line = str::read_next_line(file, true)));
    }
}
