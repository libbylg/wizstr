#include "tester.hpp"

#include "str.hpp"

TEST(test_str, append) {
    SECTION("一般情况") {
        ASSERT_EQ(str::append("aaa", "bbb"), "aaabbb");
    }
    SECTION("空串") {
        ASSERT_EQ(str::append("aaa", ""), "aaa");
        ASSERT_EQ(str::append("", "aaa"), "aaa");
        ASSERT_EQ(str::append("", ""), "");
    }
    // SECTION("追加字符") {
    //     ASSERT_EQ(str::append("aaa", 'A'), "aaaA");
    //     ASSERT_EQ(str::append("", 'A'), "A");
    //     ASSERT_EQ(str::append("", '\0'), (std::string_view{"\0", 1}));
    // }
    // SECTION("追加重复的字符") {
    //     ASSERT_EQ(str::append("aaa", 'A', 3), "aaaAAA");
    //     ASSERT_EQ(str::append("", 'A', 3), "AAA");
    //     ASSERT_EQ(str::append("", '\0', 3), (std::string_view{"\0\0\0", 3}));
    // }
    SECTION("通过proc提供数据:一般") {
        std::vector<std::string_view> items{
            "Hello",
            "World",
            "",
            "!!!",
        };
        str::size_type index = 0;
        std::string result = str::append("abc", [&items, &index]() -> std::optional<std::string_view> {
            if (index >= items.size()) {
                return std::nullopt;
            }

            return items[index++];
        });
        ASSERT_EQ(result, "abcHelloWorld!!!");
    }
    SECTION("通过proc提供数据:立即终止") {
        std::string result = str::append("abc", []() -> std::optional<std::string_view> {
            return std::nullopt;
        });
        ASSERT_EQ(result, "abc");
    }
}

TEST(test_str, prepend) {
    SECTION("一般情况") {
        ASSERT_EQ(str::prepend("aaa", {"bbb"}), "bbbaaa");
    }
    SECTION("空串") {
        ASSERT_EQ(str::prepend("aaa", {""}), "aaa");
        ASSERT_EQ(str::prepend("", {"aaa"}), "aaa");
        ASSERT_EQ(str::prepend("", {""}), "");
    }
    // SECTION("追加字符") {
    //     ASSERT_EQ(str::prepend("aaa", 'A'), "Aaaa");
    //     ASSERT_EQ(str::prepend("", 'A'), "A");
    //     ASSERT_EQ(str::prepend("", '\0'), (std::string_view{"\0", 1}));
    // }
    // SECTION("追加重复的字符") {
    //     ASSERT_EQ(str::prepend("aaa", 'A', 3), "AAAaaa");
    //     ASSERT_EQ(str::prepend("", 'A', 3), "AAA");
    //     ASSERT_EQ(str::prepend("", '\0', 3), (std::string_view{"\0\0\0", 3}));
    // }
    SECTION("通过proc提供数据:一般") {
        std::vector<std::string_view> items{
            "Hello",
            "World",
            "",
            "!!!",
        };
        str::size_type index = 0;
        std::string result = str::prepend("abc", [&items, &index]() -> std::optional<std::string_view> {
            if (index >= items.size()) {
                return std::nullopt;
            }

            return items[index++];
        });
        ASSERT_EQ(result, "!!!WorldHelloabc");
    }
    SECTION("通过proc提供数据:立即终止") {
        std::string result = str::prepend("abc", []() -> std::optional<std::string_view> {
            return std::nullopt;
        });
        ASSERT_EQ(result, "abc");
    }
}

