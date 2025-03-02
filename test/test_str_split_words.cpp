#include "tester.hpp"

#include "str.hpp"

TEST(test_view, split_words) {
    SECTION("一般情况") {
        ASSERT_EQ(str::split_words("Hello World patppy boy"), (std::vector<std::string_view>{"Hello", "World", "patppy", "boy"}));
        ASSERT_EQ(str::split_words("Hello\r World\v patppy \t \n boy"), (std::vector<std::string_view>{"Hello", "World", "patppy", "boy"}));
    }
    SECTION("无任何空白情况") {
        ASSERT_EQ(str::split_words("Hello-World-patppy-boy"), (std::vector<std::string_view>{"Hello-World-patppy-boy"}));
    }
    SECTION("首尾有空白") {
        ASSERT_EQ(str::split_words("Hello \rWorld\n "), (std::vector<std::string_view>{"Hello", "World"}));
        ASSERT_EQ(str::split_words(" \r\v\t\n Hello \rWorld\n "), (std::vector<std::string_view>{"Hello", "World"}));
        ASSERT_EQ(str::split_words(" \r\v\t\n Hello \rWorld"), (std::vector<std::string_view>{"Hello", "World"}));
    }
    SECTION("空串") {
        ASSERT_EQ(str::split_words(""), (std::vector<std::string_view>{}));
        ASSERT_EQ(str::split_words("", 0), (std::vector<std::string_view>{}));
        ASSERT_EQ(str::split_words("", 1), (std::vector<std::string_view>{}));
        ASSERT_EQ(str::split_words("", str::npos), (std::vector<std::string_view>{}));
    }
    SECTION("全空白串") {
        ASSERT_EQ(str::split_words(" \t\v\r\n "), (std::vector<std::string_view>{}));
        ASSERT_EQ(str::split_words(" \t\v\r\n ", 0), (std::vector<std::string_view>{}));
        ASSERT_EQ(str::split_words(" \t\v\r\n ", 1), (std::vector<std::string_view>{}));
        ASSERT_EQ(str::split_words(" \t\v\r\n ", str::npos), (std::vector<std::string_view>{}));
    }
    SECTION("限制数据量") {
        ASSERT_EQ(str::split_words("Hello World patppy boy", 0), (std::vector<std::string_view>{}));
        ASSERT_EQ(str::split_words("Hello World patppy boy", 1), (std::vector<std::string_view>{"Hello"}));
        ASSERT_EQ(str::split_words("Hello World patppy boy", 2), (std::vector<std::string_view>{"Hello", "World"}));
        ASSERT_EQ(str::split_words("Hello World patppy boy", 3), (std::vector<std::string_view>{"Hello", "World", "patppy"}));
        ASSERT_EQ(str::split_words("Hello World patppy boy", 4), (std::vector<std::string_view>{"Hello", "World", "patppy", "boy"}));
        ASSERT_EQ(str::split_words("Hello World patppy boy", 5), (std::vector<std::string_view>{"Hello", "World", "patppy", "boy"}));
        ASSERT_EQ(str::split_words("Hello World patppy boy", str::npos), (std::vector<std::string_view>{"Hello", "World", "patppy", "boy"}));
    }
}
