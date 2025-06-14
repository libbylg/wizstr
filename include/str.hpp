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

//! # 简介
///
/// str 库提供了一系列字符串处理函数算法，目标是成为 C++ 语言功能最丰富的字符串处理函数库。
///
/// %# 提供了下列算法：
///
/// * 追加插入（@{#append}、@{#prepend} 和 @{#insert} 系列）
/// * 大小写不敏感的比较（@{#icompare} 和 @{#iequals} 系列）
/// * 基于通配符匹配（@{#wildcmp} 系列）
/// * 两字符串之间的关系（@{#contains} 系列）
/// * 特征字符串统计（@{#count} 系列）
/// * 前后缀操作（@{#prefix} 和 @{#suffix} 系列）
/// * 查找（@{#find} 和 @{#iter} 系列）
/// * 特征测试（@{#is}、@{#has} 系列）
/// * 子串提取（@{#take}、@{#drop} 系列）
/// * 修剪和整形（@{#align}、@{#surround}、@{#unsurround}、@{#invert}、@{#simplified}、@{#trim} 系列）
/// * 按多行处理（@{#lines} 系列）
/// * 按单词处理（@{#words} 系列）
/// * 字符串生成（@{#repeat} 系列）
/// * 空白串处理（@{#spaces} 系列）
/// * 字符串遮罩（@{#cover} 系列）
/// * 字符串拆分（@{#split}、@{#partition}、@{#chunked}、@{#windowed} 系列）
/// * 字符串拼接（@{#join} 系列）
/// * 大小写转换（@{#to} 系列）
/// * 变量或者特殊符号展开（@{#expand}）
/// * 文件名路径操作（@{#basemame}、@{#extname}、@{#dirname}、@{#rawname} 系列）
/// * 字符串哈希算法（@{#hash} 系列）
/// * 字符串转义（encode、decode 系列）
/// * 文本文件读取（@{#read} 系列）
/// * 字符分组和筛选（@{#grouping} 和 @{#filter} 系列）
///
/// %# 关于函数的返回值及其使用注意事项：
///
/// str 中提供的函数根据返回值的不同可以分为三种不同的形式，使用者需要根据情况合理地选择。
///
/// * `xxx_view` 形式：
///
///     通常意味着该函数的返回值是输入参数的一个（或多个）视图，该函数不会发生任何分配行为（返回存放
///     容器的 `std::string_view`，如 `std::vector<std::string_view>` 类似的除外）。但这种形式的接口
///     是**不安全**的时也需要格外注意，其返回值可能会因为输入参数提前析构，导致失效。所以在调用这些
///     接口时，需要确保在使用前其输入参数的地址仍然是有效的。
///
/// * `xxx_inplace` 形式：
///
///     这类函数通常意味着该函数返回的是输入参数自身，返回值也通常是 `std::string&`。该函数在执行
///     过程中通常会修改输入参数，并返回。如果使用这类函数，需要确保原始输入串是可以被改写的，否则
///     建议使用 `xxx_view 形式`或者 `xxx 形式` 的函数代替。
///
/// * `xxx_range` 形式：
///
///     这类函数返回的并不是某种形式的子串，而是子串在原始串中的范围，在子串定位场景很常见。
///
/// * `xxx` 形式：
///
///     与前面几种对应，这类不带 `_view` 或者 `_inplace` 后缀的函数，其返回值不是原始输入的视图，而是一个新的字符串拷贝。
///     因此，这类函数既没有类似 `_view` 系列函数那样的返回值依赖于输入参数的生存期的问题，也没有类似 `xxx_inplace` 那样会修改
///     原始输入参数的问题。但这类函数由于总是会拷贝原始输入字符串的，所以如果返回的字符串无法充分利用字符串的 SSO 特性，
///     那么性能会比 `xxx_view` 和 `xxx_inplace` 系列要低一些。当然这类函数的优点也是显而易见的，就是更`安全`。
struct str {
    using size_type = std::string::size_type;
    using ssize_type = ssize_t;
    using value_type = std::string::value_type;
    using pointer = std::string::pointer;
    using const_pointer = std::string::const_pointer;

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

        inline auto operator |(const charset_type& b) -> charset_type {
            charset_type result;
            result.bits_[0] = bits_[0] | b.bits_[0];
            result.bits_[1] = bits_[1] | b.bits_[1];
            result.bits_[2] = bits_[2] | b.bits_[2];
            result.bits_[3] = bits_[3] | b.bits_[3];
            return result;
        }

