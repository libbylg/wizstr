//
// Created by luolijun on 2021/9/30.
//
#include "tiny/str.h"
#include "tiny/asserts.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>

namespace tiny {

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
    layout.resize(layout.len() + s.size());
    layout.fill(size(), s.data(), s.size());
    return *this;
}

str& str::append(str::value_type ch) {
    layout.resize(layout.len() + 1);
    layout.fill(size(), &ch, 1);
    return *this;
}

str& str::append(str::const_pointer s, str::size_type n) {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    layout.resize(layout.len() + 1);
    layout.fill(size(), s, n);
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
    layout.flexmove(0, layout.len(), n);
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
        layout.flexmove(pos, layout.len() - pos, n);
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
    layout.flexmove(pos, n, offset);
    return *this;
}

str& str::flex_move(str::pos_type pos, str::size_type n, int offset, str::value_type fill_ch) {
    layout.flexmove(pos, n, offset, fill_ch);
    return *this;
}

str& str::clip_move(str::pos_type pos, str::size_type n, int offset) {
    layout.clipmove(pos, n, offset);
    return *this;
}

str& str::clip_move(str::pos_type pos, str::size_type n, int offset, str::value_type fill_ch) {
    layout.clipmove(pos, n, offset, fill_ch);
    return *this;
}

str& str::remove(str::pos_type pos) {
    ASSERT(pos >= 0);
    ASSERT(pos < layout.len());
    layout.resize(pos);
    return *this;
}

str& str::remove(str::pos_type pos, str::size_type n) {
    if ((pos + n) < layout.len()) {
        layout.flexmove((pos + n), layout.len() - (pos + n), n);
        layout.resize(layout.len() - n);
        return *this;
    }

    layout.resize(pos);
    return *this;
}

str& str::remove(str::value_type ch) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::remove(str::const_pointer str) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::remove(const str& str) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::remove(const re& rx) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::remove(std::function<int(str::value_type c, bool& match)> func) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::erase(pos_type pos, pos_type n) {
    return remove(pos, n);
}

str::iterator str::erase(const_iterator p) {
    ASSERT(false); //  TODO
    return begin();
}

str::iterator str::erase(const_iterator first, const_iterator last) {
    ASSERT(false); //  TODO
    return begin();
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
    ASSERT(false); //  TODO
    return false;
}

bool str::contains(re& r) const {
    ASSERT(false); //  TODO
    return false;
}

int str::count(const str& s) const {
    ASSERT(false); //  TODO
    return false;
}

int str::count(str::const_pointer s) const {
    ASSERT(false); //  TODO
    return false;
}

int str::count(str::value_type ch) const {
    ASSERT(false); //  TODO
    return false;
}

int str::count(const re& rx) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::has_suffix(const str& s) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::has_suffix(str::const_pointer str) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::has_suffix(str::value_type c) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::has_prefix(const str& s) const {
    ASSERT(false); //  TODO
    return false;
}
bool str::has_prefix(str::const_pointer s) const {
    ASSERT(false); //  TODO
    return false;
}
bool str::has_prefix(str::value_type c) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::remove_prefix(const str& s) {
    ASSERT(false); //  TODO
    return false;
}
bool str::remove_prefix(str::const_pointer s) {
    ASSERT(false); //  TODO
    return false;
}
bool str::remove_prefix(str::value_type c) {
    ASSERT(false); //  TODO
    return false;
}

bool str::remove_suffix(const str& s) {
    ASSERT(false); //  TODO
    return false;
}
bool str::remove_suffix(str::const_pointer s) {
    ASSERT(false); //  TODO
    return false;
}
bool str::remove_suffix(str::value_type c) {
    ASSERT(false); //  TODO
    return false;
}

str& str::fill(str::value_type ch) {
    layout.fill(0, ch, layout.len());
    return *this;
}

str& str::fill(str::pos_type pos, str::value_type ch, str::size_type n) {
    layout.fill(pos, ch, layout.len());
    return *this;
}

