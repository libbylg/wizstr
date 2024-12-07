#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, prefix) {
    SECTION("一般情况") {
        ASSERT_EQ(view::prefix("aaa", "aab"), 2);
    }
    SECTION("无共同前缀") {
        ASSERT_EQ(view::prefix("aaa", "bbb"), 0);
    }
    SECTION("完全相同") {
        ASSERT_EQ(view::prefix("aaa", "aaa"), 3);
    }
    SECTION("部分为空") {
        ASSERT_EQ(view::prefix("", "aaa"), 0);
        ASSERT_EQ(view::prefix("aaa", ""), 0);
    }
    SECTION("空对空") {
        ASSERT_EQ(view::prefix("", ""), 0);
    }
    SECTION("包含关系") {
        ASSERT_EQ(view::prefix("aaa", "aa"), 2);
        ASSERT_EQ(view::prefix("aa", "aaa"), 2);
    }
}

TEST(test_view, has_prefix) {
    SECTION("一般情况") {
        ASSERT_EQ(view::has_prefix("aaa", "aa"), true);
        ASSERT_EQ(view::has_prefix("aaa", "aab"), false);
    }
    SECTION("无共同前缀") {
        ASSERT_EQ(view::has_prefix("aaa", "bb"), false);
    }
    SECTION("完全相同") {
        ASSERT_EQ(view::has_prefix("aaa", "aaa"), true);
    }
    SECTION("部分为空") {
        ASSERT_EQ(view::has_prefix("", "aaa"), false);
        ASSERT_EQ(view::has_prefix("aaa", ""), true);
    }
    SECTION("空对空") {
        ASSERT_EQ(view::has_prefix("", ""), true);
    }
    SECTION("包含关系") {
        ASSERT_EQ(view::has_prefix("aaa", "aa"), true);
    }
    SECTION("空字符串找前缀") {
        ASSERT_EQ(view::has_prefix("", "Hello"), false);
        ASSERT_EQ(view::has_prefix("Hello", ""), true);
        ASSERT_EQ(view::has_prefix("", ""), true);
    }
    SECTION("字符前缀") {
        ASSERT_EQ(view::has_prefix("HelloWorld", 'H'), true);
        ASSERT_EQ(view::has_prefix("HelloWorld", 'W'), false);
        ASSERT_EQ(view::has_prefix("", 'H'), false);
        ASSERT_EQ(view::has_prefix("", '\0'), false);
    }
}

TEST(test_view, starts_with) {
    SECTION("一般情况") {
        ASSERT_EQ(view::starts_with("aaa", "aa"), true);
        ASSERT_EQ(view::starts_with("aaa", "aab"), false);
    }
    SECTION("无共同前缀") {
        ASSERT_EQ(view::starts_with("aaa", "bb"), false);
    }
    SECTION("完全相同") {
        ASSERT_EQ(view::starts_with("aaa", "aaa"), true);
    }
    SECTION("部分为空") {
        ASSERT_EQ(view::starts_with("", "aaa"), false);
        ASSERT_EQ(view::starts_with("aaa", ""), true);
    }
    SECTION("空对空") {
        ASSERT_EQ(view::starts_with("", ""), true);
    }
    SECTION("包含关系") {
        ASSERT_EQ(view::starts_with("aaa", "aa"), true);
    }
    SECTION("空字符串找前缀") {
        ASSERT_EQ(view::starts_with("", "Hello"), false);
        ASSERT_EQ(view::starts_with("Hello", ""), true);
        ASSERT_EQ(view::starts_with("", ""), true);
    }
    SECTION("字符前缀") {
        ASSERT_EQ(view::starts_with("HelloWorld", 'H'), true);
        ASSERT_EQ(view::starts_with("HelloWorld", 'W'), false);
        ASSERT_EQ(view::starts_with("", 'H'), false);
        ASSERT_EQ(view::starts_with("", '\0'), false);
    }
}

TEST(test_view, remove_prefix) {
    SECTION("一般情况") {
        ASSERT_EQ(view::remove_prefix("aac", "aa"), "c");
        ASSERT_EQ(view::remove_prefix("aaa", "aab"), "aaa");
    }
    SECTION("无共同前缀") {
        ASSERT_EQ(view::remove_prefix("aaa", "bb"), "aaa");
    }
    SECTION("完全相同") {
        ASSERT_EQ(view::remove_prefix("aaa", "aaa"), "");
    }
    SECTION("部分为空") {
        ASSERT_EQ(view::remove_prefix("", "aaa"), "");
        ASSERT_EQ(view::remove_prefix("aaa", ""), "aaa");
    }
    SECTION("空对空") {
        ASSERT_EQ(view::remove_prefix("", ""), "");
    }
    SECTION("包含关系") {
        ASSERT_EQ(view::remove_prefix("aac", "aa"), "c");
    }
    SECTION("空字符串找前缀") {
        ASSERT_EQ(view::remove_prefix("", "Hello"), "");
        ASSERT_EQ(view::remove_prefix("Hello", ""), "Hello");
        ASSERT_EQ(view::remove_prefix("", ""), "");
    }
    SECTION("字符前缀") {
        ASSERT_EQ(view::remove_prefix("HelloWorld", 'H'), "elloWorld");
        ASSERT_EQ(view::remove_prefix("HelloWorld", 'W'), "HelloWorld");
        ASSERT_EQ(view::remove_prefix("", 'H'), "");
        ASSERT_EQ(view::remove_prefix("", '\0'), "");
    }
//    SECTION("删除指定长度的前缀") {
//        ASSERT_EQ(view::remove_prefix("HelloWorld", 0u), "HelloWorld");
//        ASSERT_EQ(view::remove_prefix("HelloWorld", 5u), "World");
//        ASSERT_EQ(view::remove_prefix("HelloWorld", 10u), "");
//        ASSERT_EQ(view::remove_prefix("HelloWorld", 11u), "");
//        ASSERT_EQ(view::remove_prefix("HelloWorld", std::string_view::npos), "");
//    }
}

