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
#include <variant>

#include "testing.hpp"

#include "str.hpp"

#include <cstdlib>

TEST(test_str, expand_envs_inplace) {
    std::string ENV_XYZ = "/home/xyz";
    std::string ENV_XYZ_KV{"ENV_XYZ=/home/xyz"};
    putenv(ENV_XYZ_KV.c_str());
#ifndef WIN32
    unsetenv("ENV_XYZ");
#endif

    SECTION("${xxx}形式") {
        std::string s;
        str::expand_envs_inplace(s = "${ENV_XYZ}/${NOTEXIST}");
        ASSERT_EQ(s, ENV_XYZ + "/");
        ASSERT_EQ(str::expand_envs_inplace(s = "${ENV_XYZ}/${NOTEXIST}", true), ENV_XYZ + "/${NOTEXIST}");
    }

    SECTION("$xxx形式") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = "$ENV_XYZ/$NOTEXIST"), ENV_XYZ + "/");
        ASSERT_EQ(str::expand_envs_inplace(s = "$ENV_XYZ/$NOTEXIST", true), ENV_XYZ + "/$NOTEXIST");
    }

    SECTION("字符串中间") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = "abcd$ENV_XYZ/${NOTEXIST}"), "abcd" + ENV_XYZ + "/");
        ASSERT_EQ(str::expand_envs_inplace(s = "abcd$ENV_XYZ/${NOTEXIST}", true), "abcd" + ENV_XYZ + "/${NOTEXIST}");
    }

    SECTION("${xxx}在字符串尾部") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = "abcd${ENV_XYZ}"), "abcd" + ENV_XYZ);
        ASSERT_EQ(str::expand_envs_inplace(s = "abcd${ENV_XYZ}", true), "abcd" + ENV_XYZ);
    }

    SECTION("一个字符串只有 $xxx") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = "$ENV_XYZ"), ENV_XYZ);
        ASSERT_EQ(str::expand_envs_inplace(s = "$ENV_XYZ", true), ENV_XYZ);
    }

    SECTION("错误变量") {
        std::string s;
        ASSERT_EQ(str::expand_envs_inplace(s = "$$$"), "$$$");
        ASSERT_EQ(str::expand_envs_inplace(s = "$$$", true), "$$$");
        ASSERT_EQ(str::expand_envs_inplace(s = "${ENV_XYZ aaa"), "${ENV_XYZ aaa");
        ASSERT_EQ(str::expand_envs_inplace(s = "${ENV_XYZ aaa", true), "${ENV_XYZ aaa");
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
