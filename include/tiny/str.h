//
// Created by libbylg on 2023/6/1.
//

#ifndef TINY_STR_H
#define TINY_STR_H

#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <vector>

struct str {
    using size_type = std::string::size_type;
    using value_type = std::string::value_type;
    using reference = std::string::reference;
    using const_reference = std::string::const_reference;
    using pointer = std::string::pointer;
    using const_pointer = std::string::const_pointer;

    static const size_type npos = std::string::npos;

    //  首尾追加
    static std::string& append(std::string& s, const std::string& other);
    static std::string& append(std::string& s, value_type ch);
    static std::string& append(std::string& s, value_type ch, size_type n);
    static std::string& append(std::string& s, const_pointer other, size_type n);
    static std::string& append(std::string& s, const_pointer other);
    static std::string& append(std::string& s, const std::vector<std::string>& items);
    static std::string& append(std::string& s, std::initializer_list<std::string> items);
    static std::string& append(std::string& s, std::initializer_list<const_pointer> items);
    static std::string& append(std::string& s, std::function<const_pointer()> proc);
    static std::string& append(std::string& s, std::function<std::string_view()> proc);
    static std::string& append(std::string& s, std::function<const std::string&()> proc);
    static std::string& prepend(std::string& s, const std::string& other);
    static std::string& prepend(std::string& s, value_type ch);
    static std::string& prepend(std::string& s, value_type ch, size_type n);
    static std::string& prepend(std::string& s, const_pointer other, size_type len);
    static std::string& prepend(std::string& s, const_pointer other);

    //  修改字符串：中间插入、首尾插入、任意位置删除
    static std::string& insert(std::string& s, size_type pos, const_pointer other, size_type n);
    static std::string& insert(std::string& s, size_type pos, const std::string& other);
    static std::string& insert(std::string& s, size_type pos, value_type ch);
    static std::string& insert(std::string& s, size_type pos, const_pointer other);

    //  首尾操作
    static std::string& push_back(std::string& s, const_pointer other, size_type n);
    static std::string& push_back(std::string& s, const bytes& other);
    static std::string& push_back(std::string& s, value_type ch);
    static std::string& push_front(std::string& s, const_pointer other, size_type n);
    static std::string& push_front(std::string& s, const bytes& other);
    static std::string& push_front(std::string& s, value_type ch);
    static value_type pop_back(std::string& s);
    static value_type pop_front(std::string& s);
    static std::string pop_back_field(std::string& s);
    static std::string pop_front_field(std::string& s);

    //  部分数据移动：柔性移动和裁剪移动
    static std::string& flex_move(std::string& s, size_type pos, size_type n, int offset);
    static std::string& flex_move(std::string& s, size_type pos, size_type n, int offset, value_type ch);
    static std::string& clip_move(std::string& s, size_type pos, size_type n, int offset);

    //  删除数据
    static auto remove(std::string& s, size_type pos) -> std::string&;
    static auto remove(std::string& s, size_type pos, size_type n) -> std::string&;
    static auto remove(std::string& s, value_type ch) -> std::string&;
    static auto remove(std::string& s, const_pointer other) -> std::string&;
    static auto remove(std::string& s, const_pointer other, size_type n) -> std::string&;
    static auto remove(std::string& s, const std::string& other) -> std::string&;
    static auto remove(std::string& s, std::function<bool(value_type ch, bool& cntu)> func) -> std::string&;
    static auto remove(std::string& s, std::function<int(const_pointer search, size_type search_n, const_pointer& match, size_type& match_n)> func) -> std::string&;

    //  比较
    static auto compare(const std::string& s, const std::string& other) -> int;
    static auto compare(const std::string& s, const_pointer s) -> int;
    static auto compare(const std::string& s, const std::string& other, size_type max_n) -> int;
    static auto compare(const std::string& s, const_pointer s, size_type max_n) -> int;
    static auto compare(const std::string& s, value_type c) -> int;

    //  是否包含子串
    static auto contains(const std::string& s, const std::string& other) -> bool;
    static auto contains(const std::string& s, const_pointer other) -> bool;
    static auto contains(const std::string& s, const_pointer other, size_type n) -> bool;
    static auto contains(const std::string& s, value_type ch) -> bool;

