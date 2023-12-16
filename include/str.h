//
// Created by libbylg on 2023/6/1.
//

#ifndef TINY_STR_H
#define TINY_STR_H

#include <cinttypes>
#include <functional>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include <cstring>

class str {
public:
    using size_type = std::string::size_type;
    using ssize_type = ssize_t;
    using value_type = std::string::value_type;
    using reference = std::string::reference;
    using const_reference = std::string::const_reference;
    using pointer = std::string::pointer;
    using const_pointer = std::string::const_pointer;

    static const size_type npos = std::string::npos;

    using string_proc = std::function<std::optional<std::string_view>()>;
    using string_pair_proc = std::function<std::optional<std::tuple<std::string_view, std::string_view>>()>;
    using char_match_proc = std::function<int(value_type ch, bool& match)>;
    using range_search_proc = std::function<std::optional<std::string_view>(std::string_view search_range)>;

//    template<typename Sequance>
//    static auto enumerator(const Sequance& seq) -> string_proc {
//        auto itr = seq.
//        return [seq]() -> std::optional<std::string_view> {
//
//        }
//    }

    //!  在尾部追加
    static auto append(std::string& s, std::string_view other) -> std::string&;
    static auto append(std::string& s, value_type ch) -> std::string&;
    static auto append(std::string& s, value_type ch, size_type n) -> std::string&;
    //
    static auto append(std::string& s, string_provider proc) -> std::string&;
    static auto append(std::string& s, const std::vector<std::string>& items) -> std::string&;
    static auto append(std::string& s, const std::vector<std::string_view>& items) -> std::string&;
    static auto append(std::string& s, std::initializer_list<std::string> items) -> std::string&;
    static auto append(std::string& s, std::initializer_list<std::string_view> items) -> std::string&;

    //!  在头部追加
    static auto prepend(std::string& s, std::string_view other) -> std::string&;
    static auto prepend(std::string& s, value_type ch) -> std::string&;
    static auto prepend(std::string& s, value_type ch, size_type n) -> std::string&;

    //  修改字符串：中间插入、首尾插入、任意位置删除
    static auto insert(std::string& s, size_type pos, std::string_view other) -> std::string&;
    static auto insert(std::string& s, size_type pos, value_type ch) -> std::string&;
    static auto insert(std::string& s, size_type pos, value_type ch, size_type n) -> std::string&;

    //  在字符串尾部追加
    static auto push_back(std::string& s, std::string_view other) -> std::string&;
    static auto push_back(std::string& s, value_type ch) -> std::string&;
    static auto push_back(std::string& s, value_type ch, size_type n) -> std::string&;

    //  在字符串首部追加
    static auto push_front(std::string& s, std::string_view other) -> std::string&;
    static auto push_front(std::string& s, value_type ch) -> std::string&;
    static auto push_front(std::string& s, value_type ch, size_type n) -> std::string&;

    // 删除最后一个字符，如果当前字符串为空，返回 '\0'
    static auto pop_back(std::string& s) -> value_type;
    static auto pop_front(std::string& s) -> value_type;

    // 删除最后一个字段，并返回该字段
    static auto pop_back_field(std::string& s) -> std::string;
    static auto pop_front_field(std::string& s) -> std::string;

    //  部分数据移动：柔性移动和裁剪移动
    static auto flex_move(std::string& s, size_type pos, size_type n, ssize_type offset) -> std::string&;
    static auto flex_move(std::string& s, size_type pos, size_type n, ssize_type offset, value_type ch) -> std::string&;
    static auto clip_move(std::string& s, size_type pos, size_type n, ssize_type offset) -> std::string&;

    //  删除数据
    static auto remove(std::string& s, size_type pos) -> std::string&;
    static auto remove(std::string& s, size_type pos, size_type n) -> std::string&;
    static auto remove(std::string& s, size_type pos, ssize_type offset) -> std::string&;
    static auto remove(std::string& s, value_type ch) -> std::string&;
    static auto remove(std::string& s, std::string_view other) -> std::string&;
    static auto remove(std::string& s, char_match_proc proc) -> std::string&;
    static auto remove(std::string& s, func) -> std::string&;

