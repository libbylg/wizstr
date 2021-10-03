//
// Created by luolijun on 2021/9/30.
//
#ifndef STR_H
#define STR_H

#include <cstdint>
#include <functional>
#include <map>
#include <regex>
#include <string>
#include <vector>

#include "tiny/asserts.h"

namespace tiny {

class str {
public:
    typedef int32_t pos_type;
    typedef uint32_t size_type;
    typedef char char_type;

    typedef str& reference;
    typedef const str& const_reference;

    typedef const char_type* const_iterator;
    typedef char_type* iterator;

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef const reverse_iterator const_reverse_iterator;

    static const pos_type npos = -1;

public:
    explicit str(const char_type* str);
    str(str&& other) noexcept;
    str(const str& other);
    explicit str(char_type ch, size_type size);
    explicit str();
    str& operator=(str&& other) noexcept;
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
    char_type at(pos_type position) const;
    char_type back() const;
    char_type& back();
    char_type front() const;
    char_type& front();

    //  长度容量
    str::size_type size() const;
    str::size_type capacity() const;

    //  清空
    void clear();

    //  对接 C 接口
    const char_type* data() const;
    char_type* data();
    void attach(size_type cap, size_type len, char_type* buf);
    void detach(size_type& cap, size_type& len, char_type*& buf);

    //  首尾追加
    str& append(const str& str);
    str& append(char_type ch);
    str& append(const char_type* str, int len);
    str& append(const char_type* str);
    str& prepend(const str& str);
    str& prepend(char_type ch);
    str& prepend(const char_type* str, int len);
    str& prepend(const char_type* str);

    //  修改字符串：中间插入、首尾插入、任意位置删除
    str& insert(pos_type position, const str& str);
    str& insert(pos_type position, char_type ch);
    str& insert(pos_type position, const char_type* s, int size);
    str& insert(pos_type position, const char_type* s);

    void push_back(const str& other);
    void push_back(char_type ch);

    void pop_back();

    str& remove(pos_type position, int n);
    str& remove(char_type ch, bool ignore_case = false);
    str& remove(const char_type* str, bool ignore_case = false);
    str& remove(const str& str, bool ignore_case = false);
    str& remove(const std::regex& rx);
    str& remove(std::function<int(int c, bool& match)> func);

    str& erase(pos_type pos = 0, pos_type len = npos);
    iterator erase(const_iterator p);
    iterator erase(const_iterator first, const_iterator last);

    //  比较
    int compare(const str& other, bool ignore_case = false) const;
    int compare(const char_type* s, bool ignore_case = false) const;
    int compare(char_type c, bool ignore_case = false) const;

    //  是否包含子串
    bool contains(const str& s, bool ignore_case = false) const;
    bool contains(const char_type* s, bool ignore_case = false) const;
    bool contains(char_type ch, bool ignore_case = false) const;
    bool contains(const std::regex& rx) const;
    bool contains(std::regex& re) const;

    //  子串统计
    int count(const str& s, bool ignore_case = false) const;
    int count(const char_type* s, bool ignore_case = false) const;
    int count(char_type ch, bool ignore_case = false) const;
    int count(const std::regex& rx) const;

    //  前后缀操作
    bool has_suffix(const str& s, bool ignore_case = false) const;
    bool has_suffix(const char_type* str, bool ignore_case = false) const;
    bool has_suffix(char_type c, bool ignore_case = false) const;

    bool has_prefix(const str& s, bool ignore_case = false) const;
    bool has_prefix(const char_type* s, bool ignore_case = false) const;
    bool has_prefix(char_type c, bool ignore_case = false) const;

    bool remove_prefix(const str& s, bool ignore_case = false);
    bool remove_prefix(const char_type* s, bool ignore_case = false);
    bool remove_prefix(char_type c, bool ignore_case = false);

    bool remove_suffix(const str& s, bool ignore_case = false);
    bool remove_suffix(const char_type* s, bool ignore_case = false);
    bool remove_suffix(char_type c, bool ignore_case = false);

    //  填充
    str& fill(char_type ch, size_type n = -1);

