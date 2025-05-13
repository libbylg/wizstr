#include "testing.hpp"

#include "str.hpp"

#include <array>

TEST(test_str, foreach_lines) {
    SECTION("一般场景:CR+LF") {
        std::string result;
        str::foreach_lines("\r\n\t A\r\n\t B\r\n\t C\r\n\t ", true,
            [&result]([[maybe_unused]] str::size_type line_index, std::string_view line_text) -> int {
                result.append(line_text).append("|");
                return 0;
            });
        ASSERT_EQ(result, "\r\n|\t A\r\n|\t B\r\n|\t C\r\n|\t |");
    }
    SECTION("一般场景:CR") {
        std::string result;
        str::foreach_lines("\r\t A\r\t B\r\t C\r\t ", true,
            [&result]([[maybe_unused]] str::size_type line_index, std::string_view line_text) -> int {
                result.append(line_text).append("|");
                return 0;
            });
        ASSERT_EQ(result, "\r|\t A\r|\t B\r|\t C\r|\t |");
    }
    SECTION("一般场景:空串") {
        std::string result;
        str::foreach_lines("", true,
            [&result]([[maybe_unused]] str::size_type line_index, std::string_view line_text) -> int {
                result.append(line_text).append("|");
                return 0;
            });
        ASSERT_EQ(result, "");
    }
    SECTION("一般场景:无换行") {
        std::string result;
        str::foreach_lines("abcd", true,
            [&result]([[maybe_unused]] str::size_type line_index, std::string_view line_text) -> int {
                result.append(line_text).append("|");
                return 0;
            });
        ASSERT_EQ(result, "abcd|");
    }
    SECTION("一般场景:提前结束") {
        std::string result;
        int32_t n = 0;
        str::foreach_lines("a\rb\r\nc\rd\n", true,
            [&n, &result]([[maybe_unused]] str::size_type line_index, std::string_view line_text) -> int {
                if (n++ == 2) {
                    return -1;
                }
                result.append(line_text).append("|");
                return 0;
            });
        ASSERT_EQ(result, "a\r|b\r\n|");
    }
    SECTION("一般场景:不要结束符") {
        std::string result;
        str::foreach_lines("a\rb\r\nc\rd", false,
            [&result]([[maybe_unused]] str::size_type line_index, std::string_view line_text) -> int {
                result.append(line_text).append("|");
                return 0;
            });
        ASSERT_EQ(result, "a|b|c|d|");
    }
}