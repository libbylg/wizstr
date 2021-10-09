//
// Created by luolijun on 2021/9/30.
//
#ifndef BYTES_H
#define BYTES_H

#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <regex>
#include <string>
#include <type_traits>
#include <vector>

#include "tiny/asserts.h"
#include "tiny/segment.h"

namespace tiny {

class re;

class bytes {
public:
    typedef int32_t pos_type;
    typedef uint32_t size_type;
    typedef char value_type;
    static_assert(sizeof(pos_type) == sizeof(size_type), "确保二者宽度一致");

    typedef value_type& reference;
    typedef const value_type& const_reference;

    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    typedef const_pointer const_iterator;
    typedef pointer iterator;

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    static const pos_type npos = -1;

    static_assert(std::is_signed<pos_type>::value, "位置必须是有符号数");
    static_assert(std::is_unsigned<size_type>::value, "长度必须是无符号数");

public:
    //  构造析构
    ~bytes();
    explicit bytes();
    explicit bytes(const_pointer s);
    bytes(const_pointer s, size_type n);
    bytes(value_type ch, size_type size);
    bytes(bytes&& s) noexcept;
    bytes(const bytes& s);
    bytes& operator=(bytes&& other) noexcept;
    bytes& operator=(const bytes& other);

    //  迭代器
    iterator begin();
    iterator end();

    //  常量迭代器
    const_iterator begin() const;
    const_iterator end() const;

    //  反向迭代器
    reverse_iterator rbegin();
    reverse_iterator rend();

    //  反向常量迭代器
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    //  元素访问
    value_type at(pos_type pos) const;
    value_type back() const;
    value_type& back();
    value_type front() const;
    value_type& front();

    //  长度容量
    size_type size() const;
    size_type capacity() const;

    //  清空
    void clear();

    //  对接 C 接口
    const_pointer data() const;
    pointer data();
    void attach(pointer buf, size_type len, size_type cap);

    //  首尾追加
    bytes& append(const bytes& other);
    bytes& append(value_type ch);
    bytes& append(const_pointer s, size_type len);
    bytes& append(const_pointer s);
    bytes& prepend(const bytes& other);
    bytes& prepend(value_type ch);
    bytes& prepend(const_pointer s, size_type len);
    bytes& prepend(const_pointer s);

    //  修改字符串：中间插入、首尾插入、任意位置删除
    bytes& insert(pos_type pos, const bytes& other);
    bytes& insert(pos_type pos, value_type ch);
    bytes& insert(pos_type pos, const_pointer s, size_type n);
    bytes& insert(pos_type pos, const_pointer s);

    //  首尾操作
    void push_back(const bytes& other);
    void push_back(value_type ch);
    void push_front(const bytes& other);
    void push_front(value_type ch);
    value_type pop_back();
    value_type pop_front();

    //  部分数据移动：柔性移动和裁剪移动
    bytes& flex_move(pos_type pos, size_type n, int offset);
    bytes& flex_move(pos_type pos, size_type n, int offset, value_type fill_ch);
    bytes& clip_move(pos_type pos, size_type n, int offset);

    bytes& remove(pos_type pos);
    bytes& remove(pos_type pos, size_type n);
    bytes& remove(value_type ch);
    bytes& remove(const_pointer s);
    bytes& remove(const bytes& other);
    bytes& remove(const re& rx);
    bytes& remove(std::function<int(value_type c, bool& match)> func);
    bytes& remove(std::function<int(const_pointer start, const_pointer end, const_pointer& match, size_type& n)> func);

    bytes& erase(pos_type pos = 0, pos_type len = npos);
    iterator erase(const_iterator p);
    iterator erase(const_iterator first, const_iterator last);

    //  比较
    int compare(const bytes& other) const;
    int compare(const_pointer s) const;
    int compare(const bytes& other, size_type max_n) const;
    int compare(const_pointer s, size_type max_n) const;
    int compare(value_type c) const;

    //  是否包含子串
    bool contains(const bytes& s) const;
    bool contains(const_pointer s) const;
    bool contains(value_type ch) const;
    bool contains(const re& rx) const;