    //  比较
    static auto compare(std::string_view s, std::string_view other) -> int;
    static auto compare(std::string_view s, const_pointer other) -> int;
    static auto compare(std::string_view s, std::string_view other, size_type max_n) -> int;
    static auto compare(std::string_view s, value_type c) -> int;

    // 不区分大小写的比较
    static auto icompare(std::string_view s, std::string_view other) -> int;
    static auto icompare(std::string_view s, std::string_view other, size_type max_n) -> int;
    static auto icompare(std::string_view s, std::string_view other, size_type max_n) -> int;
    static auto icompare(std::string_view s, value_type c) -> int;

    // 判断是否相等
    static auto equals(std::string_view s, std::string_view other) -> int;
    static auto equals(std::string_view s, const_pointer other) -> int;
    static auto equals(std::string_view s, std::string_view other, size_type max_n) -> int;
    static auto equals(std::string_view s, const_pointer other, size_type max_n) -> int;
    static auto equals(std::string_view s, value_type c) -> int;

    // 不区分大小写，判断是否相等
    static auto iequals(std::string_view s, std::string_view other) -> int;
    static auto iequals(std::string_view s, const_pointer other) -> int;
    static auto iequals(std::string_view s, std::string_view other, size_type max_n) -> int;
    static auto iequals(std::string_view s, const_pointer other, size_type max_n) -> int;
    static auto iequals(std::string_view s, value_type c) -> int;

    //  是否包含子串
    static auto contains(std::string_view s, std::string_view other) -> bool;
    static auto contains(std::string_view s, value_type ch) -> bool;

    //  子串统计
    static auto count(std::string_view s, std::string_view other) -> size_type;
    static auto count(std::string_view s, value_type ch) -> size_type;
    static auto count(std::string_view s, char_match_proc proc) -> size_type;

    // 计算公共前后缀的长度
    static auto prefix(std::string_view s, std::string_view other) -> size_type;
    static auto suffix(std::string_view s, std::string_view other) -> size_type;

    //  前缀操作
    static auto has_prefix(std::string_view s, value_type ch) -> bool;
    static auto has_prefix(std::string_view s, const std::string_view& prefix) -> bool;
    static auto starts_with(std::string_view s, value_type ch) -> bool;
    static auto starts_with(std::string_view s, const std::string_view& prefix) -> bool;
    static auto remove_prefix(std::string& s, std::string_view prefix) -> std::string&;
    static auto remove_prefix(std::string& s, value_type prefix) -> std::string&;

    //  后缀操作
    static auto has_suffix(std::string_view s, value_type suffix) -> bool;
    static auto has_suffix(std::string_view s, std::string_view suffix) -> bool;
    static auto ends_with(std::string_view s, value_type suffix) -> bool;
    static auto ends_with(std::string_view s, std::string_view suffix) -> bool;
    static auto remove_suffix(std::string& s, std::string_view suffix) -> std::string&;
    static auto remove_suffix(std::string& s, value_type suffix) -> std::string&;

    //  填充
    static auto fill(std::string& s, std::string_view other, size_type pos = 0, size_type max_n = npos) -> std::string&;
    static auto fill(std::string& s, value_type ch, size_type pos = 0, size_type max_n = npos) -> std::string&;

    //  从指定的位置查找特定的字符
    static auto index_of(std::string_view s, size_type pos, proc) -> size_type;
    static auto index_of(std::string_view s, size_type pos, value_type ch) -> size_type;
    static auto index_of(std::string_view s, size_type pos, std::string_view other) -> size_type;
    static auto index_of_eol(std::string_view s, size_type pos) -> size_type;
    static auto last_index_of(std::string_view s, std::string_view other) -> size_type;
    static auto last_index_of(std::string_view s, value_type ch) -> size_type;
    static auto last_index_of_eol(std::string_view& s, size_type pos) -> size_type;

    //  按空格分割的字段查找
    static auto index_of_field(std::string_view s, size_type pos) -> size_type;
    static auto last_index_of_field(std::string_view s, size_type pos) -> size_type;

