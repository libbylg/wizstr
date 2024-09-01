//
// Created by libbylg on 2023/6/1.
//
#include "view.h"

#include <cassert>

auto view::append(std::string_view s, std::string_view other) -> std::string {
    std::string result;
    result.reserve(s.size() + other.size());
    result.append(s);
    result.append(other);
    return result;
}

auto view::append(std::string_view s, std::string_view other, size_type n) -> std::string {
    std::string result;
    result.reserve(s.size() + other.size() * n);
    result.append(s);
    for (size_type index = 0; index < n; index++) {
        result.append(other);
    }
    return result;
}

auto view::append(std::string_view s, value_type ch) -> std::string {
    std::string result;
    result.reserve(s.size() + 1);
    result.append(s);
    result.push_back(ch);
    return result;
}

auto view::append(std::string_view s, value_type ch, size_type n) -> std::string {
    std::string result;
    result.reserve(s.size() + n);
    result.append(s);
    result.resize(s.size() + n, ch);
    return result;
}

auto view::append(std::string_view s, const view_provider_proc& proc) -> std::string {
    std::string result{s};
    const auto item = proc();
    while (item) {
        result.append(item.value());
    }

    return result;
}

auto view::prepend(std::string_view s, std::string_view other) -> std::string {
    std::string result;
    result.reserve(s.size() + other.size());
    result.append(other);
    result.append(s);
    return result;
}

auto view::prepend(std::string_view s, std::string_view other, size_type n) -> std::string {
    std::string result;
    result.reserve(s.size() + other.size() * n);
    for (size_type index = 0; index < n; index++) {
        result.append(other);
    }
    result.append(s);
    return result;
}

auto view::prepend(std::string_view s, value_type ch) -> std::string {
    return prepend(s, std::string_view{&ch, 1});
}

auto view::prepend(std::string_view s, value_type ch, size_type n) -> std::string {
    std::string result;
    result.reserve(s.size() + n);
    result.resize(n, ch);
    result.append(s);
    return result;
}

auto view::prepend(std::string_view s, const view_provider_proc& proc) -> std::string {
    std::string result;
    const auto item = proc();
    while (item) {
        result.append(item.value());
    }
    result.append(s);
    return result;
}

auto view::insert(std::string_view s, size_type pos, std::string_view other) -> std::string {
    std::string result;
    result.reserve(other.size() + s.size());
    result.append(std::string_view{s.data(), pos});
    result.append(other);
    result.append(std::string_view{s.data() + pos, s.size() - pos});
    return result;
}

auto view::insert(std::string_view s, size_type pos, std::string_view other, size_type n) -> std::string {
    size_type len = other.size() * n + s.size();
    std::string result;
    result.reserve(len);
    result.append(std::string_view{s.data(), pos});
    for (size_type index = 0; index < n; index++) {
        result.append(other);
    }
    result.resize(len);
    result.append(std::string_view{s.data() + pos, s.size() - pos});
    return result;
}

auto view::insert(std::string_view s, size_type pos, value_type ch) -> std::string {
    return insert(s, pos, std::string_view{&ch, 1});
}

auto view::insert(std::string_view s, size_type pos, value_type ch, size_type n) -> std::string {
    std::string result;
    result.reserve(s.size() + n);
    result.append(s.data(), pos);
    result.append(n, ch);
    result.append(s.data() + pos, s.size() - pos);
    return result;
}

auto view::insert(std::string_view s, size_type pos, const view_provider_proc& proc) -> std::string {
    std::string result;
    result.append(s.data(), pos);

    auto item = proc();
    while (item) {
        result.append(item.value());
    }

    result.append(std::string_view{s.data() + pos, s.size() - pos});
    return result;
}

auto view::icompare(std::string_view s, std::string_view other) -> int {
    if (s.size() < other.size()) {
        int ret = strncasecmp(s.data(), other.data(), s.size());
        return (ret == 0)?other[s.size()]:ret;
    }

    if (s.size() > other.size()) {
        int ret = strncasecmp(s.data(), other.data(), other.size());
        return (ret == 0)?s[other.size()]:ret;
    }

    return strncasecmp(s.data(), other.data(), other.size());
}