    //  子串统计
    int count(const bytes& s) const;
    int count(const_pointer s) const;
    int count(value_type ch) const;
    int count(const re& rx) const;

    //  前后缀操作
    bool has_suffix(const bytes& s) const;
    bool has_suffix(const_pointer s) const;
    bool has_suffix(value_type c) const;
    bool has_suffix(const_pointer s, size_type n) const;

    bool has_prefix(const bytes& s) const;
    bool has_prefix(const_pointer s) const;
    bool has_prefix(value_type c) const;
    bool has_prefix(const_pointer s, size_type n) const;

    bool remove_prefix(const bytes& s);
    bool remove_prefix(const_pointer s);
    bool remove_prefix(value_type c);

    bool remove_suffix(const bytes& s);
    bool remove_suffix(const_pointer s);
    bool remove_suffix(value_type c);

    //  填充
    bytes& fill(value_type ch);
    bytes& fill(pos_type pos, value_type ch, size_type n);
    bytes& fill(pos_type pos, const_pointer s, size_type n);

    //  查找
    pos_type index_of(const bytes& other, pos_type from = 0) const;
    pos_type index_of(const_pointer s, pos_type from = 0) const;
    pos_type index_of(value_type ch, pos_type from = 0) const;
    pos_type index_of(const re& rx, pos_type from = 0) const;
    pos_type index_of(std::function<int(value_type c, bool& match)> func, pos_type from = 0) const;

    pos_type last_index_of(const bytes& other, pos_type from = -1) const;
    pos_type last_index_of(value_type ch, pos_type from = -1) const;
    pos_type last_index_of(const_pointer s, pos_type from = -1) const;
    pos_type last_index_of(const re& rx, pos_type from = -1) const;
    pos_type last_index_of(std::function<int(value_type c, bool& match)> func, pos_type from = -1) const;

    //  分段查找
    pos_type section_of(bytes::pos_type from, const_pointer& s, size_type& n) const;

    //  匹配
    bool is_match(const re& rx) const;
    bool is_match(const bytes& pattern) const;
    bool is_match(const_pointer pattern) const;
    bool is_match_wild(const bytes& pattern) const;
    bool is_match_wild(const_pointer pattern) const;
    bool is_match(uint16_t charset) const;

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
    bool is_blank() const;
    bool is_print() const;
    bool is_graph() const;
    bool is_identifier() const;
    bool is_numeric() const;
    bool is_bool() const;

    //  提取子串
    bytes left(size_type n) const;
    bytes right(size_type n) const;
    bytes substr(pos_type pos, int offset_n = -1) const;

    //  定宽对齐调整
    bytes& ljust(size_type width, value_type fill = ' ', bool truncate = false);
    bytes& rjust(size_type width, value_type fill = ' ', bool truncate = false);
    bytes& center(size_type width, value_type fill = ' ', bool truncate = false);
    bytes& zfill(size_type width, value_type fill = ' ', bool truncate = false);

    //  子串替换
    bytes& replace(pos_type pos, size_type n, const bytes& after);
    bytes& replace(pos_type pos, size_type n, value_type after);
    bytes& replace(pos_type pos, size_type n, const_pointer unicode, size_type size);
    bytes& replace(value_type before, value_type after);
    bytes& replace(const_pointer before, size_type blen, const_pointer after, size_type alen);
    bytes& replace(const bytes& before, const bytes& after);
    bytes& replace(value_type ch, const bytes& after);
    bytes& replace(const re& rx, const bytes& after);
    bytes& replace(std::function<int(value_type key, value_type& val)> func);

    //  基于本字符串生成新字符串
    bytes repeat(size_type times) const;                             //  返回本字符串重复 times 次后的副本
    bytes join(const std::vector<bytes>& s) const;                   //  用本字符串连接所有的s
    bytes join(const bytes& s, ...) const;                           //  用本字符串连接所有的s
    bytes join(const_pointer s, ...) const;                          //  用本字符串连接所有的s
    bytes join(std::function<const_pointer()> provider) const;       //  用本字符串连接所有的s
    bytes join(std::initializer_list<const_pointer> ptr_list) const; //  用本字符串连接所有的s
    bytes join(std::initializer_list<bytes> ptr_list) const;         //  用本字符串连接所有的s

