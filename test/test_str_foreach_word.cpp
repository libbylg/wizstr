#include "tester.hpp"

#include "str.hpp"


#include <array>

TEST(test_str, foreach_words) {
    SECTION("一般场景:1") {
        std::string result;
        str::foreach_words("\r\n\t A\r\n\t B\r\n\t C\r\n\t ", [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "ABC");
    }
    SECTION("一般场景:2") {
        std::string result;
        str::foreach_words("DEF", [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "DEF");
    }
    SECTION("就只有一个单词") {
        std::string result;
        str::foreach_words("DEF", [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "DEF");
    }
    SECTION("空串:1") {
        std::string result;
        str::foreach_words("", [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "");
    }
    SECTION("空串:2") {
        std::string result;
        str::foreach_words("", 1, [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "");
    }
    SECTION("全空白") {
        std::string result;
        str::foreach_words("\r\n\t \n\t ", 1, [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "");
    }
    SECTION("使用pos:正常情况") {
        std::string result;
        str::foreach_words("   ABC   DEF GHI", 6, [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "DEFGHI");
    }
    SECTION("使用pos:指定pos超出范围") {
        std::string result;
        str::foreach_words("   ABC   DEF GHI", 16, [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "");
    }
    SECTION("使用pos:npos") {
        std::string result;
        str::foreach_words("   ABC   DEF GHI", str::npos, [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "");
    }
    SECTION("返回pos和n:1") {
        std::vector<std::string_view> result;
        str::foreach_words("   ABC   DEF GHXWYI", [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });
        ASSERT_EQ(result.size(), 3);
        ASSERT_EQ(result[0], "ABC");
        ASSERT_EQ(result[1], "DEF");
        ASSERT_EQ(result[2], "GHXWYI");
    }
    SECTION("返回pos和n:2") {
        std::vector<std::string_view> result;
        str::foreach_words("   ABC   DEF GHXWYI", 4, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });
        ASSERT_EQ(result.size(), 3);
        ASSERT_EQ(result[0], "ABC");
        ASSERT_EQ(result[1], "DEF");
        ASSERT_EQ(result[2], "GHXWYI");
    }
    SECTION("返回提前结束") {
        std::vector<std::string_view> result;
        str::foreach_words("   ABC   DEF GHXWYI", 4, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            if (item == "DEF") {
                return -1;
            }
            return 0;
        });
        ASSERT_EQ(result.size(), 2);
        ASSERT_EQ(result[0], "ABC");
        ASSERT_EQ(result[1], "DEF");
    }
}