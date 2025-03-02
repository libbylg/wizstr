#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, split_lines_keep_ends_false) {
    SECTION("一般情况:\\n") {
        std::string_view s{"a\nb\nc"};
        std::vector<std::string_view> expect{
            "a",
            "b",
            "c",
        };
        ASSERT_EQ(view::split_lines(s), expect);
    }
    SECTION("一般情况:\\r\\n") {
        std::string_view s{"a\r\nb\r\nc"};
        std::vector<std::string_view> expect{
            "a",
            "b",
            "c",
        };
        ASSERT_EQ(view::split_lines(s), expect);
    }
    SECTION("一般情况:\\r") {
        std::string_view s{"a\rb\rc"};
        std::vector<std::string_view> expect{
            "a",
            "b",
            "c",
        };
        ASSERT_EQ(view::split_lines(s), expect);
    }
    SECTION("一般情况:\\r收尾") {
        std::string_view s{"a\nb\nc\r"};
        std::vector<std::string_view> expect{
            "a",
            "b",
            "c",
        };
        ASSERT_EQ(view::split_lines(s), expect);
    }
    SECTION("一般情况:\\n收尾") {
        std::string_view s{"a\nb\nc\n"};
        std::vector<std::string_view> expect{
            "a",
            "b",
            "c",
        };
        ASSERT_EQ(view::split_lines(s), expect);
    }
    SECTION("一般情况:\\r\\n收尾") {
        std::string_view s{"a\rb\rc\r\n"};
        std::vector<std::string_view> expect{
            "a",
            "b",
            "c",
        };
        ASSERT_EQ(view::split_lines(s), expect);
    }
    SECTION("连续:\\r") {
        std::string_view s{"a\r\rb\r\rc"};
        std::vector<std::string_view> expect{
            "a",
            "",
            "b",
            "",
            "c",
        };
        std::vector<std::string_view> actual = view::split_lines(s);
        ASSERT_EQ(actual, expect);
    }
    SECTION("连续:\\n") {
        std::string_view s{"a\n\nb\n\nc"};
        std::vector<std::string_view> expect{
            "a",
            "",
            "b",
            "",
            "c",
        };
        ASSERT_EQ(view::split_lines(s), expect);
    }
    SECTION("单一:\\r") {
        std::string_view s{"\r"};
        std::vector<std::string_view> expect{
            "",
        };
        ASSERT_EQ(view::split_lines(s), expect);
    }
    SECTION("单一:\\n") {
        std::string_view s{"\n"};
        std::vector<std::string_view> expect{
            "",
        };
        ASSERT_EQ(view::split_lines(s), expect);
    }
    SECTION("空串") {
        std::string_view s{""};
        std::vector<std::string_view> expect{};
        ASSERT_EQ(view::split_lines(s), expect);
    }
    SECTION("无任何换行符号") {
        std::string_view s{"abcdef"};
        std::vector<std::string_view> expect{"abcdef"};
        ASSERT_EQ(view::split_lines(s), expect);
    }
    SECTION("proc:提前结束") {
        std::string_view s{"a\nb\nc\nd\ne\nf"};
        std::vector<std::string_view> expect{"a\n", "b\n"};
        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            if (result.size() >= 2) {
                return -1;
            }
            return 0;
        });
        ASSERT_EQ(result, expect);
    }
}

TEST(test_view, split_lines_keep_ends_true) {
    SECTION("一般情况:\\n") {
        std::string_view s{"a\nb\nc"};
        std::vector<std::string_view> expect{
            "a\n",
            "b\n",
            "c",
        };
        ASSERT_EQ(view::split_lines(s, true), expect);
    }
    SECTION("一般情况:\\r\\n") {
        std::string_view s{"a\r\nb\r\nc"};
        std::vector<std::string_view> expect{
            "a\r\n",
            "b\r\n",
            "c",
        };
        ASSERT_EQ(view::split_lines(s, true), expect);
    }
    SECTION("一般情况:\\r") {
        std::string_view s{"a\rb\rc"};
        std::vector<std::string_view> expect{
            "a\r",
            "b\r",
            "c",
        };
        ASSERT_EQ(view::split_lines(s, true), expect);
    }
    SECTION("一般情况:\\r收尾") {
        std::string_view s{"a\nb\nc\r"};
        std::vector<std::string_view> expect{
            "a\n",
            "b\n",
            "c\r",
        };
        ASSERT_EQ(view::split_lines(s, true), expect);
    }
    SECTION("一般情况:\\n收尾") {
        std::string_view s{"a\nb\nc\n"};
        std::vector<std::string_view> expect{
            "a\n",
            "b\n",
            "c\n",
        };
        ASSERT_EQ(view::split_lines(s, true), expect);
    }
    SECTION("一般情况:\\r\\n收尾") {
        std::string_view s{"a\rb\rc\r\n"};
        std::vector<std::string_view> expect{
            "a\r",
            "b\r",
            "c\r\n",
        };
        ASSERT_EQ(view::split_lines(s, true), expect);
    }
    SECTION("连续:\\r") {
        std::string_view s{"a\r\rb\r\rc"};
        std::vector<std::string_view> expect{
            "a\r",
            "\r",
            "b\r",
            "\r",
            "c",
        };
        ASSERT_EQ(view::split_lines(s, true), expect);
    }
    SECTION("连续:\\n") {
        std::string_view s{"a\n\nb\n\nc"};
        std::vector<std::string_view> expect{
            "a\n",
            "\n",
            "b\n",
            "\n",
            "c",
        };
        ASSERT_EQ(view::split_lines(s, true), expect);
    }
    SECTION("单一:\\r") {
        std::string_view s{"\r"};
        std::vector<std::string_view> expect{
            "\r",
        };
        ASSERT_EQ(view::split_lines(s, true), expect);
    }
    SECTION("单一:\\n") {
        std::string_view s{"\n"};
        std::vector<std::string_view> expect{
            "\n",
        };
        ASSERT_EQ(view::split_lines(s, true), expect);
    }
    SECTION("空串") {
        std::string_view s{""};
        std::vector<std::string_view> expect{};
        ASSERT_EQ(view::split_lines(s, true), expect);
    }
    SECTION("无任何换行符号") {
        std::string_view s{"abcdef"};
        std::vector<std::string_view> expect{"abcdef"};
        ASSERT_EQ(view::split_lines(s, true), expect);
    }
}