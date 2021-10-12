//
// Created by luolijun on 2021/10/7.
//
#include "tester.h"

#include "tiny/bytes.h"
#include "tiny/re.h"

#ifndef SECTION
#define SECTION(xx)
#endif

TEST(tiny_str, construct) {
    SECTION("基本构造") {
        tiny::bytes a("123123123123123123123123123123123");
        EXPECT_EQ(a.size(), 33);
    }

    SECTION("构造并填充") {
        tiny::bytes b('A', 100);
        EXPECT_EQ(b.size(), 100);
    }
}

TEST(tiny_str, prepend) {
    SECTION("向前追加") {
        tiny::bytes a("123");
        a.prepend("abc");
        EXPECT_TRUE(a == "abc123");
    }
}

TEST(tiny_str, zfill) {
    SECTION("正常情况") {
        tiny::bytes a("123");
        EXPECT_TRUE(a.zfill(5) == "00123");
    }
    SECTION("前缀") {
        EXPECT_TRUE(tiny::bytes("+123").zfill(5) == "+0123");
        EXPECT_TRUE(tiny::bytes("-123").zfill(5) == "-0123");
    }
    SECTION("空串") {
        tiny::bytes a("");
        EXPECT_TRUE(a.zfill(5) == "00000");
    }
    SECTION("非数字") {
        EXPECT_EQ(tiny::bytes("LMK").zfill(5), "00LMK");
        EXPECT_TRUE(tiny::bytes("中华人民共和国").zfill(25) == tiny::bytes("0000中华人民共和国"));
    }
    SECTION("宽度太短") {
        EXPECT_TRUE(tiny::bytes("LMK").zfill(3) == "LMK");
        EXPECT_TRUE(tiny::bytes("LMK").zfill(2) == "LMK");
        EXPECT_TRUE(tiny::bytes("LMK").zfill(0) == "LMK");
    }
}

TEST(tiny_str, remove) {
    SECTION("删除中间数据") {
        tiny::bytes a("abc123");
        a.remove(0);
        EXPECT_TRUE(a == "bc123");
        a.remove(1);
        EXPECT_TRUE(a == "b123");
    }

    SECTION("删除指定的字符") {
        tiny::bytes a("3bc1233");
        a.remove('3');
        EXPECT_TRUE(a == "bc12");
    }

    SECTION("删除指定的字符串") {
        tiny::bytes a("33b33c1233");
        a.remove("33");
        EXPECT_TRUE(a == "bc12");
    }

    SECTION("删除正则表达式所匹配的字符串") {
        tiny::bytes a("33b33333c31233");
        a.remove(tiny::re("3+"));
        EXPECT_TRUE(a == "bc12");
    }
}

TEST(tiny_str, contains) {
    SECTION("删除中间数据") {
        tiny::bytes a("3bc1233");
        EXPECT_TRUE(a.contains("12"));
        EXPECT_TRUE(a.contains("33"));
        EXPECT_FALSE(a.contains("31"));
    }
}

TEST(tiny_str, trims) {
    SECTION("前后没有空白") {
        tiny::bytes a("3bc1233");
        EXPECT_TRUE(a.trim() == "3bc1233");
    }

    SECTION("前后都有空白") {
        tiny::bytes b("  \tc1233\t  \t  ");
        EXPECT_TRUE(b.trim() == "c1233");
    }

    SECTION("指定字符") {
        tiny::bytes b("33c1233");
        EXPECT_TRUE(b.trim([](tiny::bytes::value_type ch) -> bool {
            return ch == '3';
        }) == "c12");
    }
}

TEST(tiny_str, index_of_re) {
    SECTION("找匹配正则表达式的位置") {
        tiny::bytes a("3bc12def33");
        EXPECT_EQ(a.index_of(tiny::re("b.*[0-9]")), 1);
    }
}

TEST(tiny_str, is_match) {
    SECTION("检查是否匹配某个正则表达式") {
        tiny::bytes a("3bc12def33");
        EXPECT_FALSE(a.is_match("b.*[0-9]"));
        EXPECT_TRUE(a.is_match("^[0-9]bc[0-9]+[0-9a-z]+$"));
        EXPECT_TRUE(a.is_match(tiny::bytes("[0-9a-z]+")));
    }
}

TEST(tiny_str, is_match_wild) {
    SECTION("检查是否匹配某个正则表达式") {
        tiny::bytes a("3bc12def33");
        EXPECT_TRUE(a.is_match_wild("3*"));
        EXPECT_TRUE(a.is_match_wild("3bc12def33"));
        EXPECT_TRUE(a.is_match_wild("3*de?33"));
    }
}