    //  子串统计
    static auto count(const std::string& s, const_pointer s, size_type n) -> size_type;
    static auto count(const std::string& s, const bytes& s) -> size_type;
    static auto count(const std::string& s, const_pointer s) -> size_type;
    static auto count(const std::string& s, value_type ch) -> size_type;
    static auto count(const std::string& s, std::function<bool(value_type ch, bool& cntu)> macher) -> size_type;

    // 字符映射
    using translate_proc = s;
    td::function<value_type(value_type)> static uto translate(translate_proc proc);

    //  前后缀操作
    static auto has_prefix(const std::string& s, const_pointer s, size_type n) -> bool;
    static auto has_prefix(const std::string& s, const bytes& s) -> bool;
    static auto has_prefix(const std::string& s, const_pointer s) -> bool;
    static auto has_prefix(const std::string& s, value_type c) -> bool;
    static auto starts_with(const std::string& s, const bytes& other) -> bool;
    static auto starts_with(const std::string& s, value_type ch) -> bool;
    static auto starts_with(const std::string& s, const_pointer s) -> bool;

    static auto has_suffix(const std::string& s, const_pointer s, size_type n) -> bool;
    static auto has_suffix(const std::string& s, const bytes& s) -> bool;
    static auto has_suffix(const std::string& s, const_pointer s) -> bool;
    static auto has_suffix(const std::string& s, value_type c) -> bool;
    static auto ends_with(const std::string& s, const bytes& sv) -> bool;
    static auto ends_with(const std::string& s, value_type c) -> bool;
    static auto ends_with(const std::string& s, const_pointer s) -> bool;

    static auto prefix(const std::string& s, const std::string& other) -> size_type;
    static auto suffix(const std::string& s, const std::string& other) -> size_type;

    static auto remove_prefix(std::string& s, const_pointer s, size_type n) -> std::string&;
    static auto remove_prefix(std::string& s, const bytes& s) -> std::string&;
    static auto remove_prefix(std::string& s, const_pointer s) -> std::string&;
    static auto remove_prefix(std::string& s, value_type c) -> std::string&;

    static auto remove_suffix(std::string& s, const_pointer s, size_type n) -> std::string&;
    static auto remove_suffix(std::string& s, const bytes& s) -> std::string&;
    static auto remove_suffix(std::string& s, const_pointer s) -> std::string&;
    static auto remove_suffix(std::string& s, value_type c) -> std::string&;

    //  填充
    static auto fill(std::string& s, value_type ch) -> std::string&;
    static auto fill(std::string& s, size_type pos, value_type ch, size_type n) -> std::string&;
    static auto fill(std::string& s, size_type pos, const_pointer s, size_type n) -> std::string&;
    static auto fill(std::string& s, size_type fill_from, size_type fill_n, const_pointer s, size_type n) -> std::string&;
    static auto fill(std::string& s, size_type fill_from, size_type fill_n, const_pointer s) -> std::string&;

    //  查找
    static auto index_of(const_pointer s, size_type n, size_type from) -> size_type;
    static auto index_of(const bytes& other, size_type from = 0) -> size_type;
    static auto index_of(const_pointer s, size_type from = 0) -> size_type;
    static auto index_of(value_type ch, size_type from = 0) -> size_type;
    static auto index_of(std::function<bool(value_type ch, bool& cntu)> matcher, size_type from, size_type to) -> size_type;
    static auto index_of_lineend(bytes::size_type pos_bgn) -> size_type;

    static auto last_index_of(const_pointer s, size_type n, size_type from) -> size_type;
    static auto last_index_of(const bytes& other, size_type from = npos) -> size_type;
    static auto last_index_of(value_type ch, size_type from = npos) -> size_type;
    static auto last_index_of(const_pointer s, size_type from = npos) -> size_type;
    static auto last_index_of(std::function<bool(value_type ch, bool& cntu)> matcher, size_type from, size_type to) -> size_type;