    //  容量、内存管理
    void reserve(size_type size);                    //  容量预留
    void resize(size_type size);                     //  重新调整字符串长度，多出的部分未初始化
    void resize(size_type size, value_type fill_ch); //  重新调整字符串长度，多出的部分用指定字符串填充
    void shrink_to_fit();                            //  内存调整到和字符串实际有效长度一致
    void squeeze();                                  //  内存紧缩

    //  Title 化：首字母大写
    bytes& title();

    //  反转：字符串逆序
    bytes& inversion();

    //  字符串分割
    std::vector<bytes> split(const bytes& sep) const;
    std::vector<bytes> split(const_pointer sep) const;
    std::vector<bytes> split(value_type sep) const;
    std::vector<bytes> split(const re& r) const;
    std::vector<bytes> split(std::function<int(value_type c, bool& match)>& chars_func) const;
    std::vector<bytes> split_lines(bool keep_ends = false) const;
    std::vector<bytes> split_path() const;
    void split(const bytes& sep, std::function<int(const_pointer s, size_type n)> output_func) const;
    void split(const_pointer sep, std::function<int(const_pointer s, size_type n)> output_func) const;
    void split(value_type sep, std::function<int(const_pointer s, size_type n)> output_func) const;
    void split(const re& r, std::function<int(const_pointer s, size_type n)> output_func) const;
    void split(std::function<int(value_type c, bool& match)>& chars_func, std::function<int(const_pointer s, size_type n)> output_func) const;
    void split_lines(bool keep_ends, std::function<int(const_pointer s, size_type n)> output_func) const;
    void split_path(std::function<int(const_pointer s, size_type n)> output_func) const;

    //  大小写转换
    bytes& to_lower();  //  转换成大写
    bytes& to_upper();  //  转换成小写
    bytes& swap_case(); //  大小写互换

    //  空白处理
    bytes& simplified();                                         //   空白化简，多个空白用一个空白代替
    bytes& ltrim();                                              //  去掉左边的空白
    bytes& rtrim();                                              //  去掉右边的空白
    bytes& trim();                                               //  同时去掉左边和右边的空白
    bytes& ltrim(std::function<bool(value_type ch)> func);       //  去掉左边的满足条件的字符
    bytes& rtrim(std::function<bool(value_type ch)> func);       //  去掉右边的满足条件的字符
    bytes& trim(std::function<bool(value_type ch)> func);        //  去掉右边的满足条件的字符
    bytes& ltrim_until(std::function<bool(value_type ch)> func); //  去掉左边的字符直到截止条件满足
    bytes& rtrim_until(std::function<bool(value_type ch)> func); //  去掉右边的字符直到截止条件满足
    bytes& trim_until(std::function<bool(value_type ch)> func);  //  去掉右边的满足条件的字符

    //  展开
    bytes expand(const std::map<bytes, bytes>& kvs) const;
    bytes expand(std::function<int(const bytes& key, bytes& val)> provider) const;
    bytes expand_envs(const_pointer key, const_pointer val) const;
    bytes expand_envs() const;
    bytes expand_tabs(size_type tab_size = 8) const;
    bytes expand_tmpl(const std::map<bytes, bytes>& kvs) const;
    bytes expand_tmpl(std::function<int(const bytes& key, bytes& val)> provider) const;

    //  拷贝和交换
    void swap(bytes& other);
    size_type copy(pointer dest, size_type n, pos_type pos = 0) const;

    //  路径处理
    bytes basename() const;
    bytes& basename();
    bytes dirname() const;
    bytes& dirname();

    //  bool 映射
    bool to_bool(bool* ok = nullptr) const;
    bytes& assign(bool v);

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

