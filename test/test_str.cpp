//
// Created by luolijun on 2021/10/7.
//
#include "tester.h"

#include "tiny/re.h"
#include "tiny/str.h"

#ifndef SECTION
#define SECTION(xx)
#endif

TEST(tiny_str, 构造) {
    SECTION("基本构造") {
        tiny::str a("123123123123123123123123123123123");
        EXPECT_EQ(a.size(), 33);
    }

    SECTION("构造并填充") {
        tiny::str b('A', 100);
        EXPECT_EQ(b.size(), 100);
    }
}

TEST(tiny_str, prepend) {
    SECTION("向前追加") {
        tiny::str a("123");
        a.prepend("abc");
        EXPECT_TRUE(a == "abc123");
    }
}

TEST(tiny_str, remove) {
    SECTION("删除中间数据") {
        tiny::str a("abc123");
        a.remove(0);
        EXPECT_TRUE(a == "bc123");
        a.remove(1);
        EXPECT_TRUE(a == "b123");
    }

    SECTION("删除指定的字符") {
        tiny::str a("3bc1233");
        a.remove('3');
        EXPECT_TRUE(a == "bc12");
    }
}

TEST(tiny_str, contains) {
    SECTION("删除中间数据") {
        tiny::str a("3bc1233");
        EXPECT_TRUE(a.contains("12"));
        EXPECT_TRUE(a.contains("33"));
        EXPECT_FALSE(a.contains("31"));
    }
}

TEST(tiny_str, trims) {
    SECTION("前后没有空白") {
        tiny::str a("3bc1233");
        EXPECT_TRUE(a.trim() == "3bc1233");
    }

    SECTION("前后都有空白") {
        tiny::str b("  \tc1233\t  \t  ");
        EXPECT_TRUE(b.trim() == "c1233");
    }
}

TEST(tiny_str, index_of_re) {
    SECTION("找匹配正则表达式的位置") {
        tiny::str a("3bc12def33");
        EXPECT_EQ(a.index_of(tiny::re("b.*[0-9]")), 1);
    }
}

TEST(tiny_str, is_match) {
    SECTION("检查是否匹配某个正则表达式") {
        tiny::str a("3bc12def33");
        EXPECT_FALSE(a.is_match("b.*[0-9]"));
        EXPECT_TRUE(a.is_match("^[0-9]bc[0-9]+[0-9a-z]+$"));
        EXPECT_TRUE(a.is_match(tiny::str("[0-9a-z]+")));
    }
}

TEST(tiny_str, count) {
    SECTION("检查是否匹配某个正则表达式") {
        tiny::str a("3bc12def1233");
        EXPECT_EQ(a.count("12"), 2);
        EXPECT_EQ(a.count(char('3')), 3);
        EXPECT_EQ(a.count("A"), 0);
    }
}

TEST(tiny_str, join) {
    SECTION("简单串联") {
        tiny::str a("/");
        EXPECT_EQ(a.join({"aa", "bb", "cc"}), "aa/bb/cc");
    }
}

TEST(tiny_str, repeat) {
    SECTION("简单字符串重复多次") {
        tiny::str a("0123456789");
        EXPECT_EQ(a.repeat(5), "01234567890123456789012345678901234567890123456789");
    }
}
