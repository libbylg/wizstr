//
// Created by libbylg on 2023/6/1.
//
#include "view.h"
#include "str.h"

#include <cassert>

auto view::append(std::string_view s, std::string_view other) -> std::string {
    std::string result;
    result.reserve(s.size() + other.size());
    result.append(s);
    result.append(other);
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
    auto item = proc();
    while (item) {
        result.append(item.value());
        item = proc();
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
    auto item = proc();
    while (item) {
        result.insert(0, item.value());
        item = proc();
    }
    result.append(s);
    return result;
}

auto view::insert(std::string_view s, size_type pos, std::string_view other) -> std::string {
    if (other.empty()) {
        return std::string{s};
    }

    if (pos >= s.size()) {
        pos = s.size();
    }

    std::string result;
    result.reserve(other.size() + s.size());
    result.append(std::string_view{s.data(), pos});
    result.append(other);
    result.append(std::string_view{s.data() + pos, s.size() - pos});
    return result;
}

auto view::insert(std::string_view s, size_type pos, value_type ch) -> std::string {
    return view::insert(s, pos, std::string_view{&ch, 1});
}

auto view::insert(std::string_view s, size_type pos, value_type ch, size_type n) -> std::string {
    if (n == 0) {
        return std::string{s};
    }

    if (pos >= s.size()) {
        pos = s.size();
    }

    std::string result;
    result.reserve(s.size() + n);
    result.append(s.data(), pos);
    result.append(n, ch);
    result.append(s.data() + pos, s.size() - pos);
    return result;
}

auto view::insert(std::string_view s, size_type pos, const view_provider_proc& proc) -> std::string {
    if (pos > s.size()) {
        pos = s.size();
    }

    std::string result;
    result.append(s.data(), pos);

    auto item = proc();
    while (item) {
        result.append(item.value());
        item = proc();
    }

    result.append(std::string_view{s.data() + pos, s.size() - pos});
    return result;
}

auto view::icompare(std::string_view s, std::string_view other) -> int {
    if (s.size() < other.size()) {
        int ret = strncasecmp(s.data(), other.data(), s.size());
        return (ret == 0) ? -other[s.size()] : ret;
    }

    if (s.size() > other.size()) {
        int ret = strncasecmp(s.data(), other.data(), other.size());
        return (ret == 0) ? s[other.size()] : ret;
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
    for (auto elem_ch : s) {
        count += ((elem_ch == ch) ? 1 : 0);
    }
    return count;
}

auto view::count(std::string_view s, const char_match_proc& proc) -> view::size_type {
    size_type count = 0;
    for (value_type ch : s) {
        if (proc(ch)) {
            count++;
        }
    }
    return count;
}

auto view::prefix(std::string_view s, std::string_view other) -> view::size_type {
    if ((s.empty()) || (other.empty())) {
        return 0;
    }

    size_type len = std::min(s.size(), other.size());
    size_type pos = 0;
    while (pos < len) {
        if (s[pos] != other[pos]) {
            break;
        }
        pos++;
    }

    return pos;
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
    if (!has_suffix(s, suffix)) {
        return s;
    }

    return std::string_view{s.data(), s.size() - suffix.size()};
}

auto view::remove_suffix(std::string_view s, value_type suffix) -> std::string_view {
    return remove_suffix(s, {&suffix, 1});
}

auto view::find_next_regex(std::string_view s, const std::regex& pattern, size_type pos) -> std::optional<std::string_view> {
    if (pos >= s.size()) {
        s = {};
    } else {
        s = s.substr(pos);
    }

    std::match_results<std::string_view::const_iterator> result;
    if (!std::regex_search(s.begin(), s.end(), result, pattern)) {
        return std::nullopt;
    }

    return std::string_view{result[0].first, static_cast<size_type>(result[0].second - result[0].first)};
}

auto view::find_next_regex(std::string_view s, std::string_view pattern, size_type pos) -> std::optional<std::string_view> {
    return find_next_regex(s, std::regex{pattern.begin(), pattern.end()}, pos);
}

auto view::find_next_eol(std::string_view s, size_type pos) -> std::string_view {
    if (pos >= s.size()) {
        return {};
    }

    s = std::string_view{s.data() + pos, static_cast<size_type>(s.size() - pos)};

    const_pointer endptr = (s.data() + s.size());
    const_pointer ptr = s.data();
    while (ptr < endptr) {
        if (*ptr == '\r') [[unlikely]] {
            // 遇到 xxx\r\nyyyy
            if (((ptr + 1) < endptr) && (*(ptr + 1) == '\n')) {
                return std::string_view{ptr, 2};
            }

            // 遇到 xxx\ryyyy
            return std::string_view{ptr, 1};
        }

        // 遇到 xxx\nyyyy
        if (*ptr == '\n') {
            return std::string_view{ptr, 1};
        }

        ptr++;
    }

    return {};
}

auto view::find_next_word(std::string_view s, size_type pos) -> std::string_view {
    if (pos >= s.size()) {
        return {};
    }

    auto start = std::find_if_not(s.begin() + pos, s.end(), [](value_type ch) -> bool {
        return std::isspace(ch);
    });

    if (start == s.end()) {
        return {};
    }

    auto end = std::find_if(start, s.end(), [](value_type ch) -> bool {
        return std::isspace(ch);
    });

    return std::string_view{start, static_cast<size_type>(end - start)};
}

auto view::iter_next_regex(std::string_view s, size_type& pos, const std::regex& pattern) -> std::optional<std::string_view> {
    auto result = view::find_next_regex(s, pattern, pos);
    if (!result) {
        pos = s.size();
        return std::nullopt;
    }

    pos = (result.value().data() + result.value().size()) - s.data();
    return result.value();
}

auto view::iter_next_regex(std::string_view s, size_type& pos, std::string_view pattern) -> std::optional<std::string_view> {
    return view::iter_next_regex(s, pos, std::regex{pattern.begin(), pattern.end()});
}

auto view::iter_next_string(std::string_view s, size_type& pos, std::string_view other) -> size_type {
    assert(!other.empty());

    if (pos >= s.size()) {
        pos = s.size();
        return view::npos;
    }

    auto next_pos = s.find(other, pos);
    if (next_pos == std::string_view::npos) {
        pos = s.size();
        return view::npos;
    }

    pos = next_pos + other.size();
    return next_pos;
}

auto view::iter_next_eol(std::string_view s, size_type& pos) -> std::string_view {
    std::string_view eol = view::find_next_eol(s, pos);
    if (eol.empty()) {
        pos = s.size();
        return {};
    }

    pos = static_cast<size_type>((eol.data() + eol.size()) - s.data());
    return eol;
}

// auto view::iter_prev_eol(std::string_view s, size_type& pos) -> size_type {
// }

auto view::iter_next_word(std::string_view s, size_type& pos) -> std::string_view {
    auto word = view::find_next_word(s, pos);
    if (word.empty()) {
        pos = s.size();
        return {};
    }

    pos = (word.data() + word.size()) - s.data();
    return word;
}

// auto view::iter_prev_word(std::string_view s, size_type& pos) -> std::string_view {
// }

auto view::foreach_word(std::string_view s, size_type pos, const std::function<int(size_type pos, size_type n)>& proc) -> void {
    while (pos < s.size()) {
        auto r = view::iter_next_word(s, pos);
        if (r.empty()) {
            assert(pos >= s.size());
            break;
        }

        if (proc(r.data() - s.data(), r.size()) != 0) {
            return;
        }
    }
}

auto view::foreach_word(std::string_view s, size_type pos, const std::function<int(std::string_view word)>& proc) -> void {
    view::foreach_word(s, pos, [&s, &proc](size_type pos, size_type n) -> int {
        return proc(std::string_view{s.data() + pos, n});
    });
}

auto view::foreach_word(std::string_view s, const std::function<int(size_type pos, size_type n)>& proc) -> void {
    view::foreach_word(s, 0, proc);
}

auto view::foreach_word(std::string_view s, const std::function<int(std::string_view word)>& proc) -> void {
    view::foreach_word(s, 0, [&s, &proc](size_type pos, size_type n) -> int {
        return proc(std::string_view{s.data() + pos, n});
    });
}

// auto view::is_match_wild(std::string_view s, std::string_view pattern) -> bool {
// }

// auto view::is_match_charset(std::string_view s, charset_type set) -> bool {
// }

// auto view::is_match_regex(std::string_view s, std::string_view pattern) -> bool {
// }

// auto view::is_match_regex(std::string_view s, const std::regex& pattern) -> bool {
// }

auto view::is_lower(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::islower(*ptr)) {
            return false;
        }
    }

    return true;
}