auto view::icompare(std::string_view s, std::string_view other, size_type max_n) -> int {
    if (max_n < s.size()) {
        s = s.substr(0, max_n);
    }

    if (max_n < other.size()) {
        other = other.substr(0, max_n);
    }

    return view::icompare(s, other);
}

auto view::iequals(std::string_view s, std::string_view other) -> bool {
    if (s.size() != other.size()) {
        return false;
    }

    return strncasecmp(s.data(), other.data(), s.size()) == 0;
}

auto view::iequals(std::string_view s, std::string_view other, size_type max_n) -> bool {
    if (max_n < s.size()) {
        s = s.substr(0, max_n);
    }

    if (max_n < other.size()) {
        other = other.substr(0, max_n);
    }

    return view::iequals(s, other);
}

auto view::contains(std::string_view s, std::string_view other) -> bool {
    return s.find(other) != std::string_view::npos;
}

auto view::contains(std::string_view s, value_type ch) -> bool {
    return s.find(ch) != std::string_view::npos;
}

auto view::count(std::string_view s, std::string_view other) -> view::size_type {
    if (other.empty()) {
        return s.size() + 1;
    }

    size_type count = 0;

    size_type pos = 0;
    while (true) {
        pos = s.find(other, pos);
        if (pos == std::string_view::npos) {
            break;
        }

        count++;
        pos += other.size();
    }

    return count;
}

auto view::count(std::string_view s, value_type ch) -> view::size_type {
    size_type count = 0;
    for (auto itr = s.begin(); itr != s.end(); itr++) {
        count += ((*itr == ch) ? 1 : 0);
    }
    return count;
}

auto view::count(std::string_view s, char_match_proc proc) -> view::size_type {
    size_type count = 0;
    for (auto itr = s.begin(); itr != s.end(); itr++) {
        auto result = proc(*itr);
        if (!result) {
            break;
        }
        count += (result.value() ? 1 : 0);
    }
    return count;
}

