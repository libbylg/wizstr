//
// Created by libbylg on 2023/6/3.
//
#include "catch_amalgamated.hpp"

#include "tiny/str.h"

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