        inline auto operator |=(const charset_type& b) -> charset_type& {
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

        inline auto begin_pos() const -> size_type {
            return pos;
        }

        inline auto end() const -> size_type {
            size_type result{};
            if (__builtin_add_overflow(pos, len, &result)) {
                return npos;
            }
            return result;
        }

        inline auto end_pos() const -> size_type {
            return end();
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
    using mapping_proc = std::function<auto(value_type) -> MappedType>;

    //! 单字符映射器：将一个字符映射为另一个字符
    using char_mapping_proc = mapping_proc<value_type>;

    //! 字符串映射：将一个字符串映射为另一个字符串，如果能成功映射返回映射后的字符串，否则返回 std::nullopt
    using string_mapping_proc = std::function<std::optional<std::string>(const std::string& key)>;

    //! 用于表示无效位置的值，等价于 std::string::npos 或者 std::string_view::npos
    static inline constexpr size_type npos = std::string::npos;

    //! 定义了几个操作系统强相关的常量
    ///
    /// * @ref{sep_searchpath, sep_searchpath_char} 搜索路径分隔符
    /// * @ref{sep_path, sep_path_char} 文件路径分隔符
    /// * @ref{sep_line_ends} 行结束符
    static constexpr std::string_view sep_searchpath = ":";
    static constexpr value_type sep_searchpath_char = ':';
    static constexpr std::string_view sep_path = "/";
    static constexpr value_type sep_path_char = '/';
    static constexpr std::string_view sep_line_ends = "\n";

    //! 字符分类
    ///
    /// * @ref{all_uppers} 所有大写字母集合
    /// * @ref{all_lowers} 所有小写字母集合
    /// * @ref{all_leters} 所有字母集合
    /// * @ref{all_alphas} 所有字母集合
    /// * @ref{all_digits} 所有数字字符
    /// * @ref{all_xdigits} 所有十六进制数字表示的字符集合
    /// * @ref{all_alnums} 所有的字母和数字集合
    /// * @ref{all_alnumuls} 所有的字母、数字、下划线的集合
    /// * @ref{all_aluls} 所有字母和下滑线的集合
    /// * @ref{all_spaces} 所有空白字符
    /// * @ref{all_hex_upper} 所有大写字母形式的十六进制字符集
    /// * @ref{all_hex_lower} 所有小写字母形式的十六进制字符集
    /// * @ref{all_puncts} 所有标点符号
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

    //! 在尾部追加 @anchor{append}
    ///
    /// 提供了向指定字符尾部追加一个或者多个字符串的能力。实际上，STL 中已经提供了比较丰富的追加字符串，这里针对
    /// 大量字符串拼接提供了相对简便的方法。
    ///
    /// @notice{1} 对于 @ref{append_inplace} 函数，如果 s 与 被插入字符串存在重叠时，函数的行为是不确定的，应该避免出现这种
    /// 情况。
    ///
    /// @param s: 指定向哪个字符串后添加新串。
    /// @param other: 被追加的字符串。
    /// @param times_n: 重复追加多少次，如果指定为 0，则实际不会做任何追加操作。
    /// @param proc: 由 proc 函数提供被追加的字符串，如果 proc 返回 std::nullopt，表示后续无更多字符串需要追加。
    /// @param items: 从容器 items 中获取被追加的字符串。
    static auto append(std::string_view s, std::string_view other, size_type times_n = 1) -> std::string;
    static auto append(std::string_view s, const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto append(std::string_view s, const Sequence& items) -> std::string;
    /// -
    static auto append_inplace(std::string& s, std::string_view other, size_type times_n = 1) -> std::string&;
    static auto append_inplace(std::string& s, const view_provider_proc& proc) -> std::string&;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto append_inplace(std::string& s, const Sequence& items) -> std::string&;

    //! 向头部追加 @anchor{prepend}
    ///
    /// 將一个或者多个字符串追加到指定字符串的前面。实际上，STL 中已经提供了比较丰富的字符串插入函数，这里针对
    /// 大量字符串拼接提供了相对简便的方法。需要注意，对于通过 proc 和 items 来提供被追加串的函数，字符串总是以倒
    /// 序的方式被追加。比如，`str::prepend("abc", {"123", "456", "789"})` 返回的结果是 "789456123abc"。
    /// 对于 prepend_inplace 函数，如果 s 与 被插入字符串存在重叠时，函数的行为是不确定的，应该避免出现这种情况。
    ///
    /// @param s: 所有字符串都追加到该字符串之前
    /// @param other: 被追加的字符串
    /// @param times_n: 重复追加多少次，如果指定为 0，则实际不会做任何追加操作。
    /// @param proc: 由 proc 函数提供被追加的字符串，如果 proc 返回 std::nullopt，表示后续无更多字符串需要追加。
    /// @param items: 从容器 items 中获取被追加的字符串。
    static auto prepend(std::string_view s, std::string_view other, size_type times_n = 1) -> std::string;
    static auto prepend(std::string_view s, const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto prepend(std::string_view s, const Sequence& items) -> std::string;
    /// -
    static auto prepend_inplace(std::string& s, std::string_view other, size_type times_n = 1) -> std::string&;
    static auto prepend_inplace(std::string& s, const view_provider_proc& proc) -> std::string&;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto prepend_inplace(std::string& s, const Sequence& items) -> std::string&;

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

    //! 不区分大小写的比较 @anchor{icompare}
    ///
    /// @ref{icompare} 提供了不区分大小写比较的能力，其中 max_n 用于限制最多比较字符数量。特别的，如果 max_n 等于 0，
    /// 返回 0；
    ///
    /// @param s: 参与比较的字符串
    /// @param other: 另一个参与比较的字符串
    /// @param max_n: 表示最多比较前 max_n 个字符
    /// @return 返回正数，表示 s 大于 other；返回负值，表示 s 小于 other；返回 0，表示 s 和 other 相等。
    static auto icompare(std::string_view s, std::string_view other) -> int;
    static auto icompare(std::string_view s, std::string_view other, size_type max_n) -> int;

    //! 不区分大小写的相等測試 @anchor{iequals}
    ///
    /// @ref{iequals} 提供了不区分大小写的相等比较，其中 `max_n` 用于限制最多比较字符数量。特别的，如果
    /// `max_n` 等于 0。
    ///
    /// @param s: 参与比较的字符串
    /// @param other: 另一个参与比较的字符串
    /// @param max_n: 表示最多比较前 max_n 个字符
    /// @return 如果相等，返回 true，否则返回 false
    static auto iequals(std::string_view s, std::string_view other) -> bool;
    static auto iequals(std::string_view s, std::string_view other, size_type max_n) -> bool;

    //! 基于通配符的匹配检测 @anchor{wildcmp, iwildcmp}
    ///
    /// 测试字符串 s 是否匹配通配符 pattern，@ref{wildcmp} 表示区分大小写，@ref{iwildcmp} 表示不区分大小写。
    ///
    /// @param s: 被测试的字符串。
    /// @param pattern: 通配串。
    /// @return 如果 s 字符串匹配 pattern，返回 true，否则返回 false。
    static auto wildcmp(const_pointer s, const_pointer pattern) -> bool;
    static auto wildcmp(std::string_view s, std::string_view pattern) -> bool;
    /// -
    static auto iwildcmp(const_pointer s, const_pointer pattern) -> bool;
    static auto iwildcmp(std::string_view s, std::string_view pattern) -> bool;

    //! 判断两个字符串的包含关系 @anchor{contains}
    ///
    /// 等价于在字符串 s 中查找是否存在指定的字符或者字符串。
    ///
    /// @param s: 在该字符串查找目标子串
    /// @param other: 被查找的目标子串
    /// @param ch: 在 s 中查找是否存在指定的字符
    /// @param proc: s 中的每个字符都会触发 proc 函数，proc 返回 true，表示当前字符是正在被查找的字符；
    /// @param ignore_case: 指定是否采用不区分大小写的方式来查找子串
    /// @param charset: 指定一个字符集，s 中只要有任意一个字符在 charset 中就表示 s 中包含 charset
    /// @param pattern: 指定一个正则表达式，只要 s 中有任意子串匹配 pattern，表示 s 中包含 pattern
    /// @return 如果 s 包含指定的字符或者字符串或者某种模式，返回 true，否则返回 false。
    static auto contains(std::string_view s, std::string_view other) -> bool;
    static auto contains(std::string_view s, value_type ch) -> bool;
    static auto contains(std::string_view s, const char_match_proc& proc) -> bool;
    static auto contains(std::string_view s, const charset_type& charset) -> bool;
    static auto contains(std::string_view s, const std::regex& pattern) -> bool;
    /// -
    static auto icontains(std::string_view s, std::string_view other) -> bool;
    static auto icontains(std::string_view s, value_type ch) -> bool;

    //! 子串统计 @anchor{count}
    ///
    /// 本函数用于统计 s 串中是否包含特定模式的子串的数量。需要注意，count 函数统计的子串是不重叠的子串。
    ///
    /// @param s: 在该字符串中查找
    /// @param other, ch, charset: 被统计的子串或者字符或者字符集
    /// @param proc: 用于表示满足特定条件字符，是更抽象的字符查找形式
    /// @param pattern: 用于统计满足表达式的子串的数量，多个子串之间可以重叠。
    /// @return 返回满足条件的子串或者字符的数量。特别的，当 s 或者 other 为空时，总是返回 0
    static auto count(std::string_view s, std::string_view other) -> size_type;
    static auto count(std::string_view s, value_type ch) -> size_type;
    static auto count(std::string_view s, const char_match_proc& proc) -> size_type;
    static auto count(std::string_view s, const charset_type& charset) -> size_type;
    static auto count(std::string_view s, const std::regex& pattern) -> size_type;

    //! 前缀操作 @anchor{prefix}
    ///
    /// 本组函数提供了常见的前缀操作：
    ///
    /// * @ref{prefix} 提供了计算两个字符串的公共前缀的能力；
    /// * @ref{has_prefix, starts_with} 功能一致，都用于测试字符串 s 是否有指定的前缀。
    /// * @ref{remove_prefix_view, remove_prefix, remove_prefix_inplace} 会返回从字符串 s 中去除两个字符串的共同前
    /// 缀后剩余的部分。
    ///
    /// @param s: 目标字符串
    /// @param other: 参与公共前缀计算的另一个字符串，用于 @ref{prefix}
    /// @param prefix, ch: 前缀字符串或者字符
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

    //! 后缀操作 @anchor{suffix}
    ///
    /// 本组函数提供了常见的前缀操作:
    ///
    /// @ref{suffix} 提供了计算两个字符串 s 和 other 的公共前缀的长度，返回 0 表示无前缀；
    /// @ref{has_suffix, ends_with} 功能一致，都用于测试字符串 s 是否有指定的后缀。
    /// @ref{remove_suffix_view, remove_suffix, remove_suffix_inplace} 会返回从字符串 s 中去除两个字符串的共同后剩
    /// 余的部分。
    ///
    /// @param s: 目标字符串。
    /// @param other: 参与公共前缀计算的另一个字符串，用于 @ref{suffix}。
    /// @param suffix, ch: 后缀字符串或者字符。
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

    //! 检测是否以特定的模式开头和结束 @anchor{next_char}
    ///
    /// @ref{starts_with_spaces} 如果字符串 `s` 是否以空白开头，返回 `true`，否则返回 `false`。
    /// @ref{ends_with_spaces} 如果字符串 `s` 是否以空白结尾，返回 `true`，否则返回 `false`。
    /// @ref{starts_with_margin} 如果字符串 `s` 是否的首个非空白字母为 `margin` 字符，返回 `true`，否则返回 `false`。
    ///
    /// @param s: 被检查的参数。
    /// @param margin: 边界字符。
    static auto starts_with_spaces(std::string_view s) -> bool;
    static auto ends_with_spaces(std::string_view s) -> bool;
    static auto starts_with_margin(std::string_view s, value_type margin) -> bool;

    //! 定位字符位置
    ///
    /// 在字符串 s 从特定的位置 `pos` 开始向前或者向后查找、定位特定的字符；如果找到，返回该字符在字符串中的位置；
    /// 否则返回 `std::nullopt` 。
    ///
    /// @ref{next_char} 总是从 `pos` 开始向字符串尾部查找特定的字符，并返回找到的该字符的位置。
    /// @ref{prev_char} 总是从 `pos - 1` 开始向字符串首部查找特定的字符，并返回找到的该字符的位置。
    ///
    ///
    /// @notice{1} 需要特别注意，@ref{next_xxx} 总是从 `pos - 1` 开始，向字符串首部查找（而 @ref{next_char} 总是从
    /// `pos` 开始向字符串尾部查找）。因此，如果希望从 `s` 的最后一个字符开始向前查找时，`pos` 应该指定为 `s.size()`。
    /// 当调用 @ref{prev_xxx} 系列函数且 `pos` 参数指定的值为 `0` 时， @ref{prev_xxx} 系列函数已无法继续向前查找，
    /// 此时将返回 `std::nullopt`。同理，当调用 @ref{next_xxx} 系列函数，且 `pos` 参数大于或者等于 `s.size()` 时，
    /// @ref{prev_xxx} 系列函数同样无法继续查找，此时也将返回 `std::nullopt`。
    ///
    /// @notice{2} 当找到特定的字符时，@ref{next_char} 的 `pos` 总是为该字符的下一个字符，而 @ref{pref_char} 的
    /// `pos` 总是指向该字符所在的位置。这个设计使得 @ref{next_char} 和 @ref{prev_char} 可以配合使用。
    ///
    /// @param s: 在该字符串中查找
    /// @param pos: 查找的起始位置，需要注意该字段对于 `next_xxx` 和 `prev_xxx` 具有不同的含义（参见 @ref{notice{2}}）。
    /// @param ch, charset: 用于定位的字符或者字符集。
    /// @param proc: 用于测试某个字符是否满足定位条件，常用于定制场景。
    static auto next_char(std::string_view s, size_type& pos, value_type ch) -> std::optional<size_type>;
    static auto next_char(std::string_view s, size_type& pos, const charset_type& charset) -> std::optional<size_type>;
    static auto next_char(std::string_view s, size_type& pos, std::string_view charset) -> std::optional<size_type>;
    static auto next_char(std::string_view s, size_type& pos, const char_match_proc& proc) -> std::optional<size_type>;
    /// -
    static auto prev_char(std::string_view s, size_type& pos, value_type ch) -> std::optional<size_type>;
    static auto prev_char(std::string_view s, size_type& pos, const charset_type& charset) -> std::optional<size_type>;
    static auto prev_char(std::string_view s, size_type& pos, std::string_view charset) -> std::optional<size_type>;
    static auto prev_char(std::string_view s, size_type& pos, const char_match_proc& proc) -> std::optional<size_type>;

    //! 定位子串 @anchor{next_string}
    ///
    /// 在字符串 `s` 中，查找（定位）子串 `substr` 的位置。如果找到返回找到的该子串的位置（`range_type`）、
    /// 视图（`std::string_view`）、或者子串本身（`std::string`）；否则，返回 `std::nullopt`。
    ///
    /// @notice{1} 需要特别注意，@ref{next_xxx} 总是从 `pos - 1` 开始，向字符串首部查找（而 @ref{next_char} 总是从
    /// `pos` 开始向字符串尾部查找）。因此，如果希望从 `s` 的最后一个字符开始向前查找时，`pos` 应该指定为 `s.size()`。
    /// 当调用 @ref{prev_xxx}系列函数且 `pos` 参数指定的值为 `0` 时， @ref{prev_xxx} 系列函数已无法继续向前查找，此时
    /// 将返回 `std::nullopt`。同理，当调用 @ref{next_xxx} 系列函数，且 `pos` 参数大于或者等于 `s.size()`
    /// 时，@ref{prev_xxx} 系列函数同样无法继续查找，此时也将返回 `std::nullopt`。
    ///
    /// @notice{2} 当找到特定的字符时，@ref{next_xxx} 的 `pos` 总是为该字符的下一个字符，而 @ref{pref_xxx} 的 `pos`
    /// 总是指向该字符所在的位置。这个设计使得 @ref{next_xxx} 和 @ref{prev_xxx} 可以配合使用。
    ///
    /// @ref{next_string_range, next_string_view, next_string} 从字符串 `s` 的 `pos` 位置开始向字符串尾部，
    /// 查找 `substr`。
    /// @ref{prev_string_range, prev_string_view, prev_string} 从字符串 `s` 的 `pos - 1` 位置开始向字符串首部，
    /// 查找 `substr`。
    ///
    /// @param s: 在该字符串中查找（定位）子串。
    /// @param pos: 查找的起始位置，需要注意该字段对于 `next_xxx` 和 `prev_xxx` 具有不同的含义（参见 @ref{notice{2}}）。
    /// @param substr: 待查找（定位）的子串。
    static auto next_string_range(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<range_type>;
    static auto next_string_view(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<std::string_view>;
    static auto next_string(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<std::string>;
    /// -
    static auto prev_string_range(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<range_type>;
    static auto prev_string_view(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<std::string_view>;
    static auto prev_string(std::string_view s, size_type& pos, std::string_view substr) -> std::optional<std::string>;

    //! 定位行结束符（换行符） @anchor{next_eol}
    ///
    /// 从 pos 开始查找（定位）行结束符（End-Of-Line）的位置，可用于按行拆分字符串场景。
    ///
    /// @notice{eol} 由于操作系统的差异，行结束符并非总是 `\n`，本系列函数采用下面的策略来识别行结束符
    /// * 如果当前字符为 `\n`，那么将当前字符 `\n` 识别为行结束符；
    /// * 如果当前字符为 `\r`，那么还需要额外查看下一个字符，如果下一个字符为 `\n`，那么将 `\r\n` 整体视
    ///   作行结束符；否则将 `\r` 视作行结束符。
    ///
    /// @param s: 在该字符串内查找行结束符
    /// @param pos: 查找的起始位置，需要注意该字段对于 `next_xxx` 和 `prev_xxx` 具有不同的含义（参见 @ref{notice{2}}）。
    static auto next_eol_range(std::string_view s, size_type& pos) -> std::optional<range_type>;
    static auto next_eol_view(std::string_view s, size_type& pos) -> std::optional<std::string_view>;
    static auto next_eol(std::string_view s, size_type& pos) -> std::optional<std::string>;
    /// -
    static auto prev_eol_range(std::string_view s, size_type& pos) -> std::optional<range_type>;
    static auto prev_eol_view(std::string_view s, size_type& pos) -> std::optional<std::string_view>;
    static auto prev_eol(std::string_view s, size_type& pos) -> std::optional<std::string>;

    //! 行结束符检测 @anchor{eol}
    ///
    /// @ref{ends_with_eol, has_eol_suffix} 检查字符串 `s` 是否具有行结束符
    /// @ref{eol_suffix} 检查字符串 `s` 是否以行结束符结尾，如果有返回行结束符的长度，否则返回 0
    ///
    /// @param s: 被检测的字符
    static auto ends_with_eol(std::string_view s) -> bool;
    static auto has_eol_suffix(std::string_view s) -> bool;
    static auto eol_suffix(std::string_view s) -> size_type;

    //! 移除行结束符 @anchor{remove_eol}
    ///
    /// 返回去除字符串 `s` 尾部的行结束符后的新串。需要注意，不同函数返回的数据类型有差别。
    ///
    /// @param s: 待移出行结束符的原始字符串。
    static auto remove_eol_suffix_range(std::string_view s) -> range_type;
    static auto remove_eol_suffix_view(std::string_view s) -> std::string_view;
    static auto remove_eol_suffix(std::string_view s) -> std::string;
    static auto remove_eol_suffix_inplace(std::string& s) -> std::string&;

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

    //! 定位空白块 @anchor{next_spaces}
    ///
    /// @ref{next_spaces_pos} 在字符串 `s` 中，从 `pos` 位置开始查找下一个空白块的位置，并返回该空白块的起始位置。
    /// 如果未找到，返回 `std::nullopt`。
    ///
    /// @notice{1} “空白块”是指由 @ref{all_spaces} 中的字符组成的连续的子串。
    ///
    /// @param s: 在该字符串中查找空白块
    /// @param pos: 作为输入参数时，表示查找空白块的起始位置；作为输出参数时，表示找到的空白块最后一个空白字符的
    /// 之后的位置。
    static auto next_spaces_pos(std::string_view s, size_type& pos) -> size_type;
    static auto next_spaces(std::string_view s, size_type& pos) -> std::optional<size_type>;
    static auto next_spaces_range(std::string_view s, size_type& pos) -> std::optional<range_type>;
    static auto next_spaces_view(std::string_view s, size_type& pos) -> std::optional<std::string_view>;
    /// -
    static auto prev_spaces_pos(std::string_view s, size_type& pos) -> size_type;
    static auto prev_spaces(std::string_view s, size_type& pos) -> std::optional<size_type>;
    static auto prev_spaces_range(std::string_view s, size_type& pos) -> std::optional<range_type>;
    static auto prev_spaces_view(std::string_view s, size_type& pos) -> std::optional<std::string_view>;

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

    //! 特征测试 @anchor{is_xx}
    ///
    /// @ref is_lower: 检测 `s` 中的所有字母都是小写（参考 `std::islower`）。
    /// @ref is_upper: 检测 `s` 中的所有字母都是大写字母（参考 `std::isupper`）。
    /// @ref is_capitalize: 检测 `s` 的首个字符是否为大写字母。
    /// @ref is_title:  对于给定的字符串 `s` 中，以空白分割的每个子串(单词)，如果其每个子串的首字符都是非字母或者
    /// 是大写字母返回 `true`。
    /// @ref is_digit: 检测 `s` 否所有的字符都是数字或者十六进制字符（参考 `std::xdigit`）。
    /// @ref is_xdigit: 检测 `s` 否所有的字符都是数字或者十六进制字符（参考 `std::xdigit`）。
    /// @ref is_ascii: 检测 `s` 中的所有字符是否都在 ASCII 范围内。
    /// @ref is_alpha: 检测 `s` 是否全都为字母（参考 `std::isalpha`）。
    /// @ref is_alnum: 检测 `s` 是否全都为字母或者数字（参考 `std::isalnum`）。
    /// @ref is_alnumul: 检测 `s` 是否全都为字母或者数字或者下划线。
    /// @ref is_space: 检测 `s` 是否全都为空白字符（参考 `std::isspace`）。
    /// @ref is_blank: 检测 `s` 是否全都为空格字符（参考 `std::isblank`）。
    /// @ref is_print: 检测 `s` 是否全都为可打印字符（参考 `std::isprint`）。
    /// @ref is_graph: 检测 `s` 是否全都为图形符（参考 `std::isgraph`）。
    ///
    /// @param s: 被测试的字符串
    /// @return 所有的字符串都必须按组共同的特征，才会返回 `true`，否则，（包括 `s` 为空串场景）均返回 `false`。
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

    //! 特征测试：常见词法特征类 @anchor{is_literal, is_identifier}
    ///
    /// @ref{is_identifier} 检查字符串 `s` 是否满足一个标识符的特征，即以大小写字母或者下划线开头且后续字符为字母
    /// 数字或者下划线。
    /// @ref{is_literal_bool} 检查字符串 `s` 是否是 bool 值的特征，等价于
    /// `(is_literal_true(s) || is_literal_false(s))`。
    /// @ref{is_literal_true} 检查字符串 `s` 是否可以被视作 `true`。
    /// @ref{is_literal_false} 检查字符串 `s` 是否可以被视作 `false`。
    /// @ref{is_literal_integer} 检查字符串 s 是否可以被视作整数。
    /// @ref{is_literal_real} 检查字符串 s 是否可以被视作浮点数，需要注意整数本身也可以被视作浮点数。
    ///
    /// @notice{1} 针对各种字符特征，下面为更具体的解释：
    /// * 被视作 `true` 的字符串包括 `"1"`, `"on"`, `"ON"`, `"Yes"`, `"yes"`, `"YES"`, `"True"`, `"true"`, `"TRUE"`
    /// * 被视作 `false` 的字符串包括 `"0"`, `"off"`, `"OFF"`, `"No"`, `"no"`, `"NO"`, `"False"`, `"false"`, `"FALSE"`
    /// * 被视作 `real` 的字符串，等价于匹配正则表达式 `[+-]?(([0-9]+)|([0-9]+\.)|(\.[0-9]+))([Ee][+-]?[0-9]+)?`
    /// * 被视作 `integer` 的字符串，等价于匹配正则表达式 `[+-]?[0-9]+`
    /// * 被视作 `identifier` 的字符串，等价于正则表达式 `[A-Za-z_][0-9A-Za-z_]*`
    ///
    /// @param s: 被测试的字符串。
    /// @return 所有的字符串都必须按组共同的特征，才会返回 `true`，否则，（包括 `s` 为空串场景）均返回 `false`。
    static auto is_identifier(std::string_view s) -> bool;
    static auto is_literal_bool(std::string_view s) -> bool;
    static auto is_literal_true(std::string_view s) -> bool;
    static auto is_literal_false(std::string_view s) -> bool;
    static auto is_literal_integer(std::string_view s) -> bool;
    static auto is_literal_real(std::string_view s) -> bool;

    //! 特征测试：指定字符集类 @anchor{is_all_in}
    ///
    /// @param s: 被测试的字符串
    /// @param proc: 用于测试 s 中的每个字符是否满足给定条件的函数
    /// @param charset: 指定需要满足条件的字符集
    /// @return 所有的字符串都必须按组共同的特征，才会返回 true，否则，（包括 s 为空串场景）均返回 false。
    template <typename CharMatchProc, typename = std::enable_if<std::is_function<CharMatchProc>::value>>
    static auto is_all_in(std::string_view s, const CharMatchProc& proc) -> bool;
    static auto is_all_in(std::string_view s, const charset_type& charset) -> bool;

    //! 特征测试：单一条件类 @anchor{has_any_one}
    ///
    /// @param s: 被测试的字符串
    /// @param proc: 用于测试 `s` 中的每个字符是否满足给定条件的函数
    /// @param charset: 指定需要满足条件的字符集
    /// @return 与 `is_xxx` 系列函数需要“所有字符必须全部满足指定特征”不同，`has_xxx` 系列函数只需要有任意一个
    /// 字符满足特征，立即返回 `true`。唯一的特例是空串总是返回 `false`。
    template <typename CharMatchProc, typename = std::enable_if<std::is_function<CharMatchProc>::value>>
    static auto has_any_one(std::string_view s, const CharMatchProc& proc) -> bool;
    static auto has_any_one(std::string_view s, const charset_type& charset) -> bool;

    //! 是否空白或者控
    static auto is_space_or_empty(std::string_view s) -> bool;

    //! 提取子串：基于位置 @anchor{take}
    ///
    /// @ref take_left_view, take_left, take_left_inplace: 返回字符串 s 的最左边前 n 个字符的子串
    /// @ref take_right_view, take_right, take_right_inplace: 返回字符串 s 的最右边前 n 个字符的子串
    /// @ref take_view, take, take_inplace: 返回字符串 s 中，从pos 位置开始的 n个字符组成的子串
    /// @ref take_view, take, take_inplace: 返回字符串 s 中，range 范围的子串。
    /// @ref take_view, take, take_inplace: 返回字符串 s 中，从 pos 开始偏移 shifter 的字符串。
    ///
    /// @param s: 原始字符串
    /// @param n: 指定提取的子串的最大长度。当 n 为 0 时，总是返回空串。当按照指定的方式无法获得 n 个字符的长度
    /// 时，相关函数总是试图返回尽可能多的字符串。
    /// @param pos: 用于指定待提取的子串的起始位置
    /// @param range:  指定用于需要提取的子串的范围，用于 `pos` 和 `len` 形式的范围。
    /// @param inter:  指定用于需要提取的子串的范围，用于 `begin` 和 `end` 形式的范围。
    /// @param shifter:  指定用于需要提取的子串的范围，用于 `ppos` 和 `offset` 形式的范围。
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
    /// -
#ifdef STR_UNTESTED
    static auto take_before_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto take_after_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto take_before(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;
    static auto take_after(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;
#endif // STR_UNTESTED

    //! 删除子串：基于位置 @anchor{drop}
    ///
    /// @ref drop_left_view, drop_left, drop_left_inplace: 返回去掉字符串 s 的最左边前 n 个字符后的子串
    /// @ref drop_right_view, drop_right, drop_right_inplace: 返回去掉字符串 s 的最右边的 n 个字符后的子串
    /// @ref drop, drop_inplace: 返回从字符串 s 中剔除指定范围或者模式的字符后的剩余的字符串。
    ///
    /// @param s: 原始字符串
    /// @param n: 指定删除的子串的最大长度。
    /// @param pos: 用于指定需要剔除的子串的起始位置。
    /// @param range:  指定用于需要剔除的子串的范围，用于 `pos` 和 `len` 形式的范围。
    /// @param inter:  指定用于需要剔除的子串的范围，用于 `begin` 和 `end` 形式的范围。
    /// @param shifter:  指定用于需要剔除的子串的范围，用于 `ppos` 和 `offset` 形式的范围。
    /// @param proc: 用于指定需要剔除的字符。
    /// @param charset: 用于指定需要剔除的字符集。
    /// @return 返回剔除特定字符或者子串后的剩余部分。
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
    //
#ifdef STR_UNTESTED
    static auto drop_before_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto drop_after_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto drop_before(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;
    static auto drop_after(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;
#endif // STR_UNTESTED

    //! 对齐 @anchor{align}
    ///
    /// @ref align_left, align_left_inplace: 在字符串 s 尾部填充 ch，直到字符串长度达到 width，以使得字符串看起来是
    /// 左对齐的效果
    /// @ref align_right, align_right_inplace: 在字符串 s 头部追加 ch，直到字符串长度达到 width，以使得字符串看起来
    /// 是左对齐的效果
    /// @ref align_center, align_center_inplace: 在字符串 s 首尾添加 ch，直到字符串长度达到 width，以使得字符串看起
    /// 来是居中对齐的效果
    /// @ref align_zfill, align_zfill_inplace: 在字符串 s 头部添加字符 '0'，使得字符串看起来被补齐了前缀 0，这通常
    /// 用于全是数字的字符串的场景
    ///
    /// @param s: 被对齐的字符串
    /// @param widht: 指定新生成的字符串的宽度，如果 width 小于或者等于 s 的长度，不会追加 ch，也即对齐前后字符串内容不变
    /// @param ch: 当 width 大于 s 的长度时，所采用的填充字符
    static auto align_left(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_right(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_center(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_zfill(std::string_view s, size_type width) -> std::string;
    static auto align_left_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_right_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_center_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_zfill_inplace(std::string& s, size_type width) -> std::string&;

    //! 多行文本处理 @anchor{lines}
    ///
    /// @ref foreach_lines: 用于按行遍历。
    /// @ref count_lines: 字符串 `s` 中实际有多少行。
    ///
    /// @param s: 包含多行文本而串。
    /// @param keep_ends: 是否保留行结束符。
    /// @param proc: 用于接收每个遍历的行。
    static auto foreach_lines(std::string_view s, bool keep_ends, const line_consumer_proc& proc) -> void;
    static auto count_lines(std::string_view s) -> size_type;

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

    //! 以单词为单位的处理算法 @anchor{words}
    ///
    /// @notice{1} 这里的单词（word）是指连续的非空白字符序列。
    ///
    /// * @ref foreach_words: 用于遍历字符串中的每个单词
    /// * @ref count_words: 用于统计字符串中的单词的数量
    /// * @ref next_word_view, next_word_range, next_word: 用于从指定的位置开始向字符串的尾部查找下一个单词
    /// * @ref prev_word_view, prev_word_range, prev_word: 用于从指定的位置开始向字符串的首部查找前一个单词
    /// * @ref split_words: 以空格为分隔符从字符串 s 中拆分出多个单词
    /// * @ref starts_with_word: 检查字符串 s 的第一个单词是否为期望的单词
    /// * @ref ends_with_word:  检查字符串 s 的最后一个单词是否为期望的单词
    ///
    /// @param s: 被查找或者统计的原始字符串
    /// @param pos: 指定起始位置，需要注意在 next_xxx 和 prev_xxx 函数中，pos 的含义的区别。
    /// @param proc: 指定一个函数，用来接受遍历每一个被视作单词的子串
    /// @param max_n: 用于 @ref{split_words} 函数，用于限制拆分出来的单词的数量（注意并非拆分次数）
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
#ifdef STR_UNTESTED
    static auto split_words_view(std::string_view s, size_type max_n = npos) -> std::vector<std::string_view>;
#endif // STR_UNTESTED
    /// -
    static auto starts_with_word(std::string_view s, std::string_view word) -> bool;
    static auto ends_with_word(std::string_view s, std::string_view word) -> bool;

    //! 用指定的模式串环绕字符串 @anchor{surround, unsurround}
    ///
    /// @ref{surround} 和 @ref{unsurround} 系列函数，同时在字符串两端操作，常用于添加括号和去掉括号场景。
    /// 其中，`left` 和 `right` 表示字符串首尾需要添加或者去掉的子串。对于 unsurround 系列函数，如果字符串的首
    /// 或者尾，分别无法匹配 `left` 或者 `right` 子串，那么不进行任何操作。
    ///
    /// * @ref{surround, surround_inplace} 向给定的字符串的首位添加特定的子串
    /// * @ref{unsurround, unsurround_view, unsurround_inplace} 同时去掉指定字符串前后满足特定模式的子串
    ///
    /// @param s: 被处理的字符串
    /// @param left, right: 表示在字符串首尾的需要添加或者去掉的子串
    static auto surround(std::string_view s, std::string_view left = "(", std::string_view right = ")") -> std::string;
    static auto surround_inplace(std::string& s, std::string_view left = "(", std::string_view right = ")") -> std::string&;
    /// -
    static auto unsurround_view(std::string_view s, std::string_view left = "(", std::string_view right = ")") -> std::string_view;
    static auto unsurround(std::string_view s, std::string_view left = "(", std::string_view right = ")") -> std::string;
    static auto unsurround_inplace(std::string& s, std::string_view left = "(", std::string_view right = ")") -> std::string&;

    //! 反转：字符串逆序 @anchor{invert}
    ///
    /// 将 `s` 中指定范围内的子串的前后字符串的逐个字符交换位置。
    ///
    /// @param s: 被反转的字符串
    /// @param pos: 指定反转的起始位置
    /// @param max_n: 指定从 pos 位置开始最多反转多少字符
    /// @return 返回颠倒位置后的字符串
    static auto invert(std::string_view s, size_type pos = 0, size_type max_n = npos) -> std::string;
    static auto invert_inplace(std::string& s, size_type pos = 0, size_type max_n = npos) -> std::string&;

    //! 重复序列生成 @ancho{repeat}
    ///
    /// 生成字 `s` 或者 `ch` 的内容重复出现 `times` 次后的新字符串。如果是生成空白字符的重复序列，
    /// 可考虑用 @ref{spaces}。
    ///
    /// @param ch, s: 指定重复的字符或者字符串模板。
    /// @param times: 重复次数。
    static auto repeat(std::string_view s, size_type times) -> std::string;
    static auto repeat(value_type ch, size_type times) -> std::string;

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

    //! 空白串生成 @anchor{spaces}
    ///
    /// * @ref{spaces} 用于生成指定宽度 `width` 的空白字符序列。
    /// * @ref{make_spaces，make_spaces_inplace} 只要内存允许，支持生成任意长度 ` 的空白串。
    ///
    ///  @notice{1} @ref{spaces}，@ref{make_spaces}，@ref{make_spaces_inplace} 这三个函数的功能类似。但主要差别
    ///  是 @ref{space} 利用的是预生成的字符串常量，所以速度会更快，但最大长度有限（`UNIT8_MAX`），大部分情况下
    ///  @ref{space} 函数都是够用的。当需要生成超过 @ref{space} 支持空白串时，使用 @ref{make_spaces}
    ///  或者 @ref{make_spaces_inplace} 是更合适的选择。但显然 `make_xxx` 系列函数更慢。
    ///
    /// @param s: 在 make_spaces_inplace 中表示用于接收数据的字符串
    /// @param witdh: 指定待生成的空白串的宽度，这里的空白串采用 ASCII 码为 0x20 的字符填充
    /// @return @ref{spaces} 函数返回内部预生成的空白串的视图；@{make_space} 为返回新生成的字符串（内存重新分配）；
    /// 而 @ref{make_spaces_inplace} 函数会将生成的空白串直接原位替换原始输入字符串。
    static auto spaces(uint8_t width) -> std::string_view;
    static auto make_spaces(size_type width) -> std::string;
    static auto make_spaces_inplace(std::string& s, size_type width) -> std::string&;
#ifdef STR_UNTESTED
    static auto after_skip_spaces_view(std::string_view s) -> std::string_view;
    static auto after_skip_spaces(std::string_view s) -> std::string;
    static auto after_skip_spaces_inplace(std::string& s) -> std::string&;
    static auto after_skip_spaces_view(std::string_view s, size_type pos) -> std::string_view;
    static auto after_skip_spaces(std::string_view s, size_type pos) -> std::string;
    static auto after_skip_spaces_inplace(std::string& s, size_type pos) -> std::string&;
#endif // STR_UNTESTED

    //! 字符串遮罩 @anchor{cover}
    ///
    /// 使用指定的掩码字符串 mask 对原始字符串中的关键文字替换，以遮蔽原始字符串中的敏感信息。常用于
    /// 敏感信息脱敏，长串简略缩短等场景。
    ///
    /// @ref{cover} 和 @ref{cover_inplace} 对 `s` 的中间部分使用遮罩字符串 `mask` 替换，其中 `left_n` 和 `right_n` 用于
    /// 控住遮罩之后，首尾各自保留多少长度的明文子串。
    ///
    /// @notice{1} 当 `left_n` 或者 `right_n` 或者 `left_n + right_n` 大于或者等于原始串 s 的长度时，本函数将失去
    /// 遮罩作用。对于这种情况 @ref{cover} 和 @ref{cover_inplace} 函数将返回空串，以避免无意间的信息泄露。因此，
    /// 在使用本函数之前，最好确保字符串 s 的长度达到本算法的最小值 `left_n + right_n`。
    ///
    /// @param s 被遮罩的字符串
    /// @param mask 用于遮罩的字符串，特别的，mask 为空也是允许的，但效果等价于去掉被遮罩的字符。
    /// @param left_n, right_n 用于指定字符串 `s` 的首尾两端分别保留多长的明文串
    /// @return 返回遮罩后的字符串
    static auto cover(std::string_view s, std::string_view mask = "***", size_type left_n = 1, size_type right_n = 3) -> std::string;
    static auto cover_inplace(std::string& s, std::string_view mask = "***", size_type left_n = 1, size_type right_n = 3) -> std::string&;

#ifdef STR_UNIMPL
    //! 元音（aeiou）化简算法：去掉单词中的元音字符 除非他们为单词的首字母
#endif // STR_UNIMPL

    //! 字符串拼接 @anchor{join}
    ///
    /// 以 `s` 作为分隔符，拼接多个字符串。输入串可以通过 `proc` 或者 `items` 供给。如果 `proc` 无法提供任何字符串
    /// 或者 `items` 为空，返回空串。
    ///
    /// @param s: 分隔符。
    /// @param proc: 用于通过回调函数的方式供给被拼接的字符串。
    /// @param items: 被字符串容器序列。
    /// @return 返回拼接后的字符串。
    static auto join(std::string_view s, const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join(std::string_view s, const Sequence& items) -> std::string;

    //! 拼接列表 @anchor{join_list}
    ///
    /// 使用逗号作为分隔符，拼接多个子串。输入串可以通过 proc 或者 items 供给。如果 proc 无法提供任何字符串或者 items 为空，返回空串。
    ///
    /// @param proc: 用于通过回调函数的方式供给被拼接的字符串
    /// @param items: 被字符串容器序列
    /// @return 返回合并后的字符串
    static auto join_list(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_list(const Sequence& items) -> std::string;

    //! 映射拼接 @anchor{join_map}
    ///
    /// 使用 `sep_pair` 拼接每个 key-value 对的 key 和 value 部分，并使用 `sep_list` 拼接多个 key-value 对。
    /// 该函数拼接的结果接近 json 的字典的内部结构（没有外围的花括号）。
    ///
    /// @param sep_pair: 用于拼接每个 key-value 对，当未指定该参数或者为空时，自动采用 `":"`
    /// @param sep_list: 用于拼接多个拼接好的 key-value 对，当未指定该参数或者为空时，自动采用 `","`
    /// @param proc: 用于供给 key-value 对， key-value 对由 `pair<std::string_view>` 来表示
    /// @param items: 用于存储 key-value 对的容器
    /// @return 返回拼接后的字符串
    static auto join_map(std::string_view sep_pair, std::string_view sep_list, const view_pair_provider_proc& proc) -> std::string;
    static auto join_map(const view_pair_provider_proc& proc) -> std::string;
    template <typename Map, typename = typename Map::const_iterator>
    static auto join_map(std::string_view sep_pair, std::string_view sep_list, const Map& items) -> std::string;
    template <typename Map, typename = typename Map::const_iterator>
    static auto join_map(const Map& items) -> std::string;

    //! 按行拼接 @anchor{join_lines}
    ///
    /// 将每个字符串视作一行，然后用换行符拼接成一个字符串，并返回。
    ///
    /// @param line_ends: 指定行结束符，如果未指定，默认使用 @ref{sep_line_ends} 作为分隔符
    /// @param items: 指定存储行的容器
    /// @param proc: 通过 proc 回调函数供给值，当 proc 返回 `std::nullopt` 时，终止拼接过程
    static auto join_lines(std::string_view line_ends, const view_provider_proc& proc) -> std::string;
    static auto join_lines(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_lines(std::string_view line_ends, const Sequence& items) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_lines(const Sequence& items) -> std::string;

    //! 拼接路径 @anchor{join_path}
    ///
    /// 使用指定的分隔符 `sep` 或者系统默认的路径分隔符，将不同来源的路径片段拼接成完整的文件路径。
    ///
    /// @notice{1} 需要注意：@ref{join_path} 会主动去除路径片段间的路径分隔符 '/'，但只会去除一次。
    ///
    /// @param sep: 指定的路径分隔符，如果不带该参数，默认使用 @ref{sep_path}
    /// @param proc: 通过回调函数指定路径片段，当 proc 返回 `std::nullopt` 时，拼接过程终止。
    /// @param items: 存储路径片段的容器
    /// @return 返回拼接后的路径
    static auto join_path(std::string_view sep, const view_provider_proc& proc) -> std::string;
    static auto join_path(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_path(std::string_view sep, const Sequence& items) -> std::string;
    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_path(const Sequence& items) -> std::string;

    //! 拼接搜索路径 @anchor{join_searchpath}
    ///
    /// 使用搜索路径分隔符拼接由 `proc` 或者 `items` 供给的字符串，并返回拼接后的结果。
    /// 路径分隔符可以通过 `sep` 参数指定，当调用没有该参数的形式的函数时，自动使用系统默认的分隔
    /// 符（参见 @ref{sep_searchpath}）。对于提供 `proc` 参数的接口，`proc` 会持续调用该哈数获得数据直到该函数
    /// 返回 `std::nullopt`。如果 `proc` 在第一次调用时就返回 `std::nullopt`，返回的搜索路径为空串。
    ///
    /// @param sep: 搜索路径分隔符。对于未指定该参数的函数，自动采用 @ref{sep_searchpath} 作为分隔符。
    /// @param proc: 提供搜素路径片段的函数。
    /// @param items: 存放路径片段的容器。
    /// @return 返回以当前系统的搜索路径分隔符拼接好的字符串。
    static auto join_searchpath(std::string_view sep, const view_provider_proc& proc) -> std::string;
    static auto join_searchpath(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_searchpath(std::string_view sep, const Sequence& items) -> std::string;
    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_searchpath(const Sequence& items) -> std::string;

    //! 通用字符串拆分 @anchor{split}
    ///
    /// 将字符串 `s` 以指定的分隔符（比如，字符集 `sep_charset`、字符串 `sep_str`、某抽象函数 `search_proc`）拆分为
    /// 多个子串。
    ///
    /// @param s: 被拆分的字符串。
    /// @param search_proc: 更抽象的分隔符定位方法，需要返回找到的分隔符的范围，或者返回 `std::nullopt` 表示找不到
    /// 分隔符。
    /// @param sep_charset: 分隔符集合，可以有多种形式组成，集合中的每个字符都是分隔符。
    /// @param sep_str: 以字符串为分隔符，如果该参数指定为空串，自动以连续的空白符为分隔符。
    /// @param max_n: 最多拆分多少次。如果为 0 表示不做任何拆分，返回原始字符串。如果为 npos 表示不限制拆分次数。
    /// 需要注意，当字符串 `s` 中实际的分隔符的数量大于 `max_n` 时，会返回 `max_n + 1` 个子串。
    /// @param proc: 指定如何接受拆分出来的字符串。需要注意，不同函数 proc 的原型是不同的。
    /// @return 当未指定 proc 参数时，会返回字符串列表。
    static auto split(std::string_view s, const substr_search_proc& search_proc, size_type max_n, const range_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const charset_type& sep_charset, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const charset_type& sep_charset, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const charset_type& sep_charset, size_type max_n = npos) -> std::vector<std::string>;
    static auto split(std::string_view s, std::string_view sep_str, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, std::string_view sep_str, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, std::string_view sep_str, size_type max_n = npos) -> std::vector<std::string>;
#ifdef STR_UNIMPL
    static auto split(std::string_view s, const std::regex& sep_regex, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const std::regex& sep_regex, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const std::regex& sep_regex, size_type max_n = npos) -> std::vector<std::string>;
#endif // STR_UNIMPL
    static auto split(std::string_view s, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto split(std::string_view s, size_type max_n = str::npos) -> std::vector<std::string>;
    static auto split_view(std::string_view s, const charset_type& sep_charset, size_type max_n = npos) -> std::vector<std::string_view>;
    static auto split_view(std::string_view s, std::string_view sep_str, size_type max_n = npos) -> std::vector<std::string_view>;
    static auto split_view(std::string_view s, size_type max_n = str::npos) -> std::vector<std::string_view>;

    //! 按逗号拆分 @anchor{split_list}
    ///
    /// 以逗号 `,` 为分隔符，将字符串 `s` 拆分成多个子串。
    ///
    /// @param s: 被拆分的字符串。
    /// @param proc: 指定如何接受拆分出来的字符串。
    /// @param max_n: 最多拆分多少次。如果为 0 表示不做任何拆分，将返回原始字符串。如果为 `npos` 表示不限制拆分次数。
    /// 需要注意，当字符串 `s` 中实际的分隔符的数量大于或者等于 `max_n` 时，会返回 `max_n + 1` 个子串。
    /// @return 当未指定 `proc` 参数时，会返回字符串列表。
    static auto split_list(std::string_view s, size_type max_n, const view_consumer_proc& proc) -> void;
    static auto split_list(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto split_list(std::string_view s, size_type max_n = npos) -> std::vector<std::string>;
    static auto split_list_view(std::string_view s, size_type max_n = npos) -> std::vector<std::string_view>;

    //! 拆分 key-value 对 @anchor{split_pair}
    ///
    /// @ref{split_pair} 等价于 @ref{split} 系列函数，指定 `max_n` 参数为 `1` 时的功能。
    ///
    /// @param s: 被拆分的字符串。
    /// @param sep: 用作分隔符的字符串，当 `sep` 被指定为空串时，自动以 `":"` 为分隔符。
    /// @return 返回被拆分处理的字符串。如果字符串中未找到分隔符，整个字符串作为返回值的第一个字符串，而第二个字符
    /// 串为空。
    static auto split_pair(std::string_view s, std::string_view sep = ":") -> pair<std::string>;
    static auto split_pair_view(std::string_view s, std::string_view sep = ":") -> pair<std::string_view>;

    //! 拆分多个 key-value 对 @anchor{split_map{}
    ///
    /// @ref{split_map} 会对字符串做两轮拆分：
    /// 第一轮先以 `sep_list` 为分隔符，将字符串拆分成一组字串；
    /// 第二轮再以 `sep_pair` 为分隔符将前一轮拆分出来的每个字串拆分成键值对，并将该该键值对存入 map 对象返回，
    /// 或者通过 proc 输出。
    /// 总之，@ref{split_map} 是拆分的是类型下面的数据格式的算法（以 `sep_list` 和 `sep_pair` 为缺省值时为例）：
    ///
    ///     item1:value1,item2:value2,item3:value3 ...
    ///
    /// @param s: 被拆分的字符串。
    /// @param sep_list: 用作第一轮拆分的分隔符。
    /// @param sep_pair: 用作第二轮拆分的分隔符。
    /// @param max_n: 最多拆分多少次。`max_n` 主要用于控制第一轮拆分的次数，如果指定为 0 将返回空 map 或者不
    /// 触发 proc。当次数达到后，后续的数据会被舍弃，且不会被放入 map 中，也不会再触发 proc。由于调用方无法感
    /// 知是否有剩余数据未拆分完，因此，`max_n` 通常只用在舍弃剩余字符串是无关紧要的情况下使用。
    /// @param proc: 输出拆分出来的每个键值对。
    /// @return 返回组合成的 map，对于返回值为 void 的形式，数据通过 proc 返回。
    static auto split_map(std::string_view s, std::string_view sep_list, std::string_view sep_pair, const view_pair_consumer_proc& proc) -> void;
    static auto split_map(std::string_view s, std::string_view sep_list = ",", std::string_view sep_pair = ":", size_type max_n = npos) -> std::map<std::string, std::string>;

    //! 按行拆分 @anchor{split_lines}
    ///
    /// 以行结束符为分隔符，将字符串 `s` 拆分成多行。
    ///
    /// @param s: 待拆分字符串。
    /// @param keep_ends: 是否保留行尾的分隔符。
    /// @param proc: 用于接收拆分出的子串。
    /// @return 通过 proc 接收拆分后的行或者返回存储拆分后的所有行的容器。
    static auto split_lines(std::string_view s, bool keep_ends, const view_consumer_proc& proc) -> void;
    static auto split_lines(std::string_view s, bool keep_ends = false) -> std::vector<std::string>;
    static auto split_lines_view(std::string_view s, bool keep_ends = false) -> std::vector<std::string_view>;

    //! 路径拆分 @anchor{split_path}
    ///
    /// 将字符串 `s` 视作文件或者目录的路径，按照路径分隔符 @ref{sep_path_char}，拆分成多个组成部分。
    ///
    /// @notice{1} 本函数并不支持 windows 下同时可以使用 `'/'` 和 `'\\'` 为分隔符的场景。如果有必要，可以使用
    /// 更加强大的 @ref{split} 函数代替。
    ///
    /// @notice{2} 对于以 '/' 字符开头的绝对路径，拆分出来的第一个字符串，总是 '/'。
    ///
    /// @notice{3} 对于重复的路径分隔符（比如，"a///b" 中的 "///"），会自动视作一个路径分隔符。
    ///
    /// @param s: 待拆分的路径。
    /// @param proc: 用于接收被拆分出来的字符串。
    /// @return 当返回值类型为 `void` 时，可以通过 proc 函数接收输出数据；否则，返回值表示拆分出来的多个子串。
    static auto split_path(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto split_path(std::string_view s) -> std::vector<std::string>;
    static auto split_path_view(std::string_view s) -> std::vector<std::string_view>;

    //! 拆分搜索路径 @anchor{split_searchpath}
    ///
    /// 将字符串 `s` 视作搜索目录（可以以 Linux 系统下的 `$PATH` 环境变量为参考），按照搜索路径分隔
    /// 符 @ref{sep_searchpath_char} 将 `s` 拆分成多个路径。
    ///
    /// @notice{1} 需要注意 POSIX 系统标准并不允许文件路径中存在冒号的情况，
    /// 参考: https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap08.html#tag_08_03。
    ///
    /// @notice{2} 需要注意本函数默认支持的是 `*nix` 系统的默认的搜索路径分隔符，对于 Windows 系统，可以
    /// 指定 `sep` 参数为 `";"`。
    ///
    /// @notice{3} 本系列函数并不会主动去除路径前后的空白，也对于重复路径也不去重。当 `keep_empty` 指定
    /// 为 `true` 时，空串会被传递给 proc 或者返回。
    ///
    /// @param s: 待拆分的路径
    /// @param keep_empty: 是否保留空路径（注意：POSIX 中空搜索路径通常表示当前工作路径，是有意义的）
    /// @param sep: 指定搜索路径分隔符。对于 Windows 下的搜索路径，需要额外指定该参数。
    /// @return 当返回值类型为 `void` 时，可以通过 proc 函数接收输出数据；否则，返回值表示拆分出来的多个子串。
    static auto split_searchpath(std::string_view s, bool keep_empty, value_type sep, const view_consumer_proc& proc) -> void;
    static auto split_searchpath(std::string_view s, bool keep_empty = false, value_type sep = ':') -> std::vector<std::string>;
    static auto split_searchpath_view(std::string_view s, bool keep_empty = false, value_type sep = ':') -> std::vector<std::string_view>;

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

    //! 分片 @anchor{partition}
    ///
    /// @ref{partition_range, partition_view, partition} 从 s 左侧查找首个 sep 分隔符或者满足 proc 的字符的位置，
    /// 并将字符串分割为分隔符左侧，分隔符，分隔符右侧三个部分，并返回。
    ///
    /// @param s: 输入字符串
    /// @param charset, proc, sep: 不同形式的分隔符。
    /// @return 返回依次由分隔符左侧的子串，分隔符自身，分隔符右侧子串组成的三元组（ternary）。
    static auto partition_range(std::string_view s, const charset_type& charset) -> ternary<range_type>;
    static auto partition_range(std::string_view s, const char_match_proc& proc) -> ternary<range_type>;
    static auto partition_range(std::string_view s, std::string_view sep) -> ternary<range_type>;
#ifdef STR_UNTESTED
    static auto partition_range(std::string_view s, const std::regex& pattern) -> ternary<range_type>;
    static auto partition_range(std::string_view s, const substr_search_proc& proc) -> ternary<range_type>;
#endif // STR_UNTESTED
    /// -
    static auto partition_view(std::string_view s, const charset_type& charset) -> ternary<std::string_view>;
    static auto partition_view(std::string_view s, const char_match_proc& proc) -> ternary<std::string_view>;
    static auto partition_view(std::string_view s, std::string_view sep) -> ternary<std::string_view>;
#ifdef STR_UNTESTED
    static auto partition_view(std::string_view s, const std::regex& pattern) -> ternary<std::string_view>;
#endif // STR_UNTESTED
    static auto partition_view(std::string_view s, const view_search_proc& proc) -> ternary<std::string_view>;
    /// -
    static auto partition(std::string_view s, const charset_type& charset) -> ternary<std::string>;
    static auto partition(std::string_view s, const char_match_proc& proc) -> ternary<std::string>;
    static auto partition(std::string_view s, std::string_view sep) -> ternary<std::string>;
#ifdef STR_UNTESTED
    static auto partition(std::string_view s, const std::regex& pattern) -> ternary<std::string>;
#endif // STR_UNTESTED
    static auto partition(std::string_view s, const view_search_proc& proc) -> ternary<std::string>;
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

    //! 字符串分块 @anchor{next_chunk}
    ///
    /// 从字符串 `s` 的 `pos` 位置开始，最多提取 `max_n` 长度的子串。本函数支持迭代。本函数可多次调用，
    /// 实现迭代式提取子串。当 `pos` 大于或者等于 `s` 的长度时，返回 `std::nullopt`。
    ///
    /// @param s:
    static auto next_chunk_range(std::string_view s, size_type& pos, size_type max_n) -> std::optional<range_type>;
    static auto next_chunk_view(std::string_view s, size_type& pos, size_type max_n) -> std::optional<std::string_view>;
    static auto next_chunk(std::string_view s, size_type& pos, size_type max_n) -> std::optional<std::string>;

    //! 指定宽度拆分字符串
    ///
    /// 将字符串 s 拆分成宽度为 width 的多个子串
    ///
    /// @param s 将被拆分的字符串
    /// @param width 执行拆分宽度，如果 width 为 0 自动校正为 1；当 width 大于 s 的长度时，等价于不拆分
    /// @param proc 数据输出函数
    /// @return 通过 proc 或者返回值返回拆分后的子串列表
    static auto chunked(std::string_view s, size_type width, const view_consumer_proc& proc) -> void;
    static auto chunked(std::string_view s, size_type width) -> std::vector<std::string>;
    static auto chunked_view(std::string_view s, size_type width) -> std::vector<std::string_view>;

    //! 字符串分块
    static auto next_window_range(std::string_view s, size_type& pos, size_type max_n, size_type step) -> std::optional<range_type>;
    static auto next_window_view(std::string_view s, size_type& pos, size_type max_n, size_type step) -> std::optional<std::string_view>;
    static auto next_window(std::string_view s, size_type& pos, size_type max_n, size_type step) -> std::optional<std::string>;

    // 基于窗口拆分字符串
    static auto windowed(std::string_view s, size_type width, size_type step, const view_consumer_proc& proc) -> void;
    static auto windowed(std::string_view s, size_type width, size_type step) -> std::vector<std::string>;
    static auto windowed_view(std::string_view s, size_type width, size_type step) -> std::vector<std::string_view>;

    //! 大小写转换
    ///
    /// * @ref{to_lower, to_lower_inplace} 将 s 中的所有大写字母转换为小写字母
    /// * @ref{to_upper, to_upper_inplace} 将 s 中的所有小写字母转换为大写字母
    /// * @ref{to_title, to_title_inplace} 将 s 每个单词的首字母转换为大写形式
    /// * @ref{to_capitalize, to_capitalize_inplace} 将 s 首字母转换为大写形式
    /// * @ref{swap_case, swap_case_inplace} 将 s 中的所有大写字母转换为小写字母，同时将消息字母转换为大写字母
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

    //! 化简 @anchor{simplified}
    ///
    /// * @ref{simplified, simplified_inplace} 将 s 中连续匹配 proc 条件的字符替换为 sep，对于不带 fill 和 proc 形式
    ///   的函数，表示将所有的空白字符替换成单个空格字符（0x20）。
    /// * @ref{simplified_integer, simplified_integer_inplace} 将字符串 s 视作整数（integer）在不影响其值的前提
    ///   下，清除多余的前缀正号和前缀 0。
    /// * @ref{simplified_decimal, simplified_decimal_inplace} 将字符串 s 视作小数（decimal）在不影响其值的前提
    ///   下，清除多余的前缀正号、整数部分的前缀0、小数部分尾部的多余的0。如果 s 是指数形式，指数中的多余 0 以及指数形式本身
    ///   也会被纳入化简考虑。
    ///
    /// @param s 将被化简的字符串
    /// @param fill 如果可以化简，那么化简后改用什么填充
    /// @param proc 连续字符匹配条件
    /// @return 返回化简后的字符串
    static auto simplified(std::string_view s, std::string_view sep, const char_match_proc& proc) -> std::string;
    static auto simplified(std::string_view s) -> std::string;
    static auto simplified_inplace(std::string& s, std::string_view sep, const char_match_proc& proc) -> std::string&;
    static auto simplified_inplace(std::string& s) -> std::string&;
    //
    static auto simplified_integer(std::string_view s) -> std::string;
    static auto simplified_integer_inplace(std::string& s) -> std::string&;
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

    //! 展开 @anchor{expand_envs}
    ///
    /// 将字符串 `s` 中 `$xxx` 和 `${xxx}` 形式的子串看做环境变量的占位子串，将其中的 `xxx` 视作环境变量的名字，
    /// 将整个占位子串替换为环境变量的值。本系列函数提供了多种方式获取环境变量值的方式。其中，如果没带任何获取环
    /// 境变量值的方式的话，将自动通过 `getenv` 函数，从当前系统环境变量表中获取。
    ///
    /// @param s: 模板字符串
    /// @param keep_unexpanded: 当无法通过 `key` 确定被替换的值时，由 `keep_unexpanded` 参数决定是保留原样。如果
    /// 不原样保留会直接去除占位子串。
    /// @param proc: 通过 `proc` 查询环境变量的值。
    /// @param kvs: 直接从一个指定的 map 中检索环境变量的值。
    /// @param key, val: 只将 `key` 替换为 `val`，其他的原样保留。
    /// @return 返回替换后的新串。
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

    //! tab 扩展 @anchor{expand_tabs}
    ///
    /// 将字符串中的 tab 符号（`\t`）按照 `tab_size` 宽度替换成空格（对 应ASCII 码 0x20）。每个 tab 实际被替换成
    /// 的空格的数量，并不是固定的，而是取决于 tab 字符在字符串 s 中的位置。该函数模拟了在编辑器中按 tab 键时的视
    /// 觉效果。
    ///
    /// @param s: 将被扩展的字符串
    /// @param tab_size: 指定一个 tab 应该等价于多少个空格字符（0x20）。
    /// @return 返回扩展后的字符串
    static auto expand_tabs(std::string_view s, size_type tab_size = 8) -> std::string;
    static auto expand_tabs_inplace(std::string& s, size_type tab_size = 8) -> std::string&;

    //! HOME 字符扩展 @anchor{expand_user}
    ///
    /// 扩展字符串中的 `~` 前缀为 `${HOME}` 的值，该函数模拟了 Shell 的行为。
    ///
    /// @param s: 将被扩展的字符串
    /// @return 返回扩展后的字符串
    static auto expand_user(std::string_view s) -> std::string;
    static auto expand_user_inplace(std::string& s) -> std::string&;

    //! 路径正规化 @anchor{normpath}
    ///
    /// 将输入字符串 `s` 视作文件或者目录的路径，消除路径中的相对路径（`.` 和 `..`）。
    ///
    /// @notice{1} 需要注意，本算法（函数）只是做字符串的操作，并不会访问文件系统。但这也意味着，在部分场景下
    /// 并不能完全去掉相对路径。
    ///
    /// @param s: 被视作路径的字符串。
    /// @return 返回正规化之后的路径。
    static auto normpath(std::string_view s) -> std::string;
    static auto normpath_inplace(std::string& s) -> std::string&;

#ifdef STR_UNIMPL
    //! 路径处理 @anchor{is_absolute, is_relative}
    ///
    /// 将字符串 s 视作文件路径，并检查其是否是绝对路径的形式或者相对路径的形式
    static auto is_absolute(std::string_view s) -> bool;
    static auto is_relative(std::string_view s) -> bool;
#endif // STR_UNIMPL

    //! 路径的基本部分的长度 @anchor{basename_pos, extname_pos, dirname_pos}
    ///
    /// * @ref{basename_pos} 返回路径 `s` 中的文件的基本名（basename）的长度。
    /// * @ref{dirname_pos} 返回路径 `s` 中的文件的目录部分（dirname）的长度，如果 `s` 是目录，返回其父目录。
    /// * @ref{extname_pos} 返回路径 `s` 中的文件的的基本名中的扩展名部分（extname）。
    ///
    /// @param s: 被视作路径的字符串
    /// @return  返回路径几个基本部分的路径的长度
    static auto basename_pos(std::string_view s) -> size_type;
    static auto dirname_pos(std::string_view s) -> size_type;
    static auto extname_pos(std::string_view s) -> size_type;

    //! 路径处理函数：目录 @anchor{dirname}
    ///
    /// @ref{dirname_view, dirname, dirname_inplace} 获取路径 `s` 的目录部分
    /// @ref{remove_dirname_view, remove_dirname, remove_dirname_inplace} 删除路径 `s` 的目录部分，返回剩余部分
    /// @ref{replace_dirname, replace_dirname_inplace} 使用 `new_dir` 替换路径 `s` 中的目录部分
    /// @ref{split_diename} 将路径 `s` 拆分为目录部分和剩余部分
    ///
    /// @param s: 路径字符串
    /// @param new_dir: 表示被替换成的新名字，用于 replace_xxx 函数
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
#ifdef STR_UNTESTED
    static auto remove_dirname_inplace(std::string& s) -> std::string&;
    static auto replace_dirname_inplace(std::string& s, std::string_view new_name) -> std::string&;
#endif // STR_UNTESTED

    //! 路径处理函数：文件名 @anchor{basename}
    ///
    /// @ref basename_view, basename, basename_inplace: 获取路径 `s` 的文件名部分
    /// @ref remove_basename_view, remove_basename, remove_basename_inplace: 删除路径 `s` 的文件名部分，返回剩余部分
    /// @ref replace_basename, replace_basename_inplace: 使用 `new_name` 替换路径 `s` 中的文件名
    /// @ref split_basename: 将路径 `s` 拆分为文件名和剩余部分
    ///
    /// @param s: 路径字符串。
    /// @param new_name: 表示被替换成的新名字。
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
#ifdef STR_UNTESTED
    static auto remove_basename_inplace(std::string& s) -> std::string&;
    static auto replace_basename_inplace(std::string& s, std::string_view new_name) -> std::string&;
#endif // STR_UNTESTED

    //! 路径处理函数：扩展名 @anchor{extname}
    ///
    /// @ref extname_view, extname, extname_inplace: 获取路径 s 的扩展名部分。
    /// @ref remove_extname_view, remove_extname, remove_extname_inplace: 删除路径 s 的扩展名，返回剩余部分。
    /// @ref replace_extname, replace_extname_inplace: 使用 new_name 替换路径 s 中的扩展名。
    /// @ref split_extname: 将路径 s 拆分为扩展名和剩余部分。
    ///
    /// @param s: 路径字符串。
    static auto extname_range(std::string_view s) -> range_type;
    static auto extname_view(std::string_view s) -> std::string_view;
    static auto extname(std::string_view s) -> std::string;
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

    //! 路径处理函数：裸文件名 @anchor{rawname}
    ///
    /// @notice{1} 这里的“裸文件名”是指不包含文件后缀名的部分，等价于 basename 中去掉 extname 之后的部分。
    ///
    /// @ref rawname_view, rawname, rawname_inplace: 获取路径 s 的中裸文件名部分
    /// @ref replace_rawname, replace_rawname_inplace: 使用 new_name 替换路径 s 中的裸文件名
    /// @ref split_rawname: 将路径 s 拆分为目录、裸文件名，扩展名三个部分
    ///
    /// @param s: 路径字符串
    static auto rawname_range(std::string_view s) -> range_type;
    static auto rawname_view(std::string_view s) -> std::string_view;
    static auto rawname(std::string_view s) -> std::string;
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

    //! 转义：C语言字符串 @anchor{encode_cstr}
    ///
    /// 将 s 中属于 C 语言字符串中的特殊字符，转义为 C 语言字符串的转义表示形式，或者反过来。
    ///
    /// @param s: 被编码或者解码的字符串
    /// @param proc: 用于接收转换后，生成的字符串
    /// @return 返回编码或者解码后的字符串
    static auto encode_cstr(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto encode_cstr(std::string_view s) -> std::string;
    static auto decode_cstr(std::string_view s, const view_consumer_proc& proc) -> size_type;
    static auto decode_cstr(std::string_view s) -> std::tuple<size_type, std::string>;
    /// -
    static auto encode_cstr_inplace(std::string& s) -> std::string;
    static auto decode_cstr_inplace(std::string& s) -> size_type;

    //! 编解码：base64 @anchor{encode_base64}
    ///
    /// 将 s 中属于 url 的特殊字符，转义为 url 中可表示的形式，或者反过来
    ///
    /// @param s: 被编码或者解码的字符串
    /// @param proc: 用于接收转换后，生成的字符串
    /// @return 返回编码或者解码后的字符串
    static auto encode_base64(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto encode_base64(std::string_view s) -> std::string;
    static auto decode_base64(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto decode_base64(std::string_view s) -> std::string;
    //
    static auto encode_base64_inplace(std::string& s) -> std::string&;
    static auto decode_base64_inplace(std::string& s) -> std::string&;

    //! 编解码：base16 @anchor{encode_base16}
    ///
    /// 将 s 中属于 url 的特殊字符，转义为 url 中可表示的形式，或者反过来
    ///
    /// @param s: 被编码或者解码的字符串
    /// @param proc: 用于接收转换后，生成的字符串
    /// @param upper: 转换出来的十六进制是否采用大写形式
    /// @return 返回编码或者解码后的字符串
    static auto encode_base16(std::string_view s, bool upper, const view_consumer_proc& proc) -> void;
    static auto encode_base16(std::string_view s, bool upper = false) -> std::string;
    static auto decode_base16(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto decode_base16(std::string_view s) -> std::string;
    /// -
    static auto encode_base16_inplace(std::string& s, bool upper = false) -> std::string&;
    static auto decode_base16_inplace(std::string& s) -> std::string&;

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

    //! 数据按十六进制 dump @anchor{dump_hex}
    ///
    /// 按照十六进制编辑器的样式格式化数据
    ///
    /// @param data, len: 被格式化的数据的地址和数据长度
    /// @param format: 指定 dump_hex 时的格式化信息，参考 @ref{dump_hex_format}
    /// @param proc: 用于接收格式化数据
    static auto dump_hex(const void* data, size_type len, const dump_hex_format& format, const line_consumer_proc& proc) -> void;

    //! 求和 @anchor{sump}
    ///
    /// 将字符串 `s` 中的每个字符先用 `proc` 做映射，然后做求和计算，并返回最终的结果。
    ///
    /// @param s: 被映射的字符串
    /// @param proc: 映射函数
    /// @return 返回求和后的结果
    template <typename T>
    static auto sum(std::string_view s, const mapping_proc<T>& proc) -> T;

    //! 字符集对象生成 @anchor{charset}
    ///
    /// @ref{charset} 用于将 `s` 中的所有字符设置到字符集对象中。
    ///
    /// @param s: 从 `s` 中的每个不重复字符作为字符集的一部分
    /// @return 返回生成的字符集
    static auto charset(std::string_view s) -> charset_type;

    //! 范围对象生成 @anchor{range, interval, shifter}
    ///
    /// * @ref{range} 用于根据指定的 `pos` 和 `n` 生成一个 @ref{range_type} 类型的范围对象。
    /// * @ref{interval} 用于根据指定的 `begin` 和 `end` 生成一个 @ref{interval_type} 类型的范围对象。
    /// * @ref{shifter} 用于给定起始点 `pos` 以及偏移量 `offset` 来数据的范围，其返回的是 @ref{shifter_type} 类型的对象。
    ///
    /// @notice{1} 需要特别注意 range 在某些边界场景下并不是完全等价的。比如:
    /// `str::range(pos, n)` 并非总是等价于 `str::interval(pos, (pos + n))`，因为考虑到 `pos` 或者 `n`
    /// 的值可能为 `str::npos`，此时简单地用 `(pos + n)` 做等价表示。
    ///
    /// @notice{2} @ref{shifter} 函数相对比较特殊，其 `offset` 参数可以为正值也可以为负值。如果 `offset` 为负值，
    /// 可以理解为 `(pos + offset)` 到 `pos` 的范围（不包含 `pos`）。如果 `offset` 为正值，可以理解为 `pos`
    /// 到 `(pos + offset)` 的范围。当然在遇到具体字符串时，会根据具体字符串的长度调整。
    static auto range(size_type pos, size_type n) -> range_type;
    static auto interval(size_type begin, size_type end) -> interval_type;
    static auto shifter(size_type pos, ssize_type offset) -> shifter_type;

    //! 读取一行 @anchor{read_next_line}
    ///
    /// 以迭代的方式，从指定的文件读取一行文字并返回，本函数可以连续调用，以实现逐行读取效果。但如果需要逐行
    /// 读取文件的功能，可参考 @ref{read_lines}。
    ///
    /// @param filename, file: 指定计划读取的文件的文件名，或者一个已经打开的文件对象，或者文件输入流对象
    /// @param keeo_ends: 是否保留行尾的续行符
    static auto read_next_line(FILE* file, bool keep_ends, std::string& line_text) -> bool;
    static auto read_next_line(FILE* file, bool keep_ends = false) -> std::optional<std::string>;
    static auto read_next_line(std::istream& file, std::string& line_text) -> bool;
    static auto read_next_line(std::istream& file) -> std::optional<std::string>;

    //! 读取文件的全部内容 @anchor{read_all}
    ///
    /// 将文本文件 filename 中的所有内容读取出来并返回。需要注意，读取大文件很容易触发内存分配失败。
    /// 因此，read_all 主要为尺寸比较小的文本文件提供了简便的函数。
    ///
    /// @param filename: 待读取文件内容的文件名
    /// @return 返回读取的文本文件的内容。
    static auto read_all(const std::string& filename) -> std::string;
    static auto read_all(const char* filename) -> std::string;

    //! 按行读取多行 @anchor{read_lines}
    ///
    /// 从指定的文件（`FILE*` 或者 `std::istream` 或者 `filename`）中连续读取多行文本。
    ///
    /// @notice{1} 本系列函数和 @ref{read_all} 类型，只适合文件内容较少的文本文件的读取，并不适合大型文件。
    /// 如果需要按行读取大型文件内容，可考虑基于 @ref{read_next_line} 系列函数自行实现。
    ///
    /// @param file, filename: 指定读取的数据的来源，如果是`FILE*` 或者 `std::istream` 类型的形式，表示从该文
    /// 件的当前位置开始读取。如果是 `std::string` 或者 `const char*` 形式，将自动打开文件，并逐行读取文件所
    /// 有内容。
    /// @param keep_ends: 指定指定是否需要保留每行行位的结束符（如果有的话）。
    /// @param max_n: 用于限制读取行数，当年读取的行数达到 max_n 时，自动停止读取。
    /// @param proc: 通过 proc 函数接收行输出。
    /// @return 通过 proc 或者返回值返回读取到的多行。
    static auto read_lines(FILE* file, bool keep_ends, const line_consumer_proc& proc) -> void;
    static auto read_lines(FILE* file, bool keep_ends = false, size_type max_n = npos) -> std::vector<std::string>;
    static auto read_lines(std::istream& file, const line_consumer_proc& proc) -> void;
    static auto read_lines(std::istream& file, size_type max_n = npos) -> std::vector<std::string>;
    /// -
    static auto read_lines(const std::string& filename, bool keep_ends, const line_consumer_proc& proc) -> void;
    static auto read_lines(const char* filename, bool keep_ends, const line_consumer_proc& proc) -> void;
    static auto read_lines(const std::string& filename, bool keep_ends = false, size_type max_n = npos) -> std::vector<std::string>;
    static auto read_lines(const char* filename, bool keep_ends = false, size_type max_n = npos) -> std::vector<std::string>;

    //! 打开文件并自动关闭文件 @anchor{with_file}
    ///
    /// @ref{with_file} 根据指定的路径 `filepath` 和 `mode` 打开一个文件，并将这个文件交个 `proc` 函数使用。
    /// 当 `proc` 函数使用完毕后，@ref{with_file} 函数会自动关闭该文件。本函数设计的相对特殊，其核心目的是
    /// 避免用户去处理打开和关闭文件的操作。在一定程度上可以避免文件忘记关闭问题。
    ///
    /// @param filepath: 指定文件路径
    /// @param mode: 文件打开的模式
    /// @param repl: 当遇到任何问题导致打开文件失败时，自动采用 repl 参数指定的文件对象代替。需要
    /// 注意，@with_file 函数并不会自动关闭 `repl` 文件。
    /// @param proc: 用于接收并使用被 @ref{with_file} 打开的文件对象的函数。
    static auto with_file(const std::string& filepath, const char* mode, FILE* repl, const std::function<void(FILE* f)>& proc) -> void;
    static auto with_file(const std::string& filepath, const char* mode, const std::function<void(FILE* f)>& proc) -> void;

    //! 命令行选项识别 @anchor{opt1}
    ///
    /// 将一组字符串列表视作命令行的参数（选项）序列，@ref{next_opt1} 函数从该序列中按照特定的模式读取和识别
    /// 出单个的命令行选项。
    ///
    /// @notice{1} 与很多其他的命令行参数识别库不同，@anchor{next_opt1} 函数所支持的命令行参数的识别模式是一
    /// 种无 schema 的模式，其优点是不需要在参数识别前定义一堆的 schema 信息（比如，需要定义有哪些命令行参
    /// 数，每个参数的类型），所以使用起来相对方便快捷。但，相应的，@anchor{next_opt1} 无法实现很复杂的命令
    /// 行参数设计。@anchor{next_opt1} 很适合具有少量命令行参数的情况。下面是 @anchor{next_opt1} 函数的识别算
    /// 法：
    ///
    /// * `-` 为选项识别符，所有以 `-` 开头的串均会作为键值对形式的参数，除非在此之前用 `--` 转义；
    /// * `-key` 定义一个独立的、无 `value` 选项，常常用来定义开关型的选项；`key` 部分为空也是允许的；
    /// * `-key=value` 识别为名字为 `key` 且值为 `value` 的键值对参数；若 `value` 部分为空，与 `-key` 等价；
    /// * `value` 识别为一个没有 `key`，但是有 `value` 的参数；
    /// * `-- value` 用于对选项识别符号进行转义，用于处理 value 部分本身已以 `-` 开头的情况；
    ///
    /// @param next_index: 从该位置的字符串开始识别下一个选项。
    /// @param argc, argv: 指定命令行参数序列的大小和起始地址，常用于匹配 `main` 函数的参数。
    /// @param items: 存放命令行参数的容器，用于代替 `argc` 与 `argv` 的组合。
    /// @return 以键值对的形式返回读取到的选项，并提前将 next_index 移动到选项的结尾。
    static auto next_opt1(int& next_index, int argc, const char* argv[]) -> std::optional<pair<std::string_view>>;
    static auto next_opt1(int& next_index, int argc, char* argv[]) -> std::optional<pair<std::string_view>>;
    template <typename Container, typename SizeType = typename Container::size_type>
    static auto next_opt1(SizeType& next_index, const Container& items) -> std::optional<pair<std::string_view>>;
    template <typename Iterator>
    static auto next_opt1(Iterator& itr, Iterator end) -> std::optional<pair<std::string_view>>;
    template <typename IterProc>
    static auto next_opt1(const IterProc& proc) -> std::optional<pair<std::string_view>>;

    //! 命令行选项识别 @anchor{opt2}
    ///
    /// 将一组字符串列表视作命令行的参数（选项）序列，@ref{next_opt2} 函数从该序列中按照特定的模式读取和识别
    /// 出单个的命令行选项。
    ///
    /// @notice{1} 与很多其他的命令行参数识别库不同，@anchor{next_opt2} 函数所支持的命令行参数的识别模式是一
    /// 种无 schema 的模式，其优点是不需要在参数识别前定义一堆的 schema 信息（比如，需要定义有哪些命令行参
    /// 数，每个参数的类型），所以使用起来相对方便快捷。但，相应的，@anchor{next_opt2} 无法实现很复杂的命令
    /// 行参数设计。@anchor{next_opt2} 很适合具有少量命令行参数的情况。下面是 @anchor{next_opt2} 函数的识别算
    /// 法：
    ///
    /// * `-` 为选项识别符，所有以 `-` 开头的串均会作为键值对形式的参数，除非在此之前用 `--` 转义；
    /// * `-key value` 识别为名字为 `key` 且值为 `value` 的键值对参数；若 `value` 部分为空，与 `-key` 等价；
    /// * `-key -` 定义一个独立的、无 `value` 选项，常常用来定义开关型的选项；`key` 部分为空也是允许的；
    /// 如果 `-key` 后面是另一个 key，那么，`-` 可以省略
    /// * `value` 识别为一个没有 `key`，但是有 `value` 的参数；
    /// * `-- value` 用于对选项识别符号进行转义，用于处理 value 部分本身已以 `-` 开头的情况；
    ///
    /// @param next_index: 从该位置的字符串开始识别下一个选项。
    /// @param argc, argv: 指定命令行参数序列的大小和起始地址，常用于匹配 `main` 函数的参数。
    /// @param items: 存放命令行参数的容器，用于代替 `argc` 与 `argv` 的组合。
    /// @return 以键值对的形式返回读取到的选项，并提前将 next_index 移动到选项的结尾。
    static auto next_opt2(int& next_index, int argc, const char* argv[]) -> std::optional<pair<std::string_view>>;
    static auto next_opt2(int& next_index, int argc, char* argv[]) -> std::optional<pair<std::string_view>>;
    template <typename Container, typename SizeType = typename Container::size_type>
    static auto next_opt2(SizeType& next_index, const Container& items) -> std::optional<pair<std::string_view>>;
    template <typename Iterator>
    static auto next_opt2(Iterator& itr, Iterator end) -> std::optional<pair<std::string_view>>;

    //! 简单词法识别 @anchor{accept}
    ///
    /// * @ref{accept_until} 从 `pos` 位置开始逐个扫描 `s` 中的每个字符，如果遇到哨兵字符或者字符串
    /// 或者其他形式的哨兵，则返回截止到哨兵起始位置的字符范围。当指定 `escape` 字符时，表示扫描到 `escape` 字符时，
    /// 自动忽略下一个字符，即使这个字符就是哨兵字符本身。最终，accept_until 会返回哨兵之前的字符序列的范围。
    ///
    /// * @ref{accept} 从 `pos` 位置开始逐个扫描 `s` 中的每个字符，收集满足条件的字符序列，
    /// 直到下一个字符无法满足条件为止。最终，accept 会返回满足条件的字符序列的范围。
    ///
    /// @notice{0} 如果扫描失败，那么所有函数都会返回 `std::nullopt`，且输出参数 `pos` 不会发生改变。
    ///
    /// @notice{1} accept 和 accept_until 这两组函数的行为的重要区别是：accept 系列函数总是 '盯着' 满足
    /// 条件的字符，而 accept_until 总是 '盯着' 不满足条件的字符。
    ///
    /// @notice{2} 虽然 accept 和 accept_until 这两组函数都是可以连续调用的，但是需要注意 accept_until 总是
    /// 会自动跳过已经识别出来的哨兵。因此，如果连续调用 accept_until，总是不可能得到哨兵序列的范围。如果确
    /// 实有必要获得的哨兵字符串，可以通过 pos 与返回的 range_type 来组合计算出来。如下示例：
    ///
    /// ```c++
    /// auto range = str::accept_until(s, pos, token);
    /// auto token_range = str::range(range->end_pos(), pos - range->end_pos());
    /// ```
    ///
    /// @param s: 待扫描的字符串
    /// @param pos: 扫描的起始位置，该参数为输入输出参数。如果符号识别成功，对于 accept_until 来说，pos 总是
    /// 位于哨兵序列最后一个字符的下一个字符；而对于 accept 由于并没有哨兵，它总是指向找到的第一个不满足条件
    /// 的字符的位置。
    /// @param guard_ch: 以单个字符作为哨兵
    /// @param guard_charset: 以字符集（多个可选的字符）为哨兵
    /// @param guard_proc: 更抽象的单字符的哨兵，在扫描时只要字符满足条件即表示遇到哨兵
    /// @param guard_token: 以一个字符序列为哨兵
    /// @param guard_pattern: 以一个正则表达式为哨兵（需要确保不匹配到空串）
    /// @param escape: 扫描过程中如果遇到 escape 字符，将自动忽略下一个字符，以实现字符转义的效果
    /// @param expect_ch: 用于 accept 函数，表示待识别的字符
    /// @param expect_token: 用于 accept 函数，表示待识别的字符串
    /// @param expect_pattern: 用于 accept 函数，表示需要识别的字符序列的模式
    /// @param expect_charset: 用于 accept 函数，表示待识别的满足条件的字符集
    /// @param expect_proc: 用于 accept 函数，表示待识别的字符需要满足的条件
    /// @return: accept_until 系列函数总是返回找到的哨兵字符序列之前的字符序列的范围；而 accept 系列
    /// 函数总是返回满足条件的字符序列的范围。
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

    //! @anchor skip: 跳过满足条件的字符
    ///
    /// * @ref{skip_n} 从 `pos` 开始跳过 `n` 个字符，并返回 `true`；如果长度不够，返回 `false`；
    /// * @ref{skip_max} 从 `pos` 开始跳过最多 `max_n` 个字符，跳过实际跳过的长度；
    /// * @ref{skip_spaces} 从 `pos` 开始，跳过所有空白字符，如果有的话；
    ///
    /// @param s: 被视作符号识别的缓冲区的字符串
    /// @param pos: 作为输入参数是表示指定识别的起始位置，作为输出参数时表示下一个还未被识别的字符的位置
    /// @param n: 跳过 n 个字符
    /// @param max_n: 最多跳过 max_n 个字符
    /// @return 如果识别成功，将返回符号的范围，如果识别失败，返回的范围对象长度为 0，如果 pos 已经不在 s 的范围内，pos 的值
    ///         将大于或者等于 `s.size()`。因此，可以通过测试 `(pos >= s.size())` 来确定是否所有数据已经识别完。
    static auto skip_n(std::string_view s, size_type& pos, size_type n) -> bool;
    static auto skip_max(std::string_view s, size_type& pos, size_type max_n) -> size_type;
    static auto skip_spaces(std::string_view s, size_type& pos) -> void;

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

    //! 用户根目录 @anchor{home}
    ///
    /// 返回当前用户的根目录，等价于 `*nix` 下的 `${HOME}` 环境变量的值，主要被用于 @ref{expand_user} 函数。
    ///
    /// @notice{1} 由于 Windows 下并无严格意义上的与 `*nix` 下对等的用户根目录的概念，因此，
    /// 在 Windows 下会以 `USERPROFILE` 环境变量的值来作为 `${HOME}` 的值。
    static auto home() -> std::string;
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
    return next_opt1([&itr, &end]()-> std::optional<std::string_view> {
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

#endif // TINY_STR_H
