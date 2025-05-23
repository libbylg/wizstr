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

TEST(test_str, expand_envs) {
    SECTION("${xxx}形式") {
        ASSERT_EQ(str::expand_envs("${HOME}/${NOTEXIST}"), std::string(getenv("HOME")) + "/");
        ASSERT_EQ(str::expand_envs("${HOME}/${NOTEXIST}", true), std::string(getenv("HOME")) + "/${NOTEXIST}");
    }

    SECTION("$xxx形式") {
        ASSERT_EQ(str::expand_envs("$HOME/$NOTEXIST"), std::string(getenv("HOME")) + "/");
        ASSERT_EQ(str::expand_envs("$HOME/$NOTEXIST", true), std::string(getenv("HOME")) + "/$NOTEXIST");
    }

    SECTION("字符串中间") {
        ASSERT_EQ(str::expand_envs("abcd$HOME/${NOTEXIST}"), "abcd" + std::string(getenv("HOME")) + "/");
        ASSERT_EQ(str::expand_envs("abcd$HOME/${NOTEXIST}", true), "abcd" + std::string(getenv("HOME")) + "/${NOTEXIST}");
    }

    SECTION("${xxx}在字符串尾部") {
        ASSERT_EQ(str::expand_envs("abcd${HOME}"), "abcd" + std::string(getenv("HOME")));
        ASSERT_EQ(str::expand_envs("abcd${HOME}", true), "abcd" + std::string(getenv("HOME")));
    }

    SECTION("一个字符串只有 $xxx") {
        ASSERT_EQ(str::expand_envs("$HOME"), std::string(getenv("HOME")));
        ASSERT_EQ(str::expand_envs("$HOME", true), std::string(getenv("HOME")));
    }

    SECTION("错误变量") {
        ASSERT_EQ(str::expand_envs("$$$"), "$$$");
        ASSERT_EQ(str::expand_envs("$$$", true), "$$$");
        ASSERT_EQ(str::expand_envs("${HOME aaa"), "${HOME aaa");
        ASSERT_EQ(str::expand_envs("${HOME aaa", true), "${HOME aaa");
        ASSERT_EQ(str::expand_envs("abc$"), "abc$");
        ASSERT_EQ(str::expand_envs("abc$", true), "abc$");
    }

    SECTION("空串") {
        ASSERT_EQ(str::expand_envs(""), "");
        ASSERT_EQ(str::expand_envs("", true), "");
    }

    SECTION("无可替换变量") {
        ASSERT_EQ(str::expand_envs("Hello world"), "Hello world");
        ASSERT_EQ(str::expand_envs("Hello world", true), "Hello world");
    }

    SECTION("通过map提供数据") {
        std::map<std::string, std::string> items{
            {"HOME", "XXX"},
        };
        ASSERT_EQ(str::expand_envs("${HOME}/${NOTEXIST}", items), "XXX/");
        ASSERT_EQ(str::expand_envs("${HOME}/${NOTEXIST}", true, items), "XXX/${NOTEXIST}");
    }

    SECTION("通过key-value参数提供数据") {
        ASSERT_EQ(str::expand_envs("${HOME}/${NOTEXIST}", "HOME", "XXX"), "XXX/${NOTEXIST}");
    }

    SECTION("通过proc提供数据") {
        ASSERT_EQ(str::expand_envs("${HOME}/${NOTEXIST}/${HOME}", false, [](const std::string& key) -> std::optional<std::string> {
            if (key == "HOME") {
                return "xxxx";
            }

            return std::nullopt;
        }),
            "xxxx//xxxx");
        ASSERT_EQ(str::expand_envs("${HOME}/${NOTEXIST}/${HOME}", true, [](const std::string& key) -> std::optional<std::string> {
            if (key == "HOME") {
                return "xxxx";
            }

            return std::nullopt;
        }),
            "xxxx/${NOTEXIST}/xxxx");
    }
}