TEST(tiny_str, count) {
    SECTION("检查是否匹配某个正则表达式") {
        tiny::bytes a("3bc12def1233");
        EXPECT_EQ(a.count("12"), 2);
        EXPECT_EQ(a.count(char('3')), 3);
        EXPECT_EQ(a.count("A"), 0);
    }

    SECTION("统计子串数量") {
        tiny::bytes a("333433343333334");
        EXPECT_EQ(a.count("33"), 5);
        EXPECT_EQ(a.count(""), 16);
        EXPECT_EQ(a.count("333"), 4);
    }
}

TEST(tiny_str, join) {
    SECTION("简单串联") {
        tiny::bytes a("/");
        EXPECT_EQ(a.join({ "aa", "bb", "cc" }), "aa/bb/cc");
    }
}

TEST(tiny_str, repeat) {
    SECTION("简单字符串重复多次") {
        tiny::bytes a("0123456789");
        EXPECT_EQ(a.repeat(5), "01234567890123456789012345678901234567890123456789");
    }
}

TEST(tiny_str, swap_case) {
    SECTION("简单测试") {
        tiny::bytes a("  AabbCC中华人民共和国");
        EXPECT_EQ(a.swap_case(), "  aABBcc中华人民共和国");
    }
}

TEST(tiny_str, simplified) {
    SECTION("简单测试") {
        tiny::bytes a("\t  A abbCC中华人   民共  和 \t国   ");
        EXPECT_EQ(a.simplified(), "A abbCC中华人 民共 和 国");
    }

    SECTION("没什么可以替代的") {
        tiny::bytes a("AabbCC中华人民共和国");
        EXPECT_EQ(a.simplified(), "AabbCC中华人民共和国");
    }

    SECTION("空串") {
        tiny::bytes a("AabbCC中华人民共和国");
        EXPECT_EQ(a.simplified(), "AabbCC中华人民共和国");
    }

    SECTION("全空白") {
        tiny::bytes a("   \t  \n \r \v");
        EXPECT_EQ(a.simplified(), "");
    }
}

TEST(tiny_str, has_prefix) {
    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_TRUE(s.has_prefix("Hello"));
        EXPECT_TRUE(s.has_prefix(tiny::bytes("Hello")));
        EXPECT_TRUE(s.has_prefix('H'));
        EXPECT_TRUE(s.has_prefix("HelloXYZ", 5));
        EXPECT_TRUE(s.has_prefix("", 0));
        EXPECT_TRUE(s.has_prefix(""));
    }

    SECTION("空字符串找前缀") {
        tiny::bytes s("");
        EXPECT_FALSE(s.has_prefix("Hello"));
        EXPECT_FALSE(s.has_prefix(tiny::bytes("Hello")));
        EXPECT_FALSE(s.has_prefix('H'));
        EXPECT_FALSE(s.has_prefix("HelloXYZ", 5));
        EXPECT_TRUE(s.has_prefix("", 0));
        EXPECT_TRUE(s.has_prefix(""));
    }
}

TEST(tiny_str, has_suffix) {
    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_TRUE(s.has_suffix("World"));
        EXPECT_TRUE(s.has_suffix(tiny::bytes("World")));
        EXPECT_TRUE(s.has_suffix('d'));
        EXPECT_TRUE(s.has_suffix("WorldXYZ", 5));
        EXPECT_TRUE(s.has_suffix("", 0));
        EXPECT_TRUE(s.has_suffix(""));
    }
}

TEST(tiny_str, ljust) {
    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.ljust(13, '*'), "HelloWorld***");
    }

    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.ljust(10, '*'), "HelloWorld");
    }

    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.ljust(9, '*'), "HelloWorld");
    }

    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.ljust(9, '*', true), "HelloWorl");
    }
}

TEST(tiny_str, rjust) {
    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.rjust(13, '*'), "***HelloWorld");
    }

    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.rjust(10, '*'), "HelloWorld");
    }

    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.rjust(9, '*'), "HelloWorld");
    }

    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.rjust(9, '*', true), "HelloWorl");
    }
}

TEST(tiny_str, center) {
    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.center(13, '*'), "*HelloWorld**");
    }

    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.center(10, '*'), "HelloWorld");
    }

    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.center(9, '*'), "HelloWorld");
    }

    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.center(9, '*', true), "HelloWorl");
    }
}

TEST(tiny_str, fill) {
    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.fill('*'), "**********");
    }
    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.fill(1, '*', 3), "H***oWorld");
        EXPECT_EQ(s.fill(0, '-', 3), "---*oWorld");
        EXPECT_EQ(s.fill(5, 'A', 5), "---*oAAAAA");
        EXPECT_DEBUG_DEATH(s.fill(5, 'A', 6), "");
    }
}