auto view::is_upper(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

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

auto view::is_capitalize(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    return std::isupper(s[0]);
}

auto view::is_digit(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isdigit(*ptr)) {
            return false;
        }
    }

    return true;
}

auto view::is_xdigit(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isxdigit(*ptr)) {
            return false;
        }
    }
    return true;
}

auto view::is_ascii(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if ((*ptr & 0x80) != 0) {
            return false;
        }
    }
    return true;
}

auto view::is_alpha(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isalpha(*ptr)) {
            return false;
        }
    }
    return true;
}

auto view::is_alnum(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isalnum(*ptr)) {
            return false;
        }
    }
    return true;
}

auto view::is_space(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isspace(*ptr)) {
            return false;
        }
    }
    return true;
}

auto view::is_blank(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isblank(*ptr)) {
            return false;
        }
    }
    return true;
}

auto view::is_print(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isprint(*ptr)) {
            return false;
        }
    }
    return true;
}

auto view::is_graph(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

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
    return view::is_literal_true(s) || view::is_literal_false(s);
}

auto view::is_literal_true(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    // "1"     "0"
    // "on"    "off"
    // "ON"    "OFF"
    // "Yes"   "No"
    // "yes"   "no"
    // "YES"   "NO"
    // "True"  "False"
    // "true"  "false"
    // "TRUE"  "FALSE"

    const_pointer ptr = s.data();
    switch (s.size()) {
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
            return false;
        case 1:
            switch (*ptr) {
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

auto view::is_literal_false(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    // "1"     "0"
    // "on"    "off"
    // "ON"    "OFF"
    // "Yes"   "No"
    // "yes"   "no"
    // "YES"   "NO"
    // "True"  "False"
    // "true"  "false"
    // "TRUE"  "FALSE"

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
                return false;
            }
            if ((ptr[0] == 'T') && (ptr[1] == 'r') && (ptr[2] == 'u') && (ptr[3] == 'e')) {
                return false;
            }
            if ((ptr[0] == 'T') && (ptr[1] == 'R') && (ptr[2] == 'U') && (ptr[3] == 'E')) {
                return false;
            }
            return false;
        case 3:
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
                default:
                    return false;
            }
        default:
            return false;
    }

    return false;
}

