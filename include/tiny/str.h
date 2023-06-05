//
// Created by libbylg on 2023/6/1.
//

#ifndef TINY_STR_H
#define TINY_STR_H

#include <functional>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include <cstring>

namespace str {
using size_type = std::string::size_type;
using ssize_type = ssize_t;
using value_type = std::string::value_type;
using reference = std::string::reference;
using const_reference = std::string::const_reference;
using pointer = std::string::pointer;
using const_pointer = std::string::const_pointer;

static inline const size_type npos = std::string::npos;

//  首尾追加
static inline auto append(std::string& s, const std::string& other) -> std::string& {
    return s.append(other);
}

static inline auto append(std::string& s, value_type ch) -> std::string& {
    return s.append(&ch, 1);
}

static inline auto append(std::string& s, value_type ch, size_type n) -> std::string& {
    s.resize(s.size() + n, ch);
    return s;
}

static inline auto append(std::string& s, std::string_view other) -> std::string& {
    return s.append(other);
}

static inline auto append(std::string& s, const std::vector<std::string>& items) -> std::string& {
    size_type len = 0;
    for (const auto& item : items) {
        len += item.size();
    }

    s.reserve(s.size() + len);

    for (const auto& item : items) {
        s.append(item);
    }

    return s;
}

static inline auto append(std::string& s, std::initializer_list<std::string> items) -> std::string& {
    size_type len = 0;
    for (const auto& item : items) {
        len += item.size();
    }

    s.reserve(s.size() + len);

    for (const auto& item : items) {
        s.append(item);
    }

    return s;
}

static inline auto append(std::string& s, std::initializer_list<std::string_view> items) -> std::string& {
    for (const auto& item : items) {
        s.append(item);
    }
    return s;
}

static inline auto append(std::string& s, std::function<const_pointer()> proc) -> std::string& {
    for (auto item = proc(); item != nullptr; item = proc()) {
        s.append(item);
    }
    return s;
}

static inline auto prepend(std::string& s, std::string_view other) -> std::string& {
    if (s.capacity() < (s.size() + other.size() + 1)) {
        std::string result;
        result.append(other).append(s);
        s = std::move(result);
        return s;
    }

    s.resize(s.size() + other.size());
    std::memmove(s.data() + other.size(), s.c_str(), s.size() * sizeof(value_type));
    std::memcpy(s.data(), other.data(), other.size());
    return s;
}

static inline auto prepend(std::string& s, const std::string& other) -> std::string& {
    return prepend(s, std::string_view{ other.data(), other.size() });
}

static inline auto prepend(std::string& s, value_type ch) -> std::string& {
    return prepend(s, std::string_view{ &ch, 1 });
}

static inline auto prepend(std::string& s, value_type ch, size_type n) -> std::string& {
    if (s.capacity() < (s.size() + n + 1)) {
        std::string result;
        result.reserve(s.size() + n);
        result.resize(n, ch);
        result.append(s);
        s = std::move(result);
        return s;
    }

    s.resize(s.size() + n);
    std::memmove(s.data() + n, s.c_str(), s.size() * sizeof(value_type));
    s[0] = ch;
    return s;
}

//  修改字符串：中间插入、首尾插入、任意位置删除

static inline auto insert(std::string& s, size_type pos, const std::string_view& other) -> std::string& {
    if (s.capacity() < (s.size() + other.size() + 1)) {
        std::string result;
        result.resize(other.size() + s.size());
        std::memcpy(result.data(), s.c_str(), pos);
        std::memcpy(result.data() + pos, other.data(), other.size());
        std::memcpy(result.data() + pos + other.size(), s.c_str() + pos, s.size() - pos);
        s = std::move(result);
        return s;
    }

    s.resize(other.size() + s.size());
    std::memmove(s.data() + pos + other.size(), s.c_str() + pos, s.size() - pos);
    std::memcpy(s.data() + pos, other.data(), other.size());
    return s;
}

static inline auto insert(std::string& s, size_type pos, const std::string& other) -> std::string& {
    return insert(s, pos, std::string_view{ other });
}

static inline auto insert(std::string& s, size_type pos, value_type ch) -> std::string& {
    return insert(s, pos, std::string_view{ &ch, 1 });
}

//  首尾操作
static inline auto push_back(std::string& s, std::string_view other) -> std::string& {
    return append(s, other);
}

static inline auto push_back(std::string& s, const std::string& other) -> std::string& {
    return append(s, other);
}

static inline auto push_back(std::string& s, value_type ch) -> std::string& {
    return append(s, ch);
}

static inline auto push_front(std::string& s, std::string_view other) -> std::string& {
    return append(s, other);
}

static inline auto push_front(std::string& s, const std::string& other) -> std::string& {
    return prepend(s, other);
}

static inline auto push_front(std::string& s, value_type ch) -> std::string& {
    return prepend(s, ch);
}

static inline auto pop_back(std::string& s) -> value_type {
    if (s.size() == 0) {
        return '\0';
    }

    value_type ch = s.back();
    s.resize(s.size() - 1);
    return ch;
}

static inline auto pop_front(std::string& s) -> value_type {
    value_type ch = s.front();
    std::memmove(s.data(), s.data() + 1, s.size() - 1);
    s.resize(s.size() - 1);
    return ch;
}

static inline auto pop_back_field(std::string& s) -> std::string;
static inline auto pop_front_field(std::string& s) -> std::string;

//  部分数据移动：柔性移动和裁剪移动
static inline auto flex_move(std::string& s, size_type pos, size_type n, ssize_type offset) -> std::string&;
static inline auto flex_move(std::string& s, size_type pos, size_type n, ssize_type offset, value_type ch) -> std::string&;
static inline auto clip_move(std::string& s, size_type pos, size_type n, ssize_type offset) -> std::string&;

//  删除数据
static inline auto remove(std::string& s, size_type pos) -> std::string&;
static inline auto remove(std::string& s, size_type pos, size_type n) -> std::string&;
static inline auto remove(std::string& s, value_type ch) -> std::string&;
static inline auto remove(std::string& s, std::string_view other) -> std::string&;
static inline auto remove(std::string& s, const std::string& other) -> std::string&;
static inline auto remove(std::string& s, std::function<bool(value_type ch, bool& cntu)> func) -> std::string&;
static inline auto remove(std::string& s, std::function<int(const_pointer search, size_type search_n, const_pointer& match, size_type& match_n)> func) -> std::string&;

//  比较
static inline auto compare(const std::string& s, const std::string& other) -> int;
static inline auto compare(const std::string& s, const_pointer other) -> int;
static inline auto compare(const std::string& s, const std::string& other, size_type max_n) -> int;
static inline auto compare(const std::string& s, const_pointer other, size_type max_n) -> int;
static inline auto compare(const std::string& s, value_type c) -> int;
static inline auto icompare(const std::string& s, const std::string& other) -> int;
static inline auto icompare(const std::string& s, const_pointer other) -> int;
static inline auto icompare(const std::string& s, const std::string& other, size_type max_n) -> int;
static inline auto icompare(const std::string& s, const_pointer other, size_type max_n) -> int;
static inline auto icompare(const std::string& s, value_type c) -> int;
static inline auto iequals(const std::string& s, const std::string& other) -> int;
static inline auto iequals(const std::string& s, const_pointer other) -> int;
static inline auto iequals(const std::string& s, const std::string& other, size_type max_n) -> int;
static inline auto iequals(const std::string& s, const_pointer other, size_type max_n) -> int;
static inline auto iequals(const std::string& s, value_type c) -> int;

//  是否包含子串
static inline auto contains(const std::string& s, const std::string& other) -> bool;
static inline auto contains(const std::string& s, std::string_view other) -> bool;
static inline auto contains(const std::string& s, value_type ch) -> bool;

//  子串统计
static inline auto count(const std::string& s, std::string_view other) -> size_type;
static inline auto count(const std::string& s, const std::string& other) -> size_type;
static inline auto count(const std::string& s, value_type ch) -> size_type;
static inline auto count(const std::string& s, std::function<bool(value_type ch, bool& cntu)> macher) -> size_type;

//  前缀操作
static inline auto prefix(const std::string& s, const std::string& other) -> size_type {
    if ((s.empty()) || (other.empty())) {
        return 0;
    }

    size_type len = std::min(s.size(), other.size());
    for (size_type pos = 0; pos < len; pos++) {
        if (s[pos] != other[pos]) {
            return pos;
        }
    }

    return len;
}

static inline auto prefix(const std::string& s, std::string_view other) -> size_type {
    if ((s.empty()) || (other.empty())) {
        return 0;
    }

    size_type len = std::min(s.size(), other.size());
    for (size_type pos = 0; pos < len; pos++) {
        if (s[pos] != other[pos]) {
            return pos;
        }
    }

    return len;
}

static inline auto has_prefix(const std::string& s, value_type ch) -> bool {
    if (s.empty()) {
        return false;
    }

    return s[0] == ch;
}

// static inline auto has_prefix(const std::string& s, const std::string& prefix) -> bool {
//    return str::prefix(s, prefix) == prefix.size();
//}

static inline auto has_prefix(const std::string& s, const std::string_view& prefix) -> bool {
    return str::prefix(s, prefix) == prefix.size();
}

static inline auto starts_with(const std::string& s, value_type ch) -> bool {
    return has_prefix(s, ch);
}

// static inline auto starts_with(const std::string& s, const std::string& prefix) -> bool {
//     return has_prefix(s, prefix);
// }

static inline auto starts_with(const std::string& s, const std::string_view& prefix) -> bool {
    return has_prefix(s, prefix);
}

// static inline auto remove_prefix(std::string& s, const std::string& prefix) -> std::string&;

static inline auto remove_prefix(std::string& s, std::string_view prefix) -> std::string& {
    if (!has_prefix(s, prefix)) {
        return s;
    }

    std::memmove(s.data(), s.c_str() + prefix.size(), s.size() - prefix.size());
    s.resize(s.size() - prefix.size());
    return s;
}

static inline auto remove_prefix(std::string& s, value_type prefix) -> std::string& {
    return remove_prefix(s, { &prefix, 1 });
}

//  后缀操作
static inline auto suffix(const std::string& s, const std::string& other) -> size_type {
    if ((s.empty()) || (other.empty())) {
        return 0;
    }

    size_type len = std::min(s.size(), other.size());
    const_pointer ptr_s = &s.back();
    const_pointer ptr_other = &other.back();
    while (ptr_s != (&s.back() - len)) {
        if (*ptr_s != *ptr_other) {
            return &s.back() - ptr_s;
        }

        ptr_s--;
        ptr_other--;
    }

    return len;
}

static inline auto suffix(const std::string& s, std::string_view other) -> size_type {
    if ((s.empty()) || (other.empty())) {
        return 0;
    }

    size_type len = std::min(s.size(), other.size());
    const_pointer ptr_s = &s.back();
    const_pointer ptr_other = &other.back();
    while (ptr_s != (&s.back() - len)) {
        if (*ptr_s != *ptr_other) {
            return &s.back() - ptr_s;
        }

        ptr_s--;
        ptr_other--;
    }

    return len;
}

static inline auto has_suffix(const std::string& s, value_type suffix) -> bool {
    if (s.empty()) {
        return false;
    }

    return s[0] == suffix;
}

// static inline auto has_suffix(const std::string& s, const std::string& suffix) -> bool {
//     return str::suffix(s, suffix) == suffix.size();
// }

static inline auto has_suffix(const std::string& s, const std::string_view& suffix) -> bool {
    return str::suffix(s, suffix) == suffix.size();
}

static inline auto ends_with(const std::string& s, value_type suffix) -> bool {
    return has_suffix(s, suffix);
}

// static inline auto ends_with(const std::string& s, const std::string& suffix) -> bool {
//     return has_suffix(s, suffix);
// }

static inline auto ends_with(const std::string& s, const std::string_view& suffix) -> bool {
    return has_suffix(s, suffix);
}

static inline auto remove_suffix(std::string& s, const std::string_view& suffix) -> std::string& {
    if (!has_prefix(s, suffix)) {
        return s;
    }

    s.resize(s.size() - suffix.size());
    return s;
}

static inline auto remove_suffix(std::string& s, value_type suffix) -> std::string& {
    return remove_suffix(s, { &suffix, 1 });
}

// static inline auto remove_suffix(std::string& s, const std::string& suffix) -> std::string& {
//}

//  填充
static inline auto fill(std::string& s, const std::string_view& other, size_type pos = 0, size_type max_n = npos) -> std::string& {
    if (s.empty() || other.empty()) {
        return s;
    }

    if (pos >= s.size()) {
        return s;
    }

    if (max_n == std::string::npos) {
        max_n = s.size() - pos;
    }

    pointer ptr = (s.data() + pos);
    for (size_type i = 0; i < (max_n / other.size()); i++) {
        std::memcpy(ptr, other.data(), other.size());
        ptr += other.size();
    }

    if ((max_n % other.size()) >= 0) {
        std::memcpy(ptr, other.data(), (max_n % other.size()));
    }

    return s;
}

static inline auto fill(std::string& s, value_type ch, size_type pos = 0, size_type max_n = npos) -> std::string& {
    if (s.empty()) {
        return s;
    }

    if (pos >= s.size()) {
        return s;
    }

    if (max_n == std::string::npos) {
        max_n = s.size() - pos;
    }

    std::fill(s.data() + pos, s.data() + pos + max_n, ch);
    return s;
}

// static inline auto fill(std::string& s, size_type pos, size_type max_n, const std::string& other) -> std::string& {
// }

// static inline auto fill(std::string& s, value_type ch) -> std::string& {
//     std::fill(s.begin(), s.end(), ch);
//     return s;
// }
//
// static inline auto fill(std::string& s, size_type pos, value_type ch) -> std::string& {
//     if (pos >= s.size()) {
//         return s;
//     }
//
//     std::fill(s.begin() + pos, s.end(), ch);
//     return s;
// }

//  查找
static inline auto index_of(const std::string& s, size_type pos, value_type ch) -> size_type;
static inline auto index_of(const std::string& s, size_type pos, const std::string& other) -> size_type;
static inline auto index_of(const std::string& s, size_type pos, const std::string_view& other) -> size_type;
static inline auto index_of_eol(const std::string& s, size_type pos) -> size_type;
static inline auto last_index_of(const std::string& s, const std::string& other) -> size_type;
static inline auto last_index_of(const std::string& s, const std::string_view& other) -> size_type;
static inline auto last_index_of(const std::string& s, value_type ch) -> size_type;
static inline auto last_index_of_eol(const std::string& s, size_type pos) -> size_type;

//  按空格分割的字段查找
static inline auto index_of_field(const std::string& s, size_type pos) -> size_type;
static inline auto last_index_of_field(const std::string& s, size_type pos) -> size_type;

//  按各种方式遍历
static inline auto walk(const std::string& s, size_type pos, size_type n, std::function<int(const_pointer ptr, size_type n, const_pointer next)> proc) -> void;
static inline auto walk_byte(const std::string& s, size_type pos, size_type n, std::function<int(const_pointer ptr)> proc) -> void;
static inline auto walk_byte(const std::string& s, size_type pos, size_type n, std::function<int(pointer ptr)> proc) -> void;
static inline auto walk_field(const std::string& s, size_type pos, size_type n, std::function<int(const_pointer ptr, size_type n)> proc) -> void;

//  匹配
static inline auto is_match_wild(const std::string& pattern) -> bool;
static inline auto is_match_wild(const_pointer pattern) -> bool;
static inline auto is_match(uint16_t charset) -> bool;

//  字符串特征
static inline auto is_empty(const std::string& s) -> bool {
    return s.empty();
}

static inline auto is_lower(const std::string& s) -> bool {
    for (const_pointer ptr = s.c_str(); ptr < s.c_str() + s.size(); ptr++) {
        if (!std::islower(*ptr)) {
            return false;
        }
    }

    return true;
}

static inline auto is_upper(const std::string& s) -> bool {
    for (const_pointer ptr = s.c_str(); ptr < s.c_str() + s.size(); ptr++) {
        if (!std::isupper(*ptr)) {
            return false;
        }
    }

    return true;
}

static inline auto is_title(const std::string& s) -> bool {
    if (s.empty()) {
        return false;
    }

    return std::isupper(s[0]);
}

static inline auto is_digit(const std::string& s) -> bool {
    for (const_pointer ptr = s.c_str(); ptr < s.c_str() + s.size(); ptr++) {
        if (!std::isdigit(*ptr)) {
            return false;
        }
    }
    return true;
}

static inline auto is_xdigit(const std::string& s) -> bool {
    for (const_pointer ptr = s.c_str(); ptr < s.c_str() + s.size(); ptr++) {
        if (!std::isxdigit(*ptr)) {
            return false;
        }
    }
    return true;
}

static inline auto is_ascii(const std::string& s) -> bool {
    for (const_pointer ptr = s.c_str(); ptr < s.c_str() + s.size(); ptr++) {
        if ((*ptr & 0x80) != 0) {
            return false;
        }
    }
    return true;
}

static inline auto is_alpha(const std::string& s) -> bool {
    for (const_pointer ptr = s.c_str(); ptr < s.c_str() + s.size(); ptr++) {
        if (!std::isalpha(*ptr)) {
            return false;
        }
    }
    return true;
}

static inline auto is_alnum(const std::string& s) -> bool {
    for (const_pointer ptr = s.c_str(); ptr < s.c_str() + s.size(); ptr++) {
        if (!std::isalnum(*ptr)) {
            return false;
        }
    }
    return true;
}

static inline auto is_space(const std::string& s) -> bool {
    for (const_pointer ptr = s.c_str(); ptr < s.c_str() + s.size(); ptr++) {
        if (!std::isspace(*ptr)) {
            return false;
        }
    }
    return true;
}

static inline auto is_blank(const std::string& s) -> bool {
    for (const_pointer ptr = s.c_str(); ptr < s.c_str() + s.size(); ptr++) {
        if (!std::isblank(*ptr)) {
            return false;
        }
    }
    return true;
}

static inline auto is_print(const std::string& s) -> bool {
    for (const_pointer ptr = s.c_str(); ptr < s.c_str() + s.size(); ptr++) {
        if (!std::isprint(*ptr)) {
            return false;
        }
    }
    return true;
}

static inline auto is_graph(const std::string& s) -> bool {
    for (const_pointer ptr = s.c_str(); ptr < s.c_str() + s.size(); ptr++) {
        if (!std::isgraph(*ptr)) {
            return false;
        }
    }
    return true;
}

static inline auto is_identifier(const std::string& s) -> bool {
    if (s.size() == 0) {
        return false;
    }

    if (!std::isalpha(s[0]) && s[0] != '_') {
        return false;
    }

    for (const_pointer ptr = s.c_str() + 1; ptr < s.c_str() + s.size(); ptr++) {
        if (!std::isalnum(*ptr) && (*ptr != '_')) {
            return false;
        }
    }

    return true;
}

static inline auto is_bool(const std::string& s) -> bool {
    if (s.size() <= 0) {
        return false;
    }

    //  "1"     "0"
    //  "on"    "off"
    //  "ON"    "OFF"
    //  "Yes"   "No"
    //  "yes"   "no"
    //  "YES"   "NO"
    //  "True"  "False"
    //  "true"  "false"
    //  "TRUE"  "FALSE"

    const_pointer ptr = s.c_str();
    switch (s.size()) {
        case 5:
            if ((ptr[0] == 'f') && (ptr[1] == 'a') && (ptr[2] == 'l') && (ptr[3] == 's') && (ptr[4] == 'e')) {
                return true;
            }
            if ((ptr[0] == 'F') && (ptr[1] == 'a') && (ptr[2] == 'l') && (ptr[3] == 's') && (ptr[4] == 'e')) {
                return true;
            }
            if ((ptr[0] == 'F') && (ptr[1] == 'A') && (ptr[2] == 'L') && (ptr[3] == 'S') && (ptr[4] == 'E')) {
                return true;
            }
            return false;
        case 4:
            if ((ptr[0] == 't') && (ptr[1] == 'r') && (ptr[2] == 'u') && (ptr[3] == 'e')) {
                return true;
            }
            if ((ptr[0] == 'T') && (ptr[1] == 'r') && (ptr[2] == 'u') && (ptr[3] == 'e')) {
                return true;
            }
            if ((ptr[0] == 'T') && (ptr[1] == 'R') && (ptr[2] == 'U') && (ptr[3] == 'E')) {
                return true;
            }
            return false;
        case 3:
            if ((ptr[0] == 'y') && (ptr[1] == 'e') && (ptr[2] == 's')) {
                return true;
            }
            if ((ptr[0] == 'Y') && (ptr[1] == 'e') && (ptr[2] == 's')) {
                return true;
            }
            if ((ptr[0] == 'Y') && (ptr[1] == 'E') && (ptr[2] == 'S')) {
                return true;
            }
            if ((ptr[0] == 'o') && (ptr[1] == 'f') && (ptr[2] == 'f')) {
                return true;
            }
            if ((ptr[0] == 'O') && (ptr[1] == 'f') && (ptr[2] == 'f')) {
                return true;
            }
            if ((ptr[0] == 'O') && (ptr[1] == 'F') && (ptr[2] == 'F')) {
                return true;
            }
            return false;
        case 2:
            if ((ptr[0] == 'o') && (ptr[1] == 'n')) {
                return true;
            }
            if ((ptr[0] == 'O') && (ptr[1] == 'n')) {
                return true;
            }
            if ((ptr[0] == 'O') && (ptr[1] == 'N')) {
                return true;
            }
            if ((ptr[0] == 'n') && (ptr[1] == 'o')) {
                return true;
            }
            if ((ptr[0] == 'N') && (ptr[1] == 'o')) {
                return true;
            }
            if ((ptr[0] == 'N') && (ptr[1] == 'O')) {
                return true;
            }
            return false;
        case 1:
            switch (*ptr) {
                case '0':
                    return true;
                case '1':
                    return true;
                default:
                    return false;
            }
        default:
            return false;
    }

    return false;
}

//  提取子串
static inline auto left(const std::string& s, size_type n) -> std::string {
    if (n == 0) {
        return "";
    }

    if (n > s.size()) {
        return s;
    }

    return s.substr(0, n);
}

static inline auto right(const std::string& s, size_type n) -> std::string {
    if (n == 0) {
        return "";
    }

    if (n > s.size()) {
        return s;
    }

    return s.substr(s.size() - n, n);
}

static inline auto substr(const std::string& s, size_type pos, ssize_type offset) -> std::string {
    if (offset > 0) {
        size_type n = offset;
        return s.substr(pos, std::min(n, pos + n));
    }

    if (offset < 0) {
        size_type n = -offset;
        return s.substr(pos, std::min(n, pos + n));
    }

    return "";
}

//  定宽对齐调整
static inline auto ljust(std::string& s, size_type width, value_type ch = ' ') -> std::string& {
    if (s.size() >= width) {
        return s;
    }

    size_type old_len = s.size();
    s.resize(width);
    std::memmove(s.data() + width - old_len, s.data(), old_len);
    std::fill(s.data(), s.data() + width - old_len, ch);
    return s;
}

static inline auto ljust(const std::string& s, size_type width, value_type ch = ' ') -> std::string {
    if (s.size() >= width) {
        return s;
    }

    std::string result;
    result.resize(width);
    std::fill(result.data(), result.data() + (width - s.size()), ch);
    std::memcpy(result.data() + (width - s.size()), s.c_str(), s.size());
    return result;
}

static inline auto rjust(std::string& s, size_type width, value_type ch = ' ') -> std::string& {
    if (s.size() >= width) {
        return s;
    }

    s.resize(width, ch);
    return s;
}

static inline auto rjust(const std::string& s, size_type width, value_type ch = ' ') -> std::string {
    if (s.size() >= width) {
        return s;
    }

    std::string result;
    result.reserve(width);
    result.append(s);
    result.append(width - s.size(), ch);
    return result;
}

static inline auto center(std::string& s, size_type width, value_type ch = ' ') -> std::string& {
    if (s.size() < width) {
        return s;
    }

    size_type left_len = (width - s.size()) / 2;
    size_type right_len = (width - left_len) - s.size();

    s.resize(width);
    std::memmove(s.data() + left_len, s.c_str(), s.size());
    std::fill(s.data(), s.data() + left_len, ch);
    std::fill(s.data() + width - right_len, s.data() + width, ch);
    return s;
}

static inline auto center(const std::string& s, size_type width, value_type ch = ' ') -> std::string {
    if (s.size() < width) {
        return s;
    }

    size_type left_len = (width - s.size()) / 2;
    size_type right_len = (width - left_len) - s.size();

    std::string result;
    result.reserve(width);
    result.append(left_len, ch);
    result.append(s);
    result.append(right_len, ch);
    return result;
}

static inline auto zfill(std::string& s, size_type width) -> std::string& {
    if (s.empty()) {
        s.resize(0);
        return s;
    }

    if (s.size() >= width) {
        return s;
    }

    if ((s[0] != '+') && (s[0] != '-')) {
        return ljust(s, width, '0');
    }

    size_type old_len = s.size();
    s.resize(width);
    if (old_len - 1 > 0) {
        std::memmove(s.data() + s.size() - (old_len - 1), s.c_str() + 1, old_len - 1);
    }

    std::fill(s.data() + 1, s.data() + s.size() - (old_len - 1), '0');
    return s;
}

static inline auto zfill(const std::string& s, size_type width) -> std::string {
    if (s.empty()) {
        return std::string(width, '0');
    }

    if (s.size() >= width) {
        return s;
    }

    if ((s[0] != '+') && (s[0] != '-')) {
        return ljust(s, width, '0');
    }

    std::string result;
    result.resize(width);
    pointer ptr = result.data();
    *ptr = s[0];
    ptr++;
    std::fill(ptr, ptr + (width - s.size()), '0');
    ptr += (width - s.size());
    std::memcpy(ptr, s.c_str(), s.size());

    return result;
}

// 字符串生成
static inline auto repeat(const std::string& s, size_type times) -> std::string {
    if (s.empty() || (times == 0)) {
        return "";
    }

    std::string result;
    result.reserve(times * s.size());

    for (size_type i = 0; i < times; i++) {
        result.append(s);
    }

    return result;
}

static inline auto space(size_type width) -> std::string {
    return repeat(" ", width);
}

//  基于本字符串生成新字符串

using join_proc = std::function<std::optional<std::string_view>()>;
static inline auto join(const std::string& s, join_proc proc) -> std::string {
    std::string result;
    for (auto item = proc(); item; item = proc()) {
        if (!result.empty()) {
            result.append(s);
        }
        result.append(item.value());
    }
    return result;
}

static inline auto join(const std::string& s, const std::vector<std::string>& items) -> std::string {
    auto itr = items.cbegin();
    return join(s, [&items, &itr]() -> std::optional<std::string_view> {
        if (itr == items.cend()) {
            return {};
        }
        const std::string& item = *itr;
        itr++;
        return std::string_view(item.c_str(), item.size());
    });
}

static inline auto join(const std::string& s, std::initializer_list<std::string> items) -> std::string {
    auto itr = items.begin();
    return join(s, [&items, &itr]() -> std::optional<std::string_view> {
        if (itr == items.end()) {
            return {};
        }
        const std::string& item = *itr;
        itr++;
        return std::string_view(item.c_str(), item.size());
    });
}

static inline auto join(const std::string& s, std::initializer_list<const_pointer> items) -> std::string {
    auto itr = items.begin();
    return join(s, [&items, &itr]() -> std::optional<std::string_view> {
        if (itr == items.end()) {
            return {};
        }
        const const_pointer& item = *itr;
        itr++;
        return std::string_view(item);
    });
}

static inline auto join(const std::string& s, std::initializer_list<std::string_view> items) -> std::string {
    auto itr = items.begin();
    return join(s, [&items, &itr]() -> std::optional<std::string_view> {
        if (itr == items.end()) {
            return {};
        }
        const std::string_view& item = *itr;
        itr++;
        return item;
    });
}

// 使用逗号和冒号拼接 map
using join_map_proc = std::function<int(const_pointer key, size_type key_n, const_pointer val, size_type val_n)>;
static inline auto join_map(join_map_proc proc) -> std::string;
static inline auto join_map(std::map<std::string, std::string> items) -> std::string;

// 路径拼接
using join_path_proc = std::function<std::optional<std::string_view>()>;
static inline auto join_path(join_path_proc proc) -> std::string {
    std::string result;
    for (auto item = proc(); item; item = proc()) {
        if (!result.empty()) {
            result.append("/");
        }
        result.append(item.value());
    }
    return result;
}

static inline auto join_path(const std::vector<std::string>& items) -> std::string {
    if (items.empty()) {
        return "";
    }

    auto itr = items.cbegin();
    return join_path([&itr, &items]() -> std::optional<std::string_view> {
        if (itr == items.cend()) {
            return {};
        }

        return std::string_view{ itr->c_str(), itr->size() };
    });
}

static inline auto join_path(std::initializer_list<std::string> items) -> std::string {
    auto itr = items.begin();
    return join_path([&itr, &items]() -> std::optional<std::string_view> {
        if (itr == items.end()) {
            return {};
        }

        return std::string_view{ itr->c_str(), itr->size() };
    });
}

static inline auto join_path(std::initializer_list<const_pointer> items) -> std::string {
    auto itr = items.begin();
    return join_path([&itr, &items]() -> std::optional<std::string_view> {
        if (itr == items.end()) {
            return {};
        }

        return std::string_view{ *itr };
    });
}

static inline auto join_path(std::initializer_list<std::string_view> items) -> std::string {
    auto itr = items.begin();
    return join_path([&itr, &items]() -> std::optional<std::string_view> {
        if (itr == items.end()) {
            return {};
        }

        return *itr;
    });
}

// 拼接成搜索路径
using join_search_path_proc = join_path_proc;
static inline auto join_search_path(join_search_path_proc proc) -> std::string {
    return join(":", proc);
}

static inline auto join_search_path(const std::vector<std::string>& items) -> std::string {
    return join(":", items);
}

static inline auto join_search_path(std::initializer_list<std::string> items) -> std::string {
    return join(":", items);
}

static inline auto join_search_path(std::initializer_list<const_pointer> items) -> std::string {
    return join(":", items);
}

static inline auto join_search_path(std::initializer_list<std::string_view> items) -> std::string {
    return join(":", items);
}

//  Title 化：首字母大写
static inline auto title(std::string& s) -> std::string& {
    if (s.empty()) {
        return s;
    }
}

static inline auto title(const std::string& s) -> const std::string&;
static inline auto title_fields(std::string& s) -> std::string&;
static inline auto title_fields(const std::string& s) -> const std::string&;

//  反转：字符串逆序
static inline auto invert(std::string& s, size_type pos, size_type max_n = npos) -> std::string&;
static inline auto invert(const std::string& s, size_type pos, size_type max_n = npos) -> std::string;

// 拆分字符串
using split_list_proc = std::function<int(std::string_view item)>;
static inline auto split_list(const std::string& s, const std::string_view& sep, split_list_proc proc) -> void {
    size_type pos_start = 0;
    while (pos_start < s.size()) {
        size_type pos_end = s.find(sep, pos_start);
        if (pos_end == std::string::npos) {
            break;
        }

        if (proc(std::string_view{ s.c_str() + pos_start, pos_end - pos_start }) != 0) {
            pos_start = pos_end + sep.size();
            break;
        }
        pos_start = pos_end + sep.size();
    }

    proc(std::string_view{ s.c_str() + pos_start, s.size() - pos_start });
}

static inline auto split_list(const std::string& s, const std::string& sep, split_list_proc proc) -> void {
    return split_list(s, std::string_view{ sep.c_str(), sep.size() }, proc);
}

static inline auto split_list(const std::string& s, value_type sep, split_list_proc proc) -> void {
    return split_list(s, std::string_view{ &sep, 1 }, proc);
}

static inline auto split_list(const std::string& s, const std::string_view& sep) -> std::vector<std::string> {
    std::vector<std::string> result;
    split_list(s, sep, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });
    return result;
}

