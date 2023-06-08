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

TEST_CASE("last_extname_ptr", "") {
    SECTION("简单裸名字") {
        REQUIRE(str::last_extname("abc") == std::string(""));
    }
    SECTION("简单裸名字带扩展名") {
        REQUIRE(str::last_extname("abc.ext") == std::string(".ext"));
    }
    SECTION("裸名字带路径") {
        REQUIRE(str::last_extname("abc/def") == std::string(""));
    }
    SECTION("只有路径无basename") {
        REQUIRE(str::last_extname("abc/") == std::string(""));
    }
    SECTION("裸名字带路径路径中有点") {
        REQUIRE(str::last_extname("/abc.def/basename.ext") == std::string(".ext"));
    }
    SECTION("裸名字带路径路径中有点+") {
        REQUIRE(str::last_extname("/abc.def/") == std::string(""));
    }
    SECTION("basename为隐藏文件，隐藏文件点开头不是只有扩展名") {
        REQUIRE(str::last_extname("abc/.hidefile") == std::string(""));
    }
    SECTION("basename为隐藏文件，隐藏文件点开头，但是还有额外扩展名") {
        REQUIRE(str::last_extname("abc/.hidefile.ext") == std::string(".ext"));
    }
    SECTION("basename带多个点") {
        REQUIRE(str::last_extname("abc/normal-file.xxx.ext") == std::string(".ext"));
    }
    SECTION("basename位置只有点1") {
        REQUIRE(str::last_extname("abc/.") == std::string(""));
    }
    SECTION("basename位置只有点2") {
        REQUIRE(str::last_extname("abc/..") == std::string(""));
    }
    SECTION("basename位置只有点3") {
        REQUIRE(str::last_extname("...") == std::string(""));
    }
    SECTION("basename位置,点在最后1") {
        REQUIRE(str::last_extname(".abc.") == std::string("."));
    }
    SECTION("basename位置,点在最后2") {
        REQUIRE(str::last_extname(".abc..") == std::string("."));
    }
    SECTION("basename位置,多点前缀1") {
        REQUIRE(str::last_extname("..abc") == std::string(""));
    }
    SECTION("basename位置,多点前缀1") {
        REQUIRE(str::last_extname("...abc") == std::string(""));
    }
    SECTION("空串") {
        REQUIRE(str::last_extname("") == std::string(""));
    }
    SECTION("全空白字符") {
        REQUIRE(str::last_extname(" \t ") == std::string(""));
    }
}