// auto view::is_literal_real(std::string_view s) -> bool {
// }

// auto view::is_literal_integer(std::string_view s) -> bool {
// }

///////////////////11111111111111111111

auto view::take_left(std::string_view s, size_type n) -> std::string_view {
    if (n == 0) {
        return "";
    }

    if (n > s.size()) {
        return s;
    }

    return std::string_view{s.substr(0, n)};
}

auto view::take_right(std::string_view s, size_type n) -> std::string_view {
    if (n == 0) {
        return {};
    }

    if (n > s.size()) {
        return s;
    }

    return std::string_view{s.substr(s.size() - n, n)};
}

auto view::take_mid(std::string_view s, size_type pos, size_type n) -> std::string_view {
    if (s.empty()) {
        return s;
    }

    if (pos >= s.size()) {
        return {};
    }

    if (n > (s.size() - pos)) {
        return {s.data() + pos, (s.size() - pos)};
    }

    return {s.data() + pos, n};
}

auto view::take(std::string_view s, size_type pos, ssize_type offset) -> std::string_view {
    if (s.empty() || (offset == 0)) {
        return {};
    }

    if (offset > 0) {
        if (pos >= s.size()) {
            return {};
        }
        size_type n = offset;
        return s.substr(pos, std::min(n, pos + n));
    }

    assert(offset < 0);
    size_type n = -offset;

    // 如果 pos 太大
    if (pos >= s.size()) {
        pos = s.size() - 1;
    }

    // 如果n太大
    if (n > pos) {
        return s.substr(0, (pos + 1));
    }

    // 如果n较小
    return s.substr(((pos + 1) - n), n);
}

auto view::align_left(std::string_view s, size_type width, value_type ch) -> std::string {
    if (s.size() >= width) {
        return std::string{s};
    }

    std::string result;
    result.reserve(width);
    result.append(s);
    result.append(width - s.size(), ch);
    return result;
}