static inline auto split_list(const std::string& s, value_type sep) -> std::vector<std::string> {
    return split_list(s, std::string_view{ &sep, 1 });
}

static inline auto split_list(const std::string& s, const std::string& sep) -> std::vector<std::string> {
    return split_list(s, std::string_view{ sep });
}

// 将字符串 s，按照逗号和冒号拆分成一个 map 对象
using split_map_proc = std::function<int(const std::string_view& key, const std::string_view& value)>;
static inline auto split_map(const std::string& s, split_map_proc proc) -> void;
static inline auto split_map(const std::string& s) -> std::map<std::string, std::string>;

// 按照换行符将字符串 s，拆分长多行
using split_lines_proc = std::function<int(const std::string_view& line)>;
static inline auto split_lines(const std::string& s, bool keepends, split_lines_proc proc) -> void;

// 将字符串 s 视作目录，按照路径分隔符，拆分成多个组成部分
using split_path_proc = std::function<int(const std::string_view& elem)>;
static inline auto split_path(const std::string& s, split_path_proc proc) -> void;

// 将 s 视作路径，拆分出该路径的驱动字符串（仅win下有效）
static inline auto split_drive(const std::string& s) -> std::string;

//  大小写转换
static inline auto to_lower(std::string& s) -> std::string&;
static inline auto to_lower(const std::string& s) -> std::string;
static inline auto to_upper(std::string& s) -> std::string&;
static inline auto to_upper(const std::string& s) -> std::string;
static inline auto swap_case(std::string& s, std::string& other) -> void;