TEST(test_str, insert) {
    SECTION("一般情况") {
        ASSERT_EQ(str::insert("aaa", 0, "bbb"), "bbbaaa");
        ASSERT_EQ(str::insert("aaa", 1, "bbb"), "abbbaa");
        ASSERT_EQ(str::insert("aaa", 2, "bbb"), "aabbba");
        ASSERT_EQ(str::insert("aaa", 3, "bbb"), "aaabbb");
        ASSERT_EQ(str::insert("aaa", 4, "bbb"), "aaabbb");
        ASSERT_EQ(str::insert("aaa", str::npos, "bbb"), "aaabbb");
    }
    SECTION("插入空串") {
        ASSERT_EQ(str::insert("aaa", 0, ""), "aaa");
        ASSERT_EQ(str::insert("aaa", 1, ""), "aaa");
        ASSERT_EQ(str::insert("aaa", 2, ""), "aaa");
        ASSERT_EQ(str::insert("aaa", 3, ""), "aaa");
        ASSERT_EQ(str::insert("aaa", 4, ""), "aaa");
        ASSERT_EQ(str::insert("aaa", str::npos, ""), "aaa");
    }
    SECTION("插入到空串") {
        ASSERT_EQ(str::insert("", 0, ""), ""); // 将空串插入到空串
        ASSERT_EQ(str::insert("", 0, "bbb"), "bbb");
        ASSERT_EQ(str::insert("", 1, "bbb"), "bbb");
        ASSERT_EQ(str::insert("", str::npos, "bbb"), "bbb");
    }
    SECTION("插入单个字符") {
        ASSERT_EQ(str::insert("aaa", 0, 'B'), "Baaa");
        ASSERT_EQ(str::insert("aaa", 1, 'B'), "aBaa");
        ASSERT_EQ(str::insert("aaa", 2, 'B'), "aaBa");
        ASSERT_EQ(str::insert("aaa", 3, 'B'), "aaaB");
        ASSERT_EQ(str::insert("aaa", 4, 'B'), "aaaB");
        ASSERT_EQ(str::insert("aaa", str::npos, 'B'), "aaaB");
        ASSERT_EQ(str::insert("", 0, 'B'), "B");
        ASSERT_EQ(str::insert("", 9, 'B'), "B");
    }
    SECTION("重复插入一个字符") {
        ASSERT_EQ(str::insert("aaa", 0, 'B', 3), "BBBaaa");
        ASSERT_EQ(str::insert("aaa", 1, 'B', 3), "aBBBaa");
        ASSERT_EQ(str::insert("aaa", 2, 'B', 3), "aaBBBa");
        ASSERT_EQ(str::insert("aaa", 3, 'B', 3), "aaaBBB");
        ASSERT_EQ(str::insert("aaa", 4, 'B', 3), "aaaBBB");
        ASSERT_EQ(str::insert("aaa", str::npos, 'B', 3), "aaaBBB");
        ASSERT_EQ(str::insert("", 0, 'B', 3), "BBB");
        ASSERT_EQ(str::insert("", 9, 'B', 3), "BBB");
        ASSERT_EQ(str::insert("", 0, 'B', 0), "");
        ASSERT_EQ(str::insert("aaa", 2, 'B', 0), "aaa");
    }
    SECTION("通过proc提供数据:一般") {
        std::vector<std::string_view> items{
            "Hello",
            "World",
            "",
            "!!!",
        };
        str::size_type index = 0;
        std::string result = str::insert("abc", 2, [&items, &index]() -> std::optional<std::string_view> {
            if (index >= items.size()) {
                return std::nullopt;
            }

            return items[index++];
        });
        ASSERT_EQ(result, "abHelloWorld!!!c");
    }
    SECTION("通过proc提供数据:pos超出范围1") {
        std::vector<std::string_view> items{
            "Hello",
            "World",
            "",
            "!!!",
        };
        str::size_type index = 0;
        std::string result = str::insert("abc", 5, [&items, &index]() -> std::optional<std::string_view> {
            if (index >= items.size()) {
                return std::nullopt;
            }

            return items[index++];
        });
        ASSERT_EQ(result, "abcHelloWorld!!!");
    }
    SECTION("通过proc提供数据:pos超出范围2") {
        std::vector<std::string_view> items{
            "Hello",
            "World",
            "",
            "!!!",
        };
        str::size_type index = 0;
        std::string result = str::insert("abc", str::npos, [&items, &index]() -> std::optional<std::string_view> {
            if (index >= items.size()) {
                return std::nullopt;
            }

            return items[index++];
        });
        ASSERT_EQ(result, "abcHelloWorld!!!");
    }
    SECTION("通过proc提供数据:立即终止") {
        std::string result = str::insert("abc", 2, []() -> std::optional<std::string_view> {
            return std::nullopt;
        });
        ASSERT_EQ(result, "abc");
    }
}