str::pos_type str::index_of(const str& other, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::index_of(str::const_pointer s, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::index_of(str::value_type ch, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::index_of(const re& rx, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::index_of(re& rx, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::index_of(std::function<int(str::value_type c, bool& match)> func, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::last_index_of(const str& other, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::last_index_of(str::value_type ch, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::last_index_of(str::const_pointer other, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::last_index_of(const re& rx, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::last_index_of(re& rx, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::last_index_of(std::function<int(str::value_type c, bool& match)> func, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_match(re& rx) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_match(const str& pattern) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_match(str::const_pointer& pattern) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_match_wild(const str& pattern) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_match_wild(str::const_pointer& pattern) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_empty() const {
    return layout.len() == 0;
}

bool str::is_lower() const {
    for (const_pointer p = layout.begin(); p != layout.end(); p++) {
        if (std::isupper(*p)) {
            return false;
        }
    }

    return true;
}

bool str::is_upper() const {
    for (const_pointer p = layout.begin(); p != layout.end(); p++) {
        if (std::islower(*p)) {
            return false;
        }
    }

    return true;
}

bool str::is_title() const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_digit() const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_ascii() const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_alpha() const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_alnum() const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_space() const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_printable() const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_identifier() const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_numeric() const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_bool() const {
    ASSERT(false); //  TODO
    return false;
}

str str::left(str::size_type n) const {
    ASSERT(false); //  TODO
    return *this;
}

str str::right(str::size_type n) const {
    ASSERT(false); //  TODO
    return *this;
}

str str::substr(pos_type pos, int offset_n) const {
    ASSERT(false); //  TODO
    return *this;
}

//  定宽对齐调整
str& str::ljust(str::size_type width, str::value_type fill, bool truncate) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::rjust(str::size_type width, str::value_type fill, bool truncate) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::center(str::size_type width, str::value_type fill, bool truncate) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::zfill(str::size_type width, str::value_type fill, bool truncate) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::replace(str::pos_type pos, str::size_type n, const str& after) {
    ASSERT(false); //  TODO
    return *this;
}
str& str::replace(str::pos_type pos, str::size_type n, str::value_type after) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::replace(str::pos_type pos, str::size_type n, str::const_pointer unicode, str::size_type size) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::replace(str::value_type before, str::value_type after) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::replace(str::const_pointer before, str::size_type blen, str::const_pointer after, str::size_type alen) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::replace(const str& before, const str& after) {
    ASSERT(false); //  TODO
    return *this;
}
str& str::replace(str::value_type ch, const str& after) {
    ASSERT(false); //  TODO
    return *this;
}
str& str::replace(const re& rx, const str& after) {
    ASSERT(false); //  TODO
    return *this;
}
str& str::replace(std::function<int(str::value_type key, str::value_type& val)> func) {
    ASSERT(false); //  TODO
    return *this;
}

str str::repeated(str::size_type times) const {
    ASSERT(false); //  TODO
    return str();
}

str str::join(const std::vector<str>& s) const {
    ASSERT(false); //  TODO
    return str();
}

str str::join(const str& other, ...) const {
    ASSERT(false); //  TODO
    return str();
}

str str::join(str::const_pointer s, ...) const {
    ASSERT(false); //  TODO
    return str();
}

void str::reserve(str::size_type cap) {
    layout.resize(cap);
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
    ASSERT(false); //  TODO
}

void str::split(str::const_pointer sep, std::function<int(str::const_pointer s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO
}

void str::split(str::value_type sep, std::function<int(str::const_pointer s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO
}

void str::split(const re& rx, std::function<int(str::const_pointer s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO
}

void str::split(std::function<int(str::value_type c, bool& match)>& chars_func, std::function<int(str::const_pointer s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO
}

void str::split_lines(bool keep_ends, std::function<int(str::const_pointer s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO
}

void str::split_path(std::function<int(str::const_pointer s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO
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
    ASSERT(false); // TODO
    return *this;
}

str& str::ltrim() {
    ASSERT(false); // TODO
    return *this;
}

str& str::rtrim() {
    ASSERT(false); // TODO
    return *this;
}

str& str::trim() {
    ASSERT(false); // TODO
    return *this;
}

str str::expand(const std::map<str, str>& kvs) const {
    ASSERT(false); // TODO
    return str("");
}

str str::expand(std::function<int(const str& key, str& val)> provider) const {
    ASSERT(false); // TODO
    return str("");
}

str str::expand_envs(str::const_pointer key, str::const_pointer val) const {
    ASSERT(false); // TODO
    return str("");
}

str str::expand_envs() const {
    ASSERT(false); // TODO
    return str("");
}

str str::expand_tabs(str::size_type tab_size) const {
    ASSERT(false); // TODO
    return str("");
}

str str::expand_tmpl(const std::map<str, str>& kvs) const {
    ASSERT(false); // TODO
    return str("");
}

str str::expand_tmpl(std::function<int(const str& key, str& val)> provider) const {
    ASSERT(false); // TODO
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
    ASSERT(false); // TODO
    return 0;
}

str str::basename() const {
    ASSERT(false); // TODO
    return str("");
}

str& str::basename() {
    ASSERT(false); // TODO
    return *this;
}
str str::dirname() const {
    ASSERT(false); // TODO
    return str("");
}

str& str::dirname() {
    ASSERT(false); // TODO
    return *this;
}

bool str::to_bool(bool* ok) const {
    ASSERT(false); // TODO
}

str& str::assign(bool v) {
    ASSERT(false); // TODO
    return *this;
}

double str::to_double(bool* ok) const {
    ASSERT(false); // TODO
    return 0.0;
}

float str::to_float(bool* ok) const {
    ASSERT(false); // TODO
    return 0.0;
}

int8_t str::to_int8(bool* ok, int base) const {
    ASSERT(false); // TODO
    return 0;
}

int16_t str::to_int16(bool* ok, int base) const {
    ASSERT(false); //  TODO
    return 0;
}

int32_t str::to_int32(bool* ok, int base) const {
    ASSERT(false); //  TODO
    return 0;
}

int64_t str::to_int64(bool* ok, int base) const {
    ASSERT(false); //  TODO
    return 0;
}

uint8_t str::to_uint8(bool* ok, int base) const {
    ASSERT(false); //  TODO
    return 0;
}

uint16_t str::to_uint16(bool* ok, int base) const {
    ASSERT(false); //  TODO
    return 0;
}

uint32_t str::to_uint32(bool* ok, int base) const {
    ASSERT(false); //  TODO
    return 0;
}

uint64_t str::to_uint64(bool* ok, int base) const {
    ASSERT(false); //  TODO
    return 0;
}

str& str::assign(double n, str::value_type format, int precision) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::assign(float n, str::value_type format, int precision) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::assign(int8_t n, int base) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::assign(int16_t n, int base) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::assign(int32_t n, int base) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::assign(int64_t n, int base) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::assign(uint8_t n, int base) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::assign(uint16_t n, int base) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::assign(uint32_t n, int base) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::assign(uint64_t n, int base) {
    ASSERT(false); //  TODO
    return *this;
}

int32_t str::hash_code() const {
    ASSERT(false); //  TODO
    return 0;
}

str str::number(double n, str::value_type format, int precision) {
    ASSERT(false); //  TODO
    return str("");
}

str str::number(float n, str::value_type format, int precision) {
    ASSERT(false); //  TODO
    return str("");
}

str str::number(int8_t n, int base) {
    ASSERT(false); //  TODO
    return str("");
}

str str::number(int16_t n, int base) {
    ASSERT(false); //  TODO
    return str("");
}

str str::number(int32_t n, int base) {
    ASSERT(false); //  TODO
    return str("");
}

str str::number(int64_t n, int base) {
    ASSERT(false); //  TODO
    return str("");
}

str str::number(uint8_t n, int base) {
    ASSERT(false); //  TODO
    return str("");
}

str str::number(uint16_t n, int base) {
    ASSERT(false); //  TODO
    return str("");
}

str str::number(uint32_t n, int base) {
    ASSERT(false); //  TODO
    return str("");
}

str str::number(uint64_t n, int base) {
    ASSERT(false); //  TODO
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
    ASSERT(false); //  TODO
    str new_str;
    return new_str;
}

extern const str operator+(const str& s1, str::const_pointer s2) {
    ASSERT(false); //  TODO
    str new_str;
    return new_str;
}

extern const str operator+(str::const_pointer s1, const str& s2) {
    ASSERT(false); //  TODO
    str new_str;
    return new_str;
}

extern const str operator+(str::value_type ch, const str& s) {
    ASSERT(false); //  TODO
    str new_str;
    return new_str;
}

extern const str operator+(const str& other, str::value_type ch) {
    ASSERT(false); //  TODO
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