    bytes& assign(double n, value_type format = 'g', int precision = 6);
    bytes& assign(float n, value_type format = 'g', int precision = 6);
    bytes& assign(int8_t n, int base = 10);
    bytes& assign(int16_t n, int base = 10);
    bytes& assign(int32_t n, int base = 10);
    bytes& assign(int64_t n, int base = 10);
    bytes& assign(uint8_t n, int base = 10);
    bytes& assign(uint16_t n, int base = 10);
    bytes& assign(uint32_t n, int base = 10);
    bytes& assign(uint64_t n, int base = 10);

    //  转换为 hash 值
    int32_t hash_code() const;

    //  数字转换为字符串
    static bytes number(double n, value_type format = 'g', int precision = 6);
    static bytes number(float n, value_type format = 'g', int precision = 6);
    static bytes number(int8_t n, int base = 10);
    static bytes number(int16_t n, int base = 10);
    static bytes number(int32_t n, int base = 10);
    static bytes number(int64_t n, int base = 10);
    static bytes number(uint8_t n, int base = 10);
    static bytes number(uint16_t n, int base = 10);
    static bytes number(uint32_t n, int base = 10);
    static bytes number(uint64_t n, int base = 10);

    //  运算符重载
    bool operator!=(const_pointer other) const;
    bytes& operator+=(value_type ch);
    bytes& operator+=(const bytes& other);
    bytes& operator+=(const_pointer s);
    bool operator<(const_pointer other) const;
    bool operator<=(const_pointer other) const;
    bytes& operator=(value_type ch);
    bytes& operator=(const_pointer s);
    bool operator==(const_pointer other) const;
    bool operator>(const_pointer other) const;
    bool operator>=(const_pointer other) const;
    reference& operator[](pos_type pos);
    const_reference& operator[](pos_type pos) const;

private:
#ifdef _WIN32
#define __ORDER_LITTLE_ENDIAN__ 0x1234
#define __ORDER_BIG_ENDIAN__ 0x4321
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#endif

    struct layout_tmplt {
        value_type _data[16];
    };

    struct layout_trait {
        enum type {
            small = 0x00,
            large = 0x01,
        };
        enum : uint8_t {
            type_bits = 1,
            type_mask = uint8_t(uint8_t(-1) << (sizeof(int8_t) * 8 - type_bits)),
            type_index = uint8_t(sizeof(layout_tmplt) - 1),
        };
        enum : size_type {
            small_cap_max = size_type((sizeof(layout_tmplt) - 1) - 1), //  small 布局时，容量是固定大小(不算 \0 不算最后的len)
            large_cap_max = size_type((size_type(-1) << type_bits) + 1),
        };
        enum {
            large_cap_mask = size_type(-1) << (sizeof(size_type) * 8 - type_bits),
            large_cap_shift = type_bits,
        };
    };

    struct layout_small {
        value_type _data[15];
        int8_t _len;

        void init(const_pointer s, size_type n) {
            ASSERT(n >= 0);
            ASSERT(n <= layout_trait::small_cap_max);

            if (n == 0) {
                _data[n] = '\0';
                _len = 0;
                return;
            }

            if (s != nullptr) {
                memcpy(_data, s, sizeof(value_type) * n);
            }
            _data[n] = '\0';
            _len = int8_t(n);
        }

        inline pointer begin() {
            return _data;
        }

        inline const_pointer begin() const {
            return _data;
        }

        inline pointer end() {
            return _data + _len;
        };

        inline const_pointer end() const {
            return _data + _len;
        }

        inline size_type len() const {
            return _len;
        }

        inline void len(size_type n) {
            ASSERT(n >= 0);
            ASSERT(n <= layout_trait::small_cap_max);
            _len = n;
        }

        inline size_type cap() const {
            return size_type(layout_trait::small_cap_max);
        }
    };

    struct layout_large {
        pointer _data;
        size_type _len;
        size_type _cap;

        void init(const_pointer s, size_type n) {
            ASSERT(n >= 0);
            ASSERT(n <= layout_trait::large_cap_max);

            pointer new_data = (pointer)malloc(sizeof(value_type) * (n + 1));
            ASSERT(new_data != nullptr);
            if (s != nullptr) {
                memcpy(new_data, s, sizeof(value_type) * (n + 1));
            }
            new_data[n] = '\0';
            _data = new_data;
            _len = n;
            cap(n);
        }

