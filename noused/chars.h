/**
 * Copyright (c) 2021-2021 libbylg@126.com
 * tiny is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#ifndef CHARS_H
#define CHARS_H

#include <cstdint>

namespace tiny {

struct chars {
    enum : uint16_t {
        // clang-format off
        CNTRL   = 0x0001, //  控制字符
        PRINT   = 0x0002, //  可打印字符
        BLANK   = 0x0004, //  空格字符：tab 和 空格
        SPACE   = 0x0008, //  空白字符：tab、空格、垂直tab、回车
        LOWER   = 0x0010, //  小写字母：[a-z]
        UPPER   = 0x0020, //  大写字母：[A-Z]
        PUNCT   = 0x0040, //  图像字符：PRINT 中除空格外的字符
        DIGIT   = 0x0100, //  数字：[0-9]
        HEX     = 0x0080, //  十六进制数字：[0-9A-Fa-f]
        OCT     = 0x0200, //  八进制数字：[0-7]
        BIN     = 0x0400, //  二进制数字：[0-1]
        // clang-format on

        DEC = DIGIT,           //  十进制数字：[0-9]
        ALPHA = LOWER | UPPER, //  大小写字母：[A-Za-z]
        ALNUM = ALPHA | DIGIT, //  字母：[0-9A-Za-z]
        GRAPH = ALPHA | PUNCT, //  图像字符：PRINT 中除空格外的字符
    };

    static inline uint16_t classify(char ch) {
        return chars::mapping[uint8_t(ch)];
    }

    static inline bool match(char ch, uint16_t mask) {
        return !!(classify(ch) & mask);
    }

    static inline bool not_match(char ch, uint16_t mask) {
        return !(classify(ch) & mask);
    }

    static inline bool is_lower(char ch) {
        return match(ch, LOWER);
    }

    static inline bool is_upper(char ch) {
        return match(ch, UPPER);
    }

    static inline bool is_digit(char ch) {
        return match(ch, DIGIT);
    }

    static inline bool is_alpha(char ch) {
        return match(ch, ALPHA);
    }

    static inline bool is_alnum(char ch) {
        return match(ch, ALNUM);
    }

    static inline bool is_space(char ch) {
        return match(ch, SPACE);
    }

    static inline bool is_blank(char ch) {
        return match(ch, BLANK);
    }

    static inline bool is_print(char ch) {
        return match(ch, PRINT);
    }

    static inline bool is_graph(char ch) {
        return match(ch, GRAPH);
    }

    static inline bool is_punct(char ch) {
        return match(ch, PUNCT);
    }

    static inline char to_upper(char ch) {
        if (is_lower(ch)) {
            return ch - ('a' - 'A');
        }

        return ch;
    }

    static inline char to_lower(char ch) {
        if (is_lower(ch)) {
            return ch + ('a' - 'A');
        }

        return ch;
    }

    static uint16_t mapping[256];
    static const char* ascii_letters;
    static const char* ascii_lowercase;
    static const char* ascii_uppercase;
    static const char* digits;
    static const char* hexdigits;
    static const char* hexdigits_lowercase;
    static const char* hexdigits_uppercase;
    static const char* octdigits;
    static const char* punctuation;
    static const char* printable;
    static const char* whitespace;
    static const char* blanks;
};

} // namespace tiny
#endif // CHARS_H