    //  按空格分割的字段查找
    static auto index_of_field(size_type from, const_pointer& s, size_type& n) -> size_type;
    static auto last_index_of_field(size_type from, const_pointer& s, size_type& n) -> size_type;

    //  按各种方式遍历
    static auto walk(size_type from, offset_type offset, std::function<int(const_pointer s, size_type n, const_pointer next)> proc) -> void;
    static auto walk_byte(size_type from, offset_type offset, std::function<int(const_pointer ptr)> proc) -> void;
    static auto walk_byte(size_type from, offset_type offset, std::function<int(pointer ptr)> proc) -> void;
    static auto walk_field(size_type from, offset_type offset, std::function<int(const_pointer s, size_type n)> proc) -> void;
    static auto walk_field(size_type from, offset_type offset, std::function<int(pointer s, size_type n)> proc) -> void;

    //  匹配
    static auto is_match_wild(const bytes& pattern) -> bool;
    static auto is_match_wild(const_pointer pattern) -> bool;
    static auto is_match(uint16_t charset) -> bool;

    //  字符串特征
    static auto is_empty(const std::string& s) -> bool;
    static auto is_lower(const std::string& s) -> bool;
    static auto is_upper(const std::string& s) -> bool;
    static auto is_title(const std::string& s) -> bool;
    static auto is_digit(const std::string& s) -> bool;
    static auto is_ascii(const std::string& s) -> bool;
    static auto is_alpha(const std::string& s) -> bool;
    static auto is_alnum(const std::string& s) -> bool;
    static auto is_space(const std::string& s) -> bool;
    static auto is_blank(const std::string& s) -> bool;
    static auto is_print(const std::string& s) -> bool;
    static auto is_graph(const std::string& s) -> bool;
    static auto is_identifier(const std::string& s) -> bool;
    static auto is_bool(const std::string& s) -> bool;

    //  提取子串
    static auto left(const std::string& s, size_type n) -> std::string;
    static auto right(const std::string& s, size_type n) -> std::string;
    static auto substr(const std::string& s, size_type pos = 0, int n = npos) -> std::string;
    static auto cutstr(const std::string& s, size_type pos, offset_type offset) -> std::string;

    //  定宽对齐调整
    static auto ljust(std::string& s, size_type width, value_type ch = ' ', bool truncate = false) -> std::string&;
    static auto ljust(const std::string& s, size_type width, value_type ch = ' ', bool truncate = false) -> std::string;
    static auto rjust(std::string& s, size_type width, value_type ch = ' ', bool truncate = false) -> std::string&;
    static auto rjust(const std::string& s, size_type width, value_type ch = ' ', bool truncate = false) -> std::string;
    static auto center(std::string& s, size_type width, value_type ch = ' ', bool truncate = false) -> std::string&;
    static auto center(const std::string& s, size_type width, value_type ch = ' ', bool truncate = false) -> std::string;
    static auto zfill(std::string& s, size_type width) -> std::string&;
    static auto zfill(const std::string& s, size_type width) -> std::string;

    //  子串替换
    static auto replace(const std::string& s, value_type before, value_type after, size_type maxcount = -1) -> std::string;
    static auto replace(const std::string& s, const bytes& before, const bytes& after, size_type maxcount = -1) -> std::string;
    static auto replace(const std::string& s, value_type before, value_type after, size_type maxcount = -1) -> std::string;
    static auto replace(const std::string& s, const bytes& before, const bytes& after, size_type maxcount = -1) -> std::string;

    static auto repeat(const std::string& s, size_type times) -> std::string;
    static auto space(size_type width) -> std::string;

    //  基于本字符串生成新字符串
    static join_proc = std::function<const_pointer()>;
    static auto join(const std::string& s, join_proc proc) -> std::string;
    static auto join(const std::string& s, const std::vector<std::string>& items) -> std::string;
    static auto join(const std::string& s, std::initializer_list<std::string> items) -> std::string;
    static auto join(const std::string& s, std::initializer_list<const_pointer> items) -> std::string;
    static auto join(const std::string& s, std::initializer_list<std::string> items) -> std::string;

