//
// Created by libbylg on 2023/6/3.
//
#include "tiny/str.h"

#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

TEST_CASE("split_list", "as vector") {
    SECTION("一般情况") {
        auto items = str::split_list("a,b,c", ',');
        REQUIRE(items.size() == 3u);
        REQUIRE(items[0] == "a");
        REQUIRE(items[1] == "b");
        REQUIRE(items[2] == "c");
    }
    SECTION("全空情况") {
        auto items = str::split_list(",,", ',');
        REQUIRE(items.size() == 3u);
        REQUIRE(items[0] == "");
        REQUIRE(items[1] == "");
        REQUIRE(items[2] == "");
    }
    SECTION("没有替换分个符") {
        auto items = str::split_list("abc", ',');
        REQUIRE(items.size() == 1u);
        REQUIRE(items[0] == "abc");
    }
    SECTION("分隔符号在开头") {
        auto items = str::split_list(",abc", ',');
        REQUIRE(items.size() == 2u);
        REQUIRE(items[0] == "");
        REQUIRE(items[1] == "abc");
    }
    SECTION("分隔符号在结尾") {
        auto items = str::split_list("abc,", ',');
        REQUIRE(items.size() == 2u);
        REQUIRE(items[0] == "abc");
        REQUIRE(items[1] == "");
    }
}

// TEST(tiny_bytes, prepend) {
//     SECTION("向前追加") {
//         tiny::bytes a("123");
//         a.prepend("abc");
//         EXPECT_TRUE(a == "abc123");
//     }
// }
//
// TEST(tiny_bytes, zfill) {
//     SECTION("正常情况") {
//         tiny::bytes a("123");
//         EXPECT_TRUE(a.zfill(5) == "00123");
//     }
//     SECTION("前缀") {
//         EXPECT_TRUE(tiny::bytes("+123").zfill(5) == "+0123");
//         EXPECT_TRUE(tiny::bytes("-123").zfill(5) == "-0123");
//     }
//     SECTION("空串") {
//         tiny::bytes a("");
//         EXPECT_TRUE(a.zfill(5) == "00000");
//     }
//     SECTION("非数字") {
//         EXPECT_EQ(tiny::bytes("LMK").zfill(5), "00LMK");
//         EXPECT_TRUE(tiny::bytes("中华人民共和国").zfill(25) == tiny::bytes("0000中华人民共和国"));
//     }
//     SECTION("宽度太短") {
//         EXPECT_TRUE(tiny::bytes("LMK").zfill(3) == "LMK");
//         EXPECT_TRUE(tiny::bytes("LMK").zfill(2) == "LMK");
//         EXPECT_TRUE(tiny::bytes("LMK").zfill(0) == "LMK");
//     }
// }
//
// TEST(tiny_bytes, remove) {
//     SECTION("删除中间数据") {
//         tiny::bytes a("abc123");
//         a.remove(0);
//         EXPECT_TRUE(a == "bc123");
//         a.remove(1);
//         EXPECT_TRUE(a == "b123");
//     }
//
//     SECTION("删除指定的字符") {
//         tiny::bytes a("3bc1233");
//         a.remove('3');
//         EXPECT_TRUE(a == "bc12");
//     }
//
//     SECTION("删除指定的字符串") {
//         tiny::bytes a("33b33c1233");
//         a.remove("33");
//         EXPECT_TRUE(a == "bc12");
//     }
// }
//
// TEST(tiny_bytes, contains) {
//     SECTION("删除中间数据") {
//         tiny::bytes a("3bc1233");
//         EXPECT_TRUE(a.contains("12"));
//         EXPECT_TRUE(a.contains("33"));
//         EXPECT_FALSE(a.contains("31"));
//     }
// }
//
// TEST(tiny_bytes, trims) {
//     SECTION("前后没有空白") {
//         tiny::bytes a("3bc1233");
//         EXPECT_TRUE(a.trim() == "3bc1233");
//     }
//
//     SECTION("前后都有空白") {
//         tiny::bytes b("  \tc1233\t  \t  ");
//         EXPECT_TRUE(b.trim() == "c1233");
//     }
//
//     SECTION("指定字符") {
//         tiny::bytes b("33c1233");
//         EXPECT_TRUE(b.trim([](tiny::bytes::value_type ch) -> bool {
//             return ch == '3';
//         }) == "c12");
//     }
// }
//
// TEST(tiny_bytes, is_match_wild) {
//     SECTION("检查是否匹配某个正则表达式") {
//         tiny::bytes a("3bc12def33");
//         EXPECT_TRUE(a.is_match_wild("3*"));
//         EXPECT_TRUE(a.is_match_wild("3bc12def33"));
//         EXPECT_TRUE(a.is_match_wild("3*de?33"));
//     }
// }
//
// TEST(tiny_bytes, count) {
//     SECTION("检查是否匹配某个正则表达式") {
//         tiny::bytes a("3bc12def1233");
//         EXPECT_EQ(a.count("12"), 2);
//         EXPECT_EQ(a.count(char('3')), 3);
//         EXPECT_EQ(a.count("A"), 0);
//     }
//
//     SECTION("统计子串数量") {
//         tiny::bytes a("333433343333334");
//         EXPECT_EQ(a.count("33"), 5);
//         EXPECT_EQ(a.count(""), 16);
//         EXPECT_EQ(a.count("333"), 4);
//     }
// }
//
// TEST(tiny_bytes, join) {
//     SECTION("简单串联") {
//         tiny::bytes a("/");
//         EXPECT_EQ(a.join({ "aa", "bb", "cc" }), "aa/bb/cc");
//     }
// }
//
// TEST(tiny_bytes, repeat) {
//     SECTION("简单字符串重复多次") {
//         tiny::bytes a("0123456789");
//         EXPECT_EQ(a.repeat(5), "01234567890123456789012345678901234567890123456789");
//     }
// }
//
// TEST(tiny_bytes, swap_case) {
//     SECTION("简单测试") {
//         tiny::bytes a("  AabbCC中华人民共和国");
//         EXPECT_EQ(a.swap_case(), "  aABBcc中华人民共和国");
//     }
// }

