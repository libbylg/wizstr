//
// Created by libbylg on 2023/6/1.
//
#include "str.h"
#include "view.h"

#include <fstream>
#include <cassert>

auto str::append(std::string& s, std::string_view other) -> std::string& {
    return s.append(other);
}

auto str::append(std::string& s, std::string_view other, size_type n) -> std::string& {
    s.reserve(s.size() + other.size() * n);
    for (size_type i = 0; i < n; i++) {
        s.append(other);
    }
    return s;
}

auto str::append(std::string& s, str::value_type ch) -> std::string& {
    return s.append(&ch, 1);
}

auto str::append(std::string& s, value_type ch, size_type n) -> std::string& {
    s.resize(s.size() + n, ch);
    return s;
}

auto str::append(std::string& s, const view_provider_proc& proc) -> std::string& {
    const auto item = proc();
    while (item) {
        s.append(item.value());
    }

    return s;
}

auto str::append(std::string& s, std::initializer_list<std::string_view> others) -> std::string& {
    // 先计算出需要追加的数据的总长度
    size_t append_size = 0;
    for (auto itr = others.begin(); itr != others.end(); itr++) {
        append_size += itr->size();
    }

    // 做一个一次性扩容
    s.reserve(s.size() + append_size);

    // 再逐个添加
    for (auto itr = others.begin(); itr != others.end(); itr++) {
        s.append(*itr);
    }

    return s;
}