    // 路径拼接
    using join_path_proc = std::function<const_pointer()>;
    static auto join_path(join_path_proc proc) -> std::string;
    static auto join_path(std::initializer_list<std::string> items) -> std::string;
    static auto join_path(std::initializer_list<const_pointer> items) -> std::string;
    static auto join_path(const std::vector<std::string>& items) -> std::string;

    // 使用逗号和冒号拼接 map
    using join_map_proc = std::function<int(const_pointer key, size_type key_n, const_pointer val, size_type val_n)>;
    static auto join_map(join_map_proc proc) -> std::string;
    static auto join_map(std::map<std::string, std::string> items) -> std::string;

    // 拼接成搜索路径
    using join_search_path_proc = join_path_proc;
    static auto join_search_path(join_search_path_proc proc) -> std::string;
    static auto join_search_path(std::initializer_list<std::string> items) -> std::string;
    static auto join_search_path(std::initializer_list<const_pointer> items) -> std::string;
    static auto join_search_path(std::vector<std::string> items) -> std::string;

    //  Title 化：首字母大写
    static auto title(std::string& s) -> std::string&;
    static auto title(const std::string& s) -> const std::string&;
    static auto title_fields(std::string& s) -> std::string&;
    static auto title_fields(const std::string& s) -> const std::string&;

    //  反转：字符串逆序
    static auto invert(std::string& s, size_type pos, size_type n) -> std::string&;
    static auto invert(const std::string& s, size_type pos, size_type n) -> std::string;

    // 拆分字符串
    using split_list_proc = std::function<int(const_pointer s, size_type n)>;
    static auto split_list(const std::string& s, const std::string& sep) -> std::vector<std::string>;
    static auto split_list(const std::string& s, const std::string& sep, split_list_proc proc) const;
    static auto split_list(const std::string& s, value_type sep, split_list_proc proc) const;

    // 将字符串 s，按照逗号和冒号拆分成一个 map 对象
    using split_map_proc = std::function<int(const_pointer key, size_type key_n, const_pointer val, size_type val_n)>;
    static auto split_map(const std::string& s, split_map_proc proc) void;
    static auto split_map(const std::string& s) -> std::map<std::string, std::string>;

    // 按照换行符将字符串 s，拆分长多行
    static auto split_lines(const std::string& s, bool keepends, std::function<int(const_pointer s, size_type n)> proc) const;

    // 将字符串 s 视作目录，按照路径分隔符，拆分成多个组成部分
    static auto split_path(const std::string& s, std::function<int(const_pointer s, size_type n)> proc) const;

    // 将 s 视作路径，拆分出该路径的驱动字符串（仅win下有效）
    static auto split_drive(const std::string& s) -> std::string;

    //  大小写转换
    static auto to_lower(std::string& s) -> std::string&;
    static auto to_lower(const std::string& s) -> std::string;
    static auto to_upper(std::string& s) -> std::string&;
    static auto to_upper(const std::string& s) -> std::string;
    static auto swap_case(std::string& s, std::string& other) -> void;

    // 字符串化简，将字符串中的多个空白压缩成一个空格
    static auto simplified(const std::string& s) -> std::string;
    static auto simplified(std::string& s) -> std::string&;

    // 空白祛除
    static auto ltrim(const std::string& s, std::function<bool(value_type ch)> proc) -> std::string;
    static auto ltrim(const std::string& s) -> std::string;
    static auto ltrim(std::string& s) -> std::string&;
    static auto rtrim(const std::string& s, std::function<bool(value_type ch)> proc) -> std::string;
    static auto rtrim(const std::string& s) -> std::string;
    static auto rtrim(std::string& s) -> std::string&;
    static auto trim(const std::string& s, std::function<bool(value_type ch)> proc) -> std::string;
    static auto trim(const std::string& s) -> std::string;
    static auto trim(std::string& s) -> std::string&;
    static auto trim_all(const std::string& s, std::function<bool(value_type ch)> proc) -> std::string;
    static auto trim_all(const std::string& s) -> std::string;
    static auto trim_all(std::string& s) -> std::string&;