// 字符映射
using translate_proc = std::function<value_type(value_type)>;
static inline auto translate(std::string& s, translate_proc proc) -> std::string& {
    pointer ptr = s.data();
    while (*ptr) {
        *ptr = proc(*ptr);
        ptr++;
    }
    return s;
}

// 字符串化简，将字符串中的多个空白压缩成一个空格
using simplified_proc = std::function<bool(value_type ch)>;
static inline auto simplified(std::string& s, simplified_proc proc) -> std::string& {
    if (s.size() == 0) {
        return s;
    }

    bool found = false;
    pointer w = s.data();
    pointer r = s.data();
    while (*r != '\0') {
        value_type ch = *r;
        if (found) {
            if (proc(ch)) {
                r++;
                continue;
            }

            found = false;
            *(w++) = *(r++);
            continue;
        }

        if (proc(ch)) {
            found = true;
            *(w++) = ' ';
            r++;
            continue;
        }

        *(w++) = *(r++);
    }

    s.resize(w - s.data());
    return s;
}

static inline auto simplified(const std::string& s) -> std::string {
    if (s.size() == 0) {
        return s;
    }

    std::string result;
    bool found = true;
    const_pointer r = s.c_str();
    while (*r != '\0') {
        value_type ch = *r;
        if (found) {
            if (std::isspace(ch)) {
                r++;
                continue;
            }

            found = false;
            result.append(r, 1);
            r++;
            continue;
        }

        if (std::isspace(ch)) {
            found = true;
        }

        result.append(r, 1);
        r++;
    }

    if (!result.empty()) {
        if (std::isspace(result.back())) {
            result.resize(result.size() - 1);
        }
    }

    return result;
}