    //  按各种方式遍历
    static auto walk(std::string_view s, size_type pos, size_type n, std::function<int(const_pointer ptr, size_type n, const_pointer next)> proc) -> void;
    static auto walk_byte(std::string_view s, size_type pos, size_type n, std::function<int(const_pointer ptr)> proc) -> void;
    static auto walk_byte(std::string_view s, size_type pos, size_type n, std::function<int(pointer ptr)> proc) -> void;
    static auto walk_field(std::string_view s, size_type pos, size_type n, std::function<int(const_pointer ptr, size_type n)> proc) -> void;

    //  匹配
    static auto is_match_wild(std::string_view s, std::string_view pattern) -> bool;
    static auto is_match(std::string_view s, uint16_t charset) -> bool;

    //  字符串特征
    static auto is_empty(std::string s) -> bool;
    static auto is_lower(std::string s) -> bool;
    static auto is_upper(std::string s) -> bool;
    static auto is_title(std::string s) -> bool;
    static auto is_digit(std::string s) -> bool;
    static auto is_xdigit(std::string s) -> bool;
    static auto is_ascii(std::string s) -> bool;
    static auto is_alpha(std::string s) -> bool;
    static auto is_alnum(std::string s) -> bool;
    static auto is_space(std::string s) -> bool;
    static auto is_blank(std::string s) -> bool;
    static auto is_print(std::string s) -> bool;
    static auto is_graph(std::string s) -> bool;

    // 是否一个标识符
    static auto is_identifier(std::string_view s) -> bool;
    static auto is_bool(std::string_view s) -> bool;

    //  按数量提取子串
    static auto left_n(std::string_view s, size_type n) -> std::string;
    static auto right_n(std::string_view s, size_type n) -> std::string;

    // 提取子串
    static auto substr(std::string_view s, size_type pos, ssize_type offset) -> std::string;

    //  按定宽，向右对齐
    static auto rjust_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto rjust(std::string_view s, size_type width, value_type ch = ' ') -> std::string;

    //  按定宽，向左对齐
    static auto ljust_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto ljust(std::string_view s, size_type width, value_type ch = ' ') -> std::string;

    //  按定宽，居中对齐
    static auto center_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto center(std::string_view s, size_type width, value_type ch = ' ') -> std::string;

    //  按定宽，向右对齐，对齐之后左侧使用 0 填充
    static auto zfill_inplace(std::string& s, size_type width) -> std::string&;
    static auto zfill(std::string_view s, size_type width) -> std::string;

    // 生成字符串 s 或者 字符 ch 的内容重复出现 times 次后的新字符串
    static auto repeat(std::string_view s, size_type times) -> std::string;
    static auto repeat(value_type ch, size_type times) -> std::string;

    // 生成由 width 个空格组成的新字符串
    static auto space(size_type width) -> std::string;

    //  基于本字符串生成新字符串
    static auto join(const std::string& s, join_proc proc) -> std::string;
    static auto join(const std::string& s, const std::vector<std::string>& items) -> std::string;
    static auto join(const std::string& s, std::initializer_list<std::string> items) -> std::string;
    static auto join(const std::string& s, std::initializer_list<const_pointer> items) -> std::string;
    static auto join(const std::string& s, std::initializer_list<std::string_view> items) -> std::string;

    // 使用逗号和冒号拼接 map
    static auto join_map(string_pair_proc proc) -> std::string;
    static auto join_map(std::map<std::string, std::string> items) -> std::string;

    // 使用逗号拼接 list
    static auto join_list(string_pair_proc proc) -> std::string;
    static auto join_list(std::map<std::string, std::string> items) -> std::string;

    // 文件路径拼接
    static auto join_path(string_proc proc) -> std::string;
    static auto join_path(const std::vector<std::string>& items) -> std::string;
    static auto join_path(const std::vector<std::string_view>& items) -> std::string;
    static auto join_path(std::initializer_list<std::string_view> items) -> std::string;

    // 拼接成搜索路径
    static auto join_search_path(string_proc proc) -> std::string;
    static auto join_search_path(const std::vector<std::string>& items) -> std::string;
    static auto join_search_path(const std::vector<std::string_view>& items) -> std::string;
    static auto join_search_path(std::initializer_list<std::string_view> items) -> std::string;

