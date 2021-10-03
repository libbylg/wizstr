//
// Created by luolijun on 2021/9/30.
//
#include "tiny/str.h"
#include "tiny/asserts.h"

#include <cstdlib>
#include <cstring>

namespace tiny {

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

str::str(str::char_type ch, str::size_type n) {
    layout.init(nullptr, n);
    std::fill(layout.begin(), layout.end(), ch);
}

str::str() {
    layout.init(nullptr, 0);
}

str& str::operator=(str&& other) noexcept {
    layout.clear();
    memcpy(&layout, &other.layout, sizeof(other.layout));
    other.layout.init(nullptr, 0);
}

str& str::operator=(const str& other) {
    layout.destroy();
    layout.init(other.data(), other.size());
}

str::~str() {
    layout.destroy();
}

//  迭代器
str::iterator str::begin() {
    layout.begin();
}
str::iterator str::end() {
    layout.end();
}

//  常量迭代器
str::const_iterator str::begin() const {
    layout.begin();
}
str::const_iterator str::end() const {
    layout.end();
}

//  反向迭代器
str::reverse_iterator str::rbegin() {
    return str::reverse_iterator(end() - 1);
}
str::reverse_iterator str::rend() {
    return str::reverse_iterator(begin() - 1);
}

//  反向常量迭代器
str::const_reverse_iterator str::rbegin() const {
    return str::reverse_iterator(end() - 1);
}
str::const_reverse_iterator str::rend() const {
    return str::reverse_iterator(begin() - 1);
}

//  元素访问
str::char_type str::at(str::pos_type pos) const {
    ASSERT(pos >= 0);
    ASSERT(pos < size());
    return begin()[pos];
}

str::char_type str::back() const {
    return *(end() - 1);
}

str::char_type& str::back() {
    return *(end() - 1);
}

str::char_type str::front() const {
    return begin()[0];
}

str::char_type& str::front() {
    return begin()[0];
}

//  长度容量
str::size_type str::size() const {
    return layout.len();
}

str::size_type str::capacity() const {
    return layout.cap();
}

//  清空
void str::clear() {
    layout.destroy();
    layout.init(nullptr, 0);
}

//  对接 C 接口
const str::char_type* str::data() const {
    return layout.begin();
}
str::char_type* str::data() {
    return layout.begin();
}

void str::attach(size_type cap, size_type len, str::char_type* buf) {
    ASSERT(false);
}

void str::detach(size_type& cap, size_type& len, str::char_type*& buf) {
    ASSERT(false);
}

//  首尾追加
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

str& str::append(const str::char_type* s, int n) {
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

str& str::prepend(const str& str) {
}

str& str::prepend(str::char_type ch) {
}

str& str::prepend(const str::char_type* str, int len) {
}

str& str::prepend(const str::char_type* str) {
}

//  修改字符串：中间插入、首尾插入、任意位置删除
str& str::insert(int position, const str& str) {
}
str& str::insert(int position, str::char_type ch) {
}
str& str::insert(int position, const str::char_type* s, int size) {
}
str& str::insert(int position, const str::char_type* s) {
}

void str::push_back(const str& other) {
    append(other.data(), other.size());
}
void str::push_back(str::char_type ch) {
    append(ch);
}

void str::pop_back() {
}

str& str::remove(int position, int n) {
}
str& str::remove(str::char_type ch, bool ignore_case) {
}
str& str::remove(const str::char_type* str, bool ignore_case) {
}
str& str::remove(const str& str, bool ignore_case) {
}
str& str::remove(const std::regex& rx) {
}
str& str::remove(std::function<int(int c, bool& match)> func) {
}

str& str::erase(pos_type pos, pos_type len) {
}
str::iterator str::erase(const_iterator p) {
}
str::iterator str::erase(const_iterator first, const_iterator last) {
}

//  比较
int str::compare(const str& other, bool ignore_case) const {
}
int str::compare(const str::char_type* s, bool ignore_case) const {
}
int str::compare(str::char_type c, bool ignore_case) const {
}

//  是否包含子串
bool str::contains(const str& s, bool ignore_case) const {
}
bool str::contains(const str::char_type* s, bool ignore_case) const {
}
bool str::contains(str::char_type ch, bool ignore_case) const {
}
bool str::contains(const std::regex& rx) const {
}
bool str::contains(std::regex& re) const {
}

//  子串统计
int str::count(const str& s, bool ignore_case) const {
}
int str::count(const str::char_type* s, bool ignore_case) const {
}
int str::count(str::char_type ch, bool ignore_case) const {
}
int str::count(const std::regex& rx) const {
}

//  前后缀操作
bool str::has_suffix(const str& s, bool ignore_case) const {
}
bool str::has_suffix(const str::char_type* str, bool ignore_case) const {
}
bool str::has_suffix(str::char_type c, bool ignore_case) const {
}

bool str::has_prefix(const str& s, bool ignore_case) const {
}
bool str::has_prefix(const str::char_type* s, bool ignore_case) const {
}
bool str::has_prefix(str::char_type c, bool ignore_case) const {
}

bool str::remove_prefix(const str& s, bool ignore_case) {
}
bool str::remove_prefix(const str::char_type* s, bool ignore_case) {
}
bool str::remove_prefix(str::char_type c, bool ignore_case) {
}

bool str::remove_suffix(const str& s, bool ignore_case) {
}
bool str::remove_suffix(const str::char_type* s, bool ignore_case) {
}
bool str::remove_suffix(str::char_type c, bool ignore_case) {
}

//  填充
str& str::fill(str::char_type ch, str::size_type n) {
    return *this;
}

//  查找
int str::index_of(const str& str, str::pos_type from, bool ignore_case) const {
}
int str::index_of(const str::char_type* s, str::pos_type from, bool ignore_case) const {
}
int str::index_of(str::char_type ch, str::pos_type from, bool ignore_case) const {
}
int str::index_of(const std::regex& rx, str::pos_type from) const {
}
int str::index_of(std::regex& rx, str::pos_type from) const {
}
int str::index_of(std::function<int(int c, bool& match)> func, str::pos_type from) const {
}

int str::last_index_of(const str& str, str::pos_type from, bool ignore_case) const {
}
int str::last_index_of(str::char_type ch, str::pos_type from, bool ignore_case) const {
}
int str::last_index_of(const str::char_type* str, str::pos_type from, bool ignore_case) const {
}
int str::last_index_of(const std::regex& rx, str::pos_type from) const {
}
int str::last_index_of(std::regex& rx, str::pos_type from) const {
}
int str::last_index_of(std::function<int(int c, bool& match)> func, str::pos_type from) const {
}

//  匹配
bool str::is_match(std::regex& rx) const {
}
bool str::is_match(const str& pattern) const {
}
bool str::is_match(const str::char_type*& pattern) const {
}
bool str::is_match_wild(const str& pattern, bool ignore_case) const {
}
bool str::is_match_wild(const str::char_type*& pattern, bool ignore_case) const {
}

//  字符串特征
bool str::is_empty() const {
}
bool str::is_lower() const {
}
bool str::is_upper() const {
}
bool str::is_title() const {
}
bool str::is_digit() const {
}
bool str::is_ascii() const {
}
bool str::is_alpha() const {
}
bool str::is_alnum() const {
}
bool str::is_space() const {
}
bool str::is_printable() const {
}
bool str::is_identifier() const {
}
bool str::is_numeric() const {
}

//  提取子串
str str::left(int n) const {
}
str str::right(int n) const {
}
str str::mid(int position, int n) const {
}

//  定宽对齐调整
str& str::ljust(int width, str::char_type fill, bool truncate) {
}
str& str::rjust(int width, str::char_type fill, bool truncate) {
}
str& str::center(int width, str::char_type fill, bool truncate) {
}
str& str::zfill(int width, str::char_type fill, bool truncate) {
}

//  子串替换
str& str::replace(int position, int n, const str& after) {
}
str& str::replace(int position, int n, str::char_type after) {
}
str& str::replace(int position, int n, const str::char_type* unicode, int size) {
}
str& str::replace(str::char_type before, str::char_type after, bool ignore_case) {
}
str& str::replace(const str::char_type* before, int blen, const str::char_type* after, int alen, bool ignore_case) {
}
str& str::replace(const str& before, const str& after, bool ignore_case) {
}
str& str::replace(str::char_type ch, const str& after, bool ignore_case) {
}
str& str::replace(const std::regex& rx, const str& after) {
}
str& str::replace(std::function<int(str::char_type key, str::char_type& val)> func) {
}

//  基于本字符串生成新字符串
str str::repeated(int times) const {
} //  返回本字符串重复 times 次后的副本
str str::join(const std::vector<str>& s) const {
} //  用本字符串连接所有的s
str str::join(const str& s, ...) const {
} //  用本字符串连接所有的s
str str::join(const str::char_type* s, ...) const {
} //  用本字符串连接所有的s

//  容量、内存管理
void str::reserve(int size) {
} //  容量预留
void str::resize(int size) {
} //  重新调整字符串长度，多出的部分未初始化
void str::resize(int size, str::char_type fill_char) {
} //  重新调整字符串长度，多出的部分用指定字符串填充
void str::shrink_to_fit() {
} //  内存调整到和字符串实际有效长度一致
void str::squeeze() {
} //  内存紧缩

//  Title 化：首字母大写
str& str::title() {
}

//  反转：字符串逆序
str& str::inversion() {
}

//  字符串分割
std::vector<str> str::split(const str& sep, bool ignore_case) const {
}
std::vector<str> str::split(const str::char_type* sep, bool ignore_case) const {
}
std::vector<str> str::split(str::char_type sep, bool ignore_case) const {
}
std::vector<str> str::split(const std::regex& rx) const {
}
std::vector<str> str::split(const std::regex& rx, std::function<int(const str& line)> output_func) const {
}
std::vector<str> str::split(std::function<int(int c, bool& match)>& chars_func) const {
}
std::vector<str> str::split_lines(bool keep_ends) const {
}
std::vector<str> str::split_path() const {
}
void str::split(const str& sep, bool case_sensitive, std::function<int(const str& line)> output_func) const {
}
void str::split(const str::char_type* sep, bool case_sensitive, std::function<int(const str& line)> output_func) const {
}
void str::split(str::char_type sep, bool case_sensitive, std::function<int(const str& line)> output_func) const {
}
void str::split(std::function<int(int c, bool& match)>& chars_func, std::function<int(const str& line)> output_func) const {
}
void str::split_lines(bool keep_ends, std::function<int(const str& line)> output_func) const {
}
void str::split_path(std::function<int(const str& line)> output_func) const {
}

//  大小写转换
str& str::to_lower() {
} //  转换成大写
str& str::to_upper() {
} //  转换成小写
str& str::swap_case() {
} //  大小写互换

//  空白处理
str& str::simplified() {
} //   空白化简，多个空白用一个空白代替
str& str::ltrim() {
} //  去掉左边的空白
str& str::rtrim() {
} //  去掉右边的空白
str& str::trim() {
} //  同时去掉左边和右边的空白

//  展开
str str::expand(const std::map<str, str>& kvs) {
}
str str::expand(std::function<int(const str& key, str& val)> provider) {
}
str str::expand_envs(const str::char_type* key, const str::char_type* val) {
}
str str::expand_envs() {
}
str str::expand_tabs(int tab_size) {
}
str str::expand_tmpl(const std::map<str, str>& kvs) {
}
str str::expand_tmpl(std::function<int(const str& key, str& val)> provider) {
}

//  字符串交换
void str::swap(str& other) {
}

//  路径处理
str str::basename() const {
}
str& str::basename() {
}
str str::dirname() const {
}
str& str::dirname() {
}

//  bool 映射
bool str::to_bool(bool* ok) const {
}
str& str::assign(bool v) const {
}

//  字符串转换为数字
double str::to_double(bool* ok) const {
}
float str::to_float(bool* ok) const {
}
int8_t str::to_int8(bool* ok, int base) const {
}
int16_t str::to_int16(bool* ok, int base) const {
}
int32_t str::to_int32(bool* ok, int base) const {
}
int64_t str::to_int64(bool* ok, int base) const {
}
uint8_t str::to_uint8(bool* ok, int base) const {
}
uint16_t str::to_uint16(bool* ok, int base) const {
}
uint32_t str::to_uint32(bool* ok, int base) const {
}
uint64_t str::to_uint64(bool* ok, int base) const {
}

str& str::assign(double n, str::char_type format, int precision) {
}
str& str::assign(float n, str::char_type format, int precision) {
}
str& str::assign(int8_t n, int base) {
}
str& str::assign(int16_t n, int base) {
}
str& str::assign(int32_t n, int base) {
}
str& str::assign(int64_t n, int base) {
}
str& str::assign(uint8_t n, int base) {
}
str& str::assign(uint16_t n, int base) {
}
str& str::assign(uint32_t n, int base) {
}
str& str::assign(uint64_t n, int base) {
}

//  转换为 hash 值
int32_t str::hash_code() const {
}

//  数字转换为字符串
str str::number(double n, str::char_type format, int precision) {
}
str str::number(float n, str::char_type format, int precision) {
}
str str::number(int8_t n, int base) {
}
str str::number(int16_t n, int base) {
}
str str::number(int32_t n, int base) {
}
str str::number(int64_t n, int base) {
}
str str::number(uint8_t n, int base) {
}
str str::number(uint16_t n, int base) {
}
str str::number(uint32_t n, int base) {
}
str str::number(uint64_t n, int base) {
}

//  运算符重载
bool str::operator!=(const str::char_type* other) const {
}
str& str::operator+=(str::char_type ch) {
}
str& str::operator+=(const str& other) {
}
str& str::operator+=(const str::char_type* str) {
}
bool str::operator<(const str::char_type* other) const {
}
bool str::operator<=(const str::char_type* other) const {
}
str& str::operator=(str::char_type ch) {
}
str& str::operator=(const str::char_type* str) {
}
bool str::operator==(const str::char_type* other) const {
}
bool str::operator>(const str::char_type* other) const {
}
bool str::operator>=(const str::char_type* other) const {
}
str::reference str::operator[](int position) {
}
str::const_reference str::operator[](int position) const {
}

}