static inline auto simplified(std::string& s) -> std::string& {
    if (s.size() == 0) {
        return s;
    }

    bool found = true;
    pointer w = s.data();
    const_pointer r = s.c_str();
    while (*r != '\0') {
        if (found) {
            if (std::isspace(*r)) {
                r++;
                continue;
            }

            found = false;
            *(w++) = *(r++);
            continue;
        }

        if (std::isspace(*r)) {
            found = true;
            *(w++) = ' ';
            r++;
            continue;
        }

        *(w++) = *(r++);
    }

    s.resize(w - s.c_str());

    if (!s.empty()) {
        if (std::isspace(s.back())) {
            s.resize(s.size() - 1);
        }
    }

    return s;
}

// 空白祛除
using trim_proc = std::function<bool(value_type ch)>;
static inline auto ltrim(const std::string& s, trim_proc proc) -> std::string;
static inline auto ltrim(const std::string& s) -> std::string;
static inline auto ltrim(std::string& s) -> std::string&;
static inline auto rtrim(const std::string& s, trim_proc proc) -> std::string;
static inline auto rtrim(const std::string& s) -> std::string;
static inline auto rtrim(std::string& s) -> std::string&;
static inline auto trim(const std::string& s, trim_proc proc) -> std::string;
static inline auto trim(const std::string& s) -> std::string;
static inline auto trim(std::string& s) -> std::string&;
static inline auto trim_all(const std::string& s, trim_proc proc) -> std::string;
static inline auto trim_all(const std::string& s) -> std::string;
static inline auto trim_all(std::string& s) -> std::string&;