auto view::prefix(std::string_view s, std::string_view other) -> view::size_type {
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

auto view::suffix(std::string_view s, std::string_view other) -> size_type {
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

auto view::has_prefix(std::string_view s, value_type ch) -> bool {
    if (s.empty()) {
        return false;
    }

    return s[0] == ch;
}

auto view::has_prefix(std::string_view s, std::string_view prefix) -> bool {
    return view::prefix(s, prefix) == prefix.size();
}

auto view::starts_with(std::string_view s, value_type ch) -> bool {
    return has_prefix(s, ch);
}

auto view::starts_with(std::string_view s, std::string_view prefix) -> bool {
    return has_prefix(s, prefix);
}

auto view::remove_prefix(std::string_view s, std::string_view prefix) -> std::string_view {
    if (!has_prefix(s, prefix)) {
        return s;
    }

    return std::string_view{s.data() + prefix.size(), s.size() - prefix.size()};
}

auto view::remove_prefix(std::string_view s, value_type prefix) -> std::string_view {
    return remove_prefix(s, {&prefix, 1});
}

auto view::remove_prefix(std::string_view s, size_type n) -> std::string_view {
    if (n >= s.size()) {
        return {};
    }

    return std::string_view{s.data() + n, s.size() - n};
}

auto view::has_suffix(std::string_view s, value_type suffix) -> bool {
    if (s.empty()) {
        return false;
    }

    return s.back() == suffix;
}

auto view::has_suffix(std::string_view s, std::string_view suffix) -> bool {
    return view::suffix(s, suffix) == suffix.size();
}

auto view::ends_with(std::string_view s, value_type suffix) -> bool {
    return has_suffix(s, suffix);
}

auto view::ends_with(std::string_view s, std::string_view suffix) -> bool {
    return has_suffix(s, suffix);
}

auto view::remove_suffix(std::string_view s, std::string_view suffix) -> std::string_view {
    if (!has_prefix(s, suffix)) {
        return s;
    }

    return std::string_view{s.data(), s.size() - suffix.size()};
}

auto view::remove_suffix(std::string_view s, value_type suffix) -> std::string_view {
    return remove_suffix(s, {&suffix, 1});
}

auto view::remove_suffix(std::string_view s, size_type n) -> std::string_view {
    if (n >= s.size()) {
        return {};
    }

    return std::string_view{s.data(), s.size() - n};
}

// auto view::next_char(std::string_view s, size_type pos, value_type ch) -> size_type {
// }

// auto view::prev_char(std::string_view s, value_type ch) -> size_type {
// }

// auto view::next_string(std::string_view s, size_type pos, std::string_view other) -> size_type {
// }

// auto view::prev_string(std::string_view s, std::string_view other) -> size_type {
// }

// auto view::next_eol(std::string_view s, size_type pos) -> size_type {
// }

// auto view::prev_eol(std::string_view& s, size_type pos) -> size_type {
// }

// auto view::next_word(std::string_view s, size_type pos) -> std::string_view {
// }

// auto view::prev_word(std::string_view s, size_type pos) -> std::string_view {
// }

// auto view::walk(std::string_view s, size_type pos, size_type n, std::function<int(const_pointer ptr, size_type n, const_pointer next)> proc) -> void {
// }

// auto view::walk_byte(std::string_view s, size_type pos, size_type n, std::function<int(const_pointer ptr)> proc) -> void {
// }

// auto view::walk_byte(std::string_view s, size_type pos, size_type n, std::function<int(pointer ptr)> proc) -> void {
// }

// auto view::walk_word(std::string_view s, size_type pos, size_type n, std::function<int(const_pointer ptr, size_type n)> proc) -> void {
// }

// auto view::is_match_wild(std::string_view s, std::string_view pattern) -> bool {
// }

// auto view::is_match_charset(std::string_view s, charset_type set) -> bool {
// }

// auto view::is_match_regex(std::string_view s, std::string_view pattern) -> bool {
// }

// auto view::is_match_regex(std::string_view s, const std::regex& pattern) -> bool {
// }

auto view::is_lower(std::string_view s) -> bool {
    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::islower(*ptr)) {
            return false;
        }
    }

    return true;
}

auto view::is_upper(std::string_view s) -> bool {
    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isupper(*ptr)) {
            return false;
        }
    }

    return true;
}

auto view::is_title(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    return std::isupper(s[0]);
}

auto view::is_digit(std::string_view s) -> bool {
    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isdigit(*ptr)) {
            return false;
        }
    }
    return true;
}

auto view::is_xdigit(std::string_view s) -> bool {
    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isxdigit(*ptr)) {
            return false;
        }
    }
    return true;
}

auto view::is_ascii(std::string_view s) -> bool {
    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if ((*ptr & 0x80) != 0) {
            return false;
        }
    }
    return true;
}

auto view::is_alpha(std::string_view s) -> bool {
    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isalpha(*ptr)) {
            return false;
        }
    }
    return true;
}

auto view::is_alnum(std::string_view s) -> bool {
    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isalnum(*ptr)) {
            return false;
        }
    }
    return true;
}

auto view::is_space(std::string_view s) -> bool {
    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isspace(*ptr)) {
            return false;
        }
    }
    return true;
}

auto view::is_blank(std::string_view s) -> bool {
    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isblank(*ptr)) {
            return false;
        }
    }
    return true;
}

auto view::is_print(std::string_view s) -> bool {
    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isprint(*ptr)) {
            return false;
        }
    }
    return true;
}

auto view::is_graph(std::string_view s) -> bool {
    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isgraph(*ptr)) {
            return false;
        }
    }
    return true;
}

auto view::is_identifier(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    if (!std::isalpha(s[0]) && s[0] != '_') {
        return false;
    }

    for (const_pointer ptr = s.data() + 1; ptr < s.data() + s.size(); ptr++) {
        if (!std::isalnum(*ptr) && (*ptr != '_')) {
            return false;
        }
    }

    return true;
}