auto view::align_right(std::string_view s, size_type width, value_type ch) -> std::string {
    if (s.size() >= width) {
        return std::string{s};
    }

    std::string result;
    result.resize(width);
    std::fill(result.data(), result.data() + (width - s.size()), ch);
    std::memcpy(result.data() + (width - s.size()), s.data(), s.size());
    return result;
}

auto view::align_center(std::string_view s, size_type width, value_type ch) -> std::string {
    if (s.size() >= width) {
        return std::string{s};
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

auto view::align_zfill(std::string_view s, size_type width) -> std::string {
    if (s.empty()) {
        std::string r;
        r.resize(width, '0');
        return r;
    }

    if (s.size() >= width) {
        return std::string{s};
    }

    if ((s[0] != '+') && (s[0] != '-')) {
        return view::align_right(s, width, '0');
    }

    std::string result;
    result.reserve(width);
    result.append(s.data(), 1);
    result.resize((width - s.size() + 1), '0');
    result.append(s.data() + 1, static_cast<size_type>(s.size() - 1));

    return result;
}

auto view::capitalize(std::string_view s) -> std::string {
    if (s.empty()) {
        return {};
    }

    std::string result{s};
    if (std::islower(s[0])) {
        result[0] = static_cast<value_type>(std::toupper(s[0]));
    }

    return result;
}

auto view::title(std::string_view s) -> std::string {
    std::string result{s};
    return str::title(result);
}

auto view::invert(std::string_view s, size_type pos, size_type max_n) -> std::string {
    std::string result{s};
    return str::invert(result, pos, max_n);
}

auto view::repeat(std::string_view s, size_type times) -> std::string {
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

auto view::repeat(value_type ch, size_type times) -> std::string {
    std::string result;
    result.resize(times, ch);
    return result;
}

auto view::spaces(size_type width) -> std::string {
    return repeat(' ', width);
}

auto view::skip_space(std::string_view s, size_type pos) -> std::string_view {
    if (pos >= s.size()) {
        return {};
    }

    const_pointer ptr = s.data() + pos;
    while (ptr < s.data() + s.size()) {
        if (!std::isspace(*ptr)) {
            break;
        }
        ptr++;
    }

    auto n = static_cast<size_type>(s.size() - (ptr - s.data()));
    return std::string_view{ptr, n};
}

auto view::join_list(std::string_view s, const view_provider_proc& proc) -> std::string {
    std::string result;
    bool suffix = false;
    for (auto item = proc(); item; item = proc()) {
        if (suffix) [[likely]] {
            result.append(s);
        }
        result.append(item.value());
        suffix = true;
    }
    return result;
}

auto view::join_list(const view_provider_proc& proc) -> std::string {
    return view::join_list(",", proc);
}

auto view::join_map(std::string_view sep_pair, std::string_view sep_list, const view_pair_provider_proc& proc) -> std::string {
    std::string result;
    bool suffix = false;
    for (auto item = proc(); item; item = proc()) {
        if (suffix) {
            result.append(sep_list);
        }

        result.append(std::get<0>(item.value()));
        result.append(sep_pair);
        result.append(std::get<1>(item.value()));
        suffix = true;
    }
    return result;
}

auto view::join_map(const view_pair_provider_proc& proc) -> std::string {
    return view::join_map("=", ",", proc);
}

auto view::join_path(const view_provider_proc& proc) -> std::string {
    std::string result;
    for (auto item = proc(); item; item = proc()) {
        // 如果每项为空,全部跳过
        if (item->empty()) {
            continue;
        }

        // 如果某项以斜杠开头，那么该项目将作为绝对路径项替换已有内容
        if (item->front() == '/') {
            result = *item;
            continue;
        }

        // 根据 result 的后缀决定是否要补路径分隔符
        if (!result.empty() && (result.back() != '/')) {
            result.append("/");
        }

        result.append(*item);
    }

    return result;
}

auto view::join_search_path(const view_provider_proc& proc) -> std::string {
    std::string result;
    for (auto item = proc(); item; item = proc()) {
        // 如果每项为空,全部跳过
        if (item->empty()) {
            continue;
        }

        if (!result.empty()) {
            result.append(":");
        }

        result.append(*item);
    }

    return result;
}

auto view::split_list(std::string_view s, std::string_view sep, size_type max_n, const view_consumer_proc& proc) -> void {
    assert(!sep.empty());

    // 最大拆分次数如果为0，就不需要拆了
    if (max_n == 0) {
        proc(s);
        return;
    }

    size_type n = 0;
    size_type pos_start = 0;
    while (pos_start < s.size()) {
        // 找到分隔符的位置
        size_type pos_end = s.find(sep, pos_start);
        if (pos_end == std::string::npos) {
            break;
        }

        // 执行输出,如果用户希望提前结束就直接结束后续的部分不再理会
        if (proc(std::string_view{s.data() + pos_start, (pos_end - pos_start)}) != 0) {
            return;
        }

        // 准备好下次查找的起点
        pos_start = pos_end + sep.size();

        // 根据次数判定是否还需要继续找:如果不再需要找，就中断循环，将剩余的部分丢给用户
        n++;
        if (n >= max_n) {
            break;
        }
    }

    proc(std::string_view{s.data() + pos_start, s.size() - pos_start});
}

auto view::split_list(std::string_view s, std::string_view sep, const view_consumer_proc& proc) -> void {
    view::split_list(s, sep, view::npos, [&proc](std::string_view item) -> int {
        return proc(item);
    });
}

auto view::split_list(std::string_view s, std::string_view sep, size_type max_n) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;

    view::split_list(s, sep, max_n, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });

    return result;
}

auto view::split_list(std::string_view s, const std::regex& sep, size_type max_n, const view_consumer_proc& proc) -> void {
    if (max_n == 0) {
        proc(s);
        return;
    }

    size_type n = 0;

    size_type last_pos = 0;
    size_type pos = 0;
    while (pos < s.size()) {
        // 找到满足正则表达式的位置
        auto item = view::find_next_regex(s, sep, pos);
        if (!item) {
            pos = s.size();
            break;
        }

        assert(!item->empty());

        // 将找到的数据输出给调用方
        size_type start = item->data() - s.data();
        size_type stop = start + item->size();
        if (proc(std::string_view{s.data() + last_pos, (start - last_pos)}) != 0) {
            return;
        }

        // 为下次查找做准备
        last_pos = stop;
        pos = stop;

        // 如果次数达到最大次数限制:中断循环，并将剩余的数据输出
        n++;
        if (n >= max_n) {
            break;
        }
    }

    // 最后一部分也需要单独拆出来
    proc(std::string_view{s.data() + last_pos, (s.size() - last_pos)});
}

auto view::split_list(std::string_view s, const std::regex& sep, const view_consumer_proc& proc) -> void {
    view::split_list(s, sep, view::npos, proc);
}

auto view::split_list(std::string_view s, const std::regex& sep, size_type max_n) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;

    view::split_list(s, sep, max_n, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });

    return result;
}