// 切除
static inline auto chop(std::string& s, size_type n) -> std::string& {
    if (n > s.size()) {
        s.resize(0);
        return s;
    }

    s.resize(s.size() - n);
    return s;
}

static inline auto choped(const std::string& s, size_type n) -> std::string {
    if (n > s.size()) {
        return "";
    }

    return s.substr(0, s.size() - n);
}

// 变量展开
using expand_vars_proc = std::function<std::optional<std::string>(const std::string& key)>;
static inline auto expand_envs(const std::string& s, expand_vars_proc proc) -> std::string;
static inline auto expand_envs(const std::string& s, const std::map<std::string, std::string>& kvs) -> std::string;
static inline auto expand_envs(const std::string& s, const std::tuple<const std::string&, const std::string&>& pair) -> std::string;
static inline auto expand_envs(const std::string& s, const std::tuple<const std::string_view, const std::string_view>& pair) -> std::string;
static inline auto expand_envs(const std::string& s) -> std::string;

// 将字符串中的 tab 符号(\t)按照 tab 宽度替换成空白
static inline auto expand_tabs(const std::string& s, size_type tab_size = 8) -> std::string;

// 扩展字符串中的 ~ 前缀
static inline auto expand_user(const std::string& s) -> std::string;

// 路径正常化
static inline auto normpath(const std::string& s) -> std::string;

