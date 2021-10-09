//
// Created by luolijun on 2021/10/9.
//

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
        return !!(classify(ch) | mask);
    }

    static uint16_t mapping[256];
};

} // namespace tiny
#endif // CHARS_H
