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

#include <array>
#include <bitset>
#include <cstring>
#include <regex>

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

    using charset_type = std::bitset<256>;

    // 生产器
    using view_provider_proc = std::function<std::optional<std::string_view>()>;
    using view_pair_provider_proc = std::function<std::optional<std::tuple<std::string_view, std::string_view>>()>;

    // 消费器
    using view_consumer_proc = std::function<int(std::string_view item)>;
    using view_pair_consumer_proc = std::function<int(std::string_view key, std::string_view value)>;

    //  映射和校验
    using char_mapping_proc = std::function<value_type(value_type)>;
    using char_checker_proc = std::function<bool(value_type ch)>;

    // 匹配和检索
    using char_match_proc = std::function<std::optional<bool>(value_type ch)>;
    using range_search_proc = std::function<std::optional<std::string_view>(std::string_view search_range)>;

    struct scope_t {
        size_type pos;
        ssize_type offset;
    };

    //!  在尾部追加
    static auto append(std::string& s, std::string_view other) -> std::string&;
    static auto append(std::string& s, std::string_view other, size_type n) -> std::string&;
    static auto append(std::string& s, value_type ch) -> std::string&;
    static auto append(std::string& s, value_type ch, size_type n) -> std::string&;
    static auto append(std::string& s, const view_provider_proc& proc) -> std::string&;
    static auto append(std::string& s, std::initializer_list<std::string_view> others) -> std::string&;

    //!  在头部追加
    static auto prepend(std::string& s, std::string_view other) -> std::string&;
    static auto prepend(std::string& s, std::string_view other, size_type n) -> std::string&;
    static auto prepend(std::string& s, value_type ch) -> std::string&;
    static auto prepend(std::string& s, value_type ch, size_type n) -> std::string&;
    static auto prepend(std::string& s, const view_provider_proc& proc) -> std::string&;

    //  修改字符串：中间插入、首尾插入、任意位置删除
    static auto insert(std::string& s, size_type pos, std::string_view other) -> std::string&;
    static auto insert(std::string& s, size_type pos, std::string_view other, size_type n) -> std::string&;
    static auto insert(std::string& s, size_type pos, value_type ch) -> std::string&;
    static auto insert(std::string& s, size_type pos, value_type ch, size_type n) -> std::string&;
    static auto insert(std::string& s, size_type pos, const view_provider_proc& proc) -> std::string&;

    // 删除最后一个字符，如果当前字符串为空，返回 '\0'
    static auto pop_back_char(std::string& s) -> value_type;
    static auto pop_front_char(std::string& s) -> value_type;

    // 一个字段，并返回该字段
    static auto pop_back_word(std::string& s) -> std::string;
    static auto pop_front_word(std::string& s) -> std::string;
    static auto remove_back_word(std::string& s) -> std::string&;
    static auto remove_front_word(std::string& s) -> std::string&;

    //  部分数据移动：柔性移动和裁剪移动
    static auto flex_move(std::string& s, size_type pos, size_type n, ssize_type offset) -> std::string&;
    static auto flex_move(std::string& s, size_type pos, size_type n, ssize_type offset, value_type ch) -> std::string&;
    static auto clip_move(std::string& s, size_type pos, size_type n, ssize_type offset) -> std::string&;

    //  删除数据
    static auto remove(std::string& s, size_type pos) -> std::string&;
    static auto remove(std::string& s, size_type pos, size_type n) -> std::string&;
    static auto remove(std::string& s, scope_t scope) -> std::string&;
    static auto remove(std::string& s, value_type ch) -> std::string&;
    static auto remove(std::string& s, std::string_view other) -> std::string&;
    static auto remove(std::string& s, char_match_proc proc) -> std::string&;

    //  删除字符串 s 的前缀 prefix，如果 s 确实以 prefix 开头，执行移除
    static auto remove_prefix(std::string& s, std::string_view prefix) -> std::string&;
    static auto remove_prefix(std::string& s, value_type prefix) -> std::string&;
    static auto remove_prefix(std::string& s, size_type n) -> std::string&;

    //  后缀操作
    static auto remove_suffix(std::string& s, std::string_view suffix) -> std::string&;
    static auto remove_suffix(std::string& s, value_type suffix) -> std::string&;
    static auto remove_suffix(std::string& s, size_type n) -> std::string&;

    //  填充
    static auto fill(std::string& s, std::string_view other, size_type pos = 0, size_type max_n = npos) -> std::string&;
    static auto fill(std::string& s, value_type ch, size_type pos = 0, size_type max_n = npos) -> std::string&;

    //  子串提取
    static auto left_n(std::string& s, size_type n) -> std::string&;
    static auto right_n(std::string& s, size_type n) -> std::string&;
    static auto mid_n(std::string& s, size_type pos, size_type n) -> std::string&;
    static auto substr(std::string& s, size_type pos, ssize_type offset) -> std::string&;

    //  按定宽和指定方式对齐对齐
    static auto align_left(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_right(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_center(std::string& s, size_type width, value_type ch = ' ') -> std::string&;

    //  按定宽，向右对齐，对齐之后左侧使用 0 填充
    static auto zfill(std::string& s, size_type width) -> std::string&;

    //  capitalize 和 title 操作
    static auto capitalize(std::string& s) -> std::string&;
    static auto title(std::string& s) -> std::string&;
    static auto title_words(std::string& s) -> std::string&;

    //  特殊字符串生成
    static auto repeat(std::string& s, size_type times) -> std::string&;

    //  反转：字符串逆序
    static auto invert(std::string& s, size_type pos = 0, size_type max_n = npos) -> std::string&;

    //  大小写转换
    static auto to_lower(std::string& s) -> std::string&;
    static auto to_upper(std::string& s) -> std::string&;
    static auto swap_case(std::string& s) -> std::string&;
    static auto case_fold(std::string& s) -> std::string&;

    // 字符映射
    static auto translate(std::string& s, const char_mapping_proc& proc) -> std::string&;
    static auto translate(std::string& s, std::string_view from, std::string_view to) -> std::string&;

    // 字符串化简，将字符串中的多个空白压缩成一个空格
    static auto simplified(std::string& s, const char_checker_proc& proc) -> std::string&;
    static auto simplified(std::string& s) -> std::string&;

    // 去掉字符串左侧的空白
    static auto trim_left(std::string& s, char_checker_proc proc) -> std::string&;
    static auto trim_left(std::string& s) -> std::string&;

    // 去掉字符串右侧的空白
    static auto trim_right(std::string& s, char_checker_proc proc) -> std::string&;
    static auto trim_right(std::string& s) -> std::string&;

    // 去掉字符串首尾的空白
    static auto trim_surrounding(std::string& s, char_checker_proc proc) -> std::string&;
    static auto trim_surrounding(std::string& s) -> std::string&;

    // 去掉字符串中任何位置的空白
    static auto trim_anywhere(std::string s, char_checker_proc proc) -> std::string;
    static auto trim_anywhere(std::string& s) -> std::string&;

    // 切除
    static auto drop_left(std::string& s, size_type n) -> std::string&;
    static auto drop_right(std::string& s, size_type n) -> std::string&;

    // 变量展开
    using expand_vars_proc = std::function<std::optional<std::string>(const std::string& key)>;
    static auto expand_envs(std::string& s, expand_vars_proc proc) -> std::string&;
    static auto expand_envs(std::string& s, const std::map<std::string, std::string>& kvs) -> std::string&;
    static auto expand_envs(std::string& s, const std::tuple<const std::string&, const std::string&>& pair) -> std::string&;
    static auto expand_envs(std::string& s, const std::tuple<const std::string_view, const std::string_view>& pair) -> std::string&;
    static auto expand_envs(std::string& s) -> std::string&;

    // 将字符串中的 tab 符号(\t)按照 tab 宽度替换成空白
    static auto expand_tabs(std::string& s, size_type tab_size = 8) -> std::string&;

    // 扩展字符串中的 ~ 前缀
    static auto expand_user(std::string& s) -> std::string&;

    // 路径正常化
    static auto normpath(std::string& s) -> std::string&;

    // 将 s 视作为文件路径，获取其目录名
    static auto dirname(std::string& s) -> std::string&;
    static auto remove_dirname(std::string& s) -> std::string&;
    static auto replace_dirname(std::string& s, std::string_view newname) -> std::string&;

    //  处理路径中文件名的部分
    static auto basename(std::string& s) -> std::string&;
    static auto remove_basename(std::string& s) -> std::string&;
    static auto replace_basename(std::string& s, std::string_view name) -> std::string&;

    // 扩展名相关操作
    static auto extname(std::string& s) -> std::string&;
    static auto remove_extname(std::string& s) -> std::string&;
    static auto replace_extname(std::string& s, std::string_view name) -> std::string&;

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

    // 按 properties 格式拼接
    static auto join_properties(std::string& s) -> std::string&;
    static auto split_properties(std::string& s) -> std::string&;

    // 求和
    template <typename MappedType>
    using mapper_proc = std::function<auto(value_type)->MappedType>;
    template <typename MapperProc>
    static auto sum(std::string_view s, MapperProc mapper);

    //  自动统计本字符所属的字符集
    static auto chars(std::string_view s) -> charset_type;

    // 是否全都满足proc或者在set范围内
    static auto is_all(std::string_view s, mapper_proc<bool> proc);
    static auto is_all(std::string_view s, charset_type set);

    // 是否至少有一个满足proc或者在set范围内
    static auto has_any(std::string_view s, mapper_proc<bool> proc);
    static auto has_any(std::string_view s, charset_type set);

    // 挑出满足条件的字符
    static auto take(std::string_view s, mapper_proc<bool> proc);
    static auto take(std::string_view s, charset_type set);

    // 按 proc 将字符序列分成两组，左边的满足proc，右边不满足proc
    static auto grouped(std::string_view s, mapper_proc<bool> proc) -> std::tuple<std::string, std::string>;

    // 按块切分
    static auto chunked_view(std::string_view s, size_type size) -> std::vector<std::string_view>;
    static auto chunked(std::string_view s, size_type size) -> std::vector<std::string>;

    // 读取文件内容
    static auto read_line(FILE* file) -> std::string;
    static auto read_line(std::istream& file) -> std::string;
    static auto read_lines(FILE* file, std::function<int(size_type line_index, std::string_view line_text)> proc) -> void;
    static auto read_lines(FILE* file, size_type max_n = npos) -> std::vector<std::string>;
    static auto read_lines(std::istream& file, std::function<int(size_type line_index, std::string_view line_text)> proc) -> void;
    static auto read_lines(std::istream& file, size_type max_n = npos) -> std::vector<std::string>;
    static auto read_lines(const std::string& filename, std::function<int(size_type line_index, std::string_view line_text)> proc) -> void;
    static auto read_lines(const std::string& filename, size_type max_n = npos) -> std::vector<std::string>;

};
//
// template <typename T>
// static auto to(const std::string& s, std::tuple<int> base) -> std::optional<T> {
//     return {};
// }
//
// template <typename T>
// static auto to(const std::string& s) -> std::optional<T> {
//     return {};
// }
//
// template <typename T>
// static auto to(const std::string& s, T def, std::tuple<int> base) -> T {
//     auto result = to<T>(s, base);
//     return result ? result.value() : def;
// }
//
// template <typename T>
// static auto to(const std::string& s, T def) -> T {
//     auto result = to<T>(s);
//     return result ? result.value() : def;
// }
//
// template <>
// inline auto to<bool>(const std::string& s [[maybe_unused]]) -> std::optional<bool> {
//     return {};
// }
//
// template <>
// inline auto to<float>(const std::string& s) -> std::optional<float> {
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtof(s.c_str(), &endptr);
//     static_assert(sizeof(result) >= sizeof(float));
//     if (result <= std::numeric_limits<float>::epsilon()) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<double>(const std::string& s) -> std::optional<double> {
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtod(s.c_str(), &endptr);
//     static_assert(sizeof(result) >= sizeof(double));
//     if (result <= std::numeric_limits<double>::epsilon()) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     return result;
// }
//
// static constexpr auto correct_base(int base) -> int {
//     if (base != 0) {
//         if (base < 2) {
//             return 2;
//         }
//         if (base > 36) {
//             return 36;
//         }
//     }
//
//     return base;
// }
//
// template <>
// inline auto to<int8_t>(const std::string& s, std::tuple<int> base) -> std::optional<int8_t> {
//     int nbase = correct_base(std::get<0>(base));
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtol(s.c_str(), &endptr, nbase);
//     static_assert(sizeof(result) >= sizeof(int8_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     if ((result > std::numeric_limits<int8_t>::max()) || (result < std::numeric_limits<int8_t>::min())) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<int16_t>(const std::string& s, std::tuple<int> base) -> std::optional<int16_t> {
//     int nbase = correct_base(std::get<0>(base));
//
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtol(s.c_str(), &endptr, nbase);
//     static_assert(sizeof(result) >= sizeof(int16_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     if ((result > std::numeric_limits<int16_t>::max()) || (result < std::numeric_limits<int16_t>::min())) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<int32_t>(const std::string& s, std::tuple<int> base) -> std::optional<int32_t> {
//     int nbase = correct_base(std::get<0>(base));
//
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtol(s.c_str(), &endptr, nbase);
//     static_assert(sizeof(result) >= sizeof(int32_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     if ((result > std::numeric_limits<int32_t>::max()) || (result < std::numeric_limits<int32_t>::min())) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<int64_t>(const std::string& s, std::tuple<int> base) -> std::optional<int64_t> {
//     int nbase = correct_base(std::get<0>(base));
//
//     errno = 0;
//     char* endptr = nullptr;
// #if defined __LP64__
//     auto result = std::strtol(s.c_str(), &endptr, nbase);
// #else
//     auto result = std::strtoll(s.c_str(), &endptr, nbase);
// #endif
//     static_assert(sizeof(result) >= sizeof(int64_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     if ((result > std::numeric_limits<int64_t>::max()) || (result < std::numeric_limits<int64_t>::min())) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<uint8_t>(const std::string& s, std::tuple<int> base) -> std::optional<uint8_t> {
//     int nbase = correct_base(std::get<0>(base));
//
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtoul(s.c_str(), &endptr, nbase);
//     static_assert(sizeof(result) >= sizeof(uint8_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     if ((result > std::numeric_limits<uint8_t>::max())) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<uint16_t>(const std::string& s, std::tuple<int> base) -> std::optional<uint16_t> {
//     int nbase = correct_base(std::get<0>(base));
//
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtoul(s.c_str(), &endptr, nbase);
//     static_assert(sizeof(result) >= sizeof(uint16_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     if ((result > std::numeric_limits<uint16_t>::max())) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<uint32_t>(const std::string& s, std::tuple<int> base) -> std::optional<uint32_t> {
//     int nbase = correct_base(std::get<0>(base));
//
//     errno = 0;
//     char* endptr = nullptr;
//     auto result = std::strtoul(s.c_str(), &endptr, nbase);
//     static_assert(sizeof(result) >= sizeof(uint32_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     if ((result > std::numeric_limits<uint32_t>::max())) {
//         return {};
//     }
//
//     return result;
// }
//
// template <>
// inline auto to<uint64_t>(const std::string& s, std::tuple<int> base) -> std::optional<uint64_t> {
//     int nbase = correct_base(std::get<0>(base));
//
//     errno = 0;
//     char* endptr = nullptr;
// #if defined __LP64__
//     auto result = std::strtoul(s.c_str(), &endptr, nbase);
// #else
//     auto result = std::strtoull(s.c_str(), &endptr, nbase);
// #endif
//     static_assert(sizeof(result) >= sizeof(int64_t));
//     if (result == 0) {
//         if (endptr == s.c_str()) {
//             return {};
//         }
//     }
//
//     if (errno == ERANGE) {
//         return {};
//     }
//
//     return result;
// }
//
// static auto from(double n, value_type format = 'g', int precision = 6) -> std::string;
// static auto from(float n, value_type format = 'g', int precision = 6) -> std::string;
// static auto from(int8_t n, int base = 10) -> std::string;
// inline auto from(int16_t n, int base = 10) -> std::string;
// inline auto from(int32_t n, int base = 10) -> std::string;
// inline auto from(int64_t n, int base = 10) -> std::string;
// inline auto from(uint8_t n, int base = 10) -> std::string;
// inline auto from(uint16_t n, int base = 10) -> std::string;
// inline auto from(uint32_t n, int base = 10) -> std::string;
// inline auto from(uint64_t n, int base = 10) -> std::string;

#endif // TINY_STR_H
