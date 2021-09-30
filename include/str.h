//
// Created by luolijun on 2021/9/30.
//
#ifndef STR_H
#define STR_H

#include <map>
#include <regex>
#include <vector>

class str {
    typedef int pos_t;

    typedef str& reference;
    typedef const str& const_reference;

    typedef const char* const_iterator;
    typedef char* iterator;

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef const reverse_iterator const_reverse_iterator;

public:
    explicit str(const char* str);
    explicit str(str&& other);
    explicit str(const str& other);
    explicit str(int size, char ch);
    explicit str(char ch);
    explicit str();
    str& operator=(str&& other);
    str& operator=(const str& other);

    ~str();

    //  迭代器
    str::iterator begin();
    str::iterator end();

    //  常量迭代器
    str::const_iterator begin() const;
    str::const_iterator end() const;

    //  反向迭代器
    str::reverse_iterator rbegin();
    str::reverse_iterator rend();

    //  反向常量迭代器
    str::const_reverse_iterator rbegin() const;
    str::const_reverse_iterator rend() const;

    //  元素访问
    const char at(int position) const;
    char back() const;
    char& back();
    char front() const;
    char& front();

    //  长度容量
    int size() const;
    int capacity() const;

    //  清空
    void clear();

    //  比较
    int compare(const str& other, bool case_sensitive = true) const;
    int compare(const char* s, bool case_sensitive = true) const;
    int compare(char c, bool case_sensitive = true) const;

    //  对接 C 接口
    const char* data() const;
    char* data();

    //  是否包含子串
    bool contains(const str& s, bool case_sensitive = true) const;
    bool contains(const char* s, bool case_sensitive = true) const;
    bool contains(char ch, bool case_sensitive = true) const;
    bool contains(const std::regex& rx) const;
    bool contains(std::regex& re) const;

    //  子串统计
    int count(const str& s, bool case_sensitive = true) const;
    int count(const char* s, bool case_sensitive = true) const;
    int count(char ch, bool case_sensitive = true) const;
    int count(const std::regex& rx) const;

    //  首尾追加
    str& append(const str& str);
    str& append(char ch);
    str& append(const char* str, int len);
    str& append(const char* str);
    str& prepend(const str& str);
    str& prepend(char ch);
    str& prepend(const char* str, int len);
    str& prepend(const char* str);

    //  前后缀操作
    bool has_suffix(const str& s, bool case_sensitive = true) const;
    bool has_suffix(const char* str, bool case_sensitive = true) const;
    bool has_suffix(char c, bool case_sensitive = true) const;

    bool has_prefix(const str& s, bool case_sensitive = true) const;
    bool has_prefix(const char* s, bool case_sensitive = true) const;
    bool has_prefix(char c, bool case_sensitive = true) const;

    bool remove_prefix(const str& s, bool case_sensitive = true);
    bool remove_prefix(const char* s, bool case_sensitive = true);
    bool remove_prefix(char c, bool case_sensitive = true);

    bool remove_suffix(const str& s, bool case_sensitive = true);
    bool remove_suffix(const char* s, bool case_sensitive = true);
    bool remove_suffix(char c, bool case_sensitive = true);

    //  填充
    str& fill(char ch, int size = -1);

    //  查找
    int index_of(const str& str, int from = 0, bool case_sensitive = true) const;
    int index_of(const char* s, int from = 0, bool case_sensitive = true) const;
    int index_of(char ch, int from = 0, bool case_sensitive = true) const;
    int index_of(const std::regex& rx, int from = 0) const;
    int index_of(std::regex& rx, int from = 0) const;
    int index_of(std::function<int(int c, bool& match)> func, int from = 0) const;

    int last_index_of(const str& str, int from = -1, bool case_sensitive = true) const;
    int last_index_of(char ch, int from = -1, bool case_sensitive = true) const;
    int last_index_of(const char* str, int from = -1, bool case_sensitive = true) const;
    int last_index_of(const std::regex& rx, int from = -1) const;
    int last_index_of(std::regex& rx, int from = -1) const;
    int last_index_of(std::function<int(int c, bool& match)> func, int from = -1) const;

    //  匹配
    bool is_match(std::regex& rx) const;
    bool is_match(const str& pattern) const;
    bool is_match(const char*& pattern) const;
    bool is_match_wild(const str& pattern, bool case_sensitive = true) const;
    bool is_match_wild(const char*& pattern, bool case_sensitive = true) const;

    //  字符串特征
    bool is_empty() const;
    bool is_lower() const;
    bool is_upper() const;
    bool is_title() const;
    bool is_digit() const;
    bool is_ascii() const;
    bool is_alpha() const;
    bool is_alnum() const;
    bool is_space() const;
    bool is_printable() const;
    bool is_identifier() const;
    bool is_numeric() const;

    //  提取子串
    str left(int n) const;
    str right(int n) const;
    str mid(int position, int n = -1) const;

    //  定宽对齐调整
    str& ljust(int width, char fill = ' ', bool truncate = false);
    str& rjust(int width, char fill = ' ', bool truncate = false);
    str& center(int width, char fill = ' ', bool truncate = false);
    str& zfill(int width, char fill = ' ', bool truncate = false);