    // 字符串拼接
    static auto concat(string_proc proc) -> std::string;
    static auto concat(const std::vector<std::string>& items) -> std::string;
    static auto concat(const std::vector<std::string_view>& items) -> std::string;
    static auto concat(std::initializer_list<std::string_view> items) -> std::string;

    //  Title 化：首字母大写
    static auto capitalize(std::string& s) -> std::string&;
    static auto capitalize(const std::string& s) -> std::string;

    static auto title(std::string& s) -> std::string&;

    static auto title(const std::string& s) -> std::string;

    static auto title_fields(std::string& s) -> std::string&;
    static auto title_fields(const std::string& s) -> const std::string&;

    //  反转：字符串逆序
    static auto invert(std::string& s, size_type pos = 0, size_type max_n = npos) -> std::string&;

    static auto invert(const std::string& s, size_type pos = 0, size_type max_n = npos) -> std::string;

    // 拆分字符串
    using split_list_proc = std::function<int(std::string_view item)>;
    static auto split_list(const std::string& s, const std::string_view& sep, split_list_proc proc) -> void;

    static auto split_list(const std::string& s, const std::string& sep, split_list_proc proc) -> void;
    static auto split_list(const std::string& s, value_type sep, split_list_proc proc) -> void;

    static auto split_list(const std::string& s, const std::string_view& sep) -> std::vector<std::string>;

    static auto split_list(const std::string& s, value_type sep) -> std::vector<std::string>;

    static auto split_list(const std::string& s, const std::string& sep) -> std::vector<std::string>;

    // 将字符串 s，按照逗号和冒号拆分成一个 map 对象
    using split_map_proc = std::function<int(const std::string_view& key, const std::string_view& value)>;
    static auto split_map(const std::string& s, split_map_proc proc) -> void;
    static auto split_map(const std::string& s) -> std::map<std::string, std::string>;

    // 按照换行符将字符串 s，拆分长多行
    using split_lines_proc = std::function<int(const std::string_view& line)>;
    static auto split_lines(const std::string& s, bool keep_ends, split_lines_proc proc) -> void;

    // 将字符串 s 视作目录，按照路径分隔符，拆分成多个组成部分
    using split_path_proc = std::function<int(const std::string_view& elem)>;
    static auto split_path(const std::string& s, split_path_proc proc) -> void;

    // 将 s 视作路径，拆分出该路径的驱动字符串（仅win下有效）
    static auto split_drive(const std::string& s) -> std::string;

    // 拆分 csv 数据
    static auto split_csv(const std::string& s) -> std::vector<std::string>;

    //  大小写转换
    static auto to_lower(std::string& s) -> std::string&;
    static auto to_lower(const std::string& s) -> std::string;
    static auto to_upper(std::string& s) -> std::string&;
    static auto to_upper(const std::string& s) -> std::string;
    static auto swap_case(std::string& s, std::string& other) -> void;
    static auto case_fold(std::string& s) -> std::string&;
    static auto case_fold(const std::string& s) -> std::string;

    // 字符映射
    using translate_proc = std::function<value_type(value_type)>;
    static auto translate(std::string& s, translate_proc proc) -> std::string&;

    static auto translate(std::string& s, std::string_view from, std::string_view to) -> std::string&;

    // 字符串化简，将字符串中的多个空白压缩成一个空格
    using simplified_proc = std::function<bool(value_type ch)>;
    static auto simplified(std::string& s, simplified_proc proc) -> std::string&;

    static auto simplified(const std::string& s) -> std::string;

    static auto simplified(std::string& s) -> std::string&;

    // 空白祛除
    using trim_proc = std::function<bool(value_type ch)>;
    static auto ltrim(const std::string& s, trim_proc proc) -> std::string;
    static auto ltrim(const std::string& s) -> std::string;
    static auto ltrim(std::string& s) -> std::string&;
    static auto rtrim(const std::string& s, trim_proc proc) -> std::string;
    static auto rtrim(const std::string& s) -> std::string;
    static auto rtrim(std::string& s) -> std::string&;
    static auto trim(const std::string& s, trim_proc proc) -> std::string;
    static auto trim(const std::string& s) -> std::string;
    static auto trim(std::string& s) -> std::string&;
    static auto trim_all(const std::string& s, trim_proc proc) -> std::string;
    static auto trim_all(const std::string& s) -> std::string;
    static auto trim_all(std::string& s) -> std::string&;

