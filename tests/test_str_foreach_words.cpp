/**
 * Copyright (c) 2021-2024 libbylg@126.com
 * str is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "testing.hpp"

#include "str.hpp"

#include <array>

TEST(test_str, foreach_words) {

    // [无pos+view模式] ====================================

    SECTION("无pos+view模式:简单场景") {
        std::string result;
        str::foreach_words("\r\n\t A\r\n\t B\r\n\t C\r\n\t ", [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "ABC");
    }
    SECTION("无pos+view模式:就一个单词") {
        std::string result;
        str::foreach_words("DEF", [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "DEF");
    }
    SECTION("无pos+view模式:多个单词1") {
        std::string result;
        str::foreach_words("ABC \r\nDEF MPQ  ", [&result](std::string_view word) -> int {
            result.append(word).append("|");
            return 0;
        });
        ASSERT_EQ(result, "ABC|DEF|MPQ|");
    }
    SECTION("无pos+view模式:多个单词2") {
        std::string result;
        str::foreach_words("   ABC   DEF GHI", [&result](std::string_view word) -> int {
            result.append(word).append("|");
            return 0;
        });
        ASSERT_EQ(result, "ABC|DEF|GHI|");
    }
    SECTION("无pos+view模式:空串") {
        std::string result;
        str::foreach_words("", [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "");
    }
    SECTION("无pos+view模式:全空白") {
        std::string result;
        str::foreach_words("\r\n\t \n\t ", [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "");
    }

    // [使用pos+view模式] ====================================

    SECTION("使用pos+view模式:正常情况1") {
        std::string result;
        str::foreach_words("   ABC   DEF GHI", 0, [&result](std::string_view word) -> int {
            result.append(word).append("|");
            return 0;
        });
        ASSERT_EQ(result, "ABC|DEF|GHI|");
    }
    SECTION("使用pos+view模式:正常情况2") {
        std::string result;
        str::foreach_words("   ABC   DEF GHI", 4, [&result](std::string_view word) -> int {
            result.append(word).append("|");
            return 0;
        });
        ASSERT_EQ(result, "BC|DEF|GHI|");
    }
    SECTION("使用pos+view模式:正常情况2") {
        std::string result;
        str::foreach_words("   ABC   DEF GHI", 6, [&result](std::string_view word) -> int {
            result.append(word).append("|");
            return 0;
        });
        ASSERT_EQ(result, "DEF|GHI|");
    }
    SECTION("使用pos+view模式:正常情况3") {
        std::string result;
        str::foreach_words("   ABC   DEF GHI", 15, [&result](std::string_view word) -> int {
            result.append(word).append("|");
            return 0;
        });
        ASSERT_EQ(result, "I|");
    }
    SECTION("使用pos+range模式:提前结束") {
        std::vector<std::string> result;
        str::foreach_words("   ABC   DEF GHI", 4, [&result](std::string_view word) -> int {
            result.push_back(std::string{word});
            if (result.size() == 2) {
                return -1;
            }
            return 0;
        });
        ASSERT_EQ(result.size(), 2);
        ASSERT_EQ(result[0], "BC");
        ASSERT_EQ(result[1], "DEF");
    }
    SECTION("使用pos+view模式:指定pos超出范围") {
        std::string result;
        str::foreach_words("   ABC   DEF GHI", 16, [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "");
    }
    SECTION("使用pos+view模式:指定npos") {
        std::string result;
        str::foreach_words("   ABC   DEF GHI", str::npos, [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        ASSERT_EQ(result, "");
    }

    // [无pos+range模式] ====================================

    SECTION("无pos+range模式:简单场景") {
        std::vector<str::range_type> result;
        str::foreach_words("\r\n\t A\r\n\t B\r\n\t C\r\n\t ", [&result](str::range_type range) -> int {
            result.push_back(range);
            return 0;
        });
        ASSERT_EQ(result.size(), 3);
        ASSERT_EQ(result[0], str::range(4, 1));
        ASSERT_EQ(result[1], str::range(9, 1));
        ASSERT_EQ(result[2], str::range(14, 1));
    }
    SECTION("无pos+range模式:就一个单词") {
        std::vector<str::range_type> result;
        str::foreach_words("DEF", [&result](str::range_type range) -> int {
            result.push_back(range);
            return 0;
        });
        ASSERT_EQ(result.size(), 1);
        ASSERT_EQ(result[0], str::range(0, 3));
    }
    SECTION("无pos+range模式:多个单词1") {
        std::vector<str::range_type> result;
        str::foreach_words("ABC \r\nDEF MPQ  ", [&result](str::range_type range) -> int {
            result.push_back(range);
            return 0;
        });
        ASSERT_EQ(result.size(), 3);
        ASSERT_EQ(result[0], str::range(0, 3));
        ASSERT_EQ(result[1], str::range(6, 3));
        ASSERT_EQ(result[2], str::range(10, 3));
    }
    SECTION("无pos+range模式:多个单词2") {
        std::vector<str::range_type> result;
        str::foreach_words("   ABC   DEF GHI", [&result](str::range_type range) -> int {
            result.push_back(range);
            return 0;
        });
        ASSERT_EQ(result.size(), 3);
        ASSERT_EQ(result[0], str::range(3, 3));
        ASSERT_EQ(result[1], str::range(9, 3));
        ASSERT_EQ(result[2], str::range(13, 3));
    }
    SECTION("无pos+range模式:空串") {
        std::vector<str::range_type> result;
        str::foreach_words("", [&result](str::range_type range) -> int {
            result.push_back(range);
            return 0;
        });
        ASSERT_EQ(result.size(), 0);
    }
    SECTION("无pos+range模式:全空白") {
        std::vector<str::range_type> result;
        str::foreach_words("\r\n\t \n\t ", [&result](str::range_type range) -> int {
            result.push_back(range);
            return 0;
        });
        ASSERT_EQ(result.size(), 0);
    }

    /////////////////////////////////////////////////////////

    SECTION("使用pos+range模式:正常情况1") {
        std::vector<str::range_type> result;
        str::foreach_words("   ABC   DEF GHI", 0, [&result](str::range_type range) -> int {
            result.push_back(range);
            return 0;
        });
        ASSERT_EQ(result.size(), 3);
        ASSERT_EQ(result[0], str::range(3, 3));
        ASSERT_EQ(result[1], str::range(9, 3));
        ASSERT_EQ(result[2], str::range(13, 3));
    }
    SECTION("使用pos+range模式:正常情况2") {
        std::vector<str::range_type> result;
        str::foreach_words("   ABC   DEF GHI", 4, [&result](str::range_type range) -> int {
            result.push_back(range);
            return 0;
        });
        ASSERT_EQ(result.size(), 3);
        ASSERT_EQ(result[0], str::range(4, 2));
        ASSERT_EQ(result[1], str::range(9, 3));
        ASSERT_EQ(result[2], str::range(13, 3));
    }
    SECTION("使用pos+range模式:正常情况2") {
        std::vector<str::range_type> result;
        str::foreach_words("   ABC   DEF GHI", 6, [&result](str::range_type range) -> int {
            result.push_back(range);
            return 0;
        });
        ASSERT_EQ(result.size(), 2);
        ASSERT_EQ(result[0], str::range(9, 3));
        ASSERT_EQ(result[1], str::range(13, 3));
    }
    SECTION("使用pos+range模式:正常情况3") {
        std::vector<str::range_type> result;
        str::foreach_words("   ABC   DEF GHI", 15, [&result](str::range_type range) -> int {
            result.push_back(range);
            return 0;
        });
        ASSERT_EQ(result.size(), 1);
        ASSERT_EQ(result[0], str::range(15, 1));
    }
    SECTION("使用pos+range模式:提前结束") {
        std::vector<str::range_type> result;
        str::foreach_words("   ABC   DEF GHI", 4, [&result](str::range_type range) -> int {
            result.push_back(range);
            if (result.size() == 2) {
                return -1;
            }
            return 0;
        });
        ASSERT_EQ(result.size(), 2);
        ASSERT_EQ(result[0], str::range(4, 2));
        ASSERT_EQ(result[1], str::range(9, 3));
    }
    SECTION("使用pos+range模式:指定pos超出范围") {
        std::vector<str::range_type> result;
        str::foreach_words("   ABC   DEF GHI", 16, [&result](str::range_type range) -> int {
            result.push_back(range);
            return 0;
        });
        ASSERT_EQ(result.size(), 0);
    }
    SECTION("使用pos+range模式:指定npos") {
        std::vector<str::range_type> result;
        str::foreach_words("   ABC   DEF GHI", str::npos, [&result](str::range_type range) -> int {
            result.push_back(range);
            return 0;
        });
        ASSERT_EQ(result.size(), 0);
    }
}