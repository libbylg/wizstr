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

#include <fstream>

TEST(test_str, read_lines) {
    GROUP("FILE") {
        SECTION("FILE:invalid-file-ptr") {
            FILE* file = nullptr;

            std::vector<std::string> lines;

            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, std::vector<std::string_view>{});
        }
        SECTION("FILE:empty") {
            std::string filename{str::dirname(__FILE__) + "/data/test-empty.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });

            std::vector<std::string> lines;

            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, std::vector{""});
            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, std::vector<std::string_view>{});
        }
        SECTION("FILE:general") {
            std::string filename{str::dirname(__FILE__) + "/data/test-general.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });

            std::vector<std::string> lines;
            std::vector<std::string> expect{
                "ABCDE12345\n",
                "ABCDE\n",
                "12345",
            };

            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, expect);
            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, std::vector<std::string_view>{});
        }
        SECTION("FILE:emptylines") {
            std::string filename{str::dirname(__FILE__) + "/data/test-emptylines.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });

            std::vector<std::string> lines;

            lines = str::read_lines(file, false);
            ASSERT_EQ(lines, (std::vector{"", "", "", ""}));
            lines = str::read_lines(file, false);
            ASSERT_EQ(lines, (std::vector<std::string>{}));
        }
        SECTION("FILE:510") {
            std::string filename{str::dirname(__FILE__) + "/data/test-510.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 51).append("\n"),
                "",
            };

            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, std::vector<std::string>{});
        }
        SECTION("FILE:511") {
            std::string filename{str::dirname(__FILE__) + "/data/test-511.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 51).append("0\n"),
                "",
            };

            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, expected);

            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, std::vector<std::string_view>{});
        }
        SECTION("FILE:512") {
            std::string filename{str::dirname(__FILE__) + "/data/test-512.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 51).append("01\n"),
                "",
            };

            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, std::vector<std::string>{});
        }
        SECTION("FILE:513") {
            std::string filename{str::dirname(__FILE__) + "/data/test-513.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 51).append("012\n"),
                "",
            };

            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, std::vector<std::string>{});
        }
        SECTION("FILE:1024") {
            std::string filename{str::dirname(__FILE__) + "/data/test-1024.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 102).append("0123\n"),
                "",
            };

            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, std::vector<std::string>{});
        }
        SECTION("FILE:1024x3") {
            std::string filename{str::dirname(__FILE__) + "/data/test-1024x3.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 102).append("0123\n"),
                str::repeat("0123456789", 102).append("0123\n"),
                str::repeat("0123456789", 102).append("0123\n"),
                "",
            };

            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(file, true);
            ASSERT_EQ(lines, std::vector<std::string>{});
        }
        SECTION("FILE:maxn-limit:1024x3") {
            std::string filename{str::dirname(__FILE__) + "/data/test-1024x3.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 102).append("0123"),
                str::repeat("0123456789", 102).append("0123"),
            };

            lines = str::read_lines(file, false, 2);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(file, false, 2);
            ASSERT_EQ(lines, (std::vector{str::repeat("0123456789", 102).append("0123"), std::string{""}}));
            lines = str::read_lines(file, false, 2);
            ASSERT_EQ(lines, std::vector<std::string>{});
        }
        SECTION("FILE:maxn==0:1024x3") {
            std::string filename{str::dirname(__FILE__) + "/data/test-1024x3.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{};

            lines = str::read_lines(file, false, 0);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(file, false, 0);
            ASSERT_EQ(lines, expected);
        }
    }

    GROUP("iostream") {
        SECTION("iostream:invalid-fstream") {
            std::string filename{str::dirname(__FILE__) + "/data/test-NOT-EXIST.txt"};
            std::fstream file(filename, std::ios::in);
            file.close(); // 提前关闭文件

            std::vector<std::string> lines;

            lines = str::read_lines(file);
            ASSERT_EQ(lines, std::vector<std::string_view>{});
        }
        SECTION("iostream:empty") {
            std::string filename{str::dirname(__FILE__) + "/data/test-empty.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });

            std::vector<std::string> lines;

            lines = str::read_lines(file);
            ASSERT_EQ(lines, std::vector{""});
            lines = str::read_lines(file);
            ASSERT_EQ(lines, std::vector<std::string_view>{});
        }
        SECTION("iostream:general") {
            std::string filename{str::dirname(__FILE__) + "/data/test-general.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });

            std::vector<std::string> lines;
            std::vector<std::string> expect{
                "ABCDE12345",
                "ABCDE",
                "12345",
            };

            lines = str::read_lines(file);
            ASSERT_EQ(lines, expect);
            lines = str::read_lines(file);
            ASSERT_EQ(lines, std::vector<std::string_view>{});
        }
        SECTION("iostream:emptylines") {
            std::string filename{str::dirname(__FILE__) + "/data/test-emptylines.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });

            std::vector<std::string> lines;

            lines = str::read_lines(file);
            ASSERT_EQ(lines, (std::vector{"", "", "", ""}));
            lines = str::read_lines(file);
            ASSERT_EQ(lines, (std::vector<std::string>{}));
        }
        SECTION("iostream:510") {
            std::string filename{str::dirname(__FILE__) + "/data/test-510.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 51),
                "",
            };

            lines = str::read_lines(file);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(file);
            ASSERT_EQ(lines, std::vector<std::string>{});
        }
        SECTION("iostream:511") {
            std::string filename{str::dirname(__FILE__) + "/data/test-511.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 51).append("0"),
                "",
            };

            lines = str::read_lines(file);
            ASSERT_EQ(lines, expected);

            lines = str::read_lines(file);
            ASSERT_EQ(lines, std::vector<std::string_view>{});
        }
        SECTION("iostream:512") {
            std::string filename{str::dirname(__FILE__) + "/data/test-512.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 51).append("01"),
                "",
            };

            lines = str::read_lines(file);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(file);
            ASSERT_EQ(lines, std::vector<std::string>{});
        }
        SECTION("iostream:513") {
            std::string filename{str::dirname(__FILE__) + "/data/test-513.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 51).append("012"),
                "",
            };

            lines = str::read_lines(file);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(file);
            ASSERT_EQ(lines, std::vector<std::string>{});
        }
        SECTION("iostream:1024") {
            std::string filename{str::dirname(__FILE__) + "/data/test-1024.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 102).append("0123"),
                "",
            };

            lines = str::read_lines(file);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(file);
            ASSERT_EQ(lines, std::vector<std::string>{});
        }
        SECTION("iostream:1024x3") {

            std::string filename{str::dirname(__FILE__) + "/data/test-1024x3.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 102).append("0123"),
                str::repeat("0123456789", 102).append("0123"),
                str::repeat("0123456789", 102).append("0123"),
                "",
            };

            lines = str::read_lines(file);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(file);
            ASSERT_EQ(lines, std::vector<std::string>{});
        }
        SECTION("iostream:maxn-limit:1024x3") {
            std::string filename{str::dirname(__FILE__) + "/data/test-1024x3.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 102).append("0123"),
                str::repeat("0123456789", 102).append("0123"),
            };

            lines = str::read_lines(file, 2);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(file, 2);
            ASSERT_EQ(lines, (std::vector{str::repeat("0123456789", 102).append("0123"), std::string{""}}));
            lines = str::read_lines(file, 2);
            ASSERT_EQ(lines, std::vector<std::string>{});
        }
        SECTION("iostream:maxn==0:1024x3") {
            std::string filename{str::dirname(__FILE__) + "/data/test-1024x3.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });

            std::vector<std::string> lines;
            std::vector<std::string> expected{};

            lines = str::read_lines(file, 0);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(file, 0);
            ASSERT_EQ(lines, expected);
        }
    }

    GROUP("filename") {
        SECTION("invalid-filename") {
            std::string filename;

            std::vector<std::string> lines;

            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, std::vector<std::string_view>{});

            lines = str::read_lines(static_cast<const char*>(nullptr), true);
            ASSERT_EQ(lines, std::vector<std::string_view>{});

            lines.clear();
            str::read_lines(static_cast<const char*>(nullptr), true, [&lines](str::size_type, std::string_view line_text) -> int {
                lines.emplace_back(line_text);
                return 0;
            });
            ASSERT_EQ(lines, std::vector<std::string_view>{});
        }
        SECTION("empty") {
            std::string filename{str::dirname(__FILE__) + "/data/test-empty.txt"};

            std::vector<std::string> lines;

            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, std::vector{""});
            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, std::vector{""});
        }
        SECTION("general") {
            std::string filename{str::dirname(__FILE__) + "/data/test-general.txt"};

            std::vector<std::string> lines;
            std::vector<std::string> expect{
                "ABCDE12345\n",
                "ABCDE\n",
                "12345",
            };

            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, expect);

            lines.clear();
            str::read_lines(filename, true, [&lines](str::size_type, std::string_view line_text) -> int {
                lines.emplace_back(line_text);
                return 0;
            });
            ASSERT_EQ(lines, expect);
        }
        SECTION("emptylines") {
            std::string filename{str::dirname(__FILE__) + "/data/test-emptylines.txt"};

            std::vector<std::string> lines;

            lines = str::read_lines(filename, false);
            ASSERT_EQ(lines, (std::vector{"", "", "", ""}));
            lines = str::read_lines(filename, false);
            ASSERT_EQ(lines, (std::vector{"", "", "", ""}));
        }
        SECTION("510") {
            std::string filename{str::dirname(__FILE__) + "/data/test-510.txt"};

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 51).append("\n"),
                "",
            };

            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, expected);
        }
        SECTION("511") {
            std::string filename{str::dirname(__FILE__) + "/data/test-511.txt"};

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 51).append("0\n"),
                "",
            };

            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, expected);

            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, expected);
        }
        SECTION("512") {
            std::string filename{str::dirname(__FILE__) + "/data/test-512.txt"};

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 51).append("01\n"),
                "",
            };

            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, expected);
        }
        SECTION("513") {
            std::string filename{str::dirname(__FILE__) + "/data/test-513.txt"};

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 51).append("012\n"),
                "",
            };

            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, expected);
        }
        SECTION("1024") {
            std::string filename{str::dirname(__FILE__) + "/data/test-1024.txt"};

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 102).append("0123\n"),
                "",
            };

            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, expected);
        }
        SECTION("1024x3") {
            std::string filename{str::dirname(__FILE__) + "/data/test-1024x3.txt"};

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 102).append("0123\n"),
                str::repeat("0123456789", 102).append("0123\n"),
                str::repeat("0123456789", 102).append("0123\n"),
                "",
            };

            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(filename, true);
            ASSERT_EQ(lines, expected);
        }
        SECTION("maxn-limit:1024x3") {
            std::string filename{str::dirname(__FILE__) + "/data/test-1024x3.txt"};

            std::vector<std::string> lines;
            std::vector<std::string> expected{
                str::repeat("0123456789", 102).append("0123"),
                str::repeat("0123456789", 102).append("0123"),
            };

            lines = str::read_lines(filename, false, 2);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(filename, false, 2);
            ASSERT_EQ(lines, expected);
        }
        SECTION("maxn==0:1024x3") {
            std::string filename{str::dirname(__FILE__) + "/data/test-1024x3.txt"};

            std::vector<std::string> lines;
            std::vector<std::string> expected{};

            lines = str::read_lines(filename, false, 0);
            ASSERT_EQ(lines, expected);
            lines = str::read_lines(filename, false, 0);
            ASSERT_EQ(lines, expected);
        }
    }
}
