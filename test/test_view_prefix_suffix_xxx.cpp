//
// Created by root on 9/8/24.
//
// TEST_CASE("has_prefix") {
//
//    SECTION("一般情况") {
//        REQUIRE(str::has_prefix("aaa", "aa") == true);
//    }
//    SECTION("无共同前缀") {
//        REQUIRE(str::has_prefix("aaa", "bb") == false);
//    }
//    SECTION("完全相同") {
//        REQUIRE(str::has_prefix("aaa", "aaa") == true);
//    }
//    SECTION("部分为空") {
//        REQUIRE(str::has_prefix("", "aaa") == false);
//        REQUIRE(str::has_prefix("aaa", "") == true);
//    }
//    SECTION("空对空") {
//        REQUIRE(str::has_prefix("", "") == true);
//    }
//    SECTION("包含关系") {
//        REQUIRE(str::has_prefix("aaa", "aa") == true);
//    }
//
//    SECTION("简单测试") {
//        std::string s("HelloWorld");
//        REQUIRE(str::has_prefix(s, "Hello"));
//        REQUIRE(str::has_prefix(s, std::string("Hello")));
//        REQUIRE(str::has_prefix(s, 'H'));
//        REQUIRE(str::has_prefix(s, { "HelloXYZ", 5 }));
//        REQUIRE(str::has_prefix(s, { "", 0 }));
//        REQUIRE(str::has_prefix(s, ""));
//    }
//
//    SECTION("空字符串找前缀") {
//        std::string s("");
//        REQUIRE(str::has_prefix(s, "Hello") == false);
//        REQUIRE(str::has_prefix(s, std::string("Hello")) == false);
//        REQUIRE(str::has_prefix(s, 'H') == false);
//        REQUIRE(str::has_prefix(s, { "HelloXYZ", 5 }) == false);
//        REQUIRE(str::has_prefix(s, { "", 0 }) == true);
//        REQUIRE(str::has_prefix(s, "") == true);
//    }
//}
//
// TEST_CASE("str::has_suffix") {
//    SECTION("简单测试") {
//        std::string s("HelloWorld");
//        REQUIRE(str::has_suffix(s, "World"));
//        REQUIRE(str::has_suffix(s, std::string("World")));
//        REQUIRE(str::has_suffix(s, 'd'));
//        REQUIRE(str::has_suffix(s, { "WorldXYZ", 5 }));
//        REQUIRE(str::has_suffix(s, { "", 0 }));
//        REQUIRE(str::has_suffix(s, ""));
//    }
//}


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