    //  查找
    int index_of(const str& str, int from = 0, bool ignore_case = false) const;
    int index_of(const char_type* s, int from = 0, bool ignore_case = false) const;
    int index_of(char_type ch, int from = 0, bool ignore_case = false) const;
    int index_of(const std::regex& rx, int from = 0) const;
    int index_of(std::regex& rx, int from = 0) const;
    int index_of(std::function<int(int c, bool& match)> func, int from = 0) const;

    int last_index_of(const str& str, int from = -1, bool ignore_case = false) const;
    int last_index_of(char_type ch, int from = -1, bool ignore_case = false) const;
    int last_index_of(const char_type* str, int from = -1, bool ignore_case = false) const;
    int last_index_of(const std::regex& rx, int from = -1) const;
    int last_index_of(std::regex& rx, int from = -1) const;
    int last_index_of(std::function<int(int c, bool& match)> func, int from = -1) const;

    //  匹配
    bool is_match(std::regex& rx) const;
    bool is_match(const str& pattern) const;
    bool is_match(const char_type*& pattern) const;
    bool is_match_wild(const str& pattern, bool ignore_case = false) const;
    bool is_match_wild(const char_type*& pattern, bool ignore_case = false) const;

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
    str mid(pos_type position, int n = -1) const;

    //  定宽对齐调整
    str& ljust(int width, char_type fill = ' ', bool truncate = false);
    str& rjust(int width, char_type fill = ' ', bool truncate = false);
    str& center(int width, char_type fill = ' ', bool truncate = false);
    str& zfill(int width, char_type fill = ' ', bool truncate = false);

    //  子串替换
    str& replace(pos_type position, int n, const str& after);
    str& replace(pos_type position, int n, char_type after);
    str& replace(pos_type position, int n, const char_type* unicode, int size);
    str& replace(char_type before, char_type after, bool ignore_case = false);
    str& replace(const char_type* before, int blen, const char_type* after, int alen, bool ignore_case = false);
    str& replace(const str& before, const str& after, bool ignore_case = false);
    str& replace(char_type ch, const str& after, bool ignore_case = false);
    str& replace(const std::regex& rx, const str& after);
    str& replace(std::function<int(char_type key, char_type& val)> func);

    //  基于本字符串生成新字符串
    str repeated(int times) const;             //  返回本字符串重复 times 次后的副本
    str join(const std::vector<str>& s) const; //  用本字符串连接所有的s
    str join(const str& s, ...) const;         //  用本字符串连接所有的s
    str join(const char_type* s, ...) const;   //  用本字符串连接所有的s

    //  容量、内存管理
    void reserve(int size);                     //  容量预留
    void resize(int size);                      //  重新调整字符串长度，多出的部分未初始化
    void resize(int size, char_type fill_char); //  重新调整字符串长度，多出的部分用指定字符串填充
    void shrink_to_fit();                       //  内存调整到和字符串实际有效长度一致
    void squeeze();                             //  内存紧缩

    //  Title 化：首字母大写
    str& title();

    //  反转：字符串逆序
    str& inversion();

    //  字符串分割
    std::vector<str> split(const str& sep, bool ignore_case = false) const;
    std::vector<str> split(const char_type* sep, bool ignore_case = false) const;
    std::vector<str> split(char_type sep, bool ignore_case = false) const;
    std::vector<str> split(const std::regex& rx) const;
    std::vector<str> split(const std::regex& rx, std::function<int(const str& line)> output_func) const;
    std::vector<str> split(std::function<int(int c, bool& match)>& chars_func) const;
    std::vector<str> split_lines(bool keep_ends = false) const;
    std::vector<str> split_path() const;
    void split(const str& sep, bool case_sensitive, std::function<int(const str& line)> output_func) const;
    void split(const char_type* sep, bool case_sensitive, std::function<int(const str& line)> output_func) const;
    void split(char_type sep, bool case_sensitive, std::function<int(const str& line)> output_func) const;
    void split(std::function<int(int c, bool& match)>& chars_func, std::function<int(const str& line)> output_func) const;
    void split_lines(bool keep_ends, std::function<int(const str& line)> output_func) const;
    void split_path(std::function<int(const str& line)> output_func) const;

