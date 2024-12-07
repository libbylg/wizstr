#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::append") {
    SECTION("一般情况") {
        REQUIRE(view::append("aaa", "bbb") == "aaabbb");
    }
    SECTION("空串") {
        REQUIRE(view::append("aaa", "") == "aaa");
        REQUIRE(view::append("", "aaa") == "aaa");
        REQUIRE(view::append("", "") == "");
    }
    SECTION("追加字符") {
        REQUIRE(view::append("aaa", 'A') == "aaaA");
        REQUIRE(view::append("", 'A') == "A");
        REQUIRE(view::append("", '\0') == std::string_view{"\0", 1});
    }
    SECTION("追加重复的字符") {
        REQUIRE(view::append("aaa", 'A', 3) == "aaaAAA");
        REQUIRE(view::append("", 'A', 3) == "AAA");
        REQUIRE(view::append("", '\0', 3) == std::string_view{"\0\0\0", 3});
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
        REQUIRE(result == "abcHelloWorld!!!");
    }
    SECTION("通过proc提供数据:立即终止") {
        std::string result = view::append("abc", []() -> std::optional<std::string_view> {
            return std::nullopt;
        });
        REQUIRE(result == "abc");
    }
}

TEST_CASE("view::prepend") {
    SECTION("一般情况") {
        REQUIRE(view::prepend("aaa", "bbb") == "bbbaaa");
    }
    SECTION("空串") {
        REQUIRE(view::prepend("aaa", "") == "aaa");
        REQUIRE(view::prepend("", "aaa") == "aaa");
        REQUIRE(view::prepend("", "") == "");
    }
    SECTION("追加字符") {
        REQUIRE(view::prepend("aaa", 'A') == "Aaaa");
        REQUIRE(view::prepend("", 'A') == "A");
        REQUIRE(view::prepend("", '\0') == std::string_view{"\0", 1});
    }
    SECTION("追加重复的字符") {
        REQUIRE(view::prepend("aaa", 'A', 3) == "AAAaaa");
        REQUIRE(view::prepend("", 'A', 3) == "AAA");
        REQUIRE(view::prepend("", '\0', 3) == std::string_view{"\0\0\0", 3});
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
        REQUIRE(result == "!!!WorldHelloabc");
    }
    SECTION("通过proc提供数据:立即终止") {
        std::string result = view::prepend("abc", []() -> std::optional<std::string_view> {
            return std::nullopt;
        });
        REQUIRE(result == "abc");
    }
}

TEST_CASE("view::insert") {
    SECTION("一般情况") {
        REQUIRE(view::insert("aaa", 0, "bbb") == "bbbaaa");
        REQUIRE(view::insert("aaa", 1, "bbb") == "abbbaa");
        REQUIRE(view::insert("aaa", 2, "bbb") == "aabbba");
        REQUIRE(view::insert("aaa", 3, "bbb") == "aaabbb");
        REQUIRE(view::insert("aaa", 4, "bbb") == "aaabbb");
        REQUIRE(view::insert("aaa", view::npos, "bbb") == "aaabbb");
    }
    SECTION("插入空串") {
        REQUIRE(view::insert("aaa", 0, "") == "aaa");
        REQUIRE(view::insert("aaa", 1, "") == "aaa");
        REQUIRE(view::insert("aaa", 2, "") == "aaa");
        REQUIRE(view::insert("aaa", 3, "") == "aaa");
        REQUIRE(view::insert("aaa", 4, "") == "aaa");
        REQUIRE(view::insert("aaa", view::npos, "") == "aaa");
    }
    SECTION("插入到空串") {
        REQUIRE(view::insert("", 0, "") == ""); // 将空串插入到空串
        REQUIRE(view::insert("", 0, "bbb") == "bbb");
        REQUIRE(view::insert("", 1, "bbb") == "bbb");
        REQUIRE(view::insert("", view::npos, "bbb") == "bbb");
    }
    SECTION("插入单个字符") {
        REQUIRE(view::insert("aaa", 0, 'B') == "Baaa");
        REQUIRE(view::insert("aaa", 1, 'B') == "aBaa");
        REQUIRE(view::insert("aaa", 2, 'B') == "aaBa");
        REQUIRE(view::insert("aaa", 3, 'B') == "aaaB");
        REQUIRE(view::insert("aaa", 4, 'B') == "aaaB");
        REQUIRE(view::insert("aaa", view::npos, 'B') == "aaaB");
        REQUIRE(view::insert("", 0, 'B') == "B");
        REQUIRE(view::insert("", 9, 'B') == "B");
    }
    SECTION("重复插入一个字符") {
        REQUIRE(view::insert("aaa", 0, 'B', 3) == "BBBaaa");
        REQUIRE(view::insert("aaa", 1, 'B', 3) == "aBBBaa");
        REQUIRE(view::insert("aaa", 2, 'B', 3) == "aaBBBa");
        REQUIRE(view::insert("aaa", 3, 'B', 3) == "aaaBBB");
        REQUIRE(view::insert("aaa", 4, 'B', 3) == "aaaBBB");
        REQUIRE(view::insert("aaa", view::npos, 'B', 3) == "aaaBBB");
        REQUIRE(view::insert("", 0, 'B', 3) == "BBB");
        REQUIRE(view::insert("", 9, 'B', 3) == "BBB");
        REQUIRE(view::insert("", 0, 'B', 0) == "");
        REQUIRE(view::insert("aaa", 2, 'B', 0) == "aaa");
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
        REQUIRE(result == "abHelloWorld!!!c");
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
        REQUIRE(result == "abcHelloWorld!!!");
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
        REQUIRE(result == "abcHelloWorld!!!");
    }
    SECTION("通过proc提供数据:立即终止") {
        std::string result = view::insert("abc", 2, []() -> std::optional<std::string_view> {
            return std::nullopt;
        });
        REQUIRE(result == "abc");
    }
}