        inline pointer begin() {
            return _data;
        }

        inline const_pointer begin() const {
            return _data;
        }

        inline pointer end() {
            return _data + _len;
        };

        inline const_pointer end() const {
            return _data + _len;
        }

        inline size_type len() const {
            return _len;
        }

        inline void len(size_type n) {
            ASSERT(n >= 0);
            ASSERT(n <= layout_trait::large_cap_max);
            ASSERT(n <= cap());
            _len = n;
        }

        inline size_type cap() const {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            return size_type(_cap ^ layout_trait::large_cap_mask);
#else
            return size_type(_cap >> layout_trait::large_cap_shift);
#endif
        }

        inline void cap(size_type n) {
            ASSERT(n >= 0);
            ASSERT(n <= layout_trait::large_cap_max);
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            _cap = size_type(layout_trait::large_cap_mask | n);
#else
            _cap = size_type(n << layout_trait::large_cap_shift);
#endif
        }

        //  对象内部数据释放
        void destroy() {
            if (_data != nullptr) {
                free(_data);
            }
        }

        //  attach 指定的缓冲区，但对象内部数据覆盖问题不考虑
        void attach(size_type c, size_type n, pointer d) {
            ASSERT(c >= 0);
            ASSERT(c <= layout_trait::large_cap_max);
            ASSERT(n >= 0);
            ASSERT(n <= layout_trait::large_cap_max);
            ASSERT(n <= c);
            ASSERT(d != nullptr);

            cap(c);
            _len = n;
            _data = d;
        }

        //  detach 指定的缓冲区，但不考虑对象数据引用问题
        void detach(size_type& c, size_type& n, pointer& d) {
            c = cap();
            n = _len;
            d = _data;
        }
    };

    //  小端序：内存的高地址存数值高位
    //  大端序：内存的高地址存数值低位
    //                                                              small                       large
    //  -----  -------------------------------------------------   -------------------------   --------------------------
    //         0         1         2         3          特征        type          len  cap      type           len     cap
    //  大端序  xxxx-xxxx xxxx-xxxx xxxx-xxxx 1xxx-xxxx 数值最低位    data[15]&0x80 len  14       data[15]&0x80  len     cap>>1
    //  小端序  xxxx-xxxx xxxx-xxxx xxxx-xxxx xxxx-xxx1 数值最高位    data[15]&0x80 len  14       data[15]&0x80  len     cap^0x80000000
    union layout_t {
        layout_small small;
        layout_large large;
        layout_tmplt tmplt;
        inline layout_trait::type type() const {
            return (0 == (tmplt._data[layout_trait::type_index] & layout_trait::type_mask)) ? layout_trait::type::small : layout_trait::type::large;
        }

        inline pointer begin() {
            if (type() == layout_trait::type::small) {
                return small.begin();
            }
            return large.begin();
        }

        inline const_pointer begin() const {
            if (type() == layout_trait::type::small) {
                return small.begin();
            }
            return large.begin();
        }

        inline size_type cap() const {
            if (type() == layout_trait::type::small) {
                return small.cap();
            }
            return large.cap();
        }

        inline pointer end() {
            if (type() == layout_trait::type::small) {
                return small.end();
            }
            return large.end();
        };

        inline const_pointer end() const {
            if (type() == layout_trait::type::small) {
                return small.end();
            }
            return large.end();
        }

        inline size_type len() const {
            if (type() == layout_trait::type::small) {
                return small.len();
            }
            return large.len();
        }

        inline void len(size_type n) {
            if (type() == layout_trait::type::small) {
                return small.len(n);
            }
            return large.len(n);
        }

        //  n 为初始化的长度，s 为需要拷贝的字符串的长度（不含\0）
        void init(const_pointer s, size_type n) {
            if (n <= layout_trait::small_cap_max) {
                small.init(s, n);
                return;
            }

            large.init(s, n);
        }

