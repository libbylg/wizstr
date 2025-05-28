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

TEST(test_str, dump_hex) {
    SECTION("缺省形式") {
        std::string data{"Hello world!Hello world!"};
        std::vector expected{
            "48656c6c 6f20776f 726c6421 48656c6c ",
            "6f20776f 726c6421                   ",
        };
        std::vector<std::string> lines;
        str::dump_hex(data.c_str(), data.size(), str::dump_hex_format{}, //
            [&lines](size_t, std::string_view line) -> int {
                lines.emplace_back(line);
                return 0;
            });
        ASSERT_EQ(lines, expected);
    }

    SECTION("全量形式1") {
        std::string data{"Hello world!Hello world!Hello world!!"};
        std::vector expected{
            "0000: 48656C6C 6F20776F 726C6421 48656C6C : Hello world!Hell",
            "0010: 6F20776F 726C6421 48656C6C 6F20776F : o world!Hello wo",
            "0020: 726C6421 21                         : rld!!           ",
        };
        std::vector<std::string> lines;
        str::dump_hex_format format{
            .flags = str::show_ascii | str::show_offset | str::show_upper,
            .offset_width = 4,
            .line_groups = 4,
            .group_bytes = 4,
            .ascii_mask = '.',
            .offset_margin = ": ",
            .ascii_margin = ": ",
        };
        str::dump_hex(data.c_str(), data.size(), format, //
            [&lines](size_t, std::string_view line) -> int {
                lines.emplace_back(line);
                return 0;
            });
        ASSERT_EQ(lines, expected);
    }

    SECTION("全量形式2") {
        std::string data{"Hello world!Hello world!Hello world!!"};
        std::vector expected{
            "0000: 48 65 6C 6C 6F 20 77 6F 72 6C 64 21 48 65 6C 6C : Hello world!Hell",
            "0010: 6F 20 77 6F 72 6C 64 21 48 65 6C 6C 6F 20 77 6F : o world!Hello wo",
            "0020: 72 6C 64 21 21                                  : rld!!           ",
        };
        std::vector<std::string> lines;
        str::dump_hex_format format{
            .flags = str::show_ascii | str::show_offset | str::show_upper,
            .offset_width = 4,
            .line_groups = 16,
            .group_bytes = 1,
            .ascii_mask = '.',
            .offset_margin = ": ",
            .ascii_margin = ": ",
        };
        str::dump_hex(data.c_str(), data.size(), format, //
            [&lines](size_t, std::string_view line) -> int {
                lines.emplace_back(line);
                return 0;
            });
        for (auto& line : lines) {
            printf("[%s]\n", line.c_str());
        }
        ASSERT_EQ(lines, expected);
    }
}
