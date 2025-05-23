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

TEST(test_str, expand_envs_inplace) {
    SECTION("${xxx}形式") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = "${HOME}/${NOTEXIST}"), std::string(getenv("HOME")) + "/");
        ASSERT_EQ(str::expand_envs_inplace(s = "${HOME}/${NOTEXIST}", true), std::string(getenv("HOME")) + "/${NOTEXIST}");
    }

    SECTION("$xxx形式") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = "$HOME/$NOTEXIST"), std::string(getenv("HOME")) + "/");
        ASSERT_EQ(str::expand_envs_inplace(s = "$HOME/$NOTEXIST", true), std::string(getenv("HOME")) + "/$NOTEXIST");
    }

    SECTION("字符串中间") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = "abcd$HOME/${NOTEXIST}"), "abcd" + std::string(getenv("HOME")) + "/");
        ASSERT_EQ(str::expand_envs_inplace(s = "abcd$HOME/${NOTEXIST}", true), "abcd" + std::string(getenv("HOME")) + "/${NOTEXIST}");
    }

    SECTION("${xxx}在字符串尾部") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = "abcd${HOME}"), "abcd" + std::string(getenv("HOME")));
        ASSERT_EQ(str::expand_envs_inplace(s = "abcd${HOME}", true), "abcd" + std::string(getenv("HOME")));
    }

    SECTION("一个字符串只有 $xxx") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = "$HOME"), std::string(getenv("HOME")));
        ASSERT_EQ(str::expand_envs_inplace(s = "$HOME", true), std::string(getenv("HOME")));
    }

    SECTION("错误变量") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = "$$$"), "$$$");
        ASSERT_EQ(str::expand_envs_inplace(s = "$$$", true), "$$$");
        ASSERT_EQ(str::expand_envs_inplace(s = "${HOME aaa"), "${HOME aaa");
        ASSERT_EQ(str::expand_envs_inplace(s = "${HOME aaa", true), "${HOME aaa");
        ASSERT_EQ(str::expand_envs_inplace(s = "abc$"), "abc$");
        ASSERT_EQ(str::expand_envs_inplace(s = "abc$", true), "abc$");
    }

    SECTION("空串") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = ""), "");
        ASSERT_EQ(str::expand_envs_inplace(s = "", true), "");
    }

    SECTION("无可替换变量") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = "Hello world"), "Hello world");
        ASSERT_EQ(str::expand_envs_inplace(s = "Hello world", true), "Hello world");
    }

    SECTION("通过map提供数据") {
        std::string s;
        std::map<std::string, std::string> items{
            {"HOME", "XXX"},
        };
        ASSERT_EQ(str::expand_envs_inplace(s = "${HOME}/${NOTEXIST}", items), "XXX/");
        ASSERT_EQ(str::expand_envs_inplace(s = "${HOME}/${NOTEXIST}", true, items), "XXX/${NOTEXIST}");
    }

    SECTION("通过key-value参数提供数据") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = "${HOME}/${NOTEXIST}", "HOME", "XXX"), "XXX/${NOTEXIST}");
    }

    SECTION("通过proc提供数据") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = "${HOME}/${NOTEXIST}/${HOME}", false, [](const std::string& key) -> std::optional<std::string> {
            if (key == "HOME") {
                return "xxxx";
            }

            return std::nullopt;
        }),
            "xxxx//xxxx");
        ASSERT_EQ(str::expand_envs_inplace(s = "${HOME}/${NOTEXIST}/${HOME}", true, [](const std::string& key) -> std::optional<std::string> {
            if (key == "HOME") {
                return "xxxx";
            }

            return std::nullopt;
        }),
            "xxxx/${NOTEXIST}/xxxx");
    }
}