//  拷贝和交换
static inline auto copy(pointer dest, size_type max_n, const std::string& s) -> size_type;

// 将 s 视作为文件路径，获取其目录名
static inline auto dirname(const std::string& s) -> std::string {
    if (s.empty()) {
        return ".";
    }

    // 如果尾部为斜杠，应该跳过所有的斜杠
    size_type pos = s.size();
    while (pos > 0) {
        if (s[pos - 1] != '/') {
            break;
        }
        pos--;
    }

    // 继续找最近的一个斜杠
    while (pos > 0) {
        if (s[pos - 1] == '/') {
            break;
        }
        pos--;
    }

    // 路径可能并不标准，可能会连续输入多个斜杠，此时将所有的斜杠吸收掉
    while (pos > 0) {
        if (s[pos - 1] != '/') {
            break;
        }
        pos--;
    }

    if (pos == 0) {
        return (s[pos] == '/') ? "/" : ".";
    }

    return s.substr(0, pos);
}

//  处理路径中文件名的部分
static inline auto basename_ptr(const std::string& s) -> const_pointer {
    size_type pos = s.find_last_of('/');
    if (pos == std::string::npos) {
        return s.c_str();
    }

    return s.c_str() + pos + 1;
}

static inline auto basename_ptr(std::string& s) -> pointer {
    size_type pos = s.find_last_of('/');
    if (pos == std::string::npos) {
        return s.data();
    }

    return s.data() + pos + 1;
}

