//
// Created by libbylg on 2023/6/1.
//
#ifndef TINY_STR_H
#define TINY_STR_H

#include <cinttypes>
#include <functional>
#include <initializer_list>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <regex>
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

    static inline constexpr size_type npos = std::string::npos;

    class charset_type {
    public:
        explicit charset_type() {
        }

        explicit charset_type(std::string_view s)
            : charset_type() {
            for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ptr++) {
                set(*ptr);
            }
        }

        inline auto set(std::string_view s, bool val = true) -> void {
            for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ptr++) {
                set(*ptr, val);
            }
        }

        inline auto set(value_type c, bool val = true) -> void {
            uint8_t ch = static_cast<value_type>(c);
            size_type index = ch / 64;
            size_type mask = uint64_t{1} << (ch % 64);
            if (val) {
                bits[index] |= mask;
            } else {
                bits[index] ^= ~mask;
            }
        }

        inline auto clr(std::string_view s) -> void {
            for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ptr++) {
                clr(*ptr);
            }
        }

        inline auto clr(value_type c) -> void {
            uint8_t ch = static_cast<value_type>(c);
            size_type index = ch / 64;
            size_type mask = uint64_t{1} << (ch % 64);
            bits[index] ^= ~mask;
        }

        inline auto clr() -> void {
            bits[0] = 0;
            bits[1] = 0;
            bits[2] = 0;
            bits[3] = 0;
        }

        inline auto get(value_type c) const -> bool {
            uint8_t ch = static_cast<value_type>(c);
            size_type index = ch / 64;
            size_type mask = uint64_t{1} << (ch % 64);
            return !!(bits[index] & mask);
        }

        inline auto invert() {
            bits[0] = ~bits[0];
            bits[1] = ~bits[1];
            bits[2] = ~bits[2];
            bits[3] = ~bits[3];
        }

        inline auto operator[](value_type c) const -> bool {
            return get(c);
        }

        inline auto data() const -> const uint64_t* {
            return bits;
        }

        inline auto data() -> uint64_t* {
            return bits;
        }

    private:
        uint64_t bits[4]{0, 0, 0, 0};
    };

    // 生产器
    using view_provider_proc = std::function<std::optional<std::string_view>()>;
    using view_pair_provider_proc = std::function<std::optional<std::tuple<std::string_view, std::string_view>>()>;

    // 消费器
    using split_consumer_proc = std::function<int(size_type pos, size_type n, size_type sep_end)>;
    using view_consumer_proc = std::function<int(std::string_view item)>;
    using view_pair_consumer_proc = std::function<int(std::string_view key, std::string_view value)>;

    // path 消费器
    using path_consumer_proc = std::function<int(int8_t type, std::string_view path_elem)>;

    // 映射和校验
    using char_mapping_proc = std::function<value_type(value_type)>;
    using char_checker_proc = std::function<bool(value_type ch)>;

    // 匹配和检索
    using char_match_proc = std::function<bool(value_type ch)>;
    using range_search_proc = std::function<std::optional<std::string_view>(std::string_view search_range)>;

    // 行消费
    using line_consumer_proc = std::function<int(size_type line_index, std::string_view line_text)>;

    //! 在尾部追加
    static auto append(std::string_view s, std::string_view other) -> std::string;
    static auto append(std::string_view s, value_type ch) -> std::string;
    static auto append(std::string_view s, value_type ch, size_type n) -> std::string;
    static auto append(std::string_view s, const view_provider_proc& proc) -> std::string;

    static auto append_inplace(std::string& s, std::string_view other) -> std::string&;
    static auto append_inplace(std::string& s, std::string_view other, size_type n) -> std::string&;
    static auto append_inplace(std::string& s, value_type ch) -> std::string&;
    static auto append_inplace(std::string& s, value_type ch, size_type n) -> std::string&;
    static auto append_inplace(std::string& s, const view_provider_proc& proc) -> std::string&;
    static auto append_inplace(std::string& s, std::initializer_list<std::string_view> others) -> std::string&;

    //! 在头部追加
    static auto prepend(std::string_view s, std::string_view other) -> std::string;
    static auto prepend(std::string_view s, value_type ch) -> std::string;
    static auto prepend(std::string_view s, value_type ch, size_type n) -> std::string;
    static auto prepend(std::string_view s, const view_provider_proc& proc) -> std::string;

    static auto prepend_inplace(std::string& s, std::string_view other) -> std::string&;
    static auto prepend_inplace(std::string& s, std::string_view other, size_type n) -> std::string&;
    static auto prepend_inplace(std::string& s, value_type ch) -> std::string&;
    static auto prepend_inplace(std::string& s, value_type ch, size_type n) -> std::string&;
    static auto prepend_inplace(std::string& s, const view_provider_proc& proc) -> std::string&;

    // 修改字符串：中间插入、首尾插入、任意位置删除
    static auto insert(std::string_view s, size_type pos, std::string_view other) -> std::string;
    static auto insert(std::string_view s, size_type pos, value_type ch) -> std::string;
    static auto insert(std::string_view s, size_type pos, value_type ch, size_type n) -> std::string;
    static auto insert(std::string_view s, size_type pos, const view_provider_proc& proc) -> std::string;

    static auto insert_inplace(std::string& s, size_type pos, std::string_view other) -> std::string&;
    static auto insert_inplace(std::string& s, size_type pos, std::string_view other, size_type n) -> std::string&;
    static auto insert_inplace(std::string& s, size_type pos, value_type ch) -> std::string&;
    static auto insert_inplace(std::string& s, size_type pos, value_type ch, size_type n) -> std::string&;
    static auto insert_inplace(std::string& s, size_type pos, const view_provider_proc& proc) -> std::string&;

    // 不区分大小写的比较
    static auto icmp(std::string_view s, std::string_view other) -> int;
    static auto icmp(std::string_view s, std::string_view other, size_type max_n) -> int;

    // 不区分大小写，判断是否相等
    static auto iequals(std::string_view s, std::string_view other) -> bool;
    static auto iequals(std::string_view s, std::string_view other, size_type max_n) -> bool;

    // 匹配
    static auto wildcmp(const_pointer s, const_pointer pattern) -> bool;
    static auto wildcmp(std::string_view s, std::string_view pattern) -> bool;

    // 是否包含子串
    static auto contains(std::string_view s, std::string_view other) -> bool;
    static auto contains(std::string_view s, value_type ch) -> bool;

    // 子串统计
    static auto count(std::string_view s, std::string_view other) -> size_type;
    static auto count(std::string_view s, value_type ch) -> size_type;
    static auto count(std::string_view s, const char_match_proc& proc) -> size_type;

    // 计算公共前后缀的长度
    static auto prefix(std::string_view s, std::string_view other) -> size_type;
    static auto suffix(std::string_view s, std::string_view other) -> size_type;

    // 前缀操作
    static auto has_prefix(std::string_view s, value_type ch) -> bool;
    static auto has_prefix(std::string_view s, std::string_view prefix) -> bool;
    static auto starts_with(std::string_view s, value_type ch) -> bool;
    static auto starts_with(std::string_view s, std::string_view prefix) -> bool;
    static auto remove_prefix(std::string_view s, std::string_view prefix) -> std::string_view;
    static auto remove_prefix(std::string_view s, value_type prefix) -> std::string_view;

    static auto remove_prefix_inplace(std::string& s, std::string_view prefix) -> std::string&;
    static auto remove_prefix_inplace(std::string& s, value_type prefix) -> std::string&;

    // 后缀操作
    static auto has_suffix(std::string_view s, value_type suffix) -> bool;
    static auto has_suffix(std::string_view s, std::string_view suffix) -> bool;
    static auto ends_with(std::string_view s, value_type suffix) -> bool;
    static auto ends_with(std::string_view s, std::string_view suffix) -> bool;
    static auto remove_suffix(std::string_view s, std::string_view suffix) -> std::string_view;
    static auto remove_suffix(std::string_view s, value_type suffix) -> std::string_view;

    static auto remove_suffix_inplace(std::string& s, std::string_view suffix) -> std::string&;
    static auto remove_suffix_inplace(std::string& s, value_type suffix) -> std::string&;

    // 一次性查找
    static auto find_next_regex(std::string_view s, const std::regex& pattern, size_type pos = 0) -> std::optional<std::string_view>;
    static auto find_next_regex(std::string_view s, std::string_view pattern, size_type pos = 0) -> std::optional<std::string_view>;
    static auto find_next_eol(std::string_view s, size_type pos = 0) -> std::string_view;
    static auto find_next_word(std::string_view s, size_type pos = 0) -> std::string_view;

    // 迭代找下一个
    static auto iter_next_regex(std::string_view s, size_type& pos, const std::regex& pattern) -> std::optional<std::string_view>;
    static auto iter_next_regex(std::string_view s, size_type& pos, std::string_view pattern) -> std::optional<std::string_view>;
    static auto iter_next_string(std::string_view s, size_type& pos, std::string_view other) -> size_type;
    static auto iter_next_eol(std::string_view s, size_type& pos) -> std::string_view;
    static auto iter_next_word(std::string_view s, size_type& pos) -> std::string_view;

    // 按各种方式遍历
    static auto foreach_word(std::string_view s, size_type pos, const std::function<int(size_type pos, size_type n)>& proc) -> void;
    static auto foreach_word(std::string_view s, const std::function<int(size_type pos, size_type n)>& proc) -> void;
    static auto foreach_word(std::string_view s, size_type pos, const std::function<int(std::string_view word)>& proc) -> void;
    static auto foreach_word(std::string_view s, const std::function<int(std::string_view word)>& proc) -> void;

    // 字符串特征
    static auto is_lower(std::string_view s) -> bool;
    static auto is_upper(std::string_view s) -> bool;
    static auto is_title(std::string_view s) -> bool;
    static auto is_capitalize(std::string_view s) -> bool;
    static auto is_digit(std::string_view s) -> bool;
    static auto is_xdigit(std::string_view s) -> bool;
    static auto is_ascii(std::string_view s) -> bool;
    static auto is_alpha(std::string_view s) -> bool;
    static auto is_alnum(std::string_view s) -> bool;
    static auto is_alnumul(std::string_view s) -> bool;
    static auto is_space(std::string_view s) -> bool;
    static auto is_blank(std::string_view s) -> bool;
    static auto is_print(std::string_view s) -> bool;
    static auto is_graph(std::string_view s) -> bool;

    // 是否一个标识符
    static auto is_identifier(std::string_view s) -> bool;
    static auto is_literal_bool(std::string_view s) -> bool;
    static auto is_literal_true(std::string_view s) -> bool;
    static auto is_literal_false(std::string_view s) -> bool;
    static auto is_literal_real(std::string_view s) -> bool;
    static auto is_literal_integer(std::string_view s) -> bool;

    // 提取子串
    static auto take_left(std::string_view s, size_type n) -> std::string_view;
    static auto take_right(std::string_view s, size_type n) -> std::string_view;
    static auto take_mid(std::string_view s, size_type pos, size_type n) -> std::string_view;
    static auto take_range(std::string_view s, size_type begin_pos, size_type end_pos) -> std::string_view;
    static auto take(std::string_view s, size_type pos, ssize_type offset) -> std::string_view;
    static auto take(std::string_view s, size_type pos) -> std::string_view;
    static auto take(std::string_view s, char_checker_proc proc) -> std::string;
    static auto take(std::string_view s, charset_type charset) -> std::string;
    static auto drop_left(std::string_view s, size_type n) -> std::string_view;
    static auto drop_right(std::string_view s, size_type n) -> std::string_view;
    static auto drop_mid(std::string_view s, size_type pos, size_type n) -> std::string;
    static auto drop_range(std::string_view s, size_type begin_pos, size_type end_pos) -> std::string;
    static auto drop(std::string_view s, size_type pos, ssize_type offset) -> std::string;
    static auto drop(std::string_view s, size_type pos) -> std::string;
    static auto drop(std::string_view s, char_checker_proc proc) -> std::string;
    static auto drop(std::string_view s, charset_type charset) -> std::string;

    static auto take_left_inplace(std::string& s, size_type n) -> std::string&;
    static auto take_right_inplace(std::string& s, size_type n) -> std::string&;
    static auto take_mid_inplace(std::string& s, size_type pos, size_type n) -> std::string&;
    static auto take_range_inplace(std::string& s, size_type begin_pos, size_type end_pos) -> std::string&;
    static auto take_inplace(std::string& s, size_type pos, ssize_type offset) -> std::string&;
    static auto take_inplace(std::string& s, size_type pos) -> std::string&;
    static auto take_inplace(std::string& s, char_checker_proc proc) -> std::string&;
    static auto take_inplace(std::string& s, charset_type charset) -> std::string&;
    static auto drop_left_inplace(std::string& s, size_type n) -> std::string&;
    static auto drop_right_inplace(std::string& s, size_type n) -> std::string&;
    static auto drop_mid_inplace(std::string& s, size_type pos, size_type n) -> std::string&;
    static auto drop_range_inplace(std::string& s, size_type begin_pos, size_type end_pos) -> std::string&;
    static auto drop_inplace(std::string& s, size_type pos, ssize_type offset) -> std::string&;
    static auto drop_inplace(std::string& s, size_type pos) -> std::string&;
    static auto drop_inplace(std::string& s, char_checker_proc proc) -> std::string&;
    static auto drop_inplace(std::string& s, charset_type charset) -> std::string&;

    // 对齐
    static auto align_left(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_right(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_center(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_zfill(std::string_view s, size_type width) -> std::string;

    static auto align_left_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_right_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_center_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_zfill_inplace(std::string& s, size_type width) -> std::string&;

    // Title 化：首字母大写
    static auto capitalize(std::string_view s) -> std::string;
    static auto title(std::string_view s) -> std::string;

    static auto capitalize_inplace(std::string& s) -> std::string&;
    static auto title_inplace(std::string& s) -> std::string&;

    // 反转：字符串逆序
    static auto invert(std::string_view s, size_type pos = 0, size_type max_n = npos) -> std::string;

    static auto invert_inplace(std::string& s, size_type pos, size_type max_n) -> std::string&;

    // 生成字符串 s 或者 字符 ch 的内容重复出现 times 次后的新字符串
    static auto repeat(std::string_view s, size_type times) -> std::string;
    static auto repeat(value_type ch, size_type times) -> std::string;

    // 生成由 width 个空格组成的新字符串
    static auto spaces(size_type width) -> std::string;
    static auto skip_space_remain(std::string_view s, size_type pos = 0) -> std::string_view;
    static auto skip_space(std::string_view s, size_type pos) -> size_type;
    static auto skip_space_inplace(std::string& s, size_type pos) -> size_type;

    // 用指定的分隔符,拼接一个字符串序列
    static auto join_list(std::string_view s, const view_provider_proc& proc) -> std::string;
    static auto join_list(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_list(std::string_view s, const Sequence& items) -> std::string {
        std::string result;
        auto itr = items.begin();
        return join_list(s, [&items, &itr]() -> std::optional<std::string_view> {
            if (itr == items.end()) {
                return std::nullopt;
            }

            return *(itr++);
        });
    }

    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_list(const Sequence& items) -> std::string {
        return join_list(",", items);
    }

    // 使用逗号和冒号拼接 map
    static auto join_map(std::string_view sep_pair, std::string_view sep_list, const view_pair_provider_proc& proc) -> std::string;
    static auto join_map(const view_pair_provider_proc& proc) -> std::string;

    template <typename Map, typename = typename Map::const_iterator>
    static auto join_map(std::string_view sep_pair, std::string_view sep_list, const Map& items) -> std::string {
        auto itr = items.cbegin();
        return str::join_map(sep_pair, sep_list, [&itr, end = items.cend()]() -> std::optional<std::tuple<std::string_view, std::string_view>> {
            if (itr == end) {
                return std::nullopt;
            }

            auto& [key, val] = *(itr++);
            return std::tuple{key, val};
        });
    }

    template <typename Map, typename = typename Map::const_iterator>
    static auto join_map(const Map& items) -> std::string {
        return str::join_map("=", ",", items);
    }

    // 文件路径拼接
    static auto join_path(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_path(const Sequence& items) -> std::string {
        auto itr = items.begin();
        return str::join_path([end = items.end(), &itr]() -> std::optional<std::string_view> {
            if (itr == end) {
                return std::nullopt;
            }

            return *(itr++);
        });
    }

    // 拼接成搜索路径
    static auto join_search_path(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_search_path(const Sequence& items) -> std::string {
        auto itr = items.begin();
        return str::join_search_path([end = items.end(), &itr]() -> std::optional<std::string_view> {
            if (itr == end) {
                return std::nullopt;
            }

            return *(itr++);
        });
    }

    // 拆分字符串
    static auto split_list(std::string_view s, std::string_view sep, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split_list(std::string_view s, std::string_view sep, const view_consumer_proc& proc) -> void;
    static auto split_list(std::string_view s, std::string_view sep = ",", size_type max_n = npos) -> std::vector<std::string_view>;
    static auto split_list(std::string_view s, const std::regex& sep, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split_list(std::string_view s, const std::regex& sep, const view_consumer_proc& proc) -> void;
    static auto split_list(std::string_view s, const std::regex& sep, size_type max_n = npos) -> std::vector<std::string_view>;

    // 按空格拆分，多个空格会作为一个分隔符
    static auto split_words(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto split_words(std::string_view s, size_type max_n = str::npos) -> std::vector<std::string_view>;

    static auto split_pair(std::string_view sm, std::string_view sep = ":") -> std::tuple<std::string_view, std::string_view>;

    // 将字符串 s，按照逗号和冒号拆分成一个 map 对象
    static auto split_map(std::string_view s, std::string_view sep_list, std::string_view sep_pair, const view_pair_consumer_proc& proc) -> void;
    static auto split_map(std::string_view s, std::string_view sep_list = ",", std::string_view sep_pair = ":", size_type max_n = npos) -> std::map<std::string, std::string>;

    // 按照换行符将字符串 s，拆分长多行
    static auto split_lines(std::string_view s, bool keep_ends, const view_consumer_proc& proc) -> void;
    static auto split_lines(std::string_view s, bool keep_ends = false) -> std::vector<std::string_view>;

    // 将字符串 s 视作目录，按照路径分隔符，拆分成多个组成部分
    static auto split_path(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto split_path(std::string_view s) -> std::vector<std::string_view>;

    static auto split_search_path(std::string_view s, bool keep_empty, value_type sep, const view_consumer_proc& proc) -> void;
    static auto split_search_path(std::string_view s, bool keep_empty = false, value_type sep = ':') -> std::vector<std::string_view>;

    // // 拆分 csv 数据
    // static auto split_csv(std::string_view s) -> std::vector<std::string>;
    // static auto join_csv(view_provider_proc proc) -> std::string;
    // template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    // static auto join_csv(const Sequence& items) -> std::string {
    // }
    //
    // // 按 properties 格式拼接
    // static auto split_properties(std::string_view s) -> std::string_view ;
    // static auto join_properties(std::string& s, properties_sep sep) -> std::string&;

    // 大小写转换
    static auto to_lower(std::string_view s) -> std::string;
    static auto to_upper(std::string_view s) -> std::string;
    static auto swap_case(std::string_view s) -> std::string;

    static auto to_lower_inplace(std::string& s) -> std::string&;
    static auto to_upper_inplace(std::string& s) -> std::string&;
    static auto swap_case_inplace(std::string& s) -> std::string&;

    // 字符串化简，将字符串中的多个空白压缩成一个空格
    static auto simplified(std::string_view s, std::string_view sep, const char_checker_proc& proc) -> std::string;
    static auto simplified(std::string_view s) -> std::string;

    static auto simplified_inplace(std::string& s, std::string_view sep, const char_checker_proc& proc) -> std::string&;
    static auto simplified_inplace(std::string& s) -> std::string&;

    // 去掉字符串左侧的空白
    static auto trim_left(std::string_view s, const char_checker_proc& proc) -> std::string_view;
    static auto trim_left(std::string_view s) -> std::string_view;
    static auto trim_left(std::string_view s, charset_type charset) -> std::string_view;
    static auto trim_left(std::string_view s, std::string_view charset) -> std::string_view;
    static auto trim_right(std::string_view s, const char_checker_proc& proc) -> std::string_view;
    static auto trim_right(std::string_view s, charset_type charset) -> std::string_view;
    static auto trim_right(std::string_view s, std::string_view charset) -> std::string_view;
    static auto trim_right(std::string_view s) -> std::string_view;
    static auto trim_surrounding(std::string_view s, const char_checker_proc& proc) -> std::string_view;
    static auto trim_surrounding(std::string_view s, charset_type charset) -> std::string_view;
    static auto trim_surrounding(std::string_view s, std::string_view charset) -> std::string_view;
    static auto trim_surrounding(std::string_view s) -> std::string_view;
    static auto trim_left_margin(std::string_view s) -> std::string_view;
    static auto trim_left_margin(std::string_view s, value_type ch) -> std::string_view;

    static auto trim_left_inplace(std::string& s, const char_checker_proc& proc) -> std::string&;
    static auto trim_left_inplace(std::string& s, charset_type charset) -> std::string&;
    static auto trim_left_inplace(std::string& s, std::string_view charset) -> std::string&;
    static auto trim_left_inplace(std::string& s) -> std::string&;
    static auto trim_right_inplace(std::string& s, const char_checker_proc& proc) -> std::string&;
    static auto trim_right_inplace(std::string& s, charset_type charset) -> std::string&;
    static auto trim_right_inplace(std::string& s, std::string_view charset) -> std::string&;
    static auto trim_right_inplace(std::string& s) -> std::string&;
    static auto trim_surrounding_inplace(std::string& s, const char_checker_proc& proc) -> std::string&;
    static auto trim_surrounding_inplace(std::string& s, charset_type charset) -> std::string&;
    static auto trim_surrounding_inplace(std::string& s, std::string_view charset) -> std::string&;
    static auto trim_surrounding_inplace(std::string& s) -> std::string&;
    static auto trim_left_margin(std::string& s) -> std::string&;
    static auto trim_left_margin(std::string& s, value_type ch) -> std::string&;

    // 去掉字符串中任何位置的空白
    static auto trim_anywhere(std::string_view s, const char_checker_proc& proc) -> std::string;
    static auto trim_anywhere(std::string_view s, charset_type charset) -> std::string;
    static auto trim_anywhere(std::string_view s, std::string_view charset) -> std::string;
    static auto trim_anywhere(std::string_view s) -> std::string;

    static auto trim_anywhere_inplace(std::string& s, const char_checker_proc& proc) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s, charset_type charset) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s, std::string_view charset) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s) -> std::string&;

    // 拷贝
    static auto copy(pointer dest, size_type size, std::string_view s) -> size_type;

    // 变量展开
    using expand_vars_proc = std::function<std::optional<std::string>(const std::string& key)>;
    static auto expand_envs(std::string_view s, bool keep_unexpanded, const expand_vars_proc& proc) -> std::string;
    static auto expand_envs(std::string_view s, bool keep_unexpanded = false) -> std::string;
    static auto expand_envs(std::string_view s, bool keep_unexpanded, const std::map<std::string, std::string>& kvs) -> std::string;
    static auto expand_envs(std::string_view s, const std::map<std::string, std::string>& kvs) -> std::string;
    static auto expand_envs(std::string_view s, std::string_view key, std::string_view val) -> std::string;

    static auto expand_envs_inplace(std::string& s, bool keep_unexpanded, const expand_vars_proc& proc) -> std::string&;
    static auto expand_envs_inplace(std::string& s, bool keep_unexpanded = false) -> std::string&;
    static auto expand_envs_inplace(std::string& s, bool keep_unexpanded, const std::map<std::string, std::string>& kvs) -> std::string&;
    static auto expand_envs_inplace(std::string& s, const std::map<std::string, std::string>& kvs) -> std::string&;
    static auto expand_envs_inplace(std::string& s, std::string_view key, std::string_view val) -> std::string&;

    // 将字符串中的 tab 符号(\t)按照 tab 宽度替换成空白
    static auto expand_tabs(std::string_view s, size_type tab_size = 8) -> std::string;
    static auto expand_tabs_inplace(std::string& s, size_type tab_size = 8) -> std::string&;

    // 扩展字符串中的 ~ 前缀
    static auto expand_user(std::string_view s) -> std::string;
    static auto expand_user_inplace(std::string& s) -> std::string&;

    // 路径正常化
    static auto normpath(std::string_view s) -> std::string;
    static auto normpath_inplace(std::string& s) -> std::string&;

    // 路径处理
    static auto is_absolute(std::string_view s) -> bool;
    static auto is_relative(std::string_view s) -> bool;

    // 基本的路径操控函数
    static auto basename_ptr(std::string_view s) -> std::string::const_pointer;
    static auto extname_ptr(std::string_view s) -> std::string::const_pointer;
    static auto dirname_ptr(std::string_view s) -> std::string::const_pointer;

    // 将 s 视作为文件路径，获取其目录名
    static auto dirname_view(std::string_view s) -> std::string_view;
    static auto dirname(std::string_view s) -> std::string;
    static auto remove_dirname(std::string_view s) -> std::string;
    static auto replace_dirname(std::string_view s, std::string_view newname) -> std::string;
    static auto split_dirname(std::string_view s) -> std::tuple<std::string_view, std::string_view>;

    static auto dirname_inplace(std::string& s) -> std::string&;
    static auto remove_dirname_inplace(std::string& s) -> std::string&;
    static auto replace_dirname_inplace(std::string& s, std::string_view newname) -> std::string&;

    // 处理路径中文件名的部分
    static auto basename_view(std::string_view s) -> std::string_view;
    static auto basename(std::string_view s) -> std::string;
    static auto remove_basename(std::string_view s) -> std::string;
    static auto replace_basename(std::string_view s, std::string_view name) -> std::string;
    static auto split_basename(std::string_view s) -> std::tuple<std::string_view, std::string_view>;

    static auto basename_inplace(std::string& s) -> std::string&;
    static auto remove_basename_inplace(std::string& s) -> std::string&;
    static auto replace_basename_inplace(std::string& s, std::string_view name) -> std::string&;

    // 扩展名相关操作
    static auto extname_view(std::string_view s) -> std::string_view;
    static auto extname(std::string_view s) -> std::string;
    static auto remove_extname(std::string_view s) -> std::string;
    static auto replace_extname(std::string_view s, std::string_view name) -> std::string;
    static auto split_extname(std::string_view s) -> std::tuple<std::string_view, std::string_view>;

    static auto extname_inplace(std::string& s) -> std::string&;
    static auto remove_extname_inplace(std::string& s) -> std::string&;
    static auto replace_extname_inplace(std::string& s, std::string_view name) -> std::string&;

    // 转换为 hash 值
    static auto hash(std::string_view s, uint32_t mod) -> uint32_t;
    static auto hash(std::string_view s, uint64_t mod) -> uint64_t;
    static auto md5(std::string_view s) -> std::string;
    static auto md5(void* data, size_type n) -> std::string;

    // 转义
    static auto encode_xml(std::string_view s) -> std::string;
    static auto decode_xml(std::string_view s) -> std::string;
    static auto encode_url(std::string_view s) -> std::string;
    static auto decode_url(std::string_view s) -> std::string;

    static auto encode_xml_inplace(std::string& s) -> std::string&;
    static auto decode_xml_inplace(std::string& s) -> std::string&;
    static auto encode_url_inplace(std::string& s) -> std::string&;
    static auto decode_url_inplace(std::string& s) -> std::string&;

    // C语言字符串编解码
    static auto encode_cstr(std::string_view s, view_consumer_proc proc) -> void;
    static auto decode_cstr(std::string_view s, view_consumer_proc proc) -> void;
    static auto encode_cstr(std::string_view s) -> std::string;
    static auto decode_cstr(std::string_view s) -> std::string;

    static auto encode_cstr_inplace(std::string& s) -> std::string;
    static auto decode_cstr_inplace(std::string& s) -> std::string;

    // base64 编解码
    static auto encode_base64(std::string_view s, view_consumer_proc proc) -> void;
    static auto decode_base64(std::string_view s, view_consumer_proc proc) -> void;
    static auto encode_base64(std::string_view s) -> std::string;
    static auto decode_base64(std::string_view s) -> std::string;

    static auto encode_base64_inplace(std::string& s) -> std::string&;
    static auto decode_base64_inplace(std::string& s) -> std::string&;

    // base16  编解码
    static auto encode_base16(std::string_view s, bool upper, view_consumer_proc proc) -> void;
    static auto decode_base16(std::string_view s, view_consumer_proc proc) -> void;
    static auto encode_base16(std::string_view s, bool upper = false) -> std::string;
    static auto decode_base16(std::string_view s) -> std::string;

    static auto encode_base16_inplace(std::string& s, bool upper = false) -> std::string&;
    static auto decode_base16_inplace(std::string& s) -> std::string&;

    // 求和
    template <typename MappedType>
    using mapping_proc = std::function<auto(value_type)->MappedType>;
    template <typename T>
    static auto sum(std::string_view s, mapping_proc<T> proc) -> T {
        T result = 0;
        for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ptr++) {
            result += proc(*ptr);
        }
        return result;
    }

    // 自动统计本字符所属的字符集
    static auto chars(std::string_view s) -> charset_type;
    static auto chars(std::string_view s, charset_type& set) -> charset_type&;

    // 是否全都满足proc或者在set范围内
    static auto is_all(std::string_view s, mapping_proc<bool> proc) -> bool;
    static auto is_all(std::string_view s, charset_type set) -> bool;

    // 是否至少有一个满足proc或者在set范围内
    static auto has_any(std::string_view s, mapping_proc<bool> proc) -> bool;
    static auto has_any(std::string_view s, charset_type set) -> bool;

    // 按 proc 将字符序列分成两组，左边的满足proc，右边不满足proc
    static auto grouped(std::string_view s, mapping_proc<bool> proc) -> std::tuple<std::string, std::string>;

    // 按块切分
    static auto chunked_view(std::string_view s, size_type size) -> std::vector<std::string_view>;
    static auto chunked(std::string_view s, size_type size) -> std::vector<std::string>;

    // 读取文件内容
    static auto read_all(const std::string& filename) -> std::string;
    static auto read_all(const char* filename) -> std::string;

    static auto read_line(FILE* file, bool keeo_ends = false) -> std::string;
    static auto read_line(std::istream& file) -> std::string;

    static auto read_lines(FILE* file, const line_consumer_proc& proc) -> void;
    static auto read_lines(FILE* file, size_type max_n = npos) -> std::vector<std::string>;
    static auto read_lines(std::istream& file, const line_consumer_proc& proc) -> void;
    static auto read_lines(std::istream& file, size_type max_n = npos) -> std::vector<std::string>;

    static auto read_lines(const std::string& filename, const line_consumer_proc& proc) -> void;
    static auto read_lines(const char* filename, const line_consumer_proc& proc) -> void;
    static auto read_lines(const std::string& filename, size_type max_n = npos) -> std::vector<std::string>;
    static auto read_lines(const char* filename, size_type max_n = npos) -> std::vector<std::string>;
};

#endif // TINY_STR_H