auto view::is_literal_bool(std::string_view s) -> bool {
    if (s.empty()) {
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

    const_pointer ptr = s.data();
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
                    return false;
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

// auto view::is_literal_true(std::string_view s) -> bool {
// }

// auto view::is_literal_false(std::string_view s) -> bool {
// }

// auto view::is_literal_real(std::string_view s) -> bool {
// }

// auto view::is_literal_integer(std::string_view s) -> bool {
// }

///////////////////11111111111111111111

// auto view::left_n(std::string_view s, size_type n) -> std::string {
//     if (n == 0) {
//         return "";
//     }

//     if (n > s.size()) {
//         return std::string{s};
//     }

//     return std::string{s.substr(0, n)};
// }

// auto view::right_n(std::string_view s, size_type n) -> std::string {
//     if (n == 0) {
//         return {};
//     }

//     if (n > s.size()) {
//         return std::string{s};
//     }

//     return std::string{s.substr(s.size() - n, n)};
// }

// auto view::substr(std::string_view s, size_type pos, ssize_type offset) -> std::string {
//     if (offset > 0) {
//         size_type n = offset;
//         return std::string{s.substr(pos, std::min(n, pos + n))};
//     }

//     if (offset < 0) {
//         size_type n = -offset;
//         return std::string{s.substr(pos, std::min(n, pos + n))};
//     }

//     return "";
// }

// //  定宽对齐调整
// auto view::rjust_inplace(std::string_view s, size_type width, value_type ch) -> std::string {
//     if (s.size() >= width) {
//         return s;
//     }

//     size_type old_len = s.size();
//     s.resize(width);
//     std::memmove(s.data() + width - old_len, s.data(), old_len);
//     std::fill(s.data(), s.data() + width - old_len, ch);
//     return s;
// }

// auto view::rjust(std::string_view s, size_type width, value_type ch) -> std::string {
//     if (s.size() >= width) {
//         return std::string{s};
//     }

//     std::string result;
//     result.resize(width);
//     std::fill(result.data(), result.data() + (width - s.size()), ch);
//     std::memcpy(result.data() + (width - s.size()), s.data(), s.size());
//     return result;
// }

// auto view::ljust_inplace(std::string_view s, size_type width, value_type ch) -> std::string {
//     if (s.size() >= width) {
//         return s;
//     }

//     s.resize(width, ch);
//     return s;
// }

// auto view::ljust(std::string_view s, size_type width, value_type ch) -> std::string {
//     if (s.size() >= width) {
//         return std::string{s};
//     }

//     std::string result;
//     result.reserve(width);
//     result.append(s);
//     result.append(width - s.size(), ch);
//     return result;
// }

// auto view::center_inplace(std::string_view s, size_type width, value_type ch) -> std::string {
//     if (s.size() >= width) {
//         return s;
//     }

//     size_type left_len = (width - s.size()) / 2;
//     size_type right_len = (width - left_len) - s.size();

//     s.resize(width);
//     std::memmove(s.data() + left_len, s.c_str(), s.size());
//     std::fill(s.data(), s.data() + left_len, ch);
//     std::fill(s.data() + width - right_len, s.data() + width, ch);
//     return s;
// }

// auto view::center(std::string_view s, size_type width, value_type ch) -> std::string {
//     if (s.size() >= width) {
//         return std::string{s};
//     }

//     size_type left_len = (width - s.size()) / 2;
//     size_type right_len = (width - left_len) - s.size();

//     std::string result;
//     result.reserve(width);
//     result.append(left_len, ch);
//     result.append(s);
//     result.append(right_len, ch);
//     return result;
// }

// auto view::zfill_inplace(std::string_view s, size_type width) -> std::string {
//     if (s.empty()) {
//         s.resize(0);
//         return s;
//     }

//     if (s.size() >= width) {
//         return s;
//     }

//     if ((s[0] != '+') && (s[0] != '-')) {
//         return ljust_inplace(s, width, '0');
//     }

//     size_type old_len = s.size();
//     s.resize(width);
//     if (old_len - 1 > 0) {
//         std::memmove(s.data() + s.size() - (old_len - 1), s.c_str() + 1, old_len - 1);
//     }

//     std::fill(s.data() + 1, s.data() + s.size() - (old_len - 1), '0');
//     return s;
// }

// auto view::zfill(std::string_view s, size_type width) -> std::string {
//     if (s.empty()) {
//         std::string r;
//         r.resize(width, '0');
//         return r;
//     }

//     if (s.size() >= width) {
//         return std::string{s};
//     }

//     if ((s[0] != '+') && (s[0] != '-')) {
//         return ljust(s, width, '0');
//     }

//     std::string result;
//     result.resize(width);
//     pointer ptr = result.data();
//     *ptr = s[0];
//     ptr++;
//     std::fill(ptr, ptr + (width - s.size()), '0');
//     ptr += (width - s.size());
//     std::memcpy(ptr, s.data(), s.size());

//     return result;
// }

// // 字符串生成
// auto view::repeat(std::string_view s, size_type times) -> std::string {
//     if (s.empty() || (times == 0)) {
//         return "";
//     }

//     std::string result;
//     result.reserve(times * s.size());

//     for (size_type i = 0; i < times; i++) {
//         result.append(s);
//     }

//     return result;
// }

// auto view::space(size_type width) -> std::string {
//     return repeat(" ", width);
// }

// //  基于本字符串生成新字符串

// auto view::join(std::string_view s, const view_provider_proc& proc) -> std::string {
//     std::string result;
//     for (auto item = proc(); item; item = proc()) {
//         if (!result.empty()) {
//             result.append(s);
//         }
//         result.append(item.value());
//     }
//     return result;
// }

// // 路径拼接
// auto view::join_path(const view_provider_proc& proc) -> std::string {
//     std::string result;
//     for (auto item = proc(); item; item = proc()) {
//         if (!result.empty()) {
//             result.append("/");
//         }
//         result.append(item.value());
//     }
//     return result;
// }

// auto view::join_path(std::initializer_list<std::string_view> items) -> std::string {
//     auto itr = items.begin();
//     return join_path([&itr, &items]() -> std::optional<std::string_view> {
//         if (itr == items.end()) {
//             return {};
//         }

//         return *itr;
//     });
// }

// auto view::join_search_path(const view_provider_proc& proc) -> std::string {
//     return join(":", proc);
// }

// auto view::concat(const view_provider_proc& proc) -> std::string {
//     std::string result;
//     auto item = proc();
//     while (item) {
//         result.append(item.value());
//     }
//     return result;
// }

// auto view::title_inplace(std::string_view s) -> std::string {
//     if (s.empty()) {
//         return s;
//     }

//     s[0] = static_cast<value_type>(std::toupper(s[0]));
//     return s;
// }

// auto view::title(std::string_view s) -> std::string {
//     std::string result{s};
//     title_inplace(result);
//     return result;
// }

// //  反转：字符串逆序
// auto view::invert_inplace(std::string_view s, size_type pos, size_type max_n) -> std::string {
//     if (s.empty()) {
//         return s;
//     }

//     if (pos >= s.size()) {
//         return s;
//     }

//     max_n = std::min(max_n, (s.size() - pos));
//     pointer left = s.data() + pos;
//     pointer right = s.data() + pos + max_n - 1;

//     while (left < right) {
//         value_type ch = *left;
//         *left = *right;
//         *right = ch;
//         left++;
//         right--;
//     }

//     return s;
// }

// auto view::invert(std::string_view s, size_type pos, size_type max_n) -> std::string {
//     std::string result{s};
//     invert_inplace(result, pos, max_n);
//     return result;
// }

// 拆分字符串
auto view::split_list(std::string_view s, std::string_view sep, const view_consumer_proc& proc) -> void {
    size_type pos_start = 0;
    while (pos_start < s.size()) {
        size_type pos_end = s.find(sep, pos_start);
        if (pos_end == std::string::npos) {
            break;
        }

        if (proc(std::string_view{s.data() + pos_start, pos_end - pos_start}) != 0) {
            pos_start = pos_end + sep.size();
            break;
        }
        pos_start = pos_end + sep.size();
    }

    proc(std::string_view{s.data() + pos_start, s.size() - pos_start});
}

auto view::split_list(std::string_view s, std::string_view sep) -> std::vector<std::string> {
    std::vector<std::string> result;
    split_list(s, sep, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });
    return result;
}

auto view::split_list(std::string_view s, value_type sep) -> std::vector<std::string> {
    return split_list(s, std::string_view{&sep, 1});
}

// auto view::translate(std::string_view s, const char_mapping_proc& proc) -> std::string {
//     pointer ptr = s.data();
//     while (*ptr) {
//         *ptr = proc(*ptr);
//         ptr++;
//     }
//     return s;
// }

// auto view::simplified_proc(std::string_view s, const char_checker_proc& proc) -> std::string {
//     if (s.empty()) {
//         return s;
//     }

//     bool found = false;
//     const_pointer w = s.data();
//     pointer r = s.data();
//     while (*r != '\0') {
//         value_type ch = *r;
//         if (found) {
//             if (proc(ch)) {
//                 r++;
//                 continue;
//             }

//             found = false;
//             *(w++) = *(r++);
//             continue;
//         }

//         if (proc(ch)) {
//             found = true;
//             *(w++) = ' ';
//             r++;
//             continue;
//         }

//         *(w++) = *(r++);
//     }

//     s.resize(w - s.data());
//     return s;
// }

// auto view::simplified(std::string_view s) -> std::string {
//     if (s.empty()) {
//         return std::string{s};
//     }

//     std::string result;
//     bool found = true;
//     const_pointer r = s.data();
//     while (*r != '\0') {
//         value_type ch = *r;
//         if (found) {
//             if (std::isspace(ch)) {
//                 r++;
//                 continue;
//             }

//             found = false;
//             result.append(r, 1);
//             r++;
//             continue;
//         }

//         if (std::isspace(ch)) {
//             found = true;
//         }

//         result.append(r, 1);
//         r++;
//     }

//     if (!result.empty()) {
//         if (std::isspace(result.back())) {
//             result.resize(result.size() - 1);
//         }
//     }

//     return result;
// }

// auto view::simplified_inplace(std::string_view s) -> std::string {
//     if (s.empty()) {
//         return s;
//     }

//     bool found = true;
//     pointer w = s.data();
//     const_pointer r = s.c_str();
//     while (*r != '\0') {
//         if (found) {
//             if (std::isspace(*r)) {
//                 r++;
//                 continue;
//             }

//             found = false;
//             *(w++) = *(r++);
//             continue;
//         }

//         if (std::isspace(*r)) {
//             found = true;
//             *(w++) = ' ';
//             r++;
//             continue;
//         }

//         *(w++) = *(r++);
//     }

//     s.resize(w - s.c_str());

//     if (!s.empty()) {
//         if (std::isspace(s.back())) {
//             s.resize(s.size() - 1);
//         }
//     }

//     return s;
// }

// // 切除
// auto view::drop_right(std::string_view s, size_type n) -> std::string {
//     if (n > s.size()) {
//         s.resize(0);
//         return s;
//     }

//     s.resize(s.size() - n);
//     return s;
// }

// auto view::drop_right(std::string_view s, size_type n) -> std::string {
//     if (n > s.size()) {
//         return "";
//     }

//     return std::string{s.substr(0, s.size() - n)};
// }

// //  处理路径中文件名的部分
// static auto str_basename_ptr(std::string_view s) -> std::string::const_pointer {
//     ASSERT(!s.empty);

//     std::string::const_pointer ptr = s.data() + s.size();
//     while (ptr > s.data()) {
// #ifdef WIN32
//         if ((*(ptr - 1) == '/') || (*(ptr - 1) == '\\')) {
//             break;
//         }
// #else
//         if (*(ptr - 1) == '/') {
//             break;
//         }
// #endif
//     }

//     return ptr;
// }

// // 扩展名相关操作
// static auto str_extname_ptr(std::string_view s) -> std::string::const_pointer {
//     ASSERT(!s.empty());

//     std::string::const_pointer base_ptr = str_basename_ptr(s);
//     std::string::const_pointer end = s.data() + s.size();

//     if (base_ptr[0] == '.') {
//         while (base_ptr < end) {
//             if (*base_ptr != '.') {
//                 break;
//             }
//             base_ptr++;
//         }
//     }

//     std::string::const_pointer ptr = s.data() + s.size();

//     return ptr;
// }

// auto view::basename(std::string_view s) -> std::string {
//     return str_basename_ptr(s);
// }

// auto view::remove_basename(std::string_view s) -> std::string {
//     return std::string{s.data(), str_basename_ptr(s)};
// }

// auto view::remove_basename_inplace(std::string_view s) -> std::string {
//     const_pointer ptr = basename_ptr(s);
//     s.resize(ptr - s.c_str());
//     return s;
// }

// auto view::replace_basename_inplace(std::string_view s, std::string_view name) -> std::string {
//     const_pointer ptr = basename_ptr(s);
//     size_type dir_len = (ptr - s.c_str());
//     s.reserve(dir_len + name.size());
//     s.resize(dir_len);
//     s.append(name);
//     return s;
// }

// auto view::replace_basename(std::string_view s, std::string_view name) -> std::string {
//     const_pointer ptr = basename_ptr(s);
//     std::string result;
//     result.reserve((ptr - s.c_str()) + name.size());
//     result.append(s.c_str(), ptr - s.c_str());
//     result.append(name);
//     return result;
// }

// auto view::extname_ptr(std::string_view s) -> pointer {
//     pointer ptr = basename_ptr(s);
//     if (ptr[0] == '.') {
//         return s.data() + s.size();
//     }

//     ptr = std::strchr(ptr, '.');
//     if (ptr == nullptr) {
//         return s.data() + s.size();
//     }

//     return ptr;
// }

// auto view::extname(std::string_view s) -> std::string {
//     return extname_ptr(s);
// }

// auto view::remove_extname(std::string_view s) -> std::string {
//     const_pointer ptr = extname_ptr(s);
//     return s.substr(0, ptr - s.c_str());
// }

// auto view::remove_extname_inplace(std::string_view s) -> std::string {
//     const_pointer ptr = extname_ptr(s);
//     s.resize(ptr - s.c_str());
//     return s;
// }

// auto view::replace_extname_inplace(std::string_view s, std::string_view name) -> std::string {
//     pointer ptr = extname_ptr(s);
//     s.resize((ptr - s.c_str()) + name.size());
//     std::memcpy(ptr, name.c_str(), name.size());
//     return s;
// }

// auto view::replace_extname(std::string_view s, std::string_view name) -> std::string {
//     const_pointer ptr = basename_ptr(s);
//     std::string result;
//     result.reserve((ptr - s.c_str()) + name.size());
//     result.append(s.c_str(), ptr - s.c_str());
//     result.append(name);
//     return result;
// }

// template <typename T>
// auto view::to(std::string_view s, std::tuple<int> base) -> std::optional<T> {
//     return {};
// }
//
// template <typename T>
// auto view::to(std::string_view s) -> std::optional<T> {
//     return {};
// }
//
// template <typename T>
// auto view::to(std::string_view s, T def, std::tuple<int> base) -> T {
//     auto result = to<T>(s, base);
//     return result ? result.value() : def;
// }
//
// template <typename T>
// auto view::to(std::string_view s, T def) -> T {
//     auto result = to<T>(s);
//     return result ? result.value() : def;
// }
//
// template <>
// inline auto to<bool>(std::string_view s [[maybe_unused]]) -> std::optional<bool> {
//     return {};
// }
//
// template <>
// inline auto to<float>(std::string_view s) -> std::optional<float> {
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtof(s.c_str(), &endptr);
//     static_assert(sizeof(result) >= sizeof(float));
//     if (result <= std::numeric_limits<float>::epsilon()) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<double>(std::string_view s) -> std::optional<double> {
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtod(s.c_str(), &endptr);
//     static_assert(sizeof(result) >= sizeof(double));
//     if (result <= std::numeric_limits<double>::epsilon()) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     return result;
// }
//
// static inline constexpr auto correct_base(int base) -> int {
//     if (base != 0) {
//         if (base < 2) {
//             return 2;
//         }
//         if (base > 36) {
//             return 36;
//         }
//     }
//
//     return base;
// }
//
// template <>
// inline auto to<int8_t>(std::string_view s, std::tuple<int> base) -> std::optional<int8_t> {
//     int nbase = correct_base(std::get<0>(base));
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtol(s.c_str(), &endptr, nbase);
//     static_assert(sizeof(result) >= sizeof(int8_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     if ((result > std::numeric_limits<int8_t>::max()) || (result < std::numeric_limits<int8_t>::min())) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<int16_t>(std::string_view s, std::tuple<int> base) -> std::optional<int16_t> {
//     int nbase = correct_base(std::get<0>(base));
//
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtol(s.c_str(), &endptr, nbase);
//     static_assert(sizeof(result) >= sizeof(int16_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     if ((result > std::numeric_limits<int16_t>::max()) || (result < std::numeric_limits<int16_t>::min())) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<int32_t>(std::string_view s, std::tuple<int> base) -> std::optional<int32_t> {
//     int nbase = correct_base(std::get<0>(base));
//
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtol(s.c_str(), &endptr, nbase);
//     static_assert(sizeof(result) >= sizeof(int32_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     if ((result > std::numeric_limits<int32_t>::max()) || (result < std::numeric_limits<int32_t>::min())) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<int64_t>(std::string_view s, std::tuple<int> base) -> std::optional<int64_t> {
//     int nbase = correct_base(std::get<0>(base));
//
//     errno = 0;
//     char* endptr = nullptr;
// #if defined __LP64__
//     auto result = std::strtol(s.c_str(), &endptr, nbase);
// #else
//     auto result = std::strtoll(s.c_str(), &endptr, nbase);
// #endif
//     static_assert(sizeof(result) >= sizeof(int64_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     if ((result > std::numeric_limits<int64_t>::max()) || (result < std::numeric_limits<int64_t>::min())) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<uint8_t>(std::string_view s, std::tuple<int> base) -> std::optional<uint8_t> {
//     int nbase = correct_base(std::get<0>(base));
//
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtoul(s.c_str(), &endptr, nbase);
//     static_assert(sizeof(result) >= sizeof(uint8_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     if ((result > std::numeric_limits<uint8_t>::max())) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<uint16_t>(std::string_view s, std::tuple<int> base) -> std::optional<uint16_t> {
//     int nbase = correct_base(std::get<0>(base));
//
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtoul(s.c_str(), &endptr, nbase);
//     static_assert(sizeof(result) >= sizeof(uint16_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     if ((result > std::numeric_limits<uint16_t>::max())) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<uint32_t>(std::string_view s, std::tuple<int> base) -> std::optional<uint32_t> {
//     int nbase = correct_base(std::get<0>(base));
//
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtoul(s.c_str(), &endptr, nbase);
//     static_assert(sizeof(result) >= sizeof(uint32_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     if ((result > std::numeric_limits<uint32_t>::max())) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<uint64_t>(std::string_view s, std::tuple<int> base) -> std::optional<uint64_t> {
//     int nbase = correct_base(std::get<0>(base));
//
//     errno = 0;
//     char* endptr = nullptr;
// #if defined __LP64__
//     auto result = std::strtoul(s.c_str(), &endptr, nbase);
// #else
//     auto result = std::strtoull(s.c_str(), &endptr, nbase);
// #endif
//     static_assert(sizeof(result) >= sizeof(int64_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     return result;
// }