    //  修改字符串：中间插入、首尾插入、任意位置删除
    str& insert(int position, const str& str);
    str& insert(int position, char ch);
    str& insert(int position, const char* s, int size);
    str& insert(int position, const char* s);

    void push_back(const str& other);
    void push_back(char ch);
    void push_front(const str& other);
    void push_front(char ch);

    str& remove(int position, int n);
    str& remove(char ch, bool case_sensitive = true);
    str& remove(const char* str, bool case_sensitive = true);
    str& remove(const str& str, bool case_sensitive = true);
    str& remove(const std::regex& rx);
    str& remove(std::function<int(int c, bool& match)> func);

    //  子串替换
    str& replace(int position, int n, const str& after);
    str& replace(int position, int n, char after);
    str& replace(int position, int n, const char* unicode, int size);
    str& replace(char before, char after, bool case_sensitive = true);
    str& replace(const char* before, int blen, const char* after, int alen, bool case_sensitive = true);
    str& replace(const str& before, const str& after, bool case_sensitive = true);
    str& replace(char ch, const str& after, bool case_sensitive = true);
    str& replace(const std::regex& rx, const str& after);
    str& replace(std::function<int(char key, char& val)> func);

    //  基于本字符串生成新字符串
    str repeated(int times) const;
    str join(const std::vector<str>& s) const;
    str join(const str& s, ...) const;

    //  容量、内存管理
    void reserve(int size);
    void resize(int size);
    void resize(int size, char fill_char);

    //  内存管理
    void shrink_to_fit();
    void squeeze();

    //  Title 化
    str& title();

    //  反转
    str& inversion();

    //  字符串分割
    std::vector<str> split(const str& sep, bool case_sensitive = true) const;
    std::vector<str> split(const char* sep, bool case_sensitive = true) const;
    std::vector<str> split(char sep, bool case_sensitive = true) const;
    std::vector<str> split(const std::regex& rx) const;
    std::vector<str> split(std::function<int(int c, bool& match)>& func) const;
    std::vector<str> split_lines(bool keep_ends = false) const;
    std::vector<str> split_path(bool keep_ends = false) const;

    //  大小写转换
    str& to_lower();
    str& to_upper();
    str& swap_case();

    //  空白处理
    str& simplified();
    str& ltrim();
    str& rtrim();
    str& trim();

    //  字符串交换
    void swap(str& other);

    //  路径处理
    str basename() const;
    str& basename();
    str dirname() const;
    str& dirname();

    //  bool 映射
    bool to_bool(bool* ok = nullptr) const;
    str& assign(bool v) const;

    //  字符串转换为数字
    double to_double(bool* ok = nullptr) const;
    float to_float(bool* ok = nullptr) const;
    int8_t to_int8(bool* ok = nullptr, int base = 10) const;
    int16_t to_int16(bool* ok = nullptr, int base = 10) const;
    int32_t to_int32(bool* ok = nullptr, int base = 10) const;
    int64_t to_int64(bool* ok = nullptr, int base = 10) const;
    uint8_t to_uint8(bool* ok = nullptr, int base = 10) const;
    uint16_t to_uint16(bool* ok = nullptr, int base = 10) const;
    uint32_t to_uint32(bool* ok = nullptr, int base = 10) const;
    uint64_t to_uint64(bool* ok = nullptr, int base = 10) const;

    str& assign(double n, char format = 'g', int precision = 6);
    str& assign(float n, char format = 'g', int precision = 6);
    str& assign(int8_t n, int base = 10);
    str& assign(int16_t n, int base = 10);
    str& assign(int32_t n, int base = 10);
    str& assign(int64_t n, int base = 10);
    str& assign(uint8_t n, int base = 10);
    str& assign(uint16_t n, int base = 10);
    str& assign(uint32_t n, int base = 10);
    str& assign(uint64_t n, int base = 10);

    //  转换为 hash 值
    int32_t hash_code() const;

    //  数字转换为字符串
    static str number(double n, char format = 'g', int precision = 6);
    static str number(float n, char format = 'g', int precision = 6);
    static str number(int8_t n, int base = 10);
    static str number(int16_t n, int base = 10);
    static str number(int32_t n, int base = 10);
    static str number(int64_t n, int base = 10);
    static str number(uint8_t n, int base = 10);
    static str number(uint16_t n, int base = 10);
    static str number(uint32_t n, int base = 10);
    static str number(uint64_t n, int base = 10);

    //  运算符重载
    bool operator!=(const char* other) const;
    str& operator+=(char ch);
    str& operator+=(const str& other);
    str& operator+=(const char* str);
    bool operator<(const char* other) const;
    bool operator<=(const char* other) const;
    str& operator=(char ch);
    str& operator=(const char* str);
    bool operator==(const char* other) const;
    bool operator>(const char* other) const;
    bool operator>=(const char* other) const;
    reference operator[](int position);
    const_reference operator[](int position) const;
};

#endif // STR_H
