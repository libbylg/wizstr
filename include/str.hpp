/**
 * Copyright (c) 2021-2024 libbylg@126.com
 * tiny is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
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

    // 生产器
    using view_provider_proc = std::function<std::optional<std::string_view>()>;
    using view_pair_provider_proc = std::function<std::optional<std::tuple<std::string_view, std::string_view>>()>;
    using number_provider_proc = std::function<size_type()>;

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

    class charset_type {
    public:
        explicit charset_type() {
        }

        // 支持自动转换
        explicit charset_type(value_type ch)
            : charset_type() {
            set(ch);
        }

        // 支持自动转换
        explicit charset_type(std::string_view s)
            : charset_type() {
            for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ptr++) {
                set(*ptr);
            }
        }

        explicit charset_type(const char_match_proc& proc)
            : charset_type() {
            for (value_type ch = 0; ch < std::numeric_limits<value_type>::max(); ch++) {
                if (proc(ch)) {
                    set(ch);
                }
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

    struct range_type {
        size_type pos;
        size_type len;

        inline auto begin_pos() const -> size_type {
            return pos;
        }

        inline auto end_pos() const -> size_type {
            return pos;
        }
    };

    static inline constexpr size_type npos = std::string::npos;

#if defined(_WIN32)
    static constexpr std::string_view sep_search_path = ";";
    static constexpr value_type sep_search_path_char = ';';
    static constexpr std::string_view sep_path = "\\";
    static constexpr value_type sep_path_char = '\\';
    //
    static constexpr std::string_view sep_line_ends = "\r\n";
#else
    static constexpr std::string_view sep_search_path = ":";
    static constexpr value_type sep_search_path_char = ':';
    static constexpr std::string_view sep_path = "/";
    static constexpr value_type sep_path_char = '/';
    //
    static constexpr std::string_view sep_line_ends = "\n";
#endif

    //! 在尾部追加
    ///
    /// 提供了向指定字符尾部追加一个或者多个字符串的能力。实际上，STL 中已经提供了比较丰富的追加字符串，这里针对
    /// 大量字符串拼接提供了相对简便的方法。
    /// 对于 append_inplace 函数，如果 s 与 被插入字符串存在重叠时，函数的行为是不确定的，应该避免出现这种情况。
    ///
    /// @param s 指定向哪个字符串后添加新串。
    /// @param other 被追加的字符串。
    /// @param n 重复追加多少次，如果指定为 0，则实际不会做任何追加操作。
    /// @param proc 由 proc 函数提供被追加的字符串，如果 proc 返回 std::nullopt，表示后续无更多字符串需要追加。
    /// @param items 从容器 items 中获取被追加的字符串。 
    static auto append(std::string_view s, std::string_view other, size_type n = 1) -> std::string;
    static auto append(std::string_view s, const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto append(std::string_view s, const Sequence& items) -> std::string {
        auto itr = items.begin();
        return append(s, [&items, &itr]() -> std::optional<std::string_view> {
            if (itr == items.end()) {
                return std::nullopt;
            }

            return *(itr++);
        });
    }
    //
    static auto append_inplace(std::string& s, std::string_view other, size_type n) -> std::string&;
    static auto append_inplace(std::string& s, const view_provider_proc& proc) -> std::string&;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto append_inplace(std::string& s, const Sequence& items) -> std::string& {
        auto itr = items.begin();
        return append(s, [&items, &itr]() -> std::optional<std::string_view> {
            if (itr == items.end()) {
                return std::nullopt;
            }

            return *(itr++);
        });
    }
    //$

    //! 向头部追加
    ///
    /// 將一个或者多个字符串追加到指定字符串的前面。实际上，STL 中已经提供了比较丰富的字符串插入函数，这里针对
    /// 大量字符串拼接提供了相对简便的方法。需要注意，对于通过 proc 和 items 来提供被追加串的函数，字符串总是以倒序的方式被追加。
    /// 比如，`str::prepend("abc", {"123", "456", "789"})` 返回的结果是 "789456123abc"。
    /// 对于 prepend_inplace 函数，如果 s 与 被插入字符串存在重叠时，函数的行为是不确定的，应该避免出现这种情况。
    ///
    /// @param s 所有字符串都追加到该字符串之前
    /// @param other 被追加的字符串
    /// @param n 重复追加多少次，如果指定为 0，则实际不会做任何追加操作。
    /// @param proc 由 proc 函数提供被追加的字符串，如果 proc 返回 std::nullopt，表示后续无更多字符串需要追加。
    /// @param items 从容器 items 中获取被追加的字符串。
    static auto prepend(std::string_view s, std::string_view other, size_type n) -> std::string;
    static auto prepend(std::string_view s, const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto prepend(std::string_view s, const Sequence& items) -> std::string {
        auto itr = items.begin();
        return prepend(s, [&items, &itr]() -> std::optional<std::string_view> {
            if (itr == items.end()) {
                return std::nullopt;
            }

            return *(itr++);
        });
    }
    //
    static auto prepend_inplace(std::string& s, std::string_view other, size_type n) -> std::string&;
    static auto prepend_inplace(std::string& s, const view_provider_proc& proc) -> std::string&;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto prepend_inplace(std::string& s, const Sequence& items) -> std::string& {
        auto itr = items.begin();
        return prepend_inplace(s, [&items, &itr]() -> std::optional<std::string_view> {
            if (itr == items.end()) {
                return std::nullopt;
            }

            return *(itr++);
        });
    }
    //$

    //! 向字符串中间插入
    ///
    /// 向字符串指定的位置插入新的字符串或者字符。
    /// 对于 insert_inplace 函数，如果 s 与 被插入字符串存在重叠时，函数的行为是不确定的，应该避免出现这种情况。
    ///
    /// @param s 向该字符串查询新串
    /// @param pos 指定插入的起始位置，如果 pos 大于或者等于 s 的长度，等价于 append。
    /// @param other 被插入的字符串，如果 other 与 s 存在重叠，可能会导致插入的数据达不到预期。 
    /// @param ch 被插入的字符
    /// @param n 重复插入次数
    /// @param proc 由 proc 函数提供被追加的字符串，如果 proc 返回 std::nullopt，表示后续无更多字符串需要追加。
    /// @param items 从容器 items 中获取被插入的字符串。
    static auto insert(std::string_view s, size_type pos, std::string_view other, size_type n = 1) -> std::string;
    static auto insert(std::string_view s, size_type pos, value_type ch, size_type n = 1) -> std::string;
    static auto insert(std::string_view s, size_type pos, const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto insert(std::string& s, size_type pos, const Sequence& items) -> std::string& {
        auto itr = items.begin();
        return insert(s, pos, [&items, &itr]() -> std::optional<std::string_view> {
            if (itr == items.end()) {
                return std::nullopt;
            }

            return *(itr++);
        });
    }
    //
    static auto insert_inplace(std::string& s, size_type pos, std::string_view other, size_type n = 1) -> std::string&;
    static auto insert_inplace(std::string& s, size_type pos, value_type ch, size_type n = 1) -> std::string&;
    static auto insert_inplace(std::string& s, size_type pos, const view_provider_proc& proc) -> std::string&;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto insert_inplace(std::string& s, size_type pos, const Sequence& items) -> std::string& {
        auto itr = items.begin();
        return insert_inplace(s, pos, [&items, &itr]() -> std::optional<std::string_view> {
            if (itr == items.end()) {
                return std::nullopt;
            }

            return *(itr++);
        });
    }
    //$

    //! 不区分大小写的比较
    ///
    /// icompare 提供了不区分大小写比较的能力，其中 max_n 用于限制最多比较字符数量。特别的，如果 max_n 等于 0，icompare 总是
    /// 返回 0；
    ///
    /// @param s 参与比较的字符串
    /// @param other 另一个参与比较的字符串
    /// @param max_n 表示最多比较前 max_n 个字符
    /// @return 返回正数，表示 s 大于 other；返回负值，表示 s 小于 other；返回 0，表示 s 和 other 相等。
    static auto icompare(std::string_view s, std::string_view other) -> int;
    static auto icompare(std::string_view s, std::string_view other, size_type max_n) -> int;
    //$

    //! 不区分大小写的相等測試
    ///
    /// iequals 提供了不区分大小写的相等比较，，其中 max_n 用于限制最多比较字符数量。特别的，如果 max_n 等于 0，icompare 总是
    /// 返回 true；
    ///
    /// @param s 参与比较的字符串
    /// @param other 另一个参与比较的字符串
    /// @param max_n 表示最多比较前 max_n 个字符
    /// @return 如果相等，返回 true，否则返回 false 
    static auto iequals(std::string_view s, std::string_view other) -> bool;
    static auto iequals(std::string_view s, std::string_view other, size_type max_n) -> bool;
    //$

    //! 基于通配符的匹配检测
    ///
    /// 测试字符串 s 是否匹配通配符 pattern。
    /// 
    /// @param s 被测试的字符串
    /// @param pattern 通配串
    /// @return 如果 s 字符串匹配 pattern，返回 true，否则返回 false。
    static auto wildcmp(const_pointer s, const_pointer pattern) -> bool;
    static auto wildcmp(std::string_view s, std::string_view pattern) -> bool;
    //$

    //! 判断两个字符串的包含关系
    ///
    /// 等价于在字符串 s 中查找是否存在指定的字符或者字符串。
    ///
    /// @param s 在该字符串查找目标子串
    /// @param other 被查找的目标子串
    /// @param ch 在 s 中查找是否存在指定的字符
    /// @param proc s 中的每个字符都会触发 proc 函数，proc 返回 true，表示当前字符是正在被查找的字符；
    /// @param ignore_case 指定是否采用不区分大小写的方式来查找子串
    /// @param charset 指定一个字符集，s 中只要有任意一个字符在 charset 中就表示 s 中包含 charset
    /// @param pattern 指定一个正则表达式，只要 s 中有任意子串匹配 pattern，表示 s 中包含 pattern
    /// @return 如果 s 包含指定的字符或者字符串或者某种模式，返回 true，否则返回 false。
    static auto contains(std::string_view s, std::string_view other, bool ignore_case = false) -> bool;
    static auto contains(std::string_view s, value_type ch, bool ignore_case = false) -> bool;
    static auto contains(std::string_view s, const char_match_proc& proc) -> bool;
    static auto contains(std::string_view s, const charset_type& charset) -> bool;
    static auto contains(std::string_view s, const std::regex& pattern) -> bool;

    //! 子串统计
    /// 
    /// 本函数用于统计 s 串中是否包含特定模式的子串的数量。需要注意，count 函数统计的子串是不重叠的子串。
    ///
    /// @param s 
    static auto count(std::string_view s, std::string_view other, bool ignore_case = false) -> size_type;
    static auto count(std::string_view s, value_type ch, bool ignore_case = false) -> size_type;
    static auto count(std::string_view s, const char_match_proc& proc) -> size_type;
    static auto count(std::string_view s, const charset_type& charset) -> size_type;
    static auto count(std::string_view s, const std::regex& pattern) -> size_type;

    //! 计算公共前缀和后缀的长度
    ///
    /// @param s 参与计算的字符串
    /// @param other 参与计算的另一个字符串
    /// @return 如果 s 与 other 有共同前缀，返回共同前缀的长度，否则返回 0
    static auto prefix(std::string_view s, std::string_view other) -> size_type;
    static auto suffix(std::string_view s, std::string_view other) -> size_type;

    //! 前缀操作
    ///
    /// 本组函数提供了常见的前缀操作。has_prefix 和 starts_with 功能一致，都用于测试字符串 s 是否有指定的前缀。
    /// remove_prefix_view，remove_prefix，remove_prefix_inplace 会返回从字符串 s 中去除两个字符串的共同前缀后剩余的部分。
    /// 
    /// @param s 目标字符串
    /// @param ch 在 has_prefix 和 starts_with 中表示用于判断 s 的首字母是否为 ch。
    /// @param prefix 前缀串
    static auto has_prefix(std::string_view s, value_type ch) -> bool;
    static auto has_prefix(std::string_view s, std::string_view prefix) -> bool;
    static auto starts_with(std::string_view s, value_type ch) -> bool;
    static auto starts_with(std::string_view s, std::string_view prefix) -> bool;
    static auto remove_prefix_view(std::string_view s, std::string_view prefix) -> std::string_view;
    static auto remove_prefix_view(std::string_view s, value_type prefix) -> std::string_view;
    static auto remove_prefix(std::string_view s, std::string_view prefix) -> std::string;
    static auto remove_prefix(std::string_view s, value_type prefix) -> std::string;
    //
    static auto remove_prefix_inplace(std::string& s, std::string_view prefix) -> std::string&;
    static auto remove_prefix_inplace(std::string& s, value_type prefix) -> std::string&;

    // 后缀操作
    static auto has_suffix(std::string_view s, value_type suffix) -> bool;
    static auto has_suffix(std::string_view s, std::string_view suffix) -> bool;
    static auto ends_with(std::string_view s, value_type suffix) -> bool;
    static auto ends_with(std::string_view s, std::string_view suffix) -> bool;
    static auto remove_suffix_view(std::string_view s, std::string_view suffix) -> std::string_view;
    static auto remove_suffix_view(std::string_view s, value_type suffix) -> std::string_view;
    static auto remove_suffix(std::string_view s, std::string_view suffix) -> std::string;
    static auto remove_suffix(std::string_view s, value_type suffix) -> std::string;
    //
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

    // 提取子串（基于位置系列）
    static auto take_left_view(std::string_view s, size_type n) -> std::string_view;
    static auto take_right_view(std::string_view s, size_type n) -> std::string_view;
    static auto take_mid_view(std::string_view s, size_type pos, size_type n) -> std::string_view;
    static auto take_range_view(std::string_view s, size_type begin_pos, size_type end_pos) -> std::string_view;
    static auto take_view(std::string_view s, size_type pos, ssize_type offset) -> std::string_view;
    static auto take_view(std::string_view s, size_type pos) -> std::string_view;
    //
    static auto take_left(std::string_view s, size_type n) -> std::string;
    static auto take_right(std::string_view s, size_type n) -> std::string;
    static auto take_mid(std::string_view s, size_type pos, size_type n) -> std::string;
    static auto take_range(std::string_view s, size_type begin_pos, size_type end_pos) -> std::string;
    static auto take(std::string_view s, size_type pos, ssize_type offset) -> std::string;
    static auto take(std::string_view s, size_type pos) -> std::string;
    //
    static auto take_left_inplace(std::string& s, size_type n) -> std::string&;
    static auto take_right_inplace(std::string& s, size_type n) -> std::string&;
    static auto take_mid_inplace(std::string& s, size_type pos, size_type n) -> std::string&;
    static auto take_range_inplace(std::string& s, size_type begin_pos, size_type end_pos) -> std::string&;
    static auto take_inplace(std::string& s, size_type pos, ssize_type offset) -> std::string&;
    static auto take_inplace(std::string& s, size_type pos) -> std::string&;

    // 删除串中的子串
    static auto drop_left_view(std::string_view s, size_type n) -> std::string_view;
    static auto drop_right_view(std::string_view s, size_type n) -> std::string_view;
    static auto drop_left(std::string_view s, size_type n) -> std::string;
    static auto drop_right(std::string_view s, size_type n) -> std::string;
    static auto drop_mid(std::string_view s, size_type pos, size_type n) -> std::string;
    static auto drop_range(std::string_view s, size_type begin_pos, size_type end_pos) -> std::string;
    static auto drop(std::string_view s, size_type pos, ssize_type offset) -> std::string;
    static auto drop(std::string_view s, size_type pos) -> std::string;
    static auto drop(std::string_view s, char_checker_proc proc) -> std::string;
    static auto drop(std::string_view s, charset_type charset) -> std::string;
    //
    static auto drop_left_inplace(std::string& s, size_type n) -> std::string&;
    static auto drop_right_inplace(std::string& s, size_type n) -> std::string&;
    static auto drop_mid_inplace(std::string& s, size_type pos, size_type n) -> std::string&;
    static auto drop_range_inplace(std::string& s, size_type begin_pos, size_type end_pos) -> std::string&;
    static auto drop_inplace(std::string& s, size_type pos, ssize_type offset) -> std::string&;
    static auto drop_inplace(std::string& s, size_type pos) -> std::string&;
    static auto drop_inplace(std::string& s, char_checker_proc proc) -> std::string&;
    static auto drop_inplace(std::string& s, charset_type charset) -> std::string&;

    // 提取子串（基于分隔符系列）
    static auto range_first(std::string_view s, const char_match_proc& proc) -> range_type;
    static auto range_first(std::string_view s, value_type sep_ch) -> range_type;
    static auto range_first(std::string_view s, const charset_type& sep_charset) -> range_type;
    static auto range_first(std::string_view s, std::string_view sep_str) -> range_type;
    static auto range_first(std::string_view s, std::regex sep_regex) -> range_type;
    //
    static auto range_last(std::string_view s, const char_match_proc& proc) -> range_type;
    static auto range_last(std::string_view s, value_type sep_ch) -> range_type;
    static auto range_last(std::string_view s, const charset_type& sep_charset) -> range_type;
    static auto range_last(std::string_view s, std::string_view sep_str) -> range_type;
    static auto range_last(std::string_view s, std::regex sep_regex) -> range_type;
    //
    static auto take_before_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto take_after_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto drop_before_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto drop_after_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    //
    static auto take_before(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;
    static auto take_after(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;
    static auto drop_before(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;
    static auto drop_after(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;

    // 对齐
    static auto align_left(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_right(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_center(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_zfill(std::string_view s, size_type width) -> std::string;
    //
    static auto align_left_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_right_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_center_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_zfill_inplace(std::string& s, size_type width) -> std::string&;

    // 多行字符串处理
    static auto foreach_line(std::string_view s, const line_consumer_proc& proc) -> void;
    static auto count_lines(std::string_view s) -> size_type;
    static auto lines_indentation(std::string_view s) -> size_type;
    //
    static auto numbering_lines(std::string_view s, size_type from_n, std::string_view num_format = "") -> std::string;
    static auto unnumbering_lines(std::string_view s, size_type from_n, std::string_view num_format = "") -> std::string;
    static auto indent_lines(std::string_view s, size_type n, std::string_view pad = " ") -> std::string;
    static auto dedent_lines(std::string_view s, size_type n, std::string_view pad = " ") -> std::string;
    static auto unindent_lines(std::string_view s, size_type n, std::string_view pad = " ") -> std::string;
    static auto trim_lines_indent(std::string_view s) -> std::string;
    static auto trim_lines_margin(std::string_view s, std::string_view margin = "") -> std::string;
    //
    static auto numbering_lines_inplace(std::string& s, size_type from_n, std::string_view num_format = "") -> std::string&;
    static auto unnumbering_lines_inplace(std::string& s, size_type from_n, std::string_view num_format = "") -> std::string&;
    static auto indent_lines_inplace(std::string& s, size_type n, std::string_view pad = " ") -> std::string&;
    static auto dedent_lines_inplace(std::string& s, size_type n, std::string_view pad = " ") -> std::string&;
    static auto unindent_lines_nplacet(std::string& s, size_type n, std::string_view pad = " ") -> std::string&;
    static auto trim_lines_indent_inplace(std::string& s) -> std::string&;
    static auto trim_lines_margin_inplace(std::string& s, std::string_view margin = "") -> std::string&;

    // 按单词统计
    static auto foreach_word(std::string_view s, size_type pos, const std::function<int(size_type pos, size_type n)>& proc) -> void;
    static auto foreach_word(std::string_view s, const std::function<int(size_type pos, size_type n)>& proc) -> void;
    static auto foreach_word(std::string_view s, size_type pos, const std::function<int(std::string_view word)>& proc) -> void;
    static auto foreach_word(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto count_words(std::string_view s, const char_match_proc& proc) -> size_type;
    static auto count_words(std::string_view s, charset_type sepset) -> size_type;
    static auto count_words(std::string_view s, value_type sepch) -> size_type;
    static auto count_words(std::string_view s) -> size_type;

    // 用指定的模式串环绕字符串
    static auto surround(std::string_view s, std::string_view mode = "#(#)") -> std::string;
    static auto surround_inplace(std::string_view s, std::string_view mode = "#(#)") -> std::string;

    static auto unsurround(std::string_view s, std::string_view mode = "#(#)") -> std::string;
    static auto unsurround_inplace(std::string_view s, std::string_view mode = "#(#)") -> std::string;

    // 反转：字符串逆序
    static auto invert(std::string_view s, size_type pos = 0, size_type max_n = npos) -> std::string;

    static auto invert_inplace(std::string& s, size_type pos, size_type max_n) -> std::string&;

    // 生成字符串 s 或者 字符 ch 的内容重复出现 times 次后的新字符串
    static auto repeat(std::string_view s, size_type times) -> std::string;
    static auto repeat(value_type ch, size_type times) -> std::string;

    // 随机生成字符串
    static auto random(size_type max_len, size_type min_len, const number_provider_proc& proc) -> std::string;
    static auto random(size_type max_len, size_type min_len, value_type ch, const number_provider_proc& proc) -> std::string;
    static auto random(size_type max_len, size_type min_len, std::string_view charset, const number_provider_proc& proc) -> std::string;
    static auto random(size_type max_len, size_type min_len, charset_type charset, const number_provider_proc& proc) -> std::string;
    static auto random(size_type max_len, const number_provider_proc& proc) -> std::string;
    static auto random(size_type max_len, value_type ch, const number_provider_proc& proc) -> std::string;
    static auto random(size_type max_len, std::string_view charset, const number_provider_proc& proc) -> std::string;
    static auto random(size_type max_len, charset_type charset, const number_provider_proc& proc) -> std::string;

    // 随机填充指定的字符串（不改变大小）
    static auto random_fill(std::string& s, std::string_view charset, const number_provider_proc& proc) -> std::string&;
    static auto random_fill(std::string& s, charset_type charset, const number_provider_proc& proc) -> std::string&;
    static auto random_fill(std::string& s, const number_provider_proc& proc) -> std::string&;

    // 现有字符串随机重排（不改变大小）
    static auto random_reorder(std::string& s, const number_provider_proc& proc) -> std::string&;

    // 生成由 width 个空格组成的新字符串
    static auto spaces(size_type width) -> std::string;
    static auto skip_space_view(std::string_view s) -> std::string_view;
    static auto skip_space(std::string_view s) -> std::string;
    static auto skip_space_view(std::string_view s, size_type& pos) -> std::string_view;
    static auto skip_space(std::string_view s, size_type& pos) -> std::string;
    static auto skip_space_inplace(std::string& s, size_type pos) -> std::string&;

    // 遮罩
    static auto cover_left(std::string_view s, std::string_view mask = "***", size_type show_size = 3) -> std::string;
    static auto cover_right(std::string_view s, std::string_view mask = "***", size_type show_size = 3) -> std::string;
    static auto cover_surrounding(std::string_view s, std::string_view mask = "***", size_type show_size = 3) -> std::string;
    static auto cover_auto(std::string_view s, std::string_view mask = "***", size_type show_size = 3) -> std::string;
    static auto cover_left(std::string_view s, std::string_view mask = "***", size_type show_size = 3, size_type fixed_size = 10) -> std::string;
    static auto cover_right(std::string_view s, std::string_view mask = "***", size_type show_size = 3, size_type fixed_size = 10) -> std::string;
    static auto cover_surrounding(std::string_view s, std::string_view mask = "***", size_type show_size = 3, size_type fixed_size = 10) -> std::string;
    static auto cover_auto(std::string_view s, std::string_view mask = "***", size_type show_size = 3, size_type fixed_size = 10) -> std::string;

    static auto cover_left_inplace(std::string& s, std::string_view mask = "***", size_type show_size = 3) -> std::string&;
    static auto cover_right_inplace(std::string& s, std::string_view mask = "***", size_type show_size = 3) -> std::string&;
    static auto cover_surrounding_inplace(std::string& s, std::string_view mask = "***", size_type show_size = 3) -> std::string&;
    static auto cover_auto_inplace(std::string& s, std::string_view mask = "***", size_type show_size = 3) -> std::string&;
    static auto cover_left_inplace(std::string& s, std::string_view mask = "***", size_type show_size = 3, size_type fixed_size = 10) -> std::string&;
    static auto cover_right_inplace(std::string& s, std::string_view mask = "***", size_type show_size = 3, size_type fixed_size = 10) -> std::string&;
    static auto cover_surrounding_inplace(std::string& s, std::string_view mask = "***", size_type show_size = 3, size_type fixed_size = 10) -> std::string&;
    static auto cover_auto_inplace(std::string& s, std::string_view mask = "***", size_type show_size = 3, size_type fixed_size = 10) -> std::string&;

    // 用指定的分隔符,拼接一个字符串序列
    static auto join(std::string_view s, const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join(std::string_view s, const Sequence& items) -> std::string {
        std::string result;
        auto itr = items.begin();
        return join(s, [&items, &itr]() -> std::optional<std::string_view> {
            if (itr == items.end()) {
                return std::nullopt;
            }

            return *(itr++);
        });
    }

    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join(const Sequence& items) -> std::string {
        return join("", items);
    }

    static auto join_list(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_list(const Sequence& items) -> std::string {
        return join(",", items);
    }

    //! 使用逗号和冒号拼接 map
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

    //! 按行拼接
    /// @param sep 
    /// @param proc 
    /// @param items 
    static auto join_lines(std::string_view sep, const view_provider_proc& proc) -> std::string;
    static auto join_lines(const view_provider_proc& proc) -> std::string;
    //
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_lines(std::string_view sep, const Sequence& items) -> std::string {
        auto itr = items.cbegin();
        return join_lines([&itr, &items]() -> std::optional<std::string_view> {
            if (itr == items.cend()) {
                return std::nullopt;
            }

            return *(itr++);
        });
    }
    //
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_lines(const Sequence& items) -> std::string {
        return join_lines(sep_line_ends, items);
    }

    //! 拼接文件路径
    static auto join_path(std::string_view sep, const view_provider_proc& proc) -> std::string;
    static auto join_path(const view_provider_proc& proc) -> std::string;
    //
    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_path(std::string_view sep, const Sequence& items) -> std::string {
        auto itr = items.begin();
        return str::join_path(sep, [end = items.end(), &itr]() -> std::optional<std::string_view> {
            if (itr == end) {
                return std::nullopt;
            }

            return *(itr++);
        });
    }
    //
    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_path(const Sequence& items) -> std::string {
        return str::join_path(sep_path, items);
    }

    //! 拼接搜索路径
    ///
    /// 使用搜索路径分隔符拼接由 proc 或者 items 供给的字符串，并返回拼接后的结果。
    /// 路径分隔符可以由 sep 手工指定，当调用没有该参数的形式的函数时，自动使用系统默认的分隔符（参见 @ref sep_search_path）
    /// 对于提供 proc 参数的接口，proc 会持续调用该哈数获得数据直到该函数返回 std::optnull。如果 proc 在第一次调用时就返回 
    /// std::optnull，返回的搜索路径为空串。
    /// 
    /// @param sep 搜索路径分隔符，需要注意在不同操作系统下，搜索路径的分隔符是不同的。
    /// @param proc 提供搜素路径片段的函数。
    /// @param items 存放路径片段的容器。
    /// @return 返回以当前系统的搜索路径分隔符拼接好的字符串。
    static auto join_search_path(std::string_view sep, const view_provider_proc& proc) -> std::string;
    static auto join_search_path(const view_provider_proc& proc) -> std::string;

    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_search_path(std::string_view sep, const Sequence& items) -> std::string {
        auto itr = items.begin();
        return str::join_search_path(sep, [end = items.end(), &itr]() -> std::optional<std::string_view> {
            if (itr == end) {
                return std::nullopt;
            }

            return *(itr++);
        });
    }

    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_search_path(const Sequence& items) -> std::string {
        return join_search_path(":", items);
    }
    //$

    //! 以单个字符作为分隔符拆分字符串
    ///
    /// @param s 被拆分的字符串。
    /// @param sepset 分隔符集合，可以有多种形式组成。
    /// @param max_n 最多拆分多少次。如果为 0 表示不做任何拆分，返回原始字符串。如果为 npos 表示不限制拆分次数。
    /// @param proc 指定如何接受拆分出来的字符串。
    /// @return 当未指定 proc 参数时，会返回字符串列表。
    static auto split(std::string_view s, const char_match_proc& sepset, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const charset_type& sepset, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const charset_type& sepset, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const charset_type& sepset, size_type max_n = npos) -> std::vector<std::string>;
    static auto split(std::string_view s, std::string_view sepset, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, std::string_view sepset, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, std::string_view sepset, size_type max_n = npos) -> std::vector<std::string>;
    static auto split(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, size_type max_n = str::npos) -> std::vector<std::string>;
    static auto split_view(std::string_view s, const charset_type& sepset, size_type max_n = npos) -> std::vector<std::string_view>;
    static auto split_view(std::string_view s, std::string_view sepset, size_type max_n = npos) -> std::vector<std::string_view>;
    static auto split_view(std::string_view s, size_type max_n = str::npos) -> std::vector<std::string_view>;
    //$

    //! 以字符串为分隔符拆分字符串
    ///
    /// @param s 被拆分的字符串。
    /// @param sepstr 用作分隔符的字符串，可以有多种形式组成。如果为 std::string_view 类型时，如果 sepstr 为空，自动采用默认值 ","。
    /// @param max_n 最多拆分多少次。如果为 0 表示不做任何拆分，返回原始字符串。如果为 npos 表示不限制拆分次数。
    /// @param proc 指定如何接受拆分出来的字符串。
    /// @return 当未指定 proc 参数时，会返回字符串列表。
    static auto split_list(std::string_view s, std::string_view sepstr, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split_list(std::string_view s, std::string_view sepstr, const view_consumer_proc& proc) -> void;
    static auto split_list(std::string_view s, std::string_view sepstr = ",", size_type max_n = npos) -> std::vector<std::string>;
    static auto split_list(std::string_view s, const std::regex& sepstr, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split_list(std::string_view s, const std::regex& sepstr, const view_consumer_proc& proc) -> void;
    static auto split_list(std::string_view s, const std::regex& sepstr, size_type max_n = npos) -> std::vector<std::string>;
    static auto split_list_view(std::string_view s, const std::regex& sepstr, size_type max_n = npos) -> std::vector<std::string_view>;
    static auto split_list_view(std::string_view s, std::string_view sepstr = ",", size_type max_n = npos) -> std::vector<std::string_view>;
    //$

    //! 以指定的字符串为分隔符将字符串拆分为两个部分
    ///
    /// split_list 在 max_n 等于 1 时的功能。
    ///
    /// @param s 被拆分的字符串。
    /// @param sepstr 用作分隔符的字符串。
    /// @return 返回被拆分处理的字符串。如果字符串中未找到分隔符，整个字符串作为返回值的第一个字符串，而第二个字符串为空。
    static auto split_pair(std::string_view s, std::string_view sepstr = ":") -> std::tuple<std::string, std::string>;
    static auto split_pair_view(std::string_view s, std::string_view sepstr = ":") -> std::tuple<std::string_view, std::string_view>;

    //! 将字符串 s，按照逗号和冒号拆分成一个 map 对象
    ///
    /// split_map 会对字符串做两轮拆分，第一轮先以 sep_list 为分隔符，将字符串拆分成一组字串。第二轮再以 sep_pair 为分隔符
    /// 将前一轮拆分出来的每个字串拆分成键值对，并将该该键值对存入 map 或者通过 proc 输出。
    /// 总之，split_map 是拆分的是类型下面的数据格式（以sep_list和sep_pair为缺省值时为例）：
    ///
    ///     item1:value1,item2:value2,item3:value3 ...
    ///
    /// @param s 被拆分的字符串。
    /// @param sep_list 用作第一轮拆分的分隔符
    /// @param sep_pair 用作第二轮拆分的分隔符
    /// @param max_n 最多拆分多少次。max_n 主要用于控制第一轮拆分的次数，如果指定为 0 将返回空 map 或者不触发 proc。
    ///              当次数达到后，后续的数据会被舍弃，且不会被放入 map 中，也不会再触发 proc。
    ///              由于调用方无法感知是否有剩余数据未拆分完，因此，max_n 通常只用在舍弃剩余字符串是无关紧要的情况下。
    /// @param proc 输出拆分出来的每个键值对
    /// @return 返回组合成的 map，对于返回值为 void 的形式，数据通过 proc 返回。
    static auto split_map(std::string_view s, std::string_view sep_list, std::string_view sep_pair, const view_pair_consumer_proc& proc) -> void;
    static auto split_map(std::string_view s, std::string_view sep_list = ",", std::string_view sep_pair = ":", size_type max_n = npos) -> std::map<std::string, std::string>;

    //! 按照换行符将字符串 s，拆分成多行
    /// @param keep_ends 是否保留行尾分隔符
    ///
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

    // 分片
    static auto partition_view(std::string_view s, charset_type sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto partition_view(std::string_view s, value_type sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto partition_view(std::string_view s, const char_mapping_proc& proc) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto partition_view(std::string_view s, std::string_view sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto partition_view(std::string_view s, const range_search_proc& proc) -> std::tuple<std::string_view, std::string_view, std::string_view>;

    static auto partition(std::string_view s, charset_type sep) -> std::tuple<std::string, std::string, std::string>;
    static auto partition(std::string_view s, value_type sep) -> std::tuple<std::string, std::string, std::string>;
    static auto partition(std::string_view s, const char_mapping_proc& proc) -> std::tuple<std::string, std::string, std::string>;
    static auto partition(std::string_view s, std::string_view sep) -> std::tuple<std::string, std::string, std::string>;
    static auto partition(std::string_view s, const range_search_proc& proc) -> std::tuple<std::string, std::string, std::string>;

    static auto rpartition_view(std::string_view s, charset_type sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto rpartition_view(std::string_view s, value_type sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto rpartition_view(std::string_view s, const char_mapping_proc& proc) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto rpartition_view(std::string_view s, std::string_view sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto rpartition_view(std::string_view s, const range_search_proc& proc) -> std::tuple<std::string_view, std::string_view, std::string_view>;

    static auto rpartition(std::string_view s, charset_type sep) -> std::tuple<std::string, std::string, std::string>;
    static auto rpartition(std::string_view s, value_type sep) -> std::tuple<std::string, std::string, std::string>;
    static auto rpartition(std::string_view s, const char_mapping_proc& proc) -> std::tuple<std::string, std::string, std::string>;
    static auto rpartition(std::string_view s, std::string_view sep) -> std::tuple<std::string, std::string, std::string>;
    static auto rpartition(std::string_view s, const range_search_proc& proc) -> std::tuple<std::string, std::string, std::string>;

    // 指定宽度拆分字符串
    static auto chunked(std::string_view s, size_type width, const view_consumer_proc& proc) -> void;
    static auto chunked(std::string_view s, size_type width) -> std::vector<std::string>;
    static auto chunked_view(std::string_view s, size_type width) -> std::vector<std::string_view>;

    // 基于窗口拆分字符串
    static auto windowed(std::string_view s, size_type width, size_type step, const view_consumer_proc& proc) -> void;
    static auto windowed_view(std::string_view s, size_type width, size_type step) -> std::vector<std::string_view>;
    static auto windowed(std::string_view s, size_type width, size_type step) -> std::vector<std::string>;

    // 大小写转换
    static auto to_lower(std::string_view s) -> std::string;
    static auto to_upper(std::string_view s) -> std::string;
    static auto to_title(std::string_view s) -> std::string;
    static auto to_capitalize(std::string_view s) -> std::string;
    static auto swap_case(std::string_view s) -> std::string;

    static auto to_lower_inplace(std::string& s) -> std::string&;
    static auto to_upper_inplace(std::string& s) -> std::string&;
    static auto to_title_inplace(std::string& s) -> std::string&;
    static auto to_capitalize_inplace(std::string& s) -> std::string&;
    static auto swap_case_inplace(std::string& s) -> std::string&;

    // 字符串化简，将字符串中的多个空白压缩成一个空格
    static auto simplified(std::string_view s, std::string_view sep, const char_checker_proc& proc) -> std::string;
    static auto simplified(std::string_view s) -> std::string;

    static auto simplified_inplace(std::string& s, std::string_view sep, const char_checker_proc& proc) -> std::string&;
    static auto simplified_inplace(std::string& s) -> std::string&;

    // 去掉字符串左侧的空白
    static auto trim_left_view(std::string_view s, const char_checker_proc& proc) -> std::string_view;
    static auto trim_left_view(std::string_view s) -> std::string_view;
    static auto trim_left_view(std::string_view s, const charset_type& charset) -> std::string_view;
    static auto trim_left_view(std::string_view s, std::string_view charset) -> std::string_view;
    static auto trim_right_view(std::string_view s, const char_checker_proc& proc) -> std::string_view;
    static auto trim_right_view(std::string_view s, const charset_type& charset) -> std::string_view;
    static auto trim_right_view(std::string_view s, std::string_view charset) -> std::string_view;
    static auto trim_right_view(std::string_view s) -> std::string_view;
    static auto trim_surrounding_view(std::string_view s, const char_checker_proc& proc) -> std::string_view;
    static auto trim_surrounding_view(std::string_view s, const charset_type& charset) -> std::string_view;
    static auto trim_surrounding_view(std::string_view s, std::string_view charset) -> std::string_view;
    static auto trim_surrounding_view(std::string_view s) -> std::string_view;
    //
    static auto trim_left(std::string_view s, const char_checker_proc& proc) -> std::string;
    static auto trim_left(std::string_view s) -> std::string;
    static auto trim_left(std::string_view s, charset_type charset) -> std::string;
    static auto trim_left(std::string_view s, std::string_view charset) -> std::string;
    static auto trim_right(std::string_view s, const char_checker_proc& proc) -> std::string;
    static auto trim_right(std::string_view s, charset_type charset) -> std::string;
    static auto trim_right(std::string_view s, std::string_view charset) -> std::string;
    static auto trim_right(std::string_view s) -> std::string;
    static auto trim_surrounding(std::string_view s, const char_checker_proc& proc) -> std::string;
    static auto trim_surrounding(std::string_view s, charset_type charset) -> std::string;
    static auto trim_surrounding(std::string_view s, std::string_view charset) -> std::string;
    static auto trim_surrounding(std::string_view s) -> std::string;
    //
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
    static auto trim_anywhere(std::string_view s, const charset_type& charset) -> std::string;
    static auto trim_anywhere(std::string_view s, std::string_view charset) -> std::string;
    static auto trim_anywhere(std::string_view s, value_type charset) -> std::string;
    static auto trim_anywhere(std::string_view s) -> std::string;

    static auto trim_anywhere_inplace(std::string& s, const char_checker_proc& proc) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s, const charset_type& charset) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s, std::string_view charset) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s, value_type charset) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s) -> std::string&;

    //! 数字化简
    static auto simplified_integer_view(std::string_view s) -> std::string_view;
    static auto simplified_decimal_view(std::string_view s) -> std::string_view;
    //
    static auto simplified_integer(std::string_view s) -> std::string;
    static auto simplified_decimal(std::string_view s) -> std::string;
    //
    static auto simplified_integer_inplace(std::string& s) -> std::string&;
    static auto simplified_decimal_inplace(std::string& s) -> std::string&;
    //$

    // 拷贝
    static auto copy(pointer dest, size_type size, std::string_view s) -> size_type;

    //! 变量展开
    using expand_vars_proc = std::function<std::optional<std::string>(const std::string& key)>;
    static auto expand_envs(std::string_view s, bool keep_unexpanded, const expand_vars_proc& proc) -> std::string;
    static auto expand_envs(std::string_view s, bool keep_unexpanded = false) -> std::string;
    static auto expand_envs(std::string_view s, bool keep_unexpanded, const std::map<std::string, std::string>& kvs) -> std::string;
    static auto expand_envs(std::string_view s, const std::map<std::string, std::string>& kvs) -> std::string;
    static auto expand_envs(std::string_view s, std::string_view key, std::string_view val) -> std::string;
    //
    static auto expand_envs_inplace(std::string& s, bool keep_unexpanded, const expand_vars_proc& proc) -> std::string&;
    static auto expand_envs_inplace(std::string& s, bool keep_unexpanded = false) -> std::string&;
    static auto expand_envs_inplace(std::string& s, bool keep_unexpanded, const std::map<std::string, std::string>& kvs) -> std::string&;
    static auto expand_envs_inplace(std::string& s, const std::map<std::string, std::string>& kvs) -> std::string&;
    static auto expand_envs_inplace(std::string& s, std::string_view key, std::string_view val) -> std::string&;

    //! 将字符串中的 tab 符号(\t)按照 tab 宽度替换成空白
    static auto expand_tabs(std::string_view s, size_type tab_size = 8) -> std::string;
    static auto expand_tabs_inplace(std::string& s, size_type tab_size = 8) -> std::string&;

    //! 扩展字符串中的 ~ 前缀
    static auto expand_user(std::string_view s) -> std::string;
    static auto expand_user_inplace(std::string& s) -> std::string&;

    //! 路径正常化
    static auto normpath(std::string_view s) -> std::string;
    static auto normpath_inplace(std::string& s) -> std::string&;

    //! 路径处理
    static auto is_absolute(std::string_view s) -> bool;
    static auto is_relative(std::string_view s) -> bool;

    // 基本的路径操控函数
    static auto basename_ptr(std::string_view s) -> std::string::const_pointer;
    static auto extname_ptr(std::string_view s) -> std::string::const_pointer;
    static auto dirname_ptr(std::string_view s) -> std::string::const_pointer;

    //! 将 s 视作为文件路径，获取其目录名
    static auto dirname_view(std::string_view s) -> std::string_view;
    static auto dirname(std::string_view s) -> std::string;
    static auto remove_dirname_view(std::string_view s) -> std::string_view;
    static auto remove_dirname(std::string_view s) -> std::string;
    static auto replace_dirname(std::string_view s, std::string_view newname) -> std::string;
    static auto split_dirname(std::string_view s) -> std::tuple<std::string_view, std::string_view>;
    //
    static auto dirname_inplace(std::string& s) -> std::string&;
    static auto remove_dirname_inplace(std::string& s) -> std::string&;
    static auto replace_dirname_inplace(std::string& s, std::string_view newname) -> std::string&;

    //! 处理路径中文件名的部分
    static auto basename_view(std::string_view s) -> std::string_view;
    static auto basename(std::string_view s) -> std::string;
    static auto remove_basename_view(std::string_view s) -> std::string_view;
    static auto remove_basename(std::string_view s) -> std::string;
    static auto replace_basename(std::string_view s, std::string_view name) -> std::string;
    static auto split_basename(std::string_view s) -> std::tuple<std::string_view, std::string_view>;
    //
    static auto basename_inplace(std::string& s) -> std::string&;
    static auto remove_basename_inplace(std::string& s) -> std::string&;
    static auto replace_basename_inplace(std::string& s, std::string_view name) -> std::string&;

    //! 扩展名相关操作
    static auto extname_view(std::string_view s) -> std::string_view;
    static auto extname(std::string_view s) -> std::string;
    static auto remove_extname_view(std::string_view s) -> std::string_view;
    static auto remove_extname(std::string_view s) -> std::string;
    static auto replace_extname(std::string_view s, std::string_view name) -> std::string;
    static auto split_extname(std::string_view s) -> std::tuple<std::string_view, std::string_view>;
    //
    static auto extname_inplace(std::string& s) -> std::string&;
    static auto remove_extname_inplace(std::string& s) -> std::string&;
    static auto replace_extname_inplace(std::string& s, std::string_view name) -> std::string&;

    //! 扩展名相关操作
    static auto rawname_view(std::string_view s) -> std::string_view;
    static auto rawname(std::string_view s) -> std::string;
    static auto replace_rawname(std::string_view s, std::string_view name) -> std::string;
    static auto split_rawname(std::string_view s) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    //
    static auto rawname_inplace(std::string& s) -> std::string&;
    static auto replace_rawname_inplace(std::string& s, std::string_view name) -> std::string&;

    //! 转换为 hash 值
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
    static auto charset(std::string_view s) -> charset_type;
    static auto charset(std::string_view s, charset_type& set) -> charset_type&;

    // 是否全都满足proc或者在set范围内
    static auto is_all(std::string_view s, mapping_proc<bool> proc) -> bool;
    static auto is_all(std::string_view s, charset_type set) -> bool;

    // 是否至少有一个满足proc或者在set范围内
    static auto has_any(std::string_view s, mapping_proc<bool> proc) -> bool;
    static auto has_any(std::string_view s, charset_type set) -> bool;

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

    // 读取下一个选项
    static auto read_opt(std::string_view s, size_type& pos) -> std::tuple<std::string_view, std::string_view>;

    // 读取特种类型的字符串
    static auto split_real(std::string_view s) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto read_integer(std::string_view s, size_type& pos) -> std::string_view;
    static auto read_real(std::string_view s, size_type& pos) -> std::string_view;
    static auto read_number(std::string_view s, size_type& pos) -> std::string_view;
    static auto read_identifier(std::string_view s, size_type& pos) -> std::string_view;
    static auto read_emain(std::string_view s, size_type& pos) -> std::string_view;

    // 按 proc 将字符序列分成两组，左边的满足proc，右边不满足proc
    static auto grouping(std::string_view s, const char_checker_proc& proc) -> std::tuple<std::string, std::string>;
    static auto filter(std::string_view s, const char_checker_proc& proc) -> std::string;
    static auto filter(std::string_view s, const charset_type& charset) -> std::string;
    static auto filter_inplace(std::string& s, const char_checker_proc& proc) -> std::string&;
    static auto filter_inplace(std::string& s, const charset_type& charset) -> std::string&;
};

#endif // TINY_STR_H
