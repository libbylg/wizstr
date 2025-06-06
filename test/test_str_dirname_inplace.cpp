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

TEST(test_str, dirname_inplace) {
    SECTION("全路径") {
        std::string s;
        s = "/aaa/bbb/ccc";
        ASSERT_EQ(str::dirname_inplace(s), "/aaa/bbb");
        s = "/aaa/bbb/ccc";
        ASSERT_EQ(str::basename_inplace(s), "ccc");
    }
    SECTION("相对路径[1]") {
        std::string s;
        s = "aaa/bbb/ccc";
        ASSERT_EQ(str::dirname_inplace(s), "aaa/bbb");
        s = "aaa/bbb/ccc";
        ASSERT_EQ(str::basename_inplace(s), "ccc");
    }
    SECTION("相对路径[2]") {
        std::string s;
        s = "../bbb/ccc";
        ASSERT_EQ(str::dirname_inplace(s), "../bbb");
        s = "../bbb/ccc";
        ASSERT_EQ(str::basename_inplace(s), "ccc");
    }
    SECTION("相对路径[3]") {
        std::string s;
        s = "../ccc";
        ASSERT_EQ(str::dirname_inplace(s), "..");
        s = "../ccc";
        ASSERT_EQ(str::basename_inplace(s), "ccc");
    }
    SECTION("相对路径[4]") {
        std::string s;
        s = "./ccc";
        ASSERT_EQ(str::dirname_inplace(s), ".");
        s = "./ccc";
        ASSERT_EQ(str::basename_inplace(s), "ccc");
    }
    SECTION("无路径分隔符") {
        std::string s;
        s = "abc";
        ASSERT_EQ(str::dirname_inplace(s), "");
        s = "abc";
        ASSERT_EQ(str::basename_inplace(s), "abc");
    }
    SECTION("相对路径特殊表示(.)") {
        std::string s;
        s = ".";
        ASSERT_EQ(str::dirname_inplace(s), ".");
        s = ".";
        ASSERT_EQ(str::basename_inplace(s), "");
    }
    SECTION("相对路径特殊表示(..)") {
        std::string s;
        s = "..";
        ASSERT_EQ(str::dirname_inplace(s), "..");
        s = "..";
        ASSERT_EQ(str::basename_inplace(s), "");
    }
    SECTION("相对路径特殊表示( .. ):这种不符合一般认识的路径，就应该作为文件处理") {
        std::string s;
        s = " .. ";
        ASSERT_EQ(str::dirname_inplace(s), "");
        s = " .. ";
        ASSERT_EQ(str::basename_inplace(s), " .. ");
    }
    SECTION("相对路径特殊表示( . ):这种不符合一般认识的路径，就应该作为文件处理") {
        std::string s;
        s = " . ";
        ASSERT_EQ(str::dirname_inplace(s), "");
        s = " . ";
        ASSERT_EQ(str::basename_inplace(s), " . ");
    }
    SECTION("相对路径特殊表示(./)") {
        // os.path.dirname(".")
        // ''
        // os.path.basename(".")
        // '.'
        // os.path.dirname("./")
        // '.'
        // os.path.basename("./")
        // ''
        std::string s;
        s = "./";
        ASSERT_EQ(str::dirname_inplace(s), ".");
        s = "./";
        ASSERT_EQ(str::basename_inplace(s), "");
    }
    SECTION("相对路径特殊表示(../)") {
        // os.path.basename("..")
        // '..'
        // os.path.dirname("..")
        // ''
        // os.path.dirname("../")
        // '..'
        // os.path.basename("../")
        // ''
        std::string s;
        s = "../";
        ASSERT_EQ(str::dirname_inplace(s), "..");
        s = "../";
        ASSERT_EQ(str::basename_inplace(s), "");
    }
    SECTION("./和../[3]") {
        std::string s;
        s = "./aa";
        ASSERT_EQ(str::dirname_inplace(s), ".");
        s = "./aa";
        ASSERT_EQ(str::basename_inplace(s), "aa");
    }
    SECTION("./和../[4]") {
        std::string s;
        s = "../aa";
        ASSERT_EQ(str::dirname_inplace(s), "..");
        s = "../aa";
        ASSERT_EQ(str::basename_inplace(s), "aa");
    }
    SECTION("./和../[5]") {
        std::string s;
        s = "../aa/";
        ASSERT_EQ(str::dirname_inplace(s), "../aa");
        s = "../aa/";
        ASSERT_EQ(str::basename_inplace(s), "");
    }
    SECTION("./和../[5]") {
        std::string s;
        s = "../aa/..";
        ASSERT_EQ(str::dirname_inplace(s), "../aa/..");
        s = "../aa/..";
        ASSERT_EQ(str::basename_inplace(s), "");
    }
    SECTION("绝对路径 /") {
        std::string s;
        s = "/";
        ASSERT_EQ(str::dirname_inplace(s), "/");
        s = "/";
        ASSERT_EQ(str::basename_inplace(s), "");
    }
    SECTION("绝对路径 /") {
        std::string s;
        s = "/abc";
        ASSERT_EQ(str::dirname_inplace(s), "/");
        s = "/abc";
        ASSERT_EQ(str::basename_inplace(s), "abc");
    }
    SECTION("空串") {
        std::string s;
        s = "";
        ASSERT_EQ(str::dirname_inplace(s), "");
        s = "";
        ASSERT_EQ(str::basename_inplace(s), "");
    }
    SECTION("多余的路径元素[1]") {
        std::string s;
        s = "///ccc";
        ASSERT_EQ(str::dirname_inplace(s), "/");
        s = "///ccc";
        ASSERT_EQ(str::basename_inplace(s), "ccc");
    }
    SECTION("多余的路径元素[2]") {
        std::string s;
        s = ".///ccc";
        ASSERT_EQ(str::dirname_inplace(s), ".");
        s = ".///ccc";
        ASSERT_EQ(str::basename_inplace(s), "ccc");
    }
    SECTION("隐藏文件[1]") {
        std::string s;
        s = ".///.ccc";
        ASSERT_EQ(str::dirname_inplace(s), ".");
        s = ".///.ccc";
        ASSERT_EQ(str::basename_inplace(s), ".ccc");
    }
    SECTION("隐藏文件[2]") {
        std::string s;
        s = ".ccc";
        ASSERT_EQ(str::dirname_inplace(s), "");
        s = ".ccc";
        ASSERT_EQ(str::basename_inplace(s), ".ccc");
    }
}