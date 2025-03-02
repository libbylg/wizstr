#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, dirname_basename) {
    SECTION("全路径") {
        ASSERT_EQ(view::dirname("/aaa/bbb/ccc"), "/aaa/bbb");
        ASSERT_EQ(view::basename("/aaa/bbb/ccc"), "ccc");

        ASSERT_EQ(view::remove_dirname("/aaa/bbb/ccc"), "/ccc");
        ASSERT_EQ(view::remove_basename("/aaa/bbb/ccc"), "/aaa/bbb/");

        ASSERT_EQ(view::replace_dirname("/aaa/bbb/ccc", "AAA"), "AAA/ccc");
        ASSERT_EQ(view::replace_basename("/aaa/bbb/ccc", "AAA"), "/aaa/bbb/AAA");

        // ASSERT_EQ(view::split_dirname("/aaa/bbb/ccc"), std::tuple{"/aaa/bbb", "/ccc"});
        // ASSERT_EQ(view::split_basename("/aaa/bbb/ccc"), std::tuple{"/aaa/bbb/", "ccc"});
    }
    SECTION("相对路径[1]") {
        ASSERT_EQ(view::dirname("aaa/bbb/ccc"), "aaa/bbb");
        ASSERT_EQ(view::basename("aaa/bbb/ccc"), "ccc");

        ASSERT_EQ(view::remove_dirname("aaa/bbb/ccc"), "/ccc");
        ASSERT_EQ(view::remove_basename("aaa/bbb/ccc"), "aaa/bbb/");

        ASSERT_EQ(view::replace_dirname("aaa/bbb/ccc", "AAA"), "AAA/ccc");
        ASSERT_EQ(view::replace_basename("aaa/bbb/ccc", "AAA"), "aaa/bbb/AAA");
    }
    SECTION("相对路径[2]") {
        ASSERT_EQ(view::dirname("../bbb/ccc"), "../bbb");
        ASSERT_EQ(view::basename("../bbb/ccc"), "ccc");

        ASSERT_EQ(view::remove_dirname("../bbb/ccc"), "/ccc");
        ASSERT_EQ(view::remove_basename("../bbb/ccc"), "../bbb/");

        ASSERT_EQ(view::replace_dirname("../bbb/ccc", "AAA"), "AAA/ccc");
        ASSERT_EQ(view::replace_basename("../bbb/ccc", "AAA"), "../bbb/AAA");
    }
    SECTION("相对路径[3]") {
        ASSERT_EQ(view::dirname("../ccc"), "..");
        ASSERT_EQ(view::basename("../ccc"), "ccc");

        ASSERT_EQ(view::remove_dirname("../ccc"), "/ccc");
        ASSERT_EQ(view::remove_basename("../ccc"), "../");

        ASSERT_EQ(view::replace_dirname("../ccc", "AAA"), "AAA/ccc");
        ASSERT_EQ(view::replace_basename("../ccc", "AAA"), "../AAA");
    }
    SECTION("相对路径[4]") {
        ASSERT_EQ(view::dirname("./ccc"), ".");
        ASSERT_EQ(view::basename("./ccc"), "ccc");

        ASSERT_EQ(view::remove_dirname("./ccc"), "/ccc");
        ASSERT_EQ(view::remove_basename("./ccc"), "./");

        ASSERT_EQ(view::replace_dirname("./ccc", "AAA"), "AAA/ccc");
        ASSERT_EQ(view::replace_basename("./ccc", "AAA"), "./AAA");
    }
    SECTION("无路径分隔符") {
        ASSERT_EQ(view::dirname("abc"), ".");
        ASSERT_EQ(view::basename("abc"), "abc");

        ASSERT_EQ(view::remove_dirname("abc"), "abc");
        ASSERT_EQ(view::remove_basename("abc"), "");

        ASSERT_EQ(view::replace_dirname("abc", "AAA"), "AAA/abc");
        ASSERT_EQ(view::replace_basename("abc", "AAA"), "AAA");
    }
    SECTION("相对路径特殊表示(.)") {
        ASSERT_EQ(view::dirname("."), ".");
        ASSERT_EQ(view::basename("."), ".");

        ASSERT_EQ(view::remove_dirname("."), ".");
        ASSERT_EQ(view::remove_basename("."), "");

        ASSERT_EQ(view::replace_dirname(".", "AAA"), "AAA/.");
        ASSERT_EQ(view::replace_basename(".", "AAA"), "AAA");
    }
    SECTION("相对路径特殊表示(..)") {
        ASSERT_EQ(view::dirname(".."), ".");
        ASSERT_EQ(view::basename(".."), "..");

        ASSERT_EQ(view::remove_dirname(".."), "..");
        ASSERT_EQ(view::remove_basename(".."), "");

        ASSERT_EQ(view::replace_dirname("..", "AAA"), "AAA/..");
        ASSERT_EQ(view::replace_basename("..", "AAA"), "AAA");
    }
    SECTION("相对路径特殊表示( .. ):这种不符合一般认识的路径，就应该作为文件处理") {
        ASSERT_EQ(view::dirname(" .. "), ".");
        ASSERT_EQ(view::basename(" .. "), " .. ");

        ASSERT_EQ(view::remove_dirname(" .. "), " .. ");
        ASSERT_EQ(view::remove_basename(" .. "), "");

        ASSERT_EQ(view::replace_dirname(" .. ", "AAA"), "AAA/ .. ");
        ASSERT_EQ(view::replace_basename(" .. ", "AAA"), "AAA");
    }
    SECTION("相对路径特殊表示( . ):这种不符合一般认识的路径，就应该作为文件处理") {
        ASSERT_EQ(view::dirname(" . "), ".");
        ASSERT_EQ(view::basename(" . "), " . ");

        ASSERT_EQ(view::remove_dirname(" . "), " . ");
        ASSERT_EQ(view::remove_basename(" . "), "");

        ASSERT_EQ(view::replace_dirname(" . ", "AAA"), "AAA/ . ");
        ASSERT_EQ(view::replace_basename(" . ", "AAA"), "AAA");
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
        ASSERT_EQ(view::dirname("./"), ".");
        ASSERT_EQ(view::basename("./"), "");

        ASSERT_EQ(view::remove_dirname("./"), "/");
        ASSERT_EQ(view::remove_basename("./"), "./");

        ASSERT_EQ(view::replace_dirname("./", "AAA"), "AAA/");
        ASSERT_EQ(view::replace_basename("./", "AAA"), "./AAA");
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
        ASSERT_EQ(view::dirname("../"), "..");
        ASSERT_EQ(view::basename("../"), "");

        ASSERT_EQ(view::remove_dirname("../"), "/");
        ASSERT_EQ(view::remove_basename("../"), "../");

        ASSERT_EQ(view::replace_dirname("../", "AAA"), "AAA/");
        ASSERT_EQ(view::replace_basename("../", "AAA"), "../AAA");
    }
    SECTION("./和../[3]") {
        ASSERT_EQ(view::dirname("./aa"), ".");
        ASSERT_EQ(view::basename("./aa"), "aa");

        ASSERT_EQ(view::remove_dirname("./aa"), "/aa");
        ASSERT_EQ(view::remove_basename("./aa"), "./");

        ASSERT_EQ(view::replace_dirname("./aa", "AAA"), "AAA/aa");
        ASSERT_EQ(view::replace_basename("./aa", "AAA"), "./AAA");
    }
    SECTION("./和../[4]") {
        ASSERT_EQ(view::dirname("../aa"), "..");
        ASSERT_EQ(view::basename("../aa"), "aa");

        ASSERT_EQ(view::remove_dirname("../aa"), "/aa");
        ASSERT_EQ(view::remove_basename("../aa"), "../");

        ASSERT_EQ(view::replace_dirname("../aa", "AAA"), "AAA/aa");
        ASSERT_EQ(view::replace_basename("../aa", "AAA"), "../AAA");
    }
    SECTION("./和../[5]") {
        ASSERT_EQ(view::dirname("../aa/"), "../aa");
        ASSERT_EQ(view::basename("../aa/"), "");

        ASSERT_EQ(view::remove_dirname("../aa/"), "/");
        ASSERT_EQ(view::remove_basename("../aa/"), "../aa/");

        ASSERT_EQ(view::replace_dirname("../aa/", "AAA"), "AAA/");
        ASSERT_EQ(view::replace_basename("../aa/", "AAA"), "../aa/AAA");
    }
    SECTION("./和../[5]") {
        ASSERT_EQ(view::dirname("../aa/.."), "../aa");
        ASSERT_EQ(view::basename("../aa/.."), "..");

        ASSERT_EQ(view::remove_dirname("../aa/.."), "/..");
        ASSERT_EQ(view::remove_basename("../aa/.."), "../aa/");

        ASSERT_EQ(view::replace_dirname("../aa/..", "AAA"), "AAA/..");
        ASSERT_EQ(view::replace_basename("../aa/..", "AAA"), "../aa/AAA");
    }
    SECTION("绝对路径 /") {
        ASSERT_EQ(view::dirname("/"), "/");
        ASSERT_EQ(view::basename("/"), "");

        ASSERT_EQ(view::remove_dirname("/"), "/");
        ASSERT_EQ(view::remove_basename("/"), "/");

        ASSERT_EQ(view::replace_dirname("/", "AAA"), "AAA/");
        ASSERT_EQ(view::replace_basename("/", "AAA"), "/AAA");
    }
    SECTION("空串") {
        ASSERT_EQ(view::dirname(""), ".");
        ASSERT_EQ(view::basename(""), "");

        ASSERT_EQ(view::remove_dirname(""), "");
        ASSERT_EQ(view::remove_basename(""), "");

        ASSERT_EQ(view::replace_dirname("", "AAA"), "AAA/");
        ASSERT_EQ(view::replace_basename("", "AAA"), "AAA");
    }
    SECTION("多余的路径元素[1]") {
        ASSERT_EQ(view::dirname("///ccc"), "/");
        ASSERT_EQ(view::basename("///ccc"), "ccc");

        ASSERT_EQ(view::remove_dirname("///ccc"), "///ccc");
        ASSERT_EQ(view::remove_basename("///ccc"), "///");

        ASSERT_EQ(view::remove_dirname("///ccc"), "///ccc");
        ASSERT_EQ(view::remove_basename("///ccc"), "///");
    }
    SECTION("多余的路径元素[2]") {
        ASSERT_EQ(view::dirname(".///ccc"), ".");
        ASSERT_EQ(view::basename(".///ccc"), "ccc");

        ASSERT_EQ(view::remove_dirname(".///ccc"), "///ccc");
        ASSERT_EQ(view::remove_basename(".///ccc"), ".///");

        ASSERT_EQ(view::replace_dirname(".///ccc", "AAA"), "AAA///ccc");
        ASSERT_EQ(view::replace_basename(".///ccc", "AAA"), ".///AAA");
    }
    SECTION("隐藏文件[1]") {
        ASSERT_EQ(view::dirname(".///.ccc"), ".");
        ASSERT_EQ(view::basename(".///.ccc"), ".ccc");

        ASSERT_EQ(view::remove_dirname(".///.ccc"), "///.ccc");
        ASSERT_EQ(view::remove_basename(".///.ccc"), ".///");

        ASSERT_EQ(view::replace_dirname(".///.ccc", "AAA"), "AAA///.ccc");
        ASSERT_EQ(view::replace_basename(".///.ccc", "AAA"), ".///AAA");
    }
    SECTION("隐藏文件[2]") {
        ASSERT_EQ(view::dirname(".ccc"), ".");
        ASSERT_EQ(view::basename(".ccc"), ".ccc");

        ASSERT_EQ(view::remove_dirname(".ccc"), ".ccc");
        ASSERT_EQ(view::remove_basename(".ccc"), "");

        // ASSERT_EQ(view::replace_dirname(".ccc", "AAA"), ".ccc");
        // ASSERT_EQ(view::replace_basename(".ccc"), "");
    }
}