    // 切除
    static auto drop_right(std::string& s, size_type n) -> std::string&;
    static auto drop_right(const std::string& s, size_type n) -> std::string;
    static auto drop_left(std::string& s, size_type n) -> std::string&;
    static auto drop_left(const std::string& s, size_type n) -> std::string;

    // 变量展开
    using expand_vars_proc = std::function<std::optional<std::string>(const std::string& key)>;
    static auto expand_envs(const std::string& s, expand_vars_proc proc) -> std::string;
    static auto expand_envs(const std::string& s, const std::map<std::string, std::string>& kvs) -> std::string;
    static auto expand_envs(const std::string& s, const std::tuple<const std::string&, const std::string&>& pair) -> std::string;
    static auto expand_envs(const std::string& s, const std::tuple<const std::string_view, const std::string_view>& pair) -> std::string;
    static auto expand_envs(const std::string& s) -> std::string;

    // 将字符串中的 tab 符号(\t)按照 tab 宽度替换成空白
    static auto expand_tabs(const std::string& s, size_type tab_size = 8) -> std::string;

    // 扩展字符串中的 ~ 前缀
    static auto expand_user(const std::string& s) -> std::string;

    // 路径正常化
    static auto normpath(const std::string& s) -> std::string;

    //  拷贝和交换
    static auto copy(pointer dest, size_type max_n, const std::string& s) -> size_type;

    // 路径处理
    static auto is_abs(const std::string& s) -> bool;

    // 将 s 视作为文件路径，获取其目录名
    static auto dirname(const std::string& s) -> std::string;

    //  处理路径中文件名的部分
    static auto basename_ptr(const std::string& s) -> const_pointer;

    static auto basename_ptr(std::string& s) -> pointer;

    static auto basename(const std::string& s) -> std::string;

    static auto remove_basename(const std::string& s) -> std::string;

    static auto remove_basename(std::string& s) -> std::string&;

    static auto replace_basename(std::string& s, const std::string& name) -> std::string&;

    static auto replace_basename(std::string& s, const std::string_view& name) -> std::string&;

    static auto replace_basename(const std::string& s, const std::string& name) -> std::string;

    static auto replace_basename(const std::string& s, const std::string_view& name) -> std::string;

    // 扩展名相关操作
    static auto extname_ptr(const std::string& s) -> const_pointer;
    static auto extname_ptr(std::string& s) -> pointer;

    static auto extname(const std::string& s) -> std::string;
    static auto remove_extname(const std::string& s) -> std::string;

    static auto remove_extname(std::string& s) -> std::string&;

    static auto replace_extname(std::string& s, const std::string& name) -> std::string&;

    static auto replace_extname(std::string& s, const std::string_view& name) -> std::string&;
    static auto replace_extname(const std::string& s, const std::string& name) -> std::string;
    static auto replace_extname(const std::string& s, const std::string_view& name) -> std::string;

    // 最后一截扩展名相关操作
    static auto last_extname_ptr(const std::string& s) -> const_pointer;

    static auto last_extname_ptr(std::string& s) -> pointer;

    static auto last_extname(const std::string& s) -> std::string;

    static auto remove_last_extname(const std::string& s) -> std::string;
    static auto remove_last_extname(std::string& s) -> std::string&;
    static auto replace_last_extname(std::string& s, const std::string& name) -> std::string&;
    static auto replace_last_extname(std::string& s, const std::string_view& name) -> std::string&;
    static auto replace_last_extname(const std::string& s, const std::string& name) -> std::string;
    static auto replace_last_extname(const std::string& s, const std::string_view& name) -> std::string;

    //  转换为 hash 值
    static auto hash(const std::string& s, uint32_t mod) -> uint32_t;
    static auto hash(const std::string& s, uint64_t mod) -> uint64_t;
    static auto md5(const std::string& s) -> std::string;

    template <typename T>
    static auto to(const std::string& s, std::tuple<int> base) -> std::optional<T> {
        return {};
    }

    template <typename T>
    static auto to(const std::string& s) -> std::optional<T> {
        return {};
    }