TEST(test_view, suffix) {
    SECTION("一般情况") {
        ASSERT_EQ(view::suffix("aaa", "baa"), 2);
    }
    SECTION("无共同后缀") {
        ASSERT_EQ(view::suffix("aaa", "bbb"), 0);
    }
    SECTION("完全相同") {
        ASSERT_EQ(view::suffix("aaa", "aaa"), 3);
    }
    SECTION("部分为空") {
        ASSERT_EQ(view::suffix("", "aaa"), 0);
        ASSERT_EQ(view::suffix("aaa", ""), 0);
    }
    SECTION("空对空") {
        ASSERT_EQ(view::suffix("", ""), 0);
    }
    SECTION("包含关系") {
        ASSERT_EQ(view::suffix("aaa", "aa"), 2);
        ASSERT_EQ(view::suffix("aa", "aaa"), 2);
    }
}

TEST(test_view, has_suffix) {
    SECTION("一般情况") {
        ASSERT_EQ(view::has_suffix("aaa", "baa"), false);
    }
    SECTION("无共同后缀") {
        ASSERT_EQ(view::has_suffix("aaa", "bbb"), false);
    }
    SECTION("完全相同") {
        ASSERT_EQ(view::has_suffix("aaa", "aaa"), true);
    }
    SECTION("部分为空") {
        ASSERT_EQ(view::has_suffix("", "aaa"), false);
        ASSERT_EQ(view::has_suffix("aaa", ""), true);
    }
    SECTION("空对空") {
        ASSERT_EQ(view::has_suffix("", ""), true);
    }
    SECTION("包含关系") {
        ASSERT_EQ(view::has_suffix("abc", "bc"), true);
        ASSERT_EQ(view::has_suffix("bc", "abc"), false);
    }
    SECTION("字符后缀") {
        ASSERT_EQ(view::has_suffix("HelloWorld", 'd'), true);
        ASSERT_EQ(view::has_suffix("HelloWorld", 'K'), false);
        ASSERT_EQ(view::has_suffix("", 'H'), false);
        ASSERT_EQ(view::has_suffix("", '\0'), false);
    }
}


TEST(test_view, ends_with) {
    SECTION("一般情况") {
        ASSERT_EQ(view::ends_with("aaa", "baa"), false);
    }
    SECTION("无共同后缀") {
        ASSERT_EQ(view::ends_with("aaa", "bbb"), false);
    }
    SECTION("完全相同") {
        ASSERT_EQ(view::ends_with("aaa", "aaa"), true);
    }
    SECTION("部分为空") {
        ASSERT_EQ(view::ends_with("", "aaa"), false);
        ASSERT_EQ(view::ends_with("aaa", ""), true);
    }
    SECTION("空对空") {
        ASSERT_EQ(view::ends_with("", ""), true);
    }
    SECTION("包含关系") {
        ASSERT_EQ(view::ends_with("abc", "bc"), true);
        ASSERT_EQ(view::ends_with("bc", "abc"), false);
    }
    SECTION("字符后缀") {
        ASSERT_EQ(view::ends_with("HelloWorld", 'd'), true);
        ASSERT_EQ(view::ends_with("HelloWorld", 'K'), false);
        ASSERT_EQ(view::ends_with("", 'H'), false);
        ASSERT_EQ(view::ends_with("", '\0'), false);
    }
}



TEST(test_view, remove_suffix) {
    SECTION("一般情况") {
        ASSERT_EQ(view::remove_suffix("aaa", "baa"), "aaa");
    }
    SECTION("无共同后缀") {
        ASSERT_EQ(view::remove_suffix("aaa", "bbb"), "aaa");
    }
    SECTION("完全相同") {
        ASSERT_EQ(view::remove_suffix("aaa", "aaa"), "");
    }
    SECTION("部分为空") {
        ASSERT_EQ(view::remove_suffix("", "aaa"), "");
        ASSERT_EQ(view::remove_suffix("aaa", ""), "aaa");
    }
    SECTION("空对空") {
        ASSERT_EQ(view::remove_suffix("", ""), "");
    }
    SECTION("包含关系") {
        ASSERT_EQ(view::remove_suffix("abc", "bc"), "a");
        ASSERT_EQ(view::remove_suffix("bc", "abc"), "bc");
    }
    SECTION("字符后缀") {
        ASSERT_EQ(view::remove_suffix("HelloWorld", 'd'), "HelloWorl");
        ASSERT_EQ(view::remove_suffix("HelloWorld", 'K'), "HelloWorld");
        ASSERT_EQ(view::remove_suffix("", 'H'), "");
        ASSERT_EQ(view::remove_suffix("", '\0'), "");
    }
}

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