auto view::split_words(std::string_view s, const view_consumer_proc& proc) -> void {
    size_type pos = 0;
    while (pos < s.size()) {
        std::string_view word = view::iter_next_word(s, pos);
        if (word.empty()) {
            assert(pos >= s.size());
            continue;
        }

        if (proc(word) != 0) {
            break;
        }
    }
}

auto view::split_words(std::string_view s, size_type max_n) -> std::vector<std::string_view> {
    if (max_n == 0) {
        return {};
    }

    std::vector<std::string_view> result;

    view::split_words(s, [&result, max_n](std::string_view item) -> int {
        assert(!item.empty());
        result.emplace_back(item);
        if (result.size() >= max_n) {
            return -1;
        }
        return 0;
    });

    return result;
}

// auto view::split_map(std::string_view s[2], view_pair_consumer_proc proc) -> void {
// }
//
// auto view::split_map(const std::string_view s) -> std::map<std::string, std::string> {
// }

auto view::split_lines(std::string_view s, bool keep_ends, const view_consumer_proc& proc) -> void {
    if (s.empty()) {
        return;
    }

    size_type start = 0;
    while (start < s.size()) {
        auto eol = view::find_next_eol(s, start);

        // 最后一部分没有结束符
        if (eol.empty()) {
            proc(std::string_view{s.data() + start, (s.size() - start)});
            return;
        }

        assert(!eol.empty());

        // 遇到结束符
        const_pointer next_start = eol.data() + eol.size();
        const_pointer line_text = s.data() + start;
        size_type line_size = keep_ends ? (next_start - line_text) : (next_start - line_text - eol.size());
        if (proc(std::string_view{line_text, line_size}) != 0) {
            return;
        }

        start = next_start - s.data();
    }
}

