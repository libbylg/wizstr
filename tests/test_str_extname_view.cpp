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

TEST(test_str, extname_view) {
    SECTION("简单裸名字") {
        std::string s;
        s = "abc";
        ASSERT_EQ(str::extname_view(s), s.substr(3));
    }
    SECTION("简单裸名字带扩展名") {
        std::string s;
        s = "abc.ext";
        ASSERT_EQ(str::extname_view(s), s.substr(3));
    }
    SECTION("裸名字带路径") {
        std::string s;
        s = "abc/def";
        ASSERT_EQ(str::extname_view(s), s.substr(7));
    }
    SECTION("只有路径无basename") {
        std::string s;
        s = "abc/";
        ASSERT_EQ(str::extname_view(s), s.substr(4));
    }
    SECTION("裸名字带路径路径中有点") {
        std::string s;
        s = "/abc.def/basename.ext";
        ASSERT_EQ(str::extname_view(s), s.substr(17));
    }
    SECTION("裸名字带路径路径中有点+") {
        std::string s;
        s = "/abc.def/";
        ASSERT_EQ(str::extname_view(s), s.substr(9));
    }
    SECTION("basename为隐藏文件，隐藏文件点开头不是只有扩展名") {
        std::string s;
        s = "abc/.hidefile";
        ASSERT_EQ(str::extname_view(s), s.substr(13));
    }
    SECTION("basename为隐藏文件，隐藏文件点开头，但是还有额外扩展名") {
        std::string s;
        s = "abc/.hidefile.ext";
        ASSERT_EQ(str::extname_view(s), s.substr(13));
    }
    SECTION("basename带多个点") {
        std::string s;
        s = "abc/normal-file.xxx.ext";
        ASSERT_EQ(str::extname_view(s), s.substr(19));
    }
    SECTION("basename位置只有点1") {
        std::string s;
        s = "abc/.";
        ASSERT_EQ(str::extname_view(s), s.substr(5));
    }
    SECTION("basename位置只有点2") {
        std::string s;
        s = "..";
        ASSERT_EQ(str::extname_view(s), s.substr(2));
        s = "/..";
        ASSERT_EQ(str::extname_view(s), s.substr(3));
        s = "abc/..";
        ASSERT_EQ(str::extname_view(s), s.substr(6));
    }
    SECTION("basename位置只有点3") {
        std::string s;
        s = "...";
        ASSERT_EQ(str::extname_view(s), s.substr(2));
        s = "/...";
        ASSERT_EQ(str::extname_view(s), s.substr(3));
        s = "abc/...";
        ASSERT_EQ(str::extname_view(s), s.substr(6));
    }
    SECTION("basename位置,点在最后") {
        std::string s;
        s = ".abc.";
        ASSERT_EQ(str::extname_view(s), s.substr(4));
        s = "abc.";
        ASSERT_EQ(str::extname_view(s), s.substr(3));
        s = ".abc..";
        ASSERT_EQ(str::extname_view(s), s.substr(5));
        s = "abc..";
        ASSERT_EQ(str::extname_view(s), s.substr(4));
    }
    SECTION("basename位置,多点前缀1") {
        std::string s;
        s = ".abc";
        ASSERT_EQ(str::extname_view(s), s.substr(4));
        s = "kkk/.abc";
        ASSERT_EQ(str::extname_view(s), s.substr(8));
    }
    SECTION("basename位置,多点前缀1") {
        std::string s;
        s = "..abc";
        ASSERT_EQ(str::extname_view(s), s.substr(1));
    }
    SECTION("basename位置,多点前缀1") {
        std::string s;
        s = "...abc";
        ASSERT_EQ(str::extname_view(s), s.substr(2));
    }
    SECTION("全空白字符") {
        std::string s;
        s = " \t ";
        ASSERT_EQ(str::extname_view(s), s.substr(3));
    }
    SECTION("几个特殊场景") {
        std::string s;
        s = "";
        ASSERT_EQ(str::extname_view(s), s.substr(0));

        s = "/";
        ASSERT_EQ(str::extname_view(s), s.substr(1));
        s = ".";
        ASSERT_EQ(str::extname_view(s), s.substr(1));
        s = "a";
        ASSERT_EQ(str::extname_view(s), s.substr(1));

        s = "ab";
        ASSERT_EQ(str::extname_view(s), s.substr(2));
        s = "a.";
        ASSERT_EQ(str::extname_view(s), s.substr(1));
        s = ".a";
        ASSERT_EQ(str::extname_view(s), s.substr(2));
        s = "..";
        ASSERT_EQ(str::extname_view(s), s.substr(2));
        s = "/.";
        ASSERT_EQ(str::extname_view(s), s.substr(2));
        s = "./";
        ASSERT_EQ(str::extname_view(s), s.substr(2));
        s = "//";
        ASSERT_EQ(str::extname_view(s), s.substr(2));
    }
}