    //  大小写转换
    str& to_lower();  //  转换成大写
    str& to_upper();  //  转换成小写
    str& swap_case(); //  大小写互换

    //  空白处理
    str& simplified(); //   空白化简，多个空白用一个空白代替
    str& ltrim();      //  去掉左边的空白
    str& rtrim();      //  去掉右边的空白
    str& trim();       //  同时去掉左边和右边的空白

    //  展开
    str expand(const std::map<str, str>& kvs);
    str expand(std::function<int(const str& key, str& val)> provider);
    str expand_envs(const char_type* key, const char_type* val);
    str expand_envs();
    str expand_tabs(int tab_size = 8);
    str expand_tmpl(const std::map<str, str>& kvs);
    str expand_tmpl(std::function<int(const str& key, str& val)> provider);

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

    str& assign(double n, char_type format = 'g', int precision = 6);
    str& assign(float n, char_type format = 'g', int precision = 6);
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
    static str number(double n, char_type format = 'g', int precision = 6);
    static str number(float n, char_type format = 'g', int precision = 6);
    static str number(int8_t n, int base = 10);
    static str number(int16_t n, int base = 10);
    static str number(int32_t n, int base = 10);
    static str number(int64_t n, int base = 10);
    static str number(uint8_t n, int base = 10);
    static str number(uint16_t n, int base = 10);
    static str number(uint32_t n, int base = 10);
    static str number(uint64_t n, int base = 10);

    //  运算符重载
    bool operator!=(const char_type* other) const;
    str& operator+=(char_type ch);
    str& operator+=(const str& other);
    str& operator+=(const char_type* str);
    bool operator<(const char_type* other) const;
    bool operator<=(const char_type* other) const;
    str& operator=(char_type ch);
    str& operator=(const char_type* str);
    bool operator==(const char_type* other) const;
    bool operator>(const char_type* other) const;
    bool operator>=(const char_type* other) const;
    reference operator[](pos_type position);
    const_reference operator[](pos_type position) const;

private:
#ifdef _WIN32
#define __ORDER_LITTLE_ENDIAN__ 0x1234
#define __ORDER_BIG_ENDIAN__ 0x4321
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#endif

    struct layout_small {
        char_type data[15];
        int8_t len;
    };

    struct layout_large {
        char_type* data;
        size_type len;
        size_type cap;
    };

    struct layout_trait {
        enum type {
            small = 0x00,
            large = 0x01,
        };
        enum : uint8_t {
            type_bits = 1,
            type_mask = uint8_t(uint8_t(-1) << (sizeof(int8_t) * 8 - type_bits)),
            type_index = uint8_t(sizeof(layout_large) - 1),
        };
        enum : size_type {
            small_cap_max = size_type((sizeof(layout_large) - 1) - 1), //  small 布局时，容量是固定大小(不算 \0 不算最后的len)
            large_cap_max = size_type((size_type(-1) << type_bits) + 1),
        };
        enum {
            large_cap_mask = size_type(-1) << (sizeof(size_type) * 8 - type_bits),
            large_cap_shift = type_bits,
        };
    };

    //  小端序：内存的高地址存数值高位
    //  大端序：内存的高地址存数值低位
    //                                                              small                       large
    //  -----  -------------------------------------------------   -------------------------   --------------------------
    //         0         1         2         3          特征        type          len  cap      type           len     cap
    //  大端序  xxxx-xxxx xxxx-xxxx xxxx-xxxx 1xxx-xxxx 数值最低位    data[15]&0x80 len  14       data[15]&0x80  len     cap>>1
    //  小端序  xxxx-xxxx xxxx-xxxx xxxx-xxxx xxxx-xxx1 数值最高位    data[15]&0x80 len  14       data[15]&0x80  len     cap^0x80000000
    union layout_t {
        struct layout_small small;
        struct layout_large large;
        uint8_t data[16];
        constexpr layout_trait::type type() const {
            return (0 == (data[layout_trait::type_index] & layout_trait::type_mask)) ? layout_trait::type::small : layout_trait::type::large;
        }

        inline char_type* begin() {
            if (type() == layout_trait::type::small) {
                return small.data;
            }
            return large.data;
        }

