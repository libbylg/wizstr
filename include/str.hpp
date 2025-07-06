/**
 * Copyright (c) 2021-2025  libbylg@126.com
 * str is licensed under Mulan PSL v2.
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

#include <array>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <limits>
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <vector>

#include "str_config.hpp"

//! Adaptor for namespace
#if defined(STR_NAMESPACE)
namespace STR_NAMESPACE {
#endif

struct str {
    using size_type = std::string::size_type;
    using ssize_type = std::make_signed_t<size_t>;
    using value_type = std::string::value_type;
    using pointer = std::string::pointer;
    using const_pointer = std::string::const_pointer;

    //! 用于表示无效位置的值，等价于 std::string::npos 或者 std::string_view::npos
    /// @block npos
    static inline constexpr size_type npos = std::string::npos;
    /// @end npos

    //! 字符集 @anchor{charset_type}
    ///
    /// 字符集类用于表示一组字符的集合，主要用于以字符为分隔符或者检索目标的场景。
    /// 大多数情况，我们并不会直接使用 @{charset_type}，而是使用 @{charset} 函数来生成
    /// 字符集类。
    class charset_type {
    public:
        //! 构造空的字符集对象
        explicit charset_type() {
        }

        //! 构造只有只有字符 `ch` 的字符集
        explicit charset_type(value_type ch)
            : charset_type() {
            set(ch);
        }

        //! 构造包含 `s` 中的所有字符的字符集
        explicit charset_type(std::string_view s)
            : charset_type() {
            for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ptr++) {
                set(*ptr);
            }
        }

        //! 将 `s` 中的字符加入本字符集或者从本字符集中剔除
        inline auto set(std::string_view s, bool val = true) -> void {
            for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ptr++) {
                set(*ptr, val);
            }
        }

        //! 将字符 ch 加入本字符集或者从本字符集中剔除
        inline auto set(value_type c, bool val = true) -> void {
            uint8_t ch = static_cast<value_type>(c);
            size_type index = ch / 64;
            size_type mask = uint64_t{1} << (ch % 64);
            if (val) {
                bits_[index] |= mask;
            } else {
                bits_[index] ^= ~mask;
            }
        }

        //! 将字符串 s 中的所有字符从本字符集中剔除
        inline auto clr(std::string_view s) -> void {
            for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ptr++) {
                clr(*ptr);
            }
        }

        //! 将字符 ch 从本字符集中剔除
        inline auto clr(value_type c) -> void {
            uint8_t ch = static_cast<value_type>(c);
            size_type index = ch / 64;
            size_type mask = uint64_t{1} << (ch % 64);
            bits_[index] ^= ~mask;
        }

        //! 清空本字符集中的所有字符
        inline auto clr() -> void {
            bits_[0] = 0;
            bits_[1] = 0;
            bits_[2] = 0;
            bits_[3] = 0;
        }

        //! 查询字符 ch 是否在本字符集中
        inline auto contains(value_type ch) const -> bool {
            uint8_t c = static_cast<value_type>(ch);
            size_type index = c / 64;
            size_type mask = uint64_t{1} << (c % 64);
            return !!(bits_[index] & mask);
        }

        //! 根据 ASCII 码表，生成本字符集的补集
        inline auto invert() -> void {
            bits_[0] = ~bits_[0];
            bits_[1] = ~bits_[1];
            bits_[2] = ~bits_[2];
            bits_[3] = ~bits_[3];
        }

        //! 查询字符 ch 是否在本字符集中，等价于 get(ch) 或者 contains(ch)
        inline auto operator()(value_type ch) const -> bool {
            return contains(ch);
        }

        //! 获取内部存储指针，需要注意每个 bit 位代表字符集中的一个字符
        inline auto data() const -> const uint64_t* {
            return bits_;
        }

        //! 获取内部存储指针，需要注意每个 bit 位代表字符集中的一个字符
        inline auto data() -> uint64_t* {
            return bits_;
        }

        //! 将本字符集中所包含的所有字符提取出来。
        inline auto string() const -> std::string {
            std::string result;
            for (size_type ch = 0; ch <= std::numeric_limits<value_type>::max(); ch++) {
                static_assert(sizeof(ch) > sizeof(value_type));
                if (contains(static_cast<value_type>(ch))) {
                    result.push_back(static_cast<value_type>(ch));
                }
            }
            return result;
        }

        inline auto operator|(const charset_type& b) -> charset_type {
            charset_type result;
            result.bits_[0] = bits_[0] | b.bits_[0];
            result.bits_[1] = bits_[1] | b.bits_[1];
            result.bits_[2] = bits_[2] | b.bits_[2];
            result.bits_[3] = bits_[3] | b.bits_[3];
            return result;
        }

        inline auto operator|=(const charset_type& b) -> charset_type& {
            bits_[0] |= b.bits_[0];
            bits_[1] |= b.bits_[1];
            bits_[2] |= b.bits_[2];
            bits_[3] |= b.bits_[3];
            return *this;
        }

    private:
        //! 用于表示字符集中哪些字符在本字符集中，每个 bit 位代表字符集中的一个字符。
        uint64_t bits_[4]{0, 0, 0, 0};
    };

    //! 基于位置的范围类型
    struct range_type {
        //! 起始位置
        size_type pos{0};

        //! 长度
        size_type len{0};

        //! 空范围，长度为 0
        explicit range_type() = default;

        //! 指定起始位置和长度构造范围对象
        explicit range_type(size_type tpos, size_type tn)
            : pos{tpos}
            , len{tn} {
        }

        //! 获得范围的长度（即 len）。
        inline auto size() const -> size_type {
            return len;
        }

        //! 判断范围是否为空。需要注意，不管 pos 如何，只要 len 长度为 0，范围都为空。
        inline auto empty() const -> bool {
            return len == 0;
        }

        inline auto begin() const -> size_type {
            return pos;
        }

        inline auto end() const -> size_type {
            size_type result{};
            if (__builtin_add_overflow(pos, len, &result)) {
                return npos;
            }
            return result;
        }

        inline auto operator==(const range_type& range) const -> bool {
            if (len != range.len) {
                return false;
            }

            if (len == 0) {
                return true;
            }

            return pos == range.pos;
        }

        auto shift(ssize_type offset) -> range_type& {
            pos += offset;
            return *this;
        }
    };

    //! 基于起始位置和偏移量的范围类型。
    struct shifter_type {
        size_type pos{0};
        ssize_type offset{0};

        auto empty() const -> bool {
            return offset == 0;
        }
    };

    //! 基于上下界的范围类型。
    struct interval_type {
        size_type begin{0};
        size_type end{0};

        auto empty() const -> bool {
            return begin == end;
        }
    };

    //! 二元组（相同类型的 2 个元素）。
    template <typename T>
    using pair = std::tuple<T, T>;

    //! 三元组类型（相同类型的 3 个元素）。
    template <typename T>
    using ternary = std::array<T, 3>;

    //! `std::string_view` 供给器：每次调用返回一个字符串，直到返回 `std::nullopt`。
    using view_provider_proc = std::function<std::optional<std::string_view>()>;

    //! 键值对供给器：每次调用返回一个 key-value 对组成的二元组（@ref{pair}），直到返回 `std::nullopt`。
    using view_pair_provider_proc = std::function<std::optional<pair<std::string_view>>()>;

    //! 整数供给器：每次调用返回一个 `size_type` 类型的整数，主要用于抽象随机函数。
    using number_provider_proc = std::function<size_type()>;

    //! `std::string_view` 消费器：接收一个 `std::string_view`，常用于接收一个或者多个字符串，如果需要提前结束，
    /// 可以返回非 0 值。
    using view_consumer_proc = std::function<int(std::string_view item)>;

    //! 键值对消费器：接收两个 `std::string_view` 类型的键值对（key 和 value 参数），如果需要提前结束，
    /// 可以返回非 0 值。
    using view_pair_consumer_proc = std::function<int(std::string_view key, std::string_view value)>;

    //! range 消费器：接收一个 range_type 类型的参数，常用于表示找到的子串的范围，如果需要提前结束，可以返回非 0 值
    using range_consumer_proc = std::function<int(range_type range)>;

    //! 单字符匹配器：检查作为输入参数的字符是否满足特定的条件，常用基于字符的查找和搜索场景
    using char_match_proc = std::function<bool(value_type ch)>;

    //! 字符串视图检索器：在指定的范围内查找，如果找到返回找到的子串，否则返回 std::nullopt
    using view_search_proc = std::function<std::optional<std::string_view>(std::string_view search_range)>;

    //! 字符串视图检索器：在指定的范围内查找，返回找到的子串，或者为找到时 std::nullopt
    using substr_search_proc = std::function<size_type(std::string_view text, size_type& pos)>;

    //! 行消费器：接收一个行索引和行文字，常用于字符串按行、分割读取等场景，如果需要提前结束，可以返回 0
    using line_consumer_proc = std::function<int(size_type line_index, std::string_view line_text)>;

    //! 单字符映射：将单个字符映射为另一个数据类型的值
    template <typename MappedType>
    using mapping_proc = std::function<auto(value_type)->MappedType>;

    //! 单字符映射器：将一个字符映射为另一个字符
    using char_mapping_proc = mapping_proc<value_type>;

    //! 字符串映射：将一个字符串映射为另一个字符串，如果能成功映射返回映射后的字符串，否则返回 std::nullopt
    using string_mapping_proc = std::function<std::optional<std::string>(const std::string& key)>;

    /// @block system_var
    static constexpr std::string_view sep_searchpath = ":";
    static constexpr value_type sep_searchpath_char = ':';
    static constexpr std::string_view sep_path = "/";
    static constexpr value_type sep_path_char = '/';
    static constexpr std::string_view sep_line_ends = "\n";
    /// @end system_var

    /// @block builtin_charsets
    static constexpr std::string_view all_uppers = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static constexpr std::string_view all_lowers = "abcdefghijklmnopqrstuvwxyz";
    static constexpr std::string_view all_leters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static constexpr std::string_view all_alphas = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static constexpr std::string_view all_digits = "0123456789";
    static constexpr std::string_view all_xdigits = "0123456789ABCDEFabcdef";
    static constexpr std::string_view all_alnums = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    static constexpr std::string_view all_alnumuls = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
    static constexpr std::string_view all_aluls = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
    static constexpr std::string_view all_spaces = "\x09\x0A\x0B\x0C\x0d\x20";
    static constexpr std::string_view all_hex_upper = "0123456789ABCDEF";
    static constexpr std::string_view all_hex_lower = "0123456789abcdef";
    static constexpr std::string_view all_puncts = R"(!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)";
    /// @end builtin_charsets

    /// @block append
    static auto append(std::string_view s, std::string_view other, size_type times_n = 1) -> std::string;
    static auto append(std::string_view s, const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto append(std::string_view s, const Sequence& items) -> std::string;
    /// -
    static auto append_inplace(std::string& s, std::string_view other, size_type times_n = 1) -> std::string&;
    static auto append_inplace(std::string& s, const view_provider_proc& proc) -> std::string&;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto append_inplace(std::string& s, const Sequence& items) -> std::string&;
    /// @end append

    /// @block prepend
    static auto prepend(std::string_view s, std::string_view other, size_type times_n = 1) -> std::string;
    static auto prepend(std::string_view s, const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto prepend(std::string_view s, const Sequence& items) -> std::string;
    /// -
    static auto prepend_inplace(std::string& s, std::string_view other, size_type times_n = 1) -> std::string&;
    static auto prepend_inplace(std::string& s, const view_provider_proc& proc) -> std::string&;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto prepend_inplace(std::string& s, const Sequence& items) -> std::string&;
    /// @end append
    ///
#ifdef STR_UNTESTED
    //! 向字符串中间插入 @anchor{insert}
    ///
    /// 向字符串指定的位置插入新的字符串或者字符。
    /// 对于 insert_inplace 函数，如果 s 与 被插入字符串存在重叠时，函数的行为是不确定的，应该避免出现这种情况。
    ///
    /// @param s: 向该字符串查询新串
    /// @param pos: 指定插入的起始位置，如果 pos 大于或者等于 s 的长度，等价于 append。
    /// @param other: 被插入的字符串，如果 other 与 s 存在重叠，可能会导致插入的数据达不到预期。
    /// @param ch: 被插入的字符
    /// @param n: 重复插入次数
    /// @param proc: 由 proc 函数提供被追加的字符串，如果 proc 返回 std::nullopt，表示后续无更多字符串需要追加。
    /// @param items: 从容器 items 中获取被插入的字符串。
    static auto insert(std::string_view s, size_type pos, std::string_view other, size_type times_n = 1) -> std::string;
    static auto insert(std::string_view s, size_type pos, value_type ch, size_type times_n = 1) -> std::string;
    static auto insert(std::string_view s, size_type pos, const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto insert(std::string& s, size_type pos, const Sequence& items) -> std::string&;
    //
    static auto insert_inplace(std::string& s, size_type pos, std::string_view other, size_type times_n = 1) -> std::string&;
    static auto insert_inplace(std::string& s, size_type pos, value_type ch, size_type times_n = 1) -> std::string&;
    static auto insert_inplace(std::string& s, size_type pos, const view_provider_proc& proc) -> std::string&;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto insert_inplace(std::string& s, size_type pos, const Sequence& items) -> std::string&;
#endif // STR_UNTESTED

    /// @block icompare
    static auto icompare(std::string_view s, std::string_view other) -> int;
    static auto icompare(std::string_view s, std::string_view other, size_type max_n) -> int;
    /// @end icompare

    /// @block iequals
    static auto iequals(std::string_view s, std::string_view other) -> bool;
    static auto iequals(std::string_view s, std::string_view other, size_type max_n) -> bool;
    /// @end iequals

    /// @block wildcmp
    static auto wildcmp(const_pointer s, const_pointer pattern) -> bool;
    static auto wildcmp(std::string_view s, std::string_view pattern) -> bool;
    /// -
    static auto iwildcmp(const_pointer s, const_pointer pattern) -> bool;
    static auto iwildcmp(std::string_view s, std::string_view pattern) -> bool;
    /// @end wildcmp

    /// @block contains
    static auto contains(std::string_view s, std::string_view other) -> bool;
    static auto contains(std::string_view s, value_type ch) -> bool;
    static auto contains(std::string_view s, const char_match_proc& proc) -> bool;
    static auto contains(std::string_view s, const charset_type& charset) -> bool;
    static auto contains(std::string_view s, const std::regex& pattern) -> bool;
    /// -
    static auto icontains(std::string_view s, std::string_view other) -> bool;
    static auto icontains(std::string_view s, value_type ch) -> bool;
    /// @end icontains

    /// @block count
    static auto count(std::string_view s, std::string_view other) -> size_type;
    static auto count(std::string_view s, value_type ch) -> size_type;
    static auto count(std::string_view s, const char_match_proc& proc) -> size_type;
    static auto count(std::string_view s, const charset_type& charset) -> size_type;
    static auto count(std::string_view s, const std::regex& pattern) -> size_type;
    /// @end count

    /// @block prefix
    static auto prefix(std::string_view s, std::string_view other) -> size_type;
    static auto has_prefix(std::string_view s, value_type ch) -> bool;
    static auto has_prefix(std::string_view s, std::string_view prefix) -> bool;
    static auto starts_with(std::string_view s, value_type ch) -> bool;
    static auto starts_with(std::string_view s, std::string_view prefix) -> bool;
    static auto starts_with(std::string_view s, size_type pos, value_type ch) -> bool;
    static auto starts_with(std::string_view s, size_type pos, std::string_view prefix) -> bool;
    static auto remove_prefix_view(std::string_view s, std::string_view prefix) -> std::string_view;
    static auto remove_prefix_view(std::string_view s, value_type prefix) -> std::string_view;
    static auto remove_prefix(std::string_view s, std::string_view prefix) -> std::string;
    static auto remove_prefix(std::string_view s, value_type prefix) -> std::string;
    /// -
    static auto remove_prefix_inplace(std::string& s, std::string_view prefix) -> std::string&;
    static auto remove_prefix_inplace(std::string& s, value_type prefix) -> std::string&;
    /// @end prefix

    /// @block suffix
    static auto suffix(std::string_view s, std::string_view other) -> size_type;
    static auto has_suffix(std::string_view s, value_type ch) -> bool;
    static auto has_suffix(std::string_view s, std::string_view suffix) -> bool;
    static auto ends_with(std::string_view s, value_type ch) -> bool;
    static auto ends_with(std::string_view s, std::string_view suffix) -> bool;
    static auto remove_suffix_view(std::string_view s, std::string_view suffix) -> std::string_view;
    static auto remove_suffix_view(std::string_view s, value_type ch) -> std::string_view;
    static auto remove_suffix(std::string_view s, std::string_view suffix) -> std::string;
    static auto remove_suffix(std::string_view s, value_type ch) -> std::string;
    /// -
    static auto remove_suffix_inplace(std::string& s, std::string_view suffix) -> std::string&;
    static auto remove_suffix_inplace(std::string& s, value_type ch) -> std::string&;
    /// @end suffix

    /// @block spaces_margin
    static auto starts_with_spaces(std::string_view s) -> bool;
    static auto ends_with_spaces(std::string_view s) -> bool;
    static auto starts_with_margin(std::string_view s, value_type margin) -> bool;
    /// @end spaces_margin

    /// @block next_char
    static auto next_char(std::string_view s, size_type& pos, value_type ch) -> std::optional<size_type>;
    static auto next_char(std::string_view s, size_type& pos, const charset_type& charset) -> std::optional<size_type>;
    static auto next_char(std::string_view s, size_type& pos, std::string_view charset) -> std::optional<size_type>;
    static auto next_char(std::string_view s, size_type& pos, const char_match_proc& proc) -> std::optional<size_type>;
    /// -
    static auto prev_char(std::string_view s, size_type& pos, value_type ch) -> std::optional<size_type>;
    static auto prev_char(std::string_view s, size_type& pos, const charset_type& charset) -> std::optional<size_type>;
    static auto prev_char(std::string_view s, size_type& pos, std::string_view charset) -> std::optional<size_type>;
    static auto prev_char(std::string_view s, size_type& pos, const char_match_proc& proc) -> std::optional<size_type>;
    /// @end next_char

    /// @block next_string
    static auto next_string_range(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<range_type>;
    static auto next_string_view(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<std::string_view>;
    static auto next_string(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<std::string>;
    /// -
    static auto prev_string_range(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<range_type>;
    static auto prev_string_view(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<std::string_view>;
    static auto prev_string(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<std::string>;
    /// @end next_string

    /// @block next_eol_prev_eol
    static auto next_eol_range(std::string_view s, size_type& pos) -> std::optional<range_type>;
    static auto next_eol_view(std::string_view s, size_type& pos) -> std::optional<std::string_view>;
    static auto next_eol(std::string_view s, size_type& pos) -> std::optional<std::string>;
    /// -
    static auto prev_eol_range(std::string_view s, size_type& pos) -> std::optional<range_type>;
    static auto prev_eol_view(std::string_view s, size_type& pos) -> std::optional<std::string_view>;
    static auto prev_eol(std::string_view s, size_type& pos) -> std::optional<std::string>;
    /// @end next_eol_prev_eol

    /// @block eol
    static auto ends_with_eol(std::string_view s) -> bool;
    static auto has_eol_suffix(std::string_view s) -> bool;
    static auto eol_suffix(std::string_view s) -> size_type;
    /// @end eol

    /// @block remove_eol
    static auto remove_eol_suffix_range(std::string_view s) -> range_type;
    static auto remove_eol_suffix_view(std::string_view s) -> std::string_view;
    static auto remove_eol_suffix(std::string_view s) -> std::string;
    static auto remove_eol_suffix_inplace(std::string& s) -> std::string&;
    /// @end remove_eol

#ifdef STR_UNTESTED
    //! 定位正则表达式分隔符
    static auto next_regex_range(std::string_view s, size_type& pos, const std::regex& pattern) -> range_type;
    static auto next_regex_view(std::string_view s, size_type& pos, const std::regex& pattern) -> std::string_view;
    static auto next_regex(std::string_view s, size_type& pos, const std::regex& pattern) -> std::string;
    static auto next_regex(std::string_view s, size_type& pos, std::string_view pattern) -> std::string;
#endif // STR_UNTESTED
    //
#ifdef STR_UNIMPL
    static auto prev_regex_range(std::string_view s, size_type& pos, const std::regex& pattern) -> range_type;
    static auto prev_regex_view(std::string_view s, size_type& pos, const std::regex& pattern) -> std::string_view;
    static auto prev_regex(std::string_view s, size_type& pos, const std::regex& pattern) -> std::string;
    static auto prev_regex(std::string_view s, size_type& pos, std::string_view pattern) -> std::string;
#endif // STR_UNIMPL

#ifdef STR_UNIMPL
    //! 定位路径分隔符
    static auto next_pathsep_range(std::string_view s, size_type& pos) -> range_type;
    static auto next_pathsep_view(std::string_view s, size_type& pos) -> std::string_view;
    static auto next_pathsep(std::string_view s, size_type& pos) -> std::string;
    static auto next_searchpathsep_range(std::string_view s, size_type& pos) -> range_type;
    static auto next_searchpathsep_view(std::string_view s, size_type& pos) -> std::string_view;
    static auto next_searchpathsep(std::string_view s, size_type& pos) -> std::string;
#endif // STR_UNIMPL

    /// @block next_spaces
    static auto next_spaces_pos(std::string_view s, size_type& pos) -> size_type;
    static auto next_spaces(std::string_view s, size_type& pos) -> std::optional<size_type>;
    static auto next_spaces_range(std::string_view s, size_type& pos) -> std::optional<range_type>;
    static auto next_spaces_view(std::string_view s, size_type& pos) -> std::optional<std::string_view>;
    /// -
    static auto prev_spaces_pos(std::string_view s, size_type& pos) -> size_type;
    static auto prev_spaces(std::string_view s, size_type& pos) -> std::optional<size_type>;
    static auto prev_spaces_range(std::string_view s, size_type& pos) -> std::optional<range_type>;
    static auto prev_spaces_view(std::string_view s, size_type& pos) -> std::optional<std::string_view>;
    /// @end next_spaces

#ifdef STR_UNTESTED
    //! 定位满足条件的子串
    ///
    /// next_proc_xxx 和 prev_proc_xxx 系列函数提供了更通用的子串查找（定位）算法。
    ///
    /// @notice{1} 需要特别注意，@ref{next_xxx} 总是从 `pos - 1` 开始，向字符串首部查找（而 @ref{next_char} 总是从
    /// `pos` 开始向字符串尾部查找）。因此，如果希望从 `s` 的最后一个字符开始向前查找时，`pos` 应该指定为 `s.size()`。
    /// 当调用 @ref{prev_xxx}系列函数且 `pos` 参数指定的值为 `0` 时， @ref{prev_xxx} 系列函数已无法继续向前查找，此时
    /// 将返回 `std::nullopt`。同理，当调用 @ref{next_xxx} 系列函数，且 `pos` 参数大于或者等于 `s.size()` 时，
    /// @ref{prev_xxx} 系列函数同样无法继续查找，此时也将返回 `std::nullopt`。
    ///
    /// @notice{2} 当找到特定的字符时，@ref{next_xxx} 的 `pos` 总是为该字符的下一个字符，而 @ref{pref_xxx} 的 `pos`
    /// 总是指向该字符所在的位置。这个设计使得 @ref{next_xxx} 和 @ref{prev_xxx} 可以配合使用。
    ///
    /// @param s 在该字符串内查找
    /// @param pos 查找的起始位置，需要注意该字段对于 `next_xxx` 和 `prev_xxx` 具有不同的含义（参见 @ref{notice{2}}）。
    /// @param proc 子串定位条件
    template <typename RangeSearchProc, typename = std::enable_if<std::is_function<RangeSearchProc>::value>>
    static auto next_proc_range(std::string_view s, size_type& pos, const RangeSearchProc& proc) -> range_type;
    template <typename RangeSearchProc, typename = std::enable_if<std::is_function<RangeSearchProc>::value>>
    static auto next_proc_view(std::string_view s, size_type& pos, const RangeSearchProc& proc) -> std::string_view;
    template <typename RangeSearchProc, typename = std::enable_if<std::is_function<RangeSearchProc>::value>>
    static auto next_proc(std::string_view s, size_type& pos, const RangeSearchProc& proc) -> std::string;
    /// -
#endif // STR_UNTESTED
#ifdef STR_UNIMPL
    static auto prev_proc_range(std::string_view s, size_type& pos, const substr_search_proc& proc) -> range_type;
    static auto prev_proc_view(std::string_view s, size_type& pos, const substr_search_proc& proc) -> std::string_view;
    static auto prev_proc(std::string_view s, size_type& pos, const substr_search_proc& proc) -> std::string;
#endif // STR_UNIMPL

    /// @block is_charset
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
    /// @end is_charset

    /// @block is_token
    static auto is_identifier(std::string_view s) -> bool;
    static auto is_literal_bool(std::string_view s) -> bool;
    static auto is_literal_true(std::string_view s) -> bool;
    static auto is_literal_false(std::string_view s) -> bool;
    static auto is_literal_integer(std::string_view s) -> bool;
    static auto is_literal_real(std::string_view s) -> bool;
    /// @end is_token

    /// @block is_all_in
    template <typename CharMatchProc, typename = std::enable_if<std::is_function<CharMatchProc>::value>>
    static auto is_all_in(std::string_view s, const CharMatchProc& proc) -> bool;
    static auto is_all_in(std::string_view s, const charset_type& charset) -> bool;
    /// @end is_token

    /// @end has_any_one
    template <typename CharMatchProc, typename = std::enable_if<std::is_function<CharMatchProc>::value>>
    static auto has_any_one(std::string_view s, const CharMatchProc& proc) -> bool;
    static auto has_any_one(std::string_view s, const charset_type& charset) -> bool;
    /// @block is_all_in

    //! 是否空白或者控
    // @block is_space_or_empty
    static auto is_space_or_empty(std::string_view s) -> bool;
    // @end is_space_or_empty


    // @block take
    static auto take_left_view(std::string_view s, size_type n) -> std::string_view;
    static auto take_right_view(std::string_view s, size_type n) -> std::string_view;
    static auto take_view(std::string_view s, size_type pos, size_type n) -> std::string_view;
    static auto take_view(std::string_view s, size_type pos) -> std::string_view;
    static auto take_view(std::string_view s, range_type range) -> std::string_view;
    static auto take_view(std::string_view s, interval_type inter) -> std::string_view;
    static auto take_view(std::string_view s, shifter_type shifter) -> std::string_view;
    /// -
    static auto take_left(std::string_view s, size_type n) -> std::string;
    static auto take_right(std::string_view s, size_type n) -> std::string;
    static auto take(std::string_view s, size_type pos, size_type n) -> std::string;
    static auto take(std::string_view s, size_type pos) -> std::string;
    static auto take(std::string_view s, range_type range) -> std::string;
    static auto take(std::string_view s, interval_type inter) -> std::string;
    static auto take(std::string_view s, shifter_type slider) -> std::string;
    /// -
    static auto take_left_inplace(std::string& s, size_type n) -> std::string&;
    static auto take_right_inplace(std::string& s, size_type n) -> std::string&;
    static auto take_inplace(std::string& s, size_type pos, size_type n) -> std::string&;
    static auto take_inplace(std::string& s, range_type range) -> std::string&;
    static auto take_inplace(std::string& s, size_type pos) -> std::string&;
    static auto take_inplace(std::string& s, interval_type inter) -> std::string&;
    static auto take_inplace(std::string& s, shifter_type slider) -> std::string&;
    /// @end take

#ifdef STR_UNTESTED
    static auto take_before_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto take_after_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto take_before(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;
    static auto take_after(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;
#endif // STR_UNTESTED

    /// @block drop
    static auto drop_left_view(std::string_view s, size_type n) -> std::string_view;
    static auto drop_right_view(std::string_view s, size_type n) -> std::string_view;
    static auto drop_left(std::string_view s, size_type n) -> std::string;
    static auto drop_right(std::string_view s, size_type n) -> std::string;
    static auto drop(std::string_view s, size_type pos, size_type n) -> std::string;
    static auto drop(std::string_view s, size_type pos) -> std::string;
    static auto drop(std::string_view s, range_type range) -> std::string;
    static auto drop(std::string_view s, interval_type inter) -> std::string;
    static auto drop(std::string_view s, shifter_type shifter) -> std::string;
    static auto drop(std::string_view s, const char_match_proc& proc) -> std::string;
    static auto drop(std::string_view s, const charset_type& charset) -> std::string;
    //
    static auto drop_left_inplace(std::string& s, size_type n) -> std::string&;
    static auto drop_right_inplace(std::string& s, size_type n) -> std::string&;
    static auto drop_inplace(std::string& s, size_type pos, size_type n) -> std::string&;
    static auto drop_inplace(std::string& s, size_type pos) -> std::string&;
    static auto drop_inplace(std::string& s, range_type range) -> std::string&;
    static auto drop_inplace(std::string& s, interval_type inter) -> std::string&;
    static auto drop_inplace(std::string& s, shifter_type shifter) -> std::string&;
    static auto drop_inplace(std::string& s, const char_match_proc& proc) -> std::string&;
    static auto drop_inplace(std::string& s, const charset_type& charset) -> std::string&;
    /// @end drop

#ifdef STR_UNTESTED
    static auto drop_before_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto drop_after_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto drop_before(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;
    static auto drop_after(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;
#endif // STR_UNTESTED

    /// @block align
    static auto align_left(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_right(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_center(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_zfill(std::string_view s, size_type width) -> std::string;
    static auto align_left_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_right_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_center_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_zfill_inplace(std::string& s, size_type width) -> std::string&;
    /// @end align

    /// @block lines
    static auto foreach_lines(std::string_view s, bool keep_ends, const line_consumer_proc& proc) -> void;
    static auto count_lines(std::string_view s) -> size_type;
    /// @end lines

#ifdef STR_UNTESTED
    /// @ref lines_indentation: 计算 s 的多行中共同空白前缀的空白的数量
    /// @ref indent_lines, indent_lines_inplace: 将 s 中的所有行统一增加指定宽度的缩进空白
    /// @ref unindent_lines, unindent_lines_inplace: 将 s 中的所有行统一去除指定宽度的缩进空白
    /// @ref trim_lines_indent, trim_lines_indent_inplace: 将 s 中的所有行的缩进都去掉
    /// @ref trim_lines_margin, trim_lines_margin_inplace: 将 s 中的所有行的缩进以及前导符号去掉
    ///
    /// @param from_n: 指定行编号从哪个整数开始计数
    /// @param num_format: 指定整数的格式
    /// @param pad: 指定缩进时的填充字符
    /// @param margin: 指定一个边缘字符
    static auto lines_indentation(std::string_view s) -> size_type;
#endif // STR_UNTESTED

#ifdef STR_UNTESTED
    //! 多行编号
    ///
    /// @ref numbering_lines, numbering_lines_inplace: 给字符串 s 中的多行加上行号前缀
    /// @ref unnumbering_lines, unnumbering_lines_inplace: 去掉 s 中的多行的行号前缀
    static auto numbering_lines(std::string_view s, size_type from_n) -> std::string;
    static auto unnumbering_lines(std::string_view s) -> std::string;
    //
    static auto numbering_lines_inplace(std::string& s, size_type from_n) -> std::string&;
    static auto unnumbering_lines_inplace(std::string& s) -> std::string&;
#endif // STR_UNTESTED

#ifdef STR_UNTESTED
    //! 多行缩进
    static auto indent_lines(std::string_view s, size_type n) -> std::string;
    static auto dedent_lines(std::string_view s, size_type n) -> std::string;
    static auto align_indent_lines(std::string_view s, size_type n) -> std::string;
    static auto trim_indent_lines(std::string_view s) -> std::string;
    static auto simplify_indent_lines(std::string_view s) -> std::string;
    //
    static auto indent_lines_inplace(std::string& s, size_type n) -> std::string&;
    static auto dedent_lines_inplace(std::string& s, size_type n) -> std::string&;
    static auto align_indent_lines_inplace(std::string& s, size_type n) -> std::string&;
    static auto trim_indent_lines_inplace(std::string& s) -> std::string&;
    static auto simplify_indent_lines_inplace(std::string& s) -> std::string&;
#endif // STR_UNTESTED

#ifdef STR_UNTESTED
    //! 多行留边
    static auto margin_lines(std::string_view s, size_type n, value_type margin_ch = '|') -> std::string;
    static auto trim_margin_lines(std::string_view s, value_type margin_ch = ' ') -> std::string;
    //
    static auto margin_lines_inplace(std::string& s, size_type n, value_type margin_ch = ' ') -> std::string&;
    static auto trim_margin_lines_inplace(std::string& s, value_type margin_ch = ' ') -> std::string&;
#endif // STR_UNTESTED

    /// @block words
    static auto foreach_words(std::string_view s, size_type pos, const range_consumer_proc& proc) -> void;
    static auto foreach_words(std::string_view s, size_type pos, const view_consumer_proc& proc) -> void;
    static auto foreach_words(std::string_view s, const range_consumer_proc& proc) -> void;
    static auto foreach_words(std::string_view s, const view_consumer_proc& proc) -> void;
    /// -
    static auto count_words(std::string_view s) -> size_type;
    /// -
    static auto next_word_view(std::string_view s, size_type& pos) -> std::string_view;
    static auto next_word_range(std::string_view s, size_type& pos) -> range_type;
    static auto next_word(std::string_view s, size_type& pos) -> std::string;
    /// -
    static auto prev_word_view(std::string_view s, size_type& pos) -> std::string_view;
    static auto prev_word_range(std::string_view s, size_type& pos) -> range_type;
    static auto prev_word(std::string_view s, size_type& pos) -> std::string;
    /// -
    static auto split_words(std::string_view s, size_type max_n = npos) -> std::vector<std::string>;
    /// -
    static auto starts_with_word(std::string_view s, std::string_view word) -> bool;
    static auto ends_with_word(std::string_view s, std::string_view word) -> bool;
    /// @end words
#ifdef STR_UNTESTED
    static auto split_words_view(std::string_view s, size_type max_n = npos) -> std::vector<std::string_view>;
#endif // STR_UNTESTED

    /// @block surround_unsurround
    static auto surround(std::string_view s, std::string_view left = "(", std::string_view right = ")") -> std::string;
    static auto surround_inplace(std::string& s, std::string_view left = "(", std::string_view right = ")") -> std::string&;
    /// -
    static auto unsurround_view(std::string_view s, std::string_view left = "(", std::string_view right = ")") -> std::string_view;
    static auto unsurround(std::string_view s, std::string_view left = "(", std::string_view right = ")") -> std::string;
    static auto unsurround_inplace(std::string& s, std::string_view left = "(", std::string_view right = ")") -> std::string&;
    /// @end surround_unsurround

    /// @block invert
    static auto invert(std::string_view s, size_type pos = 0, size_type max_n = npos) -> std::string;
    static auto invert_inplace(std::string& s, size_type pos = 0, size_type max_n = npos) -> std::string&;
    /// @end invert

    /// @block repeat
    static auto repeat(std::string_view s, size_type times) -> std::string;
    static auto repeat(value_type ch, size_type times) -> std::string;
    /// @end repeat

#ifdef STR_UNTESTED
    //! 随机字符序列生成
    ///
    /// 按照特定的策略，随机生成字符串
    ///
    /// @param n 指定生成的字符串的长度的范围
    /// @param proc 指定随机数函数
    /// @param ch, charset 指生成的字符串中的字符的范围
    static auto random(size_type n, const number_provider_proc& proc) -> std::string;
    static auto random(size_type n, std::string_view charset, const number_provider_proc& proc) -> std::string;
    static auto random(size_type n, const charset_type& charset, const number_provider_proc& proc) -> std::string;
#endif // STR_UNTESTED

#ifdef STR_UNTESTED
    //! 随机字符序列填充
    ///
    /// 自动随机填充指定的字符串（不改变大小）
    ///
    /// @param s 原始字符串
    /// @param charset 指定生成的字符串中的字符的范围
    /// @param proc 随机函数
    static auto random_fill(std::string& s, std::string_view charset, const number_provider_proc& proc) -> std::string&;
    static auto random_fill(std::string& s, const charset_type& charset, const number_provider_proc& proc) -> std::string&;
    static auto random_fill(std::string& s, const number_provider_proc& proc) -> std::string&;
#endif // STR_UNTESTED

#ifdef STR_UNTESTED
    //! 字符虚拟随机重排
    ///
    /// 现有字符串随机重排（不改变大小）
    ///
    /// @param s 指定被重拍的字符串
    /// @param proc 随机数生成器
    /// @return 将 s 重排后返回
    static auto random_reorder(std::string& s, const number_provider_proc& proc) -> std::string&;
#endif // STR_UNTESTED

    /// @block spaces
    static auto spaces(uint8_t width) -> std::string_view;
    static auto make_spaces(size_type width) -> std::string;
    static auto make_spaces_inplace(std::string& s, size_type width) -> std::string&;
    /// @end spaces
#ifdef STR_UNTESTED
    static auto after_skip_spaces_view(std::string_view s) -> std::string_view;
    static auto after_skip_spaces(std::string_view s) -> std::string;
    static auto after_skip_spaces_inplace(std::string& s) -> std::string&;
    static auto after_skip_spaces_view(std::string_view s, size_type pos) -> std::string_view;
    static auto after_skip_spaces(std::string_view s, size_type pos) -> std::string;
    static auto after_skip_spaces_inplace(std::string& s, size_type pos) -> std::string&;
#endif // STR_UNTESTED

    /// @block cover
    static auto cover(std::string_view s, std::string_view mask = "***", size_type left_n = 1, size_type right_n = 3) -> std::string;
    static auto cover_inplace(std::string& s, std::string_view mask = "***", size_type left_n = 1, size_type right_n = 3) -> std::string&;
    /// @end cover

#ifdef STR_UNIMPL
    //! 元音（aeiou）化简算法：去掉单词中的元音字符 除非他们为单词的首字母
#endif // STR_UNIMPL

    /// @block join
    static auto join(std::string_view s, const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join(std::string_view s, const Sequence& items) -> std::string;
    /// @end join

    /// @block join_list
    static auto join_list(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_list(const Sequence& items) -> std::string;
    /// @end join

    /// @block join_map
    static auto join_map(std::string_view sep_pair, std::string_view sep_list, const view_pair_provider_proc& proc) -> std::string;
    static auto join_map(const view_pair_provider_proc& proc) -> std::string;
    template <typename Map, typename = typename Map::const_iterator>
    static auto join_map(std::string_view sep_pair, std::string_view sep_list, const Map& items) -> std::string;
    template <typename Map, typename = typename Map::const_iterator>
    static auto join_map(const Map& items) -> std::string;
    /// @end join_map

    /// @block join_lines
    static auto join_lines(std::string_view line_ends, const view_provider_proc& proc) -> std::string;
    static auto join_lines(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_lines(std::string_view line_ends, const Sequence& items) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_lines(const Sequence& items) -> std::string;
    /// @end join

    /// @block join_path
    static auto join_path(std::string_view sep, const view_provider_proc& proc) -> std::string;
    static auto join_path(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_path(std::string_view sep, const Sequence& items) -> std::string;
    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_path(const Sequence& items) -> std::string;
    /// @end join

    /// @block join_searchpath
    static auto join_searchpath(std::string_view sep, const view_provider_proc& proc) -> std::string;
    static auto join_searchpath(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_searchpath(std::string_view sep, const Sequence& items) -> std::string;
    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_searchpath(const Sequence& items) -> std::string;
    /// @end join_searchpath

    /// @block split
    static auto split(std::string_view s, const substr_search_proc& search_proc, size_type max_n, const range_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const charset_type& sep_charset, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const charset_type& sep_charset, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const charset_type& sep_charset, size_type max_n = npos) -> std::vector<std::string>;
    static auto split(std::string_view s, std::string_view sep_str, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, std::string_view sep_str, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, std::string_view sep_str, size_type max_n = npos) -> std::vector<std::string>;
    static auto split(std::string_view s, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, size_type max_n = npos) -> std::vector<std::string>;
    static auto split_view(std::string_view s, const charset_type& sep_charset, size_type max_n = npos) -> std::vector<std::string_view>;
    static auto split_view(std::string_view s, std::string_view sep_str, size_type max_n = npos) -> std::vector<std::string_view>;
    static auto split_view(std::string_view s, size_type max_n = npos) -> std::vector<std::string_view>;
    /// @end split
#ifdef STR_UNIMPL
    static auto split(std::string_view s, const std::regex& sep_regex, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const std::regex& sep_regex, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const std::regex& sep_regex, size_type max_n = npos) -> std::vector<std::string>;
#endif // STR_UNIMPL

    /// @block split_list
    static auto split_list(std::string_view s, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split_list(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto split_list(std::string_view s, size_type max_n = npos) -> std::vector<std::string>;
    static auto split_list_view(std::string_view s, size_type max_n = npos) -> std::vector<std::string_view>;
    /// @end split_list

    /// @block split_pair
    static auto split_pair(std::string_view s, std::string_view sep = ":") -> pair<std::string>;
    static auto split_pair_view(std::string_view s, std::string_view sep = ":") -> pair<std::string_view>;
    /// @end split_pair

    /// @block split_map
    static auto split_map(std::string_view s, std::string_view sep_list, std::string_view sep_pair, const view_pair_consumer_proc& proc) -> void;
    static auto split_map(std::string_view s, std::string_view sep_list = ",", std::string_view sep_pair = ":", size_type max_n = npos) -> std::map<std::string, std::string>;
    /// @end split_map

    /// @block split_lines
    static auto split_lines(std::string_view s, bool keep_ends, const view_consumer_proc& proc) -> void;
    static auto split_lines(std::string_view s, bool keep_ends = false) -> std::vector<std::string>;
    static auto split_lines_view(std::string_view s, bool keep_ends = false) -> std::vector<std::string_view>;
    /// @end split_lines

    /// @block split_path
    static auto split_path(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto split_path(std::string_view s) -> std::vector<std::string>;
    static auto split_path_view(std::string_view s) -> std::vector<std::string_view>;
    /// @end split_path

    /// @block split_searchpath
    static auto split_searchpath(std::string_view s, bool keep_empty, value_type sep, const view_consumer_proc& proc) -> void;
    static auto split_searchpath(std::string_view s, bool keep_empty = false, value_type sep = ':') -> std::vector<std::string>;
    static auto split_searchpath_view(std::string_view s, bool keep_empty = false, value_type sep = ':') -> std::vector<std::string_view>;
    /// @end split_searchpath

#ifdef STR_UNIMPL
    // 拆分 csv 数据
    static auto split_csv(std::string_view s) -> std::vector<std::string>;
    static auto join_csv(view_provider_proc proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_csv(const Sequence& items) -> std::string {
    }

    // 按 properties 格式拼接
    static auto split_properties(std::string_view s) -> std::string_view;
    static auto join_properties(std::string& s, properties_sep sep) -> std::string&;
#endif // STR_UNIMPL

    /// @block partition
    static auto partition_range(std::string_view s, const charset_type& charset) -> ternary<range_type>;
    static auto partition_range(std::string_view s, const char_match_proc& proc) -> ternary<range_type>;
    static auto partition_range(std::string_view s, std::string_view sep) -> ternary<range_type>;
    /// -
    static auto partition_view(std::string_view s, const charset_type& charset) -> ternary<std::string_view>;
    static auto partition_view(std::string_view s, const char_match_proc& proc) -> ternary<std::string_view>;
    static auto partition_view(std::string_view s, std::string_view sep) -> ternary<std::string_view>;
    static auto partition_view(std::string_view s, const view_search_proc& proc) -> ternary<std::string_view>;
    /// -
    static auto partition(std::string_view s, const charset_type& charset) -> ternary<std::string>;
    static auto partition(std::string_view s, const char_match_proc& proc) -> ternary<std::string>;
    static auto partition(std::string_view s, std::string_view sep) -> ternary<std::string>;
    static auto partition(std::string_view s, const view_search_proc& proc) -> ternary<std::string>;
    /// @end partition
#ifdef STR_UNTESTED
    static auto partition_range(std::string_view s, const std::regex& pattern) -> ternary<range_type>;
    static auto partition_range(std::string_view s, const substr_search_proc& proc) -> ternary<range_type>;
    static auto partition_view(std::string_view s, const std::regex& pattern) -> ternary<std::string_view>;
    static auto partition(std::string_view s, const std::regex& pattern) -> ternary<std::string>;
#endif // STR_UNTESTED
    /// -
#ifdef STR_UNIMPL
    /// @ref{rpartition_view, rpartition} 从 s 右侧查找首个 sep 分隔符或者满足 proc 的字符的位置，并将字符串分
    /// 割为分隔符左侧，分隔符，分隔符右侧三个部分，并返回。
    static auto rpartition_view(std::string_view s, const charset_type& sep) -> ternary<std::string_view>;
    static auto rpartition_view(std::string_view s, const char_match_proc& sep) -> ternary<std::string_view>;
    static auto rpartition_view(std::string_view s, std::string_view sep) -> ternary<std::string_view>;
    static auto rpartition_view(std::string_view s, const std::regex& sep) -> ternary<std::string_view>;
    static auto rpartition_view(std::string_view s, const view_search_proc& sep) -> ternary<std::string_view>;
    //
    static auto rpartition(std::string_view s, const charset_type& sep) -> ternary<std::string>;
    static auto rpartition(std::string_view s, const char_match_proc& sep) -> ternary<std::string>;
    static auto rpartition(std::string_view s, std::string_view sep) -> ternary<std::string>;
    static auto rpartition(std::string_view s, const std::regex& sep) -> ternary<std::string>;
    static auto rpartition(std::string_view s, const view_search_proc& sep) -> ternary<std::string>;
#endif // STR_UNIMPL

    /// @block next_chunk
    static auto next_chunk_range(std::string_view s, size_type& pos, size_type max_n) -> std::optional<range_type>;
    static auto next_chunk_view(std::string_view s, size_type& pos, size_type max_n) -> std::optional<std::string_view>;
    static auto next_chunk(std::string_view s, size_type& pos, size_type max_n) -> std::optional<std::string>;
    /// @end next_chunk

    /// @block chunked
    static auto chunked(std::string_view s, size_type width, const view_consumer_proc& proc) -> void;
    static auto chunked(std::string_view s, size_type width) -> std::vector<std::string>;
    static auto chunked_view(std::string_view s, size_type width) -> std::vector<std::string_view>;
    /// @end chunked

    /// @block next_window
    static auto next_window_range(std::string_view s, size_type& pos, size_type max_n, size_type step) -> std::optional<range_type>;
    static auto next_window_view(std::string_view s, size_type& pos, size_type max_n, size_type step) -> std::optional<std::string_view>;
    static auto next_window(std::string_view s, size_type& pos, size_type max_n, size_type step) -> std::optional<std::string>;
    /// @end next_window

    /// @block windowed
    static auto windowed(std::string_view s, size_type width, size_type step, const view_consumer_proc& proc) -> void;
    static auto windowed(std::string_view s, size_type width, size_type step) -> std::vector<std::string>;
    static auto windowed_view(std::string_view s, size_type width, size_type step) -> std::vector<std::string_view>;
    /// @end windowed

    /// @block case_conversion
    static auto to_lower(std::string_view s) -> std::string;
    static auto to_upper(std::string_view s) -> std::string;
    static auto to_title(std::string_view s) -> std::string;
    static auto to_capitalize(std::string_view s) -> std::string;
    static auto swap_case(std::string_view s) -> std::string;
    //
    static auto to_lower_inplace(std::string& s) -> std::string&;
    static auto to_upper_inplace(std::string& s) -> std::string&;
    static auto to_title_inplace(std::string& s) -> std::string&;
    static auto to_capitalize_inplace(std::string& s) -> std::string&;
    static auto swap_case_inplace(std::string& s) -> std::string&;
    /// @end case_conversion

    //! 剔除 @anchor{trim}
    ///
    /// * @ref{trim_left, trim_left_inplace} 去掉 `s` 左侧的满足条件的字符
    /// * @ref{trim_right_view, trim_right, trim_right_inplace} 去掉 `s` 右侧的满足条件的字符
    /// * @ref{trim_surrounding_view, trim_surrounding, trim_surrounding_inplace} 去掉 `s` 两端的满足条件的字符
    /// * @ref{trim_anywhere, trim_anywhere_inplace} 去掉 `s` 中任何满足条件的满足条件的字符
    ///
    /// @notice{1} 对于未指定 `proc` 或者 `charset` 等定制条件的回到函数，表示去除空白字符。
    ///
    /// @param s: 从该字符串剔除自定的字符或者子串。
    /// @param proc: 字符匹配条件，所有满足条件的字符都将剔除。
    /// @param charset: 表示可以满足条件的字符集，`s` 中的任何字符集中的字符都将被剔除。
    /// @return 返回剔除指定字符后的剩余部分。
    /// @block trims
    static auto trim_left_view(std::string_view s, const char_match_proc& proc) -> std::string_view;
    static auto trim_left_view(std::string_view s) -> std::string_view;
    static auto trim_left_view(std::string_view s, const charset_type& charset) -> std::string_view;
    static auto trim_left_view(std::string_view s, std::string_view charset) -> std::string_view;
    static auto trim_right_view(std::string_view s, const char_match_proc& proc) -> std::string_view;
    static auto trim_right_view(std::string_view s, const charset_type& charset) -> std::string_view;
    static auto trim_right_view(std::string_view s, std::string_view charset) -> std::string_view;
    static auto trim_right_view(std::string_view s) -> std::string_view;
    static auto trim_surrounding_view(std::string_view s, const char_match_proc& proc) -> std::string_view;
    static auto trim_surrounding_view(std::string_view s, const charset_type& charset) -> std::string_view;
    static auto trim_surrounding_view(std::string_view s, std::string_view charset) -> std::string_view;
    static auto trim_surrounding_view(std::string_view s) -> std::string_view;
    /// -
    static auto trim_left(std::string_view s, const char_match_proc& proc) -> std::string;
    static auto trim_left(std::string_view s) -> std::string;
    static auto trim_left(std::string_view s, const charset_type& charset) -> std::string;
    static auto trim_left(std::string_view s, std::string_view charset) -> std::string;
    static auto trim_right(std::string_view s, const char_match_proc& proc) -> std::string;
    static auto trim_right(std::string_view s, const charset_type& charset) -> std::string;
    static auto trim_right(std::string_view s, std::string_view charset) -> std::string;
    static auto trim_right(std::string_view s) -> std::string;
    static auto trim_surrounding(std::string_view s, const char_match_proc& proc) -> std::string;
    static auto trim_surrounding(std::string_view s, const charset_type& charset) -> std::string;
    static auto trim_surrounding(std::string_view s, std::string_view charset) -> std::string;
    static auto trim_surrounding(std::string_view s) -> std::string;
    /// -
    static auto trim_left_inplace(std::string& s, const char_match_proc& proc) -> std::string&;
    static auto trim_left_inplace(std::string& s, const charset_type& charset) -> std::string&;
    static auto trim_left_inplace(std::string& s, std::string_view charset) -> std::string&;
    static auto trim_left_inplace(std::string& s) -> std::string&;
    static auto trim_right_inplace(std::string& s, const char_match_proc& proc) -> std::string&;
    static auto trim_right_inplace(std::string& s, const charset_type& charset) -> std::string&;
    static auto trim_right_inplace(std::string& s, std::string_view charset) -> std::string&;
    static auto trim_right_inplace(std::string& s) -> std::string&;
    static auto trim_surrounding_inplace(std::string& s, const char_match_proc& proc) -> std::string&;
    static auto trim_surrounding_inplace(std::string& s, const charset_type& charset) -> std::string&;
    static auto trim_surrounding_inplace(std::string& s, std::string_view charset) -> std::string&;
    static auto trim_surrounding_inplace(std::string& s) -> std::string&;
    /// -
    static auto trim_anywhere(std::string_view s, const char_match_proc& proc) -> std::string;
    static auto trim_anywhere(std::string_view s, const charset_type& charset) -> std::string;
    static auto trim_anywhere(std::string_view s, std::string_view charset) -> std::string;
    static auto trim_anywhere(std::string_view s, value_type charset) -> std::string;
    static auto trim_anywhere(std::string_view s) -> std::string;
    /// -
    static auto trim_anywhere_inplace(std::string& s, const char_match_proc& proc) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s, const charset_type& charset) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s, std::string_view charset) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s, value_type charset) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s) -> std::string&;
    /// @end trims

    /// @block simplified
    static auto simplified(std::string_view s, std::string_view sep, const char_match_proc& proc) -> std::string;
    static auto simplified(std::string_view s) -> std::string;
    static auto simplified_inplace(std::string& s, std::string_view sep, const char_match_proc& proc) -> std::string&;
    static auto simplified_inplace(std::string& s) -> std::string&;
    //
    static auto simplified_integer(std::string_view s) -> std::string;
    static auto simplified_integer_inplace(std::string& s) -> std::string&;
    /// @end simplified
    //
#ifdef STR_UNIMPL
    static auto simplified_real(std::string_view s) -> std::string;
    static auto simplified_real_inplace(std::string& s) -> std::string&;
#endif // STR_UNIMPL

#ifdef STR_UNTESTED
    //! 拷贝
    ///
    /// 将 s 中的数据拷贝到 `buffer` 中去，实际的字节数取决于 `s.size()` 与 `size` 的最小值
    ///
    /// @param buffer 缓冲区
    /// @param size 缓冲区尺寸
    /// @param s 原始字符串
    /// @return 返回实际拷贝的字节数
    static auto copy(pointer buffer, size_type size, std::string_view s) -> size_type;
#endif // STR_UNTESTED

    /// @block expand_envs
    static auto expand_envs(std::string_view s, bool keep_unexpanded, const string_mapping_proc& proc) -> std::string;
    static auto expand_envs(std::string_view s, bool keep_unexpanded = false) -> std::string;
    static auto expand_envs(std::string_view s, bool keep_unexpanded, const std::map<std::string, std::string>& kvs) -> std::string;
    static auto expand_envs(std::string_view s, const std::map<std::string, std::string>& kvs) -> std::string;
    static auto expand_envs(std::string_view s, std::string_view key, std::string_view val) -> std::string;
    //
    static auto expand_envs_inplace(std::string& s, bool keep_unexpanded, const string_mapping_proc& proc) -> std::string&;
    static auto expand_envs_inplace(std::string& s, bool keep_unexpanded = false) -> std::string&;
    static auto expand_envs_inplace(std::string& s, bool keep_unexpanded, const std::map<std::string, std::string>& kvs) -> std::string&;
    static auto expand_envs_inplace(std::string& s, const std::map<std::string, std::string>& kvs) -> std::string&;
    static auto expand_envs_inplace(std::string& s, std::string_view key, std::string_view val) -> std::string&;
    /// @end expand_envs

    /// @block expand_tabs
    static auto expand_tabs(std::string_view s, size_type tab_size = 8) -> std::string;
    static auto expand_tabs_inplace(std::string& s, size_type tab_size = 8) -> std::string&;
    /// @end expand_tabs

    /// @block expand_user
    static auto expand_user(std::string_view s) -> std::string;
    static auto expand_user_inplace(std::string& s) -> std::string&;
    /// @end expand_tabs

    /// @block normpath
    static auto normpath(std::string_view s) -> std::string;
    static auto normpath_inplace(std::string& s) -> std::string&;
    /// @end expand_user

#ifdef STR_UNIMPL
    //! 路径处理 @anchor{is_absolute, is_relative}
    ///
    /// 将字符串 s 视作文件路径，并检查其是否是绝对路径的形式或者相对路径的形式
    static auto is_absolute(std::string_view s) -> bool;
    static auto is_relative(std::string_view s) -> bool;
#endif // STR_UNIMPL

    /// @block path_pos
    static auto basename_pos(std::string_view s) -> size_type;
    static auto dirname_pos(std::string_view s) -> size_type;
    static auto extname_pos(std::string_view s) -> size_type;
    /// @end path_pos

    /// @block dirname
    static auto dirname_range(std::string_view s) -> range_type;
    static auto dirname_view(std::string_view s) -> std::string_view;
    static auto dirname(std::string_view s) -> std::string;
    static auto remove_dirname_view(std::string_view s) -> std::string_view;
    static auto remove_dirname(std::string_view s) -> std::string;
    static auto replace_dirname(std::string_view s, std::string_view new_dir) -> std::string;
    static auto split_dirname_view(std::string_view s) -> pair<std::string_view>;
    static auto split_dirname(std::string_view s) -> pair<std::string>;
    //
    static auto dirname_inplace(std::string& s) -> std::string&;
    /// @end dirname
#ifdef STR_UNTESTED
    static auto remove_dirname_inplace(std::string& s) -> std::string&;
    static auto replace_dirname_inplace(std::string& s, std::string_view new_name) -> std::string&;
#endif // STR_UNTESTED

    /// @block basename
    static auto basename_range(std::string_view s) -> range_type;
    static auto basename_view(std::string_view s) -> std::string_view;
    static auto basename(std::string_view s) -> std::string;
    static auto remove_basename_view(std::string_view s) -> std::string_view;
    static auto remove_basename(std::string_view s) -> std::string;
    static auto replace_basename(std::string_view s, std::string_view new_name) -> std::string;
    static auto split_basename_view(std::string_view s) -> pair<std::string_view>;
    static auto split_basename(std::string_view s) -> pair<std::string>;
    /// -
    static auto basename_inplace(std::string& s) -> std::string&;
    /// @end basename
#ifdef STR_UNTESTED
    static auto remove_basename_inplace(std::string& s) -> std::string&;
    static auto replace_basename_inplace(std::string& s, std::string_view new_name) -> std::string&;
#endif // STR_UNTESTED

    /// @block extname
    static auto extname_range(std::string_view s) -> range_type;
    static auto extname_view(std::string_view s) -> std::string_view;
    static auto extname(std::string_view s) -> std::string;
    /// @end basename
#ifdef STR_UNTESTED
    static auto remove_extname_view(std::string_view s) -> std::string_view;
    static auto remove_extname(std::string_view s) -> std::string;
    static auto replace_extname(std::string_view s, std::string_view new_name) -> std::string;
    static auto split_extname_view(std::string_view s) -> pair<std::string_view>;
    static auto split_extname(std::string_view s) -> pair<std::string>;
#endif // STR_UNTESTED
    //
#ifdef STR_UNTESTED
    static auto extname_inplace(std::string& s) -> std::string&;
    static auto remove_extname_inplace(std::string& s) -> std::string&;
    static auto replace_extname_inplace(std::string& s, std::string_view new_name) -> std::string&;
#endif // STR_UNTESTED

    /// @block rawname
    static auto rawname_range(std::string_view s) -> range_type;
    static auto rawname_view(std::string_view s) -> std::string_view;
    static auto rawname(std::string_view s) -> std::string;
    /// @end rawname
#ifdef STR_UNTESTED
    static auto replace_rawname(std::string_view s, std::string_view new_name) -> std::string;
    static auto split_rawname_view(std::string_view s) -> ternary<std::string_view>;
    static auto split_rawname(std::string_view s) -> ternary<std::string>;
    //
    static auto rawname_inplace(std::string& s) -> std::string&;
    static auto replace_rawname_inplace(std::string& s, std::string_view name) -> std::string&;
#endif // STR_UNTESTED

#ifdef STR_UNIMPL
    //! 哈希算法 @anchor{hash, md5}
    ///
    /// @ref{hash} 字符串到整数的 hash 算法
    /// @ref{md5} md5 摘要算法
    ///
    static auto hash(std::string_view s, uint32_t mod) -> uint32_t;
    static auto hash(std::string_view s, uint64_t mod) -> uint64_t;
    //
    static auto md5(std::string_view s) -> std::string;
    static auto md5(void* data, size_type n) -> std::string;
#endif // STR_UNIMPL

#ifdef STR_UNIMPL
    //! 转义：XML
    ///
    /// 将 s 中属于 xml 的特殊字符，转义为 xml 的表示形式，或者反过来
    ///
    /// @param s 被编码或者解码的字符串
    /// @param proc 用于接收转换后，生成的字符串
    /// @return 返回编码或者解码后的字符串
    static auto encode_xml(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto decode_xml(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto encode_xml(std::string_view s) -> std::string;
    static auto decode_xml(std::string_view s) -> std::string;
    //
    static auto encode_xml_inplace(std::string& s) -> std::string&;
    static auto decode_xml_inplace(std::string& s) -> std::string&;
#endif // STR_UNIMPL

#ifdef STR_UNIMPL
    //! 转义：URL
    ///
    /// 将 s 中属于 url 的特殊字符，转义为 url 中可表示的形式，或者反过来
    ///
    /// @param s: 被编码或者解码的字符串
    /// @param proc: 用于接收转换后，生成的字符串
    /// @return 返回编码或者解码后的字符串
    static auto encode_url(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto decode_url(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto encode_url(std::string_view s) -> std::string;
    static auto decode_url(std::string_view s) -> std::string;
    //
    static auto encode_url_inplace(std::string& s) -> std::string&;
    static auto decode_url_inplace(std::string& s) -> std::string&;
#endif // STR_UNIMPL

    /// @block encode_cstr
    static auto encode_cstr(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto encode_cstr(std::string_view s) -> std::string;
    static auto decode_cstr(std::string_view s, const view_consumer_proc& proc) -> size_type;
    static auto decode_cstr(std::string_view s) -> std::tuple<size_type, std::string>;
    /// -
    static auto encode_cstr_inplace(std::string& s) -> std::string;
    static auto decode_cstr_inplace(std::string& s) -> size_type;
    /// @end encode_cstr

    /// @block encode_base64
    static auto encode_base64(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto encode_base64(std::string_view s) -> std::string;
    static auto decode_base64(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto decode_base64(std::string_view s) -> std::string;
    //
    static auto encode_base64_inplace(std::string& s) -> std::string&;
    static auto decode_base64_inplace(std::string& s) -> std::string&;
    /// @end encode_base64

    /// @block encode_base16
    static auto encode_base16(std::string_view s, bool upper, const view_consumer_proc& proc) -> void;
    static auto encode_base16(std::string_view s, bool upper = false) -> std::string;
    static auto decode_base16(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto decode_base16(std::string_view s) -> std::string;
    /// -
    static auto encode_base16_inplace(std::string& s, bool upper = false) -> std::string&;
    static auto decode_base16_inplace(std::string& s) -> std::string&;
    /// @end encode_base16

    //! dump_hex 时的选项，可以微组合 @anchor{dump_hex_flags}
    enum dump_hex_flags : uint8_t {
        show_offset = 0x01, ///< 限制当前行的数据的偏移量
        show_ascii = 0x02,  ///< 显示 dump_hex 的 ASCII 码区域
        show_upper = 0x04,  ///< dump_hex 时，对于大于 9 的十六进制数字部分是否显示为大写
    };

    //! @ref{dump_hex} 时，用于控制输出数据格式的字段 @anchor{dump_hex_format}
    struct dump_hex_format {
        uint8_t flags{0};                     ///< 可选标记位
        uint8_t offset_width{0};              ///< shifter 的宽度
        uint8_t line_groups{4};               ///< 每行格式化多少个字节
        uint8_t group_bytes{4};               ///< 多少字节一组，如果大于 line_size，自动校正为 line_size
        value_type ascii_mask{'.'};           ///< 显示 ascii 时，对不打印字符显示的掩码字符
        std::string_view offset_margin{": "}; ///< 显示 offset 时，shifter 右侧与文本段的分隔符
        std::string_view ascii_margin{" "};   ///< 显示 ascii 时，在此之前显示的 margin 字符
    };

    /// @block dump_hex
    static auto dump_hex(const void* data, size_type len, const dump_hex_format& format, const line_consumer_proc& proc) -> void;
    /// @end dump_hex

    /// @block sum
    template <typename T>
    static auto sum(std::string_view s, const mapping_proc<T>& proc) -> T;
    /// @end sum

    /// @block charset
    static auto charset(std::string_view s) -> charset_type;
    static auto charset() -> charset_type;
    /// @end charset

    /// @block range_interval_shifter
    static auto range(size_type pos, size_type n) -> range_type;
    static auto range() -> range_type;
    static auto range(std::string_view s, size_type pos = 0) -> range_type;
    static auto interval(size_type begin, size_type end) -> interval_type;
    static auto shifter(size_type pos, ssize_type offset) -> shifter_type;
    /// @end range_interval_shifter

    /// @block read_next_line
    static auto read_next_line(FILE* file, bool keep_ends, std::string& line_text) -> bool;
    static auto read_next_line(FILE* file, bool keep_ends = false) -> std::optional<std::string>;
    static auto read_next_line(std::istream& file, std::string& line_text) -> bool;
    static auto read_next_line(std::istream& file) -> std::optional<std::string>;
    /// @end read_next_line

    /// @block read_all
    static auto read_all(const std::string& filename) -> std::string;
    static auto read_all(const char* filename) -> std::string;
    /// @end read_all


    /// @block read_lines
    static auto read_lines(FILE* file, bool keep_ends, const line_consumer_proc& proc) -> void;
    static auto read_lines(FILE* file, bool keep_ends = false, size_type max_n = npos) -> std::vector<std::string>;
    static auto read_lines(std::istream& file, const line_consumer_proc& proc) -> void;
    static auto read_lines(std::istream& file, size_type max_n = npos) -> std::vector<std::string>;
    /// -
    static auto read_lines(const std::string& filename, bool keep_ends, const line_consumer_proc& proc) -> void;
    static auto read_lines(const char* filename, bool keep_ends, const line_consumer_proc& proc) -> void;
    static auto read_lines(const std::string& filename, bool keep_ends = false, size_type max_n = npos) -> std::vector<std::string>;
    static auto read_lines(const char* filename, bool keep_ends = false, size_type max_n = npos) -> std::vector<std::string>;
    /// @end read_lines

    /// @block with_file
    static auto with_file(const std::string& filepath, const char* mode, FILE* repl, const std::function<void(FILE* f)>& proc) -> void;
    static auto with_file(const std::string& filepath, const char* mode, const std::function<void(FILE* f)>& proc) -> void;
    /// @end with_file

    /// @block next_opt1
    static auto next_opt1(int& next_index, int argc, const char* argv[]) -> std::optional<pair<std::string_view>>;
    static auto next_opt1(int& next_index, int argc, char* argv[]) -> std::optional<pair<std::string_view>>;
    template <typename Container, typename SizeType = typename Container::size_type>
    static auto next_opt1(SizeType& next_index, const Container& items) -> std::optional<pair<std::string_view>>;
    template <typename Iterator>
    static auto next_opt1(Iterator& itr, Iterator end) -> std::optional<pair<std::string_view>>;
    template <typename IterProc>
    static auto next_opt1(const IterProc& proc) -> std::optional<pair<std::string_view>>;
    /// @end next_opt1


    /// @block next_opt2
    static auto next_opt2(int& next_index, int argc, const char* argv[]) -> std::optional<pair<std::string_view>>;
    static auto next_opt2(int& next_index, int argc, char* argv[]) -> std::optional<pair<std::string_view>>;
    template <typename Container, typename SizeType = typename Container::size_type>
    static auto next_opt2(SizeType& next_index, const Container& items) -> std::optional<pair<std::string_view>>;
    template <typename Iterator>
    static auto next_opt2(Iterator& itr, Iterator end) -> std::optional<pair<std::string_view>>;
    /// @end next_opt2

    /// @block accept
    static auto accept_until(std::string_view s, size_type& pos, value_type guard_ch) -> std::optional<range_type>;
    static auto accept_until(std::string_view s, size_type& pos, value_type escape, value_type guard_ch) -> std::optional<range_type>;
    static auto accept_until(std::string_view s, size_type& pos, std::string_view guard_token) -> std::optional<range_type>;
    static auto accept_until(std::string_view s, size_type& pos, const std::regex& guard_pattern) -> std::optional<range_type>;
    static auto accept_until(std::string_view s, size_type& pos, const charset_type& guard_charset) -> std::optional<range_type>;
    static auto accept_until(std::string_view s, size_type& pos, value_type escape, const charset_type& guard_charset) -> std::optional<range_type>;
    static auto accept_until(std::string_view s, size_type& pos, const char_match_proc& guard_proc) -> std::optional<range_type>;
    static auto accept_until(std::string_view s, size_type& pos, value_type escape, const char_match_proc& guard_proc) -> std::optional<range_type>;
    /// -
    static auto accept(std::string_view s, size_type& pos, value_type expect_ch) -> std::optional<range_type>;
    static auto accept(std::string_view s, size_type& pos, std::string_view expect_token) -> std::optional<range_type>;
    static auto accept(std::string_view s, size_type& pos, const std::regex& expect_pattern) -> std::optional<range_type>;
    static auto accept(std::string_view s, size_type& pos, const charset_type& expect_charset) -> std::optional<range_type>;
    static auto accept(std::string_view s, size_type& pos, const char_match_proc& expect_proc) -> std::optional<range_type>;
    /// @end accept

    /// @block skip
    static auto skip_n(std::string_view s, size_type& pos, size_type n) -> bool;
    static auto skip_max(std::string_view s, size_type& pos, size_type max_n) -> std::optional<size_type>;
    static auto skip_spaces(std::string_view s, size_type& pos) -> void;
    static auto skip_spaces(std::string_view s, size_type& pos, size_type min_n) -> bool;
    /// @end skip

#ifdef STR_UNIMPL
    static auto accept_literal_integer(std::string_view s, size_type& pos) -> range_type;
    static auto accept_literal_real(std::string_view s, size_type& pos) -> range_type;
    static auto accept_literal_string(std::string_view s, size_type& pos) -> range_type;
    static auto accept_identifier(std::string_view s, size_type& pos) -> range_type;
    static auto accept_next_word(std::string_view s, size_type& pos) -> range_type;
    static auto accept_email(std::string_view s, size_type& pos) -> range_type;
    //
    static auto accept_until(std::string_view s, size_type& pos, std::string_view sep_str) -> range_type;
    static auto accept_until(std::string_view s, size_type& pos, const charset_type& sep_charset) -> range_type;
    static auto accept_until(std::string_view s, size_type& pos, const char_match_proc& proc) -> range_type;
    //
    static auto accept(std::string_view s, size_type& pos, std::string_view sep_str) -> range_type;
    static auto accept(std::string_view s, size_type& pos, const charset_type& sep_charset) -> range_type;
    static auto accept(std::string_view s, size_type& pos, const char_match_proc& proc) -> range_type;
#endif // STR_UNIMPL

#ifdef STR_UNIMPL
    //! 符号分割
    ///
    /// 将 s 视作特定格式的数据，将字符串拆分成多个部分
    ///
    /// @ref{split_email, split_email_view} 拆分 email
    /// @ref{split_real, split_real_view} 拆分
    /// @ref{split_uri, split_uri_view} 拆分 uri
    /// @ref{split_ipv4, split_ipv4_view} 拆分 uri
    ///
    /// @param s 待拆分字符串
    /// @param proc 用于接收输出
    /// @return 返回拆分后的字符串
    static auto split_email(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto split_email(std::string_view s) -> std::vector<std::string>;
    static auto split_email_view(std::string_view s) -> std::vector<std::string_view>;
    //
    static auto split_real(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto split_real(std::string_view s) -> std::vector<std::string>;
    static auto split_real_view(std::string_view s) -> std::vector<std::string_view>;
    //
    static auto split_uri(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto split_uri(std::string_view s) -> std::vector<std::string>;
    static auto split_uri_view(std::string_view s) -> std::vector<std::string_view>;
    //
    static auto split_ipv4(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto split_ipv4(std::string_view s) -> std::vector<std::string>;
    static auto split_ipv4_view(std::string_view s) -> std::vector<std::string_view>;
#endif // STR_UNIMPL

#ifdef STR_UNTESTED
    //! 字符筛选和分组
    ///
    /// 根据制定的条件对字符串中的字符进行分组或者筛选
    ///
    /// @ref{grouping} 按 proc 将字符序列分成两组，左边的满足 proc 的字符组成的新串，右边不满足 proc 的字符组成的新串
    /// @ref{filter, filter_inplace} 筛选出输入字符串 s 中的满足 proc 条件的字符，并组合成新串返回
    ///
    /// @param s 被用来筛选或者分组的字符串
    /// @param proc 字符匹配条件
    static auto grouping(std::string_view s, const char_match_proc& proc) -> std::tuple<std::string, std::string>;
    static auto filter(std::string_view s, const char_match_proc& proc) -> std::string;
    static auto filter(std::string_view s, const charset_type& charset) -> std::string;
    static auto filter_inplace(std::string& s, const char_match_proc& proc) -> std::string&;
    static auto filter_inplace(std::string& s, const charset_type& charset) -> std::string&;
#endif // STR_UNTESTED

#ifdef STR_UNTESTED
    //! 映射
    ///
    /// 将字符串 s 中在 source 中的字符映射到 target 的对应位置的字符中去。
    ///
    /// @param s 被处理的字符串
    /// @param match_charset 匹配字符集
    /// @param replace_charset 替换字符集
    /// @param proc 将匹配的字符映射为其他字符集
    static auto mapping(std::string_view s, std::string_view match_charset, std::string_view replace_charset) -> std::string;
    static auto mapping(std::string_view s, const char_mapping_proc& proc) -> std::string;
    static auto mapping_inplace(std::string& s, std::string_view match_charset, std::string_view replace_charset) -> std::string&;
    static auto mapping_inplace(std::string& s, const char_mapping_proc& proc) -> std::string&;
#endif // STR_UNTESTED

    /// @block home
    static auto home() -> std::string;
    /// @end home
};

template <typename Sequence, typename>
auto str::append(std::string_view s, const Sequence& items) -> std::string {
    auto itr = items.begin();
    return append(s, [&items, &itr]() -> std::optional<std::string_view> {
        if (itr == items.end()) {
            return std::nullopt;
        }

        return *(itr++);
    });
}

template <typename Sequence, typename>
auto str::append_inplace(std::string& s, const Sequence& items) -> std::string& {
    auto itr = items.begin();
    s = append(s, [&items, &itr]() -> std::optional<std::string_view> {
        if (itr == items.end()) {
            return std::nullopt;
        }

        return *(itr++);
    });
    return s;
}

template <typename Sequence, typename>
auto str::prepend(std::string_view s, const Sequence& items) -> std::string {
    auto itr = items.begin();
    return prepend(s, [&items, &itr]() -> std::optional<std::string_view> {
        if (itr == items.end()) {
            return std::nullopt;
        }

        return *(itr++);
    });
}

template <typename Sequence, typename>
auto str::prepend_inplace(std::string& s, const Sequence& items) -> std::string& {
    auto itr = items.begin();
    return prepend_inplace(s, [&items, &itr]() -> std::optional<std::string_view> {
        if (itr == items.end()) {
            return std::nullopt;
        }

        return *(itr++);
    });
}

#ifdef STR_UNIMPL
template <typename Sequence, typename>
auto str::insert(std::string& s, size_type pos, const Sequence& items) -> std::string& {
    auto itr = items.begin();
    return insert(s, pos, [&items, &itr]() -> std::optional<std::string_view> {
        if (itr == items.end()) {
            return std::nullopt;
        }

        return *(itr++);
    });
}
#endif // STR_UNIMPL

#ifdef STR_UNIMPL
template <typename Sequence, typename>
auto str::insert_inplace(std::string& s, size_type pos, const Sequence& items) -> std::string& {
    auto itr = items.begin();
    return insert_inplace(s, pos, [&items, &itr]() -> std::optional<std::string_view> {
        if (itr == items.end()) {
            return std::nullopt;
        }

        return *(itr++);
    });
}
#endif // STR_UNIMPL

#ifdef STR_UNTESTED
template <typename RangeSearchProc, typename>
auto str::next_proc_range(std::string_view s, size_type& pos, const RangeSearchProc& proc) -> range_type {
    if (pos >= s.size()) {
        pos = s.size();
        return range_type{};
    }

    auto range = proc(s, range_type{pos, (s.size() - pos + 1)});
    if (range.empty()) {
        pos = s.size();
        return range_type{};
    }

    pos = range.end_pos();
    return range;
}

template <typename RangeSearchProc, typename>
auto str::next_proc_view(std::string_view s, size_type& pos, const RangeSearchProc& proc) -> std::string_view {
    auto range = next_proc_range(s, pos, proc);
    if (range.empty()) {
        return {};
    }

    return {s.data() + range.pos, range.len};
}

template <typename RangeSearchProc, typename>
auto str::next_proc(std::string_view s, size_type& pos, const RangeSearchProc& proc) -> std::string {
    return std::string{next_proc_view(s, pos, proc)};
}
#endif // STR_UNTESTED

template <typename CharMatchProc, typename>
auto str::is_all_in(std::string_view s, const CharMatchProc& proc) -> bool {
    for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ptr++) {
        if (!proc(*ptr)) {
            return false;
        }
    }
    return true;
}

template <typename CharMatchProc, typename>
auto str::has_any_one(std::string_view s, const CharMatchProc& proc) -> bool {
    for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ptr++) {
        if (proc(*ptr)) {
            return true;
        }
    }
    return false;
}

template <typename Sequence, typename>
auto str::join(std::string_view s, const Sequence& items) -> std::string {
    std::string result;
    auto itr = items.begin();
    return join(s, [&items, &itr]() -> std::optional<std::string_view> {
        if (itr == items.end()) {
            return std::nullopt;
        }

        return *(itr++);
    });
}

template <typename Sequence, typename>
auto str::join_list(const Sequence& items) -> std::string {
    return join(",", items);
}

template <typename Map, typename>
auto str::join_map(std::string_view sep_pair, std::string_view sep_list, const Map& items) -> std::string {
    auto itr = items.cbegin();
    return str::join_map(sep_pair, sep_list,
        [&itr, end = items.cend()]() -> std::optional<std::tuple<std::string_view, std::string_view>> {
            if (itr == end) {
                return std::nullopt;
            }

            auto& [key, val] = *(itr++);
            return std::tuple{key, val};
        });
}

template <typename Map, typename>
auto str::join_map(const Map& items) -> std::string {
    return str::join_map("=", ",", items);
}

template <typename Sequence, typename>
auto str::join_lines(std::string_view line_ends, const Sequence& items) -> std::string {
    auto itr = items.begin();
    if (itr == items.end()) {
        return {};
    }

    return join_lines(line_ends, [&itr, &items]() -> std::optional<std::string_view> {
        if (itr == items.end()) {
            return std::nullopt;
        }

        return *(itr++);
    });
}

template <typename Sequence, typename>
auto str::join_lines(const Sequence& items) -> std::string {
    return join_lines(sep_line_ends, items);
}

template <typename Sequence, typename>
auto str::join_path(std::string_view sep, const Sequence& items) -> std::string {
    auto itr = items.begin();
    return str::join_path(sep, [end = items.end(), &itr]() -> std::optional<std::string_view> {
        if (itr == end) {
            return std::nullopt;
        }

        return *(itr++);
    });
}

template <typename Sequence, typename>
auto str::join_path(const Sequence& items) -> std::string {
    return str::join_path(sep_path, items);
}

template <typename Sequence, typename>
auto str::join_searchpath(std::string_view sep, const Sequence& items) -> std::string {
    auto itr = items.begin();
    return str::join_searchpath(sep, [end = items.end(), &itr]() -> std::optional<std::string_view> {
        if (itr == end) {
            return std::nullopt;
        }

        return *(itr++);
    });
}

template <typename Sequence, typename>
auto str::join_searchpath(const Sequence& items) -> std::string {
    return join_searchpath(":", items);
}

template <typename T>
auto str::sum(std::string_view s, const mapping_proc<T>& proc) -> T {
    T result = 0;
    for (const_pointer ptr = s.data(); ptr < (s.data() + s.size()); ptr++) {
        result += proc(*ptr);
    }
    return result;
}

template <typename Container, typename SizeType>
auto str::next_opt1(SizeType& next_index, const Container& items) -> std::optional<pair<std::string_view>> {
    return next_opt1([&next_index, &items]() -> std::optional<std::string_view> {
        if constexpr (std::is_signed<SizeType>::value) {
            if (next_index < 0) {
                next_index = 0;
            }

            if (items.size() <= 0) {
                next_index = 0;
                return std::nullopt;
            }
        }

        if (next_index >= items.size()) {
            next_index = items.size();
            return std::nullopt;
        }

        return items[next_index++];
    });
}

template <typename Iterator>
auto str::next_opt1(Iterator& itr, Iterator end) -> std::optional<pair<std::string_view>> {
    return next_opt1([&itr, &end]() -> std::optional<std::string_view> {
        if (itr == end) {
            return std::nullopt;
        }

        return *(itr++);
    });
}

template <typename IterProc>
auto str::next_opt1(const IterProc& proc) -> std::optional<pair<std::string_view>> {
    std::optional<std::string_view> item_opt = proc();
    if (!item_opt) {
        return std::nullopt;
    }

    std::string_view item = item_opt.value();
    if (item.empty()) {
        return str::pair<std::string_view>{std::string_view{}, item};
    }

    if (item[0] != '-') {
        return str::pair<std::string_view>{std::string_view{}, item};
    }

    if (item == std::string_view{"--"}) {
        item_opt = proc();
        if (!item_opt) {
            return str::pair<std::string_view>{std::string_view{}, std::string_view{}};
        }

        return str::pair<std::string_view>{std::string_view{}, item_opt.value()};
    }

    auto pos = item.find('=', 0);
    if (pos == std::string_view::npos) {
        return str::pair<std::string_view>{item, std::string_view{}};
    }

    return str::pair<std::string_view>{item.substr(0, pos), item.substr(pos + 1)};
}

template <typename Container, typename SizeType>
auto str::next_opt2(SizeType& next_index, const Container& items) -> std::optional<pair<std::string_view>> {
    if constexpr (std::is_signed<SizeType>::value) {
        if (next_index < 0) {
            next_index = 0;
        }

        if (items.size() <= 0) {
            next_index = 0;
            return std::nullopt;
        }
    }

    if (next_index >= static_cast<SizeType>(items.size())) {
        next_index = static_cast<SizeType>(items.size());
        return std::nullopt;
    }

    auto itr = items.begin();
    std::advance(itr, next_index);

    auto result = next_opt2(itr, items.end());
    next_index = static_cast<SizeType>(std::distance(items.begin(), itr));
    return result;
}

template <typename Iterator>
auto str::next_opt2(Iterator& itr, Iterator end) -> std::optional<pair<std::string_view>> {
    if (itr == end) {
        return std::nullopt;
    }

    std::string_view curr = *(itr++);

    // ""
    if (curr.empty()) {
        return pair<std::string_view>{std::string_view{}, curr};
    }

    // value
    if (!str::starts_with(curr, std::string_view{"-"})) {
        return pair<std::string_view>{std::string_view{}, curr};
    }

    // -- value
    if (curr == std::string_view{"--"}) {
        // -- EOL
        if (itr == end) {
            return pair<std::string_view>{std::string_view{}, std::string_view{}};
        }

        // -- value
        return pair<std::string_view>{std::string_view{}, *(itr++)};
    }

    // -key EOL
    if (itr == end) {
        return pair<std::string_view>{std::string_view{curr}, std::string_view{}};
    }

    // -key value
    if (!str::starts_with(*itr, std::string_view{"-"})) {
        return pair<std::string_view>{curr, *(itr++)};
    }

    // -key -
    if (*itr == std::string_view{"-"}) {
        itr++;
        return pair<std::string_view>{std::string_view{curr}, std::string_view{}};
    }

    if (*itr == std::string_view{"--"}) {
        itr++;

        // -key -- $
        if (itr == end) {
            return pair<std::string_view>{std::string_view{curr}, std::string_view{}};
        }

        // -key -- value
        return pair<std::string_view>{curr, *(itr++)};
    }

    // -key -key2
    return pair<std::string_view>{curr, std::string_view{}};
}

#if defined(STR_NAMESPACE)
};
#endif

#endif // TINY_STR_H