    // 变量展开
    static auto expand_envs(const std::string& s, const std::map<std::string, std::string>& kvs) -> std::string;
    static auto expand_envs(const std::string& s, std::function<int(const std::string& key, std::string& val)> provider) -> std::string;
    static auto expand_envs(const std::string& s, const_pointer key, const_pointer val) -> std::string;
    static auto expand_envs(const std::string& s, const std::string& key, const std::string& val) -> std::string;
    static auto expand_envs(const std::string& s) -> std::string;

    // 将字符串中的 tab 符号(\t)按照 tab 宽度替换成空白
    static auto expand_tabs(const std::string& s, size_type tab_size = 8) -> std::string;
    static auto expand_user(const std::string& s) -> std::string;

    // 路径正常化
    static auto normpath(const std::string& s) -> std::string;

    //  拷贝和交换
    static auto copy(pointer dest, size_type n, const std::string& s) size_type;

    // 将 s 视作为文件路径，获取其目录名
    static auto dirname(const std::string& s) -> std::string;

    //  处理路径中文件名的部分
    static auto basename_ptr(const std::string& s) -> const_pointer;
    static auto basename(const std::string& s) -> std::string;
    static auto remove_basename(const std::string& s) -> std::string;
    static auto remove_basename(std::string& s) -> std::string&;
    static auto replace_basename(const std::string& s, const std::string& name) -> std::string;
    static auto replace_basename(std::string& s, const std::string& name) -> std::string&;
    static auto replace_basename(std::string& s, const_pointer name) -> std::string&;
    static auto replace_basename(const std::string& s, const_pointer name) -> std::string;

    // 扩展名相关操作
    static auto extname_ptr(const std::string& s) -> const_pointer;
    static auto extname(const std::string& s) -> std::string;
    static auto remove_extname(const std::string& s) -> std::string;
    static auto remove_extname(std::string& s) -> std::string&;
    static auto replace_extname(const std::string& s, const std::string& name) -> std::string;
    static auto replace_extname(std::string& s, const std::string& name) -> std::string&;
    static auto replace_extname(std::string& s, const_pointer name) -> std::string&;
    static auto replace_extname(const std::string& s, const_pointer name) -> std::string;

    // 最后一截扩展名相关操作
    static auto last_extname_ptr(const std::string& s) -> const_pointer;
    static auto last_extname(const std::string& s) -> std::string;
    static auto remove_last_extname(const std::string& s) -> std::string;
    static auto remove_last_extname(std::string& s) -> std::string&;
    static auto replace_last_extname(const std::string& s, const std::string& name) -> std::string;
    static auto replace_last_extname(std::string& s, const std::string& name) -> std::string&;
    static auto replace_last_extname(std::string& s, const_pointer name) -> std::string&;
    static auto replace_last_extname(const std::string& s, const_pointer name) -> std::string;

    //  转换为 hash 值
    static auto hash(const std::string& s, uint32_t mod) -> uint32_t;
    static auto hash(const std::string& s, uint64_t mod) -> uint64_t;

    // 计算 s 和 other 的共同前缀或者共同后缀
    static auto prefix(const std::string& s, const std::string& other) -> size_type;
    static auto suffix(const std::string& s, const std::string& other) -> size_type;
};

//  数字转换为字符串
static bytes from(double n, value_type format = 'g', int precision = 6);
static bytes from(float n, value_type format = 'g', int precision = 6);
static bytes from(int8_t n, int base = 10);
static bytes from(int16_t n, int base = 10);
static bytes from(int32_t n, int base = 10);
static bytes from(int64_t n, int base = 10);
static bytes from(uint8_t n, int base = 10);
static bytes from(uint16_t n, int base = 10);
static bytes from(uint32_t n, int base = 10);
static bytes from(uint64_t n, int base = 10);

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

bytes& assign(size_type count, value_type ch);
bytes& assign(const bytes& other);
bytes& assign(const bytes& other, size_type pos);
bytes& assign(const bytes& other, size_type pos, size_type count);
bytes& assign(bytes&& other);
bytes& assign(const_pointer s);
bytes& assign(const_pointer s, size_type count);

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
#endif // TINY_STR_H
