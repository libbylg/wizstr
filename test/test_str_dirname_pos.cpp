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

TEST(test_str, dirname_pos) {
    SECTION("全路径") {
        ASSERT_EQ(str::dirname_pos("/aaa/bbb/ccc"), 8);
        ASSERT_EQ(str::basename_pos("/aaa/bbb/ccc"), 9);
    }
    SECTION("相对路径[1]") {
        ASSERT_EQ(str::dirname_pos("aaa/bbb/ccc"), 7);
        ASSERT_EQ(str::basename_pos("aaa/bbb/ccc"), 8);
    }
    SECTION("相对路径[2]") {
        ASSERT_EQ(str::dirname_pos("../bbb/ccc"), 6);
        ASSERT_EQ(str::basename_pos("../bbb/ccc"), 7);
    }
    SECTION("相对路径[3]") {
        ASSERT_EQ(str::dirname_pos("../ccc"), 2);
        ASSERT_EQ(str::basename_pos("../ccc"), 3);
    }
    SECTION("相对路径[4]") {
        ASSERT_EQ(str::dirname_pos("./ccc"), 1);
        ASSERT_EQ(str::basename_pos("./ccc"), 2);
    }
    SECTION("无路径分隔符") {
        ASSERT_EQ(str::dirname_pos("abc"), 0);
        ASSERT_EQ(str::basename_pos("abc"), 0);
    }
    SECTION("相对路径特殊表示(.)") {
        ASSERT_EQ(str::dirname_pos("."), 1);
        ASSERT_EQ(str::basename_pos("."), 1);
    }
    SECTION("相对路径特殊表示(..)") {
        ASSERT_EQ(str::dirname_pos(".."), 2);
        ASSERT_EQ(str::basename_pos(".."), 2);
    }
    SECTION("相对路径特殊表示( .. ):这种不符合一般认识的路径，就应该作为文件处理") {
        ASSERT_EQ(str::dirname_pos(" .. "), 0);
        ASSERT_EQ(str::basename_pos(" .. "), 0);
    }
    SECTION("相对路径特殊表示( . ):这种不符合一般认识的路径，就应该作为文件处理") {
        ASSERT_EQ(str::dirname_pos(" . "), 0);
        ASSERT_EQ(str::basename_pos(" . "), 0);
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
        ASSERT_EQ(str::dirname_pos("./"), 1);
        ASSERT_EQ(str::basename_pos("./"), 2);
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
        ASSERT_EQ(str::dirname_pos("../"), 2);
        ASSERT_EQ(str::basename_pos("../"), 3);
    }
    SECTION("./和../[3]") {
        ASSERT_EQ(str::dirname_pos("./aa"), 1);
        ASSERT_EQ(str::basename_pos("./aa"), 2);
    }
    SECTION("./和../[4]") {
        ASSERT_EQ(str::dirname_pos("../aa"), 2);
        ASSERT_EQ(str::basename_pos("../aa"), 3);
    }
    SECTION("./和../[5]") {
        ASSERT_EQ(str::dirname_pos("../aa/"), 5);
        ASSERT_EQ(str::basename_pos("../aa/"), 6);
    }
    SECTION("./和../[5]") {
        ASSERT_EQ(str::dirname_pos("../aa/.."), 8);
        ASSERT_EQ(str::basename_pos("../aa/.."), 8);
    }
    SECTION("绝对路径 /") {
        ASSERT_EQ(str::dirname_pos("/"), 1);
        ASSERT_EQ(str::basename_pos("/"), 1);
    }
    SECTION("空串") {
        ASSERT_EQ(str::dirname_pos(""), 0);
        ASSERT_EQ(str::basename_pos(""), 0);
    }
    SECTION("多余的路径元素[1]") {
        ASSERT_EQ(str::dirname_pos("///ccc"), 1);
        ASSERT_EQ(str::basename_pos("///ccc"), 3);
    }
    SECTION("多余的路径元素[2]") {
        ASSERT_EQ(str::dirname_pos(".///ccc"), 1);
        ASSERT_EQ(str::basename_pos(".///ccc"), 4);
    }
    SECTION("隐藏文件[1]") {
        ASSERT_EQ(str::dirname_pos(".///.ccc"), 1);
        ASSERT_EQ(str::basename_pos(".///.ccc"), 4);
    }
    SECTION("隐藏文件[2]") {
        ASSERT_EQ(str::dirname_pos(".ccc"), 0);
        ASSERT_EQ(str::basename_pos(".ccc"), 0);
    }
}