auto view::split_lines(std::string_view s, bool keep_ends) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    view::split_lines(s, keep_ends, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });
    return result;
}

auto view::split_path(std::string_view s, const view_consumer_proc& proc) -> void {
    if (s.empty()) {
        return;
    }

    const_pointer ptr = s.data();
    if (*ptr == '/') {
        if (proc(std::string_view{ptr, 1}) != 0) {
            return;
        }

        ptr++;
    }

    const_pointer endptr = s.data() + s.size();
    while (ptr < endptr) {

        // 跳过多余的斜杠，定位到起始位置
        const_pointer start = ptr;
        while (start < endptr) {
            if (*start != '/') {
                break;
            }
            start++;
        }

        // 找到结束位置
        ptr = start;
        while (ptr < endptr) {
            if (*ptr == '/') {
                break;
            }
            ptr++;
        }

        // 长度太短时，是无效的elem
        auto len = static_cast<size_type>(ptr - start);
        if (len == 0) {
            break;
        }

        if (proc(std::string_view{start, len}) != 0) {
            return;
        }
    }
}

auto view::split_path(std::string_view s) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    view::split_path(s, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });
    return result;
}

// auto view::split_search_path(std::string_view s, const view_consumer_proc& proc) -> void {
// }
//
// auto view::split_search_path(std::string_view s) -> std::vector<std::string_view> {
// }

// auto view::split_csv(std::string_view s) -> std::vector<std::string> {
// }

auto view::to_lower(std::string_view s) -> std::string {
    std::string result{s};
    return str::to_lower(result);
}

auto view::to_upper(std::string_view s) -> std::string {
    std::string result{s};
    return str::to_upper(result);
}

auto view::swap_case(std::string_view s) -> std::string {
    std::string result{s};
    return str::swap_case(result);
}

// auto view::case_fold(std::string_view s) -> std::string {
// }

// auto view::translate(std::string_view s, const char_mapping_proc& proc) -> std::string {
// }

auto view::simplified(std::string_view s, std::string_view sep, char_checker_proc proc) -> std::string {
    if (s.empty()) {
        return std::string{s};
    }

    // 字符串最左端的空白
    const_pointer ptr = s.data();
    while (ptr < (s.data() + s.size())) {
        if (!proc(*ptr)) {
            break;
        }
        ptr++;
    }

    // 非空白
    const_pointer start = ptr;
    while (ptr < (s.data() + s.size())) {
        value_type ch = *ptr;
        if (proc(ch)) {
            break;
        }
        ptr++;
    }

    // 保存非空白
    std::string result;
    if (ptr != start) {
        result.append(start, (ptr - start));
    }

    while (ptr < (s.data() + s.size())) {
        // 跳过空白
        while (ptr < (s.data() + s.size())) {
            if (!proc(*ptr)) {
                break;
            }
            ptr++;
        }

        // 如果提前结束了
        if (ptr >= (s.data() + s.size())) {
            break;
        }

        result.append(sep);

        // 非空白
        start = ptr;
        while (ptr < (s.data() + s.size())) {
            if (proc(*ptr)) {
                break;
            }
            ptr++;
        }

        if (ptr != start) {
            result.append(start, (ptr - start));
        }
    }

    return result;
}

auto view::simplified(std::string_view s) -> std::string {
    return view::simplified(s, " ", [](value_type ch) -> bool {
        return std::isspace(ch);
    });
}

auto view::trim_left(std::string_view s, char_checker_proc proc) -> std::string_view {
    if (s.empty()) [[unlikely]] {
        return s;
    }

    auto left_ptr = s.data();
    while (left_ptr < (s.data() + s.size())) {
        if (!proc(*left_ptr)) [[unlikely]] {
            break;
        }

        left_ptr++;
    }

    return std::string_view{left_ptr, s.size() - (left_ptr - s.data())};
}

auto view::trim_left(std::string_view s) -> std::string_view {
    return view::trim_left(s, [](value_type ch) -> bool {
        return std::isspace(ch);
    });
}

