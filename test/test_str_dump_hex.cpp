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

TEST(test_str, dump_hex) {
    SECTION("缺省形式") {
        std::string data{"Hello world!Hello world!"};
        std::vector expected{
            "48656c6c 6f20776f 726c6421 48656c6c",
            "6f20776f 726c6421                  ",
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
            "0000: 48656C6C 6F20776F 726C6421 48656C6C: Hello world!Hell",
            "0010: 6F20776F 726C6421 48656C6C 6F20776F: o world!Hello wo",
            "0020: 726C6421 21                        : rld!!           ",
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
            "0000: 48 65 6C 6C 6F 20 77 6F 72 6C 64 21 48 65 6C 6C: Hello world!Hell",
            "0010: 6F 20 77 6F 72 6C 64 21 48 65 6C 6C 6F 20 77 6F: o world!Hello wo",
            "0020: 72 6C 64 21 21                                 : rld!!           ",
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
        ASSERT_EQ(lines, expected);
    }

    SECTION("全量形式:小写形式") {
        std::string data{"Hello world!Hello world!Hello world!!"};
        std::vector expected{
            "0000: 48 65 6c 6c 6f 20 77 6f 72 6c 64 21 48 65 6c 6c: Hello world!Hell",
            "0010: 6f 20 77 6f 72 6c 64 21 48 65 6c 6c 6f 20 77 6f: o world!Hello wo",
            "0020: 72 6c 64 21 21                                 : rld!!           ",
        };
        std::vector<std::string> lines;
        str::dump_hex_format format{
            .flags = str::show_ascii | str::show_offset,
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
        ASSERT_EQ(lines, expected);
    }

    SECTION("全量形式:非可视字符") {
        std::string data{"\xe4\xb8\xad\xe5\x8d\x8e\xe4\xba\xba\xe6\xb0\x91\xe5\x85\xb1\xe5\x92\x8c\xe5\x9b\xbd"}; // "中华人民共和国"
        std::vector expected{
            "0000: |e4b8 ade5 8d8e e4ba| ........",
            "0008: |bae6 b091 e585 b1e5| ........",
            "0010: |928c e59b bd       | .....   ",
        };
        std::vector<std::string> lines;
        str::dump_hex_format format{
            .flags = str::show_ascii | str::show_offset,
            .offset_width = 4,
            .line_groups = 4,
            .group_bytes = 2,
            .ascii_mask = '.',
            .offset_margin = ": |",
            .ascii_margin = "| ",
        };
        str::dump_hex(data.c_str(), data.size(), format, //
            [&lines](size_t, std::string_view line) -> int {
                lines.emplace_back(line);
                return 0;
            });
        ASSERT_EQ(lines, expected);
    }

    SECTION("全量形式:offset=0(auto)") {
        std::string data{"\xe4\xb8\xad\xe5\x8d\x8e\xe4\xba\xba\xe6\xb0\x91\xe5\x85\xb1\xe5\x92\x8c\xe5\x9b\xbd"}; // "中华人民共和国"
        std::vector expected{
            "00: |e4b8 ade5| ....",
            "04: |8d8e e4ba| ....",
            "08: |bae6 b091| ....",
            "0c: |e585 b1e5| ....",
            "10: |928c e59b| ....",
            "14: |bd       | .   ",
        };
        std::vector<std::string> lines;
        str::dump_hex_format format{
            .flags = str::show_ascii | str::show_offset,
            .offset_width = 0,
            .line_groups = 2,
            .group_bytes = 2,
            .ascii_mask = '.',
            .offset_margin = ": |",
            .ascii_margin = "| ",
        };
        str::dump_hex(data.c_str(), data.size(), format, //
            [&lines](size_t, std::string_view line) -> int {
                lines.emplace_back(line);
                return 0;
            });
        // for (auto& line : lines) {
        //     printf("[%s]\n", line.c_str());
        // }
        // fflush(stdout);
        ASSERT_EQ(lines, expected);
    }

    SECTION("全量形式:data==nullptr") {
        std::string data{"abc"}; // "中华人民共和国"
        std::vector<std::string> expected{};
        std::vector<std::string> lines;
        str::dump_hex_format format{};
        str::dump_hex(nullptr, data.size(), format, //
            [&lines](size_t, std::string_view line) -> int {
                lines.emplace_back(line);
                return 0;
            });
        ASSERT_EQ(lines, expected);
    }

    SECTION("全量形式:len==0") {
        std::string data{"abc"}; // "中华人民共和国"
        std::vector<std::string> expected{};
        std::vector<std::string> lines;
        str::dump_hex_format format{};
        str::dump_hex(data.data(), 0, format, //
            [&lines](size_t, std::string_view line) -> int {
                lines.emplace_back(line);
                return 0;
            });
        ASSERT_EQ(lines, expected);
    }

    SECTION("全量形式:proc==nullptr") {
        std::string data{"abc"}; // "中华人民共和国"
        std::vector<std::string> expected{};
        std::vector<std::string> lines;
        str::dump_hex_format format{};
        str::dump_hex(data.data(), 0, format, //
            [&lines](size_t, std::string_view line) -> int {
                lines.emplace_back(line);
                return 0;
            });
        ASSERT_EQ(lines, expected);
    }

    SECTION("全量形式:proc提前返回") {
        std::string data{"Hello world!Hello world!Hello world!!"};
        std::vector expected{
            "0000: 48 65 6c 6c 6f 20 77 6f 72 6c 64 21 48 65 6c 6c: Hello world!Hell",
        };
        std::vector<std::string> lines;
        str::dump_hex_format format{
            .flags = str::show_ascii | str::show_offset,
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
                if (lines.size() >= 1) {
                    return -1;
                }
                return 0;
            });
        ASSERT_EQ(lines, expected);
    }
}