TEST_CASE("simplified-reference", "") {
    SECTION("简单测试") {
        std::string a("\t  A abbCC中华人   民共  和 \t国   ");
        str::simplified(a);
        REQUIRE(a == "A abbCC中华人 民共 和 国");
    }

    SECTION("没什么可以替代的") {
        std::string a("AabbCC中华人民共和国");
        REQUIRE(str::simplified(a) == "AabbCC中华人民共和国");
    }

    SECTION("空串") {
        std::string a("");
        REQUIRE(str::simplified(a) == "");
    }

    SECTION("全空白") {
        std::string a("   \t  \n \r \v");
        REQUIRE(str::simplified(a) == "");
    }
}

TEST_CASE("simplified-copy", "") {
    SECTION("简单测试") {
        REQUIRE(str::simplified("\t  A abbCC中华人   民共  和 \t国   ") == "A abbCC中华人 民共 和 国");
    }

    SECTION("没什么可以替代的") {
        REQUIRE(str::simplified("AabbCC中华人民共和国") == "AabbCC中华人民共和国");
    }

    SECTION("空串") {
        REQUIRE(str::simplified("") == "");
    }

    SECTION("全空白") {
        REQUIRE(str::simplified("   \t  \n \r \v") == "");
    }
}

TEST_CASE("dirname") {
    SECTION("全路径") {
        REQUIRE(str::dirname("/aaa/bbb/ccc") == "/aaa/bbb");
    }
    SECTION("相对路径") {
        REQUIRE(str::dirname("aaa/bbb/ccc") == "aaa/bbb");
        REQUIRE(str::dirname("../bbb/ccc") == "../bbb");
        REQUIRE(str::dirname("../ccc") == "..");
        REQUIRE(str::dirname("./ccc") == ".");
    }
    SECTION(".和..") {
        REQUIRE(str::dirname(".") == ".");
        REQUIRE(str::dirname("..") == ".");
    }
    SECTION("无路径分隔符") {
        REQUIRE(str::dirname("abc") == ".");
    }
    SECTION("./和../") {
        REQUIRE(str::dirname("./") == ".");
        REQUIRE(str::dirname("../") == ".");
        REQUIRE(str::dirname("./aa") == ".");
        REQUIRE(str::dirname("../aa") == "..");
    }
    SECTION("绝对路径 /") {
        REQUIRE(str::dirname("/") == "/");
    }
    SECTION("空串") {
        REQUIRE(str::dirname("") == ".");
    }
    SECTION("多余的路径元素") {
        REQUIRE(str::dirname("///ccc") == "/");
        REQUIRE(str::dirname(".///ccc") == ".");
    }
}

