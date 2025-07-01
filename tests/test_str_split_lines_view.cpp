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

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, split_lines_view) {
    GROUP("不保留行尾") {
        SECTION("一般情况:\\n") {
            std::string_view s{"a\nb\nc"};
            std::vector<std::string_view> expect{
                "a",
                "b",
                "c",
            };
            ASSERT_EQ(str::split_lines_view(s), expect);
        }
        SECTION("一般情况:\\r\\n") {
            std::string_view s{"a\r\nb\r\nc"};
            std::vector<std::string_view> expect{
                "a",
                "b",
                "c",
            };
            ASSERT_EQ(str::split_lines_view(s), expect);
        }
        SECTION("一般情况:\\r") {
            std::string_view s{"a\rb\rc"};
            std::vector<std::string_view> expect{
                "a",
                "b",
                "c",
            };
            ASSERT_EQ(str::split_lines_view(s), expect);
        }
        SECTION("一般情况:\\r收尾") {
            std::string_view s{"a\nb\nc\r"};
            std::vector<std::string_view> expect{
                "a",
                "b",
                "c",
            };
            ASSERT_EQ(str::split_lines_view(s), expect);
        }
        SECTION("一般情况:\\n收尾") {
            std::string_view s{"a\nb\nc\n"};
            std::vector<std::string_view> expect{
                "a",
                "b",
                "c",
            };
            ASSERT_EQ(str::split_lines_view(s), expect);
        }
        SECTION("一般情况:\\r\\n收尾") {
            std::string_view s{"a\rb\rc\r\n"};
            std::vector<std::string_view> expect{
                "a",
                "b",
                "c",
            };
            ASSERT_EQ(str::split_lines_view(s), expect);
        }
        SECTION("连续:\\r") {
            std::string_view s{"a\r\rb\r\rc"};
            std::vector<std::string_view> expect{
                "a",
                "",
                "b",
                "",
                "c",
            };
            std::vector<std::string_view> actual = str::split_lines_view(s);
            ASSERT_EQ(actual, expect);
        }
        SECTION("连续:\\n") {
            std::string_view s{"a\n\nb\n\nc"};
            std::vector<std::string_view> expect{
                "a",
                "",
                "b",
                "",
                "c",
            };
            ASSERT_EQ(str::split_lines_view(s), expect);
        }
        SECTION("单一:\\r") {
            std::string_view s{"\r"};
            std::vector<std::string_view> expect{
                "",
            };
            ASSERT_EQ(str::split_lines_view(s), expect);
        }
        SECTION("单一:\\n") {
            std::string_view s{"\n"};
            std::vector<std::string_view> expect{
                "",
            };
            ASSERT_EQ(str::split_lines_view(s), expect);
        }
        SECTION("空串") {
            std::string_view s{""};
            std::vector<std::string_view> expect{};
            ASSERT_EQ(str::split_lines_view(s), expect);
        }
        SECTION("无任何换行符号") {
            std::string_view s{"abcdef"};
            std::vector<std::string_view> expect{"abcdef"};
            ASSERT_EQ(str::split_lines_view(s), expect);
        }
    }

    GROUP("保留行尾") {
        SECTION("一般情况:\\n") {
            std::string_view s{"a\nb\nc"};
            std::vector<std::string_view> expect{
                "a\n",
                "b\n",
                "c",
            };
            ASSERT_EQ(str::split_lines_view(s, true), expect);
        }
        SECTION("一般情况:\\r\\n") {
            std::string_view s{"a\r\nb\r\nc"};
            std::vector<std::string_view> expect{
                "a\r\n",
                "b\r\n",
                "c",
            };
            ASSERT_EQ(str::split_lines_view(s, true), expect);
        }
        SECTION("一般情况:\\r") {
            std::string_view s{"a\rb\rc"};
            std::vector<std::string_view> expect{
                "a\r",
                "b\r",
                "c",
            };
            ASSERT_EQ(str::split_lines_view(s, true), expect);
        }
        SECTION("一般情况:\\r收尾") {
            std::string_view s{"a\nb\nc\r"};
            std::vector<std::string_view> expect{
                "a\n",
                "b\n",
                "c\r",
            };
            ASSERT_EQ(str::split_lines_view(s, true), expect);
        }
        SECTION("一般情况:\\n收尾") {
            std::string_view s{"a\nb\nc\n"};
            std::vector<std::string_view> expect{
                "a\n",
                "b\n",
                "c\n",
            };
            ASSERT_EQ(str::split_lines_view(s, true), expect);
        }
        SECTION("一般情况:\\r\\n收尾") {
            std::string_view s{"a\rb\rc\r\n"};
            std::vector<std::string_view> expect{
                "a\r",
                "b\r",
                "c\r\n",
            };
            ASSERT_EQ(str::split_lines_view(s, true), expect);
        }
        SECTION("连续:\\r") {
            std::string_view s{"a\r\rb\r\rc"};
            std::vector<std::string_view> expect{
                "a\r",
                "\r",
                "b\r",
                "\r",
                "c",
            };
            ASSERT_EQ(str::split_lines_view(s, true), expect);
        }
        SECTION("连续:\\n") {
            std::string_view s{"a\n\nb\n\nc"};
            std::vector<std::string_view> expect{
                "a\n",
                "\n",
                "b\n",
                "\n",
                "c",
            };
            ASSERT_EQ(str::split_lines_view(s, true), expect);
        }
        SECTION("单一:\\r") {
            std::string_view s{"\r"};
            std::vector<std::string_view> expect{
                "\r",
            };
            ASSERT_EQ(str::split_lines_view(s, true), expect);
        }
        SECTION("单一:\\n") {
            std::string_view s{"\n"};
            std::vector<std::string_view> expect{
                "\n",
            };
            ASSERT_EQ(str::split_lines_view(s, true), expect);
        }
        SECTION("空串") {
            std::string_view s{""};
            std::vector<std::string_view> expect{};
            ASSERT_EQ(str::split_lines_view(s, true), expect);
        }
        SECTION("无任何换行符号") {
            std::string_view s{"abcdef"};
            std::vector<std::string_view> expect{"abcdef"};
            ASSERT_EQ(str::split_lines_view(s, true), expect);
        }
    }
}