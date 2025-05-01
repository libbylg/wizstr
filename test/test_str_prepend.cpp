#include "testing.hpp"

#include "str.hpp"

TEST(test_str, prepend) {
    SECTION("普通字符串") {
        ASSERT_EQ(str::prepend("aaa", "bbb", 3), "bbbbbbbbbaaa");
        ASSERT_EQ(str::prepend("aaa", "bbb", 0), "aaa");
        ASSERT_EQ(str::prepend("aaa", "bbb"), "bbbaaa");
        ASSERT_EQ(str::prepend("", "bbb"), "bbb");
        ASSERT_EQ(str::prepend("bbb", ""), "bbb");
        ASSERT_EQ(str::prepend("", ""), "");
        ASSERT_EQ(str::prepend("", "", 3), "");
    }
    SECTION("给定容器") {
        ASSERT_EQ(str::prepend("aaa", std::array{"bbb", "ccc"}), "cccbbbaaa");
        ASSERT_EQ(str::prepend("aaa", std::vector<const char*>{}), "aaa");
        ASSERT_EQ(str::prepend("aaa", {}), "aaa");
        ASSERT_EQ(str::prepend("aaa", {""}), "aaa");
        ASSERT_EQ(str::prepend("", {"aaa", "", "def"}), "defaaa");
        ASSERT_EQ(str::prepend("", {"aaa", "QQ", "def"}), "defQQaaa");
        ASSERT_EQ(str::prepend("", {""}), "");
    }
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
