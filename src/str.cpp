/**
 * Copyright (c) 2021-2024 libbylg@126.com
 * tiny is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "str.hpp"

#include <fcntl.h>
#include <sys/stat.h>

#include <cassert>

#include <fstream>

auto str::append(std::string_view s, std::string_view other, size_type times_n) -> std::string {
    if (other.empty() || (times_n == 0)) [[unlikely]] {
        return std::string{s};
    }

    std::string result;
    result.reserve(s.size() + (other.size() * times_n));
    result.append(s);

    for (size_type index = 0; index < times_n; index++) {
        result.append(other);
    }

    return result;
}

auto str::append_inplace(std::string& s, std::string_view other, size_type times_n) -> std::string& {
    if (other.empty() || (times_n == 0)) [[unlikely]] {
        return s;
    }

    s.reserve(s.size() + other.size() * times_n);
    for (size_type i = 0; i < times_n; i++) {
        s.append(other);
    }
    return s;
}

auto str::prepend(std::string_view s, std::string_view other, size_type times_n) -> std::string {
    if (other.empty() || (times_n == 0)) [[unlikely]] {
        return std::string{s};
    }

    std::string result;
    result.reserve(s.size() + other.size() * times_n);
    for (size_type index = 0; index < times_n; index++) {
        result.append(other);
    }
    result.append(s);
    return result;
}

auto str::prepend_inplace(std::string& s, std::string_view other, size_type times_n) -> std::string& {
    if (other.empty() || (times_n == 0)) [[unlikely]] {
        return s;
    }

    size_type require_size = s.size() + other.size() * times_n;
    if (s.capacity() >= require_size) {
        size_type origin_size = s.size();
        s.resize(require_size);
        std::memmove(s.data() + (other.size() * times_n), s.data(), origin_size);
        for (size_type i = 0; i < times_n; i++) {
            std::memcpy(s.data() + (other.size() * i), other.data(), other.size());
        }

        return s;
    }

    // 反正要分配新内存，还不如创建新对象
    std::string result;
    result.resize(require_size);
    for (size_type i = 0; i < times_n; i++) {
        std::memcpy(result.data() + (other.size() * i), other.data(), other.size());
    }
    std::memcpy(result.data() + (other.size() * times_n), s.data(), s.size());

    // result 赋值给 s
    s = std::move(result);

    return s;
}

auto str::insert(std::string_view s, size_type pos, std::string_view other, size_type times_n) -> std::string {
    if (other.empty() || (times_n == 0)) [[unlikely]] {
        return std::string{s};
    }

    if (pos >= s.size()) {
        pos = s.size();
    }

    std::string result;
    result.reserve(other.size() + (s.size() * times_n));
    result.append(std::string_view{s.data(), pos});
    for (size_type index = 0; index < times_n; index++) {
        result.append(other);
    }
    result.append(std::string_view{s.data() + pos, s.size() - pos});
    return result;
}

auto str::insert(std::string_view s, size_type pos, value_type ch, size_type times_n) -> std::string {
    if (times_n == 0) [[unlikely]] {
        return std::string{s};
    }

    if (pos >= s.size()) {
        pos = s.size();
    }

    std::string result;
    result.reserve(s.size() + times_n);
    result.append(s.data(), pos);
    result.append(times_n, ch);
    result.append(s.data() + pos, s.size() - pos);
    return result;
}

auto str::insert_inplace(std::string& s, size_type pos, std::string_view other, size_type times_n) -> std::string& {
    if (other.empty() || (times_n == 0)) [[unlikely]] {
        return s;
    }

    size_type count = 0;
    s = str::insert(s, pos, [times_n, &count, &other]() -> std::optional<std::string_view> {
        if (count >= times_n) {
            return {};
        }

        count++;
        return other;
    });

    return s;
}

auto str::insert_inplace(std::string& s, size_type pos, value_type ch, size_type times_n) -> std::string& {
    if (times_n == 0) [[unlikely]] {
        return s;
    }

    if (pos >= s.size()) {
        s.append(times_n, ch);
        return s;
    }

    if (s.capacity() > (s.size() + times_n)) {
        s.resize(s.size() + times_n);
        std::memmove(s.data() + pos + times_n, s.data() + pos, (s.size() - pos));
        std::fill(s.data() + pos, s.data() + pos + times_n, ch);
        return s;
    }

    std::string result;
    result.reserve(s.size() + times_n);
    result.append(s.data(), pos);
    result.append(times_n, ch);
    result.append(s.data() + pos, s.size() - pos);
    s = std::move(result);
    return s;
}

auto str::icompare(std::string_view s, std::string_view other) -> int {
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

auto str::icompare(std::string_view s, std::string_view other, size_type max_n) -> int {
    if (max_n < s.size()) {
        s = s.substr(0, max_n);
    }

    if (max_n < other.size()) {
        other = other.substr(0, max_n);
    }

    return str::icompare(s, other);
}

auto str::iequals(std::string_view s, std::string_view other) -> bool {
    if (s.size() != other.size()) {
        return false;
    }

    return strncasecmp(s.data(), other.data(), s.size()) == 0;
}

auto str::iequals(std::string_view s, std::string_view other, size_type max_n) -> bool {
    if (max_n < s.size()) {
        s = s.substr(0, max_n);
    }

    if (max_n < other.size()) {
        other = other.substr(0, max_n);
    }

    return str::iequals(s, other);
}

auto str::wildcmp(const_pointer s, const_pointer pattern) -> bool {
    const_pointer cp = nullptr;
    const_pointer mp = nullptr;

    // 非星号
    while ((*s) && (*pattern != '*')) {
        // 如果遇到不匹配的情况
        if ((*pattern != *s) && (*pattern != '?')) {
            return false;
        }

        // 字符相等或者?被匹配
        pattern++;
        s++;
    }

    while (*s) {
        // 星号模式匹配任何字符
        if (*pattern == '*') {
            if (!*++pattern) {
                return true;
            }
            mp = pattern;
            cp = s + 1;
        } else if ((*pattern == *s) || (*pattern == '?')) {
            pattern++;
            s++;
        } else {
            pattern = mp;
            s = cp++;
        }
    }

    while (*pattern == '*') {
        pattern++;
    }
    return !*pattern;
}

auto str::wildcmp(std::string_view s, std::string_view pattern) -> bool {
    if (pattern.empty()) {
        return s.empty();
    }

    const_pointer s_ptr = s.data();
    const_pointer p_ptr = pattern.data();
    const_pointer s_end = s.data() + s.size();
    const_pointer p_end = pattern.data() + pattern.size();
    if (s_ptr < s_end) {
        while (*p_ptr != '*') {
            if ((*p_ptr != *s_ptr) && (*p_ptr != '?')) {
                return false;
            }

            p_ptr++;
            s_ptr++;

            // 模式串结束了，但是还有更多字符没匹配完
            if (p_ptr >= p_end) {
                return (s_ptr >= s_end);
            }
        }
    }

    const_pointer ms = nullptr;
    const_pointer mp = nullptr;
    while (s_ptr < s_end) {
        // 遇到星号其实表示星号后的字符要匹配
        if (*p_ptr == '*') {
            ++p_ptr;

            // 星号是最后一个模式，那么星号一定能匹配后续任何字符
            if (p_ptr >= p_end) {
                return true;
            }

            // 星号后面还有更多字符，那么先记忆回朔的位置
            mp = p_ptr;
            ms = s_ptr + 1;
            continue;
        }

        // 非星号的匹配方式
        if ((*p_ptr == *s_ptr) || (*p_ptr == '?')) {
            p_ptr++;
            s_ptr++;

            // 如果模式串还未结束，尝试继续匹配后续部分
            if (p_ptr < p_end) {
                continue;
            }

            // 如果模式串和被匹配串同时结束
            if (s_ptr >= s_end) {
                return true;
            }

            // 如果模式串结束了，但是被匹配串未结束，尝试回退后继续匹配
        }

        // 当前不匹配，回退继续查找
        p_ptr = mp;
        s_ptr = ms++;
    }

    // 到这里，被匹配串已经全部吃掉，检查模式串后续是否是多余的字符
    while (p_ptr < p_end) {
        if (*p_ptr == '*') {
            p_ptr++;
            continue;
        }
        break;
    }

    // 如果模式串中的星号吃掉后，还有多余部分，说明不匹配
    return (p_ptr >= p_end);
}

auto str::iwildcmp(const_pointer s, const_pointer pattern) -> bool {

    const_pointer cp = nullptr;
    const_pointer mp = nullptr;

    // 非星号
    while ((*s) && (*pattern != '*')) {
        // 如果遇到不匹配的情况
        if ((std::tolower(*pattern) != std::tolower(*s)) && (*pattern != '?')) {
            return false;
        }

        // 字符相等或者?被匹配
        pattern++;
        s++;
    }

    while (*s) {
        // 星号模式匹配任何字符
        if (*pattern == '*') {
            if (!*++pattern) {
                return true;
            }
            mp = pattern;
            cp = s + 1;
        } else if (std::tolower(*pattern) == std::tolower(*s) || (*pattern == '?')) {
            pattern++;
            s++;
        } else {
            pattern = mp;
            s = cp++;
        }
    }

    while (*pattern == '*') {
        pattern++;
    }
    return !*pattern;
}

auto str::iwildcmp(std::string_view s, std::string_view pattern) -> bool {
    if (pattern.empty()) {
        return s.empty();
    }

    const_pointer s_ptr = s.data();
    const_pointer p_ptr = pattern.data();
    const_pointer s_end = s.data() + s.size();
    const_pointer p_end = pattern.data() + pattern.size();
    if (s_ptr < s_end) {
        while (*p_ptr != '*') {
            if ((std::tolower(*p_ptr) != std::tolower(*s_ptr)) && (*p_ptr != '?')) {
                return false;
            }

            p_ptr++;
            s_ptr++;

            // 模式串结束了，但是还有更多字符没匹配完
            if (p_ptr >= p_end) {
                return (s_ptr >= s_end);
            }
        }
    }

    const_pointer ms = nullptr;
    const_pointer mp = nullptr;
    while (s_ptr < s_end) {
        // 遇到星号其实表示星号后的字符要匹配
        if (*p_ptr == '*') {
            ++p_ptr;

            // 星号是最后一个模式，那么星号一定能匹配后续任何字符
            if (p_ptr >= p_end) {
                return true;
            }

            // 星号后面还有更多字符，那么先记忆回朔的位置
            mp = p_ptr;
            ms = s_ptr + 1;
            continue;
        }

        // 非星号的匹配方式
        if ((std::tolower(*p_ptr) == (*s_ptr)) || (*p_ptr == '?')) {
            p_ptr++;
            s_ptr++;

            // 如果模式串还未结束，尝试继续匹配后续部分
            if (p_ptr < p_end) {
                continue;
            }

            // 如果模式串和被匹配串同时结束
            if (s_ptr >= s_end) {
                return true;
            }

            // 如果模式串结束了，但是被匹配串未结束，尝试回退后继续匹配
        }

        // 当前不匹配，回退继续查找
        p_ptr = mp;
        s_ptr = ms++;
    }

    // 到这里，被匹配串已经全部吃掉，检查模式串后续是否是多余的字符
    while (p_ptr < p_end) {
        if (*p_ptr == '*') {
            p_ptr++;
            continue;
        }
        break;
    }

    // 如果模式串中的星号吃掉后，还有多余部分，说明不匹配
    return (p_ptr >= p_end);
}

auto str::contains(std::string_view s, std::string_view other) -> bool {
    if (other.empty()) {
        return true;
    }

    if (s.empty()) {
        return false;
    }

    return std::search(s.begin(), s.end(), other.begin(), other.end(), [](value_type a, value_type b) -> bool {
        return a == b;
    }) != s.cend();
}

auto str::contains(std::string_view s, value_type ch) -> bool {
    return contains(s, std::string_view{&ch, 1});
}

auto str::contains(std::string_view s, const char_match_proc& proc) -> bool {
    return std::find_if(s.begin(), s.end(), proc) != s.cend();
}

auto str::contains(std::string_view s, const charset_type& charset) -> bool {
    return std::find_if(s.begin(), s.end(), [&charset](value_type ch) -> bool {
        return charset.get(ch);
    }) != s.cend();
}

auto str::contains(std::string_view s, const std::regex& pattern) -> bool {
    std::regex_iterator<std::string_view::const_iterator> end;
    std::regex_iterator<std::string_view::const_iterator> itr(s.begin(), s.end(), pattern);
    return (itr != end);
}

auto str::icontains(std::string_view s, std::string_view other) -> bool {
    if (other.empty()) {
        return true;
    }

    if (s.empty()) {
        return false;
    }

    return std::search(s.begin(), s.end(), other.begin(), other.end(), [](value_type a, value_type b) -> bool {
        return std::tolower(a) == std::tolower(b);
    }) != s.cend();
}

auto str::icontains(std::string_view s, value_type ch) -> bool {
    return icontains(s, std::string_view{&ch, 1});
}

auto str::count(std::string_view s, std::string_view other) -> size_type {
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

auto str::count(std::string_view s, const char_match_proc& proc) -> size_type {
    size_type count = 0;
    for (value_type ch : s) {
        if (proc(ch)) {
            count++;
        }
    }
    return count;
}

auto str::count(std::string_view s, value_type ch) -> size_type {
    size_type count = 0;
    for (auto elem_ch : s) {
        count += ((elem_ch == ch) ? 1 : 0);
    }
    return count;
}

auto str::count(std::string_view s, const charset_type& charset) -> size_type {
    if (s.empty()) {
        return false;
    }

    return count(s, [&charset](value_type ch) -> bool {
        return charset.get(ch);
    });
}

auto str::count(std::string_view s, const std::regex& pattern) -> size_type {
    std::regex_iterator<std::string_view::const_iterator> end;
    size_type matched_count = 0;
    for (std::regex_iterator<std::string_view::const_iterator> itr(s.begin(), s.end(), pattern); itr != end; ++itr) {
        matched_count++;
    }

    return matched_count;
}

auto str::prefix(std::string_view s, std::string_view other) -> size_type {
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

auto str::has_prefix(std::string_view s, value_type ch) -> bool {
    if (s.empty()) {
        return false;
    }

    return s[0] == ch;
}

auto str::has_prefix(std::string_view s, std::string_view prefix) -> bool {
    return str::prefix(s, prefix) == prefix.size();
}

auto str::starts_with(std::string_view s, value_type ch) -> bool {
    return has_prefix(s, ch);
}

auto str::starts_with(std::string_view s, std::string_view prefix) -> bool {
    return has_prefix(s, prefix);
}

auto str::remove_prefix_view(std::string_view s, std::string_view prefix) -> std::string_view {
    if (!has_prefix(s, prefix)) {
        return s;
    }

    return std::string_view{s.data() + prefix.size(), s.size() - prefix.size()};
}

auto str::remove_prefix_view(std::string_view s, value_type prefix) -> std::string_view {
    return remove_prefix_view(s, {&prefix, 1});
}

auto str::remove_prefix(std::string_view s, std::string_view prefix) -> std::string {
    return std::string{remove_prefix_view(s, prefix)};
}

auto str::remove_prefix(std::string_view s, value_type prefix) -> std::string {
    return std::string{remove_prefix_view(s, prefix)};
}

auto str::remove_prefix_inplace(std::string& s, std::string_view prefix) -> std::string& {
    s = remove_prefix_view(s, prefix);
    return s;
}

auto str::remove_prefix_inplace(std::string& s, value_type prefix) -> std::string& {
    s = remove_prefix_view(s, prefix);
    return s;
}

auto str::suffix(std::string_view s, std::string_view other) -> size_type {
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

auto str::has_suffix(std::string_view s, value_type ch) -> bool {
    if (s.empty()) {
        return false;
    }

    return s.back() == ch;
}

auto str::has_suffix(std::string_view s, std::string_view suffix) -> bool {
    return str::suffix(s, suffix) == suffix.size();
}

auto str::ends_with(std::string_view s, value_type ch) -> bool {
    return has_suffix(s, ch);
}

auto str::ends_with(std::string_view s, std::string_view suffix) -> bool {
    return has_suffix(s, suffix);
}

auto str::remove_suffix_view(std::string_view s, std::string_view suffix) -> std::string_view {
    if (!has_suffix(s, suffix)) {
        return s;
    }

    return std::string_view{s.data(), s.size() - suffix.size()};
}

auto str::remove_suffix_view(std::string_view s, value_type ch) -> std::string_view {
    return remove_suffix_view(s, {&ch, 1});
}

auto str::remove_suffix(std::string_view s, std::string_view suffix) -> std::string {
    return std::string{remove_suffix_view(s, suffix)};
}

auto str::remove_suffix(std::string_view s, value_type ch) -> std::string {
    return std::string{remove_suffix_view(s, ch)};
}

auto str::remove_suffix_inplace(std::string& s, std::string_view suffix) -> std::string& {
    s = remove_suffix_view(s, suffix);
    return s;
}

auto str::remove_suffix_inplace(std::string& s, value_type ch) -> std::string& {
    s = remove_suffix_view(s, ch);
    return s;
}

auto str::starts_with_spaces(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    return std::isspace(s.front());
}

auto str::ends_with_spaces(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    return std::isspace(s.back());
}

auto str::starts_with_margin(std::string_view s, value_type margin) -> bool {
    if (s.empty()) {
        return false;
    }

    const_pointer ptr = s.data();
    while (ptr < (s.data() + s.size())) {
        if (!std::isspace(*ptr)) {
            return (*ptr == margin);
        }
        ptr++;
    }

    return false;
}

auto str::next_char(std::string_view s, size_type& pos, value_type ch) -> std::optional<size_type> {
    return next_char(s, pos, [ch](value_type c) -> bool {
        return c == ch;
    });
}

auto str::next_char(std::string_view s, size_type& pos, const charset_type& charset) -> std::optional<size_type> {
    return next_char(s, pos, [&charset](value_type ch) -> bool {
        return charset(ch);
    });
}

auto str::next_char(std::string_view s, size_type& pos, std::string_view charset) -> std::optional<size_type> {
    return next_char(s, pos, charset_type{charset});
}

auto str::next_char(std::string_view s, size_type& pos, const char_match_proc& proc) -> std::optional<size_type> {
    if (pos >= s.size()) {
        pos = s.size();
        return npos;
    }

    const_pointer ptr = s.data() + pos;
    while (ptr < (s.data() + s.size())) {
        if (proc(*ptr)) {
            pos = ptr - s.data();
            return pos++;
        }
        ptr++;
    }

    pos = ptr - s.data();
    return npos;
}

auto str::prev_char(std::string_view s, size_type& pos, value_type ch) -> std::optional<size_type> {
    return prev_char(s, pos, [&ch](value_type c) -> bool {
        return c == ch;
    });
}

auto str::prev_char(std::string_view s, size_type& pos, const charset_type& charset) -> std::optional<size_type> {
    return prev_char(s, pos, [&charset](value_type ch) -> bool {
        return charset.get(ch);
    });
}

auto str::prev_char(std::string_view s, size_type& pos, std::string_view charset) -> std::optional<size_type> {
    return prev_char(s, pos, charset_type{charset});
}

auto str::prev_char(std::string_view s, size_type& pos, const char_match_proc& proc) -> std::optional<size_type> {
    if (pos == 0) {
        return std::nullopt;
    }

    if (pos > s.size()) {
        pos = s.size();
    }

    const_pointer ptr = s.data() + pos;
    while (ptr > s.data()) {
        if (proc(*(ptr - 1))) {
            pos = (ptr - 1) - s.data();
            return pos;
        }
        ptr--;
    }

    pos = 0;
    return std::nullopt;
}

auto str::next_string_range(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<range_type> {
    // 查找空串会导致大量无法控制的行为(UB)，因此考虑不支持更好
    if (substr.empty()) {
        pos = s.size();
        return std::nullopt;
    }

    if (pos >= s.size()) {
        pos = s.size();
        return std::nullopt;
    }

    auto next_pos = s.find(substr, pos);
    if (next_pos == std::string_view::npos) {
        pos = s.size();
        return std::nullopt;
    }

    pos = next_pos + substr.size();
    return range_type{next_pos, substr.size()};
}

auto str::next_string_view(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<std::string_view> {
    auto range = next_string_range(s, pos, substr);
    if (!range) {
        return std::nullopt;
    }
    return std::string_view{s.data() + range->begin_pos(), range->size()};
}

auto str::next_string(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<std::string> {
    auto view = next_string_view(s, pos, substr);
    if (!view) {
        return std::nullopt;
    }
    return std::string{*view};
}

auto str::prev_string_range(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<range_type> {
    // 查找空串会导致大量无法控制的行为(UB)，因此考虑不支持更好
    if (substr.empty()) {
        pos = 0;
        return std::nullopt;
    }

    if (pos <= 0) {
        pos = 0;
        return std::nullopt;
    }

    if (pos > s.size()) {
        pos = s.size();
    }

    auto start = s.rfind(substr, pos - 1);
    if (start == std::string_view::npos) {
        pos = 0;
        return std::nullopt;
    }

    pos = start;
    return range_type{start, substr.size()};
}

auto str::prev_string_view(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<std::string_view> {
    auto range = prev_string_range(s, pos, substr);
    if (!range) {
        return std::nullopt;
    }

    return std::string_view{s.data() + range->pos, range->len};
}

auto str::prev_string(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<std::string> {
    auto view = prev_string_view(s, pos, substr);
    if (!view) {
        return std::nullopt;
    }

    return std::string{*view};
}

auto str::next_eol_range(std::string_view s, size_type& pos) -> std::optional<range_type> {
    if (pos >= s.size()) {
        pos = s.size();
        return range_type{pos, 0};
    }

    const_pointer endptr = (s.data() + s.size());
    const_pointer ptr = s.data() + pos;
    while (ptr < endptr) {
        if (*ptr == '\r') [[unlikely]] {
            // 遇到 xxx\r\nyyyy
            if (((ptr + 1) < endptr) && (*(ptr + 1) == '\n')) {
                pos = static_cast<size_type>((ptr + 2) - s.data());
                return range_type{static_cast<size_type>(ptr - s.data()), 2};
            }

            // 遇到 xxx\ryyyy
            pos = static_cast<size_type>((ptr + 1) - s.data());
            return range_type{static_cast<size_type>(ptr - s.data()), 1};
        }

        // 遇到 xxx\nyyyy
        if (*ptr == '\n') {
            pos = static_cast<size_type>((ptr + 1) - s.data());
            return range_type{static_cast<size_type>(ptr - s.data()), 1};
        }

        ptr++;
    }

    pos = s.size();
    return range_type{pos, 0};
}

auto str::next_eol_view(std::string_view s, size_type& pos) -> std::optional<std::string_view> {
    auto range = next_eol_range(s, pos);
    if (!range) {
        return std::nullopt;
    }
    return std::string_view{s.data() + range->begin_pos(), range->size()};
}

auto str::next_eol(std::string_view s, size_type& pos) -> std::optional<std::string> {
    auto view = next_eol_view(s, pos);
    if (!view) {
        return std::nullopt;
    }
    return std::string{*view};
}

auto str::prev_eol_range(std::string_view s, size_type& pos) -> std::optional<range_type> {
    if (pos == 0) {
        return std::nullopt;
    }

    if (pos > s.size()) {
        pos = s.size();
    }

    while (pos > 0) {
        --pos;
        if (s[pos] == '\n') {
            if ((pos <= 0) || (s[pos - 1] != '\r')) {
                return range_type{pos, 1};
            }

            return range_type{--pos, 2};
        }

        if (s[pos] == '\r') {
            return range_type{pos, 1};
        }
    }

    return std::nullopt;
}

auto str::prev_eol_view(std::string_view s, size_type& pos) -> std::optional<std::string_view> {
    auto range = prev_eol_range(s, pos);
    if (!range) {
        return std::nullopt;
    }

    return s.substr(range->begin_pos(), range->size());
}

auto str::prev_eol(std::string_view s, size_type& pos) -> std::optional<std::string> {
    auto view = prev_eol_view(s, pos);
    if (!view) {
        return std::nullopt;
    }
    return std::string{*view};
}

auto str::ends_with_eol(std::string_view s) -> bool {
    return has_eol_suffix(s);
}

auto str::has_eol_suffix(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }
    return (s.back() == '\n' || s.back() == '\r');
}

auto str::eol_suffix(std::string_view s) -> size_type {
    if (s.empty()) {
        return 0;
    }

    if (s.back() == '\r') {
        return 1;
    }

    if (s.back() == '\n') {
        if (s.size() <= 1) {
            return 1;
        }

        if (s[s.size() - 2] == '\r') {
            return 2;
        }

        return 1;
    }

    return 0;
}

auto str::remove_eol_suffix_range(std::string_view s) -> range_type {
    auto len = eol_suffix(s);
    if (len == 0) {
        return range_type{0, s.size()};
    }

    return range_type{0, s.size() - len};
}

auto str::remove_eol_suffix_view(std::string_view s) -> std::string_view {
    auto range = remove_eol_suffix_range(s);
    return {s.data() + range.begin_pos(), range.size()};
}

auto str::remove_eol_suffix(std::string_view s) -> std::string {
    return std::string{remove_eol_suffix_view(s)};
}

auto str::remove_eol_suffix_inplace(std::string& s) -> std::string& {
    auto len = eol_suffix(s);
    s.resize(s.size() - len);
    return s;
}

auto str::next_regex_range(std::string_view s, size_type& pos, const std::regex& pattern) -> range_type {
    if (pos >= s.size()) {
        pos = s.size();
        return range_type{};
    }

    auto itr = s.begin();
    std::advance(itr, pos);
    std::match_results<std::string_view::const_iterator> result;
    if (!std::regex_search(itr, s.end(), result, pattern)) {
        pos = s.size();
        return range_type{};
    }

    auto range = range_type{static_cast<size_type>(result[0].first - s.data()),
        static_cast<size_type>(result[0].second - result[0].first)};
    pos = range.end_pos();
    return range;
}

auto str::next_regex_view(std::string_view s, size_type& pos, const std::regex& pattern) -> std::string_view {
    auto range = next_regex_range(s, pos, pattern);
    if (range.empty()) {
        return {};
    }

    return {s.data() + range.pos, range.len};
}

auto str::next_regex(std::string_view s, size_type& pos, const std::regex& pattern) -> std::string {
    return std::string{next_regex_view(s, pos, pattern)};
}

auto str::next_regex(std::string_view s, size_type& pos, std::string_view pattern) -> std::string {
    return std::string{next_regex_view(s, pos, std::regex{pattern.begin(), pattern.end()})};
}

// auto str::prev_regex_range(std::string_view s, size_type& pos, const std::regex& pattern) -> range_type;
// auto str::prev_regex_view(std::string_view s, size_type& pos, const std::regex& pattern) -> std::string_view;
// auto str::prev_regex(std::string_view s, size_type& pos, const std::regex& pattern) -> std::string;
// auto str::prev_regex(std::string_view s, size_type& pos, std::string_view pattern) -> std::string;

// auto str::next_pathsep_range(std::string_view s, size_type& pos) -> range_type;
// auto str::next_pathsep_view(std::string_view s, size_type& pos) -> std::string_view;
// auto str::next_pathsep(std::string_view s, size_type& pos) -> std::string;

// auto str::next_searchpathsep_range(std::string_view s, size_type& pos) -> range_type;
// auto str::next_searchpathsep_view(std::string_view s, size_type& pos) -> std::string_view;
// auto str::next_searchpathsep(std::string_view s, size_type& pos) -> std::string;

// auto str::prev_proc_range(std::string_view s, size_type& pos, const range_search_proc& proc) -> range_type;
// auto str::prev_proc_view(std::string_view s, size_type& pos, const range_search_proc& proc) -> std::string_view;
// auto str::prev_proc(std::string_view s, size_type& pos, const range_search_proc& proc) -> std::string;

auto str::is_lower(std::string_view s) -> bool {
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

auto str::is_upper(std::string_view s) -> bool {
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

auto str::is_title(std::string_view s) -> bool {
    bool result = true;
    str::foreach_words(s, [&s, &result](std::string_view item) -> int {
        const_pointer ptr = item.data();
        while (ptr < (s.data() + s.size())) {
            if (std::isalpha(*ptr)) {
                if (std::islower(*ptr)) {
                    result = false;
                    return -1;
                }
                break;
            }
            ptr++;
        }
        return 0;
    });

    return result;
}

auto str::is_capitalize(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    return std::isupper(s[0]);
}

auto str::is_digit(std::string_view s) -> bool {
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

auto str::is_xdigit(std::string_view s) -> bool {
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

auto str::is_ascii(std::string_view s) -> bool {
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

auto str::is_alpha(std::string_view s) -> bool {
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

auto str::is_alnum(std::string_view s) -> bool {
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

auto str::is_alnumul(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (!std::isalnum(*ptr) || (*ptr == '_')) {
            return false;
        }
    }
    return true;
}

auto str::is_space(std::string_view s) -> bool {
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

auto str::is_blank(std::string_view s) -> bool {
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

auto str::is_print(std::string_view s) -> bool {
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

auto str::is_graph(std::string_view s) -> bool {
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

auto str::is_identifier(std::string_view s) -> bool {
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

auto str::is_literal_bool(std::string_view s) -> bool {
    return str::is_literal_true(s) || str::is_literal_false(s);
}

auto str::is_literal_true(std::string_view s) -> bool {
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

auto str::is_literal_false(std::string_view s) -> bool {
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

auto str::is_literal_integer(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    const_pointer p = s.data();
    if ((*p == '+') || (*p == '-')) {
        p++;
    }

    if (p >= (s.data() + s.size())) {
        return false;
    }

    while (p < (s.data() + s.size())) {
        if (!std::isdigit(*p)) {
            return false;
        }

        p++;
    }

    return true;
}

auto str::is_literal_real(std::string_view s) -> bool {
    if (s.empty()) {
        return false;
    }

    const_pointer p = s.data();
    const_pointer end = s.data() + s.size();

    // 前缀加减号
    if ((*p == '+') || (*p == '-')) {
        p++;
    }

    // 数字部分
    const_pointer int_start = nullptr;
    while (p < end) {
        if (!std::isdigit(*p)) {
            break;
        }

        if (int_start == nullptr) [[unlikely]] {
            int_start = p;
        }

        p++;
    };

    if (p >= end) {
        return (int_start != nullptr);
    }

    const_pointer decimal_start = nullptr;
    if (*p == '.') {
        p++;

        while (p < end) {
            if (!std::isdigit(*p)) {
                break;
            }

            if (decimal_start == nullptr) [[unlikely]] {
                decimal_start = p;
            }

            p++;
        }

        if (p >= end) {
            return (int_start != nullptr) || (decimal_start != nullptr);
        }
    }

    if ((int_start == nullptr) && (decimal_start == nullptr)) {
        return false;
    }

    const_pointer pow_start = nullptr;
    if ((*p == 'e') || (*p == 'E')) {
        p++;

        if (p >= end) {
            return false;
        }

        if ((*p == '+') || (*p == '-')) {
            p++;
        }

        while (p < end) {
            if (!std::isdigit(*p)) {
                break;
            }

            if (pow_start == nullptr) [[unlikely]] {
                pow_start = p;
            }

            p++;
        }

        if (p >= end) {
            return (pow_start != nullptr);
        }
    }

    return (p >= end);
}

auto str::is_all(std::string_view s, const charset_type& set) -> bool {
    for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ptr++) {
        if (!set.get(*ptr)) {
            return false;
        }
    }
    return true;
}

auto str::has_any(std::string_view s, const charset_type& set) -> bool {
    for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ptr++) {
        if (set.get(*ptr)) {
            return true;
        }
    }
    return false;
}

auto str::take_left_view(std::string_view s, size_type n) -> std::string_view {
    if (n == 0) {
        return {};
    }

    if (n > s.size()) {
        return s;
    }

    return std::string_view{s.substr(0, n)};
}

auto str::take_right_view(std::string_view s, size_type n) -> std::string_view {
    if (n == 0) {
        return {};
    }

    if (n > s.size()) {
        return s;
    }

    return std::string_view{s.substr(s.size() - n, n)};
}

auto str::take_mid_view(std::string_view s, size_type pos, size_type n) -> std::string_view {
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

auto str::take_range_view(std::string_view s, size_type begin_pos, size_type end_pos) -> std::string_view {
    return str::take_mid_view(s, begin_pos, begin_pos + end_pos);
}

auto str::take_range_view(std::string_view s, range_type range) -> std::string_view {
    return take_range_view(s, range.begin_pos(), range.end_pos());
}

auto str::take_view(std::string_view s, size_type pos, ssize_type offset) -> std::string_view {
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

    // 如果 rpos 太大
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

auto str::take_view(std::string_view s, size_type pos) -> std::string_view {
    if (pos >= s.size()) {
        return {};
    }

    return s.substr(pos);
}

auto str::take_left(std::string_view s, size_type n) -> std::string {
    return std::string{take_left_view(s, n)};
}

auto str::take_right(std::string_view s, size_type n) -> std::string {
    return std::string{take_right_view(s, n)};
}

auto str::take_mid(std::string_view s, size_type pos, size_type n) -> std::string {
    return std::string{take_mid_view(s, pos, n)};
}

auto str::take_range(std::string_view s, size_type begin_pos, size_type end_pos) -> std::string {
    return std::string{take_range_view(s, begin_pos, end_pos)};
}

auto str::take_range(std::string_view s, range_type range) -> std::string {
    return take_range(s, range.begin_pos(), range.end_pos());
}

auto str::take(std::string_view s, size_type pos, ssize_type offset) -> std::string {
    return std::string{take_view(s, pos, offset)};
}

auto str::take(std::string_view s, size_type pos) -> std::string {
    return std::string{take_view(s, pos)};
}

auto str::take_left_inplace(std::string& s, size_type n) -> std::string& {
    if (n >= s.size()) {
        return s;
    }

    s.resize(n);
    return s;
}

auto str::take_right_inplace(std::string& s, size_type n) -> std::string& {
    if (n == 0) {
        s.resize(0);
        return s;
    }

    if (n >= s.size()) {
        return s;
    }

    std::memmove(s.data() + (s.size() - n), s.data(), n);
    s.resize(n);
    return s;
}

auto str::take_mid_inplace(std::string& s, size_type pos, size_type n) -> std::string& {
    if (pos >= s.size()) {
        s.resize(0);
        return s;
    }

    if (n == 0) {
        s.resize(0);
        return s;
    }

    if (n > (s.size() - pos)) {
        std::memmove(s.data(), (s.data() + pos), (s.size() - pos));
        s.resize(s.size() - pos);
        return s;
    }

    std::memmove(s.data(), (s.data() + pos), n);
    s.resize(n);
    return s;
}

auto str::take_range_inplace(std::string& s, size_type begin_pos, size_type end_pos) -> std::string& {
    if (begin_pos == end_pos) [[unlikely]] {
        s.resize(0);
        return s;
    }

    if (begin_pos > end_pos) [[unlikely]] {
        std::swap(begin_pos, end_pos);
    }

    if (begin_pos >= s.size()) [[unlikely]] {
        s.resize(0);
        return s;
    }

    if (end_pos >= s.size()) {
        std::memmove(s.data(), (s.data() + begin_pos), (s.size() - begin_pos));
        s.resize(s.size() - begin_pos);
        return s;
    }

    std::memmove(s.data(), (s.data() + begin_pos), (end_pos - begin_pos));
    s.resize(end_pos - begin_pos);
    return s;
}

auto str::take_range_inplace(std::string& s, range_type range) -> std::string& {
    return take_range_inplace(s, range.begin_pos(), range.end_pos());
}

auto str::take_inplace(std::string& s, size_type pos, ssize_type offset) -> std::string& {
    if (offset == 0) {
        s.resize(0);
        return s;
    }

    if (offset > 0) {
        return take_mid_inplace(s, pos, offset);
    }

    return take_mid_inplace(s, (pos + offset + 1), offset);
}

auto str::take_inplace(std::string& s, size_type pos) -> std::string& {
    if (pos >= s.size()) {
        s.resize(0);
        return s;
    }

    std::memmove(s.data(), s.data() + pos, (s.size() - pos));
    s.resize(s.size() - pos);
    return s;
}

auto str::take_before_view(std::string_view s, range_type sep_range, bool with_sep) -> std::string_view {
    if (s.empty()) {
        return s;
    }

    if (sep_range.pos >= s.size()) {
        return s;
    }

    if ((sep_range.pos + sep_range.len) > s.size()) {
        sep_range.len = s.size() - sep_range.pos;
    }

    if (with_sep) {
        return {s.data(), sep_range.end_pos()};
    }

    return {s.data(), sep_range.begin_pos()};
}

auto str::take_after_view(std::string_view s, range_type sep_range, bool with_sep) -> std::string_view {
    if (s.empty()) {
        return s;
    }

    if (sep_range.pos >= s.size()) {
        return s;
    }

    if ((sep_range.pos + sep_range.len) > s.size()) {
        sep_range.len = s.size() - sep_range.pos;
    }

    if (with_sep) {
        return {s.data() + sep_range.begin_pos(), s.size() - sep_range.begin_pos()};
    }

    return {s.data() + sep_range.end_pos(), s.size() - sep_range.end_pos()};
}

auto str::take_before(std::string_view s, range_type sep_range, bool with_sep) -> std::string {
    return std::string{take_before_view(s, sep_range, with_sep)};
}

auto str::take_after(std::string_view s, range_type sep_range, bool with_sep) -> std::string {
    return std::string{take_after_view(s, sep_range, with_sep)};
}

auto str::drop_left_view(std::string_view s, size_type n) -> std::string_view {
    if (n > s.size()) {
        return {};
    }

    return s.substr(n);
}

auto str::drop_right_view(std::string_view s, size_type n) -> std::string_view {
    if (n > s.size()) {
        return {};
    }

    return s.substr(0, s.size() - n);
}

auto str::drop_left(std::string_view s, size_type n) -> std::string {
    return std::string{drop_left_view(s, n)};
}

auto str::drop_right(std::string_view s, size_type n) -> std::string {
    return std::string{drop_right_view(s, n)};
}

auto str::drop_mid(std::string_view s, size_type pos, size_type n) -> std::string {
    if ((n == 0) || (pos > s.size())) {
        return std::string{s};
    }

    if (pos >= s.size()) {
        return std::string{s};
    }

    if (n > (s.size() - pos)) {
        return std::string{s.substr(0, pos)};
    }

    std::string result;
    result.reserve(s.size() - n);
    result.append(s.data(), pos);
    result.append(s.data() + pos + n, (s.size() - pos - n));
    return result;
}

auto str::drop_range(std::string_view s, size_type begin_pos, size_type end_pos) -> std::string {
    if (end_pos <= begin_pos) {
        return {};
    }

    if (end_pos > s.size()) {
        end_pos = s.size();
    }

    if (begin_pos >= s.size()) {
        return {};
    }

    std::string result;
    result.reserve(s.size() - (end_pos - begin_pos));

    if (begin_pos > 0) {
        result.append(s.data(), begin_pos);
    }

    if ((s.size() - end_pos) > 0) {
        result.append(s.data() + end_pos, (s.size() - end_pos));
    }
    return result;
}

auto str::drop_range(std::string_view s, range_type range) -> std::string {
    return drop_range(s, range.begin_pos(), range.end_pos());
}

auto str::drop(std::string_view s, size_type pos, ssize_type offset) -> std::string {
    if (s.empty()) {
        return std::string{s};
    }

    if (offset > 0) {
        if (pos > s.size()) {
            return std::string{s};
        }

        if (offset >= static_cast<ssize_t>(s.size() - pos)) {
            return std::string{s.substr(0, pos)};
        }

        std::string result;
        result.append(s.data(), pos);
        result.append(s.data() + pos + offset, s.size() - (pos + offset));
        return result;
    }

    if (offset < 0) {
        size_type n = -offset;

        if (pos >= s.size()) {
            pos = s.size() - 1;
        }

        if (n > pos) {
            return std::string{s.substr(pos + 1)};
        }

        std::string result;
        result.append(s.substr(0, ((pos + 1) - n)));
        if ((pos + 1) < s.size()) {
            result.append(s.substr(pos + 1));
        }
        return result;
    }

    return std::string{s};
}

auto str::drop(std::string_view s, size_type pos) -> std::string {
    return str::drop(s, pos, s.size());
}

auto str::drop(std::string_view s, const charset_type& charset) -> std::string {
    if (s.empty()) {
        return {};
    }

    std::string result;
    for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ptr++) {
        if (!charset.get(*ptr)) {
            result.append(ptr, 1);
        }
    }

    return result;
}

auto str::drop_left_inplace(std::string& s, size_type n) -> std::string& {
    std::string_view range = drop_left_view(s, n);
    if (range.empty()) {
        s.resize(0);
        return s;
    }

    std::memmove(s.data(), range.data(), range.size());
    s.resize(range.size());
    return s;
}

auto str::drop_right_inplace(std::string& s, size_type n) -> std::string& {
    std::string_view range = drop_right_view(s, n);
    if (range.empty()) {
        s.resize(0);
        return s;
    }

    assert(s.data() == range.data());
    s.resize(range.size());
    return s;
}

auto str::drop_mid_inplace(std::string& s, size_type pos, size_type n) -> std::string& {
    s = drop_mid(s, pos, n);
    return s;
}

auto str::drop_range_inplace(std::string& s, size_type begin_pos, size_type end_pos) -> std::string& {
    s = drop_range(s, begin_pos, end_pos);
    return s;
}

auto str::drop_range_inplace(std::string& s, range_type range) -> std::string& {
    return drop_range_inplace(s, range.begin_pos(), range.end_pos());
}

auto str::drop_inplace(std::string& s, size_type pos, ssize_type offset) -> std::string& {
    s = drop(s, pos, offset);
    return s;
}

auto str::drop_inplace(std::string& s, size_type pos) -> std::string& {
    s = drop(s, pos);
    return s;
}

auto str::drop_inplace(std::string& s, const charset_type& charset) -> std::string& {
    s = drop(s, charset);
    return s;
}

auto str::drop_before_view(std::string_view s, range_type sep_range, bool with_sep) -> std::string_view {
    if (s.empty()) {
        return s;
    }

    if (sep_range.pos >= s.size()) {
        return s;
    }

    if ((sep_range.pos + sep_range.len) > s.size()) {
        sep_range.len = s.size() - sep_range.pos;
    }

    if (with_sep) {
        return {s.data() + sep_range.end_pos(), s.size() - sep_range.end_pos()};
    }

    return {s.data() + sep_range.begin_pos(), s.size() - sep_range.begin_pos()};
}

auto str::drop_after_view(std::string_view s, range_type sep_range, bool with_sep) -> std::string_view {
    if (s.empty()) {
        return s;
    }

    if (sep_range.pos >= s.size()) {
        return s;
    }

    if ((sep_range.pos + sep_range.len) > s.size()) {
        sep_range.len = s.size() - sep_range.pos;
    }

    if (with_sep) {
        return {s.data(), sep_range.begin_pos()};
    }

    return {s.data(), sep_range.end_pos()};
}

auto str::drop_before(std::string_view s, range_type sep_range, bool with_sep) -> std::string {
    return std::string{drop_before_view(s, sep_range, with_sep)};
}

auto str::drop_after(std::string_view s, range_type sep_range, bool with_sep) -> std::string {
    return std::string{drop_after_view(s, sep_range, with_sep)};
}

auto str::align_left(std::string_view s, size_type width, value_type ch) -> std::string {
    if (s.size() >= width) {
        return std::string{s};
    }

    std::string result;
    result.reserve(width);
    result.append(s);
    result.append(width - s.size(), ch);
    return result;
}

auto str::align_right(std::string_view s, size_type width, value_type ch) -> std::string {
    if (s.size() >= width) {
        return std::string{s};
    }

    std::string result;
    result.resize(width);
    std::fill(result.data(), result.data() + (width - s.size()), ch);
    std::memcpy(result.data() + (width - s.size()), s.data(), s.size());
    return result;
}

auto str::align_right(std::string_view s, size_type width, value_type ch, const view_consumer_proc& proc) -> void {
    if (s.size() >= width) {
        proc(s);
        return;
    }

    std::string result;
    result.resize(width);
    std::fill(result.data(), result.data() + (width - s.size()), ch);
    std::memcpy(result.data() + (width - s.size()), s.data(), s.size());
    proc(result);
    return;
}

auto str::align_center(std::string_view s, size_type width, value_type ch) -> std::string {
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

auto str::align_zfill(std::string_view s, size_type width, const view_consumer_proc& proc) -> void {
    if (s.empty()) {
        std::string r;
        r.resize(width, '0');
        proc(r);
        return;
    }

    if (s.size() >= width) {
        proc(s);
        return;
    }

    if ((s[0] != '+') && (s[0] != '-')) {
        str::align_right(s, width, '0', proc);
        return;
    }

    std::string result;
    result.reserve(width);
    result.append(s.data(), 1);
    result.resize((width - s.size() + 1), '0');
    result.append(s.data() + 1, static_cast<size_type>(s.size() - 1));

    proc(result);
    return;
}

auto str::align_zfill(std::string_view s, size_type width) -> std::string {
    if (s.empty()) {
        std::string r;
        r.resize(width, '0');
        return r;
    }

    if (s.size() >= width) {
        return std::string{s};
    }

    if ((s[0] != '+') && (s[0] != '-')) {
        return str::align_right(s, width, '0');
    }

    std::string result;
    result.reserve(width);
    result.append(s.data(), 1);
    result.resize((width - s.size() + 1), '0');
    result.append(s.data() + 1, static_cast<size_type>(s.size() - 1));

    return result;
}

auto str::align_left_inplace(std::string& s, size_type width, value_type ch) -> std::string& {
    s = align_left(s, width, ch);
    return s;
}

auto str::align_right_inplace(std::string& s, size_type width, value_type ch) -> std::string& {
    s = align_right(s, width, ch);
    return s;
}

auto str::align_center_inplace(std::string& s, size_type width, value_type ch) -> std::string& {
    s = align_center(s, width, ch);
    return s;
}

auto str::align_zfill_inplace(std::string& s, size_type width) -> std::string& {
    s = align_zfill(s, width);
    return s;
}

// auto str::foreach_lines(std::string_view s, bool keep_ends, const line_consumer_proc& proc) -> void {
//     size_type line_index = 0;
//     size_type line_start = 0;
//     size_type pos = 0;
//     while (pos < s.size()) {
//         range_type range = next_eol_range(s, pos);
//         if (range.empty()) {
//             break;
//         }
//
// size_type endpos = keep_ends ? range.end_pos() : range.begin_pos();
// if (proc(line_index++, s.substr(line_start, endpos - line_start)) != 0) {
//     return;
// }
// }
//
// if (pos < s.size()) {
//     proc(line_index++, s.substr(line_start, (s.size() - line_start)));
// }
// }

auto str::count_lines(std::string_view s) -> size_type {
    size_type count = 0;
    foreach_lines(s, true, [&count](size_type, std::string_view) -> int {
        count++;
        return 0;
    });
    return count;
}

auto str::lines_indentation(std::string_view s) -> size_type {
    size_type min_spaces = 0;
    foreach_lines(s, true, [&min_spaces](size_type, std::string_view line_text) -> int {
        size_type pos = 0;
        skip_spaces(line_text, pos);
        if (pos < min_spaces) {
            min_spaces = pos;
        }

        return 0;
    });
    return min_spaces;
}

auto str::numbering_lines(std::string_view s, size_type from_n) -> std::string {
    std::string result;
    result.reserve(s.size());
    foreach_lines(s, true, [&result, from_n](size_type line_index, std::string_view line_text) -> int {
        value_type buf[64]{0};
        int len = snprintf(buf, sizeof(buf), "%lu ", from_n + line_index);
        assert(len >= 0);
        result.append(std::string{buf, static_cast<size_type>(len)});
        result.append(line_text);
        return 0;
    });

    return result;
}

auto str::unnumbering_lines(std::string_view s) -> std::string {
    std::string result;
    result.reserve(s.size());
    foreach_lines(s, true, [&result](size_type, std::string_view line_text) -> int {
        const_pointer ptr = line_text.data();
        const_pointer end = line_text.data() + line_text.size();
        for (; ptr < end; ptr++) {
            if (!std::isdigit(*ptr)) {
                break;
            }
        }

        for (; ptr < end; ptr++) {
            if (!std::isspace(*ptr)) {
                break;
            }
        }

        result.append(std::string{ptr, static_cast<size_type>(end - ptr)});
        return 0;
    });

    return result;
}

auto str::numbering_lines_inplace(std::string& s, size_type from_n) -> std::string& {
    s = numbering_lines(s, from_n);
    return s;
}

auto str::unnumbering_lines_inplace(std::string& s) -> std::string& {
    s = unnumbering_lines(s);
    return s;
}

auto str::indent_lines(std::string_view s, size_type n) -> std::string {
    std::string result;
    result.reserve(s.size());
    foreach_lines(s, true, [&result, n](size_type, std::string_view line_text) -> int {
        result.append(n, ' ');
        result.append(line_text);
        return 0;
    });

    return result;
}

auto str::dedent_lines(std::string_view s, size_type n) -> std::string {
    std::string result;
    result.reserve(s.size());
    foreach_lines(s, true, [&result, n](size_type, std::string_view line_text) -> int {
        const_pointer ptr = line_text.data();
        const_pointer end = std::min((line_text.data() + line_text.size()), (line_text.data() + n));
        for (; ptr < end; ptr++) {
            if (!std::isspace(*ptr)) {
                break;
            }
        }

        result.append(ptr, static_cast<size_type>((line_text.data() + line_text.size()) - ptr));
        return 0;
    });

    return result;
}

auto str::align_indent_lines(std::string_view s, size_type n) -> std::string {
    std::string result;
    result.reserve(s.size());
    foreach_lines(s, true, [&result, n](size_type, std::string_view line_text) -> int {
        result.append(n, ' ');
        result.append(after_skip_spaces_view(line_text));
        return 0;
    });

    return result;
}

auto str::trim_indent_lines(std::string_view s) -> std::string {
    std::string result;
    result.reserve(s.size());
    foreach_lines(s, true, [&result](size_type, std::string_view line_text) -> int {
        result.append(after_skip_spaces_view(line_text));
        return 0;
    });

    return result;
}

auto str::simplify_indent_lines(std::string_view s) -> std::string {
    std::string result;
    result.reserve(s.size());

    // 计算最大缩减量
    size_type max_n = lines_indentation(s);
    if (max_n == 0) {
        result = s;
        return result;
    }

    foreach_lines(s, true, [&result, max_n](size_type, std::string_view line_text) -> int {
        result.append(line_text.substr(max_n));
        return 0;
    });

    return result;
}

auto str::indent_lines_inplace(std::string& s, size_type n) -> std::string& {
    s = indent_lines(s, n);
    return s;
}

auto str::dedent_lines_inplace(std::string& s, size_type n) -> std::string& {
    s = dedent_lines(s, n);
    return s;
}

auto str::align_indent_lines_inplace(std::string& s, size_type n) -> std::string& {
    s = align_indent_lines(s, n);
    return s;
}

auto str::trim_indent_lines_inplace(std::string& s) -> std::string& {
    s = trim_indent_lines(s);
    return s;
}

auto str::simplify_indent_lines_inplace(std::string& s) -> std::string& {
    s = simplify_indent_lines(s);
    return s;
}

auto str::margin_lines(std::string_view s, size_type n, value_type margin_ch) -> std::string {
    std::string result;
    result.reserve(s.size());
    foreach_lines(s, true, [&result, n, margin_ch](size_type, std::string_view line_text) -> int {
        result.append(n, ' ');
        result.append(1, margin_ch);
        result.append(line_text);
        return 0;
    });

    return result;
}

auto str::trim_margin_lines(std::string_view s, value_type margin_ch) -> std::string {
    std::string result;
    result.reserve(s.size());
    foreach_lines(s, true, [&result, margin_ch](size_type, std::string_view line_text) -> int {
        const_pointer ptr = line_text.data();
        const_pointer end = line_text.data() + line_text.size();
        for (; ptr < end; ptr++) {
            if (!std::isspace(*ptr)) {
                if ((ptr < end) && (*ptr == margin_ch)) {
                    ptr++;
                }
                break;
            }
        }

        result.append(ptr, static_cast<size_type>(end - ptr));
        return 0;
    });

    return result;
}

auto str::margin_lines_inplace(std::string& s, size_type n, value_type margin_ch) -> std::string& {
    s = margin_lines(s, n, margin_ch);
    return s;
}

auto str::trim_margin_lines_inplace(std::string& s, value_type margin_ch) -> std::string& {
    s = trim_margin_lines(s, margin_ch);
    return s;
}

auto str::count_words(std::string_view s) -> size_type {
    size_type count = 0;
    str::foreach_words(s, [&count](range_type) -> int {
        count++;
        return 0;
    });
    return count;
}

// auto str::first_word_view(std::string_view s) -> std::string_view {
//     size_type pos = 0;
//     return next_word_view(s, pos);
// }

// auto str::first_word_range(std::string_view s) -> range_type {
//     size_type pos = 0;
//     return next_word_range(s, pos);
// }

// auto str::first_word(std::string_view s) -> std::string {
//     return std::string{first_word_view(s)};
// }

// auto str::last_word_view(std::string_view s) -> std::string_view {
//     auto range = last_word_range(s);
//     return std::string_view{s.data() + range.begin_pos(), range.size()};
// }

// auto str::last_word_range(std::string_view s) -> range_type {
//     auto pos = s.size();
//     return prev_word_range(s, pos);
// }

// auto str::last_word(std::string_view s) -> std::string {
//     return std::string{last_word_view(s)};
// }

auto str::next_word_view(std::string_view s, size_type& pos) -> std::string_view {
    auto range = next_word_range(s, pos);
    return std::string_view{(s.data() + range.begin_pos()), range.size()};
}

auto str::next_word_range(std::string_view s, size_type& pos) -> range_type {
    if (pos >= s.size()) {
        pos = s.size();
        return range_type{s.size(), 0};
    }

    auto itr_pos = s.cbegin();
    std::advance(itr_pos, pos);

    // 先找到非空白
    auto itr_begin = std::find_if(itr_pos, s.cend(), [](std::string_view::value_type ch) -> bool {
        return !std::isspace(ch);
    });

    if (itr_begin == s.cend()) {
        pos = s.size();
        return range_type{s.size(), 0};
    }

    // 找到空白
    auto itr_end = std::find_if(itr_begin, s.cend(), [](std::string_view::value_type ch) -> bool {
        return std::isspace(ch);
    });

    if (itr_end == s.cend()) {
        pos = s.size();
    } else {
        pos = std::distance(s.cbegin(), itr_end);
    }

    size_type pos_begin = std::distance(s.cbegin(), itr_begin);
    return range_type{pos_begin, (pos - pos_begin)};
}

auto str::next_word(std::string_view s, size_type& pos) -> std::string {
    return std::string{next_word_view(s, pos)};
}

auto str::prev_word_view(std::string_view s, size_type& pos) -> std::string_view {
    auto range = prev_word_range(s, pos);
    return std::string_view(s.data() + range.begin_pos(), range.size());
}

auto str::prev_word_range(std::string_view s, size_type& pos) -> range_type {
    if (pos == 0) {
        return range_type{0, 0};
    }

    if (pos > s.size()) {
        pos = s.size();
    }

    const_pointer ptr_end = s.data() + pos - 1;

    while (ptr_end > s.data()) {
        if (!std::isspace(*(ptr_end - 1))) {
            break;
        }
        ptr_end--;
    }

    const_pointer ptr_begin = ptr_end;

    while (ptr_begin > s.data()) {
        if (std::isspace(*(ptr_begin - 1))) {
            break;
        }
        ptr_begin--;
    }

    return range_type{size_type(ptr_begin - s.data()), size_type(ptr_end - ptr_begin)};
}

auto str::prev_word(std::string_view s, size_type& pos) -> std::string {
    return std::string{prev_word_view(s, pos)};
}

auto str::surround(std::string_view s, std::string_view left, std::string_view right) -> std::string {
    std::string result;
    result.reserve(s.size() + left.size() + right.size());
    return result.append(left).append(s).append(right);
}

auto str::surround_inplace(std::string& s, std::string_view left, std::string_view right) -> std::string& {
    s = surround(s, left, right);
    return s;
}

auto str::unsurround_view(std::string_view s, std::string_view left, std::string_view right) -> std::string_view {
    return remove_suffix_view(remove_prefix_view(s, left), right);
}

auto str::unsurround(std::string_view s, std::string_view left, std::string_view right) -> std::string {
    return std::string{unsurround_view(s, left, right)};
}

auto str::unsurround_inplace(std::string& s, std::string_view left, std::string_view right) -> std::string& {
    s = unsurround(s, left, right);
    return s;
}

auto str::invert(std::string_view s, size_type pos, size_type max_n) -> std::string {
    std::string result{s};
    return str::invert_inplace(result, pos, max_n);
}

auto str::invert_inplace(std::string& s, size_type pos, size_type max_n) -> std::string& {
    if ((s.size() < 2) || (pos >= s.size()) || (max_n < 2)) {
        return s;
    }

    max_n = std::min(max_n, (s.size() - pos));
    pointer left = s.data() + pos;
    pointer right = s.data() + pos + max_n - 1;

    while (left < right) {
        value_type ch = *left;
        *left = *right;
        *right = ch;
        left++;
        right--;
    }

    return s;
}

auto str::repeat(std::string_view s, size_type times) -> std::string {
    if (s.empty() || (times == 0)) {
        return {};
    }

    std::string result;
    result.reserve(times * s.size());

    for (size_type i = 0; i < times; i++) {
        result.append(s);
    }

    return result;
}

auto str::repeat(value_type ch, size_type times) -> std::string {
    std::string result;
    result.resize(times, ch);
    return result;
}

auto str::random(size_type n, const number_provider_proc& proc) -> std::string {
    if (n == 0) [[unlikely]] {
        return {};
    }

    std::string result;
    result.reserve(n);
    for (size_type index = 0; index < n; index++) {
        result.push_back(static_cast<value_type>(proc()));
    }

    return result;
}

auto str::random(size_type n, std::string_view charset, const number_provider_proc& proc) -> std::string {
    if (charset.empty() || (n == 0)) [[unlikely]] {
        return {};
    }

    std::string result;
    result.reserve(n);
    for (size_type index = 0; index < n; index++) {
        result.push_back(charset[proc() % charset.size()]);
    }

    return result;
}

auto str::random(size_type n, charset_type charset, const number_provider_proc& proc) -> std::string {
    return random(n, charset.string(), proc);
}

auto str::random_fill(std::string& s, std::string_view charset, const number_provider_proc& proc) -> std::string& {
    if (charset.empty()) {
        return s;
    }

    for (value_type& ch : s) {
        ch = charset[proc() % charset.size()];
    }

    return s;
}

auto str::random_fill(std::string& s, charset_type charset, const number_provider_proc& proc) -> std::string& {
    return random_fill(s, charset.string(), proc);
}

auto str::random_fill(std::string& s, const number_provider_proc& proc) -> std::string& {
    if (s.empty()) {
        return s;
    }

    for (value_type& ch : s) {
        ch = static_cast<value_type>(proc());
    }

    return s;
}

auto str::random_reorder(std::string& s, const number_provider_proc& proc) -> std::string& {
    if (s.empty()) {
        return s;
    }

    for (size_type index = 0; index < s.size(); index++) {
        size_type target = proc() % (s.size() - index);
        std::swap(s[index], s[target]);
    }

    return s;
}

auto str::make_spaces(size_type width) -> std::string {
    return repeat(' ', width);
}

auto str::after_skip_spaces_view(std::string_view s) -> std::string_view {
    size_type pos = 0;
    return after_skip_spaces_view(s, pos);
}

auto str::after_skip_spaces(std::string_view s) -> std::string {
    size_type pos = 0;
    return std::string{after_skip_spaces_view(s, pos)};
}

auto str::after_skip_spaces_inplace(std::string& s) -> std::string& {
    return trim_left_inplace(s);
}

auto str::after_skip_spaces_view(std::string_view s, size_type pos) -> std::string_view {
    if (pos >= s.size()) {
        pos = s.size();
        return {};
    }

    const_pointer ptr = s.data() + pos;
    while (ptr < s.data() + s.size()) {
        if (!std::isspace(*ptr)) {
            break;
        }
        ptr++;
    }

    pos = ptr - s.data();
    return s.substr(pos);
}

auto str::after_skip_spaces(std::string_view s, size_type pos) -> std::string {
    if (pos >= s.size()) {
        pos = s.size();
        return {};
    }

    const_pointer ptr = s.data() + pos;
    while (ptr < s.data() + s.size()) {
        if (!std::isspace(*ptr)) {
            break;
        }
        ptr++;
    }

    pos = ptr - s.data();
    return std::string{s.data() + pos, s.size() - pos};
}

auto str::after_skip_spaces_inplace(std::string& s, size_type pos) -> std::string& {
    std::string_view remain = after_skip_spaces_view(s, pos);
    std::memmove(s.data(), remain.data(), remain.size());
    s.resize(remain.size());
    return s;
}

auto str::cover(std::string_view s, std::string_view mask, size_type left_n, size_type right_n) -> std::string {
    if (s.size() < (left_n + right_n)) {
        return {};
    }

    std::string result;
    result.reserve(mask.size() + left_n + right_n);

    if (left_n > 0) {
        result += take_left_view(s, left_n);
    }

    result += mask;

    if (right_n > 0) {
        result += take_right_view(s, right_n);
    }

    return result;
}

auto str::cover_inplace(std::string& s, std::string_view mask, size_type left_n, size_type right_n) -> std::string& {
    s = cover(s, mask, left_n, right_n);
    return s;
}

auto str::join(std::string_view s, const view_provider_proc& proc) -> std::string {
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

auto str::join_list(const view_provider_proc& proc) -> std::string {
    return join(",", proc);
}

auto str::join_map(std::string_view sep_pair, std::string_view sep_list,
    const view_pair_provider_proc& proc) -> std::string {
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

auto str::join_map(const view_pair_provider_proc& proc) -> std::string {
    return str::join_map("=", ",", proc);
}

auto str::join_lines(std::string_view line_ends, const view_provider_proc& proc) -> std::string {
    assert(!line_ends.empty());

    std::string result;
    auto item = proc();
    while (!item) {
        result.append(item.value());
        if (!ends_with(item.value(), line_ends)) {
            result.append(line_ends);
        }
    }

    return result;
}

auto str::join_lines(const view_provider_proc& proc) -> std::string {
    return join_lines("\n", proc);
}

auto str::join_path(std::string_view sep, const view_provider_proc& proc) -> std::string {
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
            result.append(sep);
        }

        result.append(*item);
    }

    return result;
}

auto str::join_path(const view_provider_proc& proc) -> std::string {
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

auto str::join_searchpath(std::string_view sep, const view_provider_proc& proc) -> std::string {
    std::string result;
    for (auto item = proc(); item; item = proc()) {
        // 如果每项为空,全部跳过
        if (item->empty()) {
            continue;
        }

        if (!result.empty()) {
            result.append(sep);
        }

        result.append(*item);
    }

    return result;
}

auto str::join_searchpath(const view_provider_proc& proc) -> std::string {
    return join_searchpath(":", proc);
}

auto str::split(std::string_view s, const char_match_proc& sepset, size_type max_n, const view_consumer_proc& proc) -> void {
    assert(sepset);

    // 最大拆分次数如果为0，就不需要拆了
    if (max_n == 0) {
        proc(s);
        return;
    }

    size_type n = 0;
    size_type pos_start = 0;
    while (pos_start < s.size()) {
        // 找到分隔符的位置
        auto itr_begin = s.begin();
        std::advance(itr_begin, pos_start);
        auto itr_pos = std::find_if(
            itr_begin, s.end(), [&sepset](value_type ch) -> bool {
                return sepset(ch);
            });
        size_type pos_end = std::distance(s.begin(), itr_pos);
        if (pos_end == std::string::npos) {
            break;
        }

        // 执行输出,如果用户希望提前结束就直接结束后续的部分不再理会
        if (proc(std::string_view{s.data() + pos_start, (pos_end - pos_start)}) != 0) {
            return;
        }

        // 准备好下次查找的起点
        pos_start = pos_end + 1;

        // 根据次数判定是否还需要继续找:如果不再需要找，就中断循环，将剩余的部分丢给用户
        n++;
        if (n >= max_n) {
            break;
        }
    }

    proc(std::string_view{s.data() + pos_start, s.size() - pos_start});
}

auto str::split(std::string_view s, const charset_type& sepset, size_type max_n, const view_consumer_proc& proc) -> void {
    split(
        s, [&sepset](value_type ch) -> bool {
            return sepset.get(ch);
        },
        max_n, proc);
}

auto str::split(std::string_view s, const charset_type& sepset, const view_consumer_proc& proc) -> void {
    split(
        s, [&sepset](value_type ch) -> bool {
            return sepset.get(ch);
        },
        npos, proc);
}

auto str::split(std::string_view s, const charset_type& sepset, size_type max_n) -> std::vector<std::string> {
    std::vector<std::string> result;
    split(s, sepset, max_n, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });
    return result;
}

auto str::split(std::string_view s, std::string_view sepset, size_type max_n, const view_consumer_proc& proc) -> void {
    split(s, charset_type{sepset}, max_n, proc);
}

auto str::split(std::string_view s, std::string_view sepset, const view_consumer_proc& proc) -> void {
    split(s, charset_type{sepset}, npos, proc);
}

auto str::split(std::string_view s, std::string_view sepset, size_type max_n) -> std::vector<std::string> {
    std::vector<std::string> result;
    split(s, sepset, max_n, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });
    return result;
}

auto str::split(std::string_view s, const view_consumer_proc& proc) -> void {
    size_type pos = 0;
    while (pos < s.size()) {
        std::string_view word = str::next_word_view(s, pos);
        if (word.empty()) {
            assert(pos >= s.size());
            continue;
        }

        if (proc(word) != 0) {
            break;
        }
    }
}

auto str::split(std::string_view s, size_type max_n) -> std::vector<std::string> {
    if (max_n == 0) {
        return {};
    }

    std::vector<std::string> result;

    str::split(s, [&result, max_n](std::string_view item) -> int {
        assert(!item.empty());
        result.emplace_back(item);
        if (result.size() >= max_n) {
            return -1;
        }
        return 0;
    });

    return result;
}

auto str::split_view(std::string_view s, const charset_type& sepset, size_type max_n) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    split(s, sepset, max_n, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });
    return result;
}

auto str::split_view(std::string_view s, std::string_view sepset, size_type max_n) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    split(s, charset_type{sepset}, max_n, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });
    return result;
}

auto str::split_view(std::string_view s, size_type max_n) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    split(
        s,
        [](value_type ch) -> bool {
            return std::isspace(ch);
        },
        max_n,
        [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });
    return result;
}

auto str::split_list(std::string_view s, std::string_view sep, size_type max_n, const view_consumer_proc& proc) -> void {
    if (sep.empty()) {
        sep = ",";
    }

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

auto str::split_list(std::string_view s, std::string_view sep, const view_consumer_proc& proc) -> void {
    str::split_list(s, sep, str::npos, [&proc](std::string_view item) -> int {
        return proc(item);
    });
}

auto str::split_list(std::string_view s, std::string_view sep, size_type max_n) -> std::vector<std::string> {
    std::vector<std::string> result;

    str::split_list(s, sep, max_n, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });

    return result;
}

auto str::split_list(std::string_view s, const std::regex& sep, size_type max_n, const view_consumer_proc& proc) -> void {
    if (max_n == 0) {
        proc(s);
        return;
    }

    size_type n = 0;

    size_type last_pos = 0;
    size_type pos = 0;
    while (pos < s.size()) {
        // 找到满足正则表达式的位置
        range_type range = str::next_regex_range(s, pos, sep);
        if (range.empty()) {
            break;
        }

        // 将找到的数据输出给调用方
        size_type start = range.begin_pos();
        size_type stop = range.end_pos();
        if (proc(std::string_view{s.data() + last_pos, (start - last_pos)}) != 0) {
            return;
        }

        // 为下次查找做准备
        last_pos = stop;

        // 如果次数达到最大次数限制:中断循环，并将剩余的数据输出
        n++;
        if (n >= max_n) {
            break;
        }
    }

    // 最后一部分也需要单独拆出来
    proc(std::string_view{s.data() + last_pos, (s.size() - last_pos)});
}

auto str::split_list(std::string_view s, const std::regex& sep, const view_consumer_proc& proc) -> void {
    str::split_list(s, sep, str::npos, proc);
}

auto str::split_list(std::string_view s, const std::regex& sep, size_type max_n) -> std::vector<std::string> {
    std::vector<std::string> result;

    str::split_list(s, sep, max_n, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });

    return result;
}

auto str::split_list_view(std::string_view s, const std::regex& sep, size_type max_n) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;

    str::split_list(s, sep, max_n, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });

    return result;
}

auto str::split_list_view(std::string_view s, std::string_view sep, size_type max_n) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;

    str::split_list(s, sep, max_n, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });

    return result;
}

auto str::split_pair(std::string_view s, std::string_view sep) -> std::tuple<std::string, std::string> {
    std::array<std::string, 2> pair;
    size_t n = 0;
    str::split_list(s, sep, 1, [&n, &pair](std::string_view item) -> int {
        pair[n++] = item;
        return 0;
    });

    return {pair[0], pair[1]};
}

auto str::split_pair_view(std::string_view s, std::string_view sep) -> std::tuple<std::string_view, std::string_view> {
    std::array<std::string_view, 2> pair;
    size_t n = 0;
    str::split_list(s, sep, 1, [&n, &pair](std::string_view item) -> int {
        pair[n++] = item;
        return 0;
    });

    return {pair[0], pair[1]};
}

auto str::split_map(std::string_view s, std::string_view sep_list, std::string_view sep_pair,
    const view_pair_consumer_proc& proc) -> void {
    if (s.empty()) [[unlikely]] {
        return;
    }

    if (sep_list.empty()) [[unlikely]] {
        sep_list = ",";
    }

    if (sep_pair.empty()) [[unlikely]] {
        sep_pair = ":";
    }

    str::split_list(s, sep_list, str::npos, [sep_pair, &proc](std::string_view item) -> int {
        auto key_val = str::split_pair(item, sep_pair);
        return proc(std::get<0>(key_val), std::get<1>(key_val));
    });
}

auto str::split_map(std::string_view s, std::string_view sep_list, std::string_view sep_pair,
    size_type max_n) -> std::map<std::string, std::string> {
    if (max_n == 0) {
        return {};
    }

    std::map<std::string, std::string> result;
    str::split_map(s, sep_list, sep_pair, [max_n, &result](std::string_view key, std::string_view value) -> int {
        result[std::string{key}] = std::string{value};
        if (result.size() >= max_n) {
            return -1;
        }

        return 0;
    });

    return result;
}

auto str::split_lines(std::string_view s, bool keep_ends, const view_consumer_proc& proc) -> void {
    if (s.empty()) {
        return;
    }

    size_type start = 0;
    const_pointer ptr_line = s.data();
    while (start < s.size()) {
        auto eol = str::next_eol_range(s, start);

        // 最后一部分没有结束符
        if (!eol) {
            break;
        }

        assert(!eol->empty());

        // 遇到结束符
        size_type line_size = 0;
        if (keep_ends) {
            line_size = static_cast<size_type>(s.data() + eol->end_pos() - ptr_line);
        } else {
            line_size = static_cast<size_type>(s.data() + eol->begin_pos() - ptr_line);
        }

        if (proc(std::string_view{ptr_line, line_size}) != 0) {
            return;
        }

        // 移动到下一行起始位置
        ptr_line = s.data() + eol->end_pos();
    }

    // 最后一部分可能没有换行符
    if (ptr_line < (s.data() + s.size())) {
        proc(std::string_view{ptr_line, static_cast<size_type>((s.data() + s.size()) - ptr_line)});
    }
}

auto str::split_lines(std::string_view s, bool keep_ends) -> std::vector<std::string> {
    std::vector<std::string> result;
    str::split_lines(s, keep_ends, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });
    return result;
}

auto str::split_lines_view(std::string_view s, bool keep_ends) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    str::split_lines(s, keep_ends, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });
    return result;
}

auto str::split_path(std::string_view s, const view_consumer_proc& proc) -> void {
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

auto str::split_path(std::string_view s) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    str::split_path(s, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });
    return result;
}

auto str::split_searchpath(std::string_view s, bool keep_empty, value_type sep, const view_consumer_proc& proc) -> void {
    str::split_list(s, std::string_view{&sep, 1}, [keep_empty, &proc](std::string_view item) -> int {
        if (!keep_empty && item.empty()) {
            return 0;
        }

        return proc(item);
    });
}

auto str::split_searchpath(std::string_view s, bool keep_empty, value_type sep) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    str::split_searchpath(s, keep_empty, sep, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });
    return result;
}

// auto str::split_csv(std::string_view s) -> std::vector<std::string> {
// }

auto str::partition_range(std::string_view s, charset_type charset) -> std::tuple<range_type, range_type, range_type> {
    return partition_range(s, [&charset](value_type ch) -> bool {
        return charset.get(ch);
    });
}

auto str::partition_range(std::string_view s,
    const char_match_proc& proc) -> std::tuple<range_type, range_type, range_type> {
    auto itr = std::find_if(s.begin(), s.end(), proc);
    if (itr == s.cend()) {
        return {range_type{0, s.size()}, range_type{}, range_type{}};
    }

    size_type pos = std::distance(s.begin(), itr);
    return {range_type{0, pos}, range_type{pos, 1}, range_type{(pos + 1), (s.size() - (pos + 1))}};
}

auto str::partition_range(std::string_view s, std::string_view sep) -> std::tuple<range_type, range_type, range_type> {
    size_type pos = s.find(sep, 0);
    if (pos >= s.size()) {
        return {range_type{0, s.size()}, range_type{}, range_type{}};
    }

    return {range_type{0, pos}, range_type{pos, 1}, range_type{(pos + 1), (s.size() - (pos + 1))}};
}

auto str::partition_range(std::string_view s, const std::regex& pattern) -> std::tuple<range_type, range_type, range_type> {
    std::match_results<std::string_view::const_iterator> match;
    bool ret = std::regex_search(s.begin(), s.end(), match, pattern);
    if (!ret) {
        return {range_type{0, s.size()}, range_type{}, range_type{}};
    }

    size_type pos = match.position(0);
    size_type len = match.length(0);
    return {range_type{0, pos}, range_type{pos, len}, range_type{(pos + len), (s.size() - pos - len)}};
}

auto str::partition_range(std::string_view s,
    const range_search_proc& proc) -> std::tuple<range_type, range_type, range_type> {
    auto matched = proc(s, range_type{0, s.size()});
    if (matched.empty()) {
        return {range_type{0, s.size()}, range_type{}, range_type{}};
    }

    range_type right = range_type{(matched.begin_pos() + matched.size()), s.size() - matched.end_pos()};
    return {range_type{0, matched.begin_pos()}, matched, right};
}

auto str::partition_view(std::string_view s, charset_type charset)
    -> std::tuple<std::string_view, std::string_view, std::string_view> {
    return partition_view(s, [&charset](value_type ch) -> bool {
        return charset.get(ch);
    });
}

auto str::partition_view(std::string_view s, const char_match_proc& proc)
    -> std::tuple<std::string_view, std::string_view, std::string_view> {
    auto itr = std::find_if(s.begin(), s.end(), proc);
    if (itr == s.cend()) {
        return {s, {}, {}};
    }

    size_type pos = std::distance(s.begin(), itr);
    return {{s.data(), pos},
        {(s.data() + pos), 1},
        {(s.data() + pos + 1), (s.size() - (pos + 1))}};
}

auto str::partition_view(std::string_view s, std::string_view sep)
    -> std::tuple<std::string_view, std::string_view, std::string_view> {
    size_type pos = s.find(sep, 0);
    if (pos >= s.size()) {
        return {s, {}, {}};
    }

    return {{s.data(), pos},
        {(s.data() + pos), 1},
        {(s.data() + pos + 1), (s.size() - (pos + 1))}};
}

auto str::partition_view(std::string_view s, const std::regex& pattern)
    -> std::tuple<std::string_view, std::string_view, std::string_view> {
    std::match_results<std::string_view::const_iterator> match;
    bool ret = std::regex_search(s.begin(), s.end(), match, pattern);
    if (!ret) {
        return {s, {}, {}};
    }

    size_type pos = match.position(0);
    size_type len = match.length(0);
    return {{s.data(), pos},
        {s.data() + pos, len},
        {(s.data() + pos + len), (s.size() - pos - len)}};
}

auto str::partition_view(std::string_view s, const view_search_proc& proc)
    -> std::tuple<std::string_view, std::string_view, std::string_view> {
    auto result = proc(s);
    if (!result) {
        return {s, {}, {}};
    }

    std::string_view matched = result.value();
    std::string_view right = {(matched.data() + matched.size()),
        s.size() - (matched.data() + matched.size() - s.data())};
    return {{s.data(), static_cast<size_type>(matched.data() - s.data())}, matched, right};
}

auto str::partition(std::string_view s, charset_type charset) -> std::tuple<std::string, std::string, std::string> {
    auto abc = partition_view(s, charset);
    return std::tuple{std::string{std::get<0>(abc)}, std::string{std::get<0>(abc)}, std::string{std::get<0>(abc)}};
}

auto str::partition(std::string_view s, const char_match_proc& proc) -> std::tuple<std::string, std::string, std::string> {
    auto abc = partition_view(s, proc);
    return std::tuple{std::string{std::get<0>(abc)}, std::string{std::get<0>(abc)}, std::string{std::get<0>(abc)}};
}

auto str::partition(std::string_view s, std::string_view sep) -> std::tuple<std::string, std::string, std::string> {
    auto abc = partition_view(s, sep);
    return std::tuple{std::string{std::get<0>(abc)}, std::string{std::get<0>(abc)}, std::string{std::get<0>(abc)}};
}

auto str::partition(std::string_view s, const std::regex& pattern) -> std::tuple<std::string, std::string, std::string> {
    auto abc = partition_view(s, pattern);
    return std::tuple{std::string{std::get<0>(abc)}, std::string{std::get<0>(abc)}, std::string{std::get<0>(abc)}};
}

auto str::partition(std::string_view s, const view_search_proc& proc) -> std::tuple<std::string, std::string, std::string> {
    auto abc = partition_view(s, proc);
    return std::tuple{std::string{std::get<0>(abc)}, std::string{std::get<0>(abc)}, std::string{std::get<0>(abc)}};
}

// auto str::rpartition_view(std::string_view s, charset_type sep) -> std::tuple<std::string_view, std::string_view, std::string_view> {
// }
//
// auto str::rpartition_view(std::string_view s, const char_match_proc& sep) -> std::tuple<std::string_view, std::string_view, std::string_view> {
// }
//
// auto str::rpartition_view(std::string_view s, std::string_view sep) -> std::tuple<std::string_view, std::string_view, std::string_view> {
// }
//
// auto str::rpartition_view(std::string_view s, const std::regex& sep) -> std::tuple<std::string_view, std::string_view, std::string_view> {
// }
//
// auto str::rpartition_view(std::string_view s, const view_search_proc& sep) -> std::tuple<std::string_view, std::string_view, std::string_view> {
// }
//
////
// auto str::rpartition(std::string_view s, charset_type sep) -> std::tuple<std::string, std::string, std::string> {
// }
//
// auto str::rpartition(std::string_view s, const char_match_proc& sep) -> std::tuple<std::string, std::string, std::string> {
// }
//
// auto str::rpartition(std::string_view s, std::string_view sep) -> std::tuple<std::string, std::string, std::string> {
// }
//
// auto str::rpartition(std::string_view s, const std::regex& sep) -> std::tuple<std::string, std::string, std::string> {
// }
//
// auto str::rpartition(std::string_view s, const view_search_proc& sep) -> std::tuple<std::string, std::string, std::string> {
// }

auto str::chunked(std::string_view s, size_type width, const view_consumer_proc& proc) -> void {
    if (width == 0) {
        proc(s);
        return;
    }

    std::vector<std::string_view> result;

    size_type pos = 0;
    while ((pos + width) <= s.size()) {
        if (proc(s.substr(pos, width)) != 0) {
            return;
        }
    }

    if (pos < s.size()) {
        proc(s.substr(pos));
    }
}

auto str::chunked(std::string_view s, size_type width) -> std::vector<std::string> {
    std::vector<std::string> result;
    chunked(s, width, [&result](std::string_view item) -> int {
        result.emplace_back(std::move(item));
        return 0;
    });
    return result;
}

auto str::chunked_view(std::string_view s, size_type width) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    chunked(s, width, [&result](std::string_view item) -> int {
        result.emplace_back(std::move(item));
        return 0;
    });
    return result;
}

auto str::take_window_view(std::string_view s, size_type& pos, size_type max_n) -> std::string_view {
    if (pos >= s.size()) {
        return {};
    }

    if ((s.size() - pos) < max_n) {
        max_n = (s.size() - pos);
    }

    auto result = std::string_view{s.data() + pos, max_n};
    pos += max_n;
    return result;
}

auto str::take_window(std::string_view s, size_type& pos, size_type max_n) -> std::string {
    return std::string{take_window_view(s, pos, max_n)};
}

auto str::windowed(std::string_view s, size_type width, size_type step, const view_consumer_proc& proc) -> void {
    size_type pos = 0;
    while (pos < s.size()) {
        if (proc(str::take_window_view(s, pos, width)) != 0) {
            return;
        }
        pos += step;
    }
}

auto str::windowed_view(std::string_view s, size_type width, size_type step) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    windowed(s, width, step, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });
    return result;
}

auto str::windowed(std::string_view s, size_type width, size_type step) -> std::vector<std::string> {
    std::vector<std::string> result;
    windowed(s, width, step, [&result](std::string_view item) -> int {
        result.emplace_back(item);
        return 0;
    });
    return result;
}

auto str::to_lower(std::string_view s) -> std::string {
    std::string result{s};
    return str::to_lower_inplace(result);
}

auto str::to_upper(std::string_view s) -> std::string {
    std::string result{s};
    return str::to_upper_inplace(result);
}

auto str::to_title(std::string_view s) -> std::string {
    std::string result{s};
    return str::to_title_inplace(result);
}

auto str::to_capitalize(std::string_view s) -> std::string {
    if (s.empty()) {
        return {};
    }

    std::string result{s};
    if (std::islower(s[0])) {
        result[0] = static_cast<value_type>(std::toupper(s[0]));
    }

    return result;
}

auto str::swap_case(std::string_view s) -> std::string {
    std::string result{s};
    return str::swap_case_inplace(result);
}

auto str::to_lower_inplace(std::string& s) -> std::string& {
    for (value_type& ch : s) {
        ch = static_cast<value_type>(std::tolower(ch));
    }

    return s;
}

auto str::to_upper_inplace(std::string& s) -> std::string& {
    for (value_type& ch : s) {
        ch = static_cast<value_type>(std::toupper(ch));
    }

    return s;
}

auto str::to_title_inplace(std::string& s) -> std::string& {
    str::foreach_words(s, [&s](range_type range) -> int {
        pointer ptr = s.data() + range.pos;
        while (ptr < (s.data() + range.pos + range.len)) {
            if (std::isalpha(*ptr)) {
                *ptr = static_cast<value_type>(std::toupper(*ptr));
                break;
            }
            ptr++;
        }
        return 0;
    });

    return s;
}

auto str::to_capitalize_inplace(std::string& s) -> std::string& {
    if (s.empty()) {
        return s;
    }

    s[0] = static_cast<value_type>(std::toupper(s[0]));
    return s;
}

auto str::swap_case_inplace(std::string& s) -> std::string& {
    for (value_type& ch : s) {
        if (std::islower(ch)) {
            ch = static_cast<value_type>(std::toupper(ch));
            continue;
        }

        if (std::isupper(ch)) {
            ch = static_cast<value_type>(std::tolower(ch));
            continue;
        }
    }

    return s;
}

auto str::trim_left_view(std::string_view s, const char_match_proc& proc) -> std::string_view {
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

auto str::trim_left_view(std::string_view s) -> std::string_view {
    return str::trim_left_view(s, [](value_type ch) -> bool {
        return std::isspace(ch);
    });
}

auto str::trim_left_view(std::string_view s, const charset_type& charset) -> std::string_view {
    return trim_left_view(s, [&charset](value_type ch) -> bool {
        return charset.get(ch);
    });
}

auto str::trim_left_view(std::string_view s, std::string_view charset) -> std::string_view {
    return trim_left_view(s, charset_type{charset});
}

auto str::trim_right_view(std::string_view s, const char_match_proc& proc) -> std::string_view {
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

auto str::trim_right_view(std::string_view s, const charset_type& charset) -> std::string_view {
    return trim_right_view(s, [charset](value_type ch) -> bool {
        return charset.get(ch);
    });
}

auto str::trim_right_view(std::string_view s, std::string_view charset) -> std::string_view {
    return trim_right_view(s, charset_type{charset});
}

auto str::trim_right_view(std::string_view s) -> std::string_view {
    return str::trim_right_view(s, [](value_type ch) -> bool {
        return std::isspace(ch);
    });
}

auto str::trim_surrounding_view(std::string_view s, const char_match_proc& proc) -> std::string_view {
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

auto str::trim_surrounding_view(std::string_view s, const charset_type& charset) -> std::string_view {
    return trim_surrounding_view(s, [charset](value_type ch) -> bool {
        return charset.get(ch);
    });
}

auto str::trim_surrounding_view(std::string_view s, std::string_view charset) -> std::string_view {
    return trim_surrounding_view(s, charset_type{charset});
}

auto str::trim_surrounding_view(std::string_view s) -> std::string_view {
    return str::trim_surrounding_view(s, [](value_type ch) -> bool {
        return std::isspace(ch);
    });
}

auto str::trim_left(std::string_view s, const char_match_proc& proc) -> std::string {
    return std::string{trim_left_view(s, proc)};
}

auto str::trim_left(std::string_view s) -> std::string {
    return std::string{trim_left_view(s)};
}

auto str::trim_left(std::string_view s, const charset_type& charset) -> std::string {
    return std::string{trim_left_view(s, charset)};
}

auto str::trim_left(std::string_view s, std::string_view charset) -> std::string {
    return std::string{trim_left_view(s, charset)};
}

auto str::trim_right(std::string_view s, const char_match_proc& proc) -> std::string {
    return std::string{trim_right_view(s, proc)};
}

auto str::trim_right(std::string_view s, const charset_type& charset) -> std::string {
    return std::string{trim_right_view(s, charset)};
}

auto str::trim_right(std::string_view s, std::string_view charset) -> std::string {
    return std::string{trim_right_view(s, charset)};
}

auto str::trim_right(std::string_view s) -> std::string {
    return std::string{trim_right_view(s)};
}

auto str::trim_surrounding(std::string_view s, const char_match_proc& proc) -> std::string {
    return std::string{trim_surrounding_view(s, proc)};
}

auto str::trim_surrounding(std::string_view s, const charset_type& charset) -> std::string {
    return std::string{trim_surrounding_view(s, charset)};
}

auto str::trim_surrounding(std::string_view s, std::string_view charset) -> std::string {
    return std::string{trim_surrounding_view(s, charset)};
}

auto str::trim_surrounding(std::string_view s) -> std::string {
    return std::string{trim_surrounding_view(s)};
}

auto str::trim_left_inplace(std::string& s, const char_match_proc& proc) -> std::string& {
    s = trim_left(s, proc);
    return s;
}

auto str::trim_left_inplace(std::string& s, const charset_type& charset) -> std::string& {
    s = trim_left(s, charset);
    return s;
}

auto str::trim_left_inplace(std::string& s, std::string_view charset) -> std::string& {
    s = trim_left(s, charset);
    return s;
}

auto str::trim_left_inplace(std::string& s) -> std::string& {
    s = trim_left(s);
    return s;
}

auto str::trim_right_inplace(std::string& s, const char_match_proc& proc) -> std::string& {
    s = trim_right(s, proc);
    return s;
}

auto str::trim_right_inplace(std::string& s, const charset_type& charset) -> std::string& {
    s = trim_right(s, charset);
    return s;
}

auto str::trim_right_inplace(std::string& s, std::string_view charset) -> std::string& {
    s = trim_right(s, charset);
    return s;
}

auto str::trim_right_inplace(std::string& s) -> std::string& {
    s = trim_right(s);
    return s;
}

auto str::trim_surrounding_inplace(std::string& s, const char_match_proc& proc) -> std::string& {
    s = trim_surrounding(s, proc);
    return s;
}

auto str::trim_surrounding_inplace(std::string& s, const charset_type& charset) -> std::string& {
    s = trim_surrounding(s, charset);
    return s;
}

auto str::trim_surrounding_inplace(std::string& s, std::string_view charset) -> std::string& {
    s = trim_surrounding(s, charset);
    return s;
}

auto str::trim_surrounding_inplace(std::string& s) -> std::string& {
    s = trim_surrounding(s);
    return s;
}

auto str::trim_anywhere(std::string_view s, const char_match_proc& proc) -> std::string {
    std::string result;

    for (const_pointer r = s.data(); r < (s.data() + s.size()); r++) {
        if (proc(*r)) {
            continue;
        }

        result.append(1, *r);
    }

    return result;
}

auto str::trim_anywhere(std::string_view s, const charset_type& charset) -> std::string {
    return trim_anywhere(s, [&charset](value_type ch) -> bool {
        return charset.get(ch);
    });
}

auto str::trim_anywhere(std::string_view s, std::string_view charset) -> std::string {
    return trim_anywhere(s, charset_type{charset});
}

auto str::trim_anywhere(std::string_view s, value_type charset) -> std::string {
    return trim_anywhere(s, [charset](value_type ch) -> bool {
        return charset == ch;
    });
}

auto str::trim_anywhere(std::string_view s) -> std::string {
    return str::trim_anywhere(s, [](value_type ch) -> bool {
        return std::isspace(ch);
    });
}

auto str::trim_anywhere_inplace(std::string& s, const char_match_proc& proc) -> std::string& {
    s = trim_anywhere(s, proc);
    return s;
}

auto str::trim_anywhere_inplace(std::string& s, const charset_type& charset) -> std::string& {
    s = trim_anywhere(s, charset);
    return s;
}

auto str::trim_anywhere_inplace(std::string& s, std::string_view charset) -> std::string& {
    s = trim_anywhere(s, charset);
    return s;
}

auto str::trim_anywhere_inplace(std::string& s, value_type charset) -> std::string& {
    s = trim_anywhere(s, charset);
    return s;
}

auto str::trim_anywhere_inplace(std::string& s) -> std::string& {
    s = trim_anywhere(s);
    return s;
}

auto str::simplified(std::string_view s, std::string_view sep, const char_match_proc& proc) -> std::string {
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

auto str::simplified(std::string_view s) -> std::string {
    return str::simplified(s, " ", [](value_type ch) -> bool {
        return std::isspace(ch);
    });
}

auto str::simplified_inplace(std::string& s, std::string_view sep, const char_match_proc& proc) -> std::string& {
    s = simplified(s, sep, proc);
    return s;
}

auto str::simplified_inplace(std::string& s) -> std::string& {
    s = simplified(s);
    return s;
}

auto str::simplified_integer(std::string_view s) -> std::string {
    if (s.empty()) {
        return {};
    }

    const_pointer ptr = s.data();

    // 如果是前导加号
    if (*ptr == '+') {
        ptr++;
    }

    while (ptr < (s.data() + s.size())) {
        if (*ptr != '0') {
            break;
        }
        ptr++;
    }

    // 如果遇到全0场景
    if (ptr >= (s.data() + s.size())) {
        ptr--;
    }

    return std::string{ptr, static_cast<size_type>((s.data() + s.size()) - ptr)};
}

auto str::simplified_integer_inplace(std::string& s) -> std::string& {
    s = simplified_integer(s);
    return s;
}

// auto str::simplified_real(std::string_view s) -> std::string;
// auto str::simplified_real_inplace(std::string& s) -> std::string&;

auto str::copy(pointer buffer, size_type size, std::string_view s) -> size_type {
    assert(buffer != nullptr);
    size_type len = std::min(size, s.size());
    if (len == 0) {
        return len;
    }

    std::memcpy(buffer, s.data(), len);
    return len;
}

auto str::expand_envs(std::string_view s, bool keep_unexpanded, const string_mapping_proc& proc) -> std::string {
    std::string result;
    size_type start = 0;
    std::string key;
    while (start < s.size()) {
        size_type pos = s.find('$', start);

        // start 之后已经没有 $ 符号了
        if (pos == str::npos) {
            break;
        }

        // 将 start 到 rpos 之间的部分原样保存起来
        if (pos > start) {
            result.append(s.substr(start, pos - start));
            start = pos;
        }

        // 如果是  ${xxx} 的形式
        if (((pos + 1) < s.size()) && (s[pos + 1] == '{')) {
            size_type end = s.find('}', pos + 2);

            // 如果只有 "${" 却没有 "}"
            if (end == str::npos) {
                break;
            }

            // 匹配到 ${xxx} 的形式，确定 key
            key = s.substr(pos + 2, end - (pos + 2));

            // 尝试找到替换值
            auto val = proc(key);

            // 如果找到匹配项
            if (val) {
                result.append(*val);
                start = end + 1;
                continue;
            }

            // 如果没有找到匹配项
            if (keep_unexpanded) {
                result.append(s.substr(pos, ((end + 1) - pos)));
            }

            start = end + 1;
            continue;
        }

        // 如果是 $xxx 的形式

        // 将 $ 之后的名字提取出来
        size_type end = pos + 1;
        while (end < s.size()) {
            if (!(std::isalnum(s[end]) || (s[end] == '_'))) {
                break;
            }
            end++;
        }

        if ((end - (pos + 1)) <= 0) {
            break;
        }

        // 确定 key
        key = s.substr(pos + 1, end - (pos + 1));

        // 尝试找到替换值
        auto val = proc(key);

        // 如果找到匹配项
        if (val) {
            result.append(*val);
            start = end;
            continue;
        }

        // 如果没有找到匹配项
        if (keep_unexpanded) {
            result.append(s.substr(pos, (end - pos)));
        }

        start = end;
    }

    if (start < s.size()) {
        result.append(s.substr(start));
    }
    return result;
}

auto str::expand_envs(std::string_view s, bool keep_unexpanded) -> std::string {
    return str::expand_envs(s, keep_unexpanded, [](const std::string& key) -> std::optional<std::string> {
        const char* val = getenv(key.c_str());
        if (val == nullptr) {
            return std::nullopt;
        }

        return std::string{val};
    });
}

auto str::expand_envs(std::string_view s, bool keep_unexpanded,
    const std::map<std::string, std::string>& kvs)
    -> std::string {
    return str::expand_envs(s, keep_unexpanded, [&kvs](const std::string& key) -> std::optional<std::string> {
        auto itr = kvs.find(key);
        if (itr == kvs.cend()) {
            return std::nullopt;
        }

        return itr->second;
    });
}

auto str::expand_envs(std::string_view s, const std::map<std::string, std::string>& kvs) -> std::string {
    return str::expand_envs(s, false, kvs);
}

auto str::expand_envs(std::string_view s, std::string_view key, std::string_view val) -> std::string {
    return str::expand_envs(s, true, [&key, &val](const std::string& name) -> std::optional<std::string> {
        if (name != key) {
            return std::nullopt;
        }

        return std::string{val};
    });
}

auto str::expand_envs_inplace(std::string& s, bool keep_unexpanded, const string_mapping_proc& proc) -> std::string& {
    s = expand_envs(s, keep_unexpanded, proc);
    return s;
}

auto str::expand_envs_inplace(std::string& s, bool keep_unexpanded) -> std::string& {
    s = expand_envs(s, keep_unexpanded);
    return s;
}

auto str::expand_envs_inplace(std::string& s, bool keep_unexpanded,
    const std::map<std::string, std::string>& kvs)
    -> std::string& {
    s = expand_envs(s, keep_unexpanded, kvs);
    return s;
}

auto str::expand_envs_inplace(std::string& s, const std::map<std::string, std::string>& kvs) -> std::string& {
    s = expand_envs(s, kvs);
    return s;
}

auto str::expand_envs_inplace(std::string& s, std::string_view key, std::string_view val) -> std::string& {
    s = expand_envs(s, key, val);
    return s;
}

auto str::expand_tabs(std::string_view s, size_type tab_size) -> std::string {
    if (tab_size == 0) {
        tab_size = 8;
    }

    std::string result;
    result.reserve(s.size());

    size_type start = 0;
    size_type pos = 0;
    while (start < s.size()) {
        pos = s.find('\t', start);
        if (pos == str::npos) {
            break;
        }

        if (pos > start) {
            result.append(s.substr(start, pos - start));
        }
        result.append(tab_size - (result.size() % tab_size), ' ');
        start = pos + 1;
    }

    if (start < s.size()) {
        result.append(s.substr(start));
    }

    return result;
}

auto str::expand_tabs_inplace(std::string& s, size_type tab_size) -> std::string& {
    s = expand_tabs(s, tab_size);
    return s;
}

auto str::expand_user(std::string_view s) -> std::string {
    if (((s.size() == 1) && (s[0] == '~')) || ((s.size() >= 2) && (s[0] == '~') && (s[1] == '/'))) {
        const char* ptr_home = getenv("HOME");
        if (ptr_home == nullptr) {
            return std::string{s};
        }

        std::string_view home{ptr_home};

        std::string result;
        result.reserve(home.size() + s.size() - 1);
        result.append(home);
        result.append(s.substr(1));
        return result;
    }

    return std::string{s};
}

auto str::expand_user_inplace(std::string& s) -> std::string& {
    s = expand_user(s);
    return s;
}

auto str::normpath(std::string_view s) -> std::string {
    if (s.empty()) {
        return ".";
    }

    auto components = str::split_path(s);
    if (components.size() < 2) {
        return std::string{components[0]};
    }

    std::vector<std::string_view> result{components[0]};
    size_type fixlen = ((result[0] == "/") ? 1 : 0);

    for (ssize_type rpos = 1; rpos < static_cast<ssize_type>(components.size()); rpos++) {
        if (components[rpos] == ".") {
            continue;
        }

        if (components[rpos] == "..") {
            // 如果遇到已经固定的长度了
            if (result.size() == fixlen) {
                // 如果已经到跟目录了，根目录可以吸收所有的后退运算符号
                if (!result.empty() && result.back() == "/") {
                    continue;
                }

                // 如果碰到固定长度，意味着退无可退了，只能将自己继续作为父级目录增长 fixlen
                result.emplace_back(components[rpos]);
                fixlen++;
                continue;
            }

            assert(!result.empty());
            result.resize(result.size() - 1);
            continue;
        }

        if (result.size() > fixlen) {
            if (result.back() == ".") [[unlikely]] {
                result.pop_back();
            }
        }

        result.emplace_back(components[rpos]);
    }

    // 如果计算之后数组为空，而且不是绝对路径形式，那么裁定为当前目录
    if (result.empty()) {
        return ".";
    }

    return str::join_path(result);
}

auto str::normpath_inplace(std::string& s) -> std::string& {
    s = normpath(s);
    return s;
}

// auto str::is_absolute(std::string_view s) -> bool;
// auto str::is_relative(std::string_view s) -> bool;

// 处理路径中文件名的部分
auto str::basename_ptr(std::string_view s) -> std::string::const_pointer {
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
auto str::extname_ptr(std::string_view s) -> std::string::const_pointer {
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

auto str::dirname_ptr(std::string_view s) -> std::string::const_pointer {
    auto ptr = basename_ptr(s);
    while (ptr > s.data()) {
        if (*(ptr - 1) != '/') {
            break;
        }

        ptr--;
    }
    return ptr;
}

auto str::dirname_range(std::string_view s) -> range_type {
    auto ptr = str::dirname_ptr(s);
    if ((ptr == s.data()) && (*ptr == '/')) {
        return range_type{0, 1};
    }
    return range_type{0, static_cast<size_type>(ptr - s.data())};
}

auto str::dirname_view(std::string_view s) -> std::string_view {
    auto range = dirname_range(s);
    return std::string_view{s.data() + range.begin_pos(), range.size()};
}

auto str::dirname(std::string_view s) -> std::string {
    std::string_view dir_view = dirname_view(s);
    if (dir_view.empty()) {
        return ".";
    }
    return std::string{dir_view};
}

auto str::remove_dirname_view(std::string_view s) -> std::string_view {
    auto ptr = str::dirname_ptr(s);
    return std::string_view{ptr, static_cast<size_type>(s.data() + s.size() - ptr)};
}

auto str::remove_dirname(std::string_view s) -> std::string {
    return std::string{remove_dirname_view(s)};
}

auto str::replace_dirname(std::string_view s, std::string_view new_dir) -> std::string {
    auto ptr = str::dirname_ptr(s);

    auto remain_len = (s.data() + s.size() - ptr);

    assert(ptr >= s.data());

    std::string result;
    size_type result_len = new_dir.size() + remain_len;
    if ((ptr < (s.data() + s.size())) && (*ptr == '/')) {
        result.reserve(result_len);
        result.append(new_dir);
        result.append(ptr, remain_len);
    } else {
        result.reserve((result_len + 1));
        result.append(new_dir);
        result.append("/");
        result.append(ptr, remain_len);
    }

    return result;
}

auto str::split_dirname_view(std::string_view s) -> std::tuple<std::string_view, std::string_view> {
    auto base_ptr = str::basename_ptr(s);
    auto dir_ptr = str::dirname_ptr({s.data(), static_cast<size_type>(base_ptr - s.data())});
    return {{s.data(), static_cast<size_type>(dir_ptr - s.data())},
        {base_ptr, static_cast<size_type>((s.data() + s.size()) - base_ptr)}};
}

auto str::split_dirname(std::string_view s) -> std::tuple<std::string, std::string> {
    auto [dir, name] = split_dirname_view(s);
    return std::tuple{std::string{dir}, std::string{name}};
}

auto str::dirname_inplace(std::string& s) -> std::string& {
    s = dirname_view(s);
    return s;
}

auto str::remove_dirname_inplace(std::string& s) -> std::string& {
    s = remove_dirname_view(s);
    return s;
}

auto str::replace_dirname_inplace(std::string& s, std::string_view new_name) -> std::string& {
    s = replace_dirname(s, new_name);
    return s;
}

auto str::basename_range(std::string_view s) -> range_type {
    auto ptr = str::basename_ptr(s);
    return range_type{static_cast<size_type>(ptr - s.data()), static_cast<size_type>(s.data() + s.size() - ptr)};
}

auto str::basename_view(std::string_view s) -> std::string_view {
    auto ptr = str::basename_ptr(s);
    return std::string_view{ptr, static_cast<size_type>(s.data() + s.size() - ptr)};
}

auto str::basename(std::string_view s) -> std::string {
    return std::string{str::basename_view(s)};
}

auto str::remove_basename_view(std::string_view s) -> std::string_view {
    auto ptr = str::basename_ptr(s);
    return std::string_view{s.data(), static_cast<size_type>(ptr - s.data())};
}

auto str::remove_basename(std::string_view s) -> std::string {
    return std::string{remove_basename_view(s)};
}

auto str::replace_basename(std::string_view s, std::string_view newname) -> std::string {
    auto ptr = str::basename_ptr(s);
    std::string result;
    result.reserve((ptr - s.data()) + newname.size());
    result.append(s.data(), static_cast<size_type>(ptr - s.data()));
    result.append(newname);
    return result;
}

auto str::split_basename_view(std::string_view s) -> std::tuple<std::string_view, std::string_view> {
    auto ptr = str::basename_ptr(s);
    return {{s.data(), static_cast<size_type>(ptr - s.data())},
        {ptr, static_cast<size_type>(s.data() + s.size() - ptr)}};
}

auto str::split_basename(std::string_view s) -> std::tuple<std::string, std::string> {
    auto items = split_basename_view(s);
    return std::tuple{std::string{std::get<0>(items)}, std::string{std::get<1>(items)}};
}

auto str::basename_inplace(std::string& s) -> std::string& {
    s = basename_view(s);
    return s;
}

auto str::remove_basename_inplace(std::string& s) -> std::string& {
    s = remove_basename_view(s);
    return s;
}

auto str::replace_basename_inplace(std::string& s, std::string_view new_name) -> std::string& {
    s = replace_basename(s, new_name);
    return s;
}

auto str::extname_range(std::string_view s) -> range_type {
    auto ptr = str::extname_ptr(s);
    return range_type{static_cast<size_type>(ptr - s.data()), static_cast<size_type>(s.data() + s.size() - ptr)};
}

auto str::extname_view(std::string_view s) -> std::string_view {
    auto range = str::extname_range(s);
    return s.substr(range.begin_pos(), range.size());
}

auto str::extname(std::string_view s) -> std::string {
    auto ptr = str::extname_ptr(s);
    return std::string{ptr, static_cast<size_type>(s.data() + s.size() - ptr)};
}

auto str::remove_extname_view(std::string_view s) -> std::string_view {
    auto ptr = str::extname_ptr(s);
    return std::string_view{s.data(), static_cast<size_type>(ptr - s.data())};
}

auto str::remove_extname(std::string_view s) -> std::string {
    return std::string{remove_extname_view(s)};
}

auto str::replace_extname(std::string_view s, std::string_view new_name) -> std::string {
    auto ptr = str::extname_ptr(s);
    std::string result;
    result.reserve(ptr - s.data() + new_name.size());
    result.append(s.data(), static_cast<size_type>(ptr - s.data()));
    result.append(new_name);
    return result;
}

auto str::split_extname_view(std::string_view s) -> std::tuple<std::string_view, std::string_view> {
    auto ptr = str::extname_ptr(s);
    return {{s.data(), static_cast<size_type>(ptr - s.data())},
        {ptr, static_cast<size_type>(s.data() + s.size() - ptr)}};
}

auto str::split_extname(std::string_view s) -> std::tuple<std::string, std::string> {
    auto [left, name] = split_extname_view(s);
    return {std::string{left}, std::string{name}};
}

auto str::extname_inplace(std::string& s) -> std::string& {
    s = extname_view(s);
    return s;
}

auto str::remove_extname_inplace(std::string& s) -> std::string& {
    s = remove_extname_view(s);
    return s;
}

auto str::replace_extname_inplace(std::string& s, std::string_view new_name) -> std::string& {
    s = replace_extname(s, new_name);
    return s;
}

auto str::rawname_range(std::string_view s) -> range_type {
    if (s.empty()) {
        return range_type{};
    }

    std::string::const_pointer base_ptr = basename_ptr(s);
    std::string::const_pointer ptr = s.data() + s.size() - 1;

    while (ptr > base_ptr) {
        if (*ptr == '.') {
            return range_type{static_cast<size_type>(base_ptr - s.data()), static_cast<size_type>(ptr - base_ptr)};
        }
        ptr--;
    }

    auto ext_pos = static_cast<size_type>(base_ptr - s.data());
    return range_type{ext_pos, s.size() - ext_pos};
}

auto str::rawname_view(std::string_view s) -> std::string_view {
    // return remove_extname_view(basename_view(s));
    auto range = rawname_range(s);
    return std::string_view{s.data() + range.begin_pos(), range.size()};
}

auto str::rawname(std::string_view s) -> std::string {
    return std::string{rawname_view(s)};
}

auto str::replace_rawname(std::string_view s, std::string_view new_name) -> std::string {
    std::string result;
    result.append(remove_basename_view(s)).append(new_name).append(extname_view(s));
    return result;
}

auto str::split_rawname_view(std::string_view s) -> std::tuple<std::string_view, std::string_view, std::string_view> {
    auto [dirname, basename] = split_basename_view(s);
    auto [rawname, extname] = split_extname_view(basename);
    return std::tuple{dirname, rawname, extname};
}

auto str::split_rawname(std::string_view s) -> std::tuple<std::string, std::string, std::string> {
    auto items = split_rawname_view(s);
    return std::tuple{std::string{std::get<0>(items)}, std::string{std::get<1>(items)},
        std::string{std::get<2>(items)}};
}

auto str::rawname_inplace(std::string& s) -> std::string& {
    s = rawname(s);
    return s;
}

auto str::replace_rawname_inplace(std::string& s, std::string_view name) -> std::string& {
    std::string_view rawname = rawname_view(s);
    assert((rawname.data() >= s.data()) && (rawname.data() < (s.data() + s.size())));
    size_type extlen = (s.data() + s.size()) - (rawname.data() + rawname.size());
    if (rawname.size() > name.size()) {
        std::memmove((s.data() + (rawname.data() - s.data())),
            (s.data() + (rawname.data() + rawname.size() - s.data())), extlen);
        s.resize(s.size() - rawname.size());
    } else {
        std::memmove((s.data() + (rawname.data() - s.data())),
            (s.data() + (rawname.data() + rawname.size() - s.data())), extlen);
        s.resize(s.size() - rawname.size());
    }
    return s;
}

// auto str::hash(std::string_view s, uint32_t mod) -> uint32_t {
// }

// auto str::hash(std::string_view s, uint64_t mod) -> uint64_t {
// }

// auto str::md5(std::string_view s) -> std::string {
// }

// auto str::md5(void* data, size_type n) -> std::string {
// }

// auto str::encode_xml(std::string_view s, const view_consumer_proc& proc) -> void;
// auto str::decode_xml(std::string_view s, const view_consumer_proc& proc) -> void;
// auto str::encode_xml(std::string_view s) -> std::string;
// auto str::decode_xml(std::string_view s) -> std::string;
// auto str::encode_xml_inplace(std::string& s) -> std::string&;
// auto str::decode_xml_inplace(std::string& s) -> std::string&;
// auto str::encode_url(std::string_view s, const view_consumer_proc& proc) -> void;
// auto str::decode_url(std::string_view s, const view_consumer_proc& proc) -> void;
// auto str::encode_url(std::string_view s) -> std::string;
// auto str::decode_url(std::string_view s) -> std::string;
// auto str::encode_url_inplace(std::string& s) -> std::string&;
// auto str::decode_url_inplace(std::string& s) -> std::string&;

auto str::encode_cstr(std::string_view s, const view_consumer_proc& proc) -> void {
    static std::string_view map[256]{
        "\\x00", // 0	0	00	NUL (null)
        "\\x01", // 1	1	01	SOH (start of header)
        "\\x02", // 2	2	02	STX (start of text)
        "\\x03", // 3	3	03	ETX (end of text)
        "\\x04", // 4	4	04	EOT (end of transmission)
        "\\x05", // 5	5	05	ENQ (enquiry)
        "\\x06", // 6	6	06	ACK (acknowledge)
        "\\a",   // 7	7	07	BEL (bell)
        "\\b",   // 8	10	08	BS (backspace)
        "\\t",   // 9	11	09	HT (horizontal tab)
        "\\n",   // 10	12	0a	LF (line feed - new line)
        "\\b",   // 11	13	0b	VT (vertical tab)
        "\\f",   // 12	14	0c	FF (form feed - new page)
        "\\r",   // 13	15	0d	CR (carriage return)
        "\\x0e", // 14	16	0e	SO (shift out)
        "\\x0f", // 15	17	0f	SI (shift in)
        "\\x10", // 16	20	10	DLE (data link escape)
        "\\x11", // 17	21	11	DC1 (device control 1)
        "\\x12", // 18	22	12	DC2 (device control 2)
        "\\x13", // 19	23	13	DC3 (device control 3)
        "\\x14", // 20	24	14	DC4 (device control 4)
        "\\x15", // 21	25	15	NAK (negative acknowledge)
        "\\x16", // 22	26	16	SYN (synchronous idle)
        "\\x17", // 23	27	17	ETB (end of transmission block)
        "\\x18", // 24	30	18	CAN (cancel)
        "\\x19", // 25	31	19	EM (end of medium)
        "\\x1a", // 26	32	1a	SUB (substitute)
        "\\x1b", // 27	33	1b	ESC (escape)
        "\\x1c", // 28	34	1c	FS (file separator)
        "\\x1d", // 29	35	1d	GS (group separator)
        "\\x1e", // 30	36	1e	RS (record separator)
        "\\x1f", // 31	37	1f	US (unit separator)
        " ",     // 32	40	20	(space)
        "!",     // 33	41	21	!
        "\\\"",  // 34	42	22	"
        "#",     // 35	43	23	#
        "$",     // 36	44	24	$
        "%",     // 37	45	25	%
        "&",     // 38	46	26	&
        "\\'",   // 39	47	27	'
        "(",     // 40	50	28	(
        ")",     // 41	51	29	)
        "*",     // 42	52	2a	*
        "+",     // 43	53	2b	+
        ",",     // 44	54	2c	,
        "-",     // 45	55	2d	-
        ".",     // 46	56	2e	.
        "/",     // 47	57	2f	/
        "0",     // 48	60	30	0
        "1",     // 49	61	31	1
        "2",     // 50	62	32	2
        "3",     // 51	63	33	3
        "4",     // 52	64	34	4
        "5",     // 53	65	35	5
        "6",     // 54	66	36	6
        "7",     // 55	67	37	7
        "8",     // 56	70	38	8
        "9",     // 57	71	39	9
        ":",     // 58	72	3a	:
        ";",     // 59	73	3b	;
        "<",     // 60	74	3c	<
        "=",     // 61	75	3d	=
        ">",     // 62	76	3e	>
        "?",     // 63	77	3f	?
        "@",     // 64	100	40	@
        "A",     // 65	101	41	A
        "B",     // 66	102	42	B
        "C",     // 67	103	43	C
        "D",     // 68	104	44	D
        "E",     // 69	105	45	E
        "F",     // 70	106	46	F
        "G",     // 71	107	47	G
        "H",     // 72	110	48	H
        "I",     // 73	111	49	I
        "J",     // 74	112	4a	J
        "K",     // 75	113	4b	K
        "L",     // 76	114	4c	L
        "M",     // 77	115	4d	M
        "N",     // 78	116	4e	N
        "O",     // 79	117	4f	O
        "P",     // 80	120	50	P
        "Q",     // 81	121	51	Q
        "R",     // 82	122	52	R
        "S",     // 83	123	53	S
        "T",     // 84	124	54	T
        "U",     // 85	125	55	U
        "V",     // 86	126	56	V
        "W",     // 87	127	57	W
        "X",     // 88	130	58	X
        "Y",     // 89	131	59	Y
        "Z",     // 90	132	5a	Z
        "[",     // 91	133	5b	[
        "\\\\",  // 92	134	5c	\ <---
        "]",     // 93	135	5d	]
        "^",     // 94	136	5e	^
        "_",     // 95	137	5f	_
        "`",     // 96	140	60	`
        "a",     // 97	141	61	a
        "b",     // 98	142	62	b
        "c",     // 99	143	63	c
        "d",     // 100	144	64	d
        "e",     // 101	145	65	e
        "f",     // 102	146	66	f
        "g",     // 103	147	67	g
        "h",     // 104	150	68	h
        "i",     // 105	151	69	i
        "j",     // 106	152	6a	j
        "k",     // 107	153	6b	k
        "l",     // 108	154	6c	l
        "m",     // 109	155	6d	m
        "n",     // 110	156	6e	n
        "o",     // 111	157	6f	o
        "p",     // 112	160	70	p
        "q",     // 113	161	71	q
        "r",     // 114	162	72	r
        "s",     // 115	163	73	s
        "t",     // 116	164	74	t
        "u",     // 117	165	75	u
        "v",     // 118	166	76	v
        "w",     // 119	167	77	w
        "x",     // 120	170	78	x
        "y",     // 121	171	79	y
        "z",     // 122	172	7a	z
        "{",     // 123	173	7b	{
        "|",     // 124	174	7c	|
        "}",     // 125	175	7d	}
        "~",     // 126	176	7e	~
        "\\x7f", // 127	177	7f	DEL (delete)
    };

    if (s.empty()) {
        return;
    }

    char buf[5]{};

    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(s.data());
    const uint8_t* end = reinterpret_cast<const uint8_t*>(s.data() + s.size());
    while (ptr < end) {
        if (*ptr <= 0x7f) [[likely]] {
            if (proc(map[*ptr]) != 0) {
                return;
            }
        } else {
            static char hexmap[]{"0123456789abcdef"};
            buf[0] = '\\';
            buf[1] = 'x';
            buf[2] = hexmap[(*ptr & 0xF0) >> 4];
            buf[3] = hexmap[(*ptr & 0x0F) >> 0];
            if (proc(std::string_view{buf, 4}) != 0) {
                return;
            }
        }

        ptr++;
    }
}

auto str::encode_cstr(std::string_view s) -> std::string {
    std::string result;
    result.reserve(result.size() * 4 / 3);
    str::encode_cstr(s, [&result](std::string_view item) -> int {
        result.append(item);
        return 0;
    });
    return result;
}

auto str::decode_cstr(std::string_view s, const view_consumer_proc& proc) -> void {
    if (s.empty()) {
        return;
    }

    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(s.data());
    const uint8_t* end = reinterpret_cast<const uint8_t*>(s.data() + s.size());

    const char* w = s.data();
    const char* c = s.data();
    for (; ptr < end; ptr++) {
        // clang-format off
        switch (*ptr) {
            case 'A'...'Z':
                [[fallthrough]];
            case 'a'...'z':
                [[fallthrough]];
            case '0'...'9':
                [[fallthrough]];
            case ' ':
                [[fallthrough]];
            case '!':
                [[fallthrough]];
            case '#':
                [[fallthrough]];
            case '$':
                [[fallthrough]];
            case '%':
                [[fallthrough]];
            case '&':
                [[fallthrough]];
            case '(':
                [[fallthrough]];
            case ')':
                [[fallthrough]];
            case '*':
                [[fallthrough]];
            case '+':
                [[fallthrough]];
            case ',':
                [[fallthrough]];
            case '-':
                [[fallthrough]];
            case '.':
                [[fallthrough]];
            case '/':
                [[fallthrough]];
            case ':':
                [[fallthrough]];
            case ';':
                [[fallthrough]];
            case '<':
                [[fallthrough]];
            case '=':
                [[fallthrough]];
            case '>':
                [[fallthrough]];
            case '?':
                [[fallthrough]];
            case '@':
                [[fallthrough]];
            case '[':
                [[fallthrough]];
            case ']':
                [[fallthrough]];
            case '^':
                [[fallthrough]];
            case '_':
                [[fallthrough]];
            case '`':
                [[fallthrough]];
            case '{':
                [[fallthrough]];
            case '|':
                [[fallthrough]];
            case '}':
                [[fallthrough]];
            case '~':
                c++;
                break;
            case '\\':
                if (w != c) {
                    if (proc({w, static_cast<size_t>(c - w)}) != 0) {
                        return;
                    }

                    w = c;
                }
                ptr++;
                if (ptr > end) {
                    return;
                }

                value_type ch = 0;
                switch (*ptr) {
                    case '\'':
                        ch = '\'';
                        break;
                    case '\"':
                        ch = '\"';
                        break;
                    case '?':
                        ch = '?';
                        break;
                    case 'a':
                        ch = '\a';
                        break;
                    case 'b':
                        ch = '\b';
                        break;
                    case 'f':
                        ch = '\f';
                        break;
                    case 'n':
                        ch = '\n';
                        break;
                    case 'r':
                        ch = '\r';
                        break;
                    case 't':
                        ch = '\t';
                        break;
                    case 'v':
                        ch = '\v';
                        break;
                    case '0':
                        [[fallthrough]];
                    case '1':
                        [[fallthrough]];
                    case '2':
                        [[fallthrough]];
                    case '3':
                        [[fallthrough]];
                    case '4':
                        [[fallthrough]];
                    case '5':
                        [[fallthrough]];
                    case '6':
                        [[fallthrough]];
                    case '7':
                        break;
                    case 'x':
                        break;
                }

                if (proc(std::string_view{&ch, 1}) != 0) {
                    return;
                }

                w = c;
                break;
        }
        // clang-format on
    }
}

auto str::decode_cstr(std::string_view s) -> std::string {
    std::string result;
    result.reserve(result.size() * 4 / 3);
    str::decode_cstr(s, [&result](std::string_view item) -> int {
        result.append(item);
        return 0;
    });
    return result;
}

auto str::encode_cstr_inplace(std::string& s) -> std::string {
    s = encode_cstr(s);
    return s;
}

auto str::decode_cstr_inplace(std::string& s) -> std::string {
    s = decode_cstr(s);
    return s;
}

auto str::encode_base64(std::string_view s, const view_consumer_proc& proc) -> void {
    if (s.empty()) {
        proc({});
        return;
    }

    // base64 的数据编码表
    static const value_type table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    // 数据输入源
    auto src = reinterpret_cast<const uint8_t*>(s.data());

    // 不足3字节的长度
    size_t mod = s.size() % 3;

    // 整3字节的长度
    size_t mod_len = s.size() - mod;

    // 用于临时交换数据的缓冲区
    uint8_t out[4];

    str::size_type pos = 0;
    for (pos = 0; pos < mod_len; pos += 3) {
        // 将3字节组合在一起(总共24bit)
        uint32_t cache = ((uint32_t)(src[pos]) << 16) + ((uint32_t)(src[pos + 1]) << 8) + (uint32_t)(src[pos + 2]);

        // 拆分成4字节(每个字节实际只有6bit，对应0x3f的掩码)
        out[0] = table[static_cast<uint8_t>((cache >> 18) & 0x3f)];
        out[1] = table[static_cast<uint8_t>((cache >> 12) & 0x3f)];
        out[2] = table[static_cast<uint8_t>((cache >> 6) & 0x3f)];
        out[3] = table[static_cast<uint8_t>((cache >> 0) & 0x3f)];

        // 输出一部分数据
        if (proc(std::string_view(reinterpret_cast<const_pointer>(out), 4)) != 0) {
            return;
        }
    }

    // 输出最后一部分
    if (mod == 1) {
        uint32_t cache = (uint32_t)src[pos] << 16;
        out[0] = table[(cache >> 18) & 0x3f];
        out[1] = table[(cache >> 12) & 0x3f];
        out[2] = '=';
        out[3] = '=';
        proc(std::string_view(reinterpret_cast<const_pointer>(out), 4));
    } else if (mod == 2) {
        uint32_t cache = ((uint32_t)(src[pos]) << 16) + ((uint32_t)(src[pos + 1]) << 8);
        out[0] = table[(cache >> 18) & 0x3f];
        out[1] = table[(cache >> 12) & 0x3f];
        out[2] = table[(cache >> 6) & 0x3f];
        out[3] = '=';
        proc(std::string_view(reinterpret_cast<const_pointer>(out), 4));
    }
}

auto str::encode_base64(std::string_view s) -> std::string {
    std::string result;
    result.reserve(((s.size() / 3 + ((s.size() % 3) ? 1 : 0)) * 4));
    str::encode_base64(s, [&result](std::string_view item) -> int {
        result.append(item);
        return 0;
    });
    return result;
}

auto str::decode_base64(std::string_view s, const view_consumer_proc& proc) -> void {
    static uint8_t table[256]{};
    if (table['+'] == 0) {
        table['+'] = 62;
        table['/'] = 63;

        table['0'] = 52;
        table['1'] = 53;
        table['2'] = 54;
        table['3'] = 55;
        table['4'] = 56;
        table['5'] = 57;
        table['6'] = 58;
        table['7'] = 59;
        table['8'] = 60;
        table['9'] = 61;
        table['='] = 0;
        table['A'] = 0;
        table['B'] = 1;
        table['C'] = 2;
        table['D'] = 3;
        table['E'] = 4;
        table['F'] = 5;
        table['G'] = 6;
        table['H'] = 7;
        table['I'] = 8;
        table['J'] = 9;
        table['K'] = 10;
        table['L'] = 11;
        table['M'] = 12;
        table['N'] = 13;
        table['O'] = 14;
        table['P'] = 15;
        table['Q'] = 16;
        table['R'] = 17;
        table['S'] = 18;
        table['T'] = 19;
        table['U'] = 20;
        table['V'] = 21;
        table['W'] = 22;
        table['X'] = 23;
        table['Y'] = 24;
        table['Z'] = 25;
        table['a'] = 26;
        table['b'] = 27;
        table['c'] = 28;
        table['d'] = 29;
        table['e'] = 30;
        table['f'] = 31;
        table['g'] = 32;
        table['h'] = 33;
        table['i'] = 34;
        table['j'] = 35;
        table['k'] = 36;
        table['l'] = 37;
        table['m'] = 38;
        table['n'] = 39;
        table['o'] = 40;
        table['p'] = 41;
        table['q'] = 42;
        table['r'] = 43;
        table['s'] = 44;
        table['t'] = 45;
        table['u'] = 46;
        table['v'] = 47;
        table['w'] = 48;
        table['x'] = 49;
        table['y'] = 50;
        table['z'] = 51;
    }

    if (s.empty()) {
        proc(std::string_view{});
        return;
    }

    assert((s.size() % 4) == 0);

    size_t postfix_len = 0;
    if (s[s.size() - 1] == '=') {
        postfix_len += (s[s.size() - 2] == '=') ? 2 : 1;
    }

    uint8_t o[4] = {};
    auto src = reinterpret_cast<const uint8_t*>(s.data());
    for (size_t i = 0; i < s.size() / 4; i++) {
        uint8_t t[4]{};
        t[0] = table[src[i * 4]];
        t[1] = table[src[i * 4 + 1]];
        t[2] = table[src[i * 4 + 2]];
        t[3] = table[src[i * 4 + 3]];

        o[0] = ((t[0] & 0x3f) << 2) | ((t[1] & 0x30) >> 4);
        o[1] = ((t[1] & 0x0f) << 4) | ((t[2] & 0x3c) >> 2);
        o[2] = ((t[2] & 0x03) << 6) | ((t[3] & 0x3f) >> 0);

        size_t n = 3;
        if ((i + 4) >= s.size()) [[unlikely]] {
            n = postfix_len;
        }

        if (proc(std::string_view{reinterpret_cast<const_pointer>(o), n}) != 0) {
            return;
        }
    }
}

auto str::decode_base64(std::string_view s) -> std::string {
    std::string result;
    str::decode_base64(s, [&result](std::string_view item) -> int {
        result.append(item);
        return 0;
    });
    return result;
}

auto str::encode_base64_inplace(std::string& s) -> std::string& {
    s = encode_base64(s);
    return s;
}

auto str::decode_base64_inplace(std::string& s) -> std::string& {
    s = decode_base64(s);
    return s;
}

auto str::encode_base16(std::string_view s, bool upper, const view_consumer_proc& proc) -> void {
    if (s.empty()) {
        proc({});
        return;
    }

    static const value_type table_lower[] = "0123456789abcdef";
    static const value_type table_upper[] = "0123456789ABCDEF";
    const value_type* table = upper ? table_upper : table_lower;

    auto src = reinterpret_cast<const uint8_t*>(s.data());
    auto end = reinterpret_cast<const uint8_t*>(s.data() + s.size());

    value_type o[2] = {};
    while (src < end) {
        o[0] = table[(*src & 0xF0) >> 4];
        o[1] = table[*src & 0x0F];
        if (proc(std::string_view{reinterpret_cast<const_pointer>(o), 2}) != 0) {
            break;
        }
        src++;
    }
}

auto str::encode_base16(std::string_view s, bool upper) -> std::string {
    std::string result;
    str::encode_base16(s, upper, [&result](std::string_view item) -> int {
        result.append(item);
        return 0;
    });
    return result;
}

auto str::decode_base16(std::string_view s, const view_consumer_proc& proc) -> void {
    assert((s.size() % 2) == 0);

    if (s.empty()) {
        proc({});
        return;
    }

    auto src = reinterpret_cast<const uint8_t*>(s.data());
    auto end = reinterpret_cast<const uint8_t*>(s.data() + s.size());
    value_type o[2]{};
    while (src < end) {
        switch (src[0]) {
            case '0' ... '9':
                o[1] = static_cast<value_type>(src[0] - '0');
                break;
            case 'A' ... 'F':
                o[1] = static_cast<value_type>(src[0] - 'A' + 10);
                break;
            case 'a' ... 'f':
                o[1] = static_cast<value_type>(src[0] - 'a' + 10);
                break;
            default:
                assert(false);
                break;
        }
        switch (*(src + 1)) {
            case '0' ... '9':
                o[0] = static_cast<value_type>((o[0] << 4) | (src[1] - '0'));
                break;
            case 'A' ... 'F':
                o[0] = static_cast<value_type>((o[0] << 4) | (src[1] - '0' + 10));
                break;
            case 'a' ... 'f':
                o[0] = static_cast<value_type>((o[0] << 4) | (src[1] - 'a' + 10));
                break;
            default:
                assert(false);
                break;
        }

        if (proc(std::string{o, 1}) != 0) {
            return;
        }

        src += 2;
    }
}

auto str::decode_base16(std::string_view s) -> std::string {
    std::string result;
    str::decode_base16(s, [&result](std::string_view item) -> int {
        result.append(item);
        return 0;
    });
    return result;
}

auto str::encode_base16_inplace(std::string& s, bool upper) -> std::string& {
    s = encode_base16(s, upper);
    return s;
}

auto str::decode_base16_inplace(std::string& s) -> std::string& {
    s = decode_base16(s);
    return s;
}

auto str::dump_hex_offset(size_type offset, uint8_t offset_width, bool upper, std::string& line) -> void {
    dump_hex_offset(offset, offset_width, upper, [&line](std::string_view text) -> int {
        line += text;
        return 0;
    });
}

auto str::dump_hex_offset(size_type offset, uint8_t offset_width, bool upper, const view_consumer_proc& proc) -> void {
    value_type offset_buffer[32];
    int wlen = snprintf(offset_buffer, sizeof(offset_buffer), (upper ? "%lX" : "%lx"), offset);
    assert(wlen > 0);
    str::align_zfill(std::string_view{offset_buffer, static_cast<size_type>(wlen)}, offset_width, proc);
}

auto str::dump_hex_ascii(const void* data, size_type len, value_type ascii_mask, std::string& line) -> void {
    if ((data == nullptr) || (len == 0)) {
        return;
    }

    dump_hex_ascii(data, len, ascii_mask, [&line](std::string_view text) -> int {
        line += text;
        return 0;
    });
}

auto str::dump_hex_ascii(const void* data, size_type len, value_type ascii_mask, const view_consumer_proc& proc) -> void {
    if ((data == nullptr) || (len == 0)) {
        return;
    }

    std::string line;
    line.resize(len);

    const_pointer ptr = static_cast<const_pointer>(data);
    for (size_type index = 0; index < len; index++) {
        line += std::isprint(ptr[index]) ? ptr[index] : ascii_mask;
    }

    proc(line);
}

auto str::dump_hex_groups(const void* data, size_type len, uint8_t group_bytes, bool upper, std::string& line) -> size_type {
    if ((data == nullptr) || (len == 0)) {
        return 0;
    }

    return dump_hex_groups(data, len, group_bytes, upper, [&line](std::string_view text) -> int {
        line += text;
        return 0;
    });
}

auto str::dump_hex_groups(const void* data, size_type len, uint8_t group_bytes, bool upper,
    const view_consumer_proc& proc)
    -> size_type {
    if ((data == nullptr) || (len == 0)) {
        return 0;
    }

    std::string_view hex = upper ? all_hex_upper : all_hex_lower;

    std::string line;

    // 处理完整组
    const_pointer ptr_group = static_cast<const_pointer>(data);
    for (size_type group_index = 0; group_index < len / group_bytes; group_index++) {
        for (size_type byte_index = 0; byte_index < group_bytes; byte_index++) {
            uint8_t ch = static_cast<uint8_t>(ptr_group[byte_index]);
            line += hex[(ch & 0xF0) >> 4];
            line += hex[(ch & 0x0F) >> 0];
        }

        line += ' ';
        ptr_group += group_bytes;
    }

    // 处理不完整的分组
    for (size_type byte_index = 0; byte_index < len % group_bytes; byte_index++) {
        uint8_t ch = static_cast<uint8_t>(ptr_group[byte_index]);
        line += hex[(ch & 0xF0) >> 4];
        line += hex[(ch & 0x0F) >> 0];
    }

    proc(line);

    return line.size();
}

auto str::dump_hex(const void* data, size_type len, const dump_hex_format& format, const line_consumer_proc& proc) -> void {
    if ((data == nullptr) || (len == 0)) {
        return;
    }

    bool upper = (format.flags & dump_hex_format::show_upper) != 0;

    // 每行多少字节
    size_type line_bytes = format.line_groups * format.group_bytes;
    size_type groups_len = line_bytes + format.line_groups;

    // 完整行的数量
    size_type full_line_num = len / line_bytes;

    std::string line;
    size_type line_index = 0;
    const_pointer ptr_line = static_cast<const_pointer>(data);
    for (; line_index < full_line_num; line_index++) {
        line.clear();

        // Dump offset
        if (format.flags & dump_hex_format::show_offset) {
            dump_hex_offset(line_index * line_bytes, upper, format.offset_width, line);
            line += format.offset_margin;
        }

        // Dump groups
        [[maybe_unused]] auto dumped_groups_len = dump_hex_groups(ptr_line, line_bytes, format.group_bytes, upper, line);
        assert(dumped_groups_len == groups_len);

        // Dump ascii
        if (format.flags & dump_hex_format::show_ascii) {
            dump_hex_ascii(ptr_line, line_bytes, format.ascii_mask, line);
            line += format.ascii_margin;
        }

        // 输出当前行
        if (proc(line_index, line) != 0) {
            return;
        }

        ptr_line += line_bytes;
    }

    // 处理不完整行
    if ((full_line_num * line_bytes) < len) {
        line.clear();
        const_pointer ptr_line = static_cast<const_pointer>(data) + line_index * line_bytes;
        size_type remain_len = len - (full_line_num * line_bytes);

        // Dump offset
        if (format.flags & dump_hex_format::show_offset) {
            dump_hex_offset(line_index * line_bytes, format.offset_width, upper, line);
            line += format.offset_margin;
        }

        // Dump groups
        size_type dumped_groups_len = dump_hex_groups(ptr_line, remain_len, format.group_bytes, upper, line);
        for (size_type index = 0; index < (groups_len - dumped_groups_len); index++) {
            line += ' ';
        }

        // Dump ascii
        if (format.flags & dump_hex_format::show_ascii) {
            dump_hex_ascii(ptr_line, line_bytes, format.ascii_mask, line);
            line += format.ascii_margin;
        }

        // 输出当前行
        if (proc(line_index, line) != 0) {
            return;
        }
    }
}

auto str::charset(std::string_view s) -> charset_type {
    return charset_type{s};
}

auto str::charset(std::string_view s, charset_type& set) -> charset_type& {
    set.set(s);
    return set;
}

auto str::read_all(const std::string& filename) -> std::string {
    return read_all(filename.c_str());
}

auto str::read_all(const char* filename) -> std::string {
    assert(filename != nullptr);

    std::string result;

    FILE* file = fopen(filename, "r");
    if (file == nullptr) {
        return result;
    }

    struct stat buff {};
    if (fstat(fileno(file), &buff) != 0) {
        return result;
    }

    result.resize(buff.st_size);

    errno = 0;
    size_t n = fread(result.data(), 1, buff.st_size, file);
    if (n == 0) {
        return result;
    }

    assert(n <= result.size());

    result.resize(n);

    return result;
}

auto str::read_line(FILE* file, bool keep_ends) -> std::optional<std::string> {
    if (file == nullptr) {
        return std::nullopt;
    }

    if (feof(file) || ferror(file)) {
        return std::nullopt;
    }

    std::string result;

    char buffer[512];

    do {
        char* ptr = fgets(buffer, sizeof(buffer), file);
        if (ptr == nullptr) {
            break;
        }

        size_type len = strlen(ptr);
        result.append(buffer, len);

        // fgets 会自动补字符串尾部的\0，所以长度一定小于缓冲区长度
        assert(len < sizeof(buffer));

        // 缓冲区没填充满，说明结束了
        if (len >= (sizeof(buffer) - 1)) {
            // 如果缓冲区结尾刚好就是换行符号，说明遇到结束符了: [\n][\0]$
            if (buffer[sizeof(buffer) - 2] == '\n') [[unlikely]] {
                break;
            }
        }
    } while (!feof(file) && !ferror(file));

    if (!keep_ends) {
        return remove_eol_suffix(result);
    }

    return result;
}

auto str::read_line(std::istream& file) -> std::optional<std::string> {
    if (file.eof() || file.bad()) {
        return std::nullopt;
    }

    std::string line_text;
    std::getline(file, line_text);
    return line_text;
}

auto str::read_lines(FILE* file, const line_consumer_proc& proc) -> void {
    assert(file != nullptr);

    size_type line_index = 0;
    std::string line_text;

    char buffer[512];
    while (!feof(file) || !ferror(file)) {
        char* ptr = fgets(buffer, sizeof(buffer), file);
        if (ptr == nullptr) {
            proc(line_index, line_text);
            return;
        }

        size_type len = strlen(ptr);
        line_text.append(buffer, len);

        // "fgets 会自动补字符串尾部的\\0，所以长度一定小于缓冲区长度"
        assert(len < sizeof(buffer));

        // 缓冲区已经填满，需要看尾部换行以裁定是否遇到行尾
        if (len >= (sizeof(buffer) - 1)) {
            // 缓冲区已经填满，需要看尾部换行以裁定是否遇到行尾
            if (buffer[sizeof(buffer) - 2] != '\n') [[unlikely]] {
                continue;
            }
        }

        if (proc(line_index, line_text) != 0) {
            return;
        }

        line_index++;
        line_text.clear();
    }
}

auto str::read_lines(FILE* file, size_type max_n) -> std::vector<std::string> {
    if (max_n == 0) {
        return {};
    }

    std::vector<std::string> result;
    read_lines(file, [max_n, &result](size_type line_index [[maybe_unused]], std::string_view line_text) -> int {
        result.emplace_back(line_text);
        if (result.size() == max_n) {
            return 1;
        }

        return 0;
    });

    return result;
}

auto str::read_lines(std::istream& file, const line_consumer_proc& proc) -> void {
    size_type line_index = 0;
    std::string line_text;
    while (!file.bad() && !file.eof()) {
        std::getline(file, line_text);
        if (proc(line_index, line_text) != 0) {
            return;
        }
        line_index++;
        line_text.clear();
    }
}

auto str::read_lines(std::istream& file, size_type max_n) -> std::vector<std::string> {
    if (max_n == 0) {
        return {};
    }

    std::vector<std::string> result;
    read_lines(file, [max_n, &result](size_type line_index [[maybe_unused]], std::string_view line_text) -> int {
        result.emplace_back(line_text);
        if (result.size() == max_n) {
            return 1;
        }

        return 0;
    });

    return result;
}

auto str::read_lines(const std::string& filename, const line_consumer_proc& proc) -> void {
    read_lines(filename.c_str(), proc);
}

auto str::read_lines(const char* filename, const line_consumer_proc& proc) -> void {
    assert(filename != nullptr);

    std::ifstream file{filename};
    str::read_lines(file, proc);
}

auto str::read_lines(const std::string& filename, size_type max_n) -> std::vector<std::string> {
    return read_lines(filename.c_str(), max_n);
}

auto str::read_lines(const char* filename, size_type max_n) -> std::vector<std::string> {
    assert(filename != nullptr);
    std::ifstream file{filename};
    return str::read_lines(file, max_n);
}

auto str::with_file(const std::string& filepath, const char* mode, const std::function<void(FILE* file)>& proc) -> void {
    std::unique_ptr<FILE, void (*)(FILE* file)> file{fopen(filepath.c_str(), mode), //
        [](FILE* file) -> void {
            if (file != nullptr) {
                fclose(file);
            }
        }};
    if (file == nullptr) {
        return;
    }

    proc(file.get());
}

class argv_view {
public:
    using size_type = int;
    using value_type = char*;
    using const_iterator = const char**;

    argv_view(int argc, char* argv[])
        : argc_{argc}
        , argv_{argv} {
        assert(argc >= 0);
    }

    inline auto size() const -> int {
        assert(argc_ >= 0);
        return argc_;
    }

    inline auto operator[](size_type index) const -> value_type {
        return argv_[index];
    }

private:
    int argc_;
    char** argv_;
};

auto str::next_opt(int& next_index, int n, char* items[]) -> std::tuple<std::string_view, std::string_view> {
    return next_opt(next_index, argv_view{n, items});
}

auto str::skip_spaces(std::string_view s, size_type& pos) -> void {
    if (pos > s.size()) {
        pos = s.size();
        return;
    }

    for (; pos < s.size(); ++pos) {
        if (!std::isspace(s[pos])) {
            break;
        }
    }
}

// auto str::accept_literal_integer(std::string_view s, size_type& pos) -> range_type;
// auto str::accept_literal_real(std::string_view s, size_type& pos) -> range_type;
// auto str::accept_literal_string(std::string_view s, size_type& pos) -> range_type;
// auto str::accept_identifier(std::string_view s, size_type& pos) -> range_type;
// auto str::accept_next_word(std::string_view s, size_type& pos) -> range_type;
// auto str::accept_email(std::string_view s, size_type& pos) -> range_type;

// auto str::accept_until(std::string_view s, size_type& pos, std::string_view sep_str) -> range_type;
// auto str::accept_until(std::string_view s, size_type& pos, const charset_type& sep_charset) -> range_type;
// auto str::accept_until(std::string_view s, size_type& pos, const char_match_proc& proc) -> range_type;

// auto str::accept(std::string_view s, size_type& pos, std::string_view sep_str) -> range_type;
// auto str::accept(std::string_view s, size_type& pos, const charset_type& sep_charset) -> range_type;
// auto str::accept(std::string_view s, size_type& pos, const char_match_proc& proc) -> range_type;

// auto str::split_email(std::string_view s, const view_consumer_proc& proc) -> void;
// auto str::split_email(std::string_view s) -> std::vector<std::string>;
// auto str::split_email_view(std::string_view s) -> std::vector<std::string_view>;
// auto str::split_real(std::string_view s, const view_consumer_proc& proc) -> void;
// auto str::split_real(std::string_view s) -> std::vector<std::string>;
// auto str::split_real_view(std::string_view s) -> std::vector<std::string_view>;
// auto str::split_uri(std::string_view s, const view_consumer_proc& proc) -> void;
// auto str::split_uri(std::string_view s) -> std::vector<std::string>;
// auto str::split_uri_view(std::string_view s) -> std::vector<std::string_view>;
// auto str::split_ipv4(std::string_view s, const view_consumer_proc& proc) -> void;
// auto str::split_ipv4(std::string_view s) -> std::vector<std::string>;
// auto str::split_ipv4_view(std::string_view s) -> std::vector<std::string_view>;

auto str::grouping(std::string_view s, const char_match_proc& proc) -> std::tuple<std::string, std::string> {
    std::string matched;
    std::string unmatched;

    for (const_pointer ptr = s.data(); ptr < s.data() + s.size(); ptr++) {
        if (proc(*ptr)) {
            matched.append(1, *ptr);
        } else {
            unmatched.append(1, *ptr);
        }
    }

    return {matched, unmatched};
}

auto str::filter(std::string_view s, const char_match_proc& proc) -> std::string {
    std::string result;
    for (const_pointer rptr = s.data(); rptr < (s.data() + s.size()); rptr++) {
        if (proc(*rptr)) {
            result.append(1, *rptr);
        }
    }
    return result;
}

auto str::filter(std::string_view s, const charset_type& charset) -> std::string {
    std::string result;
    for (const_pointer rptr = s.data(); rptr < (s.data() + s.size()); rptr++) {
        if (charset.get(*rptr)) {
            result.append(1, *rptr);
        }
    }
    return result;
}

auto str::filter_inplace(std::string& s, const char_match_proc& proc) -> std::string& {
    if (s.empty()) {
        return s;
    }

    pointer wptr = s.data();

    for (const_pointer rptr = s.data(); rptr < (s.data() + s.size()); rptr++) {
        if (proc(*rptr)) {
            *(wptr++) = *rptr;
        }
    }

    s.resize(wptr - s.data());
    return s;
}

auto str::filter_inplace(std::string& s, const charset_type& charset) -> std::string& {
    if (s.empty()) {
        return s;
    }

    pointer wptr = s.data();

    for (const_pointer rptr = s.data(); rptr < (s.data() + s.size()); rptr++) {
        if (charset.get(*rptr)) {
            *(wptr++) = *rptr;
        }
    }

    s.resize(wptr - s.data());
    return s;
}

auto str::mapping(std::string_view s, std::string_view match_charset, std::string_view replace_charset) -> std::string {
    if (match_charset.empty() || replace_charset.empty()) {
        return std::string{s};
    }

    value_type maptable[256]{'\0'};
    for (size_type index = 0; index < std::min(match_charset.size(), replace_charset.size()); ++index) {
        maptable[static_cast<std::make_unsigned_t<value_type>>(match_charset[index])] = replace_charset[index];
    }

    return mapping(s, [maptable](value_type ch) -> value_type {
        auto index = static_cast<std::make_unsigned_t<value_type>>(ch);
        return (maptable[index] != '\0') ? maptable[index] : ch;
    });
}

auto str::mapping(std::string_view s, const char_mapping_proc& proc) -> std::string {
    std::string result;
    result.reserve(s.size());

    for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ++ptr) {
        result.append(proc(*ptr), 1);
    }

    return result;
}

auto str::mapping_inplace(std::string& s, std::string_view match_charset,
    std::string_view replace_charset) -> std::string& {
    if (match_charset.empty() || replace_charset.empty()) {
        return s;
    }

    value_type maptable[256]{'\0'};
    for (size_type index = 0; index < std::min(match_charset.size(), replace_charset.size()); ++index) {
        maptable[static_cast<std::make_unsigned_t<value_type>>(match_charset[index])] = replace_charset[index];
    }

    return mapping_inplace(s, [maptable](value_type ch) -> value_type {
        auto index = static_cast<std::make_unsigned_t<value_type>>(ch);
        return (maptable[index] != '\0') ? maptable[index] : ch;
    });
}

auto str::mapping_inplace(std::string& s, const char_mapping_proc& proc) -> std::string& {
    for (pointer ptr = s.data(); ptr < (s.data() + s.size()); ++ptr) {
        *ptr = proc(*ptr);
    }

    return s;
}

//============================

auto str::append(std::string_view s, const view_provider_proc& proc) -> std::string {
    std::string result{s};
    auto item = proc();
    while (item) {
        result.append(item.value());
        item = proc();
    }

    return result;
}

auto str::append_inplace(std::string& s, const view_provider_proc& proc) -> std::string& {
    auto item = proc();
    while (item) {
        s.append(item.value());
        item = proc();
    }

    return s;
}

auto str::prepend(std::string_view s, const view_provider_proc& proc) -> std::string {
    std::string result;
    auto item = proc();
    while (item) {
        result.insert(0, item.value());
        item = proc();
    }
    result.append(s);
    return result;
}

auto str::prepend_inplace(std::string& s, const view_provider_proc& proc) -> std::string& {
    auto item = proc();
    while (item) {
        prepend_inplace(s, item.value(), 1);
        item = proc();
    }

    return s;
}

auto str::insert(std::string_view s, size_type pos, const view_provider_proc& proc) -> std::string {
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

auto str::insert_inplace(std::string& s, size_type pos, const view_provider_proc& proc) -> std::string& {
    auto item = proc();
    while (item) {
        str::insert_inplace(s, pos, item.value(), 1); // TODO 不是最优的，会导致多次扩容
        item = proc();
    }

    return s;
}

auto str::foreach_words(std::string_view s, size_type pos, const range_consumer_proc& proc) -> void {
    while (pos < s.size()) {
        range_type r = str::next_word_range(s, pos);
        if (r.empty()) {
            assert(pos >= s.size());
            break;
        }

        if (proc(r) != 0) {
            return;
        }
    }
}

auto str::foreach_words(std::string_view s, size_type pos, const view_consumer_proc& proc) -> void {
    str::foreach_words(s, pos, [&s, &proc](range_type range) -> int {
        return proc(std::string_view{s.data() + range.pos, range.len});
    });
}

auto str::foreach_words(std::string_view s, const range_consumer_proc& proc) -> void {
    str::foreach_words(s, 0, proc);
}

auto str::foreach_words(std::string_view s, const view_consumer_proc& proc) -> void {
    str::foreach_words(s, 0, [&s, &proc](range_type range) -> int {
        return proc(std::string_view{s.data() + range.pos, range.len});
    });
}

auto str::foreach_lines(std::string_view s, bool keep_ends, const line_consumer_proc& proc) -> void {
    size_type line_index = 0;
    size_type line_start = 0;
    size_type pos = 0;
    while (pos < s.size()) {
        auto range = next_eol_range(s, pos);
        if (!range) {
            break;
        }

        size_type endpos = keep_ends ? range->end_pos() : range->begin_pos();
        if (proc(line_index++, s.substr(line_start, endpos - line_start)) != 0) {
            return;
        }
    }

    if (pos < s.size()) {
        proc(line_index++, s.substr(line_start, (s.size() - line_start)));
    }
}
