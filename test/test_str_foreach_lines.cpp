#include "testing.hpp"

#include "str.hpp"

#include <array>

TEST(test_str, foreach_lines) {
    SECTION("一般场景:1") {
        std::string result;
        str::foreach_lines("\r\n\t A\r\n\t B\r\n\t C\r\n\t ", true,
            [&result]([[maybe_unused]] str::size_type line_index, std::string_view line_text) -> int {
                result.append(line_text).append("|");
                return 0;
            });
        ASSERT_EQ(result, "\r\n|\t A\r\n|\t B\r\n|\t C\r\n|\t |");
    }
}