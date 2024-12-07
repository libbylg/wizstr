#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::dirname+view::basename") {
    SECTION("全路径") {
        REQUIRE(view::dirname("/aaa/bbb/ccc") == "/aaa/bbb");
        REQUIRE(view::basename("/aaa/bbb/ccc") == "ccc");

        REQUIRE(view::remove_dirname("/aaa/bbb/ccc") == "/ccc");
        REQUIRE(view::remove_basename("/aaa/bbb/ccc") == "/aaa/bbb/");

        REQUIRE(view::replace_dirname("/aaa/bbb/ccc", "AAA") == "AAA/ccc");
        REQUIRE(view::replace_basename("/aaa/bbb/ccc", "AAA") == "/aaa/bbb/AAA");

        // REQUIRE(view::split_dirname("/aaa/bbb/ccc") == std::tuple{"/aaa/bbb", "/ccc"});
        // REQUIRE(view::split_basename("/aaa/bbb/ccc") == std::tuple{"/aaa/bbb/", "ccc"});
    }
    SECTION("相对路径[1]") {
        REQUIRE(view::dirname("aaa/bbb/ccc") == "aaa/bbb");
        REQUIRE(view::basename("aaa/bbb/ccc") == "ccc");

        REQUIRE(view::remove_dirname("aaa/bbb/ccc") == "/ccc");
        REQUIRE(view::remove_basename("aaa/bbb/ccc") == "aaa/bbb/");

        REQUIRE(view::replace_dirname("aaa/bbb/ccc", "AAA") == "AAA/ccc");
        REQUIRE(view::replace_basename("aaa/bbb/ccc", "AAA") == "aaa/bbb/AAA");
    }
    SECTION("相对路径[2]") {
        REQUIRE(view::dirname("../bbb/ccc") == "../bbb");
        REQUIRE(view::basename("../bbb/ccc") == "ccc");

        REQUIRE(view::remove_dirname("../bbb/ccc") == "/ccc");
        REQUIRE(view::remove_basename("../bbb/ccc") == "../bbb/");

        REQUIRE(view::replace_dirname("../bbb/ccc", "AAA") == "AAA/ccc");
        REQUIRE(view::replace_basename("../bbb/ccc", "AAA") == "../bbb/AAA");
    }
    SECTION("相对路径[3]") {
        REQUIRE(view::dirname("../ccc") == "..");
        REQUIRE(view::basename("../ccc") == "ccc");

        REQUIRE(view::remove_dirname("../ccc") == "/ccc");
        REQUIRE(view::remove_basename("../ccc") == "../");

        REQUIRE(view::replace_dirname("../ccc", "AAA") == "AAA/ccc");
        REQUIRE(view::replace_basename("../ccc", "AAA") == "../AAA");
    }
    SECTION("相对路径[4]") {
        REQUIRE(view::dirname("./ccc") == ".");
        REQUIRE(view::basename("./ccc") == "ccc");

        REQUIRE(view::remove_dirname("./ccc") == "/ccc");
        REQUIRE(view::remove_basename("./ccc") == "./");

        REQUIRE(view::replace_dirname("./ccc", "AAA") == "AAA/ccc");
        REQUIRE(view::replace_basename("./ccc", "AAA") == "./AAA");
    }
    SECTION("无路径分隔符") {
        REQUIRE(view::dirname("abc") == ".");
        REQUIRE(view::basename("abc") == "abc");

        REQUIRE(view::remove_dirname("abc") == "abc");
        REQUIRE(view::remove_basename("abc") == "");

        REQUIRE(view::replace_dirname("abc", "AAA") == "AAA/abc");
        REQUIRE(view::replace_basename("abc", "AAA") == "AAA");
    }
    SECTION("相对路径特殊表示(.)") {
        REQUIRE(view::dirname(".") == ".");
        REQUIRE(view::basename(".") == ".");

        REQUIRE(view::remove_dirname(".") == ".");
        REQUIRE(view::remove_basename(".") == "");

        REQUIRE(view::replace_dirname(".", "AAA") == "AAA/.");
        REQUIRE(view::replace_basename(".", "AAA") == "AAA");
    }
    SECTION("相对路径特殊表示(..)") {
        REQUIRE(view::dirname("..") == ".");
        REQUIRE(view::basename("..") == "..");

        REQUIRE(view::remove_dirname("..") == "..");
        REQUIRE(view::remove_basename("..") == "");

        REQUIRE(view::replace_dirname("..", "AAA") == "AAA/..");
        REQUIRE(view::replace_basename("..", "AAA") == "AAA");
    }
    SECTION("相对路径特殊表示( .. ):这种不符合一般认识的路径，就应该作为文件处理") {
        REQUIRE(view::dirname(" .. ") == ".");
        REQUIRE(view::basename(" .. ") == " .. ");

        REQUIRE(view::remove_dirname(" .. ") == " .. ");
        REQUIRE(view::remove_basename(" .. ") == "");

        REQUIRE(view::replace_dirname(" .. ", "AAA") == "AAA/ .. ");
        REQUIRE(view::replace_basename(" .. ", "AAA") == "AAA");
    }
    SECTION("相对路径特殊表示( . ):这种不符合一般认识的路径，就应该作为文件处理") {
        REQUIRE(view::dirname(" . ") == ".");
        REQUIRE(view::basename(" . ") == " . ");

        REQUIRE(view::remove_dirname(" . ") == " . ");
        REQUIRE(view::remove_basename(" . ") == "");

        REQUIRE(view::replace_dirname(" . ", "AAA") == "AAA/ . ");
        REQUIRE(view::replace_basename(" . ", "AAA") == "AAA");
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
        REQUIRE(view::dirname("./") == ".");
        REQUIRE(view::basename("./") == "");

        REQUIRE(view::remove_dirname("./") == "/");
        REQUIRE(view::remove_basename("./") == "./");

        REQUIRE(view::replace_dirname("./", "AAA") == "AAA/");
        REQUIRE(view::replace_basename("./", "AAA") == "./AAA");
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
        REQUIRE(view::dirname("../") == "..");
        REQUIRE(view::basename("../") == "");

        REQUIRE(view::remove_dirname("../") == "/");
        REQUIRE(view::remove_basename("../") == "../");

        REQUIRE(view::replace_dirname("../", "AAA") == "AAA/");
        REQUIRE(view::replace_basename("../", "AAA") == "../AAA");
    }
    SECTION("./和../[3]") {
        REQUIRE(view::dirname("./aa") == ".");
        REQUIRE(view::basename("./aa") == "aa");

        REQUIRE(view::remove_dirname("./aa") == "/aa");
        REQUIRE(view::remove_basename("./aa") == "./");

        REQUIRE(view::replace_dirname("./aa", "AAA") == "AAA/aa");
        REQUIRE(view::replace_basename("./aa", "AAA") == "./AAA");
    }
    SECTION("./和../[4]") {
        REQUIRE(view::dirname("../aa") == "..");
        REQUIRE(view::basename("../aa") == "aa");

        REQUIRE(view::remove_dirname("../aa") == "/aa");
        REQUIRE(view::remove_basename("../aa") == "../");

        REQUIRE(view::replace_dirname("../aa", "AAA") == "AAA/aa");
        REQUIRE(view::replace_basename("../aa", "AAA") == "../AAA");
    }
    SECTION("./和../[5]") {
        REQUIRE(view::dirname("../aa/") == "../aa");
        REQUIRE(view::basename("../aa/") == "");

        REQUIRE(view::remove_dirname("../aa/") == "/");
        REQUIRE(view::remove_basename("../aa/") == "../aa/");

        REQUIRE(view::replace_dirname("../aa/", "AAA") == "AAA/");
        REQUIRE(view::replace_basename("../aa/", "AAA") == "../aa/AAA");
    }
    SECTION("./和../[5]") {
        REQUIRE(view::dirname("../aa/..") == "../aa");
        REQUIRE(view::basename("../aa/..") == "..");

        REQUIRE(view::remove_dirname("../aa/..") == "/..");
        REQUIRE(view::remove_basename("../aa/..") == "../aa/");

        REQUIRE(view::replace_dirname("../aa/..", "AAA") == "AAA/..");
        REQUIRE(view::replace_basename("../aa/..", "AAA") == "../aa/AAA");
    }
    SECTION("绝对路径 /") {
        REQUIRE(view::dirname("/") == "/");
        REQUIRE(view::basename("/") == "");

        REQUIRE(view::remove_dirname("/") == "/");
        REQUIRE(view::remove_basename("/") == "/");

        REQUIRE(view::replace_dirname("/", "AAA") == "AAA/");
        REQUIRE(view::replace_basename("/", "AAA") == "/AAA");
    }
    SECTION("空串") {
        REQUIRE(view::dirname("") == ".");
        REQUIRE(view::basename("") == "");

        REQUIRE(view::remove_dirname("") == "");
        REQUIRE(view::remove_basename("") == "");

        REQUIRE(view::replace_dirname("", "AAA") == "AAA/");
        REQUIRE(view::replace_basename("", "AAA") == "AAA");
    }
    SECTION("多余的路径元素[1]") {
        REQUIRE(view::dirname("///ccc") == "/");
        REQUIRE(view::basename("///ccc") == "ccc");

        REQUIRE(view::remove_dirname("///ccc") == "///ccc");
        REQUIRE(view::remove_basename("///ccc") == "///");

        REQUIRE(view::remove_dirname("///ccc") == "///ccc");
        REQUIRE(view::remove_basename("///ccc") == "///");
    }
    SECTION("多余的路径元素[2]") {
        REQUIRE(view::dirname(".///ccc") == ".");
        REQUIRE(view::basename(".///ccc") == "ccc");

        REQUIRE(view::remove_dirname(".///ccc") == "///ccc");
        REQUIRE(view::remove_basename(".///ccc") == ".///");

        REQUIRE(view::replace_dirname(".///ccc", "AAA") == "AAA///ccc");
        REQUIRE(view::replace_basename(".///ccc", "AAA") == ".///AAA");
    }
    SECTION("隐藏文件[1]") {
        REQUIRE(view::dirname(".///.ccc") == ".");
        REQUIRE(view::basename(".///.ccc") == ".ccc");

        REQUIRE(view::remove_dirname(".///.ccc") == "///.ccc");
        REQUIRE(view::remove_basename(".///.ccc") == ".///");

        REQUIRE(view::replace_dirname(".///.ccc", "AAA") == "AAA///.ccc");
        REQUIRE(view::replace_basename(".///.ccc", "AAA") == ".///AAA");
    }
    SECTION("隐藏文件[2]") {
        REQUIRE(view::dirname(".ccc") == ".");
        REQUIRE(view::basename(".ccc") == ".ccc");

        REQUIRE(view::remove_dirname(".ccc") == ".ccc");
        REQUIRE(view::remove_basename(".ccc") == "");

        // REQUIRE(view::replace_dirname(".ccc", "AAA") == ".ccc");
        // REQUIRE(view::replace_basename(".ccc") == "");
    }
}