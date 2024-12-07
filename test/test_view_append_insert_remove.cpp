#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, append) {
    SECTION("一般情况") {
        ASSERT_EQ(view::append("aaa", "bbb"), "aaabbb");
    }
    SECTION("空串") {
        ASSERT_EQ(view::append("aaa", ""), "aaa");
        ASSERT_EQ(view::append("", "aaa"), "aaa");
        ASSERT_EQ(view::append("", ""), "");
    }
    SECTION("追加字符") {
        ASSERT_EQ(view::append("aaa", 'A'), "aaaA");
        ASSERT_EQ(view::append("", 'A'), "A");
        ASSERT_EQ(view::append("", '\0'), (std::string_view{"\0", 1}));
    }
    SECTION("追加重复的字符") {
        ASSERT_EQ(view::append("aaa", 'A', 3), "aaaAAA");
        ASSERT_EQ(view::append("", 'A', 3), "AAA");
        ASSERT_EQ(view::append("", '\0', 3), (std::string_view{"\0\0\0", 3}));
    }
    SECTION("通过proc提供数据:一般") {
        std::vector<std::string_view> items{
            "Hello",
            "World",
            "",
            "!!!",
        };
        view::size_type index = 0;
        std::string result = view::append("abc", [&items, &index]() -> std::optional<std::string_view> {
            if (index >= items.size()) {
                return std::nullopt;
            }

            return items[index++];
        });
        ASSERT_EQ(result, "abcHelloWorld!!!");
    }
    SECTION("通过proc提供数据:立即终止") {
        std::string result = view::append("abc", []() -> std::optional<std::string_view> {
            return std::nullopt;
        });
        ASSERT_EQ(result, "abc");
    }
}

TEST(test_view, prepend) {
    SECTION("一般情况") {
        ASSERT_EQ(view::prepend("aaa", "bbb"), "bbbaaa");
    }
    SECTION("空串") {
        ASSERT_EQ(view::prepend("aaa", ""), "aaa");
        ASSERT_EQ(view::prepend("", "aaa"), "aaa");
        ASSERT_EQ(view::prepend("", ""), "");
    }
    SECTION("追加字符") {
        ASSERT_EQ(view::prepend("aaa", 'A'), "Aaaa");
        ASSERT_EQ(view::prepend("", 'A'), "A");
        ASSERT_EQ(view::prepend("", '\0'), (std::string_view{"\0", 1}));
    }
    SECTION("追加重复的字符") {
        ASSERT_EQ(view::prepend("aaa", 'A', 3), "AAAaaa");
        ASSERT_EQ(view::prepend("", 'A', 3), "AAA");
        ASSERT_EQ(view::prepend("", '\0', 3), (std::string_view{"\0\0\0", 3}));
    }
    SECTION("通过proc提供数据:一般") {
        std::vector<std::string_view> items{
            "Hello",
            "World",
            "",
            "!!!",
        };
        view::size_type index = 0;
        std::string result = view::prepend("abc", [&items, &index]() -> std::optional<std::string_view> {
            if (index >= items.size()) {
                return std::nullopt;
            }

            return items[index++];
        });
        ASSERT_EQ(result, "!!!WorldHelloabc");
    }
    SECTION("通过proc提供数据:立即终止") {
        std::string result = view::prepend("abc", []() -> std::optional<std::string_view> {
            return std::nullopt;
        });
        ASSERT_EQ(result, "abc");
    }
}

TEST(test_view, insert) {
    SECTION("一般情况") {
        ASSERT_EQ(view::insert("aaa", 0, "bbb"), "bbbaaa");
        ASSERT_EQ(view::insert("aaa", 1, "bbb"), "abbbaa");
        ASSERT_EQ(view::insert("aaa", 2, "bbb"), "aabbba");
        ASSERT_EQ(view::insert("aaa", 3, "bbb"), "aaabbb");
        ASSERT_EQ(view::insert("aaa", 4, "bbb"), "aaabbb");
        ASSERT_EQ(view::insert("aaa", view::npos, "bbb"), "aaabbb");
    }
    SECTION("插入空串") {
        ASSERT_EQ(view::insert("aaa", 0, ""), "aaa");
        ASSERT_EQ(view::insert("aaa", 1, ""), "aaa");
        ASSERT_EQ(view::insert("aaa", 2, ""), "aaa");
        ASSERT_EQ(view::insert("aaa", 3, ""), "aaa");
        ASSERT_EQ(view::insert("aaa", 4, ""), "aaa");
        ASSERT_EQ(view::insert("aaa", view::npos, ""), "aaa");
    }
    SECTION("插入到空串") {
        ASSERT_EQ(view::insert("", 0, ""), ""); // 将空串插入到空串
        ASSERT_EQ(view::insert("", 0, "bbb"), "bbb");
        ASSERT_EQ(view::insert("", 1, "bbb"), "bbb");
        ASSERT_EQ(view::insert("", view::npos, "bbb"), "bbb");
    }
    SECTION("插入单个字符") {
        ASSERT_EQ(view::insert("aaa", 0, 'B'), "Baaa");
        ASSERT_EQ(view::insert("aaa", 1, 'B'), "aBaa");
        ASSERT_EQ(view::insert("aaa", 2, 'B'), "aaBa");
        ASSERT_EQ(view::insert("aaa", 3, 'B'), "aaaB");
        ASSERT_EQ(view::insert("aaa", 4, 'B'), "aaaB");
        ASSERT_EQ(view::insert("aaa", view::npos, 'B'), "aaaB");
        ASSERT_EQ(view::insert("", 0, 'B'), "B");
        ASSERT_EQ(view::insert("", 9, 'B'), "B");
    }
    SECTION("重复插入一个字符") {
        ASSERT_EQ(view::insert("aaa", 0, 'B', 3), "BBBaaa");
        ASSERT_EQ(view::insert("aaa", 1, 'B', 3), "aBBBaa");
        ASSERT_EQ(view::insert("aaa", 2, 'B', 3), "aaBBBa");
        ASSERT_EQ(view::insert("aaa", 3, 'B', 3), "aaaBBB");
        ASSERT_EQ(view::insert("aaa", 4, 'B', 3), "aaaBBB");
        ASSERT_EQ(view::insert("aaa", view::npos, 'B', 3), "aaaBBB");
        ASSERT_EQ(view::insert("", 0, 'B', 3), "BBB");
        ASSERT_EQ(view::insert("", 9, 'B', 3), "BBB");
        ASSERT_EQ(view::insert("", 0, 'B', 0), "");
        ASSERT_EQ(view::insert("aaa", 2, 'B', 0), "aaa");
    }
    SECTION("通过proc提供数据:一般") {
        std::vector<std::string_view> items{
            "Hello",
            "World",
            "",
            "!!!",
        };
        view::size_type index = 0;
        std::string result = view::insert("abc", 2, [&items, &index]() -> std::optional<std::string_view> {
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
        view::size_type index = 0;
        std::string result = view::insert("abc", 5, [&items, &index]() -> std::optional<std::string_view> {
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
        view::size_type index = 0;
        std::string result = view::insert("abc", view::npos, [&items, &index]() -> std::optional<std::string_view> {
            if (index >= items.size()) {
                return std::nullopt;
            }

            return items[index++];
        });
        ASSERT_EQ(result, "abcHelloWorld!!!");
    }
    SECTION("通过proc提供数据:立即终止") {
        std::string result = view::insert("abc", 2, []() -> std::optional<std::string_view> {
            return std::nullopt;
        });
        ASSERT_EQ(result, "abc");
    }
}