TEST_CASE("has_prefix") {
    SECTION("简单测试") {
        std::string s("HelloWorld");
        REQUIRE(str::has_prefix(s, "Hello"));
        REQUIRE(str::has_prefix(s, std::string("Hello")));
        REQUIRE(str::has_prefix(s, 'H'));
        REQUIRE(str::has_prefix(s, { "HelloXYZ", 5 }));
        REQUIRE(str::has_prefix(s, { "", 0 }));
        REQUIRE(str::has_prefix(s, ""));
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

// TEST(tiny_bytes, has_suffix) {
//     SECTION("简单测试") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_TRUE(s.has_suffix("World"));
//         EXPECT_TRUE(s.has_suffix(tiny::bytes("World")));
//         EXPECT_TRUE(s.has_suffix('d'));
//         EXPECT_TRUE(s.has_suffix("WorldXYZ", 5));
//         EXPECT_TRUE(s.has_suffix("", 0));
//         EXPECT_TRUE(s.has_suffix(""));
//     }
// }
//
// TEST(tiny_bytes, ljust) {
//     SECTION("简单测试") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.ljust(13, '*'), "HelloWorld***");
//     }
//
//     SECTION("简单测试") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.ljust(10, '*'), "HelloWorld");
//     }
//
//     SECTION("简单测试") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.ljust(9, '*'), "HelloWorld");
//     }
//
//     SECTION("简单测试") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.ljust(9, '*', true), "HelloWorl");
//     }
// }
//
// TEST(tiny_bytes, rjust) {
//     SECTION("简单测试") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.rjust(13, '*'), "***HelloWorld");
//     }
//
//     SECTION("简单测试") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.rjust(10, '*'), "HelloWorld");
//     }
//
//     SECTION("简单测试") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.rjust(9, '*'), "HelloWorld");
//     }
//
//     SECTION("简单测试") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.rjust(9, '*', true), "HelloWorl");
//     }
// }
//
// TEST(tiny_bytes, center) {
//     SECTION("简单测试") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.center(13, '*'), "*HelloWorld**");
//     }
//
//     SECTION("简单测试") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.center(10, '*'), "HelloWorld");
//     }
//
//     SECTION("简单测试") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.center(9, '*'), "HelloWorld");
//     }
//
//     SECTION("简单测试") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.center(9, '*', true), "HelloWorl");
//     }
// }

TEST_CASE("fill") {
    SECTION("简单测试") {
        std::string s("HelloWorld");
        REQUIRE(str::fill(s, '*') == "**********");
    }
    SECTION("简单测试") {
        tiny::bytes s("HelloWorld");
        EXPECT_EQ(s.fill(1, '*', 3), "H***oWorld");
        EXPECT_EQ(s.fill(0, '-', 3), "---*oWorld");
        EXPECT_EQ(s.fill(5, 'A', 5), "---*oAAAAA");
        EXPECT_DEBUG_DEATH(s.fill(5, 'A', 6), "");
    }
}

// TEST(tiny_bytes, invert) {
//     SECTION("全部颠倒") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.invert(), "dlroWolleH");
//     }
//     SECTION("部分颠倒") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.invert(2, 3), "HeollWorld");
//     }
//     SECTION("指定位置之后的颠倒") {
//         tiny::bytes s("HelloWorld");
//         EXPECT_EQ(s.invert(5), "HellodlroW");
//     }
// }
//
// TEST(tiny_bytes, is_bool) {
//     SECTION("简单场景") {
//         EXPECT_TRUE(tiny::bytes("1").is_bool());
//         EXPECT_TRUE(tiny::bytes("0").is_bool());
//         EXPECT_TRUE(tiny::bytes("on").is_bool());
//         EXPECT_TRUE(tiny::bytes("off").is_bool());
//         EXPECT_TRUE(tiny::bytes("ON").is_bool());
//         EXPECT_TRUE(tiny::bytes("OFF").is_bool());
//         EXPECT_TRUE(tiny::bytes("Yes").is_bool());
//         EXPECT_TRUE(tiny::bytes("No").is_bool());
//         EXPECT_TRUE(tiny::bytes("yes").is_bool());
//         EXPECT_TRUE(tiny::bytes("no").is_bool());
//         EXPECT_TRUE(tiny::bytes("YES").is_bool());
//         EXPECT_TRUE(tiny::bytes("NO").is_bool());
//         EXPECT_TRUE(tiny::bytes("True").is_bool());
//         EXPECT_TRUE(tiny::bytes("False").is_bool());
//         EXPECT_TRUE(tiny::bytes("true").is_bool());
//         EXPECT_TRUE(tiny::bytes("false").is_bool());
//         EXPECT_TRUE(tiny::bytes("TRUE").is_bool());
//         EXPECT_TRUE(tiny::bytes("FALSE").is_bool());
//     }
// }
//
// TEST(tiny_bytes, expand_tabs) {
//     SECTION("简单场景") {
//         tiny::bytes a("1\t123\t1234\t12345\t123456\t$");
//         EXPECT_EQ(a.expand_tabs(4), "1   123 1234    12345   123456  $");
//     }
// }
//
// TEST(tiny_bytes, to_int8) {
//     tiny::bytes a("1111111");
//     EXPECT_EQ(a.to_int8(nullptr, 2), 127);
// }
//
// TEST(tiny_bytes, from) {
//     EXPECT_EQ(tiny::bytes::from(int8_t(-1), 2), "11111111");
//     EXPECT_EQ(tiny::bytes::from(int16_t(-1), 2), "1111111111111111");
//     EXPECT_EQ(tiny::bytes::from(int32_t(-1), 2), "11111111111111111111111111111111");
//     EXPECT_EQ(tiny::bytes::from(int64_t(-1), 2), "1111111111111111111111111111111111111111111111111111111111111111");
//
//     EXPECT_EQ(tiny::bytes::from(uint8_t(-1), 2), "11111111");
//     EXPECT_EQ(tiny::bytes::from(uint16_t(-1), 2), "1111111111111111");
//     EXPECT_EQ(tiny::bytes::from(uint32_t(-1), 2), "11111111111111111111111111111111");
//     EXPECT_EQ(tiny::bytes::from(uint64_t(-1), 2), "1111111111111111111111111111111111111111111111111111111111111111");
//
//     EXPECT_EQ(tiny::bytes::from(int8_t(-1), 10), "-1");
//     EXPECT_EQ(tiny::bytes::from(int16_t(-1), 10), "-1");
//     EXPECT_EQ(tiny::bytes::from(int32_t(-1), 10), "-1");
//     EXPECT_EQ(tiny::bytes::from(int64_t(-1), 10), "-1");
//
//     EXPECT_EQ(tiny::bytes::from(uint8_t(-1), 10), "255");
//     EXPECT_EQ(tiny::bytes::from(uint16_t(-1), 10), "65535");
//     EXPECT_EQ(tiny::bytes::from(uint32_t(-1), 10), "4294967295");
//     EXPECT_EQ(tiny::bytes::from(uint64_t(-1), 10), "18446744073709551615");
//
//     EXPECT_EQ(tiny::bytes::from(uint8_t(-1), 16), "ff");
//     EXPECT_EQ(tiny::bytes::from(uint16_t(-1), 16), "ffff");
//     EXPECT_EQ(tiny::bytes::from(uint32_t(-1), 16), "ffffffff");
//     EXPECT_EQ(tiny::bytes::from(uint64_t(-1), 16), "ffffffffffffffff");
// }
//
// TEST(tiny_bytes, split_by_char) {
//     tiny::bytes a("HelloWorld-HelloWorld");
//     std::vector<tiny::bytes> v;
//     a.split('-', [&v](tiny::bytes::const_pointer s, tiny::bytes::size_type n) -> int {
//         v.emplace_back(s, n);
//         return 0;
//     });
//     EXPECT_EQ(v.size(), 2);
//     EXPECT_EQ(v[0], "HelloWorld");
//     EXPECT_EQ(v[1], "HelloWorld");
//     std::vector<tiny::bytes> p{ tiny::bytes{ "HelloWorld" }, tiny::bytes{ "HelloWorld" } };
//     EXPECT_EQ(v, p);
// }
//
// TEST(tiny_bytes, find_first_of_test) {
//     tiny::bytes a("hello world");
//     ASSERT(a.find_first_of("wr", 0, 100) == 6);
//     ASSERT(a.find_first_of("mkf", 0, 100) == tiny::bytes::npos);
// }
//
// TEST(tiny_bytes, c_str) {
//     SECTION("短字符串") {
//         tiny::bytes a("hello world");
//         ASSERT_TRUE(strcmp(a.c_str(), "hello world") == 0);
//         ASSERT_TRUE(a.data() == a.c_str());
//     }
//
//     SECTION("长字符串") {
//         const char* cb = "hello worldhello worldhello worldhello worldhello worldhello worldhello world";
//         tiny::bytes b(cb);
//         ASSERT_TRUE(strcmp(b.c_str(), cb) == 0);
//         ASSERT_TRUE(b.data() == b.c_str());
//     }
//
//     SECTION("空字符串1") {
//         tiny::bytes b;
//         ASSERT_TRUE(strcmp(b.c_str(), "") == 0);
//         ASSERT_TRUE(b.data() == b.c_str());
//     }
//
//     SECTION("空字符串2") {
//         tiny::bytes b;
//         b.append("");
//         ASSERT_TRUE(strcmp(b.c_str(), "") == 0);
//         ASSERT_TRUE(b.data() == b.c_str());
//     }
// }
//
// TEST(tiny_bytes, attach) {
//     char buff[100] = "abc";
//     tiny::bytes b("12345678901234567890");
//     const char* olddata = b.data();
//     ASSERT_EQ(b, "12345678901234567890");
//
//     b.attach(buff, 3, 100);
//     ASSERT_NE(b.data(), olddata);
//     ASSERT_EQ(b, "abc");
//     ASSERT_EQ(b.size(), 3);
//     ASSERT_EQ(b.capacity(), 100);
//
//     b.detach();
//     ASSERT_EQ(b.size(), 0);
// }
//
// TEST(tiny_bytes, expand) {
//     SECTION("简单扩展") {
//         tiny::bytes b("${HOME}/${NOTEXIST}");
//         tiny::bytes result = b.expand_envs();
//         ASSERT_EQ(result, tiny::bytes(getenv("HOME")) + "/");
//     }
//
//     SECTION("简单扩展") {
//         tiny::bytes b("$HOME/${NOTEXIST}");
//         tiny::bytes result = b.expand_envs();
//         ASSERT_EQ(result, tiny::bytes(getenv("HOME")) + "/");
//     }
//
//     SECTION("简单扩展") {
//         tiny::bytes b("abcd$HOME/${NOTEXIST}");
//         tiny::bytes result = b.expand_envs();
//         ASSERT_EQ(result, "abcd" + tiny::bytes(getenv("HOME")) + "/");
//     }
//
//     SECTION("简单扩展") {
//         tiny::bytes b("abcd${HOME}");
//         tiny::bytes result = b.expand_envs();
//         ASSERT_EQ(result, "abcd" + tiny::bytes(getenv("HOME")));
//     }
//
//     SECTION("简单扩展") {
//         tiny::bytes b("$HOME");
//         tiny::bytes result = b.expand_envs();
//         ASSERT_EQ(result, tiny::bytes(getenv("HOME")));
//     }
//
//     SECTION("简单扩展") {
//         tiny::bytes b("$$$");
//         tiny::bytes result = b.expand_envs();
//         ASSERT_EQ(result, "$$$");
//     }
//
//     SECTION("简单扩展") {
//         tiny::bytes b("");
//         tiny::bytes result = b.expand_envs();
//         ASSERT_EQ(result, "");
//     }
//
//     SECTION("简单扩展") {
//         tiny::bytes b("$");
//         tiny::bytes result = b.expand_envs();
//         ASSERT_EQ(result, "$");
//     }
//
//     SECTION("简单扩展") {
//         tiny::bytes b("abc$");
//         tiny::bytes result = b.expand_envs();
//         ASSERT_EQ(result, "abc$");
//     }
// }
//
// TEST(tiny_bytes, prefix_of) {
//     SECTION("首个参数不同") {
//         ASSERT_EQ(0, tiny::bytes::prefix_of(tiny::bytes{ "abc" }, tiny::bytes{ "def" }));
//     }
//     SECTION("一个字符相同") {
//         ASSERT_EQ(1, tiny::bytes::prefix_of(tiny::bytes{ "abca" }, tiny::bytes{ "aef" }));
//     }
//     SECTION("所有字符相同") {
//         ASSERT_EQ(3, tiny::bytes::prefix_of(tiny::bytes{ "abc" }, tiny::bytes{ "abc" }));
//     }
//     SECTION("前缀相同，但是长度不同1") {
//         ASSERT_EQ(3, tiny::bytes::prefix_of(tiny::bytes{ "abc" }, tiny::bytes{ "abcdef" }));
//     }
//     SECTION("前缀相同，但是长度不同2") {
//         ASSERT_EQ(3, tiny::bytes::prefix_of(tiny::bytes{ "abcdef" }, tiny::bytes{ "abc" }));
//     }
//     SECTION("前缀相同，但是长度不同2") {
//         ASSERT_EQ(3, tiny::bytes::prefix_of(tiny::bytes{ "abcdef" }, tiny::bytes{ "abcmpqwewew" }));
//     }
//     SECTION("空字符串") {
//         ASSERT_EQ(0, tiny::bytes::prefix_of(tiny::bytes{ "" }, tiny::bytes{ "" }));
//     }
//     SECTION("空字符串2") {
//         ASSERT_EQ(0, tiny::bytes::prefix_of(tiny::bytes{ "" }, tiny::bytes{ "abc" }));
//     }
// }
//
// TEST(tiny_bytes, suffix_of) {
//     SECTION("首个参数不同") {
//         ASSERT_EQ(0, tiny::bytes::suffix_of(tiny::bytes{ "abc" }, tiny::bytes{ "def" }));
//     }
//     SECTION("一个字符相同") {
//         ASSERT_EQ(1, tiny::bytes::suffix_of(tiny::bytes{ "bca" }, tiny::bytes{ "efa" }));
//     }
//     SECTION("所有字符相同") {
//         ASSERT_EQ(3, tiny::bytes::suffix_of(tiny::bytes{ "abc" }, tiny::bytes{ "abc" }));
//     }
//     SECTION("前缀相同，但是长度不同1") {
//         ASSERT_EQ(3, tiny::bytes::suffix_of(tiny::bytes{ "abc" }, tiny::bytes{ "defabc" }));
//     }
//     SECTION("前缀相同，但是长度不同2") {
//         ASSERT_EQ(3, tiny::bytes::suffix_of(tiny::bytes{ "defabc" }, tiny::bytes{ "abc" }));
//     }
//     SECTION("前缀相同，但是长度不同2") {
//         ASSERT_EQ(3, tiny::bytes::suffix_of(tiny::bytes{ "defabc" }, tiny::bytes{ "mpqwewewabc" }));
//     }
//     SECTION("空字符串") {
//         ASSERT_EQ(0, tiny::bytes::suffix_of(tiny::bytes{ "" }, tiny::bytes{ "" }));
//     }
//     SECTION("空字符串2") {
//         ASSERT_EQ(0, tiny::bytes::suffix_of(tiny::bytes{ "" }, tiny::bytes{ "abc" }));
//     }
// }
//
// TEST(tiny_bytes, split_by_string) {
//     SECTION("简单场景") {
//         tiny::bytes s{ "abcdbceebc" };
//         std::vector<tiny::bytes> fileds = {
//             tiny::bytes{ "a" },
//             tiny::bytes{ "d" },
//             tiny::bytes{ "ee" },
//             tiny::bytes{ "" },
//         };
//         std::vector<tiny::bytes> result;
//         s.split(tiny::bytes("bc"), [&result](tiny::bytes::const_pointer s, tiny::bytes::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("刚好匹配一个") {
//         tiny::bytes s{ "bc" };
//         std::vector<tiny::bytes> fileds = {
//             tiny::bytes{ "" },
//             tiny::bytes{ "" },
//         };
//         std::vector<tiny::bytes> result;
//         s.split(tiny::bytes("bc"), [&result](tiny::bytes::const_pointer s, tiny::bytes::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("前面字段") {
//         tiny::bytes s{ "AAAbc" };
//         std::vector<tiny::bytes> fileds = {
//             tiny::bytes{ "AAA" },
//             tiny::bytes{ "" },
//         };
//         std::vector<tiny::bytes> result;
//         s.split(tiny::bytes("bc"), [&result](tiny::bytes::const_pointer s, tiny::bytes::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("后面字段") {
//         tiny::bytes s{ "bcAAA" };
//         std::vector<tiny::bytes> fileds = {
//             tiny::bytes{ "" },
//             tiny::bytes{ "AAA" },
//         };
//         std::vector<tiny::bytes> result;
//         s.split(tiny::bytes("bc"), [&result](tiny::bytes::const_pointer s, tiny::bytes::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("空串") {
//         tiny::bytes s{ "" };
//         std::vector<tiny::bytes> fileds = {
//             tiny::bytes{ "" },
//         };
//         std::vector<tiny::bytes> result;
//         s.split(tiny::bytes("bc"), [&result](tiny::bytes::const_pointer s, tiny::bytes::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("连续匹配") {
//         tiny::bytes s{ "AAbcbcBB" };
//         std::vector<tiny::bytes> fileds = {
//             tiny::bytes{ "AA" },
//             tiny::bytes{ "" },
//             tiny::bytes{ "BB" },
//         };
//         std::vector<tiny::bytes> result;
//         s.split(tiny::bytes("bc"), [&result](tiny::bytes::const_pointer s, tiny::bytes::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("连续匹配2") {
//         tiny::bytes s{ "bcbc" };
//         std::vector<tiny::bytes> fileds = {
//             tiny::bytes{ "" },
//             tiny::bytes{ "" },
//             tiny::bytes{ "" },
//         };
//         std::vector<tiny::bytes> result;
//         s.split(tiny::bytes("bc"), [&result](tiny::bytes::const_pointer s, tiny::bytes::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("无法匹配") {
//         tiny::bytes s{ "defgui" };
//         std::vector<tiny::bytes> fileds = {
//             tiny::bytes{ "defgui" },
//         };
//         std::vector<tiny::bytes> result;
//         s.split(tiny::bytes("bc"), [&result](tiny::bytes::const_pointer s, tiny::bytes::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
// }
//
// TEST(tiny_bytes, split_lines_notkeepends) {
//     SECTION("简单场景") {
//         tiny::bytes s{ "\na\r\nb\ncccc\n" };
//         std::vector<tiny::bytes> fields = {
//             tiny::bytes{ "" },
//             tiny::bytes{ "a" },
//             tiny::bytes{ "b" },
//             tiny::bytes{ "cccc" },
//             tiny::bytes{ "" },
//         };
//
//         std::vector<tiny::bytes> result;
//         s.split_lines([&result](tiny::bytes::const_pointer p, tiny::bytes::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//     SECTION("空串") {
//         tiny::bytes s{ "" };
//         std::vector<tiny::bytes> fields = {
//             tiny::bytes{ "" },
//         };
//
//         std::vector<tiny::bytes> result;
//         s.split_lines([&result](tiny::bytes::const_pointer p, tiny::bytes::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//     SECTION("纯换行") {
//         tiny::bytes s{ "\n" };
//         std::vector<tiny::bytes> fields = {
//             tiny::bytes{ "" },
//             tiny::bytes{ "" },
//         };
//
//         std::vector<tiny::bytes> result;
//         s.split_lines([&result](tiny::bytes::const_pointer p, tiny::bytes::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//
//     SECTION("综合测试") {
//         tiny::bytes s{ "a\n\ryyy\r\r\nb\n\nc\n\r" };
//         std::vector<tiny::bytes> fields = {
//             tiny::bytes{ "a" },
//             tiny::bytes{ "" },
//             tiny::bytes{ "yyy" },
//             tiny::bytes{ "" },
//             tiny::bytes{ "b" },
//             tiny::bytes{ "" },
//             tiny::bytes{ "c" },
//             tiny::bytes{ "" },
//             tiny::bytes{ "" },
//         };
//
//         std::vector<tiny::bytes> result;
//         s.split_lines([&result](tiny::bytes::const_pointer p, tiny::bytes::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
// }
//
// TEST(tiny_bytes, split_lines_keepends) {
//     SECTION("简单场景") {
//         tiny::bytes s{ "\na\r\nb\ncccc\n" };
//         std::vector<tiny::bytes> fields = {
//             tiny::bytes{ "\n" },
//             tiny::bytes{ "a\r\n" },
//             tiny::bytes{ "b\n" },
//             tiny::bytes{ "cccc\n" },
//             tiny::bytes{ "" },
//         };
//
//         std::vector<tiny::bytes> result;
//         s.split_lines(true, [&result](tiny::bytes::const_pointer p, tiny::bytes::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//     SECTION("空串") {
//         tiny::bytes s{ "" };
//         std::vector<tiny::bytes> fields = {
//             tiny::bytes{ "" },
//         };
//
//         std::vector<tiny::bytes> result;
//         s.split_lines(true, [&result](tiny::bytes::const_pointer p, tiny::bytes::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//     SECTION("纯换行") {
//         tiny::bytes s{ "\n" };
//         std::vector<tiny::bytes> fields = {
//             tiny::bytes{ "\n" },
//             tiny::bytes{ "" },
//         };
//
//         std::vector<tiny::bytes> result;
//         s.split_lines(true, [&result](tiny::bytes::const_pointer p, tiny::bytes::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//
//     SECTION("综合测试") {
//         tiny::bytes s{ "a\n\ryyy\r\r\nb\n\nc\n\r" };
//         std::vector<tiny::bytes> fields = {
//             tiny::bytes{ "a\n" },
//             tiny::bytes{ "\r" },
//             tiny::bytes{ "yyy\r" },
//             tiny::bytes{ "\r\n" },
//             tiny::bytes{ "b\n" },
//             tiny::bytes{ "\n" },
//             tiny::bytes{ "c\n" },
//             tiny::bytes{ "\r" },
//             tiny::bytes{ "" },
//         };
//
//         std::vector<tiny::bytes> result;
//         s.split_lines(true, [&result](tiny::bytes::const_pointer p, tiny::bytes::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
// }