auto view::trim_right(std::string_view s, char_checker_proc proc) -> std::string_view {
    if (s.empty()) [[unlikely]] {
        return s;
    }

    auto right_ptr = s.data() + s.size();
    while (right_ptr > s.data()) {
        if (!proc(*(right_ptr - 1))) {
            break;
        }
        right_ptr--;
    }

    return {s.data(), static_cast<size_type>(right_ptr - s.data())};
}

auto view::trim_right(std::string_view s) -> std::string_view {
    return view::trim_right(s, [](value_type ch) -> bool {
        return std::isspace(ch);
    });
}

auto view::trim_surrounding(std::string_view s, char_checker_proc proc) -> std::string_view {
    if (s.empty()) {
        return s;
    }

    auto left_ptr = s.data();
    while (left_ptr < (s.data() + s.size())) {
        if (!proc(*left_ptr)) [[unlikely]] {
            break;
        }

        left_ptr++;
    }

    auto right_ptr = s.data() + s.size();
    while (right_ptr > left_ptr) {
        if (!proc(*(right_ptr - 1))) {
            break;
        }
        right_ptr--;
    }

    return std::string_view{left_ptr, static_cast<size_type>(right_ptr - left_ptr)};
}

auto view::trim_surrounding(std::string_view s) -> std::string_view {
    return view::trim_surrounding(s, [](value_type ch) -> bool {
        return std::isspace(ch);
    });
}

auto view::trim_anywhere(std::string_view s, char_checker_proc proc) -> std::string {
    std::string result;

    const_pointer r = s.data();
    const_pointer end = s.data() + s.size();
    const_pointer ptr = s.data();
    while (ptr < end) {
        // 找到需要清除的起点
        while (ptr < end) {
            if (proc(*ptr)) {
                break;
            }
            ptr++;
        }

        if (r < ptr) {
            result.append(r, ptr - r);
        }
        r = nullptr;

        // 找到需要清除的终点
        while (ptr < end) {
            if (!proc(*ptr)) {
                r = ptr;
                break;
            }
            ptr++;
        }
    }

    if ((r != nullptr) && (r < end)) {
        result.append(r, end - r);
    }

    return result;
}

auto view::trim_anywhere(std::string_view s) -> std::string {
    return view::trim_anywhere(s, [](value_type ch) -> bool {
        return std::isspace(ch);
    });
}

// // 切除
// auto view::drop_right(std::string_view s, size_type n) -> std::string {
//     if (n > s.size()) {
//         s.resize(0);
//         return s;
//     }

// s.resize(s.size() - n);
// return s;
// }

// auto view::drop_right(std::string_view s, size_type n) -> std::string {
//     if (n > s.size()) {
//         return "";
//     }

// return std::string{s.substr(0, s.size() - n)};
// }

// 处理路径中文件名的部分
auto view::basename_ptr(std::string_view s) -> std::string::const_pointer {
    // if (s.empty() || (s == ".") || (s == "..")) [[unlikely]] {
    if (s.empty()) [[unlikely]] {
        return (s.data() + s.size());
    }

    const_pointer ptr = s.data() + s.size();
    while (ptr > s.data()) {
#ifdef WIN32
        if ((*(ptr - 1) == '/') || (*(ptr - 1) == '\\')) {
            break;
        }
#else
        if (*(ptr - 1) == '/') {
            break;
        }
        ptr--;
#endif
    }

    // auto base_view = std::string_view{ptr, s.size() - (ptr - s.data())};
    // if ((base_view == "..") || (base_view == ".")) {
    //     return ptr += base_view.size();
    // }

    return ptr;
}

// 扩展名相关操作
auto view::extname_ptr(std::string_view s) -> std::string::const_pointer {
    if (s.empty()) {
        return (s.data() + s.size());
    }

    std::string::const_pointer base_ptr = basename_ptr(s);
    std::string::const_pointer ptr = s.data() + s.size() - 1;

    while (ptr > base_ptr) {
        if (*ptr == '.') {
            return ptr;
        }
        ptr--;
    }

    return s.data() + s.size();
}

auto view::dirname_ptr(std::string_view s) -> std::string::const_pointer {
    auto ptr = basename_ptr(s);
    while (ptr > s.data()) {
        if (*(ptr - 1) != '/') {
            break;
        }

        ptr--;
    }
    return ptr;
}

