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

TEST(test_str, dirname_basename) {
    SECTION("全路径") {
        ASSERT_EQ(str::dirname("/aaa/bbb/ccc"), "/aaa/bbb");
        ASSERT_EQ(str::basename("/aaa/bbb/ccc"), "ccc");

        ASSERT_EQ(str::remove_dirname("/aaa/bbb/ccc"), "/ccc");
        ASSERT_EQ(str::remove_basename("/aaa/bbb/ccc"), "/aaa/bbb/");

        ASSERT_EQ(str::replace_dirname("/aaa/bbb/ccc", "AAA"), "AAA/ccc");
        ASSERT_EQ(str::replace_basename("/aaa/bbb/ccc", "AAA"), "/aaa/bbb/AAA");

        ASSERT_EQ(str::split_dirname("/aaa/bbb/ccc"), (std::tuple{"/aaa/bbb", "/ccc"}));
        ASSERT_EQ(str::split_basename("/aaa/bbb/ccc"), (std::tuple{"/aaa/bbb/", "ccc"}));
    }
    SECTION("相对路径[1]") {
        ASSERT_EQ(str::dirname("aaa/bbb/ccc"), "aaa/bbb");
        ASSERT_EQ(str::basename("aaa/bbb/ccc"), "ccc");

        ASSERT_EQ(str::remove_dirname("aaa/bbb/ccc"), "/ccc");
        ASSERT_EQ(str::remove_basename("aaa/bbb/ccc"), "aaa/bbb/");

        ASSERT_EQ(str::replace_dirname("aaa/bbb/ccc", "AAA"), "AAA/ccc");
        ASSERT_EQ(str::replace_basename("aaa/bbb/ccc", "AAA"), "aaa/bbb/AAA");

        ASSERT_EQ(str::split_dirname("aaa/bbb/ccc"), (std::tuple{"aaa/bbb", "/ccc"}));
        ASSERT_EQ(str::split_basename("aaa/bbb/ccc"), (std::tuple{"aaa/bbb/", "ccc"}));
    }
    SECTION("相对路径[2]") {
        ASSERT_EQ(str::dirname("../bbb/ccc"), "../bbb");
        ASSERT_EQ(str::basename("../bbb/ccc"), "ccc");

        ASSERT_EQ(str::remove_dirname("../bbb/ccc"), "/ccc");
        ASSERT_EQ(str::remove_basename("../bbb/ccc"), "../bbb/");

        ASSERT_EQ(str::replace_dirname("../bbb/ccc", "AAA"), "AAA/ccc");
        ASSERT_EQ(str::replace_basename("../bbb/ccc", "AAA"), "../bbb/AAA");

        ASSERT_EQ(str::split_dirname("../bbb/ccc"), (std::tuple{"../bbb", "/ccc"}));
        ASSERT_EQ(str::split_basename("../bbb/ccc"), (std::tuple{"../bbb/", "ccc"}));
    }
    SECTION("相对路径[3]") {
        ASSERT_EQ(str::dirname("../ccc"), "..");
        ASSERT_EQ(str::basename("../ccc"), "ccc");

        ASSERT_EQ(str::remove_dirname("../ccc"), "/ccc");
        ASSERT_EQ(str::remove_basename("../ccc"), "../");

        ASSERT_EQ(str::replace_dirname("../ccc", "AAA"), "AAA/ccc");
        ASSERT_EQ(str::replace_basename("../ccc", "AAA"), "../AAA");

        ASSERT_EQ(str::split_dirname("../ccc"), (std::tuple{"..", "/ccc"}));
        ASSERT_EQ(str::split_basename("../ccc"), (std::tuple{"../", "ccc"}));
    }
    SECTION("相对路径[4]") {
        ASSERT_EQ(str::dirname("./ccc"), ".");
        ASSERT_EQ(str::basename("./ccc"), "ccc");

        ASSERT_EQ(str::remove_dirname("./ccc"), "/ccc");
        ASSERT_EQ(str::remove_basename("./ccc"), "./");

        ASSERT_EQ(str::replace_dirname("./ccc", "AAA"), "AAA/ccc");
        ASSERT_EQ(str::replace_basename("./ccc", "AAA"), "./AAA");

        ASSERT_EQ(str::split_dirname("./ccc"), (std::tuple{".", "/ccc"}));
        ASSERT_EQ(str::split_basename("./ccc"), (std::tuple{"./", "ccc"}));
    }
    SECTION("无路径分隔符") {
        ASSERT_EQ(str::dirname("abc"), ".");
        ASSERT_EQ(str::basename("abc"), "abc");

        ASSERT_EQ(str::remove_dirname("abc"), "abc");
        ASSERT_EQ(str::remove_basename("abc"), "");

        ASSERT_EQ(str::replace_dirname("abc", "AAA"), "AAA/abc");
        ASSERT_EQ(str::replace_basename("abc", "AAA"), "AAA");

        ASSERT_EQ(str::split_dirname("abc"), (std::tuple{"", "abc"}));
        ASSERT_EQ(str::split_basename("abc"), (std::tuple{"", "abc"}));
    }
    SECTION("相对路径特殊表示(.)") {
        ASSERT_EQ(str::dirname("."), ".");
        ASSERT_EQ(str::basename("."), "");

        ASSERT_EQ(str::remove_dirname("."), "");
        ASSERT_EQ(str::remove_basename("."), ".");

        ASSERT_EQ(str::replace_dirname(".", "AAA"), "AAA");
        ASSERT_EQ(str::replace_basename(".", "AAA"), "./AAA");

        ASSERT_EQ(str::split_dirname("."), (std::tuple{".", ""}));
        ASSERT_EQ(str::split_basename("."), (std::tuple{".", ""}));
    }
    SECTION("相对路径特殊表示(..)") {
        ASSERT_EQ(str::dirname(".."), "..");
        ASSERT_EQ(str::basename(".."), "");

        ASSERT_EQ(str::remove_dirname(".."), "");
        ASSERT_EQ(str::remove_basename(".."), "..");

        ASSERT_EQ(str::replace_dirname("..", "AAA"), "AAA");
        ASSERT_EQ(str::replace_basename("..", "AAA"), "../AAA");

        ASSERT_EQ(str::split_dirname(".."), (std::tuple{"..", ""}));
        ASSERT_EQ(str::split_basename(".."), (std::tuple{"..", ""}));
    }
    SECTION("相对路径特殊表示( .. ):这种不符合一般认识的路径，就应该作为文件处理") {
        ASSERT_EQ(str::dirname(" .. "), ".");
        ASSERT_EQ(str::basename(" .. "), " .. ");

        ASSERT_EQ(str::remove_dirname(" .. "), " .. ");
        ASSERT_EQ(str::remove_basename(" .. "), "");

        ASSERT_EQ(str::replace_dirname(" .. ", "AAA"), "AAA/ .. ");
        ASSERT_EQ(str::replace_basename(" .. ", "AAA"), "AAA");

        ASSERT_EQ(str::split_dirname(" .. "), (std::tuple{"", " .. "}));
        ASSERT_EQ(str::split_basename(" .. "), (std::tuple{"", " .. "}));
    }
    SECTION("相对路径特殊表示( . ):这种不符合一般认识的路径，就应该作为文件处理") {
        ASSERT_EQ(str::dirname(" . "), ".");
        ASSERT_EQ(str::basename(" . "), " . ");

        ASSERT_EQ(str::remove_dirname(" . "), " . ");
        ASSERT_EQ(str::remove_basename(" . "), "");

        ASSERT_EQ(str::replace_dirname(" . ", "AAA"), "AAA/ . ");
        ASSERT_EQ(str::replace_basename(" . ", "AAA"), "AAA");

        ASSERT_EQ(str::split_dirname(" . "), (std::tuple{"", " . "}));
        ASSERT_EQ(str::split_basename(" . "), (std::tuple{"", " . "}));
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
        ASSERT_EQ(str::dirname("./"), ".");
        ASSERT_EQ(str::basename("./"), "");

        ASSERT_EQ(str::remove_dirname("./"), "/");
        ASSERT_EQ(str::remove_basename("./"), "./");

        ASSERT_EQ(str::replace_dirname("./", "AAA"), "AAA/");
        ASSERT_EQ(str::replace_basename("./", "AAA"), "./AAA");

        ASSERT_EQ(str::split_dirname("./"), (std::tuple{".", "/"}));
        ASSERT_EQ(str::split_basename("./"), (std::tuple{"./", ""}));
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
        ASSERT_EQ(str::dirname("../"), "..");
        ASSERT_EQ(str::basename("../"), "");

        ASSERT_EQ(str::remove_dirname("../"), "/");
        ASSERT_EQ(str::remove_basename("../"), "../");

        ASSERT_EQ(str::replace_dirname("../", "AAA"), "AAA/");
        ASSERT_EQ(str::replace_basename("../", "AAA"), "../AAA");

        ASSERT_EQ(str::split_dirname("../"), (std::tuple{"..", "/"}));
        ASSERT_EQ(str::split_basename("../"), (std::tuple{"../", ""}));
    }
    SECTION("./和../[3]") {
        ASSERT_EQ(str::dirname("./aa"), ".");
        ASSERT_EQ(str::basename("./aa"), "aa");

        ASSERT_EQ(str::remove_dirname("./aa"), "/aa");
        ASSERT_EQ(str::remove_basename("./aa"), "./");

        ASSERT_EQ(str::replace_dirname("./aa", "AAA"), "AAA/aa");
        ASSERT_EQ(str::replace_basename("./aa", "AAA"), "./AAA");

        ASSERT_EQ(str::split_dirname("./aa"), (std::tuple{".", "/aa"}));
        ASSERT_EQ(str::split_basename("./aa"), (std::tuple{"./", "aa"}));
    }
    SECTION("./和../[4]") {
        ASSERT_EQ(str::dirname("../aa"), "..");
        ASSERT_EQ(str::basename("../aa"), "aa");

        ASSERT_EQ(str::remove_dirname("../aa"), "/aa");
        ASSERT_EQ(str::remove_basename("../aa"), "../");

        ASSERT_EQ(str::replace_dirname("../aa", "AAA"), "AAA/aa");
        ASSERT_EQ(str::replace_basename("../aa", "AAA"), "../AAA");

        ASSERT_EQ(str::split_dirname("../aa"), (std::tuple{"..", "/aa"}));
        ASSERT_EQ(str::split_basename("../aa"), (std::tuple{"../", "aa"}));
    }
    SECTION("./和../[5]") {
        ASSERT_EQ(str::dirname("../aa/"), "../aa");
        ASSERT_EQ(str::basename("../aa/"), "");

        ASSERT_EQ(str::remove_dirname("../aa/"), "/");
        ASSERT_EQ(str::remove_basename("../aa/"), "../aa/");

        ASSERT_EQ(str::replace_dirname("../aa/", "AAA"), "AAA/");
        ASSERT_EQ(str::replace_basename("../aa/", "AAA"), "../aa/AAA");

        ASSERT_EQ(str::split_dirname("../aa/"), (std::tuple{"../aa", "/"}));
        ASSERT_EQ(str::split_basename("../aa/"), (std::tuple{"../aa/", ""}));
    }
    SECTION("./和../[5]") {
        ASSERT_EQ(str::dirname("../aa/.."), "../aa/..");
        ASSERT_EQ(str::basename("../aa/.."), "");

        ASSERT_EQ(str::remove_dirname("../aa/.."), "");
        ASSERT_EQ(str::remove_basename("../aa/.."), "../aa/..");

        ASSERT_EQ(str::replace_dirname("../aa/..", "AAA"), "AAA");
        ASSERT_EQ(str::replace_basename("../aa/..", "AAA"), "../aa/../AAA");

        ASSERT_EQ(str::split_dirname("../aa/.."), (std::tuple{"../aa/..", ""}));
        ASSERT_EQ(str::split_basename("../aa/.."), (std::tuple{"../aa/..", ""}));
    }
    SECTION("绝对路径 /") {
        ASSERT_EQ(str::dirname("/"), "/");
        ASSERT_EQ(str::basename("/"), "");

        ASSERT_EQ(str::remove_dirname("/"), "");
        ASSERT_EQ(str::remove_basename("/"), "/");

        ASSERT_EQ(str::replace_dirname("/", "AAA"), "AAA");
        ASSERT_EQ(str::replace_basename("/", "AAA"), "/AAA");

        ASSERT_EQ(str::split_dirname("/"), (std::tuple{"/", ""}));
        ASSERT_EQ(str::split_basename("/"), (std::tuple{"/", ""}));
    }
    SECTION("空串") {
        ASSERT_EQ(str::dirname(""), ".");
        ASSERT_EQ(str::basename(""), "");

        ASSERT_EQ(str::remove_dirname(""), "");
        ASSERT_EQ(str::remove_basename(""), "");

        ASSERT_EQ(str::replace_dirname("", "AAA"), "AAA");
        ASSERT_EQ(str::replace_basename("", "AAA"), "AAA");

        ASSERT_EQ(str::split_dirname(""), (std::tuple{"", ""}));
        ASSERT_EQ(str::split_basename(""), (std::tuple{"", ""}));
    }
    SECTION("多余的路径元素[1]") {
        ASSERT_EQ(str::dirname("///ccc"), "/");
        ASSERT_EQ(str::basename("///ccc"), "ccc");

        ASSERT_EQ(str::remove_dirname("///ccc"), "//ccc");
        ASSERT_EQ(str::remove_basename("///ccc"), "///");

        ASSERT_EQ(str::replace_dirname("///ccc", "AAA"), "AAA//ccc");
        ASSERT_EQ(str::replace_basename("///ccc", "AAA"), "///AAA");

        ASSERT_EQ(str::split_dirname("///ccc"), (std::tuple{"/", "//ccc"}));
        ASSERT_EQ(str::split_basename("///ccc"), (std::tuple{"///", "ccc"}));
    }
    SECTION("多余的路径元素[2]") {
        ASSERT_EQ(str::dirname(".///ccc"), ".");
        ASSERT_EQ(str::basename(".///ccc"), "ccc");

        ASSERT_EQ(str::remove_dirname(".///ccc"), "///ccc");
        ASSERT_EQ(str::remove_basename(".///ccc"), ".///");

        ASSERT_EQ(str::replace_dirname(".///ccc", "AAA"), "AAA///ccc");
        ASSERT_EQ(str::replace_basename(".///ccc", "AAA"), ".///AAA");

        ASSERT_EQ(str::split_dirname(".///ccc"), (std::tuple{".", "///ccc"}));
        ASSERT_EQ(str::split_basename(".///ccc"), (std::tuple{".///", "ccc"}));
    }
    SECTION("隐藏文件[1]") {
        ASSERT_EQ(str::dirname(".///.ccc"), ".");
        ASSERT_EQ(str::basename(".///.ccc"), ".ccc");

        ASSERT_EQ(str::remove_dirname(".///.ccc"), "///.ccc");
        ASSERT_EQ(str::remove_basename(".///.ccc"), ".///");

        ASSERT_EQ(str::replace_dirname(".///.ccc", "AAA"), "AAA///.ccc");
        ASSERT_EQ(str::replace_basename(".///.ccc", "AAA"), ".///AAA");

        ASSERT_EQ(str::split_dirname(".///.ccc"), (std::tuple{".", "///.ccc"}));
        ASSERT_EQ(str::split_basename(".///.ccc"), (std::tuple{".///", ".ccc"}));
    }
    SECTION("隐藏文件[2]") {
        ASSERT_EQ(str::dirname(".ccc"), ".");
        ASSERT_EQ(str::basename(".ccc"), ".ccc");

        ASSERT_EQ(str::remove_dirname(".ccc"), ".ccc");
        ASSERT_EQ(str::remove_basename(".ccc"), "");

        // ASSERT_EQ(str::replace_dirname(".ccc", "AAA"), ".ccc");
        // ASSERT_EQ(str::replace_basename(".ccc"), "");

        ASSERT_EQ(str::split_dirname(".ccc"), (std::tuple{"", ".ccc"}));
        ASSERT_EQ(str::split_basename(".ccc"), (std::tuple{"", ".ccc"}));
    }
}