        //  对象内部数据释放
        void destroy() {
            //  释放老数据
            if (type() == layout_trait::type::large) {
                large.destroy();
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

            //  如果容量不够时：首先数据需要拷贝，其次 large 场景缓冲区需要重新分配

            //  容量不够时需要重新分配存储空间
            size_type new_cap = n;
            pointer new_data = (pointer)malloc(sizeof(value_type) * new_cap);
            ASSERT(new_data != nullptr);

            //  还原数据
            size_type old_len = len();
            memcpy(new_data, begin(), sizeof(value_type) * old_len);

            //  如果需要，先释放旧缓冲区
            destroy();

            //  将 large 关联到对应的缓冲区
            large.attach(new_cap, old_len, new_data);
        }

        //  改变长度和，追加\0，但是内容不调整
        void resize(size_type n) {
            //  先预留内存
            reserve(n);

            //  直接改变长度
            len(n);

            //  增加结尾
            begin()[n] = '\0';
        }

        //  往缓冲区填入数据，但必须由外部保证填入的数据不会溢出
        void fill(pos_type pos, const_pointer s, size_type n) {
            ASSERT(pos >= 0);
            ASSERT(n >= 0);
            ASSERT(s != nullptr);
            ASSERT(pos + n <= cap());
            memcpy(begin() + pos, s, n * sizeof(value_type));
        }

        //  往缓冲区填入数据，但必须由外部保证填入的数据不会溢出
        void fill(pos_type pos, value_type c, size_type n) {
            ASSERT(pos >= 0);
            ASSERT(n >= 0);
            ASSERT(pos + n <= cap());
            std::fill(begin() + pos, begin() + pos + n, c);
        }

        //  高阶操作：移动 [pos, pos+n) 范围内的部分数据：
        //  当 offset < 0 时，向左移，超出 0     位置时，自动扩容，但中间部分数据为随机值；
        //  当 offset > 0 时，向右移，超出 len() 位置时，自动扩容，但中间部分数据为随机值；
        //  如果不发生扩容，字符串长度不变
        void flexmove(pos_type pos, size_type n, int offset, std::function<void(pointer begin, size_type n)> action) {
            ASSERT(n >= 0);
            ASSERT(pos >= 0);
            ASSERT(pos < len());
            ASSERT((pos + n) >= 0);
            ASSERT((pos + n) <= len());

            if (offset == 0) {
                return;
            }

            if (offset < 0) {
                //  如果未超过 [0,len()) 的范围
                if (pos >= -offset) {
                    std::memmove(begin() + (pos + offset), begin() + pos, n * sizeof(value_type));
                    return;
                }

                //  Here: 如果超出 [0,len()) 的范围

                //  重新计算新长度并重新分配内存
                size_type new_len = -offset - pos + len();
                size_type new_cap = new_len;
                pointer new_data = (pointer)malloc(new_cap * sizeof(value_type));
                ASSERT(new_data != nullptr);

                //  先将需要拷贝的数据拷贝过来
                std::memcpy(new_data, begin() + pos, n);

                //  计算交叉数据的长度
                pos_type corss_len = n - (-offset - pos);

                //  根据是否交叉决定如何拷贝数据
                if (corss_len > 0) {
                    //  如果有交叉
                    std::memcpy(new_data + n, begin() + corss_len, len() - corss_len);
                } else {
                    //  如果无交叉
                    std::memcpy(new_data + n - corss_len, begin(), len());
                    action(new_data, -offset - pos - n);
                }

                //  先销毁老的数据
                destroy();

                //  再关联新数据
                large.attach(new_cap, new_len, new_data);
                return;
            }

            if (offset > 0) {
                //  如果未超出 [0,len()) 的 范围
                if ((pos + n + offset) < len()) {
                    std::memmove(begin() + (pos + offset), begin() + pos, n * sizeof(value_type));
                    return;
                }

                //  Here: 如果超出 [0,len()) 的范围

                //  重新计算新长度并重新分配内存
                size_type new_len = offset + pos + len();
                size_type new_cap = new_len;
                pointer new_data = (pointer)malloc(new_cap * sizeof(value_type));
                ASSERT(new_data != nullptr);

                //  计算交叉数据的长度
                pos_type corss_len = len() - (pos + offset);

                //  先移动数据
                std::memcpy(new_data + (pos + offset), begin() + pos, n);

                //  根据是否交叉决定如何拷贝剩余的数据
                if (corss_len > 0) {
                    std::memcpy(new_data, begin(), pos + offset);
                } else {
                    std::memcpy(new_data, begin(), len());
                    action(new_data + len(), (pos + offset - len()));
                }

                //  先销毁老的数据
                destroy();

                //  再关联新数据
                large.attach(new_cap, new_len, new_data);
                return;
            }
        }

        //  高阶操作：移动 [pos, pos+n) 范围内的部分数据：
        //  当 offset < 0 时，向左移，超出 0     位置时，超出的部分会被截断
        //  当 offset > 0 时，向右移，超出 len() 位置时，超出的部分会被阶段
        //  不管如何移动，字符串总长度不变
        void clipmove(pos_type pos, size_type n, int offset) {
            ASSERT(n >= 0);
            ASSERT(pos >= 0);
            ASSERT(pos < len());
            ASSERT((pos + n) >= 0);
            ASSERT((pos + n) <= len());

            if (offset == 0) {
                return;
            }

            if (offset < 0) {
                //  如果未超过 [0,len()) 的范围
                if (pos >= -offset) {
                    std::memmove(begin() + (pos + offset), begin() + pos, n * sizeof(value_type));
                    return;
                }

                //  计算交叉数据的长度
                pos_type corss_len = n - (-offset - pos);

                //  如果移动越界太多，完全没有交叉部分，那么相当于没有做任何移动,否则执行移动
                if (corss_len > 0) {
                    std::memmove(begin(), begin() - offset, corss_len);
                    return;
                }
                return;
            }

            if (offset > 0) {
                //  如果未超出 [0,len()) 的 范围
                if ((pos + n + offset) < len()) {
                    std::memmove(begin() + (pos + offset), begin() + pos, n * sizeof(value_type));
                    return;
                }

                //  计算交叉数据的长度
                pos_type corss_len = len() - (pos + offset);

                //  如果移动越界太多，完全没有交叉部分，那么相当于没有做任何移动,否则执行移动
                if (corss_len > 0) {
                    std::memmove(begin() + pos + offset, begin() + pos, corss_len);
                    return;
                }

                return;
            }
        }
    };