    template <typename T>
    static auto to(const std::string& s, T def, std::tuple<int> base) -> T {
        auto result = to<T>(s, base);
        return result ? result.value() : def;
    }

    template <typename T>
    static auto to(const std::string& s, T def) -> T {
        auto result = to<T>(s);
        return result ? result.value() : def;
    }

    template <>
    inline auto to<bool>(const std::string& s [[maybe_unused]]) -> std::optional<bool> {
        return {};
    }

    template <>
    inline auto to<float>(const std::string& s) -> std::optional<float> {
        errno = 0;
        char* endptr = nullptr;
        auto result = std::strtof(s.c_str(), &endptr);
        static_assert(sizeof(result) >= sizeof(float));
        if (result <= std::numeric_limits<float>::epsilon()) {
            if (endptr == s.c_str()) {
                return {};
            }
        }

        if (errno == ERANGE) {
            return {};
        }

        return result;
    }

    template <>
    inline auto to<double>(const std::string& s) -> std::optional<double> {
        errno = 0;
        char* endptr = nullptr;
        auto result = std::strtod(s.c_str(), &endptr);
        static_assert(sizeof(result) >= sizeof(double));
        if (result <= std::numeric_limits<double>::epsilon()) {
            if (endptr == s.c_str()) {
                return {};
            }
        }

        if (errno == ERANGE) {
            return {};
        }

        return result;
    }

    static constexpr auto correct_base(int base) -> int {
        if (base != 0) {
            if (base < 2) {
                return 2;
            }
            if (base > 36) {
                return 36;
            }
        }

        return base;
    }

    template <>
    inline auto to<int8_t>(const std::string& s, std::tuple<int> base) -> std::optional<int8_t> {
        int nbase = correct_base(std::get<0>(base));
        errno = 0;
        char* endptr = nullptr;
        auto result = std::strtol(s.c_str(), &endptr, nbase);
        static_assert(sizeof(result) >= sizeof(int8_t));
        if (result == 0) {
            if (endptr == s.c_str()) {
                return {};
            }
        }

        if (errno == ERANGE) {
            return {};
        }

        if ((result > std::numeric_limits<int8_t>::max()) || (result < std::numeric_limits<int8_t>::min())) {
            return {};
        }

        return result;
    }

    template <>
    inline auto to<int16_t>(const std::string& s, std::tuple<int> base) -> std::optional<int16_t> {
        int nbase = correct_base(std::get<0>(base));

        errno = 0;
        char* endptr = nullptr;
        auto result = std::strtol(s.c_str(), &endptr, nbase);
        static_assert(sizeof(result) >= sizeof(int16_t));
        if (result == 0) {
            if (endptr == s.c_str()) {
                return {};
            }
        }

        if (errno == ERANGE) {
            return {};
        }

        if ((result > std::numeric_limits<int16_t>::max()) || (result < std::numeric_limits<int16_t>::min())) {
            return {};
        }

        return result;
    }

    template <>
    inline auto to<int32_t>(const std::string& s, std::tuple<int> base) -> std::optional<int32_t> {
        int nbase = correct_base(std::get<0>(base));

        errno = 0;
        char* endptr = nullptr;
        auto result = std::strtol(s.c_str(), &endptr, nbase);
        static_assert(sizeof(result) >= sizeof(int32_t));
        if (result == 0) {
            if (endptr == s.c_str()) {
                return {};
            }
        }

        if (errno == ERANGE) {
            return {};
        }

        if ((result > std::numeric_limits<int32_t>::max()) || (result < std::numeric_limits<int32_t>::min())) {
            return {};
        }

        return result;
    }

    template <>
    inline auto to<int64_t>(const std::string& s, std::tuple<int> base) -> std::optional<int64_t> {
        int nbase = correct_base(std::get<0>(base));

        errno = 0;
        char* endptr = nullptr;
#if defined __LP64__
        auto result = std::strtol(s.c_str(), &endptr, nbase);
#else
        auto result = std::strtoll(s.c_str(), &endptr, nbase);
#endif
        static_assert(sizeof(result) >= sizeof(int64_t));
        if (result == 0) {
            if (endptr == s.c_str()) {
                return {};
            }
        }

        if (errno == ERANGE) {
            return {};
        }

        if ((result > std::numeric_limits<int64_t>::max()) || (result < std::numeric_limits<int64_t>::min())) {
            return {};
        }

        return result;
    }