// TEST(tiny_bytes, prepend) {
//     SECTION("向前追加") {
//         std::string a("123");
//         a.prepend("abc");
//         EXPECT_TRUE(a == "abc123");
//     }
// }
//
// TEST(tiny_bytes, zfill) {
//     SECTION("正常情况") {
//         std::string a("123");
//         EXPECT_TRUE(a.zfill(5) == "00123");
//     }
//     SECTION("前缀") {
//         EXPECT_TRUE(std::string("+123").zfill(5) == "+0123");
//         EXPECT_TRUE(std::string("-123").zfill(5) == "-0123");
//     }
//     SECTION("空串") {
//         std::string a("");
//         EXPECT_TRUE(a.zfill(5) == "00000");
//     }
//     SECTION("非数字") {
//         EXPECT_EQ(std::string("LMK").zfill(5), "00LMK");
//         EXPECT_TRUE(std::string("中华人民共和国").zfill(25) == std::string("0000中华人民共和国"));
//     }
//     SECTION("宽度太短") {
//         EXPECT_TRUE(std::string("LMK").zfill(3) == "LMK");
//         EXPECT_TRUE(std::string("LMK").zfill(2) == "LMK");
//         EXPECT_TRUE(std::string("LMK").zfill(0) == "LMK");
//     }
// }
//
// TEST(tiny_bytes, remove) {
//     SECTION("删除中间数据") {
//         std::string a("abc123");
//         a.remove(0);
//         EXPECT_TRUE(a == "bc123");
//         a.remove(1);
//         EXPECT_TRUE(a == "b123");
//     }
//
//     SECTION("删除指定的字符") {
//         std::string a("3bc1233");
//         a.remove('3');
//         EXPECT_TRUE(a == "bc12");
//     }
//
//     SECTION("删除指定的字符串") {
//         std::string a("33b33c1233");
//         a.remove("33");
//         EXPECT_TRUE(a == "bc12");
//     }
// }
//
// TEST(tiny_bytes, contains) {
//     SECTION("删除中间数据") {
//         std::string a("3bc1233");
//         EXPECT_TRUE(a.contains("12"));
//         EXPECT_TRUE(a.contains("33"));
//         EXPECT_FALSE(a.contains("31"));
//     }
// }
//
// TEST(tiny_bytes, trims) {
//     SECTION("前后没有空白") {
//         std::string a("3bc1233");
//         EXPECT_TRUE(a.trim() == "3bc1233");
//     }
//
//     SECTION("前后都有空白") {
//         std::string b("  \tc1233\t  \t  ");
//         EXPECT_TRUE(b.trim() == "c1233");
//     }
//
//     SECTION("指定字符") {
//         std::string b("33c1233");
//         EXPECT_TRUE(b.trim([](std::string::value_type ch) -> bool {
//             return ch == '3';
//         }) == "c12");
//     }
// }
//
// TEST(tiny_bytes, is_match_wild) {
//     SECTION("检查是否匹配某个正则表达式") {
//         std::string a("3bc12def33");
//         EXPECT_TRUE(a.is_match_wild("3*"));
//         EXPECT_TRUE(a.is_match_wild("3bc12def33"));
//         EXPECT_TRUE(a.is_match_wild("3*de?33"));
//     }
// }
//
// TEST(tiny_bytes, count) {
//     SECTION("检查是否匹配某个正则表达式") {
//         std::string a("3bc12def1233");
//         EXPECT_EQ(a.count("12"), 2);
//         EXPECT_EQ(a.count(char('3')), 3);
//         EXPECT_EQ(a.count("A"), 0);
//     }
//
//     SECTION("统计子串数量") {
//         std::string a("333433343333334");
//         EXPECT_EQ(a.count("33"), 5);
//         EXPECT_EQ(a.count(""), 16);
//         EXPECT_EQ(a.count("333"), 4);
//     }
// }
//
// TEST(tiny_bytes, join) {
//     SECTION("简单串联") {
//         std::string a("/");
//         EXPECT_EQ(a.join({ "aa", "bb", "cc" }), "aa/bb/cc");
//     }
// }
//
// TEST(tiny_bytes, repeat) {
//     SECTION("简单字符串重复多次") {
//         std::string a("0123456789");
//         EXPECT_EQ(a.repeat(5), "01234567890123456789012345678901234567890123456789");
//     }
// }
//
// TEST(tiny_bytes, swap_case) {
//     SECTION("简单测试") {
//         std::string a("  AabbCC中华人民共和国");
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
        std::string s("");
        REQUIRE(str::has_prefix(s, "Hello") == false);
        REQUIRE(str::has_prefix(s, std::string("Hello")) == false);
        REQUIRE(str::has_prefix(s, 'H') == false);
        REQUIRE(str::has_prefix(s, { "HelloXYZ", 5 }) == false);
        REQUIRE(str::has_prefix(s, { "", 0 }) == true);
        REQUIRE(str::has_prefix(s, "") == true);
    }
}

TEST_CASE("str::has_suffix") {
    SECTION("简单测试") {
        std::string s("HelloWorld");
        REQUIRE(str::has_suffix(s, "World"));
        REQUIRE(str::has_suffix(s, std::string("World")));
        REQUIRE(str::has_suffix(s, 'd'));
        REQUIRE(str::has_suffix(s, { "WorldXYZ", 5 }));
        REQUIRE(str::has_suffix(s, { "", 0 }));
        REQUIRE(str::has_suffix(s, ""));
    }
}

