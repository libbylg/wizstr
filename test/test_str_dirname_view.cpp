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

TEST(test_str, dirname_view) {
    SECTION("全路径") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = "/aaa/bbb/ccc"), s.substr(0, 8));
        ASSERT_EQ(str::basename_view(s = "/aaa/bbb/ccc"), s.substr(9, 3));
    }
    SECTION("相对路径[1]") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = "aaa/bbb/ccc"), s.substr(0, 7));
        ASSERT_EQ(str::basename_view(s = "aaa/bbb/ccc"), s.substr(8, 3));
    }
    SECTION("相对路径[2]") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = "../bbb/ccc"), s.substr(0, 6));
        ASSERT_EQ(str::basename_view(s = "../bbb/ccc"), s.substr(7, 3));
    }
    SECTION("相对路径[3]") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = "../ccc"), s.substr(0, 2));
        ASSERT_EQ(str::basename_view(s = "../ccc"), s.substr(3, 3));
    }
    SECTION("相对路径[4]") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = "./ccc"), s.substr(0, 1));
        ASSERT_EQ(str::basename_view(s = "./ccc"), s.substr(2, 3));
    }
    SECTION("无路径分隔符") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = "abc"), s.substr(0, 0));
        ASSERT_EQ(str::basename_view(s = "abc"), s.substr(0, 3));
    }
    SECTION("相对路径特殊表示(.)") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = "."), s.substr(0, 1));
        ASSERT_EQ(str::basename_view(s = "."), s.substr(1, 0));
    }
    SECTION("相对路径特殊表示(..)") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = ".."), s.substr(0, 2));
        ASSERT_EQ(str::basename_view(s = ".."), s.substr(2, 0));
    }
    SECTION("相对路径特殊表示( .. ):这种不符合一般认识的路径，就应该作为文件处理") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = " .. "), s.substr(0, 0));
        ASSERT_EQ(str::basename_view(s = " .. "), s.substr(0, 4));
    }
    SECTION("相对路径特殊表示( . ):这种不符合一般认识的路径，就应该作为文件处理") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = " . "), s.substr(0, 0));
        ASSERT_EQ(str::basename_view(s = " . "), s.substr(0, 3));
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
        ASSERT_EQ(str::dirname_view(s = "./"), s.substr(0, 1));
        ASSERT_EQ(str::basename_view(s = "./"), s.substr(2, 0));
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
        ASSERT_EQ(str::dirname_view(s = "../"), s.substr(0, 2));
        ASSERT_EQ(str::basename_view(s = "../"), s.substr(3, 0));
    }
    SECTION("./和../[3]") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = "./aa"), s.substr(0, 1));
        ASSERT_EQ(str::basename_view(s = "./aa"), s.substr(2, 2));
    }
    SECTION("./和../[4]") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = "../aa"), s.substr(0, 2));
        ASSERT_EQ(str::basename_view(s = "../aa"), s.substr(3, 2));
    }
    SECTION("./和../[5]") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = "../aa/"), s.substr(0, 5));
        ASSERT_EQ(str::basename_view(s = "../aa/"), s.substr(6, 0));
    }
    SECTION("./和../[5]") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = "../aa/.."), s.substr(0, 8));
        ASSERT_EQ(str::basename_view(s = "../aa/.."), s.substr(8, 0));
    }
    SECTION("绝对路径 /") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = "/"), s.substr(0, 1));
        ASSERT_EQ(str::basename_view(s = "/"), s.substr(1, 0));
    }
    SECTION("空串") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = ""), s.substr(0, 0));
        ASSERT_EQ(str::basename_view(s = ""), s.substr(0, 0));
    }
    SECTION("多余的路径元素[1]") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = "///ccc"), s.substr(0, 1));
        ASSERT_EQ(str::basename_view(s = "///ccc"), s.substr(3, 3));
    }
    SECTION("多余的路径元素[2]") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = ".///ccc"), s.substr(0, 1));
        ASSERT_EQ(str::basename_view(s = ".///ccc"), s.substr(4, 3));
    }
    SECTION("隐藏文件[1]") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = ".///.ccc"), s.substr(0, 1));
        ASSERT_EQ(str::basename_view(s = ".///.ccc"), s.substr(4, 4));
    }
    SECTION("隐藏文件[2]") {
        std::string s;
        ASSERT_EQ(str::dirname_view(s = ".ccc"), s.substr(0, 0));
        ASSERT_EQ(str::basename_view(s = ".ccc"), s.substr(0, 4));
    }
}