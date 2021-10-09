//
// Created by luolijun on 2021/9/30.
//
#include "tiny/bytes.h"
#include "tiny/asserts.h"
#include "tiny/chars.h"
#include "tiny/re.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>

namespace tiny {

//  进制映射范围
enum number_base {
    base_min = 2,
    base_max = 36,
};

//  数据格式化时的映射
static bytes::const_pointer number_map_upper = "0123456789ABCDEFGHIJKLMNOPQRETUVWXYZ";
static bytes::const_pointer number_map_lower = "0123456789abcdefghijklmnopqretuvwxyz";

//  反向查找字符串
static const char* strrstr(const char* s1, const char* s2) {

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

static const char* strrstr(const char* begin, const char* end, const char* ptn, bytes::size_type n) {
    return nullptr;
}

int wildcmp(const char* wild, const char* s) {
    const char* cp = nullptr;
    const char* mp = nullptr;
    while ((*s) && (*wild != '*')) {
        if ((*wild != *s) && (*wild != '?')) {
            return 1;
        }
        wild++;
        s++;
    }

    while (*s) {
        if (*wild == '*') {
            if (!*(++wild)) {
                return 0;
            }
            mp = wild;
            cp = s + 1;
        } else if ((*wild == *s) || (*wild == '?')) {
            wild++;
            s++;
        } else {
            wild = mp;
            s = cp++;
        }
    }

    while (*wild == '*') {
        wild++;
    }

    return *wild;
}

bytes::bytes() {
    layout.init(nullptr, 0);
}

bytes::bytes(bytes::const_pointer s, bytes::size_type n) {
    layout.init(s, n);
}

bytes::bytes(bytes::value_type ch, bytes::size_type n) {
    layout.init(nullptr, n);
    std::fill(layout.begin(), layout.end(), ch);
}

bytes::bytes(bytes::const_pointer s) {
    ASSERT(s != nullptr);
    layout.init(s, std::strlen(s));
}

bytes::bytes(bytes&& other) noexcept {
    memcpy(&layout, &other.layout, sizeof(other.layout));
    other.layout.init(nullptr, 0);
}

bytes::bytes(const bytes& other) {
    layout.init(other.data(), other.size());
}

bytes& bytes::operator=(bytes&& other) noexcept {
    if (&other == this) {
        return *this;
    }

    layout.destroy();
    memcpy(&layout, &other.layout, sizeof(other.layout));
    other.layout.init(nullptr, 0);

    return *this;
}

bytes& bytes::operator=(const bytes& other) {
    if (&other == this) {
        return *this;
    }

    layout.destroy();
    layout.init(other.layout.begin(), other.layout.len());

    return *this;
}

bytes::~bytes() {
    layout.destroy();
}

//  迭代器
bytes::iterator bytes::begin() {
    return layout.begin();
}

bytes::iterator bytes::end() {
    return layout.end();
}

//  常量迭代器
bytes::const_iterator bytes::begin() const {
    return layout.begin();
}

bytes::const_iterator bytes::end() const {
    return layout.end();
}

bytes::reverse_iterator bytes::rbegin() {
    return bytes::reverse_iterator(layout.end() - 1);
}

bytes::reverse_iterator bytes::rend() {
    return bytes::reverse_iterator(layout.begin() - 1);
}

bytes::const_reverse_iterator bytes::rbegin() const {
    return bytes::const_reverse_iterator(layout.end() - 1);
}

bytes::const_reverse_iterator bytes::rend() const {
    return bytes::const_reverse_iterator(layout.begin() - 1);
}

bytes::value_type bytes::at(bytes::pos_type pos) const {
    ASSERT(pos >= 0);
    ASSERT(pos < size());
    return layout.begin()[pos];
}

bytes::value_type bytes::back() const {
    ASSERT(size() > 0);
    return *(layout.end() - 1);
}

bytes::value_type& bytes::back() {
    ASSERT(size() > 0);
    return *(layout.end() - 1);
}

bytes::value_type bytes::front() const {
    ASSERT(size() > 0);
    return layout.begin()[0];
}

bytes::value_type& bytes::front() {
    ASSERT(size() > 0);
    return layout.begin()[0];
}

bytes::size_type bytes::size() const {
    return layout.len();
}

bytes::size_type bytes::capacity() const {
    return layout.cap();
}

void bytes::clear() {
    layout.destroy();
    layout.init(nullptr, 0);
}

bytes::const_pointer bytes::data() const {
    return layout.begin();
}

bytes::pointer bytes::data() {
    return layout.begin();
}

void bytes::attach(bytes::pointer buf, bytes::size_type len, bytes::size_type cap) {
    ASSERT(buf != nullptr);
    ASSERT(len >= 0);
    ASSERT(cap >= 0);
    ASSERT(cap >= len);
    layout.destroy();
    layout.large.attach(cap, len, buf);
}

bytes& bytes::append(const bytes& s) {
    bytes::pos_type pos = layout.len();
    layout.resize(layout.len() + s.size());
    layout.fill(pos, s.data(), s.size());
    return *this;
}

bytes& bytes::append(bytes::value_type ch) {
    bytes::pos_type pos = layout.len();
    layout.resize(layout.len() + 1);
    layout.fill(pos, &ch, 1);
    return *this;
}

bytes& bytes::append(bytes::const_pointer s, bytes::size_type n) {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    bytes::pos_type pos = layout.len();
    layout.resize(layout.len() + n);
    layout.fill(pos, s, n);
    return *this;
}

bytes& bytes::append(bytes::const_pointer s) {
    ASSERT(s != nullptr);
    append(s, std::strlen(s));
    return *this;
}

bytes& bytes::prepend(const bytes& s) {
    ASSERT(this != &s);
    return prepend(s.begin(), s.size());
}

bytes& bytes::prepend(bytes::value_type ch) {
    return prepend(&ch, 1);
}

bytes& bytes::prepend(bytes::const_pointer s, bytes::size_type n) {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    layout.flexmove(0, layout.len(), n, [](pointer fill, size_type n) {
    });
    layout.fill(0, s, n);
    return *this;
}

bytes& bytes::prepend(bytes::const_pointer s) {
    ASSERT(s != nullptr);
    return prepend(s, std::strlen(s));
}

bytes& bytes::insert(bytes::pos_type pos, const bytes& str) {
    ASSERT(pos >= 0);
    return insert(pos, str.layout.begin(), layout.len());
}

bytes& bytes::insert(bytes::pos_type pos, bytes::value_type ch) {
    return insert(pos, &ch, 1);
}

bytes& bytes::insert(bytes::pos_type pos, bytes::const_pointer s, bytes::size_type n) {
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

bytes& bytes::insert(bytes::pos_type pos, bytes::const_pointer s) {
    return insert(pos, s, std::strlen(s));
}

void bytes::push_back(const bytes& s) {
    append(s.data(), s.size());
}

void bytes::push_back(bytes::value_type ch) {
    append(ch);
}

void bytes::push_front(const bytes& s) {
    prepend(s);
}

void bytes::push_front(bytes::value_type ch) {
    prepend(ch);
}

bytes::value_type bytes::pop_back() {
    ASSERT(layout.len() > 0);
    bytes::value_type ch = *(layout.end() - 1);
    layout.resize(layout.len() - 1);
    return ch;
}

bytes::value_type bytes::pop_front() {
    ASSERT(size() > 0);
    bytes::value_type ch = *(layout.begin());
    layout.clipmove(0, layout.len(), -1);
    layout.resize(layout.len() - 1);
    return ch;
}

bytes& bytes::flex_move(bytes::pos_type pos, bytes::size_type n, int offset) {
    layout.flexmove(pos, n, offset, [](pointer fill, size_type n) {
    });
    return *this;
}

bytes& bytes::flex_move(bytes::pos_type pos, bytes::size_type n, int offset, bytes::value_type fill_ch) {
    layout.flexmove(pos, n, offset, [fill_ch](pointer fill_pos, size_type n) {
        std::fill(fill_pos, fill_pos + n, fill_ch);
    });
    return *this;
}

bytes& bytes::clip_move(bytes::pos_type pos, bytes::size_type n, int offset) {
    layout.clipmove(pos, n, offset);
    return *this;
}

bytes& bytes::remove(bytes::pos_type pos) {
    ASSERT(pos >= 0);
    ASSERT(pos < layout.len());
    layout.clipmove(pos + 1, layout.len() - (pos + 1), -1);
    layout.resize(layout.len() - 1);
    return *this;
}

bytes& bytes::remove(bytes::pos_type pos, bytes::size_type n) {
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

bytes& bytes::remove(bytes::value_type ch) {
    return remove([ch](bytes::value_type c, bool& match) -> int {
        match = (c == ch);
        return 0;
    });
}

bytes& bytes::remove(bytes::const_pointer s) {
    ASSERT(s != nullptr);

    bytes::size_type slen = std::strlen(s);

    return remove([slen, s](bytes::const_pointer start, bytes::const_pointer end, bytes::const_pointer& match, bytes::size_type& n) -> int {
        if ((end - start) < slen) {
            match = nullptr;
            n = 0;
            return -1;
        }

        bytes::const_pointer ptr = std::strstr(start, s);
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

bytes& bytes::remove(const bytes& other) {
    return remove(other.layout.begin());
}

bytes& bytes::remove(const re& rx) {
    ASSERT(rx);

    return remove([&rx](bytes::const_pointer start, bytes::const_pointer end, bytes::const_pointer& match_ptr, bytes::size_type& match_n) -> int {
        if (end == start) {
            match_ptr = nullptr;
            match_n = 0;
            return -1;
        }

        //  从 start 位置开始只查找一个
        int cnt = rx.find(start, 0, [&match_ptr, &match_n](const re::segment_type* segs, re::size_type n) -> int {
            match_ptr = segs[0].ptr;
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

bytes& bytes::remove(std::function<int(bytes::value_type c, bool& match)> func) {
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

bytes& bytes::remove(std::function<int(bytes::const_pointer start, bytes::const_pointer end, bytes::const_pointer& match, bytes::size_type& n)> func) {
    bytes::pointer w = layout.begin();

    bytes::pointer r = layout.begin();
    while (*r != '\0') {

        bytes::const_pointer match_ptr = nullptr;
        bytes::size_type match_n = 0;
        int cntnu = func(r, layout.end(), match_ptr, match_n);

        //  没找到匹配项
        if (match_ptr == nullptr) {
            break;
        }

        ASSERT(match_n > 0);

        bytes::size_type mov_len = match_ptr - r;
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

bytes& bytes::erase(pos_type pos, pos_type n) {
    return remove(pos, n);
}

bytes::iterator bytes::erase(const_iterator p) {
    return erase(p, (p + 1));
}

bytes::iterator bytes::erase(const_iterator first, const_iterator last) {
    ASSERT(first >= layout.begin());
    ASSERT(first < layout.end());
    ASSERT(last > layout.begin());
    ASSERT(last <= layout.end());
    layout.clipmove(last - layout.begin(), layout.end() - last, -(last - first));
    return layout.begin() + (first - layout.begin());
}

int bytes::compare(const bytes& s) const {
    return strcmp(layout.begin(), s.layout.begin());
}

int bytes::compare(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return strcmp(layout.begin(), s);
}

int bytes::compare(bytes::const_pointer s, bytes::size_type max_n) const {
    ASSERT(s != nullptr);
    return strncmp(layout.begin(), s, max_n);
}

int bytes::compare(const bytes& other, bytes::size_type max_n) const {
    return strncmp(layout.begin(), other.layout.begin(), max_n);
}

int bytes::compare(bytes::value_type c) const {
    return (size() == 1) && (layout.begin()[0] == c);
}

bool bytes::contains(const bytes& s) const {
    return strstr(layout.begin(), s.begin()) != nullptr;
}

bool bytes::contains(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return strstr(layout.begin(), s) != nullptr;
}

bool bytes::contains(bytes::value_type ch) const {
    return strchr(layout.begin(), ch) != nullptr;
}

bool bytes::contains(const re& r) const {
    bool found = false;
    r.find(*this, 0, [&found](const re::segment_type* segment, re::size_type n) -> int {
        found = true;
        return -1;
    });
    return found;
}

int bytes::count(const bytes& other) const {
    int cnt = 0;

    size_type slen = other.size();

    const_pointer start = layout.begin();
    while ((start = std::strstr(start, other.data())) != nullptr) {
        cnt++;
        start += slen;
    }

    return cnt;
}

int bytes::count(bytes::const_pointer s) const {
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

int bytes::count(bytes::value_type ch) const {
    int cnt = 0;

    const_pointer start = layout.begin();
    while ((start = std::strchr(start, ch)) != nullptr) {
        cnt++;
        start++;
    }

    return cnt;
}

int bytes::count(const re& rx) const {
    ASSERT(rx);

    int cnt = 0;
    rx.find(layout.begin(), 0, [&cnt](const re::segment_type* segs, re::size_type n) -> int {
        cnt++;
        return 0;
    });
    return cnt;
}

bool bytes::has_suffix(const bytes& other) const {
    return has_suffix(other.data(), other.size());
}

bool bytes::has_suffix(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return has_suffix(s, std::strlen(s));
}

bool bytes::has_suffix(bytes::value_type ch) const {
    if (size() > 0) {
        return (back() == ch);
    }

    return ch == '\0';
}

bool bytes::has_suffix(bytes::const_pointer s, bytes::size_type n) const {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);

    if (n == 0) {
        return true;
    }

    auto pt = layout.end() - 1;
    bytes::const_pointer ps = s + (n - 1);
    while ((pt != (layout.begin() - 1)) && (ps != (s - 1))) {
        if (*pt != *ps) {
            break;
        }
        pt--;
        ps--;
    }

    return (ps == (s - 1));
}

bool bytes::has_prefix(const bytes& other) const {
    return has_prefix(other.data(), other.size());
}

bool bytes::has_prefix(bytes::const_pointer s) const {
    ASSERT(s != nullptr);

    bytes::const_pointer pt = layout.begin();
    bytes::const_pointer ps = s;
    while (*pt != '\0' && *ps != '\0') {
        if (*pt != *ps) {
            break;
        }
        pt++;
        ps++;
    }

    return (*ps == '\0');
}

bool bytes::has_prefix(bytes::value_type ch) const {
    if (size() > 0) {
        return (front() == ch);
    }

    return ch == '\0';
}

bool bytes::has_prefix(bytes::const_pointer s, bytes::size_type n) const {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);

    if (n == 0) {
        return true;
    }

    bytes::const_pointer pt = layout.begin();
    bytes::const_pointer ps = s;
    while ((pt != layout.end()) && (ps != (s + n))) {
        if (*pt != *ps) {
            break;
        }
        pt++;
        ps++;
    }

    return (ps == (s + n));
}

bool bytes::remove_prefix(const bytes& other) {
    return false;
}

bool bytes::remove_prefix(bytes::const_pointer s) {
    ASSERT(false); //  TODO - bool bytes::remove_prefix(bytes::const_pointer s)
    return false;
}

bool bytes::remove_prefix(bytes::value_type c) {
    ASSERT(false); //  TODO - bool bytes::remove_prefix(bytes::value_type c)
    return false;
}

bool bytes::remove_suffix(const bytes& s) {
    ASSERT(false); //  TODO - bool bytes::remove_suffix(const bytes& s)
    return false;
}

bool bytes::remove_suffix(bytes::const_pointer s) {
    ASSERT(false); //  TODO - bool bytes::remove_suffix(bytes::const_pointer s)
    return false;
}

bool bytes::remove_suffix(bytes::value_type c) {
    ASSERT(false); //  TODO - bool bytes::remove_suffix(bytes::value_type c)
    return false;
}

bytes& bytes::fill(bytes::value_type ch) {
    layout.fill(0, ch, layout.len());
    return *this;
}

bytes& bytes::fill(bytes::pos_type pos, bytes::value_type ch, bytes::size_type n) {
    ASSERT(n > 0);
    ASSERT(pos >= 0);
    ASSERT(pos < size());
    ASSERT((pos + n) >= 0);
    ASSERT((pos + n) <= size());

    layout.fill(pos, ch, layout.len());
    return *this;
}

bytes& bytes::fill(bytes::pos_type pos, bytes::const_pointer s, bytes::size_type n) {
    ASSERT(n > 0);
    ASSERT(s != nullptr);
    ASSERT(pos >= 0);
    ASSERT(pos < size());
    ASSERT((pos + n) >= 0);
    ASSERT((pos + n) <= size());

    layout.fill(pos, s, n);
    return *this;
}

bytes::pos_type bytes::index_of(const bytes& other, bytes::pos_type from) const {
    return index_of(other.data(), from);
}

bytes::pos_type bytes::index_of(bytes::const_pointer s, bytes::pos_type from) const {
    ASSERT(s != nullptr);
    ASSERT(from >= 0);
    ASSERT(from < size());

    const_pointer d = data();
    const_pointer pos = std::strstr(d + from, s);
    if (pos == nullptr) {
        return bytes::npos;
    }

    return bytes::pos_type(pos - d);
}

bytes::pos_type bytes::index_of(bytes::value_type ch, bytes::pos_type from) const {
    ASSERT(from >= 0);
    ASSERT(from < size());

    const_pointer d = data();
    const_pointer pos = std::strchr(d + from, ch);
    if (pos == nullptr) {
        return bytes::npos;
    }

    return bytes::pos_type(pos - d);
}

bytes::pos_type bytes::index_of(const re& rx, bytes::pos_type from) const {
    ASSERT(from >= 0);
    ASSERT(from < layout.len());

    bytes::pos_type pos = -1;
    rx.find(layout.begin() + from, 0, [&pos](const re::segment_type* segs, re::size_type n) -> int {
        pos = segs[0].pos;
        return bytes::npos;
    });

    return from + pos;
}

bytes::pos_type bytes::index_of(std::function<int(bytes::value_type c, bool& match)> func, bytes::pos_type from) const {

    bytes::const_pointer ptr = layout.begin();
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

    return bytes::npos;
}

bytes::pos_type bytes::last_index_of(const bytes& other, bytes::pos_type from) const {
    return last_index_of(other.data(), from);
}

bytes::pos_type bytes::last_index_of(bytes::value_type ch, bytes::pos_type from) const {
    ASSERT(from >= 0);
    ASSERT(from < size());

    const_pointer d = data();
    const_pointer pos = std::strrchr(d + from, ch);
    if (pos == nullptr) {
        return bytes::npos;
    }

    return bytes::pos_type(pos - d);
}

bytes::pos_type bytes::last_index_of(bytes::const_pointer s, bytes::pos_type from) const {
    ASSERT(from >= 0);
    ASSERT(from < size());

    const_pointer d = data();
    const_pointer pos = strrstr(d + from, s);
    if (pos == nullptr) {
        return bytes::npos;
    }

    return bytes::pos_type(pos - d);
}

bytes::pos_type bytes::last_index_of(const re& rx, bytes::pos_type from) const {
    ASSERT(false); //  TODO - bytes::pos_type bytes::last_index_of(const re& rx, bytes::pos_type from) const
    return false;
}

bytes::pos_type bytes::last_index_of(std::function<int(bytes::value_type c, bool& match)> func, bytes::pos_type from) const {
    ASSERT(false); //  TODO - bytes::pos_type bytes::last_index_of(std::function<int(bytes::value_type c, bool& match)> func, bytes::pos_type from) const
    return false;
}

bytes::pos_type bytes::section_of(bytes::pos_type from, bytes::const_pointer& s, bytes::size_type& n) const {
    ASSERT(from >= 0);
    ASSERT(from <= size());

    if (from == size()) {
        return bytes::npos;
    }

    bytes::const_pointer ptr = layout.begin() + from;

    //  找到第一个非空白的字符，确定起点
    while ((*ptr != '\0') && chars::match(*ptr, chars::SPACE)) {
        ptr++;
    }

    //  找下一个空白或者结束位置，确定终点 // 不是最优，因为这里多了依次额外的检测
    n = 0;
    while ((ptr[n] != '\0') && !chars::match(ptr[n], chars::SPACE)) {
        n++;
    }

    if (n <= 0) {
        return bytes::npos;
    }

    s = ptr;
    return bytes::pos_type(ptr - layout.begin());
}

bool bytes::is_match(const re& rx) const {
    return rx.match(*this, 0);
}

bool bytes::is_match(const bytes& pattern) const {
    return re(pattern).match(*this, 0);
}

bool bytes::is_match(bytes::const_pointer pattern) const {
    ASSERT(pattern != nullptr);
    return re(pattern).match(*this, 0);
}

bool bytes::is_match_wild(const bytes& pattern) const {
    return wildcmp(pattern.data(), data()) == 0;
}

bool bytes::is_match_wild(bytes::const_pointer pattern) const {
    ASSERT(pattern != nullptr);
    return wildcmp(pattern, data()) == 0;
}

bool bytes::is_match(uint16_t charset) const {
    if (layout.len() == 0) {
        return false;
    }

    for (const_pointer ptr = layout.begin(); ptr != layout.end(); ptr++) {
        if (0 == (chars::mapping[*ptr] & charset)) {
            return false;
        }
    }

    return true;
}

bool bytes::is_empty() const {
    return layout.len() == 0;
}

bool bytes::is_lower() const {
    return is_match(chars::UPPER);
}

bool bytes::is_upper() const {
    return is_match(chars::LOWER);
}

bool bytes::is_title() const {
    ASSERT(false); //  TODO - bool bytes::is_title() const
    return false;
}

bool bytes::is_digit() const {
    return is_match(chars::DIGIT);
}

bool bytes::is_ascii() const {
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

bool bytes::is_alpha() const {
    return is_match(chars::ALPHA);
}

bool bytes::is_alnum() const {
    return is_match(chars::ALNUM);
}

bool bytes::is_space() const {
    return is_match(chars::SPACE);
}

bool bytes::is_blank() const {
    return is_match(chars::BLANK);
}

bool bytes::is_print() const {
    return is_match(chars::PRINT);
}

bool bytes::is_graph() const {
    return is_match(chars::GRAPH);
}

bool bytes::is_identifier() const {

    if (layout.len() == 0) {
        return false;
    }

    const_pointer ptr = layout.begin();
    if (!((*ptr == '_') || chars::match(*ptr, chars::ALPHA))) {
        return false;
    }

    for (ptr++; ptr != layout.end(); ptr++) {
        if (!((*ptr == '_') || chars::match(*ptr, chars::ALNUM))) {
            return false;
        }
    }

    return true;
}

bool bytes::is_numeric() const {
    ASSERT(false); //  TODO - bool bytes::is_numeric() const
    return false;
}

bool bytes::is_bool() const {
    ASSERT(false); //  TODO - bool bytes::is_bool() const
    return false;
}

bytes bytes::left(bytes::size_type n) const {
    return bytes(layout.begin(), n);
}

bytes bytes::right(bytes::size_type n) const {
    return bytes(layout.end() - n, n);
}

bytes bytes::substr(pos_type pos, int offset) const {
    if (offset < 0) {
        return bytes(layout.begin() + pos + offset, -offset);
    }

    if (offset > 0) {
        return bytes(layout.begin() + pos, offset);
    }

    return bytes();
}

bytes& bytes::ljust(bytes::size_type width, bytes::value_type fill, bool truncate) {
    ASSERT(width >= 0);

    // 宽度大于 bytes 的总长度时，在尾部追加数据
    if (width > layout.len()) {
        bytes::size_type old_len = layout.len();
        layout.resize(width);
        layout.fill(old_len, fill, width - old_len);
        return *this;
    }

    //  宽度一样，啥也不需要做
    if (width == layout.len()) {
        return *this;
    }

    //  width 太小时，由 truncate 参数决定是否需要掐断
    if (truncate) {
        layout.resize(width);
    }

    return *this;
}

bytes& bytes::rjust(bytes::size_type width, bytes::value_type fill, bool truncate) {
    ASSERT(width >= 0);

    // 宽度大于 bytes 的总长度时，在尾部追加数据
    if (width > layout.len()) {
        bytes::size_type old_len = layout.len();
        layout.flexmove(0, old_len, (width - old_len), [](pointer fill, size_type n) {
        });
        layout.fill(0, fill, (width - old_len));
        return *this;
    }

    //  宽度一样，啥也不需要做
    if (width == layout.len()) {
        return *this;
    }

    //  width 太小时，由 truncate 参数决定是否需要掐断
    if (truncate) {
        layout.resize(width);
    }

    return *this;
}

bytes& bytes::center(bytes::size_type width, bytes::value_type fill_ch, bool truncate) {
    ASSERT(width >= 0);

    // 宽度大于 bytes 的总长度时，在尾部追加数据
    if (width > layout.len()) {
        bytes::size_type old_len = layout.len();
        bytes::size_type lfill_len = (width - old_len) / 2;
        bytes::size_type rfill_len = (width - old_len) - lfill_len;

        //  不够优秀：存在多余的一次拷贝
        layout.resize(width);

        //  字符串整体右移
        layout.flexmove(0, old_len, lfill_len, [](pointer fill, size_type n) {
        });

        layout.fill(0, fill_ch, lfill_len);
        layout.fill(lfill_len + old_len, fill_ch, rfill_len);
        return *this;
    }

    //  宽度一样，啥也不需要做
    if (width == layout.len()) {
        return *this;
    }

    //  width 太小时，由 truncate 参数决定是否需要掐断
    if (truncate) {
        layout.resize(width);
    }

    return *this;
}

bytes& bytes::zfill(bytes::size_type width, bytes::value_type fill, bool truncate) {
    ASSERT(false); //  TODO - bytes& bytes::zfill(bytes::size_type width, bytes::value_type fill, bool truncate)
    return *this;
}

bytes& bytes::replace(bytes::pos_type pos, bytes::size_type n, const bytes& after) {
    ASSERT(false); //  TODO - bytes& bytes::replace(bytes::pos_type pos, bytes::size_type n, const bytes& after)
    return *this;
}

bytes& bytes::replace(bytes::pos_type pos, bytes::size_type n, bytes::value_type after) {
    ASSERT(false); //  TODO - bytes& bytes::replace(bytes::pos_type pos, bytes::size_type n, bytes::value_type after)
    return *this;
}

bytes& bytes::replace(bytes::pos_type pos, bytes::size_type n, bytes::const_pointer unicode, bytes::size_type size) {
    ASSERT(false); //  TODO - bytes& bytes::replace(bytes::pos_type pos, bytes::size_type n, bytes::const_pointer unicode, bytes::size_type size)
    return *this;
}

bytes& bytes::replace(bytes::value_type before, bytes::value_type after) {
    ASSERT(false); //  TODO - bytes& bytes::replace(bytes::value_type before, bytes::value_type after)
    return *this;
}

bytes& bytes::replace(bytes::const_pointer before, bytes::size_type blen, bytes::const_pointer after, bytes::size_type alen) {
    ASSERT(false); //  TODO - bytes& bytes::replace(bytes::const_pointer before, bytes::size_type blen, bytes::const_pointer after, bytes::size_type alen)
    return *this;
}

bytes& bytes::replace(const bytes& before, const bytes& after) {
    ASSERT(false); //  TODO - bytes& bytes::replace(const bytes& before, const bytes& after)
    return *this;
}

bytes& bytes::replace(bytes::value_type ch, const bytes& after) {
    ASSERT(false); //  TODO - bytes& bytes::replace(bytes::value_type ch, const bytes& after)
    return *this;
}

bytes& bytes::replace(const re& rx, const bytes& after) {
    ASSERT(false); //  TODO - bytes& bytes::replace(const re& rx, const bytes& after)
    return *this;
}

bytes& bytes::replace(std::function<int(bytes::value_type key, bytes::value_type& val)> func) {
    ASSERT(false); //  TODO - bytes& bytes::replace(std::function<int(bytes::value_type key, bytes::value_type& val)> func)
    return *this;
}

bytes bytes::repeat(bytes::size_type times) const {
    ASSERT(times >= 0);

    bytes result;

    result.resize(size() * times);

    bytes::pos_type pos = 0;
    for (bytes::size_type i = 0; i < times; i++) {
        result.fill(pos + i * size(), data(), size());
    }

    return result;
}

bytes bytes::join(const std::vector<bytes>& s) const {
    auto itr = s.begin();
    return join([&s, &itr]() -> bytes::const_pointer {
        bytes::const_pointer s = itr->data();
        itr++;
        return s;
    });
}

bytes bytes::join(bytes::const_pointer s, ...) const {
    va_list valist;
    va_start(valist, s);

    bytes result = join([&valist]() -> bytes::const_pointer {
        return va_arg(valist, bytes::const_pointer);
    });

    va_end(valist);

    return result;
}

bytes bytes::join(std::function<bytes::const_pointer()> provider) const {
    bytes result;

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

bytes bytes::join(std::initializer_list<bytes::const_pointer> ptr_list) const {
    auto itr = ptr_list.begin();
    return join([&ptr_list, &itr]() -> bytes::const_pointer {
        if (itr == ptr_list.end()) {
            return nullptr;
        }

        return *(itr++);
    });
}

bytes bytes::join(std::initializer_list<bytes> ptr_list) const {
    auto itr = ptr_list.begin();
    return join([&ptr_list, &itr]() -> bytes::const_pointer {
        if (itr == ptr_list.end()) {
            return nullptr;
        }

        return (*(itr++)).data();
    });
}

void bytes::reserve(bytes::size_type cap) {
    layout.reserve(cap);
}

void bytes::resize(bytes::size_type n) {
    layout.resize(n);
}

void bytes::resize(bytes::size_type n, bytes::value_type fill_ch) {
    ASSERT(n >= 0);

    bytes::size_type old_len = size();
    layout.resize(n);
    if (n > old_len) {
        layout.fill(old_len, fill_ch, (n - old_len));
    }
}

void bytes::shrink_to_fit() {
}

void bytes::squeeze() {
}

bytes& bytes::title() {
    return *this;
}

bytes& bytes::inversion() {
    return *this;
}

std::vector<bytes> bytes::split(const bytes& sep) const {
    std::vector<bytes> result;
    split(sep, [&result](const_pointer s, size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<bytes> bytes::split(bytes::const_pointer sep) const {
    std::vector<bytes> result;
    split(sep, [&result](bytes::const_pointer s, bytes::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<bytes> bytes::split(bytes::value_type sep) const {
    std::vector<bytes> result;
    split(sep, [&result](bytes::const_pointer s, bytes::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<bytes> bytes::split(const re& r) const {
    std::vector<bytes> result;
    split(r, [&result](bytes::const_pointer s, bytes::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<bytes> bytes::split(std::function<int(bytes::value_type c, bool& match)>& chars_func) const {
    std::vector<bytes> result;
    split(chars_func, [&result](bytes::const_pointer s, bytes::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<bytes> bytes::split_lines(bool keep_ends) const {
    std::vector<bytes> result;
    split_lines(keep_ends, [&result](bytes::const_pointer s, bytes::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<bytes> bytes::split_path() const {
    std::vector<bytes> result;
    split_path([&result](bytes::const_pointer s, bytes::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

void bytes::split(const bytes& sep, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void bytes::split(const bytes& sep, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const
}

void bytes::split(bytes::const_pointer sep, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void bytes::split(bytes::const_pointer sep, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const
}

void bytes::split(bytes::value_type sep, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void bytes::split(bytes::value_type sep, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const
}

void bytes::split(const re& rx, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const {
    rx.split(*this, 0, [&output_func](const re::segment_type& segs) -> int {
        output_func(segs.ptr, segs.len);
        return 0;
    });
}

void bytes::split(std::function<int(bytes::value_type c, bool& match)>& chars_func, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void bytes::split(std::function<int(bytes::value_type c, bool& match)>& chars_func, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const
}

void bytes::split_lines(bool keep_ends, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void bytes::split_lines(bool keep_ends, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const
}

void bytes::split_path(std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void bytes::split_path(std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const
}

bytes& bytes::to_lower() {
    std::transform(layout.begin(), layout.end(), layout.begin(), ::tolower);
    return *this;
}

bytes& bytes::to_upper() {
    std::transform(layout.begin(), layout.end(), layout.begin(), ::toupper);
    return *this;
}

bytes& bytes::swap_case() {
    std::transform(layout.begin(), layout.end(), layout.begin(), [](bytes::value_type& ch) -> bytes::value_type {
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

bytes& bytes::simplified() {
    bytes::pointer w = layout.begin();
    bytes::pos_type pos = 0;

    const_pointer ptr = nullptr;
    size_type len = 0;
    while (true) {
        pos = section_of(pos, ptr, len);
        if (pos == bytes::npos) {
            break;
        }

        ASSERT(ptr != nullptr);
        ASSERT(len > 0);

        if (w != layout.begin()) {
            *(w++) = ' ';
        }
        if (w < ptr) {
            std::memmove(w, ptr, len);
        }
        w += len;
        pos += len;
    }

    layout.resize(w - layout.begin());
    return *this;
}

bytes& bytes::ltrim() {
    return ltrim_until([](bytes::value_type ch) -> bool {
        return !std::isspace(ch);
    });
}

bytes& bytes::rtrim() {
    return rtrim_until([](bytes::value_type ch) -> bool {
        return !std::isspace(ch);
    });
}

bytes& bytes::trim() {
    return this->rtrim().ltrim();
}

bytes& bytes::ltrim(std::function<bool(bytes::value_type ch)> func) {
    bytes::pointer p = nullptr;
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

bytes& bytes::rtrim(std::function<bool(bytes::value_type ch)> func) {
    bytes::pointer p = nullptr;
    for (p = layout.end() - 1; p != (layout.begin() - 1); p--) {
        if (!func(*p)) {
            break;
        }
    }
    layout.resize((p + 1) - layout.begin());
    return *this;
}

bytes& bytes::trim(std::function<bool(bytes::value_type ch)> func) {
    return rtrim(func).ltrim(func);
}

bytes& bytes::ltrim_until(std::function<bool(bytes::value_type ch)> func) {
    return ltrim([&func](bytes::value_type ch) -> bool {
        return !func(ch);
    });
}

bytes& bytes::rtrim_until(std::function<bool(bytes::value_type ch)> func) {
    return rtrim([func](bytes::value_type ch) -> bool {
        return !func(ch);
    });
}

bytes& bytes::trim_until(std::function<bool(bytes::value_type ch)> func) {
    return rtrim_until(func).ltrim_until(func);
}

bytes bytes::expand(const std::map<bytes, bytes>& kvs) const {
    ASSERT(false); // TODO - bytes bytes::expand(const std::map<bytes, bytes>& kvs) const
    return bytes("");
}

bytes bytes::expand(std::function<int(const bytes& key, bytes& val)> provider) const {
    ASSERT(false); // TODO - bytes bytes::expand(std::function<int(const bytes& key, bytes& val)> provider) const
    return bytes("");
}

bytes bytes::expand_envs(bytes::const_pointer key, bytes::const_pointer val) const {
    ASSERT(false); // TODO - bytes bytes::expand_envs(bytes::const_pointer key, bytes::const_pointer val) const
    return bytes("");
}

bytes bytes::expand_envs() const {
    ASSERT(false); // TODO - bytes bytes::expand_envs() const
    return bytes("");
}

bytes bytes::expand_tabs(bytes::size_type tab_size) const {
    ASSERT(false); // TODO - bytes bytes::expand_tabs(bytes::size_type tab_size) const
    return bytes("");
}

bytes bytes::expand_tmpl(const std::map<bytes, bytes>& kvs) const {
    ASSERT(false); // TODO - bytes bytes::expand_tmpl(const std::map<bytes, bytes>& kvs) const
    return bytes("");
}

bytes bytes::expand_tmpl(std::function<int(const bytes& key, bytes& val)> provider) const {
    ASSERT(false); // TODO - bytes bytes::expand_tmpl(std::function<int(const bytes& key, bytes& val)> provider) const
    return bytes("");
}

void bytes::swap(bytes& s) {
    if (&s == this) {
        return;
    }

    layout_tmplt t = s.layout.tmplt;
    s.layout.tmplt = layout.tmplt;
    layout.tmplt = t;
}

bytes::size_type bytes::copy(bytes::pointer dest, bytes::size_type n, bytes::pos_type pos) const {
    ASSERT(dest != nullptr);
    ASSERT(n > 0);
    ASSERT(pos >= 0);
    ASSERT(pos < size());

    bytes::size_type copy_n = std::min(n, (size() - pos));
    std::memcpy(dest, layout.begin() + pos, copy_n);

    return copy_n;
}

bytes bytes::basename() const {
    ASSERT(false); // TODO - bytes bytes::basename() const
    return bytes("");
}

bytes& bytes::basename() {
    ASSERT(false); // TODO - bytes& bytes::basename()
    return *this;
}

bytes bytes::dirname() const {
    ASSERT(false); // TODO - bytes bytes::dirname() const
    return bytes("");
}

bytes& bytes::dirname() {
    ASSERT(false); // TODO - bytes& bytes::dirname()
    return *this;
}

bool bytes::to_bool(bool* ok) const {
    ASSERT(false); // TODO - bool bytes::to_bool(bool* ok) const
}

bytes& bytes::assign(bool v) {
    if (v) {
        layout.resize(4);
        layout.fill(0, "true", 4);
    } else {
        layout.resize(5);
        layout.fill(0, "false", 5);
    }
    return *this;
}

double bytes::to_double(bool* ok) const {
    ASSERT(false); // TODO - double bytes::to_double(bool* ok) const
    return 0.0;
}

float bytes::to_float(bool* ok) const {
    ASSERT(false); // TODO - float bytes::to_float(bool* ok) const
    return 0.0;
}

int8_t bytes::to_int8(bool* ok, int base) const {
    ASSERT(false); // TODO - int8_t bytes::to_int8(bool* ok, int base) const
    return 0;
}

int16_t bytes::to_int16(bool* ok, int base) const {
    ASSERT(false); //  TODO - int16_t bytes::to_int16(bool* ok, int base) const
    return 0;
}

int32_t bytes::to_int32(bool* ok, int base) const {
    ASSERT(false); //  TODO - int32_t bytes::to_int32(bool* ok, int base) const
    return 0;
}

int64_t bytes::to_int64(bool* ok, int base) const {
    ASSERT(false); //  TODO - int64_t bytes::to_int64(bool* ok, int base) const
    return 0;
}

uint8_t bytes::to_uint8(bool* ok, int base) const {
    ASSERT(false); //  TODO - uint8_t bytes::to_uint8(bool* ok, int base) const
    return 0;
}

uint16_t bytes::to_uint16(bool* ok, int base) const {
    ASSERT(false); //  TODO - uint16_t bytes::to_uint16(bool* ok, int base) const
    return 0;
}

uint32_t bytes::to_uint32(bool* ok, int base) const {
    ASSERT(false); //  TODO - uint32_t bytes::to_uint32(bool* ok, int base) const
    return 0;
}

uint64_t bytes::to_uint64(bool* ok, int base) const {
    ASSERT(false); //  TODO - uint64_t bytes::to_uint64(bool* ok, int base) const
    return 0;
}

bytes& bytes::assign(double n, bytes::value_type format, int precision) {
    ASSERT(false); //  TODO - bytes& bytes::assign(double n, bytes::value_type format, int precision)
    return *this;
}

bytes& bytes::assign(float n, bytes::value_type format, int precision) {
    ASSERT(false); //  TODO - bytes& bytes::assign(float n, bytes::value_type format, int precision)
    return *this;
}

bytes& bytes::assign(int8_t n, int base) {
    ASSERT(false); //  TODO - bytes& bytes::assign(int8_t n, int base)
    return *this;
}

bytes& bytes::assign(int16_t n, int base) {
    ASSERT(false); //  TODO - bytes& bytes::assign(int16_t n, int base)
    return *this;
}

bytes& bytes::assign(int32_t n, int base) {
    ASSERT(false); //  TODO - bytes& bytes::assign(int32_t n, int base)
    return *this;
}

bytes& bytes::assign(int64_t n, int base) {
    ASSERT(false); //  TODO - bytes& bytes::assign(int64_t n, int base)
    return *this;
}

bytes& bytes::assign(uint8_t n, int base) {
    ASSERT(false); //  TODO - bytes& bytes::assign(uint8_t n, int base)
    return *this;
}

bytes& bytes::assign(uint16_t n, int base) {
    ASSERT(false); //  TODO - bytes& bytes::assign(uint16_t n, int base)
    return *this;
}

bytes& bytes::assign(uint32_t n, int base) {
    ASSERT(false); //  TODO - bytes& bytes::assign(uint32_t n, int base)
    return *this;
}

bytes& bytes::assign(uint64_t n, int base) {
    ASSERT(false); //  TODO - bytes& bytes::assign(uint64_t n, int base)
    return *this;
}

int32_t bytes::hash_code() const {
    ASSERT(false); //  TODO - int32_t bytes::hash_code() const
    return 0;
}

bytes bytes::number(double n, bytes::value_type format, int precision) {
    ASSERT(false); //  TODO - bytes bytes::number(double n, bytes::value_type format, int precision)
    return bytes("");
}

bytes bytes::number(float n, bytes::value_type format, int precision) {
    ASSERT(false); //  TODO - bytes bytes::number(float n, bytes::value_type format, int precision)
    return bytes("");
}

bytes bytes::number(int8_t n, int base) {
    return bytes("");
}

bytes bytes::number(int16_t n, int base) {
    ASSERT(false); //  TODO - bytes bytes::number(int16_t n, int base)
    return bytes("");
}

bytes bytes::number(int32_t n, int base) {
    ASSERT(false); //  TODO - bytes bytes::number(int32_t n, int base)
    return bytes("");
}

bytes bytes::number(int64_t n, int base) {
    ASSERT(false); //  TODO - bytes bytes::number(int64_t n, int base)
    return bytes("");
}

bytes bytes::number(uint8_t n, int base) {
    ASSERT(false); //  TODO - bytes bytes::number(uint8_t n, int base)
    return bytes("");
}

bytes bytes::number(uint16_t n, int base) {
    ASSERT(false); //  TODO - bytes bytes::number(uint16_t n, int base)
    return bytes("");
}

bytes bytes::number(uint32_t n, int base) {
    ASSERT(false); //  TODO - bytes bytes::number(uint32_t n, int base)
    return bytes("");
}

bytes bytes::number(uint64_t n, int base) {
    ASSERT(false); //  TODO - bytes bytes::number(uint64_t n, int base)
    return bytes("");
}

bool bytes::operator!=(bytes::const_pointer s) const {
    return 0 != compare(s);
}

bytes& bytes::operator+=(bytes::value_type ch) {
    return append(ch);
}

bytes& bytes::operator+=(const bytes& s) {
    return append(s);
}

bytes& bytes::operator+=(bytes::const_pointer s) {
    ASSERT(s != nullptr);
    return append(s);
}

bool bytes::operator<(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) < 0;
}

bool bytes::operator<=(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) <= 0;
}

bytes& bytes::operator=(bytes::value_type ch) {
    layout.destroy();
    layout.init(&ch, 1);
    return *this;
}

bytes& bytes::operator=(bytes::const_pointer s) {
    ASSERT(s != nullptr);
    layout.destroy();
    layout.init(s, std::strlen(s));
    return *this;
}

bool bytes::operator==(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) == 0;
}

bool bytes::operator>(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) > 0;
}

bool bytes::operator>=(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) >= 0;
}

bytes::value_type& bytes::operator[](bytes::pos_type pos) {
    return layout.begin()[pos];
}

const bytes::value_type& bytes::operator[](bytes::pos_type pos) const {
    return layout.begin()[pos];
}

extern bool operator!=(const bytes& s1, const bytes& s2) {
    return s2.compare(s1) != 0;
}

extern bool operator!=(bytes::const_pointer s1, const bytes& s2) {
    return s2.compare(s1) != 0;
}

extern const bytes operator+(const bytes& s1, const bytes& s2) {
    ASSERT(false); //  TODO - extern const bytes operator+(const bytes& s1, const bytes& s2)
    bytes new_str;
    return new_str;
}

extern const bytes operator+(const bytes& s1, bytes::const_pointer s2) {
    ASSERT(false); //  TODO - extern const bytes operator+(const bytes& s1, bytes::const_pointer s2)
    bytes new_str;
    return new_str;
}

extern const bytes operator+(bytes::const_pointer s1, const bytes& s2) {
    ASSERT(false); //  TODO - extern const bytes operator+(bytes::const_pointer s1, const bytes& s2)
    bytes new_str;
    return new_str;
}

extern const bytes operator+(bytes::value_type ch, const bytes& s) {
    ASSERT(false); //  TODO - extern const bytes operator+(bytes::value_type ch, const bytes& s)
    bytes new_str;
    return new_str;
}

extern const bytes operator+(const bytes& other, bytes::value_type ch) {
    ASSERT(false); //  TODO - extern const bytes operator+(const bytes& other, bytes::value_type ch)
    bytes new_str;
    return new_str;
}

extern bool operator<(const bytes& s1, const bytes& s2) {
    return s2.compare(s1) > 0;
}

extern bool operator<(bytes::const_pointer s1, const bytes& s2) {
    return s2.compare(s1) > 0;
}

extern bool operator<=(const bytes& s1, const bytes& s2) {
    return s2.compare(s1) >= 0;
}

extern bool operator<=(bytes::const_pointer s1, const bytes& s2) {
    return s2.compare(s1) >= 0;
}

extern bool operator==(const bytes& s1, const bytes& s2) {
    return s2.compare(s1) == 0;
}

extern bool operator==(bytes::const_pointer s1, const bytes& s2) {
    return s2.compare(s1) == 0;
}

extern bool operator>(const bytes& s1, const bytes& s2) {
    return s2.compare(s1) < 0;
}

extern bool operator>(bytes::const_pointer s1, const bytes& s2) {
    return s2.compare(s1) < 0;
}

extern bool operator>=(const bytes& s1, const bytes& s2) {
    return s2.compare(s1) <= 0;
}

extern bool operator>=(bytes::const_pointer s1, const bytes& s2) {
    return s2.compare(s1) <= 0;
}

} // namespace tiny