TEST_CASE("str::ljust") {
    SECTION("一般情况：字符串左对齐，右边填充") {
        std::string s("HelloWorld");
        REQUIRE(str::ljust(s, 13, '*') == "HelloWorld***");
    }

    SECTION("宽度和原字符串相同：字符串不变") {
        std::string s("HelloWorld");
        REQUIRE(str::ljust(s, 10, '*') == "HelloWorld");
    }

    SECTION("宽度太窄时保持原样") {
        std::string s("HelloWorld");
        REQUIRE(str::ljust(s, 9, '*') == "HelloWorld");
    }
}

TEST_CASE("str::rjust") {
    SECTION("简单测试") {
        std::string s("HelloWorld");
        REQUIRE(str::rjust(s, 13, '*') == "***HelloWorld");
    }

    SECTION("简单测试") {
        std::string s("HelloWorld");
        REQUIRE(str::rjust(s, 10, '*') == "HelloWorld");
    }

    SECTION("简单测试") {
        std::string s("HelloWorld");
        REQUIRE(str::rjust(s, 9, '*') == "HelloWorld");
    }

    SECTION("简单测试") {
        std::string s("HelloWorld");
        REQUIRE(str::rjust(s, 9, '*') == "HelloWorld");
    }
}

TEST_CASE("str::center") {
    SECTION("简单测试") {
        std::string s("HelloWorld");
        REQUIRE(str::center(s, 13, '*') == "*HelloWorld**");
    }

    SECTION("简单测试") {
        std::string s("HelloWorld");
        REQUIRE(str::center(s, 10, '*') == "HelloWorld");
    }

    SECTION("简单测试") {
        std::string s("HelloWorld");
        REQUIRE(str::center(s, 9, '*') == "HelloWorld");
    }

    SECTION("简单测试") {
        std::string s("HelloWorld");
        REQUIRE(str::center(s, 9, '*') == "HelloWorld");
    }
}

TEST_CASE("str::fill") {
    SECTION("简单测试") {
        std::string s("HelloWorld");
        REQUIRE(str::fill(s, '*') == "**********");
    }
    SECTION("简单测试") {
        std::string s("HelloWorld");
        REQUIRE(str::fill(s, '*', 1, 3) == "H***oWorld");
        REQUIRE(str::fill(s, '-', 0, 3) == "---*oWorld");
        REQUIRE(str::fill(s, 'A', 5, 5) == "---*oAAAAA");
        REQUIRE(str::fill(s, 'B', 5, 6) == "---*oBBBBB");
    }
}

TEST_CASE("str::invert") {
    SECTION("全部颠倒") {
        std::string s("HelloWorld");
        REQUIRE(str::invert(s) == "dlroWolleH");
    }
    SECTION("部分颠倒") {
        std::string s("HelloWorld");
        REQUIRE(str::invert(s, 2, 3) == "HeollWorld");
    }
    SECTION("指定位置之后的颠倒") {
        std::string s("HelloWorld");
        REQUIRE(str::invert(s, 5) == "HellodlroW");
    }
}

