#include "tester.hpp"

#include "str.hpp"


#include <list>

TEST(test_str, normpath) {
    SECTION("一般场景") {
        ASSERT_EQ(str::normpath("a/b/c/d"), "a/b/c/d");
    }
    SECTION("当前目录") {
        ASSERT_EQ(str::normpath("."), ".");
        ASSERT_EQ(str::normpath("././."), ".");
        ASSERT_EQ(str::normpath("./b/c/d"), "b/c/d");
        ASSERT_EQ(str::normpath("b/././c/./d/."), "b/c/d");
    }
    SECTION("多分隔符处理") {
        ASSERT_EQ(str::normpath("/"), "/");
        ASSERT_EQ(str::normpath("//"), "/");
        ASSERT_EQ(str::normpath("////"), "/");
        ASSERT_EQ(str::normpath("////b/c/d"), "/b/c/d");
        ASSERT_EQ(str::normpath("////b////c//d//"), "/b/c/d");
    }
    SECTION("父目录处理") {
        ASSERT_EQ(str::normpath("a/b/../../../m/../n"), "../n");
        ASSERT_EQ(str::normpath("/../"), "/");
        ASSERT_EQ(str::normpath("/a/../../"), "/");
        ASSERT_EQ(str::normpath("/a/../../../"), "/");
        ASSERT_EQ(str::normpath("a/b/c/d/.."), "a/b/c");
        ASSERT_EQ(str::normpath("a/b/c/d/../m/n"), "a/b/c/m/n");
        ASSERT_EQ(str::normpath("a/b/../../"), ".");
        ASSERT_EQ(str::normpath("a/b/../../"), ".");
    }
    SECTION("空串") {
        ASSERT_EQ(str::normpath(""), ".");
    }
}