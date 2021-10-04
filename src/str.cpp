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

str::str(const str::char_type* s, str::size_type n) {
    layout.init(s, n);
}

str::str(str::char_type ch, str::size_type n) {
    layout.init(nullptr, n);
    std::fill(layout.begin(), layout.end(), ch);
}

str::str(const str::char_type* s) {
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

str::char_type str::at(str::pos_type pos) const {
    ASSERT(pos >= 0);
    ASSERT(pos < size());
    return begin()[pos];
}

str::char_type str::back() const {
    ASSERT(size() > 0);
    return *(end() - 1);
}

str::char_type& str::back() {
    ASSERT(size() > 0);
    return *(end() - 1);
}

str::char_type str::front() const {
    ASSERT(size() > 0);
    return begin()[0];
}

str::char_type& str::front() {
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

const str::char_type* str::data() const {
    return layout.begin();
}

str::char_type* str::data() {
    return layout.begin();
}

void str::attach(str::char_type* buf, str::size_type len, str::size_type cap) {
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

str& str::append(str::char_type ch) {
    layout.resize(layout.len() + 1);
    layout.fill(size(), &ch, 1);
    return *this;
}

str& str::append(const str::char_type* s, str::size_type n) {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    layout.resize(layout.len() + 1);
    layout.fill(size(), s, n);
    return *this;
}

str& str::append(const str::char_type* s) {
    ASSERT(s != nullptr);
    append(s, std::strlen(s));
    return *this;
}

str& str::prepend(const str& s) {
    ASSERT(this != &s);
    return prepend(s.begin(), s.size());
}

str& str::prepend(str::char_type ch) {
    return prepend(&ch, 1);
}

str& str::prepend(const str::char_type* s, str::size_type n) {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    layout.flexmove(0, layout.len(), n);
    layout.fill(0, s, n);
    return *this;
}

str& str::prepend(const str::char_type* s) {
    ASSERT(s != nullptr);
    return prepend(s, std::strlen(s));
}

str& str::insert(str::pos_type pos, const str& str) {
    ASSERT(pos >= 0);
    return insert(pos, str.layout.begin(), layout.len());
}

str& str::insert(str::pos_type pos, str::char_type ch) {
    return insert(pos, &ch, 1);
}

str& str::insert(str::pos_type pos, const str::char_type* s, str::size_type n) {
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

str& str::insert(str::pos_type pos, const str::char_type* s) {
    return insert(pos, s, std::strlen(s));
}

void str::push_back(const str& s) {
    append(s.data(), s.size());
}

void str::push_back(str::char_type ch) {
    append(ch);
}

void str::push_front(const str& s) {
    prepend(s);
}

void str::push_front(str::char_type ch) {
    prepend(ch);
}

str::char_type str::pop_back() {
    ASSERT(size() > 0);
    str::char_type ch = *(end() - 1);
    resize(size() - 1);
    return ch;
}

str::char_type str::pop_front() {
    ASSERT(size() > 0);
    str::char_type ch = *(begin());
    layout.clipmove(0, layout.len(), -1);
    layout.resize(layout.len() - 1);
    return ch;
}

str& str::flex_move(str::pos_type pos, str::size_type n, int offset) {
    layout.flexmove(pos, n, offset);
    return *this;
}

str& str::flex_move(str::pos_type pos, str::size_type n, int offset, str::char_type fill_ch) {
    layout.flexmove(pos, n, offset, fill_ch);
    return *this;
}

str& str::clip_move(str::pos_type pos, str::size_type n, int offset) {
    layout.clipmove(pos, n, offset);
    return *this;
}

str& str::clip_move(str::pos_type pos, str::size_type n, int offset, str::char_type fill_ch) {
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

str& str::remove(str::char_type ch, bool ignore_case) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::remove(const str::char_type* str, bool ignore_case) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::remove(const str& str, bool ignore_case) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::remove(const std::regex& rx) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::remove(std::function<int(str::char_type c, bool& match)> func) {
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

int str::compare(const str& other, bool ignore_case) const {
    ASSERT(false); //  TODO
    return -1;
}

int str::compare(const str::char_type* s, bool ignore_case) const {
    ASSERT(false); //  TODO
    return -1;
}

int str::compare(str::char_type c, bool ignore_case) const {
    ASSERT(false); //  TODO
    return -1;
}

bool str::contains(const str& s, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::contains(const str::char_type* s, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::contains(str::char_type ch, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::contains(const std::regex& rx) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::contains(std::regex& re) const {
    ASSERT(false); //  TODO
    return false;
}

int str::count(const str& s, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

int str::count(const str::char_type* s, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

int str::count(str::char_type ch, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

int str::count(const std::regex& rx) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::has_suffix(const str& s, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::has_suffix(const str::char_type* str, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::has_suffix(str::char_type c, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::has_prefix(const str& s, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}
bool str::has_prefix(const str::char_type* s, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}
bool str::has_prefix(str::char_type c, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::remove_prefix(const str& s, bool ignore_case) {
    ASSERT(false); //  TODO
    return false;
}
bool str::remove_prefix(const str::char_type* s, bool ignore_case) {
    ASSERT(false); //  TODO
    return false;
}
bool str::remove_prefix(str::char_type c, bool ignore_case) {
    ASSERT(false); //  TODO
    return false;
}

bool str::remove_suffix(const str& s, bool ignore_case) {
    ASSERT(false); //  TODO
    return false;
}
bool str::remove_suffix(const str::char_type* s, bool ignore_case) {
    ASSERT(false); //  TODO
    return false;
}
bool str::remove_suffix(str::char_type c, bool ignore_case) {
    ASSERT(false); //  TODO
    return false;
}

str& str::fill(str::char_type ch) {
    layout.fill(0, ch, layout.len());
    return *this;
}

str& str::fill(str::pos_type pos, str::char_type ch, str::size_type n) {
    layout.fill(pos, ch, layout.len());
    return *this;
}

str::pos_type str::index_of(const str& str, str::pos_type from, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::index_of(const str::char_type* s, str::pos_type from, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::index_of(str::char_type ch, str::pos_type from, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::index_of(const std::regex& rx, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::index_of(std::regex& rx, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

str::pos_type str::index_of(std::function<int(str::char_type c, bool& match)> func, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}


str::pos_type str::last_index_of(const str& str, str::pos_type from, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}


str::pos_type str::last_index_of(str::char_type ch, str::pos_type from, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}


str::pos_type str::last_index_of(const str::char_type* str, str::pos_type from, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}


str::pos_type str::last_index_of(const std::regex& rx, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}


str::pos_type str::last_index_of(std::regex& rx, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}


str::pos_type str::last_index_of(std::function<int(str::char_type c, bool& match)> func, str::pos_type from) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_match(std::regex& rx) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_match(const str& pattern) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_match(const str::char_type*& pattern) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_match_wild(const str& pattern, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_match_wild(const str::char_type*& pattern, bool ignore_case) const {
    ASSERT(false); //  TODO
    return false;
}

bool str::is_empty() const {
    return layout.len() == 0;
}

bool str::is_lower() const {
    for (const char_type* p = layout.begin(); p != layout.end(); p++) {
        if (std::isupper(*p)) {
            return false;
        }
    }

    return true;
}

bool str::is_upper() const {
    for (const char_type* p = layout.begin(); p != layout.end(); p++) {
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

str str::left(str::size_type n) const {
    ASSERT(false); //  TODO
    return *this;
}
str str::right(str::size_type n) const {
    ASSERT(false); //  TODO
    return *this;
}
str str::mid(str::pos_type position, str::size_type n) const {
    ASSERT(false); //  TODO
    return *this;
}

//  定宽对齐调整
str& str::ljust(str::size_type width, str::char_type fill, bool truncate) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::rjust(str::size_type width, str::char_type fill, bool truncate) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::center(str::size_type width, str::char_type fill, bool truncate) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::zfill(str::size_type width, str::char_type fill, bool truncate) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::replace(str::pos_type position, str::size_type n, const str& after) {
    ASSERT(false); //  TODO
    return *this;
}
str& str::replace(str::pos_type position, str::size_type n, str::char_type after) {
    ASSERT(false); //  TODO
    return *this;
}
str& str::replace(str::pos_type position, str::size_type n, const str::char_type* unicode, str::size_type size) {
    ASSERT(false); //  TODO
    return *this;
}
str& str::replace(str::char_type before, str::char_type after, bool ignore_case) {
    ASSERT(false); //  TODO
    return *this;
}
str& str::replace(const str::char_type* before, str::size_type blen, const str::char_type* after, str::size_type alen, bool ignore_case) {
    ASSERT(false); //  TODO
    return *this;
}
str& str::replace(const str& before, const str& after, bool ignore_case) {
    ASSERT(false); //  TODO
    return *this;
}
str& str::replace(str::char_type ch, const str& after, bool ignore_case) {
    ASSERT(false); //  TODO
    return *this;
}
str& str::replace(const std::regex& rx, const str& after) {
    ASSERT(false); //  TODO
    return *this;
}
str& str::replace(std::function<int(str::char_type key, str::char_type& val)> func) {
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

str str::join(const str& s, ...) const {
    ASSERT(false); //  TODO
    return str();
}

str str::join(const str::char_type* s, ...) const {
    ASSERT(false); //  TODO
    return str();
}

void str::reserve(str::size_type cap) {
    layout.resize(cap);
}

void str::resize(str::size_type n) {
    layout.resize(n);
}

void str::resize(str::size_type n, str::char_type fill_ch) {
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

std::vector<str> str::split(const str& sep, bool ignore_case) const {
    std::vector<str> result;
    split(sep, ignore_case, [&result](const char_type* s, size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<str> str::split(const str::char_type* sep, bool ignore_case) const {
    std::vector<str> result;
    split(sep, ignore_case, [&result](const str::char_type* s, str::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<str> str::split(str::char_type sep, bool ignore_case) const {
    std::vector<str> result;
    split(sep, ignore_case, [&result](const str::char_type* s, str::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<str> str::split(const std::regex& r) const {
    std::vector<str> result;
    split(r, [&result](const str::char_type* s, str::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<str> str::split(std::function<int(str::char_type c, bool& match)>& chars_func) const {
    std::vector<str> result;
    split(chars_func, [&result](const str::char_type* s, str::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<str> str::split_lines(bool keep_ends) const {
    std::vector<str> result;
    split_lines(keep_ends, [&result](const str::char_type* s, str::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<str> str::split_path() const {
    std::vector<str> result;
    split_path([&result](const str::char_type* s, str::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

void str::split(const str& sep, bool case_sensitive, std::function<int(const str::char_type* s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO
}

void str::split(const str::char_type* sep, bool case_sensitive, std::function<int(const str::char_type* s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO
}

void str::split(str::char_type sep, bool case_sensitive, std::function<int(const str::char_type* s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO
}

void str::split(const std::regex& rx, std::function<int(const str::char_type* s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO
}

void str::split(std::function<int(str::char_type c, bool& match)>& chars_func, std::function<int(const str::char_type* s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO
}

void str::split_lines(bool keep_ends, std::function<int(const str::char_type* s, str::size_type n)> output_func) const {
    ASSERT(false); //  TODO
}

void str::split_path(std::function<int(const str::char_type* s, str::size_type n)> output_func) const {
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
    std::transform(begin(), end(), begin(), [](str::char_type& ch) -> str::char_type {
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

str str::expand_envs(const str::char_type* key, const str::char_type* val) const {
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

str& str::assign(double n, str::char_type format, int precision) {
    ASSERT(false); //  TODO
    return *this;
}

str& str::assign(float n, str::char_type format, int precision) {
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

str str::number(double n, str::char_type format, int precision) {
    ASSERT(false); //  TODO
    return str("");
}

str str::number(float n, str::char_type format, int precision) {
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

bool str::operator!=(const str::char_type* s) const {
    return 0 != compare(s);
}

str& str::operator+=(str::char_type ch) {
    return append(ch);
}

str& str::operator+=(const str& s) {
    return append(s);
}

str& str::operator+=(const str::char_type* s) {
    ASSERT(s != nullptr);
    return append(s);
}

bool str::operator<(const str::char_type* s) const {
    ASSERT(s != nullptr);
    return compare(s) < 0;
}

bool str::operator<=(const str::char_type* s) const {
    ASSERT(s != nullptr);
    return compare(s) <= 0;
}

str& str::operator=(str::char_type ch) {
    layout.destroy();
    layout.init(&ch, 1);
    return *this;
}

str& str::operator=(const str::char_type* s) {
    ASSERT(s != nullptr);
    layout.destroy();
    layout.init(s, std::strlen(s));
    return *this;
}

bool str::operator==(const str::char_type* s) const {
    ASSERT(s != nullptr);
    return compare(s) == 0;
}

bool str::operator>(const str::char_type* s) const {
    ASSERT(s != nullptr);
    return compare(s) > 0;
}

bool str::operator>=(const str::char_type* s) const {
    ASSERT(s != nullptr);
    return compare(s) >= 0;
}

str::char_type& str::operator[](str::pos_type pos) {
    return layout.begin()[pos];
}

const str::char_type& str::operator[](str::pos_type pos) const {
    return layout.begin()[pos];
}

extern bool operator!=(const str& s1, const str& s2) {
    return s2.compare(s1) != 0;
}

extern bool operator!=(const str::char_type* s1, const str& s2) {
    return s2.compare(s1) != 0;
}

extern const str operator+(const str& s1, const str& s2) {
    ASSERT(false); //  TODO
    str new_str;
    return new_str;
}

extern const str operator+(const str& s1, const str::char_type* s2) {
    ASSERT(false); //  TODO
    str new_str;
    return new_str;
}

extern const str operator+(const char* s1, const str& s2) {
    ASSERT(false); //  TODO
    str new_str;
    return new_str;
}

extern const str operator+(str::char_type ch, const str& s) {
    ASSERT(false); //  TODO
    str new_str;
    return new_str;
}

extern const str operator+(const str& s, str::char_type ch) {
    ASSERT(false); //  TODO
    str new_str;
    return new_str;
}

extern bool operator<(const str& s1, const str& s2) {
    return s2.compare(s1) > 0;
}

extern bool operator<(const str::char_type* s1, const str& s2) {
    return s2.compare(s1) > 0;
}

extern bool operator<=(const str& s1, const str& s2) {
    return s2.compare(s1) >= 0;
}

extern bool operator<=(const str::char_type* s1, const str& s2) {
    return s2.compare(s1) >= 0;
}

extern bool operator==(const str& s1, const str& s2) {
    return s2.compare(s1) == 0;
}

extern bool operator==(const str::char_type* s1, const str& s2) {
    return s2.compare(s1) == 0;
}

extern bool operator>(const str& s1, const str& s2) {
    return s2.compare(s1) < 0;
}

extern bool operator>(const str::char_type* s1, const str& s2) {
    return s2.compare(s1) < 0;
}

extern bool operator>=(const str& s1, const str& s2) {
    return s2.compare(s1) <= 0;
}

extern bool operator>=(const str::char_type* s1, const str& s2) {
    return s2.compare(s1) <= 0;
}

} // namespace tiny