static inline auto basename(const std::string& s) -> std::string {
    return basename_ptr(s);
}

static inline auto remove_basename(const std::string& s) -> std::string {
    const_pointer ptr = basename_ptr(s);
    return s.substr(0, ptr - s.c_str());
}

static inline auto remove_basename(std::string& s) -> std::string& {
    const_pointer ptr = basename_ptr(s);
    s.resize(ptr - s.c_str());
    return s;
}

static inline auto replace_basename(std::string& s, const std::string& name) -> std::string& {
    pointer ptr = basename_ptr(s);
    s.resize((ptr - s.c_str()) + name.size());
    std::memcpy(ptr, name.c_str(), name.size());
    return s;
}

static inline auto replace_basename(std::string& s, const std::string_view& name) -> std::string& {
    pointer ptr = basename_ptr(s);
    s.resize((ptr - s.c_str()) + name.size());
    std::memcpy(ptr, name.data(), name.size());
    return s;
}

static inline auto replace_basename(const std::string& s, const std::string& name) -> std::string {
    const_pointer ptr = basename_ptr(s);
    std::string result;
    result.reserve((ptr - s.c_str()) + name.size());
    result.append(s.c_str(), ptr - s.c_str());
    result.append(name);
    return result;
}

static inline auto replace_basename(const std::string& s, const std::string_view& name) -> std::string {
    const_pointer ptr = basename_ptr(s);
    std::string result;
    result.reserve((ptr - s.c_str()) + name.size());
    result.append(s.c_str(), ptr - s.c_str());
    result.append(name);
    return result;
}

// 扩展名相关操作
static inline auto extname_ptr(const std::string& s) -> const_pointer {
    const_pointer ptr = basename_ptr(s);
    if (ptr[0] == '.') {
        return s.c_str() + s.size();
    }

    ptr = std::strchr(ptr, '.');
    if (ptr == nullptr) {
        return s.c_str() + s.size();
    }

    return ptr;
}

static inline auto extname_ptr(std::string& s) -> pointer {
    pointer ptr = basename_ptr(s);
    if (ptr[0] == '.') {
        return s.data() + s.size();
    }

    ptr = std::strchr(ptr, '.');
    if (ptr == nullptr) {
        return s.data() + s.size();
    }

    return ptr;
}

static inline auto extname(const std::string& s) -> std::string {
    return extname_ptr(s);
}

static inline auto remove_extname(const std::string& s) -> std::string {
    const_pointer ptr = extname_ptr(s);
    return s.substr(0, ptr - s.c_str());
}

static inline auto remove_extname(std::string& s) -> std::string& {
    const_pointer ptr = extname_ptr(s);
    s.resize(ptr - s.c_str());
    return s;
}

static inline auto replace_extname(std::string& s, const std::string& name) -> std::string& {
    pointer ptr = extname_ptr(s);
    s.resize((ptr - s.c_str()) + name.size());
    std::memcpy(ptr, name.c_str(), name.size());
    return s;
}

static inline auto replace_extname(std::string& s, const std::string_view& name) -> std::string& {
    pointer ptr = extname_ptr(s);
    s.resize((ptr - s.c_str()) + name.size());
    std::memcpy(ptr, name.data(), name.size());
    return s;
}

static inline auto replace_extname(const std::string& s, const std::string& name) -> std::string {
    const_pointer ptr = basename_ptr(s);
    std::string result;
    result.reserve((ptr - s.c_str()) + name.size());
    result.append(s.c_str(), ptr - s.c_str());
    result.append(name);
    return result;
}

static inline auto replace_extname(const std::string& s, const std::string_view& name) -> std::string {
    const_pointer ptr = basename_ptr(s);
    std::string result;
    result.reserve((ptr - s.c_str()) + name.size());
    result.append(s.c_str(), ptr - s.c_str());
    result.append(name);
    return result;
}

// 最后一截扩展名相关操作
static inline auto last_extname_ptr(const std::string& s) -> const_pointer {
}

static inline auto last_extname(const std::string& s) -> std::string;
static inline auto remove_last_extname(const std::string& s) -> std::string;
static inline auto remove_last_extname(std::string& s) -> std::string&;
static inline auto replace_last_extname(std::string& s, const std::string& name) -> std::string&;
static inline auto replace_last_extname(std::string& s, const std::string_view& name) -> std::string&;
static inline auto replace_last_extname(const std::string& s, const std::string& name) -> std::string;
static inline auto replace_last_extname(const std::string& s, const std::string_view& name) -> std::string;

//  转换为 hash 值
static inline auto hash(const std::string& s, uint32_t mod) -> uint32_t;
static inline auto hash(const std::string& s, uint64_t mod) -> uint64_t;

template <typename T>
static inline auto to(const std::string& s, std::tuple<int> base) -> std::optional<T> {
    return {};
}

template <typename T>
static inline auto to(const std::string& s) -> std::optional<T> {
    return {};
}

template <typename T>
static inline auto to(const std::string& s, T def, std::tuple<int> base) -> T {
    auto result = to<T>(s, base);
    return result ? result.value() : def;
}

template <typename T>
static inline auto to(const std::string& s, T def) -> T {
    auto result = to<T>(s);
    return result ? result.value() : def;
}

template <>
inline auto to<bool>(const std::string& s [[maybe_unused]]) -> std::optional<bool> {
    return {};
}

template <>
inline auto to<float>(const std::string& s) -> std::optional<float> {
    errno = 0;
    char* endptr = nullptr;
    auto result = std::strtof(s.c_str(), &endptr);
    static_assert(sizeof(result) >= sizeof(float));
    if (result <= std::numeric_limits<float>::epsilon()) {
        if (endptr == s.c_str()) {
            return {};
        }
    }

    if (errno == ERANGE) {
        return {};
    }

    return result;
}

