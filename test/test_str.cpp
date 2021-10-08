//
// Created by luolijun on 2021/10/7.
//
#include "tester.h"

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
