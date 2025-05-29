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

#include <fstream>

TEST(test_str, read_next_line) {
    GROUP("FILE") {
        SECTION("general") {
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
        SECTION("emptylines") {
            std::string filename{str::dirname(__FILE__) + "/data/test-emptylines.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file, false)));
            ASSERT_EQ(line, "");
            ASSERT_TRUE((line = str::read_next_line(file, false)));
            ASSERT_EQ(line, "");
            ASSERT_TRUE((line = str::read_next_line(file, false)));
            ASSERT_EQ(line, "");
            ASSERT_FALSE((line = str::read_next_line(file, false)));
            ASSERT_FALSE((line = str::read_next_line(file, false)));
        }
        SECTION("510") {
            std::string expected;
            expected = str::repeat("0123456789", 51);
            expected.append("\n");

            std::string filename{str::dirname(__FILE__) + "/data/test-510.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file, true)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_FALSE((line = str::read_next_line(file, true)));
            ASSERT_FALSE((line = str::read_next_line(file, true)));
        }
        SECTION("511") {
            std::string expected;
            expected = str::repeat("0123456789", 51);
            expected.append("0\n");

            std::string filename{str::dirname(__FILE__) + "/data/test-511.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file, true)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_FALSE((line = str::read_next_line(file, true)));
            ASSERT_FALSE((line = str::read_next_line(file, true)));
        }
        SECTION("512") {
            std::string expected;
            expected = str::repeat("0123456789", 51);
            expected.append("01\n");

            std::string filename{str::dirname(__FILE__) + "/data/test-512.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file, true)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_FALSE((line = str::read_next_line(file, true)));
            ASSERT_FALSE((line = str::read_next_line(file, true)));
        }
        SECTION("513") {
            std::string expected;
            expected = str::repeat("0123456789", 51);
            expected.append("012\n");

            std::string filename{str::dirname(__FILE__) + "/data/test-513.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file, true)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_FALSE((line = str::read_next_line(file, true)));
            ASSERT_FALSE((line = str::read_next_line(file, true)));
        }
        SECTION("1024") {
            std::string expected;
            expected = str::repeat("0123456789", 102);
            expected.append("0123\n");

            std::string filename{str::dirname(__FILE__) + "/data/test-1024.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file, true)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_FALSE((line = str::read_next_line(file, true)));
            ASSERT_FALSE((line = str::read_next_line(file, true)));
        }
        SECTION("1024x3") {
            std::string expected;
            expected = str::repeat("0123456789", 102);
            expected.append("0123\n");

            std::string filename{str::dirname(__FILE__) + "/data/test-1024x3.txt"};
            FILE* file = fopen(filename.c_str(), "r");
            assert(file != nullptr);
            scope_guard file_guard([file] { fclose(file); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file, true)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_TRUE((line = str::read_next_line(file, true)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_TRUE((line = str::read_next_line(file, true)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_FALSE((line = str::read_next_line(file, true)));
            ASSERT_FALSE((line = str::read_next_line(file, true)));
        }
    }
    GROUP("fstream") {
        SECTION("general") {
            std::string filename{str::dirname(__FILE__) + "/data/test-general.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file)));
            ASSERT_EQ(line, "ABCDE12345");
            ASSERT_TRUE((line = str::read_next_line(file)));
            ASSERT_EQ(line, "ABCDE");
            ASSERT_TRUE((line = str::read_next_line(file)));
            ASSERT_EQ(line, "12345");
            ASSERT_FALSE((line = str::read_next_line(file)));
            ASSERT_FALSE((line = str::read_next_line(file)));
        }
        SECTION("emptylines") {
            std::string filename{str::dirname(__FILE__) + "/data/test-emptylines.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file)));
            ASSERT_EQ(line, "");
            ASSERT_TRUE((line = str::read_next_line(file)));
            ASSERT_EQ(line, "");
            ASSERT_TRUE((line = str::read_next_line(file)));
            ASSERT_EQ(line, "");
            ASSERT_FALSE((line = str::read_next_line(file)));
            ASSERT_FALSE((line = str::read_next_line(file)));
        }
        SECTION("510") {
            std::string expected;
            expected = str::repeat("0123456789", 51);

            std::string filename{str::dirname(__FILE__) + "/data/test-510.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_FALSE((line = str::read_next_line(file)));
            ASSERT_FALSE((line = str::read_next_line(file)));
        }
        SECTION("511") {
            std::string expected;
            expected = str::repeat("0123456789", 51);
            expected.append("0");

            std::string filename{str::dirname(__FILE__) + "/data/test-511.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_FALSE((line = str::read_next_line(file)));
            ASSERT_FALSE((line = str::read_next_line(file)));
        }
        SECTION("512") {
            std::string expected;
            expected = str::repeat("0123456789", 51);
            expected.append("01");

            std::string filename{str::dirname(__FILE__) + "/data/test-512.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_FALSE((line = str::read_next_line(file)));
            ASSERT_FALSE((line = str::read_next_line(file)));
        }
        SECTION("513") {
            std::string expected;
            expected = str::repeat("0123456789", 51);
            expected.append("012");

            std::string filename{str::dirname(__FILE__) + "/data/test-513.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_FALSE((line = str::read_next_line(file)));
            ASSERT_FALSE((line = str::read_next_line(file)));
        }
        SECTION("1024") {
            std::string expected;
            expected = str::repeat("0123456789", 102);
            expected.append("0123");

            std::string filename{str::dirname(__FILE__) + "/data/test-1024.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_FALSE((line = str::read_next_line(file)));
            ASSERT_FALSE((line = str::read_next_line(file)));
        }
        SECTION("1024x3") {
            std::string expected;
            expected = str::repeat("0123456789", 102);
            expected.append("0123");

            std::string filename{str::dirname(__FILE__) + "/data/test-1024x3.txt"};
            std::fstream file(filename, std::ios::in);
            scope_guard file_guard([&file] { file.close(); });
            std::optional<std::string> line;
            ASSERT_TRUE((line = str::read_next_line(file)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_TRUE((line = str::read_next_line(file)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_TRUE((line = str::read_next_line(file)));
            ASSERT_EQ(line.value(), expected);
            ASSERT_FALSE((line = str::read_next_line(file)));
            ASSERT_FALSE((line = str::read_next_line(file)));
        }
    }
}