template <>
inline auto to<double>(const std::string& s) -> std::optional<double> {
    errno = 0;
    char* endptr = nullptr;
    auto result = std::strtod(s.c_str(), &endptr);
    static_assert(sizeof(result) >= sizeof(double));
    if (result <= std::numeric_limits<double>::epsilon()) {
        if (endptr == s.c_str()) {
            return {};
        }
    }

    if (errno == ERANGE) {
        return {};
    }

    return result;
}

static inline constexpr auto correct_base(int base) -> int {
    if (base != 0) {
        if (base < 2) {
            return 2;
        }
        if (base > 36) {
            return 36;
        }
    }

    return base;
}

template <>
inline auto to<int8_t>(const std::string& s, std::tuple<int> base) -> std::optional<int8_t> {
    int nbase = correct_base(std::get<0>(base));
    errno = 0;
    char* endptr = nullptr;
    auto result = std::strtol(s.c_str(), &endptr, nbase);
    static_assert(sizeof(result) >= sizeof(int8_t));
    if (result == 0) {
        if (endptr == s.c_str()) {
            return {};
        }
    }

    if (errno == ERANGE) {
        return {};
    }

    if ((result > std::numeric_limits<int8_t>::max()) || (result < std::numeric_limits<int8_t>::min())) {
        return {};
    }

    return result;
}

template <>
inline auto to<int16_t>(const std::string& s, std::tuple<int> base) -> std::optional<int16_t> {
    int nbase = correct_base(std::get<0>(base));

    errno = 0;
    char* endptr = nullptr;
    auto result = std::strtol(s.c_str(), &endptr, nbase);
    static_assert(sizeof(result) >= sizeof(int16_t));
    if (result == 0) {
        if (endptr == s.c_str()) {
            return {};
        }
    }

    if (errno == ERANGE) {
        return {};
    }

    if ((result > std::numeric_limits<int16_t>::max()) || (result < std::numeric_limits<int16_t>::min())) {
        return {};
    }

    return result;
}

template <>
inline auto to<int32_t>(const std::string& s, std::tuple<int> base) -> std::optional<int32_t> {
    int nbase = correct_base(std::get<0>(base));

    errno = 0;
    char* endptr = nullptr;
    auto result = std::strtol(s.c_str(), &endptr, nbase);
    static_assert(sizeof(result) >= sizeof(int32_t));
    if (result == 0) {
        if (endptr == s.c_str()) {
            return {};
        }
    }

    if (errno == ERANGE) {
        return {};
    }

    if ((result > std::numeric_limits<int32_t>::max()) || (result < std::numeric_limits<int32_t>::min())) {
        return {};
    }

    return result;
}

template <>
inline auto to<int64_t>(const std::string& s, std::tuple<int> base) -> std::optional<int64_t> {
    int nbase = correct_base(std::get<0>(base));

    errno = 0;
    char* endptr = nullptr;
#if defined __LP64__
    auto result = std::strtol(s.c_str(), &endptr, nbase);
#else
    auto result = std::strtoll(s.c_str(), &endptr, nbase);
#endif
    static_assert(sizeof(result) >= sizeof(int64_t));
    if (result == 0) {
        if (endptr == s.c_str()) {
            return {};
        }
    }

    if (errno == ERANGE) {
        return {};
    }

    if ((result > std::numeric_limits<int64_t>::max()) || (result < std::numeric_limits<int64_t>::min())) {
        return {};
    }

    return result;
}

template <>
inline auto to<uint8_t>(const std::string& s, std::tuple<int> base) -> std::optional<uint8_t> {
    int nbase = correct_base(std::get<0>(base));

    errno = 0;
    char* endptr = nullptr;
    auto result = std::strtoul(s.c_str(), &endptr, nbase);
    static_assert(sizeof(result) >= sizeof(uint8_t));
    if (result == 0) {
        if (endptr == s.c_str()) {
            return {};
        }
    }

    if (errno == ERANGE) {
        return {};
    }

    if ((result > std::numeric_limits<uint8_t>::max())) {
        return {};
    }

    return result;
}

template <>
inline auto to<uint16_t>(const std::string& s, std::tuple<int> base) -> std::optional<uint16_t> {
    int nbase = correct_base(std::get<0>(base));

    errno = 0;
    char* endptr = nullptr;
    auto result = std::strtoul(s.c_str(), &endptr, nbase);
    static_assert(sizeof(result) >= sizeof(uint16_t));
    if (result == 0) {
        if (endptr == s.c_str()) {
            return {};
        }
    }

    if (errno == ERANGE) {
        return {};
    }

    if ((result > std::numeric_limits<uint16_t>::max())) {
        return {};
    }

    return result;
}

template <>
inline auto to<uint32_t>(const std::string& s, std::tuple<int> base) -> std::optional<uint32_t> {
    int nbase = correct_base(std::get<0>(base));

    errno = 0;
    char* endptr = nullptr;
    auto result = std::strtoul(s.c_str(), &endptr, nbase);
    static_assert(sizeof(result) >= sizeof(uint32_t));
    if (result == 0) {
        if (endptr == s.c_str()) {
            return {};
        }
    }

    if (errno == ERANGE) {
        return {};
    }

    if ((result > std::numeric_limits<uint32_t>::max())) {
        return {};
    }

    return result;
}

template <>
inline auto to<uint64_t>(const std::string& s, std::tuple<int> base) -> std::optional<uint64_t> {
    int nbase = correct_base(std::get<0>(base));

    errno = 0;
    char* endptr = nullptr;
#if defined __LP64__
    auto result = std::strtoul(s.c_str(), &endptr, nbase);
#else
    auto result = std::strtoull(s.c_str(), &endptr, nbase);
#endif
    static_assert(sizeof(result) >= sizeof(int64_t));
    if (result == 0) {
        if (endptr == s.c_str()) {
            return {};
        }
    }

    if (errno == ERANGE) {
        return {};
    }

    return result;
}

static inline auto from(double n, value_type format = 'g', int precision = 6) -> std::string;
static inline auto from(float n, value_type format = 'g', int precision = 6) -> std::string;
static inline auto from(int8_t n, int base = 10) -> std::string;
inline auto from(int16_t n, int base = 10) -> std::string;
inline auto from(int32_t n, int base = 10) -> std::string;
inline auto from(int64_t n, int base = 10) -> std::string;
inline auto from(uint8_t n, int base = 10) -> std::string;
inline auto from(uint16_t n, int base = 10) -> std::string;
inline auto from(uint32_t n, int base = 10) -> std::string;
inline auto from(uint64_t n, int base = 10) -> std::string;

};     // namespace str

#endif // TINY_STR_H