auto view::dirname_view(std::string_view s) -> std::string_view {
    auto ptr = view::dirname_ptr(s);
    if ((ptr == s.data()) && (*ptr == '/')) {
        return std::string_view{s.data(), 1};
    }
    return std::string_view{s.data(), static_cast<size_type>(ptr - s.data())};
}

auto view::dirname(std::string_view s) -> std::string {
    std::string_view dir_view = dirname_view(s);
    if (dir_view.empty()) {
        return ".";
    }
    return std::string{dir_view};
}

auto view::remove_dirname(std::string_view s) -> std::string {
    auto ptr = view::dirname_ptr(s);
    return std::string{ptr, static_cast<size_type>(s.data() + s.size() - ptr)};
}

auto view::replace_dirname(std::string_view s, std::string_view newdir) -> std::string {
    auto ptr = view::dirname_ptr(s);

    auto remain_len = (s.data() + s.size() - ptr);

    assert(ptr >= s.data());

    std::string result;
    size_type result_len = newdir.size() + remain_len;
    if ((ptr < (s.data() + s.size())) && (*ptr == '/')) {
        result.reserve(result_len);
        result.append(newdir);
        result.append(ptr, remain_len);
    } else {
        result.reserve((result_len + 1));
        result.append(newdir);
        result.append("/");
        result.append(ptr, remain_len);
    }

    return result;
}

auto view::split_dirname(std::string_view s) -> std::tuple<std::string_view, std::string_view> {
    auto base_ptr = view::basename_ptr(s);
    auto dir_ptr = view::dirname_ptr({s.data(), static_cast<size_type>(base_ptr - s.data())});
    return {{s.data(), static_cast<size_type>(dir_ptr - s.data())},
        {base_ptr, static_cast<size_type>((s.data() + s.size()) - base_ptr)}};
}

auto view::basename_view(std::string_view s) -> std::string_view {
    auto ptr = view::basename_ptr(s);
    return std::string_view{ptr, static_cast<size_type>(s.data() + s.size() - ptr)};
}

auto view::basename(std::string_view s) -> std::string {
    return std::string{view::basename_view(s)};
}

auto view::remove_basename(std::string_view s) -> std::string {
    auto ptr = view::basename_ptr(s);
    return std::string{s.data(), static_cast<size_type>(ptr - s.data())};
}

auto view::replace_basename(std::string_view s, std::string_view newname) -> std::string {
    auto ptr = view::basename_ptr(s);
    std::string result;
    result.reserve((ptr - s.data()) + newname.size());
    result.append(s.data(), static_cast<size_type>(ptr - s.data()));
    result.append(newname);
    return result;
}

auto view::split_basename(std::string_view s) -> std::tuple<std::string_view, std::string_view> {
    auto ptr = view::basename_ptr(s);
    return {{s.data(), static_cast<size_type>(ptr - s.data())}, {ptr, static_cast<size_type>(s.data() + s.size() - ptr)}};
}

auto view::extname_view(std::string_view s) -> std::string_view {
    auto ptr = view::extname_ptr(s);
    return std::string_view{ptr, static_cast<size_type>(s.data() + s.size() - ptr)};
}

auto view::extname(std::string_view s) -> std::string {
    auto ptr = view::extname_ptr(s);
    return std::string{ptr, static_cast<size_type>(s.data() + s.size() - ptr)};
}

auto view::remove_extname(std::string_view s) -> std::string {
    auto ptr = view::extname_ptr(s);
    return std::string{s.data(), static_cast<size_type>(ptr - s.data())};
}

auto view::replace_extname(std::string_view s, std::string_view newname) -> std::string {
    auto ptr = view::extname_ptr(s);
    std::string result;
    result.reserve(ptr - s.data() + newname.size());
    result.append(s.data(), static_cast<size_type>(ptr - s.data()));
    result.append(newname);
    return result;
}

auto view::split_extname(std::string_view s) -> std::tuple<std::string_view, std::string_view> {
    auto ptr = view::extname_ptr(s);
    return {{s.data(), static_cast<size_type>(ptr - s.data())}, {ptr, static_cast<size_type>(s.data() + s.size() - ptr)}};
}
