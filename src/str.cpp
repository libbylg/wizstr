//
// Created by luolijun on 2021/9/30.
//
#include "tiny/str.h"
#include "tiny/asserts.h"
#include "tiny/re.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>

namespace tiny {

enum char_type : uint8_t {
    // clang-format off
    SYMBL = 0x01, //  可打印字符
    BIN   = 0x02, //  二进制数字：[0-1]
    OCT   = 0x04, //  八进制数字：[0-7]
    DEC   = 0x08, //  十进制数字：[0-9]
    HEX   = 0x10, //  16进制数字：[0-9A-Fa-f]
    SPACE = 0x20, //  空白字符：空格、水平tab、垂直tab
    LOWER = 0x40, //  小写字母：[a-z]
    UPPER = 0x80, //  大写字母：[A-Z]
    // clang-format on

    DIGIT = DEC,                           //  数字：[0-9]
    ALPHA = LOWER | UPPER,                 //  大小写字母：[A-Za-z]
    ALNUM = ALPHA | DIGIT,                 //  字母：[0-9A-Za-z]
    PRINT = ALPHA | DIGIT | SYMBL | SPACE, //  所有可打印字符：字母、数字、符号、空白
};

static uint8_t chars_mapping[256] = {
    /*  <NUL>           0x00 */ 0,
    /*  <SOH>           0x01 */ 0,
    /*  <STX>           0x02 */ 0,
    /*  <ETX>           0x03 */ 0,
    /*  <EOT>           0x04 */ 0,
    /*  <ENQ>           0x05 */ 0,
    /*  <ACK>           0x06 */ 0,
    /*  <BEL>    '\a'   0x07 */ 0,
    /*  <BS>            0x08 */ 0,
    /*  <HT>     '\t'   0x09 */ SPACE,
    /*  <LF>     '\n'   0x0a */ SPACE,
    /*  <VT>     '\v'   0x0b */ SPACE,
    /*  <FF>     '\f'   0x0c */ SPACE,
    /*  <CR>     '\r'   0x0d */ SPACE,
    /*  <SO>            0x0e */ 0,
    /*  <SI>            0x0f */ 0,
    /*  <DLE>           0x10 */ 0,
    /*  <DC1>           0x11 */ 0,
    /*  <DC2>           0x12 */ 0,
    /*  <DC3>           0x13 */ 0,
    /*  <DC4>           0x14 */ 0,
    /*  <NAK>           0x15 */ 0,
    /*  <SYN>           0x16 */ 0,
    /*  <ETB>           0x17 */ 0,
    /*  <CAN>           0x18 */ 0,
    /*  <EM>            0x19 */ 0,
    /*  <SUB>           0x1a */ 0,
    /*  <ESC>           0x1b */ 0,
    /*  <FS>            0x1c */ 0,
    /*  <GS>            0x1d */ 0,
    /*  <RS>            0x1e */ 0,
    /*  <US>            0x1f */ 0,
    /*  <SP>     ' '    0x20 */ SPACE,
    /*  -----    '!'    0x21 */ SYMBL,
    /*  -----    '"'    0x22 */ SYMBL,
    /*  -----    '#'    0x23 */ SYMBL,
    /*  -----    '$'    0x24 */ SYMBL,
    /*  -----    '%'    0x25 */ SYMBL,
    /*  -----    '&'    0x26 */ SYMBL,
    /*  -----    '''    0x27 */ SYMBL,
    /*  -----    '('    0x28 */ SYMBL,
    /*  -----    ')'    0x29 */ SYMBL,
    /*  -----    '*'    0x2a */ SYMBL,
    /*  -----    '+'    0x2b */ SYMBL,
    /*  -----    ','    0x2c */ SYMBL,
    /*  -----    '-'    0x2d */ SYMBL,
    /*  -----    '.'    0x2e */ SYMBL,
    /*  -----    '/'    0x2f */ SYMBL,
    /*  -----    '0'    0x30 */ HEX | DEC | OCT | BIN,
    /*  -----    '1'    0x31 */ HEX | DEC | OCT | BIN,
    /*  -----    '2'    0x32 */ HEX | DEC | OCT,
    /*  -----    '3'    0x33 */ HEX | DEC | OCT,
    /*  -----    '4'    0x34 */ HEX | DEC | OCT,
    /*  -----    '5'    0x35 */ HEX | DEC | OCT,
    /*  -----    '6'    0x36 */ HEX | DEC | OCT,
    /*  -----    '7'    0x37 */ HEX | DEC | OCT,
    /*  -----    '8'    0x38 */ HEX | DEC | HEX,
    /*  -----    '9'    0x39 */ HEX | DEC | HEX,
    /*  -----    ':'    0x3a */ SYMBL,
    /*  -----    ';'    0x3b */ SYMBL,
    /*  -----    '<'    0x3c */ SYMBL,
    /*  -----    '='    0x3d */ SYMBL,
    /*  -----    '>'    0x3e */ SYMBL,
    /*  -----    '?'    0x3f */ SYMBL,
    /*  -----    '@'    0x40 */ SYMBL,
    /*  -----    'A'    0x41 */ UPPER | HEX,
    /*  -----    'B'    0x42 */ UPPER | HEX,
    /*  -----    'C'    0x43 */ UPPER | HEX,
    /*  -----    'D'    0x44 */ UPPER | HEX,
    /*  -----    'E'    0x45 */ UPPER | HEX,
    /*  -----    'F'    0x46 */ UPPER | HEX,
    /*  -----    'G'    0x47 */ UPPER,
    /*  -----    'H'    0x48 */ UPPER,
    /*  -----    'I'    0x49 */ UPPER,
    /*  -----    'J'    0x4a */ UPPER,
    /*  -----    'K'    0x4b */ UPPER,
    /*  -----    'L'    0x4c */ UPPER,
    /*  -----    'M'    0x4d */ UPPER,
    /*  -----    'N'    0x4e */ UPPER,
    /*  -----    'O'    0x4f */ UPPER,
    /*  -----    'P'    0x50 */ UPPER,
    /*  -----    'Q'    0x51 */ UPPER,
    /*  -----    'R'    0x52 */ UPPER,
    /*  -----    'S'    0x53 */ UPPER,
    /*  -----    'T'    0x54 */ UPPER,
    /*  -----    'U'    0x55 */ UPPER,
    /*  -----    'V'    0x56 */ UPPER,
    /*  -----    'W'    0x57 */ UPPER,
    /*  -----    'X'    0x58 */ UPPER,
    /*  -----    'Y'    0x59 */ UPPER,
    /*  -----    'Z'    0x5a */ UPPER,
    /*  -----    '['    0x5b */ SYMBL,
    /*  -----    '\'    0x5c */ SYMBL,
    /*  -----    ']'    0x5d */ SYMBL,
    /*  -----    '^'    0x5e */ SYMBL,
    /*  -----    '_'    0x5f */ SYMBL,
    /*  -----    '`'    0x60 */ SYMBL,
    /*  -----    'a'    0x61 */ LOWER | HEX,
    /*  -----    'b'    0x62 */ LOWER | HEX,
    /*  -----    'c'    0x63 */ LOWER | HEX,
    /*  -----    'd'    0x64 */ LOWER | HEX,
    /*  -----    'e'    0x65 */ LOWER | HEX,
    /*  -----    'f'    0x66 */ LOWER | HEX,
    /*  -----    'g'    0x67 */ LOWER,
    /*  -----    'h'    0x68 */ LOWER,
    /*  -----    'i'    0x69 */ LOWER,
    /*  -----    'j'    0x6a */ LOWER,
    /*  -----    'k'    0x6b */ LOWER,
    /*  -----    'l'    0x6c */ LOWER,
    /*  -----    'm'    0x6d */ LOWER,
    /*  -----    'n'    0x6e */ LOWER,
    /*  -----    'o'    0x6f */ LOWER,
    /*  -----    'p'    0x70 */ LOWER,
    /*  -----    'q'    0x71 */ LOWER,
    /*  -----    'r'    0x72 */ LOWER,
    /*  -----    's'    0x73 */ LOWER,
    /*  -----    't'    0x74 */ LOWER,
    /*  -----    'u'    0x75 */ LOWER,
    /*  -----    'v'    0x76 */ LOWER,
    /*  -----    'w'    0x77 */ LOWER,
    /*  -----    'x'    0x78 */ LOWER,
    /*  -----    'y'    0x79 */ LOWER,
    /*  -----    'z'    0x7a */ LOWER,
    /*  -----    '{'    0x7b */ SYMBL,
    /*  -----    '|'    0x7c */ SYMBL,
    /*  -----    '}'    0x7d */ SYMBL,
    /*  -----    '~'    0x7e */ SYMBL,
    /*  <DEL>    ---    0x7f */ 0,
};

//  进制映射范围
enum number_base {
    base_min = 2,
    base_max = 36,
};

//  数据格式化时的映射
static str::const_pointer number_map_upper = "0123456789ABCDEFGHIJKLMNOPQRETUVWXYZ";
static str::const_pointer number_map_lower = "0123456789abcdefghijklmnopqretuvwxyz";

//  反向查找字符串
const char* strrstr(const char* s1, const char* s2) {

    const char* sc1;
    const char* sc2;
    const char* psc1;
    const char* ps1;

    if (*s2 == '\0') {
        return s1;
    }

    ps1 = s1 + strlen(s1);

    while (ps1 != s1) {
        --ps1;
        for (psc1 = ps1, sc2 = s2;;) {
            if (*(psc1++) != *(sc2++)) {
                break;
            }

            if (*sc2 == '\0') {
                return ps1;
            }
        }
    }

    return nullptr;
}

str::str() {
    layout.init(nullptr, 0);
}

str::str(str::const_pointer s, str::size_type n) {
    layout.init(s, n);
}

str::str(str::value_type ch, str::size_type n) {
    layout.init(nullptr, n);
    std::fill(layout.begin(), layout.end(), ch);
}

str::str(str::const_pointer s) {
    ASSERT(s != nullptr);
    layout.init(s, std::strlen(s));
}

str::str(str&& other) noexcept {
    memcpy(&layout, &other.layout, sizeof(other.layout));
    other.layout.init(nullptr, 0);
}

str::str(const str& other) {
    layout.init(other.data(), other.size());
}

str& str::operator=(str&& other) noexcept {
    if (&other == this) {
        return *this;
    }

    layout.destroy();
    memcpy(&layout, &other.layout, sizeof(other.layout));
    other.layout.init(nullptr, 0);

    return *this;
}

str& str::operator=(const str& other) {
    if (&other == this) {
        return *this;
    }

    layout.destroy();
    layout.init(other.layout.begin(), other.layout.len());

    return *this;
}

str::~str() {
    layout.destroy();
}

//  迭代器
str::iterator str::begin() {
    return layout.begin();
}

str::iterator str::end() {
    return layout.end();
}

//  常量迭代器
str::const_iterator str::begin() const {
    return layout.begin();
}

str::const_iterator str::end() const {
    return layout.end();
}

str::reverse_iterator str::rbegin() {
    return str::reverse_iterator(end() - 1);
}

str::reverse_iterator str::rend() {
    return str::reverse_iterator(begin() - 1);
}

str::const_reverse_iterator str::rbegin() const {
    return str::const_reverse_iterator(end() - 1);
}

str::const_reverse_iterator str::rend() const {
    return str::const_reverse_iterator(begin() - 1);
}

str::value_type str::at(str::pos_type pos) const {
    ASSERT(pos >= 0);
    ASSERT(pos < size());
    return begin()[pos];
}

str::value_type str::back() const {
    ASSERT(size() > 0);
    return *(end() - 1);
}

str::value_type& str::back() {
    ASSERT(size() > 0);
    return *(end() - 1);
}

str::value_type str::front() const {
    ASSERT(size() > 0);
    return begin()[0];
}

str::value_type& str::front() {
    ASSERT(size() > 0);
    return begin()[0];
}

str::size_type str::size() const {
    return layout.len();
}

str::size_type str::capacity() const {
    return layout.cap();
}

void str::clear() {
    layout.destroy();
    layout.init(nullptr, 0);
}

str::const_pointer str::data() const {
    return layout.begin();
}

str::pointer str::data() {
    return layout.begin();
}

void str::attach(str::pointer buf, str::size_type len, str::size_type cap) {
    ASSERT(buf != nullptr);
    ASSERT(len >= 0);
    ASSERT(cap >= 0);
    ASSERT(cap >= len);
    layout.destroy();
    layout.large.attach(cap, len, buf);
}

str& str::append(const str& s) {
    str::pos_type pos = layout.len();
    layout.resize(layout.len() + s.size());
    layout.fill(pos, s.data(), s.size());
    return *this;
}

str& str::append(str::value_type ch) {
    str::pos_type pos = layout.len();
    layout.resize(layout.len() + 1);
    layout.fill(pos, &ch, 1);
    return *this;
}

str& str::append(str::const_pointer s, str::size_type n) {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    str::pos_type pos = layout.len();
    layout.resize(layout.len() + n);
    layout.fill(pos, s, n);
    return *this;
}

str& str::append(str::const_pointer s) {
    ASSERT(s != nullptr);
    append(s, std::strlen(s));
    return *this;
}

str& str::prepend(const str& s) {
    ASSERT(this != &s);
    return prepend(s.begin(), s.size());
}

str& str::prepend(str::value_type ch) {
    return prepend(&ch, 1);
}

str& str::prepend(str::const_pointer s, str::size_type n) {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    layout.flexmove(0, layout.len(), n, [](pointer fill, size_type n) {
    });
    layout.fill(0, s, n);
    return *this;
}

str& str::prepend(str::const_pointer s) {
    ASSERT(s != nullptr);
    return prepend(s, std::strlen(s));
}

str& str::insert(str::pos_type pos, const str& str) {
    ASSERT(pos >= 0);
    return insert(pos, str.layout.begin(), layout.len());
}

str& str::insert(str::pos_type pos, str::value_type ch) {
    return insert(pos, &ch, 1);
}

str& str::insert(str::pos_type pos, str::const_pointer s, str::size_type n) {
    ASSERT(pos >= 0);
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    if (pos < layout.len()) {
        layout.flexmove(pos, layout.len() - pos, n, [](pointer fill, size_type n) {
        });
        layout.fill(pos, s, n);
        return *this;
    }

    layout.resize(layout.len() + pos + n);
    layout.fill(pos, s, n);
    return *this;
}

str& str::insert(str::pos_type pos, str::const_pointer s) {
    return insert(pos, s, std::strlen(s));
}

void str::push_back(const str& s) {
    append(s.data(), s.size());
}

void str::push_back(str::value_type ch) {
    append(ch);
}

void str::push_front(const str& s) {
    prepend(s);
}

void str::push_front(str::value_type ch) {
    prepend(ch);
}

str::value_type str::pop_back() {
    ASSERT(size() > 0);
    str::value_type ch = *(end() - 1);
    resize(size() - 1);
    return ch;
}

str::value_type str::pop_front() {
    ASSERT(size() > 0);
    str::value_type ch = *(begin());
    layout.clipmove(0, layout.len(), -1);
    layout.resize(layout.len() - 1);
    return ch;
}

str& str::flex_move(str::pos_type pos, str::size_type n, int offset) {
    layout.flexmove(pos, n, offset, [](pointer fill, size_type n) {
    });
    return *this;
}

str& str::flex_move(str::pos_type pos, str::size_type n, int offset, str::value_type fill_ch) {
    layout.flexmove(pos, n, offset, [fill_ch](pointer fill_pos, size_type n) {
        std::fill(fill_pos, fill_pos + n, fill_ch);
    });
    return *this;
}

str& str::clip_move(str::pos_type pos, str::size_type n, int offset) {
    layout.clipmove(pos, n, offset);
    return *this;
}

str& str::remove(str::pos_type pos) {
    ASSERT(pos >= 0);
    ASSERT(pos < layout.len());
    layout.clipmove(pos + 1, layout.len() - (pos + 1), -1);
    layout.resize(layout.len() - 1);
    return *this;
}

str& str::remove(str::pos_type pos, str::size_type n) {
    ASSERT(n > 0);
    ASSERT(pos >= 0);
    ASSERT(pos < layout.len());
    ASSERT((pos + n) < layout.len());
    if ((pos + n) < layout.len()) {
        layout.clipmove((pos + n), layout.len() - (pos + n), n);
        layout.resize(layout.len() - n);
        return *this;
    }

    layout.resize(pos);
    return *this;
}

str& str::remove(str::value_type ch) {
    return remove([ch](str::value_type c, bool& match) -> int {
        match = (c == ch);
        return 0;
    });
}

str& str::remove(str::const_pointer s) {
    ASSERT(s != nullptr);

    str::size_type slen = std::strlen(s);

    return remove([slen, s](str::const_pointer start, str::const_pointer end, str::const_pointer& match, str::size_type& n) -> int {
        if ((end - start) < slen) {
            match = nullptr;
            n = 0;
            return -1;
        }

        str::const_pointer ptr = std::strstr(start, s);
        if (ptr == nullptr) {
            match = nullptr;
            n = 0;
            return -1;
        }

        match = ptr;
        n = slen;
        return 0;
    });
}

str& str::remove(const str& other) {
    return remove(other.layout.begin());
}

str& str::remove(const re& rx) {
    ASSERT(rx);

    return remove([&rx](str::const_pointer start, str::const_pointer end, str::const_pointer& match_ptr, str::size_type& match_n) -> int {
        if (end == start) {
            match_ptr = nullptr;
            match_n = 0;
            return -1;
        }

        //  从 start 位置开始只查找一个
        int cnt = rx.find(start, 0, [&match_ptr, &match_n](const re::segment_type* segs, re::size_type n) -> int {
            match_ptr = segs[0].start;
            match_n = segs[0].len;
            return -1;
        });

        //  如果搜索不到
        if (cnt <= 0) {
            match_ptr = nullptr;
            match_n = 0;
            return 0;
        }

        //  如果已经搜索到了，直接返回
        return 0;
    });
}

str& str::remove(std::function<int(str::value_type c, bool& match)> func) {
    pointer w = layout.begin();
    pointer r = layout.begin();
    while (*r) {
        bool match = false;
        int contnu = func(*r, match);
        if (!match) {
            *(w++) = *r;
        }

        r++;
    }

    layout.resize(w - layout.begin());

    return *this;
}

str& str::remove(std::function<int(str::const_pointer start, str::const_pointer end, str::const_pointer& match, str::size_type& n)> func) {
    str::pointer w = layout.begin();

    str::pointer r = layout.begin();
    while (*r != '\0') {

        str::const_pointer match_ptr = nullptr;
        str::size_type match_n = 0;
        int cntnu = func(r, layout.end(), match_ptr, match_n);

        //  没找到匹配项
        if (match_ptr == nullptr) {
            break;
        }

        ASSERT(match_n > 0);

        str::size_type mov_len = match_ptr - r;
        if (mov_len > 0) {
            std::memmove(w, r, mov_len);
            w += mov_len;
        }

        r = layout.begin() + (match_ptr - layout.begin()) + match_n;

        //  确定是否需要继续
        if (0 != cntnu) {
            break;
        }
    }

    while (*r != '\0') {
        *(w++) = *(r++);
    }

    layout.resize(w - layout.begin());

    return *this;
}

str& str::erase(pos_type pos, pos_type n) {
    return remove(pos, n);
}

str::iterator str::erase(const_iterator p) {
    return erase(p, (p + 1));
}

str::iterator str::erase(const_iterator first, const_iterator last) {
    ASSERT(first >= layout.begin());
    ASSERT(first < layout.end());
    ASSERT(last > layout.begin());
    ASSERT(last <= layout.end());
    layout.clipmove(last - layout.begin(), layout.end() - last, -(last - first));
    return layout.begin() + (first - layout.begin());
}

int str::compare(const str& s) const {
    return strcmp(begin(), s.begin());
}

int str::compare(str::const_pointer s) const {
    ASSERT(s != nullptr);
    return strcmp(begin(), s);
}

int str::compare(str::const_pointer s, str::size_type max_n) const {
    ASSERT(s != nullptr);
    return strncmp(begin(), s, max_n);
}

int str::compare(const str& other, str::size_type max_n) const {
    return strncmp(begin(), other.begin(), max_n);
}

int str::compare(str::value_type c) const {
    return (size() == 1) && (begin()[0] == c);
}

bool str::contains(const str& s) const {
    return strstr(begin(), s.begin()) != nullptr;
}

bool str::contains(str::const_pointer s) const {
    ASSERT(s != nullptr);
    return strstr(begin(), s) != nullptr;
}

bool str::contains(str::value_type ch) const {
    return strchr(begin(), ch) != nullptr;
}

bool str::contains(const re& r) const {
    bool found = false;
    r.find(*this, 0, [&found](const re::segment_type* segment, re::size_type n) -> int {
        found = true;
        return -1;
    });
    return found;
}

int str::count(const str& other) const {
    int cnt = 0;

    size_type slen = other.size();

    const_pointer start = layout.begin();
    while ((start = std::strstr(start, other.data())) != nullptr) {
        cnt++;
        start += slen;
    }

    return cnt;
}

int str::count(str::const_pointer s) const {
    ASSERT(s != nullptr);

    int cnt = 0;

    size_type slen = std::strlen(s);

    const_pointer start = layout.begin();
    while ((start = std::strstr(start, s)) != nullptr) {
        cnt++;
        start += slen;
    }

    return cnt;
}

int str::count(str::value_type ch) const {
    int cnt = 0;

    const_pointer start = layout.begin();
    while ((start = std::strchr(start, ch)) != nullptr) {
        cnt++;
        start++;
    }

    return cnt;
}

int str::count(const re& rx) const {
    ASSERT(rx);

    int cnt = 0;
    rx.find(layout.begin(), 0, [&cnt](const re::segment_type* segs, re::size_type n) -> int {
        cnt++;
        return 0;
    });
    return cnt;
}

bool str::has_suffix(const str& s) const {
    ASSERT(false); //  TODO - bool str::has_suffix(const str& s) const
    return false;
}

bool str::has_suffix(str::const_pointer str) const {
    ASSERT(false); //  TODO - bool str::has_suffix(str::const_pointer str) const
    return false;
}

bool str::has_suffix(str::value_type c) const {
    ASSERT(false); //  TODO - bool str::has_suffix(str::value_type c) const
    return false;
}

bool str::has_prefix(const str& s) const {
    ASSERT(false); //  TODO - bool str::has_prefix(const str& s) const
    return false;
}

bool str::has_prefix(str::const_pointer s) const {
    ASSERT(false); //  TODO - bool str::has_prefix(str::const_pointer s) const
    return false;
}

bool str::has_prefix(str::value_type c) const {
    ASSERT(false); //  TODO - bool str::has_prefix(str::value_type c) const
    return false;
}

bool str::remove_prefix(const str& s) {
    ASSERT(false); //  TODO - bool str::remove_prefix(const str& s)
    return false;
}

bool str::remove_prefix(str::const_pointer s) {
    ASSERT(false); //  TODO - bool str::remove_prefix(str::const_pointer s)
    return false;
}

bool str::remove_prefix(str::value_type c) {
    ASSERT(false); //  TODO - bool str::remove_prefix(str::value_type c)
    return false;
}

bool str::remove_suffix(const str& s) {
    ASSERT(false); //  TODO - bool str::remove_suffix(const str& s)
    return false;
}

bool str::remove_suffix(str::const_pointer s) {
    ASSERT(false); //  TODO - bool str::remove_suffix(str::const_pointer s)
    return false;
}

bool str::remove_suffix(str::value_type c) {
    ASSERT(false); //  TODO - bool str::remove_suffix(str::value_type c)
    return false;
}

str& str::fill(str::value_type ch) {
    layout.fill(0, ch, layout.len());
    return *this;
}

str& str::fill(str::pos_type pos, str::value_type ch, str::size_type n) {
    ASSERT(n > 0);
    ASSERT(pos >= 0);
    ASSERT(pos < size());
    ASSERT((pos + n) >= 0);
    ASSERT((pos + n) <= size());

    layout.fill(pos, ch, layout.len());
    return *this;
}

str& str::fill(str::pos_type pos, str::const_pointer s, str::size_type n) {
    ASSERT(n > 0);
    ASSERT(s != nullptr);
    ASSERT(pos >= 0);
    ASSERT(pos < size());
    ASSERT((pos + n) >= 0);
    ASSERT((pos + n) <= size());

    layout.fill(pos, s, n);
    return *this;
}

str::pos_type str::index_of(const str& other, str::pos_type from) const {
    return index_of(other.data(), from);
}

str::pos_type str::index_of(str::const_pointer s, str::pos_type from) const {
    ASSERT(s != nullptr);
    ASSERT(from >= 0);
    ASSERT(from < size());

    const_pointer d = data();
    const_pointer pos = std::strstr(d + from, s);
    if (pos == nullptr) {
        return str::npos;
    }

    return str::pos_type(pos - d);
}

str::pos_type str::index_of(str::value_type ch, str::pos_type from) const {
    ASSERT(from >= 0);
    ASSERT(from < size());

    const_pointer d = data();
    const_pointer pos = std::strchr(d + from, ch);
    if (pos == nullptr) {
        return str::npos;
    }

    return str::pos_type(pos - d);
}

str::pos_type str::index_of(const re& rx, str::pos_type from) const {
    ASSERT(from >= 0);
    ASSERT(from < layout.len());

    str::pos_type pos = -1;
    rx.find(layout.begin() + from, 0, [&pos](const re::segment_type* segs, re::size_type n) -> int {
        pos = segs[0].pos;
        return str::npos;
    });

    return from + pos;
}

str::pos_type str::index_of(std::function<int(str::value_type c, bool& match)> func, str::pos_type from) const {

    str::const_pointer ptr = layout.begin();
    while (*ptr) {
        bool match = false;
        int cntnu = func(*ptr, match);
        if (match) {
            return ptr - layout.begin();
        }

        if (cntnu != 0) {
            break;
        }
    }

    return str::npos;
}

str::pos_type str::last_index_of(const str& other, str::pos_type from) const {
    ASSERT(false); //  TODO - str::pos_type str::last_index_of(const str& other, str::pos_type from) const
    return false;
}

str::pos_type str::last_index_of(str::value_type ch, str::pos_type from) const {
    ASSERT(from >= 0);
    ASSERT(from < size());

    const_pointer d = data();
    const_pointer pos = std::strrchr(d + from, ch);
    if (pos == nullptr) {
        return str::npos;
    }

    return str::pos_type(pos - d);
}

str::pos_type str::last_index_of(str::const_pointer s, str::pos_type from) const {
    ASSERT(false); //  TODO - str::pos_type str::last_index_of(str::const_pointer s, str::pos_type from) const
    return false;
}

str::pos_type str::last_index_of(const re& rx, str::pos_type from) const {
    ASSERT(false); //  TODO - str::pos_type str::last_index_of(const re& rx, str::pos_type from) const
    return false;
}

str::pos_type str::last_index_of(std::function<int(str::value_type c, bool& match)> func, str::pos_type from) const {
    ASSERT(false); //  TODO - str::pos_type str::last_index_of(std::function<int(str::value_type c, bool& match)> func, str::pos_type from) const
    return false;
}

bool str::is_match(const re& rx) const {
    return rx.match(*this, 0);
}

bool str::is_match(const str& pattern) const {
    return re(pattern).match(*this, 0);
}

bool str::is_match(str::const_pointer pattern) const {
    ASSERT(pattern != nullptr);
    return re(pattern).match(*this, 0);
}

bool str::is_match_wild(const str& pattern) const {
    ASSERT(false); //  TODO - bool str::is_match_wild(str::str& pattern) const
    return false;
}

bool str::is_match_wild(str::const_pointer pattern) const {
    ASSERT(false); //  TODO - bool str::is_match_wild(str::const_pointer pattern) const
    return false;
}

bool str::is_empty() const {
    return layout.len() == 0;
}

bool str::is_lower() const {
    for (const_pointer ptr = layout.begin(); ptr != layout.end(); ptr++) {
        if (0 != (chars_mapping[*ptr] & UPPER)) {
            return false;
        }
    }

    return true;
}

bool str::is_upper() const {
    for (const_pointer ptr = layout.begin(); ptr != layout.end(); ptr++) {
        if (0 != (chars_mapping[*ptr] & LOWER)) {
            return false;
        }
    }

    return true;
}

bool str::is_title() const {
    ASSERT(false); //  TODO - bool str::is_title() const
    return false;
}

bool str::is_digit() const {
    if (layout.len() == 0) {
        return false;
    }

    for (const_pointer ptr = layout.begin(); ptr != layout.end(); ptr++) {
        if (0 == (chars_mapping[*ptr] & DIGIT)) {
            return false;
        }
    }

    return true;
}

bool str::is_ascii() const {
    if (layout.len() == 0) {
        return false;
    }

    for (const_pointer ptr = layout.begin(); ptr != layout.end(); ptr++) {
        if ((uint8_t(*ptr) & 0x80) != 0) {
            return false;
        }
    }

    return true;
}

bool str::is_alpha() const {
    if (layout.len() == 0) {
        return false;
    }

    for (const_pointer ptr = layout.begin(); ptr != layout.end(); ptr++) {
        if (0 == (chars_mapping[*ptr] & ALPHA)) {
            return false;
        }
    }

    return true;
}

bool str::is_alnum() const {
    if (layout.len() == 0) {
        return false;
    }

    for (const_pointer ptr = layout.begin(); ptr != layout.end(); ptr++) {
        if (0 == (chars_mapping[*ptr] & ALNUM)) {
            return false;
        }
    }

    return true;
}

bool str::is_space() const {
    if (layout.len() == 0) {
        return false;
    }

    for (const_pointer ptr = layout.begin(); ptr != layout.end(); ptr++) {
        if (0 == (chars_mapping[*ptr] & SPACE)) {
            return false;
        }
    }

    return true;
}

bool str::is_printable() const {
    if (layout.len() == 0) {
        return false;
    }

    for (const_pointer ptr = layout.begin(); ptr != layout.end(); ptr++) {
        if (0 == (chars_mapping[*ptr] & PRINT)) {
            return false;
        }
    }

    return true;
}

bool str::is_identifier() const {

    if (layout.len() == 0) {
        return false;
    }

    const_pointer ptr = layout.begin();
    if (!((*ptr == '_') || (*ptr >= 'A' && *ptr <= 'Z') || (*ptr >= 'a' && *ptr <= 'z'))) {
        return false;
    }

    for (ptr++; ptr != layout.end(); ptr++) {
        if (!((*ptr == '_') || (*ptr >= 'A' && *ptr <= 'Z') || (*ptr >= 'a' && *ptr <= 'z') || (*ptr >= '0' && *ptr <= '9'))) {
            return false;
        }
    }

    return true;
}

bool str::is_numeric() const {
    ASSERT(false); //  TODO - bool str::is_numeric() const
    return false;
}

bool str::is_bool() const {
    ASSERT(false); //  TODO - bool str::is_bool() const
    return false;
}

str str::left(str::size_type n) const {
    return str(layout.begin(), n);
}

str str::right(str::size_type n) const {
    return str(layout.end() - n, n);
}

str str::substr(pos_type pos, int offset) const {
    if (offset < 0) {
        return str(layout.begin() + pos + offset, -offset);
    }

    if (offset > 0) {
        return str(layout.begin() + pos, offset);
    }

    return str();
}

//  定宽对齐调整
str& str::ljust(str::size_type width, str::value_type fill, bool truncate) {
    ASSERT(false); //  TODO - str& str::ljust(str::size_type width, str::value_type fill, bool truncate)
    return *this;
}

str& str::rjust(str::size_type width, str::value_type fill, bool truncate) {
    ASSERT(false); //  TODO - str& str::rjust(str::size_type width, str::value_type fill, bool truncate)
    return *this;
}

str& str::center(str::size_type width, str::value_type fill, bool truncate) {
    ASSERT(false); //  TODO - str& str::center(str::size_type width, str::value_type fill, bool truncate)
    return *this;
}

str& str::zfill(str::size_type width, str::value_type fill, bool truncate) {
    ASSERT(false); //  TODO - str& str::zfill(str::size_type width, str::value_type fill, bool truncate)
    return *this;
}

str& str::replace(str::pos_type pos, str::size_type n, const str& after) {
    ASSERT(false); //  TODO - str& str::replace(str::pos_type pos, str::size_type n, const str& after)
    return *this;
}

str& str::replace(str::pos_type pos, str::size_type n, str::value_type after) {
    ASSERT(false); //  TODO - str& str::replace(str::pos_type pos, str::size_type n, str::value_type after)
    return *this;
}

str& str::replace(str::pos_type pos, str::size_type n, str::const_pointer unicode, str::size_type size) {
    ASSERT(false); //  TODO - str& str::replace(str::pos_type pos, str::size_type n, str::const_pointer unicode, str::size_type size)
    return *this;
}

str& str::replace(str::value_type before, str::value_type after) {
    ASSERT(false); //  TODO - str& str::replace(str::value_type before, str::value_type after)
    return *this;
}

str& str::replace(str::const_pointer before, str::size_type blen, str::const_pointer after, str::size_type alen) {
    ASSERT(false); //  TODO - str& str::replace(str::const_pointer before, str::size_type blen, str::const_pointer after, str::size_type alen)
    return *this;
}

str& str::replace(const str& before, const str& after) {
    ASSERT(false); //  TODO - str& str::replace(const str& before, const str& after)
    return *this;
}

str& str::replace(str::value_type ch, const str& after) {
    ASSERT(false); //  TODO - str& str::replace(str::value_type ch, const str& after)
    return *this;
}

str& str::replace(const re& rx, const str& after) {
    ASSERT(false); //  TODO - str& str::replace(const re& rx, const str& after)
    return *this;
}

str& str::replace(std::function<int(str::value_type key, str::value_type& val)> func) {
    ASSERT(false); //  TODO - str& str::replace(std::function<int(str::value_type key, str::value_type& val)> func)
    return *this;
}

str str::repeat(str::size_type times) const {
    ASSERT(times >= 0);

    str result;

    result.resize(size() * times);

    str::pos_type pos = 0;
    for (str::size_type i = 0; i < times; i++) {
        result.fill(pos + i * size(), data(), size());
    }

    return result;
}

str str::join(const std::vector<str>& s) const {
    auto itr = s.begin();
    return join([&s, &itr]() -> str::const_pointer {
        str::const_pointer s = itr->data();
        itr++;
        return s;
    });
}

str str::join(str::const_pointer s, ...) const {
    va_list valist;
    va_start(valist, s);

    str result = join([&valist]() -> str::const_pointer {
        return va_arg(valist, str::const_pointer);
    });

    va_end(valist);

    return result;
}

str str::join(std::function<str::const_pointer()> provider) const {
    str result;

    //  第一次特殊处理
    const_pointer ptr = provider();
    if (ptr == nullptr) {
        return result;
    }

    //  追加第一个数据
    result.append(ptr);

    //  获取下一个
    ptr = provider();

    //  第二次及之后都要追加 *this
    while (ptr != nullptr) {
        result.append(*this);
        result.append(ptr);
        ptr = provider();
    }

    return result;
}

str str::join(std::initializer_list<str::const_pointer> ptr_list) const {
    auto itr = ptr_list.begin();
    return join([&ptr_list, &itr]() -> str::const_pointer {
        if (itr == ptr_list.end()) {
            return nullptr;
        }

        return *(itr++);
    });
}

str str::join(std::initializer_list<str> ptr_list) const {
    auto itr = ptr_list.begin();
    return join([&ptr_list, &itr]() -> str::const_pointer {
        if (itr == ptr_list.end()) {
            return nullptr;
        }

        return (*(itr++)).data();
    });
}

void str::reserve(str::size_type cap) {
    layout.reserve(cap);
}

void str::resize(str::size_type n) {
    layout.resize(n);
}

void str::resize(str::size_type n, str::value_type fill_ch) {
    ASSERT(n >= 0);

    str::size_type old_len = size();
    layout.resize(n);
    if (n > old_len) {
        layout.fill(old_len, fill_ch, (n - old_len));
    }
}

void str::shrink_to_fit() {
}

void str::squeeze() {
}

str& str::title() {
    return *this;
}

str& str::inversion() {
    return *this;
}

std::vector<str> str::split(const str& sep) const {
    std::vector<str> result;
    split(sep, [&result](const_pointer s, size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<str> str::split(str::const_pointer sep) const {
    std::vector<str> result;
    split(sep, [&result](str::const_pointer s, str::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<str> str::split(str::value_type sep) const {
    std::vector<str> result;
    split(sep, [&result](str::const_pointer s, str::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<str> str::split(const re& r) const {
    std::vector<str> result;
    split(r, [&result](str::const_pointer s, str::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<str> str::split(std::function<int(str::value_type c, bool& match)>& chars_func) const {
    std::vector<str> result;
    split(chars_func, [&result](str::const_pointer s, str::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<str> str::split_lines(bool keep_ends) const {
    std::vector<str> result;
    split_lines(keep_ends, [&result](str::const_pointer s, str::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<str> str::split_path() const {
    std::vector<str> result;
    split_path([&result](str::const_pointer s, str::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

void str::split(const str& sep, std::function<int(str::const_pointer s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void str::split(const str& sep, std::function<int(str::const_pointer s, str::size_type n)> output_func) const
}

void str::split(str::const_pointer sep, std::function<int(str::const_pointer s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void str::split(str::const_pointer sep, std::function<int(str::const_pointer s, str::size_type n)> output_func) const
}

void str::split(str::value_type sep, std::function<int(str::const_pointer s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void str::split(str::value_type sep, std::function<int(str::const_pointer s, str::size_type n)> output_func) const
}

void str::split(const re& rx, std::function<int(str::const_pointer s, str::size_type n)> output_func) const {
    rx.split(*this, 0, [&output_func](const re::segment_type& segs) -> int {
        output_func(segs.start, segs.len);
        return 0;
    });
}

void str::split(std::function<int(str::value_type c, bool& match)>& chars_func, std::function<int(str::const_pointer s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void str::split(std::function<int(str::value_type c, bool& match)>& chars_func, std::function<int(str::const_pointer s, str::size_type n)> output_func) const
}

void str::split_lines(bool keep_ends, std::function<int(str::const_pointer s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void str::split_lines(bool keep_ends, std::function<int(str::const_pointer s, str::size_type n)> output_func) const
}

void str::split_path(std::function<int(str::const_pointer s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void str::split_path(std::function<int(str::const_pointer s, str::size_type n)> output_func) const
}

str& str::to_lower() {
    std::transform(begin(), end(), begin(), ::tolower);
    return *this;
}

str& str::to_upper() {
    std::transform(begin(), end(), begin(), ::toupper);
    return *this;
}

str& str::swap_case() {
    std::transform(begin(), end(), begin(), [](str::value_type& ch) -> str::value_type {
        if (std::islower(ch)) {
            return std::toupper(ch);
        }

        if (std::isupper(ch)) {
            return std::tolower(ch);
        }

        return ch;
    });

    return *this;
}

str& str::simplified() {
    ASSERT(false); //  TODO - str& str::simplified()
    return *this;
}

str& str::ltrim() {
    return ltrim_until([](str::value_type ch) -> bool {
        return !std::isspace(ch);
    });
}

str& str::rtrim() {
    return rtrim_until([](str::value_type ch) -> bool {
        return !std::isspace(ch);
    });
}

str& str::trim() {
    return this->rtrim().ltrim();
}

str& str::ltrim(std::function<bool(str::value_type ch)> func) {
    str::pointer p = nullptr;
    for (p = layout.begin(); p != layout.end(); p++) {
        if (!func(*p)) {
            break;
        }
    }

    size_type remain_len = layout.end() - p;
    if (remain_len > 0) {
        layout.clipmove(p - layout.begin(), remain_len, -(p - layout.begin()));
    }

    layout.resize(remain_len);
    return *this;
}

str& str::rtrim(std::function<bool(str::value_type ch)> func) {
    str::pointer p = nullptr;
    for (p = layout.end() - 1; p != (layout.begin() - 1); p--) {
        if (!func(*p)) {
            break;
        }
    }
    layout.resize((p + 1) - layout.begin());
    return *this;
}

str& str::trim(std::function<bool(str::value_type ch)> func) {
    return rtrim(func).ltrim(func);
}

str& str::ltrim_until(std::function<bool(str::value_type ch)> func) {
    return ltrim([&func](str::value_type ch) -> bool {
        return !func(ch);
    });
}

str& str::rtrim_until(std::function<bool(str::value_type ch)> func) {
    return rtrim([func](str::value_type ch) -> bool {
        return !func(ch);
    });
}

str& str::trim_until(std::function<bool(str::value_type ch)> func) {
    return rtrim_until(func).ltrim_until(func);
}

str str::expand(const std::map<str, str>& kvs) const {
    ASSERT(false); // TODO - str str::expand(const std::map<str, str>& kvs) const
    return str("");
}

str str::expand(std::function<int(const str& key, str& val)> provider) const {
    ASSERT(false); // TODO - str str::expand(std::function<int(const str& key, str& val)> provider) const
    return str("");
}

str str::expand_envs(str::const_pointer key, str::const_pointer val) const {
    ASSERT(false); // TODO - str str::expand_envs(str::const_pointer key, str::const_pointer val) const
    return str("");
}

str str::expand_envs() const {
    ASSERT(false); // TODO - str str::expand_envs() const
    return str("");
}

str str::expand_tabs(str::size_type tab_size) const {
    ASSERT(false); // TODO - str str::expand_tabs(str::size_type tab_size) const
    return str("");
}

str str::expand_tmpl(const std::map<str, str>& kvs) const {
    ASSERT(false); // TODO - str str::expand_tmpl(const std::map<str, str>& kvs) const
    return str("");
}

str str::expand_tmpl(std::function<int(const str& key, str& val)> provider) const {
    ASSERT(false); // TODO - str str::expand_tmpl(std::function<int(const str& key, str& val)> provider) const
    return str("");
}

void str::swap(str& s) {
    if (&s == this) {
        return;
    }

    layout_tmplt t = s.layout.tmplt;
    s.layout.tmplt = layout.tmplt;
    layout.tmplt = t;
}

str::size_type str::copy(str::pointer dest, str::size_type n, str::pos_type pos) const {
    ASSERT(dest != nullptr);
    ASSERT(n > 0);
    ASSERT(pos >= 0);
    ASSERT(pos < size());

    str::size_type copy_n = std::min(n, (size() - pos));
    std::memcpy(dest, begin() + pos, copy_n);

    return copy_n;
}

str str::basename() const {
    ASSERT(false); // TODO - str str::basename() const
    return str("");
}

str& str::basename() {
    ASSERT(false); // TODO - str& str::basename()
    return *this;
}

str str::dirname() const {
    ASSERT(false); // TODO - str str::dirname() const
    return str("");
}

str& str::dirname() {
    ASSERT(false); // TODO - str& str::dirname()
    return *this;
}

bool str::to_bool(bool* ok) const {
    ASSERT(false); // TODO - bool str::to_bool(bool* ok) const
}

str& str::assign(bool v) {
    if (v) {
        layout.resize(4);
        layout.fill(0, "true", 4);
    } else {
        layout.resize(5);
        layout.fill(0, "false", 5);
    }
    return *this;
}

double str::to_double(bool* ok) const {
    ASSERT(false); // TODO - double str::to_double(bool* ok) const
    return 0.0;
}

float str::to_float(bool* ok) const {
    ASSERT(false); // TODO - float str::to_float(bool* ok) const
    return 0.0;
}

int8_t str::to_int8(bool* ok, int base) const {
    ASSERT(false); // TODO - int8_t str::to_int8(bool* ok, int base) const
    return 0;
}

int16_t str::to_int16(bool* ok, int base) const {
    ASSERT(false); //  TODO - int16_t str::to_int16(bool* ok, int base) const
    return 0;
}

int32_t str::to_int32(bool* ok, int base) const {
    ASSERT(false); //  TODO - int32_t str::to_int32(bool* ok, int base) const
    return 0;
}

int64_t str::to_int64(bool* ok, int base) const {
    ASSERT(false); //  TODO - int64_t str::to_int64(bool* ok, int base) const
    return 0;
}

uint8_t str::to_uint8(bool* ok, int base) const {
    ASSERT(false); //  TODO - uint8_t str::to_uint8(bool* ok, int base) const
    return 0;
}

uint16_t str::to_uint16(bool* ok, int base) const {
    ASSERT(false); //  TODO - uint16_t str::to_uint16(bool* ok, int base) const
    return 0;
}

uint32_t str::to_uint32(bool* ok, int base) const {
    ASSERT(false); //  TODO - uint32_t str::to_uint32(bool* ok, int base) const
    return 0;
}

uint64_t str::to_uint64(bool* ok, int base) const {
    ASSERT(false); //  TODO - uint64_t str::to_uint64(bool* ok, int base) const
    return 0;
}

str& str::assign(double n, str::value_type format, int precision) {
    ASSERT(false); //  TODO - str& str::assign(double n, str::value_type format, int precision)
    return *this;
}

str& str::assign(float n, str::value_type format, int precision) {
    ASSERT(false); //  TODO - str& str::assign(float n, str::value_type format, int precision)
    return *this;
}

str& str::assign(int8_t n, int base) {
    ASSERT(false); //  TODO - str& str::assign(int8_t n, int base)
    return *this;
}

str& str::assign(int16_t n, int base) {
    ASSERT(false); //  TODO - str& str::assign(int16_t n, int base)
    return *this;
}

str& str::assign(int32_t n, int base) {
    ASSERT(false); //  TODO - str& str::assign(int32_t n, int base)
    return *this;
}

str& str::assign(int64_t n, int base) {
    ASSERT(false); //  TODO - str& str::assign(int64_t n, int base)
    return *this;
}

str& str::assign(uint8_t n, int base) {
    ASSERT(false); //  TODO - str& str::assign(uint8_t n, int base)
    return *this;
}

str& str::assign(uint16_t n, int base) {
    ASSERT(false); //  TODO - str& str::assign(uint16_t n, int base)
    return *this;
}

str& str::assign(uint32_t n, int base) {
    ASSERT(false); //  TODO - str& str::assign(uint32_t n, int base)
    return *this;
}

str& str::assign(uint64_t n, int base) {
    ASSERT(false); //  TODO - str& str::assign(uint64_t n, int base)
    return *this;
}

int32_t str::hash_code() const {
    ASSERT(false); //  TODO - int32_t str::hash_code() const
    return 0;
}

str str::number(double n, str::value_type format, int precision) {
    ASSERT(false); //  TODO - str str::number(double n, str::value_type format, int precision)
    return str("");
}

str str::number(float n, str::value_type format, int precision) {
    ASSERT(false); //  TODO - str str::number(float n, str::value_type format, int precision)
    return str("");
}

str str::number(int8_t n, int base) {
    return str("");
}

str str::number(int16_t n, int base) {
    ASSERT(false); //  TODO - str str::number(int16_t n, int base)
    return str("");
}

str str::number(int32_t n, int base) {
    ASSERT(false); //  TODO - str str::number(int32_t n, int base)
    return str("");
}

str str::number(int64_t n, int base) {
    ASSERT(false); //  TODO - str str::number(int64_t n, int base)
    return str("");
}

str str::number(uint8_t n, int base) {
    ASSERT(false); //  TODO - str str::number(uint8_t n, int base)
    return str("");
}

str str::number(uint16_t n, int base) {
    ASSERT(false); //  TODO - str str::number(uint16_t n, int base)
    return str("");
}

str str::number(uint32_t n, int base) {
    ASSERT(false); //  TODO - str str::number(uint32_t n, int base)
    return str("");
}

str str::number(uint64_t n, int base) {
    ASSERT(false); //  TODO - str str::number(uint64_t n, int base)
    return str("");
}

bool str::operator!=(str::const_pointer s) const {
    return 0 != compare(s);
}

str& str::operator+=(str::value_type ch) {
    return append(ch);
}

str& str::operator+=(const str& s) {
    return append(s);
}

str& str::operator+=(str::const_pointer s) {
    ASSERT(s != nullptr);
    return append(s);
}

bool str::operator<(str::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) < 0;
}

bool str::operator<=(str::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) <= 0;
}

str& str::operator=(str::value_type ch) {
    layout.destroy();
    layout.init(&ch, 1);
    return *this;
}

str& str::operator=(str::const_pointer s) {
    ASSERT(s != nullptr);
    layout.destroy();
    layout.init(s, std::strlen(s));
    return *this;
}

bool str::operator==(str::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) == 0;
}

bool str::operator>(str::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) > 0;
}

bool str::operator>=(str::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) >= 0;
}

str::value_type& str::operator[](str::pos_type pos) {
    return layout.begin()[pos];
}

const str::value_type& str::operator[](str::pos_type pos) const {
    return layout.begin()[pos];
}

extern bool operator!=(const str& s1, const str& s2) {
    return s2.compare(s1) != 0;
}

extern bool operator!=(str::const_pointer s1, const str& s2) {
    return s2.compare(s1) != 0;
}

extern const str operator+(const str& s1, const str& s2) {
    ASSERT(false); //  TODO - extern const str operator+(const str& s1, const str& s2)
    str new_str;
    return new_str;
}

extern const str operator+(const str& s1, str::const_pointer s2) {
    ASSERT(false); //  TODO - extern const str operator+(const str& s1, str::const_pointer s2)
    str new_str;
    return new_str;
}

extern const str operator+(str::const_pointer s1, const str& s2) {
    ASSERT(false); //  TODO - extern const str operator+(str::const_pointer s1, const str& s2)
    str new_str;
    return new_str;
}

extern const str operator+(str::value_type ch, const str& s) {
    ASSERT(false); //  TODO - extern const str operator+(str::value_type ch, const str& s)
    str new_str;
    return new_str;
}

extern const str operator+(const str& other, str::value_type ch) {
    ASSERT(false); //  TODO - extern const str operator+(const str& other, str::value_type ch)
    str new_str;
    return new_str;
}

extern bool operator<(const str& s1, const str& s2) {
    return s2.compare(s1) > 0;
}

extern bool operator<(str::const_pointer s1, const str& s2) {
    return s2.compare(s1) > 0;
}

extern bool operator<=(const str& s1, const str& s2) {
    return s2.compare(s1) >= 0;
}

extern bool operator<=(str::const_pointer s1, const str& s2) {
    return s2.compare(s1) >= 0;
}

extern bool operator==(const str& s1, const str& s2) {
    return s2.compare(s1) == 0;
}

extern bool operator==(str::const_pointer s1, const str& s2) {
    return s2.compare(s1) == 0;
}

extern bool operator>(const str& s1, const str& s2) {
    return s2.compare(s1) < 0;
}

extern bool operator>(str::const_pointer s1, const str& s2) {
    return s2.compare(s1) < 0;
}

extern bool operator>=(const str& s1, const str& s2) {
    return s2.compare(s1) <= 0;
}

extern bool operator>=(str::const_pointer s1, const str& s2) {
    return s2.compare(s1) <= 0;
}

} // namespace tiny