    union layout_t layout;

    static_assert(sizeof(uintptr_t) == 8, "仅支持 64 位系统");
    static_assert(sizeof(layout_tmplt) == sizeof(layout_small), "确保内存布局是兼容的");
    static_assert(sizeof(layout_tmplt) == sizeof(layout_large), "确保内存布局是兼容的");
    static_assert(sizeof(layout_tmplt) == sizeof(layout_t), "确保内存布局是兼容的");
    static_assert((sizeof(layout_tmplt) % sizeof(uintptr_t)) == 0, "确保布局是指针的整数倍");
};

extern bool operator!=(const bytes& s1, const bytes& s2);
extern bool operator!=(const bytes::pointer s1, const bytes& s2);
extern const bytes operator+(const bytes& s1, const bytes& s2);
extern const bytes operator+(const bytes& s1, const bytes::pointer s2);
extern const bytes operator+(const char* s1, const bytes& s2);
extern const bytes operator+(bytes::value_type ch, const bytes& s);
extern const bytes operator+(const bytes& s, bytes::value_type ch);
extern bool operator<(const bytes& s1, const bytes& s2);
extern bool operator<(const bytes::pointer s1, const bytes& s2);
extern bool operator<=(const bytes& s1, const bytes& s2);
extern bool operator<=(const bytes::pointer s1, const bytes& s2);
extern bool operator==(const bytes& s1, const bytes& s2);
extern bool operator==(const bytes::pointer s1, const bytes& s2);
extern bool operator>(const bytes& s1, const bytes& s2);
extern bool operator>(const bytes::pointer s1, const bytes& s2);
extern bool operator>=(const bytes& s1, const bytes& s2);
extern bool operator>=(const bytes::pointer s1, const bytes& s2);

} // namespace tiny

#endif // BYTES_H