        inline const char_type* begin() const {
            return begin();
        }

        inline size_type cap() const {
            if (type() == layout_trait::type::small) {
                //  small 布局采用的是固定大小
                return size_type(layout_trait::small_cap_max);
            }

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            return size_type(large.cap ^ layout_trait::large_cap_mask);
#else
            return large.cap >> layout_trait::large_cap_shift;
#endif
        }

        inline char_type* end() {
            if (type() == layout_trait::type::small) {
                return small.data + small.len;
            }
            return large.data + large.len;
        };

        inline const char_type* end() const {
            return end();
        }

        inline size_type len() const {
            if (type() == layout_trait::type::small) {
                return small.len;
            }
            return large.len;
        }

        inline void len(size_type n) {
            ASSERT(n >= 0);
            if (type() == layout_trait::type::small) {
                ASSERT(n <= layout_trait::small_cap_max);
                small.len = n;
            } else {
                large.len = n;
            }
        }

        //  n 为初始化的长度，s 为需要拷贝的字符串的长度（不含\0）
        void init(const char_type* s, size_type n) {
            ASSERT(n >= 0);
            if (n == 0) {
                small.data[n] = '\0';
                small.len = 0;
            } else if (n <= layout_trait::small_cap_max) {
                if (s != nullptr) {
                    memcpy(small.data, s, sizeof(char_type) * n);
                }
                small.data[n] = '\0';
                small.len = int8_t(n);
            } else {
                ASSERT(n <= layout_trait::large_cap_max);
                char_type* new_data = (char_type*)malloc(sizeof(char_type) * (n + 1));
                ASSERT(new_data != nullptr);
                if (s != nullptr) {
                    memcpy(new_data, s, sizeof(char_type) * (n + 1));
                }
                new_data[n] = '\0';
                large.data = new_data;
                large.len = n;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                large.cap = size_type(layout_trait::large_cap_mask | n);
#else
                large.cap = str::size_type(n << layout_trait::large_cap_shift);
#endif
            }
        }

        //  对象内部数据释放
        void destroy() {
            //  释放老数据
            if (type() == layout_trait::type::large) {
                free(large.data);
            }
        }

        //  预留数据
        void reserve(size_type n) {
            ASSERT(n >= 0);
            ASSERT(n <= layout_trait::large_cap_max);

            //  如果容量足够
            if (n <= cap()) {
                return;
            }

            //  容量不够时需要重新分配存储空间
            char_type* new_data = (char_type*)malloc(sizeof(char_type) * n);
            ASSERT(new_data != nullptr);

            //  还原数据
            size_type old_len = len();
            memcpy(new_data, begin(), sizeof(char_type) * len());

            //  释放旧
            if (type() == layout_trait::type::large) {
                free(large.data);
            }

            //  保存新数据,len 长度不变
            large.data = new_data;
            large.data[old_len] = '\0';

            //  改变容量
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            large.cap = size_type(layout_trait::large_cap_mask | n);
#else
            large.cap = str::size_type(n << layout_trait::large_cap_shift);
#endif
        }

        //  改变长度和\0，但是内容不调整
        void resize(size_type n) {
            reserve(n);
            len(n);
            begin()[n] = '\0';
        }

        //  往缓冲区填入数据，但必须由外部保证填入的数据不会溢出
        void fill(pos_type pos, const char_type* d, size_type n) {
            ASSERT(pos >= 0);
            ASSERT(n >= 0);
            ASSERT(d != nullptr);
            ASSERT(cap() >= len() + n);
            memcpy(begin() + pos, d, n * sizeof(char_type));
        }

        //  往缓冲区填入数据，但必须由外部保证填入的数据不会溢出
        void fill(pos_type pos, char_type c, size_type n) {
            ASSERT(pos >= 0);
            ASSERT(n >= 0);
            ASSERT(cap() >= len() + n);
            std::fill(begin() + pos, begin() + pos + n, c);
        }
    };

    union layout_t layout;

    static_assert(sizeof(union layout_t) == 2 * sizeof(uintptr_t));
    static_assert(sizeof(uintptr_t) == 8);
};

} // namespace tiny

#endif // STR_H