TEST_CASE("str::is_bool") {
    SECTION("简单场景") {
        REQUIRE(str::is_bool("1"));
        REQUIRE(str::is_bool("0"));
        REQUIRE(str::is_bool("on"));
        REQUIRE(str::is_bool("off"));
        REQUIRE(str::is_bool("ON"));
        REQUIRE(str::is_bool("OFF"));
        REQUIRE(str::is_bool("Yes"));
        REQUIRE(str::is_bool("No"));
        REQUIRE(str::is_bool("yes"));
        REQUIRE(str::is_bool("no"));
        REQUIRE(str::is_bool("YES"));
        REQUIRE(str::is_bool("NO"));
        REQUIRE(str::is_bool("True"));
        REQUIRE(str::is_bool("False"));
        REQUIRE(str::is_bool("true"));
        REQUIRE(str::is_bool("false"));
        REQUIRE(str::is_bool("TRUE"));
        REQUIRE(str::is_bool("FALSE"));
    }
}
//
// TEST(tiny_bytes, expand_tabs) {
//     SECTION("简单场景") {
//         std::string a("1\t123\t1234\t12345\t123456\t$");
//         EXPECT_EQ(a.expand_tabs(4), "1   123 1234    12345   123456  $");
//     }
// }
//
// TEST(tiny_bytes, to_int8) {
//     std::string a("1111111");
//     EXPECT_EQ(a.to_int8(nullptr, 2), 127);
// }
//
// TEST(tiny_bytes, from) {
//     EXPECT_EQ(std::string::from(int8_t(-1), 2), "11111111");
//     EXPECT_EQ(std::string::from(int16_t(-1), 2), "1111111111111111");
//     EXPECT_EQ(std::string::from(int32_t(-1), 2), "11111111111111111111111111111111");
//     EXPECT_EQ(std::string::from(int64_t(-1), 2), "1111111111111111111111111111111111111111111111111111111111111111");
//
//     EXPECT_EQ(std::string::from(uint8_t(-1), 2), "11111111");
//     EXPECT_EQ(std::string::from(uint16_t(-1), 2), "1111111111111111");
//     EXPECT_EQ(std::string::from(uint32_t(-1), 2), "11111111111111111111111111111111");
//     EXPECT_EQ(std::string::from(uint64_t(-1), 2), "1111111111111111111111111111111111111111111111111111111111111111");
//
//     EXPECT_EQ(std::string::from(int8_t(-1), 10), "-1");
//     EXPECT_EQ(std::string::from(int16_t(-1), 10), "-1");
//     EXPECT_EQ(std::string::from(int32_t(-1), 10), "-1");
//     EXPECT_EQ(std::string::from(int64_t(-1), 10), "-1");
//
//     EXPECT_EQ(std::string::from(uint8_t(-1), 10), "255");
//     EXPECT_EQ(std::string::from(uint16_t(-1), 10), "65535");
//     EXPECT_EQ(std::string::from(uint32_t(-1), 10), "4294967295");
//     EXPECT_EQ(std::string::from(uint64_t(-1), 10), "18446744073709551615");
//
//     EXPECT_EQ(std::string::from(uint8_t(-1), 16), "ff");
//     EXPECT_EQ(std::string::from(uint16_t(-1), 16), "ffff");
//     EXPECT_EQ(std::string::from(uint32_t(-1), 16), "ffffffff");
//     EXPECT_EQ(std::string::from(uint64_t(-1), 16), "ffffffffffffffff");
// }
//
// TEST(tiny_bytes, split_by_char) {
//     std::string a("HelloWorld-HelloWorld");
//     std::vector<std::string> v;
//     a.split('-', [&v](std::string::const_pointer s, std::string::size_type n) -> int {
//         v.emplace_back(s, n);
//         return 0;
//     });
//     EXPECT_EQ(v.size(), 2);
//     EXPECT_EQ(v[0], "HelloWorld");
//     EXPECT_EQ(v[1], "HelloWorld");
//     std::vector<std::string> p{ std::string{ "HelloWorld" }, std::string{ "HelloWorld" } };
//     EXPECT_EQ(v, p);
// }
//
// TEST(tiny_bytes, find_first_of_test) {
//     std::string a("hello world");
//     ASSERT(a.find_first_of("wr", 0, 100) == 6);
//     ASSERT(a.find_first_of("mkf", 0, 100) == std::string::npos);
// }
//
// TEST(tiny_bytes, c_str) {
//     SECTION("短字符串") {
//         std::string a("hello world");
//         ASSERT_TRUE(strcmp(a.c_str(), "hello world") == 0);
//         ASSERT_TRUE(a.data() == a.c_str());
//     }
//
//     SECTION("长字符串") {
//         const char* cb = "hello worldhello worldhello worldhello worldhello worldhello worldhello world";
//         std::string b(cb);
//         ASSERT_TRUE(strcmp(b.c_str(), cb) == 0);
//         ASSERT_TRUE(b.data() == b.c_str());
//     }
//
//     SECTION("空字符串1") {
//         std::string b;
//         ASSERT_TRUE(strcmp(b.c_str(), "") == 0);
//         ASSERT_TRUE(b.data() == b.c_str());
//     }
//
//     SECTION("空字符串2") {
//         std::string b;
//         b.append("");
//         ASSERT_TRUE(strcmp(b.c_str(), "") == 0);
//         ASSERT_TRUE(b.data() == b.c_str());
//     }
// }
//
// TEST(tiny_bytes, attach) {
//     char buff[100] = "abc";
//     std::string b("12345678901234567890");
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
//         std::string b("${HOME}/${NOTEXIST}");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, std::string(getenv("HOME")) + "/");
//     }
//
//     SECTION("简单扩展") {
//         std::string b("$HOME/${NOTEXIST}");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, std::string(getenv("HOME")) + "/");
//     }
//
//     SECTION("简单扩展") {
//         std::string b("abcd$HOME/${NOTEXIST}");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, "abcd" + std::string(getenv("HOME")) + "/");
//     }
//
//     SECTION("简单扩展") {
//         std::string b("abcd${HOME}");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, "abcd" + std::string(getenv("HOME")));
//     }
//
//     SECTION("简单扩展") {
//         std::string b("$HOME");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, std::string(getenv("HOME")));
//     }
//
//     SECTION("简单扩展") {
//         std::string b("$$$");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, "$$$");
//     }
//
//     SECTION("简单扩展") {
//         std::string b("");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, "");
//     }
//
//     SECTION("简单扩展") {
//         std::string b("$");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, "$");
//     }
//
//     SECTION("简单扩展") {
//         std::string b("abc$");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, "abc$");
//     }
// }
//
// TEST(tiny_bytes, prefix_of) {
//     SECTION("首个参数不同") {
//         ASSERT_EQ(0, std::string::prefix_of(std::string{ "abc" }, std::string{ "def" }));
//     }
//     SECTION("一个字符相同") {
//         ASSERT_EQ(1, std::string::prefix_of(std::string{ "abca" }, std::string{ "aef" }));
//     }
//     SECTION("所有字符相同") {
//         ASSERT_EQ(3, std::string::prefix_of(std::string{ "abc" }, std::string{ "abc" }));
//     }
//     SECTION("前缀相同，但是长度不同1") {
//         ASSERT_EQ(3, std::string::prefix_of(std::string{ "abc" }, std::string{ "abcdef" }));
//     }
//     SECTION("前缀相同，但是长度不同2") {
//         ASSERT_EQ(3, std::string::prefix_of(std::string{ "abcdef" }, std::string{ "abc" }));
//     }
//     SECTION("前缀相同，但是长度不同2") {
//         ASSERT_EQ(3, std::string::prefix_of(std::string{ "abcdef" }, std::string{ "abcmpqwewew" }));
//     }
//     SECTION("空字符串") {
//         ASSERT_EQ(0, std::string::prefix_of(std::string{ "" }, std::string{ "" }));
//     }
//     SECTION("空字符串2") {
//         ASSERT_EQ(0, std::string::prefix_of(std::string{ "" }, std::string{ "abc" }));
//     }
// }
//
// TEST(tiny_bytes, suffix_of) {
//     SECTION("首个参数不同") {
//         ASSERT_EQ(0, std::string::suffix_of(std::string{ "abc" }, std::string{ "def" }));
//     }
//     SECTION("一个字符相同") {
//         ASSERT_EQ(1, std::string::suffix_of(std::string{ "bca" }, std::string{ "efa" }));
//     }
//     SECTION("所有字符相同") {
//         ASSERT_EQ(3, std::string::suffix_of(std::string{ "abc" }, std::string{ "abc" }));
//     }
//     SECTION("前缀相同，但是长度不同1") {
//         ASSERT_EQ(3, std::string::suffix_of(std::string{ "abc" }, std::string{ "defabc" }));
//     }
//     SECTION("前缀相同，但是长度不同2") {
//         ASSERT_EQ(3, std::string::suffix_of(std::string{ "defabc" }, std::string{ "abc" }));
//     }
//     SECTION("前缀相同，但是长度不同2") {
//         ASSERT_EQ(3, std::string::suffix_of(std::string{ "defabc" }, std::string{ "mpqwewewabc" }));
//     }
//     SECTION("空字符串") {
//         ASSERT_EQ(0, std::string::suffix_of(std::string{ "" }, std::string{ "" }));
//     }
//     SECTION("空字符串2") {
//         ASSERT_EQ(0, std::string::suffix_of(std::string{ "" }, std::string{ "abc" }));
//     }
// }
//
// TEST(tiny_bytes, split_by_string) {
//     SECTION("简单场景") {
//         std::string s{ "abcdbceebc" };
//         std::vector<std::string> fileds = {
//             std::string{ "a" },
//             std::string{ "d" },
//             std::string{ "ee" },
//             std::string{ "" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("刚好匹配一个") {
//         std::string s{ "bc" };
//         std::vector<std::string> fileds = {
//             std::string{ "" },
//             std::string{ "" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("前面字段") {
//         std::string s{ "AAAbc" };
//         std::vector<std::string> fileds = {
//             std::string{ "AAA" },
//             std::string{ "" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("后面字段") {
//         std::string s{ "bcAAA" };
//         std::vector<std::string> fileds = {
//             std::string{ "" },
//             std::string{ "AAA" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("空串") {
//         std::string s{ "" };
//         std::vector<std::string> fileds = {
//             std::string{ "" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("连续匹配") {
//         std::string s{ "AAbcbcBB" };
//         std::vector<std::string> fileds = {
//             std::string{ "AA" },
//             std::string{ "" },
//             std::string{ "BB" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("连续匹配2") {
//         std::string s{ "bcbc" };
//         std::vector<std::string> fileds = {
//             std::string{ "" },
//             std::string{ "" },
//             std::string{ "" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("无法匹配") {
//         std::string s{ "defgui" };
//         std::vector<std::string> fileds = {
//             std::string{ "defgui" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
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
//         std::string s{ "\na\r\nb\ncccc\n" };
//         std::vector<std::string> fields = {
//             std::string{ "" },
//             std::string{ "a" },
//             std::string{ "b" },
//             std::string{ "cccc" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines([&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//     SECTION("空串") {
//         std::string s{ "" };
//         std::vector<std::string> fields = {
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines([&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//     SECTION("纯换行") {
//         std::string s{ "\n" };
//         std::vector<std::string> fields = {
//             std::string{ "" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines([&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//
//     SECTION("综合测试") {
//         std::string s{ "a\n\ryyy\r\r\nb\n\nc\n\r" };
//         std::vector<std::string> fields = {
//             std::string{ "a" },
//             std::string{ "" },
//             std::string{ "yyy" },
//             std::string{ "" },
//             std::string{ "b" },
//             std::string{ "" },
//             std::string{ "c" },
//             std::string{ "" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines([&result](std::string::const_pointer p, std::string::size_type n) -> int {
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
//         std::string s{ "\na\r\nb\ncccc\n" };
//         std::vector<std::string> fields = {
//             std::string{ "\n" },
//             std::string{ "a\r\n" },
//             std::string{ "b\n" },
//             std::string{ "cccc\n" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//     SECTION("空串") {
//         std::string s{ "" };
//         std::vector<std::string> fields = {
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//     SECTION("纯换行") {
//         std::string s{ "\n" };
//         std::vector<std::string> fields = {
//             std::string{ "\n" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//
//     SECTION("综合测试") {
//         std::string s{ "a\n\ryyy\r\r\nb\n\nc\n\r" };
//         std::vector<std::string> fields = {
//             std::string{ "a\n" },
//             std::string{ "\r" },
//             std::string{ "yyy\r" },
//             std::string{ "\r\n" },
//             std::string{ "b\n" },
//             std::string{ "\n" },
//             std::string{ "c\n" },
//             std::string{ "\r" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
// }
