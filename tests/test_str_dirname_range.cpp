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

TEST(test_str, dirname_range) {
    SECTION("全路径") {
        ASSERT_EQ(str::dirname_range("/aaa/bbb/ccc"), str::range(0, 8));
        ASSERT_EQ(str::basename_range("/aaa/bbb/ccc"), str::range(9, 3));
    }
    SECTION("相对路径[1]") {
        ASSERT_EQ(str::dirname_range("aaa/bbb/ccc"), str::range(0, 7));
        ASSERT_EQ(str::basename_range("aaa/bbb/ccc"), str::range(8, 3));
    }
    SECTION("相对路径[2]") {
        ASSERT_EQ(str::dirname_range("../bbb/ccc"), str::range(0, 6));
        ASSERT_EQ(str::basename_range("../bbb/ccc"), str::range(7, 3));
    }
    SECTION("相对路径[3]") {
        ASSERT_EQ(str::dirname_range("../ccc"), str::range(0, 2));
        ASSERT_EQ(str::basename_range("../ccc"), str::range(3, 3));
    }
    SECTION("相对路径[4]") {
        ASSERT_EQ(str::dirname_range("./ccc"), str::range(0, 1));
        ASSERT_EQ(str::basename_range("./ccc"), str::range(2, 3));
    }
    SECTION("无路径分隔符") {
        ASSERT_EQ(str::dirname_range("abc"), str::range(0, 0));
        ASSERT_EQ(str::basename_range("abc"), str::range(0, 3));
    }
    SECTION("相对路径特殊表示(.)") {
        ASSERT_EQ(str::dirname_range("."), str::range(0, 1));
        ASSERT_EQ(str::basename_range("."), str::range(1, 0));
    }
    SECTION("相对路径特殊表示(..)") {
        ASSERT_EQ(str::dirname_range(".."), str::range(0, 2));
        ASSERT_EQ(str::basename_range(".."), str::range(2, 0));
    }
    SECTION("相对路径特殊表示( .. ):这种不符合一般认识的路径，就应该作为文件处理") {
        ASSERT_EQ(str::dirname_range(" .. "), str::range(0, 0));
        ASSERT_EQ(str::basename_range(" .. "), str::range(0, 4));
    }
    SECTION("相对路径特殊表示( . ):这种不符合一般认识的路径，就应该作为文件处理") {
        ASSERT_EQ(str::dirname_range(" . "), str::range(0, 0));
        ASSERT_EQ(str::basename_range(" . "), str::range(0, 3));
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
        ASSERT_EQ(str::dirname_range("./"), str::range(0, 1));
        ASSERT_EQ(str::basename_range("./"), str::range(2, 0));
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
        ASSERT_EQ(str::dirname_range("../"), str::range(0, 2));
        ASSERT_EQ(str::basename_range("../"), str::range(3, 0));
    }
    SECTION("./和../[3]") {
        ASSERT_EQ(str::dirname_range("./aa"), str::range(0, 1));
        ASSERT_EQ(str::basename_range("./aa"), str::range(2, 2));
    }
    SECTION("./和../[4]") {
        ASSERT_EQ(str::dirname_range("../aa"), str::range(0, 2));
        ASSERT_EQ(str::basename_range("../aa"), str::range(3, 2));
    }
    SECTION("./和../[5]") {
        ASSERT_EQ(str::dirname_range("../aa/"), str::range(0, 5));
        ASSERT_EQ(str::basename_range("../aa/"), str::range(6, 0));
    }
    SECTION("./和../[5]") {
        ASSERT_EQ(str::dirname_range("../aa/.."), str::range(0, 8));
        ASSERT_EQ(str::basename_range("../aa/.."), str::range(8, 0));
    }
    SECTION("绝对路径 /") {
        ASSERT_EQ(str::dirname_range("/"), str::range(0, 1));
        ASSERT_EQ(str::basename_range("/"), str::range(1, 0));
    }
    SECTION("绝对路径 /") {
        ASSERT_EQ(str::dirname_range("/abc"), str::range(0, 1));
        ASSERT_EQ(str::basename_range("/abc"), str::range(1, 3));
    }
    SECTION("空串") {
        ASSERT_EQ(str::dirname_range(""), str::range(0, 0));
        ASSERT_EQ(str::basename_range(""), str::range(0, 0));
    }
    SECTION("多余的路径元素[1]") {
        ASSERT_EQ(str::dirname_range("///ccc"), str::range(0, 1));
        ASSERT_EQ(str::basename_range("///ccc"), str::range(3, 3));
    }
    SECTION("多余的路径元素[2]") {
        ASSERT_EQ(str::dirname_range(".///ccc"), str::range(0, 1));
        ASSERT_EQ(str::basename_range(".///ccc"), str::range(4, 3));
    }
    SECTION("隐藏文件[1]") {
        ASSERT_EQ(str::dirname_range(".///.ccc"), str::range(0, 1));
        ASSERT_EQ(str::basename_range(".///.ccc"), str::range(4, 4));
    }
    SECTION("隐藏文件[2]") {
        ASSERT_EQ(str::dirname_range(".ccc"), str::range(0, 0));
        ASSERT_EQ(str::basename_range(".ccc"), str::range(0, 4));
    }
}