    template <>
    inline auto to<uint8_t>(const std::string& s, std::tuple<int> base) -> std::optional<uint8_t> {
        int nbase = correct_base(std::get<0>(base));

        errno = 0;
        char* endptr = nullptr;
        auto result = std::strtoul(s.c_str(), &endptr, nbase);
        static_assert(sizeof(result) >= sizeof(uint8_t));
        if (result == 0) {
            if (endptr == s.c_str()) {
                return {};
            }
        }

        if (errno == ERANGE) {
            return {};
        }

        if ((result > std::numeric_limits<uint8_t>::max())) {
            return {};
        }

        return result;
    }

    template <>
    inline auto to<uint16_t>(const std::string& s, std::tuple<int> base) -> std::optional<uint16_t> {
        int nbase = correct_base(std::get<0>(base));

        errno = 0;
        char* endptr = nullptr;
        auto result = std::strtoul(s.c_str(), &endptr, nbase);
        static_assert(sizeof(result) >= sizeof(uint16_t));
        if (result == 0) {
            if (endptr == s.c_str()) {
                return {};
            }
        }

        if (errno == ERANGE) {
            return {};
        }

        if ((result > std::numeric_limits<uint16_t>::max())) {
            return {};
        }

        return result;
    }

    template <>
    inline auto to<uint32_t>(const std::string& s, std::tuple<int> base) -> std::optional<uint32_t> {
        int nbase = correct_base(std::get<0>(base));

        errno = 0;
        char* endptr = nullptr;
        auto result = std::strtoul(s.c_str(), &endptr, nbase);
        static_assert(sizeof(result) >= sizeof(uint32_t));
        if (result == 0) {
            if (endptr == s.c_str()) {
                return {};
            }
        }

        if (errno == ERANGE) {
            return {};
        }

        if ((result > std::numeric_limits<uint32_t>::max())) {
            return {};
        }

        return result;
    }

    template <>
    inline auto to<uint64_t>(const std::string& s, std::tuple<int> base) -> std::optional<uint64_t> {
        int nbase = correct_base(std::get<0>(base));

        errno = 0;
        char* endptr = nullptr;
#if defined __LP64__
        auto result = std::strtoul(s.c_str(), &endptr, nbase);
#else
        auto result = std::strtoull(s.c_str(), &endptr, nbase);
#endif
        static_assert(sizeof(result) >= sizeof(int64_t));
        if (result == 0) {
            if (endptr == s.c_str()) {
                return {};
            }
        }

        if (errno == ERANGE) {
            return {};
        }

        return result;
    }

    static auto from(double n, value_type format = 'g', int precision = 6) -> std::string;
    static auto from(float n, value_type format = 'g', int precision = 6) -> std::string;
    static auto from(int8_t n, int base = 10) -> std::string;
    inline auto from(int16_t n, int base = 10) -> std::string;
    inline auto from(int32_t n, int base = 10) -> std::string;
    inline auto from(int64_t n, int base = 10) -> std::string;
    inline auto from(uint8_t n, int base = 10) -> std::string;
    inline auto from(uint16_t n, int base = 10) -> std::string;
    inline auto from(uint32_t n, int base = 10) -> std::string;
    inline auto from(uint64_t n, int base = 10) -> std::string;

    // 转义
    static auto encode_cstr(std::string& s) -> std::string&;
    static auto decode_cstr(std::string& s) -> std::string&;
    static auto encode_xml(std::string& s) -> std::string&;
    static auto decode_xml(std::string& s) -> std::string&;
    static auto encode_hex(std::string& s) -> std::string&;
    static auto decode_hex(std::string& s) -> std::string&;
    static auto encode_base64(std::string& s) -> std::string&;
    static auto decode_base64(std::string& s) -> std::string&;
    static auto encode_url(std::string& s) -> std::string&;
    static auto decode_url(std::string& s) -> std::string&;

    static auto join_properties(std::string& s) -> std::string&;
    static auto split_properties(std::string& s) -> std::string&;
}; // namespace str

#endif // TINY_STR_H