auto str::prepend(std::string& s, std::string_view other) -> std::string& {
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

auto str::prepend(std::string& s, std::string_view other, size_type n) -> std::string& {
    size_type require_size = s.size() + other.size() * n;
    if (s.capacity() >= require_size) {
        size_type origin_size = s.size();
        s.resize(require_size);
        std::memmove(s.data() + (other.size() * n), s.data(), origin_size);
        for (size_type i = 0; i < n; i++) {
            std::memcpy(s.data() + (other.size() * i), other.data(), other.size());
        }

        return s;
    }

    // 反正要分配新内存，还不如创建新对象
    std::string result;
    result.resize(require_size);
    for (size_type i = 0; i < n; i++) {
        std::memcpy(result.data() + (other.size() * i), other.data(), other.size());
    }
    std::memcpy(result.data() + (other.size() * n), s.data(), s.size());

    // result 赋值给 s
    s = std::move(result);

    return s;
}

auto str::prepend(std::string& s, value_type ch) -> std::string& {
    return prepend(s, std::string_view{&ch, 1});
}

auto str::prepend(std::string& s, value_type ch, size_type n) -> std::string& {
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

auto str::prepend(std::string& s, const view_provider_proc& proc) -> std::string& {
    const auto item = proc();
    while (item) {
        prepend(s, item.value());
    }

    return s;
}

auto str::insert(std::string& s, size_type pos, std::string_view other) -> std::string& {
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

auto str::insert(std::string& s, size_type pos, std::string_view other, size_type n) -> std::string& {
    // TODO 性能不是最优的，主要是会导致多次扩容
    size_type count = 0;
    str::insert(s, pos, [n, &count, &other]() -> std::optional<std::string_view> {
        if (count >= n) {
            return {};
        }

        count++;
        return other;
    });

    return s;
}

auto str::insert(std::string& s, size_type pos, value_type ch) -> std::string& {
    return insert(s, pos, std::string_view{&ch, 1});
}

auto str::insert(std::string& s, size_type pos, value_type ch, size_type n) -> std::string& {
    if (pos >= s.size()) {
        s.append(ch, n);
        return s;
    }

    if (s.capacity() > (s.size() + n)) {
        s.resize(s.size() + n);
        std::memmove(s.data() + pos + n, s.data() + pos, (s.size() - pos));
        std::fill(s.data() + pos, s.data() + pos + n, ch);
        return s;
    }

    std::string result;
    result.reserve(s.size() + n);
    result.append(s.data(), pos);
    result.append(ch, n);
    result.append(s.data() + pos, s.size() - pos);
    s = std::move(result);
    return s;
}

auto str::insert(std::string& s, size_type pos, const view_provider_proc& proc) -> std::string& {
    auto item = proc();
    while (item) {
        str::insert(s, pos, item.value()); // TODO 不是最优的，会导致多次扩容
    }

    return s;
}

auto str::pop_back_char(std::string& s) -> value_type {
    if (s.empty()) {
        return '\0';
    }

    value_type ch = s.back();
    s.resize(s.size() - 1);
    return ch;
}

auto str::pop_front_char(std::string& s) -> value_type {
    value_type ch = s.front();
    std::memmove(s.data(), s.data() + 1, s.size() - 1);
    s.resize(s.size() - 1);
    return ch;
}

// auto str::pop_back_word(std::string& s) -> std::string {
// }

// auto str::pop_front_word(std::string& s) -> std::string {
// }

// auto str::remove_back_word(std::string& s) -> std::string& {
// }

// auto str::remove_front_word(std::string& s) -> std::string& {
// }

// auto str::flex_move(std::string& s, size_type pos, size_type n, ssize_type offset) -> std::string& {
// }

// auto str::flex_move(std::string& s, size_type pos, size_type n, ssize_type offset, value_type ch) -> std::string& {
// }

// auto str::clip_move(std::string& s, size_type pos, size_type n, ssize_type offset) -> std::string& {
// }

auto str::remove(std::string& s, size_type pos) -> std::string& {
    if (pos >= s.size()) {
        return s;
    }

    s.resize(pos);
    return s;
}

auto str::remove(std::string& s, size_type pos, size_type n) -> std::string& {
    if ((pos >= s.size()) || (n == 0) || (s.empty())) {
        return s;
    }

    if (n >= (s.size() - pos)) {
        s.resize(pos);
        return s;
    }

    std::memmove(s.data() + pos, s.data() + pos + n, (s.size() - pos - n));
    s.resize(s.size() - n);
    return s;
}

auto str::remove(std::string& s, scope_t scope) -> std::string& {
    size_type len = (scope.offset > 0) ? scope.offset : -scope.offset;
    size_type pos = (scope.offset > 0) ? scope.pos : (scope.pos + scope.offset);
    return str::remove(s, pos, len);
}

// auto str::remove(std::string& s, value_type ch) -> std::string& {
// }

// auto str::remove(std::string& s, std::string_view other) -> std::string& {
// }

// auto str::remove(std::string& s, char_match_proc proc) -> std::string& {
// }

auto str::remove_prefix(std::string& s, std::string_view prefix) -> std::string& {
    if (!view::has_prefix(s, prefix)) {
        return s;
    }

    std::memmove(s.data(), s.c_str() + prefix.size(), s.size() - prefix.size());
    s.resize(s.size() - prefix.size());
    return s;
}

auto str::remove_prefix(std::string& s, value_type prefix) -> std::string& {
    return remove_prefix(s, {&prefix, 1});
}

auto str::remove_prefix(std::string& s, size_type n) -> std::string& {
    if (n >= s.size()) {
        s.clear();
        return s;
    }

    std::memmove(s.data(), s.data() + n, (s.size() - n));
    s.resize(s.size() - n);
    return s;
}

auto str::remove_suffix(std::string& s, std::string_view suffix) -> std::string& {
    if (!view::has_prefix(s, suffix)) {
        return s;
    }

    s.resize(s.size() - suffix.size());
    return s;
}

auto str::remove_suffix(std::string& s, value_type suffix) -> std::string& {
    return remove_suffix(s, {&suffix, 1});
}

auto str::remove_suffix(std::string& s, size_type n) -> std::string& {
    if (n >= s.size()) {
        s.clear();
        return s;
    }

    s.resize(s.size() - n);
    return s;
}

auto str::fill(std::string& s, std::string_view other, size_type pos, size_type max_n) -> std::string& {
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

auto str::fill(std::string& s, value_type ch, size_type pos, size_type max_n) -> std::string& {
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

auto str::left_n(std::string& s, size_type n) -> std::string& {
    if (n == 0) {
        s.clear();
        return s;
    }

    if (n > s.size()) {
        return s;
    }

    s.resize(n);

    return s;
}

auto str::right_n(std::string& s, size_type n) -> std::string& {
    if (n == 0) {
        s.clear();
        return s;
    }

    if (n > s.size()) {
        return s;
    }

    size_type len = (s.size() - n);
    std::memmove(s.data(), s.c_str() + len, n);
    s.resize(n);

    return s;
}

auto str::mid_n(std::string& s, size_type pos, size_type n) -> std::string& {
    if (s.empty()) {
        return s;
    }

    if (pos >= s.size()) {
        s.clear();
        return s;
    }

    size_type move_size = ((pos + n) < s.size()) ? n : (s.size() - pos);
    std::memmove(s.data(), s.data() + pos, move_size);
    s.resize(move_size);
    return s;
}

auto str::substr(std::string& s, size_type pos, ssize_type offset) -> std::string& {
    if (offset > 0) {
        return str::mid_n(s, pos, offset);
    }

    if (offset < 0) {
        return str::mid_n(s, pos - offset, offset);
    }

    s.clear();
    return s;
}

auto str::align_left(std::string& s, size_type width, value_type ch) -> std::string& {
    if (s.size() >= width) {
        return s;
    }

    s.resize(width, ch);
    return s;
}

auto str::align_right(std::string& s, size_type width, value_type ch) -> std::string& {
    if (s.size() >= width) {
        return s;
    }

    size_type old_len = s.size();
    s.resize(width);
    std::memmove(s.data() + width - old_len, s.data(), old_len);
    std::fill(s.data(), s.data() + width - old_len, ch);
    return s;
}

auto str::align_center(std::string& s, size_type width, value_type ch) -> std::string& {
    if (s.size() >= width) {
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

auto str::zfill(std::string& s, size_type width) -> std::string& {
    if (s.empty()) {
        s.resize(0);
        return s;
    }

    if (s.size() >= width) {
        return s;
    }

    if ((s[0] != '+') && (s[0] != '-')) {
        return align_left(s, width, '0');
    }

    size_type old_len = s.size();
    s.resize(width);
    if (old_len - 1 > 0) {
        std::memmove(s.data() + s.size() - (old_len - 1), s.c_str() + 1, old_len - 1);
    }

    std::fill(s.data() + 1, s.data() + s.size() - (old_len - 1), '0');
    return s;
}

// auto str::capitalize(std::string& s) -> std::string& {
// }

auto str::title(std::string& s) -> std::string& {
    if (s.empty()) {
        return s;
    }

    s[0] = static_cast<value_type>(std::toupper(s[0]));
    return s;
}

// auto str::title_words(std::string& s) -> std::string& {
// }

auto str::repeat(std::string& s, size_type times) -> std::string& {
    assert(times != npos);
    if (s.empty() || (times == 0)) {
        s.clear();
        return s;
    }

    std::string result;
    result.reserve(times * s.size());

    for (size_type i = 0; i < times; i++) {
        result.append(s);
    }

    return result;
}

auto str::invert(std::string& s, size_type pos, size_type max_n) -> std::string& {
    if (s.empty() || (pos >= s.size()) || (max_n == 0)) {
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

auto str::to_lower(std::string& s) -> std::string& {
}

auto str::to_upper(std::string& s) -> std::string& {
}

auto str::swap_case(std::string& s) -> std::string& {
}

auto str::case_fold(std::string& s) -> std::string& {
}

auto str::translate(std::string& s, const char_mapping_proc& proc) -> std::string& {
    pointer ptr = s.data();
    while (*ptr) {
        *ptr = proc(*ptr);
        ptr++;
    }
    return s;
}

// auto str::translate(std::string& s, std::string_view from, std::string_view to) -> std::string& {
// }

// auto str::simplified(std::string& s, const char_checker_proc& proc) -> std::string& {
// }

auto str::simplified(std::string& s) -> std::string& {
    if (s.empty()) {
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

auto str::trim_left(std::string& s, char_checker_proc proc) -> std::string& {
    if (s.empty()) [[unlikely]] {
        return s;
    }

    auto left_ptr = s.c_str();
    while (left_ptr < (s.c_str() + s.size())) {
        if (!proc(*left_ptr)) [[unlikely]] {
            break;
        }

        left_ptr++;
    }

    size_type remain_size = s.size() - (left_ptr - s.c_str());
    std::memmove(s.data(), left_ptr, remain_size);
    s.resize(remain_size);

    return s;
}

auto str::trim_left(std::string& s) -> std::string& {
    return str::trim_left(s, [](value_type ch) -> bool {
        return std::isspace(ch);
    });
}

auto str::trim_right(std::string& s, char_checker_proc proc) -> std::string& {
    if (s.empty()) [[unlikely]] {
        return s;
    }

    auto right_ptr = s.c_str() + s.size();
    while (right_ptr > s.c_str()) {
        if (!proc(*(right_ptr - 1))) {
            break;
        }
        right_ptr--;
    }

    s.resize(right_ptr - s.c_str());
    return s;
}

auto str::trim_right(std::string& s) -> std::string& {
    return str::trim_right(s, [](value_type ch) -> bool {
        return std::isspace(ch);
    });
}

auto str::trim_surrounding(std::string& s, char_checker_proc proc) -> std::string& {
    if (s.empty()) {
        return s;
    }

    auto left_ptr = s.c_str();
    while (left_ptr < (s.c_str() + s.size())) {
        if (!proc(*left_ptr)) [[unlikely]] {
            break;
        }

        left_ptr++;
    }

    auto right_ptr = s.c_str() + s.size();
    while (right_ptr > left_ptr) {
        if (!proc(*(right_ptr - 1))) {
            break;
        }
        right_ptr--;
    }

    size_type len = right_ptr - left_ptr;
    std::memmove(s.data(), left_ptr, len);
    s.resize(len);

    return s;
}

auto str::trim_surrounding(std::string& s) -> std::string& {
    return str::trim_surrounding(s, [](value_type ch) -> bool {
        return std::isspace(ch);
    });
}

// auto str::trim_anywhere(std::string s, char_checker_proc proc) -> std::string {
// }

// auto str::trim_anywhere(std::string& s) -> std::string& {
// }

// auto str::drop_left(std::string& s, size_type n) -> std::string& {
// }

// auto str::drop_right(std::string& s, size_type n) -> std::string& {
// }

// auto str::expand_envs(std::string& s, expand_vars_proc proc) -> std::string& {
// }

// auto str::expand_envs(std::string& s, const std::map<std::string, std::string>& kvs) -> std::string& {
// }

// auto str::expand_envs(std::string& s, const std::tuple<const std::string&, const std::string&>& pair) -> std::string& {
// }

// auto str::expand_envs(std::string& s, const std::tuple<const std::string_view, const std::string_view>& pair) -> std::string& {
// }

// auto str::expand_envs(std::string& s) -> std::string& {
// }

// auto str::expand_tabs(std::string& s, size_type tab_size = 8) -> std::string& {
// }

// auto str::expand_user(std::string& s) -> std::string& {
// }

// auto str::normpath(std::string& s) -> std::string& {
// }

//  处理路径中文件名的部分
static auto basename_ptr(std::string_view s) -> std::string::const_pointer {
    assert(!s.empty());

    std::string::const_pointer ptr = s.data() + s.size();
    while (ptr > s.data()) {
#ifdef WIN32
        if ((*(ptr - 1) == '/') || (*(ptr - 1) == '\\')) {
            break;
        }
#else
        if (*(ptr - 1) == '/') {
            break;
        }
#endif
    }

    return ptr;
}

// 扩展名相关操作
static auto extname_ptr(std::string_view s) -> std::string::const_pointer {
    assert(!s.empty());

    std::string::const_pointer base_ptr = basename_ptr(s);
    std::string::const_pointer end = s.data() + s.size();

    if (base_ptr[0] == '.') {
        while (base_ptr < end) {
            if (*base_ptr != '.') {
                break;
            }
            base_ptr++;
        }
    }

    std::string::const_pointer ptr = s.data() + s.size();

    return ptr;
}

// auto str::dirname(std::string& s) -> std::string& {
// }

// auto str::remove_dirname(std::string& s) -> std::string& {
// }

// auto str::replace_dirname(std::string& s, std::string_view newname) -> std::string& {
// }

auto str::basename(std::string& s) -> std::string& {
    auto ptr = basename_ptr(s);
    auto len = s.size() - (ptr - s.c_str());
    std::memmove(s.data(), ptr, len);
    s.resize(len);
    return s;
}

auto str::remove_basename(std::string& s) -> std::string& {
    auto ptr = basename_ptr(s);
    s.resize(ptr - s.c_str());
    return s;
}

auto str::replace_basename(std::string& s, std::string_view name) -> std::string& {
    const_pointer ptr = basename_ptr(s);
    size_type dir_len = (ptr - s.c_str());
    s.reserve(dir_len + name.size());
    s.resize(dir_len);
    s.append(name);
    return s;
}

// auto str::extname(std::string& s) -> std::string& {
// }

auto str::remove_extname(std::string& s) -> std::string& {
    auto ptr = extname_ptr(s);
    s.resize(ptr - s.c_str());
    return s;
}

auto str::replace_extname(std::string& s, std::string_view name) -> std::string& {
    pointer ptr = const_cast<pointer>(extname_ptr(s));
    s.resize((ptr - s.c_str()) + name.size());
    std::memcpy(ptr, name.data(), name.size());
    return s;
}

// auto str::encode_cstr(std::string& s) -> std::string& {
// }

// auto str::decode_cstr(std::string& s) -> std::string& {
// }

// auto str::encode_xml(std::string& s) -> std::string& {
// }

// auto str::decode_xml(std::string& s) -> std::string& {
// }

// auto str::encode_hex(std::string& s) -> std::string& {
// }

// auto str::decode_hex(std::string& s) -> std::string& {
// }

// auto str::encode_base64(std::string& s) -> std::string& {
// }

// auto str::decode_base64(std::string& s) -> std::string& {
// }

// auto str::encode_url(std::string& s) -> std::string& {
// }

// auto str::decode_url(std::string& s) -> std::string& {
// }

auto str::read_line(FILE* file) -> std::string {
    assert(file != nullptr);

    std::string result;

    char buffer[512];
    while (!feof(file) || !ferror(file)) {
        char* ptr = fgets(buffer, sizeof(buffer), file);
        if (ptr == nullptr) {
            return result;
        }

        // [\n][\0]$
        size_type len = strlen(ptr);
        if (len < (sizeof(buffer) - 1)) {
            result.append(buffer, len);
            return result;
        }

        result.append(buffer, len);
    }
}

auto str::read_line(std::istream& file) -> std::string {
    std::string line_text;
    std::getline(file, line_text);
    return line_text;
}

auto str::read_lines(FILE* file, std::function<int(size_type line_index, std::string_view line_text)> proc) -> void {
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

        // [\n][\0]$
        if (len >= (sizeof(buffer) - 1)) {
            line_text.append(buffer, len);
            continue;
        }

        line_text.append(buffer, len);
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
    read_lines(file, [max_n, &result](size_type line_index, std::string_view line_text) -> int {
        result.emplace_back(line_text);
        if (result.size() == max_n) {
            return 1;
        }

        return 0;
    });
}

auto str::read_lines(std::istream& file, std::function<int(size_type line_index, std::string_view line_text)> proc) -> void {
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
    read_lines(file, [max_n, &result](size_type line_index, std::string_view line_text) -> int {
        result.emplace_back(line_text);
        if (result.size() == max_n) {
            return 1;
        }

        return 0;
    });
}

auto str::read_lines(const std::string& filename, std::function<int(size_type line_index, std::string_view line_text)> proc) -> void {
    std::ifstream file{filename};
    str::read_lines(file, proc);
}

auto str::read_lines(const std::string& filename, size_type max_n) -> std::vector<std::string> {
    std::ifstream file{filename};
    return str::read_lines(file, max_n);
}
