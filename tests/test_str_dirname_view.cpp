/**
 * Copyright (c) 2021-2025  libbylg@126.com
 * wizstr is licensed under Mulan PSL v2.
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

#if defined(WIZSTR_NAMESPACE)
using str = WIZSTR_NAMESPACE::str;
#endif

TEST(test_str, dirname_view) {
    SECTION("全路径") {
        std::string s;
        s = "/aaa/bbb/ccc";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 8));
        s = "/aaa/bbb/ccc";
        ASSERT_EQ(str::basename_view(s), s.substr(9, 3));
    }
    SECTION("相对路径[1]") {
        std::string s;
        s = "aaa/bbb/ccc";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 7));
        s = "aaa/bbb/ccc";
        ASSERT_EQ(str::basename_view(s), s.substr(8, 3));
    }
    SECTION("相对路径[2]") {
        std::string s;
        s = "../bbb/ccc";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 6));
        s = "../bbb/ccc";
        ASSERT_EQ(str::basename_view(s), s.substr(7, 3));
    }
    SECTION("相对路径[3]") {
        std::string s;
        s = "../ccc";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 2));
        s = "../ccc";
        ASSERT_EQ(str::basename_view(s), s.substr(3, 3));
    }
    SECTION("相对路径[4]") {
        std::string s;
        s = "./ccc";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 1));
        s = "./ccc";
        ASSERT_EQ(str::basename_view(s), s.substr(2, 3));
    }
    SECTION("无路径分隔符") {
        std::string s;
        s = "abc";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 0));
        s = "abc";
        ASSERT_EQ(str::basename_view(s), s.substr(0, 3));
    }
    SECTION("相对路径特殊表示(.)") {
        std::string s;
        s = ".";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 1));
        s = ".";
        ASSERT_EQ(str::basename_view(s), s.substr(1, 0));
    }
    SECTION("相对路径特殊表示(..)") {
        std::string s;
        s = "..";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 2));
        s = "..";
        ASSERT_EQ(str::basename_view(s), s.substr(2, 0));
    }
    SECTION("相对路径特殊表示( .. ):这种不符合一般认识的路径，就应该作为文件处理") {
        std::string s;
        s = " .. ";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 0));
        s = " .. ";
        ASSERT_EQ(str::basename_view(s), s.substr(0, 4));
    }
    SECTION("相对路径特殊表示( . ):这种不符合一般认识的路径，就应该作为文件处理") {
        std::string s;
        s = " . ";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 0));
        s = " . ";
        ASSERT_EQ(str::basename_view(s), s.substr(0, 3));
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
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 1));
        s = "./";
        ASSERT_EQ(str::basename_view(s), s.substr(2, 0));
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
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 2));
        s = "../";
        ASSERT_EQ(str::basename_view(s), s.substr(3, 0));
    }
    SECTION("./和../[3]") {
        std::string s;
        s = "./aa";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 1));
        s = "./aa";
        ASSERT_EQ(str::basename_view(s), s.substr(2, 2));
    }
    SECTION("./和../[4]") {
        std::string s;
        s = "../aa";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 2));
        s = "../aa";
        ASSERT_EQ(str::basename_view(s), s.substr(3, 2));
    }
    SECTION("./和../[5]") {
        std::string s;
        s = "../aa/";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 5));
        s = "../aa/";
        ASSERT_EQ(str::basename_view(s), s.substr(6, 0));
    }
    SECTION("./和../[5]") {
        std::string s;
        s = "../aa/..";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 8));
        s = "../aa/..";
        ASSERT_EQ(str::basename_view(s), s.substr(8, 0));
    }
    SECTION("绝对路径 /") {
        std::string s;
        s = "/";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 1));
        s = "/";
        ASSERT_EQ(str::basename_view(s), s.substr(1, 0));
    }
    SECTION("绝对路径 /") {
        std::string s;
        s = "/abc";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 1));
        s = "/abc";
        ASSERT_EQ(str::basename_view(s), s.substr(1, 3));
    }
    SECTION("空串") {
        std::string s;
        s = "";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 0));
        s = "";
        ASSERT_EQ(str::basename_view(s), s.substr(0, 0));
    }
    SECTION("多余的路径元素[1]") {
        std::string s;
        s = "///ccc";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 1));
        s = "///ccc";
        ASSERT_EQ(str::basename_view(s), s.substr(3, 3));
    }
    SECTION("多余的路径元素[2]") {
        std::string s;
        s = ".///ccc";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 1));
        s = ".///ccc";
        ASSERT_EQ(str::basename_view(s), s.substr(4, 3));
    }
    SECTION("隐藏文件[1]") {
        std::string s;
        s = ".///.ccc";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 1));
        s = ".///.ccc";
        ASSERT_EQ(str::basename_view(s), s.substr(4, 4));
    }
    SECTION("隐藏文件[2]") {
        std::string s;
        s = ".ccc";
        ASSERT_EQ(str::dirname_view(s), s.substr(0, 0));
        s = ".ccc";
        ASSERT_EQ(str::basename_view(s), s.substr(0, 4));
    }
}