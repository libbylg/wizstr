
# 简介

str 提供了一系列字符串处理函数算法库，
目标是成为 C++ 语言功能最丰富的函数库。

当前提供了下列算法：

* 追加插入（[append](#)、[prepend](#) 和 [insert](#) 系列）
* 大小写不敏感的比较（[icompare](#) 和 [iequals](#) 系列）
* 基于通配符匹配（[wildcmp](#) 系列）
* 两字符串之间的关系（[contains](#) 系列）
* 特征字符串统计（[count](#) 系列）
* 前后缀操作（[prefix](#) 和 [suffix](#) 系列）
* 查找（[find](#) 和 [iter](#) 系列）
* 特征测试（[is](#)、[has](#) 系列）
* 子串提取（[take](#)、[drop](#)、[range](#) 系列）
* 修剪和整形（[align](#)、[surround](#)、[unsurround](#)、[invert](#)、[simplified](#)、[trim](#) 列）
* 按多行处理（[lines](#) 系列）
* 按单词处理（[words](#) 系列）
* 字符串生成（[repeat](#)、[random](#) 系列）
* 空白串处理（[spaces](#) 系列）
* 字符串遮罩（[cover](#) 系列）
* 字符串拆分（[split](#)、[partition](#)、[chunked](#)、[windowed](#) 系列）
* 字符串拼接（[join](#) 系列）
* 大小写转换（[to](#) 系列）
* 变量展开（[expand](#) 系列）
* 文件名路径操作（[basemame](#)、[extname](#)、[dirname](#) 系列）
* 字符串哈希算法（[hash](#) 系列）
* 字符串转义（[encode](#)、[decode](#) 系列）
* 文本文件读取（[read](#) 系列）
* 字符分组和筛选（[grouping](#) 和 [filter](#) 系列）

关于函数的返回值及其使用注意事项：

str 中提供的函数根据返回值的不同可以分为三种不同的形式，使用者需要根据情况合理地选择。

* `xxx_view` 形式：

通常意味着该函数的返回值是输入参数的一个（或多个）视图，该函数不会发生任何分配行为（返回存放
容器的 `std::string_view`，如 `std::vector<std::string_view>` 类似的除外）。但这种形式的接口是**不安全**的



时也需要格外注意，其返回值可能会因为输入参数提前析构，导致失效。所以在调用这些接口时，需要确保在使用
前其输入参数的地址仍然是有效的。


* `xxx_inplace` 形式：

这类函数通常意味着该函数返回的是输入参数自身，返回值也通常是 `std::string&`。该函数在执行
过程中通常会修改输入参数，并返回。如果使用这类函数，需要确保原始输入串是可以被改写的，否则建议使用 `xxx_view 形式`或
者 `xxx 形式` 的函数代替。

* `xxx` 形式：

与前面几种对应，这类不带 `_view` 或者 `_inplace` 后缀的函数，其返回值不是原始输入的视图，而是一个新的字符串拷贝。
因此，这类函数既没有类似 `_view` 系列函数那样的返回值依赖于输入参数的生存期的问题，也没有类似 `xxx_inplace` 那样会修改
原始输入参数的问题。但这类函数由于总是会拷贝原始输入字符串的，所以如果返回的字符串无法充分利用字符串的 SSO 特性，
那么性能会比 `xxx_view` 和 `xxx_inplace` 系列要低一些。当然这类函数的优点也是显而易见的，就是更`安全`。

## 常用类型

*| class charset_type | 字符集 |
*| struct range_type  | 基于位置的范围类型|

## 常用函数

*| @ref{view_provider_proc}       |   std::string_view 供给器：每次调用返回一个字符串，直到返回 std::nullopt  |
*| @ref{view_pair_provider_proc}  |   键值对供给器：每次调用返回一个 key-value 对组成的 tuple，直到返回 std::nullopt  |
*| @ref{number_provider_proc}     |   整数供给器：每次调用返回一个 size_type 类型的整数，主要用于抽象随机函数     |
*| @ref{view_consumer_proc}       |   std::string_view 消费器：接收一个 std::string_view，常用于接收一个或者多个字符串，如果需要提前结束，可以返回 0  |
*| @ref{view_pair_consumer_proc}  |   键值对消费器：接收两个 std::string_view 类型的键值对（key 和 value 参数），如果需要提前结束，可以返回 0     |
*| @ref{range_consumer_proc}      |   range 消费器：接收一个 range_type 类型的参数，常用于表示找到的子串的范围，如果需要提前结束，可以返回 0  |
*| @ref{char_match_proc}          |   单字符匹配器：检查作为输入参数的字符是否满足特定的条件，常用基于字符的查找和搜索场景    |
*| @ref{range_search_proc}        |   字符串视图检索器：在指定的范围内查找，如果找到返回找到的子串，否则返回 std::nullopt |
*| @ref{line_consumer_proc}       |   行消费器：接收一个行索引和行文字，常用于字符串按行、分割读取等场景，如果需要提前结束，可以返回 0    |
*| @ref{mapping_proc}             |   单字符映射：将单个字符映射为另一个数据类型的值  |
*| @ref{char_mapping_proc}        |   单字符映射器：将一个字符映射为另一个字符    |
*| @ref{string_mapping_proc}      |   字符串映射：将一个字符串映射为另一个字符串，如果能成功映射返回映射后的字符串，否则返回 std::nullopt     |

## 常用常量：操作系统相关

*| @ref{sep_search_path, sep_search_path_char}  |  搜索路径分隔符 |             
*| @ref{sep_path, sep_path_char}                |  文件路径分隔符 |             
*| @ref{sep_line_ends}                          |  行结束符       |         


## 常用常量：字符分类   

*| @ref{all_uppers}   | 所有大写字母集合    |
*| @ref{all_lowers}   | 所有小写字母集合    |
*| @ref{all_leters}   | 所有字母集合    |
*| @ref{all_alphas}   | 所有字母集合    |
*| @ref{all_digits}   | 所有数字字符    |
*| @ref{all_xdigits}  | 所有十六进制数字表示的字符集合  |
*| @ref{all_alnums}   | 所有的字母和数字集合    |
*| @ref{all_alnumuls} | 所有的字母、数字、下划线的集合  |
*| @ref{all_aluls}    | 所有字母和下滑线的集合  |
*| @ref{all_spaces}   | 所有空白字符    |

## 在尾部追加 @anchor{append}

### append、append_inplace

提供了向指定字符尾部追加一个或者多个字符串的能力。实际上，STL 中已经提供了比较丰富的追加字符串，这里针对
大量字符串拼接提供了相对简便的方法。
对于 append_inplace 函数，如果 s 与 被插入字符串存在重叠时，函数的行为是不确定的，应该避免出现这种情况。

*| @param{s}        |   指定向哪个字符串后添加新串。    |
*| @param{other}    |   被追加的字符串。    |
*| @param{n}        |   重复追加多少次，如果指定为 0，则实际不会做任何追加操作。    |
*| @param{proc}     |   由 proc 函数提供被追加的字符串，如果 proc 返回 std::nullopt，表示后续无更多字符串需要追加。 |
*| @param{items}    |   从容器 items 中获取被追加的字符串。 |

<table>
<tr>
<td>
<code>
static auto append(std::string_view s, std::string_view other, size_type n = 1) -> std::string;
</code>
</td>
<td>
(1)
</td>
</tr>
<tr>
<td>
<code>
static auto append(std::string_view s, const view_provider_proc& proc) -> std::string;
</code>
</td>
<td>
(2)
</td>
</tr>
<tr>
<td>
<code>
template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
static auto append(std::string_view s, const Sequence& items) -> std::string;
</code>
</td>
<td>
(3)
</td>
</tr>
<tr>
<td>
<code>
static auto append_inplace(std::string& s, std::string_view other, size_type n) -> std::string&;
</code>
</td>
<td>
(4)
</td>
</tr>
<tr>
<td>
<code>
static auto append_inplace(std::string& s, const view_provider_proc& proc) -> std::string&;
</code>
</td>
<td>
(5)
</td>
</tr>
<tr>
<td>
<code>
template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
static auto append_inplace(std::string& s, const Sequence& items) -> std::string&;
</code>
</td>
<td>
(6)
</td>
</tr>
<tr>
<td>
<code>
</table>

    //! 向头部追加 @anchor{prepend}
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
    static auto prepend(std::string_view s, const Sequence& items) -> std::string;
    //
    static auto prepend_inplace(std::string& s, std::string_view other, size_type n) -> std::string&;
    static auto prepend_inplace(std::string& s, const view_provider_proc& proc) -> std::string&;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto prepend_inplace(std::string& s, const Sequence& items) -> std::string&;

    //! 向字符串中间插入 @anchor{insert}
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
    static auto insert(std::string& s, size_type pos, const Sequence& items) -> std::string&;
    //
    static auto insert_inplace(std::string& s, size_type pos, std::string_view other, size_type n = 1) -> std::string&;
    static auto insert_inplace(std::string& s, size_type pos, value_type ch, size_type n = 1) -> std::string&;
    static auto insert_inplace(std::string& s, size_type pos, const view_provider_proc& proc) -> std::string&;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto insert_inplace(std::string& s, size_type pos, const Sequence& items) -> std::string&;

    //! 不区分大小写的比较 @anchor{icompare}
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

    //! 不区分大小写的相等測試 @anchor{iequals}
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

    //! 基于通配符的匹配检测 @anchor{wildcmp}}
    ///
    /// 测试字符串 s 是否匹配通配符 pattern。
    ///
    /// @param s 被测试的字符串
    /// @param pattern 通配串
    /// @return 如果 s 字符串匹配 pattern，返回 true，否则返回 false。
    static auto wildcmp(const_pointer s, const_pointer pattern) -> bool;
    static auto wildcmp(std::string_view s, std::string_view pattern) -> bool;

    //! 判断两个字符串的包含关系 @anchor{contains}
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

    //! 子串统计 @anchor{count}
    ///
    /// 本函数用于统计 s 串中是否包含特定模式的子串的数量。需要注意，count 函数统计的子串是不重叠的子串。
    ///
    /// @param s 在该字符串中查找
    /// @param other, ch, charset 被统计的子串或者字符或者字符集
    /// @param proc 用于表示满足特定条件字符
    /// @param pattern 用于统计满足表达式的子串的数量
    /// @param ignore_case 如果为 true，表示忽略大小写差异，否则，表示采用大小写敏感的方式查找。
    /// @return 返回满足条件的子串或者字符的数量。特别的，当 s 或者 other 为空时，总是返回 0
    static auto count(std::string_view s, std::string_view other, bool ignore_case = false) -> size_type;
    static auto count(std::string_view s, value_type ch, bool ignore_case = false) -> size_type;
    static auto count(std::string_view s, const char_match_proc& proc) -> size_type;
    static auto count(std::string_view s, const charset_type& charset) -> size_type;
    static auto count(std::string_view s, const std::regex& pattern) -> size_type;

    //! 前缀操作 @anchor{prefix}
    ///
    /// 本组函数提供了常见的前缀操作：
    ///
    /// * @ref{prefix} 提供了计算两个字符串的公共前缀的能力；
    /// * @ref{has_prefix, starts_with} 功能一致，都用于测试字符串 s 是否有指定的前缀。
    /// * @ref{remove_prefix_view, remove_prefix, remove_prefix_inplace} 会返回从字符串 s 中去除两个字符串的共同前缀后剩余的部分。
    ///
    /// @param s 目标字符串
    /// @param other 参与公共前缀计算的另一个字符串，用于 @ref{prefix}
    /// @param prefix, ch 前缀字符串或者字符
    static auto prefix(std::string_view s, std::string_view other) -> size_type;
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

    //! 后缀操作 @anchor{suffix}
    ///
    /// 本组函数提供了常见的前缀操作:
    ///
    /// * @ref{suffix} 提供了计算两个字符串的公共前缀的能力；
    /// * @ref{has_suffix, ends_with} 功能一致，都用于测试字符串 s 是否有指定的后缀。
    /// * @ref{remove_suffix_view, remove_suffix, remove_suffix_inplace} 会返回从字符串 s 中去除两个字符串的共同后剩余的部分。
    ///
    /// @param s 目标字符串
    /// @param suffix, ch 后缀字符串或者字符
    static auto suffix(std::string_view s, std::string_view other) -> size_type;
    static auto has_suffix(std::string_view s, value_type ch) -> bool;
    static auto has_suffix(std::string_view s, std::string_view suffix) -> bool;
    static auto ends_with(std::string_view s, value_type ch) -> bool;
    static auto ends_with(std::string_view s, std::string_view suffix) -> bool;
    static auto remove_suffix_view(std::string_view s, std::string_view suffix) -> std::string_view;
    static auto remove_suffix_view(std::string_view s, value_type ch) -> std::string_view;
    static auto remove_suffix(std::string_view s, std::string_view suffix) -> std::string;
    static auto remove_suffix(std::string_view s, value_type ch) -> std::string;
    //
    static auto remove_suffix_inplace(std::string& s, std::string_view suffix) -> std::string&;
    static auto remove_suffix_inplace(std::string& s, value_type suffix) -> std::string&;

    //! 查找下一个子串  @anchor{iter, find}
    ///
    /// 从指定的 pos 位置开始，查找并返回满足匹配条件的字符串或者位置。需要注意 `find_xxx` 系列函数的 pos 参数是值类型的，
    /// 而 `iter_xxx` 系列在的 pos 类型是引用类型。所以，`iter_xxx` 系列函数在每次查找之后都会修改 pos，以便再次调用时可以
    /// 继续查找下一个满足条件的字符串。因此，如果只是简单地查找一次 `find_xxx` 系列函数显然更方便，如果需要在同一输入串中
    /// 联系多次，查找多个满足匹配条件的子串，用 `iter_xxx` 系列函数会更方便。
    ///
    /// @param s 从该字符串中查找子串
    /// @param pattern 子串需要满足的正则表达式
    /// @param other 需要匹配的子串
    /// @param pos 查找字符串的起始位置，对于该参数为引用类型的函数，可以通过测试 `pos == str::npos` 来决定是否已经找到
    ///            输入串 s 的结尾。通常也用于判断是否找到需要的子串
    /// @return 不同函数返回值有些差别，其中一个比较重要的场景是“如何判断并未在 s 找到满足条件的子串”，不同函数检测方式如下：
    ///     * xxx_next_regex 系列：其返回值是一个 optional 对象，可以通过测试返回值是否未负值判确定是否找到满足条件的子串
    ///     * xxx_string 系列：其返回值是 size_type，如果返回值为 `str::npos`，则表示未找到匹配的串。
    ///     * next_eol 和 next_word 系列：如果返回值的字符串是空串，即表示未找到匹配串
    static auto find_next_regex_view(std::string_view s, const std::regex& pattern, size_type pos = 0) -> std::optional<std::string_view>;
    static auto find_next_regex_view(std::string_view s, std::string_view pattern, size_type pos = 0) -> std::optional<std::string_view>;
    static auto find_next_regex(std::string_view s, const std::regex& pattern, size_type pos = 0) -> std::optional<std::string>;
    static auto find_next_regex(std::string_view s, std::string_view pattern, size_type pos = 0) -> std::optional<std::string>;
    static auto find_next_string(std::string_view s, std::string_view pattern, size_type pos = 0) -> size_type;
    static auto find_next_eol_view(std::string_view s, size_type pos = 0) -> std::string_view;
    static auto find_next_eol(std::string_view s, size_type pos = 0) -> range_type;
    static auto find_next_words_view(std::string_view s, size_type pos = 0) -> std::string_view;
    static auto find_next_words(std::string_view s, size_type pos = 0) -> std::string;
    //
    static auto iter_next_regex_view(std::string_view s, size_type& pos, const std::regex& pattern) -> std::optional<std::string_view>;
    static auto iter_next_regex_view(std::string_view s, size_type& pos, std::string_view pattern) -> std::optional<std::string_view>;
    static auto iter_next_regex(std::string_view s, size_type& pos, const std::regex& pattern) -> std::optional<std::string>;
    static auto iter_next_regex(std::string_view s, size_type& pos, std::string_view pattern) -> std::optional<std::string>;
    static auto iter_next_string(std::string_view s, size_type& pos, std::string_view pattern) -> size_type;
    static auto iter_next_eol_view(std::string_view s, size_type& pos) -> std::string_view;
    static auto iter_next_eol(std::string_view s, size_type& pos) -> std::string;
    static auto iter_next_words_view(std::string_view s, size_type& pos) -> std::string_view;
    static auto iter_next_words(std::string_view s, size_type& pos) -> std::string;

    //! 特征测试：传统类 @anchor{is}
    ///
    /// @ref is_lower 检测 s 中的所有字母都是小写（参考 std::islower）
    /// @ref is_upper 检测 s 中的所有字母都是大写字母（参考 std::isupper）
    /// @ref is_capitalize 检测 s 的首个字符是否为大写字母
    /// @ref is_title  对于给定的字符串 s 中，以空白分割的每个子串(单词)，如果其每个子串的首字符都是非字母或者是大写字母返回 true
    /// @ref is_digit 检测 s 否所有的字符都是数字或者十六进制字符（参考 std::xdigit）
    /// @ref is_xdigit 检测 s 否所有的字符都是数字或者十六进制字符（参考 std::xdigit）
    /// @ref is_ascii 检测 s 中的所有字符是否都在 ASCII 范围内
    /// @ref is_alpha 检测 s 是否全都为字母（参考 std::isalpha）
    /// @ref is_alnum 检测 s 是否全都为字母或者数字（参考 std::isalnum）
    /// @ref is_alnumul 检测 s 是否全都为字母或者数字或者下划线
    /// @ref is_space 检测 s 是否全都为空白字符（参考 std::isspace）
    /// @ref is_blank 检测 s 是否全都为空格字符（参考 std::isblank）
    /// @ref is_print 检测 s 是否全都为可打印字符（参考 std::isprint）
    /// @ref is_graph 检测 s 是否全都为图形符（参考 std::isgraph）
    ///
    /// @param s 被测试的字符串
    /// @return 所有的字符串都必须按组共同的特征，才会返回 true，否则，（包括 s 为空串场景）均返回 false。
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

    //! 特征测试：常见词法特征类
    ///
    /// @ref is_identifier 检查字符串 s 是否满足一个标识符的特征，即以大小写字母或者下划线开头且后续字符为字母数字或者下划线。
    /// @ref is_literal_bool 检查字符串 s 是否是 bool 值的特征，等价于 `(is_literal_true(s) || is_literal_false(s))`。
    /// @ref is_literal_true 检查字符串 s 是否可以被视作 true
    /// @ref is_literal_false 检查字符串 s 是否可以被视作 false
    /// @ref is_literal_integer 检查字符串 s 是否可以被视作整数
    /// @ref is_literal_real 检查字符串 s 是否可以被视作浮点数，需要注意整数本身也可以被视作浮点数
    ///
    /// * 被视作 true 的字符串包括 "1", "on", "ON", "Yes", "yes", "YES", "True", "true", "TRUE"
    /// * 被视作 false 的字符串包括 "0", "off", "OFF", "No", "no", "NO", "False", "false", "FALSE"
    /// * 被视作 real 的字符串，等价于匹配正则表达式 `[+-]?(([0-9]+)|([0-9]+\.)|(\.[0-9]+))([Ee][+-]?[0-9]+)?`
    /// * 被视作 integer 的字符串，等价于匹配正则表达式 `[+-]?[0-9]+`
    /// * 被视作 identifier 的字符串，等价于正则表达式 `[A-Za-z_][0-9A-Za-z_]*`
    ///
    /// @param s 被测试的字符串
    /// @return 所有的字符串都必须按组共同的特征，才会返回 true，否则，（包括 s 为空串场景）均返回 false。
    static auto is_identifier(std::string_view s) -> bool;
    static auto is_literal_bool(std::string_view s) -> bool;
    static auto is_literal_true(std::string_view s) -> bool;
    static auto is_literal_false(std::string_view s) -> bool;
    static auto is_literal_integer(std::string_view s) -> bool;
    static auto is_literal_real(std::string_view s) -> bool;

    //! 特征测试：指定字符集类
    ///
    /// @param s 被测试的字符串
    /// @param proc 用于测试 s 中的每个字符是否满足给定条件的函数
    /// @param charset 指定需要满足条件的字符集
    /// @return 所有的字符串都必须按组共同的特征，才会返回 true，否则，（包括 s 为空串场景）均返回 false。
    static auto is_all(std::string_view s, const char_match_proc& proc) -> bool;
    static auto is_all(std::string_view s, const charset_type& charset) -> bool;

    //! 特征测试：单一条件类 @anchor{has}
    ///
    /// @param s 被测试的字符串
    /// @param proc 用于测试 s 中的每个字符是否满足给定条件的函数
    /// @param charset 指定需要满足条件的字符集
    /// @return 与 is_xxx 系列函数需要“所有字符必须全部满足指定特征”不同，has_xxx 系列函数只需要有任意一个字符满足特征，
    ///         立即返回 true。唯一的特例是空串总是返回 false。
    static auto has_any(std::string_view s, const char_match_proc& proc) -> bool;
    static auto has_any(std::string_view s, const charset_type& charset) -> bool;

    //! 提取子串：基于位置 @anchor{take}
    ///
    /// * @ref take_left_view, take_left, take_left_inplace: 返回字符串 s 的最左边前 n 个字符的子串
    /// * @ref take_right_view, take_right, take_right_inplace: 返回字符串 s 的最右边前 n 个字符的子串
    /// * @ref take_mid_view, take_mid, take_mid_inplace: 返回字符串 s 中，从pos 位置开始的 n个字符组成的子串
    /// * @ref take_range_view, take_range, take_range_inplace: 返回字符串 s 中，range 范围的子串。
    /// * @ref take_view, take, take_inplace: 返回字符串 s 中，从 pos 开始偏移 offset 的字符串。
    ///
    /// @param s 原始字符串
    /// @param n 指定提取的子串的最大长度。当 n 为 0 时，总是返回空串。当按照指定的方式无法获得 n 个字符的长度时，相关函数总是试图返回尽可能多的字符串。
    /// @param offset 该参数是有符号整数，用于指定提取的字符串的长度和提取的方向。如果为负数，表示向左提取，如果为正表示向右提取。offset 的绝对值表示期望提取的数据的长度。
    /// @param pos 用于指定子串提取的起始位置，通常与 n 或者 offset 组合起来确定数据的提取范围
    /// @param begin_pos, end_pos  用于提取字符串的提取范围。
    /// @param range  用于提取字符串的提取范围，一般用在可以替代 begin_pos 和 end_pos 的场景。
    static auto take_left_view(std::string_view s, size_type n) -> std::string_view;
    static auto take_right_view(std::string_view s, size_type n) -> std::string_view;
    static auto take_mid_view(std::string_view s, size_type pos, size_type n) -> std::string_view;
    static auto take_range_view(std::string_view s, size_type begin_pos, size_type end_pos) -> std::string_view;
    static auto take_range_view(std::string_view s, range_type range) -> std::string_view;
    static auto take_view(std::string_view s, size_type pos, ssize_type offset) -> std::string_view;
    static auto take_view(std::string_view s, size_type pos) -> std::string_view;
    //
    static auto take_left(std::string_view s, size_type n) -> std::string;
    static auto take_right(std::string_view s, size_type n) -> std::string;
    static auto take_mid(std::string_view s, size_type pos, size_type n) -> std::string;
    static auto take_range(std::string_view s, size_type begin_pos, size_type end_pos) -> std::string;
    static auto take_range(std::string_view s, range_type range) -> std::string;
    static auto take(std::string_view s, size_type pos, ssize_type offset) -> std::string;
    static auto take(std::string_view s, size_type pos) -> std::string;
    //
    static auto take_left_inplace(std::string& s, size_type n) -> std::string&;
    static auto take_right_inplace(std::string& s, size_type n) -> std::string&;
    static auto take_mid_inplace(std::string& s, size_type pos, size_type n) -> std::string&;
    static auto take_range_inplace(std::string& s, size_type begin_pos, size_type end_pos) -> std::string&;
    static auto take_range_inplace(std::string& s, range_type range) -> std::string&;
    static auto take_inplace(std::string& s, size_type pos, ssize_type offset) -> std::string&;
    static auto take_inplace(std::string& s, size_type pos) -> std::string&;
    //
    static auto take_before_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto take_after_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto take_before(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;
    static auto take_after(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;

    //! 删除子串：基于位置 @anchor{drop}
    ///
    /// @ref drop_left_view, drop_left, drop_left_inplace 返回去掉字符串 s 的最左边前 n 个字符后的子串
    /// @ref drop_right_view, drop_right, drop_right_inplace 返回去掉字符串 s 的最右边的 n 个字符后的子串
    /// @ref drop_mid, drop_mid_inplace 返回去掉字符串 s 中从 pos 开始的 n 的字符后的子串
    /// @ref drop_range, drop_range_inplace 返回去掉字符串 s 中指定范围内的字符后的子串
    /// @ref drop, drop_inplace 返回去掉字符串 s 中特定的字符集后的子串
    ///
    /// @param s 原始字符串
    /// @param n 指定删除的子串的最大长度。
    /// @param offset 该参数是有符号整数，用于指定删除字符串的长度和方向。如果为负数，表示向左删，如果为正表示向右删。offset 的绝对值表示期望提取的数据的长度。
    /// @param pos 用于指定子串提取的起始位置，通常与 n 或者 offset 组合起来确定删除子串范围
    /// @param begin_pos, end_pos  用于提取字符串的提取范围。
    /// @param range  用于提取字符串的提取范围，一般用在可以替代 begin_pos 和 end_pos 的场景。
    static auto drop_left_view(std::string_view s, size_type n) -> std::string_view;
    static auto drop_right_view(std::string_view s, size_type n) -> std::string_view;
    static auto drop_left(std::string_view s, size_type n) -> std::string;
    static auto drop_right(std::string_view s, size_type n) -> std::string;
    static auto drop_mid(std::string_view s, size_type pos, size_type n) -> std::string;
    static auto drop_range(std::string_view s, size_type begin_pos, size_type end_pos) -> std::string;
    static auto drop_range(std::string_view s, range_type range) -> std::string;
    static auto drop(std::string_view s, size_type pos, ssize_type offset) -> std::string;
    static auto drop(std::string_view s, size_type pos) -> std::string;
    static auto drop(std::string_view s, const char_match_proc& proc) -> std::string;
    static auto drop(std::string_view s, const charset_type& charset) -> std::string;
    //
    static auto drop_left_inplace(std::string& s, size_type n) -> std::string&;
    static auto drop_right_inplace(std::string& s, size_type n) -> std::string&;
    static auto drop_mid_inplace(std::string& s, size_type pos, size_type n) -> std::string&;
    static auto drop_range_inplace(std::string& s, size_type begin_pos, size_type end_pos) -> std::string&;
    static auto drop_range_inplace(std::string& s, range_type range) -> std::string&;
    static auto drop_inplace(std::string& s, size_type pos, ssize_type offset) -> std::string&;
    static auto drop_inplace(std::string& s, size_type pos) -> std::string&;
    static auto drop_inplace(std::string& s, const char_match_proc& proc) -> std::string&;
    static auto drop_inplace(std::string& s, const charset_type& charset) -> std::string&;
    //
    static auto drop_before_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto drop_after_view(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string_view;
    static auto drop_before(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;
    static auto drop_after(std::string_view s, range_type sep_range, bool with_sep = false) -> std::string;

    //! 定位子串 @anchor{range}
    ///
    /// 给定条件找到满足该条件的第一个（@ref{range_first}）或者最后一个（@ref{range_last}）子串的位置。
    /// 通常会配合 [take](#) 或者 [drop](#) 系列函数使用，用于提取子串。
    ///
    /// @param s 将在该字符串类查找和定位
    /// @param proc 用于定位特定条件的字符的位置。
    /// @param sep_ch 指定字符分隔符
    /// @param sep_charset 指定一个字符集作为分隔符
    /// @param sep_str 指定一个字符串作为定位条件
    /// @param sep_regex 指定一个正则表达式作为子串的匹配条件
    static auto range_first(std::string_view s, const char_match_proc& proc) -> range_type;
    static auto range_first(std::string_view s, value_type sep_ch) -> range_type;
    static auto range_first(std::string_view s, const charset_type& sep_charset) -> range_type;
    static auto range_first(std::string_view s, std::string_view sep_str) -> range_type;
    static auto range_first(std::string_view s, const std::regex& sep_regex) -> range_type;
    //
    static auto range_last(std::string_view s, const char_match_proc& proc) -> range_type;
    static auto range_last(std::string_view s, value_type sep_ch) -> range_type;
    static auto range_last(std::string_view s, const charset_type& sep_charset) -> range_type;
    static auto range_last(std::string_view s, std::string_view sep_str) -> range_type;
    static auto range_last(std::string_view s, const std::regex& sep_regex) -> range_type;

    //! 对齐 @anchor{align}
    ///
    /// @ref align_left, align_left_inplace 在字符串 s 尾部填充 ch，直到字符串长度达到 width，以使得字符串看起来是左对齐的效果
    /// @ref align_right, align_right_inplace 在字符串 s 头部追加 ch，直到字符串长度达到 width，以使得字符串看起来是左对齐的效果
    /// @ref align_center, align_center_inplace 在字符串 s 首尾添加 ch，直到字符串长度达到 width，以使得字符串看起来是居中对齐的效果
    /// @ref align_zfill, align_zfill_inplace 在字符串 s 头部添加字符 '0'，使得字符串看起来被补齐了前缀 0，这通常用于全是数字的字符串的场景
    ///
    /// @param s 被对齐的字符串
    /// @param widht 指定新生成的字符串的宽度，如果 width 小于或者等于 s 的长度，不会追加 ch，也即对齐前后字符串内容不变
    /// @param ch 当 width 大于 s 的长度时，所采用的填充字符
    static auto align_left(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_right(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_center(std::string_view s, size_type width, value_type ch = ' ') -> std::string;
    static auto align_zfill(std::string_view s, size_type width) -> std::string;
    //
    static auto align_left_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_right_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_center_inplace(std::string& s, size_type width, value_type ch = ' ') -> std::string&;
    static auto align_zfill_inplace(std::string& s, size_type width) -> std::string&;

    //! 多行文本处理 @anchor{lines}
    ///
    /// * @ref foreach_lines 用于按行遍历
    /// * @ref count_lines 字符串 s 中实际有多少行
    /// * @ref lines_indentation 计算 s 的多行中共同空白前缀的空白的数量
    /// * @ref numbering_lines, numbering_lines_inplace 给字符串 s 中的多行加上行号前缀
    /// * @ref unnumbering_lines, unnumbering_lines_inplace 去掉 s 中的多行的行号前缀
    /// * @ref indent_lines, indent_lines_inplace 将 s 中的所有行统一增加指定宽度的缩进空白
    /// * @ref dedent_lines, dedent_lines_inplace
    /// * @ref unindent_lines, unindent_lines_nplacet 将 s 中的所有行统一去除指定宽度的缩进空白
    /// * @ref trim_lines_indent, trim_lines_indent_inplace 将 s 中的所有行的缩进都去掉
    /// * @ref trim_lines_margin, trim_lines_margin_inplace 将 s 中的所有行的缩进以及前导符号去掉
    ///
    /// @param s 包含多行文本而串
    /// @param proc 用于接收每个遍历的行
    /// @param from_n 指定行编号从哪个整数开始计数
    /// @param num_format 指定整数的格式
    /// @param pad 指定缩进时的填充字符
    /// @param margin 指定一个边缘字符
    static auto foreach_lines(std::string_view s, const line_consumer_proc& proc) -> void;
    static auto count_lines(std::string_view s) -> size_type;
    static auto lines_indentation(std::string_view s) -> size_type;
    //
    static auto numbering_lines(std::string_view s, size_type from_n, std::string_view num_format = "") -> std::string;
    static auto unnumbering_lines(std::string_view s, size_type from_n, std::string_view num_format = "") -> std::string;
    static auto indent_lines(std::string_view s, size_type n, value_type pad = ' ') -> std::string;
    static auto dedent_lines(std::string_view s, size_type n, value_type pad = ' ') -> std::string;
    static auto unindent_lines(std::string_view s, size_type n, value_type pad = ' ') -> std::string;
    static auto trim_lines_indent(std::string_view s) -> std::string;
    static auto trim_lines_margin(std::string_view s, value_type margin = ' ') -> std::string;
    //
    static auto numbering_lines_inplace(std::string& s, size_type from_n, std::string_view num_format = "") -> std::string&;
    static auto unnumbering_lines_inplace(std::string& s, size_type from_n, std::string_view num_format = "") -> std::string&;
    static auto indent_lines_inplace(std::string& s, size_type n, value_type pad = ' ') -> std::string&;
    static auto dedent_lines_inplace(std::string& s, size_type n, value_type pad = ' ') -> std::string&;
    static auto unindent_lines_nplacet(std::string& s, size_type n, value_type pad = ' ') -> std::string&;
    static auto trim_lines_indent_inplace(std::string& s) -> std::string&;
    static auto trim_lines_margin_inplace(std::string& s, value_type margin = ' ') -> std::string&;

    //! 按单词统计 @anchor{words}
    ///
    /// * @ref foreach_words 用于遍历字符串中的每个单词
    /// * @ref count_words 用于统计字符串中的单词的数量
    ///
    /// @param s 被便利或者统计的字符串
    /// @param pos 指定起始位置
    /// @param proc 指定一个函数，用来接受遍历每一个被视作单词的子串
    /// @param sep_proc 用于识别分隔符的函数
    /// @param sep_ch 用于拆分字符串的分隔字符
    static auto foreach_words(std::string_view s, size_type pos, const range_consumer_proc& proc) -> void;
    static auto foreach_words(std::string_view s, size_type pos, const view_consumer_proc& proc) -> void;
    static auto foreach_words(std::string_view s, const range_consumer_proc& proc) -> void;
    static auto foreach_words(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto count_words(std::string_view s, const char_match_proc& sep_proc) -> size_type;
    static auto count_words(std::string_view s, const charset_type& sep_charset) -> size_type;
    static auto count_words(std::string_view s, value_type sep_ch) -> size_type;
    static auto count_words(std::string_view s) -> size_type;

    //! 用指定的模式串环绕字符串 @anchor{surround, unsurround}
    ///
    /// 同时在字符串两端操作，常用于添加括号和去掉括号场景。其中，pattern 参数具有固定模式。
    /// pattern 的第一个字符表示占位符，其在字符串第二次出现时，表示目标子串的位置。而第二次出现位置前后的部分被视作
    /// 环绕在 s 前后的子串，或者需要去掉的首尾的子串。比如，`str::surround("abc", "#{{#}}")` 表示在 `"abc"` 前后
    /// 分别添加 "{{" 和 "}}" 子串，最终结果为 `"{{abc}}"`
    ///
    /// * @ref surround, surround_inplace 向给定的字符串的首位添加特定的子串
    /// * @ref unsurround, unsurround_inplace 同时去掉指定字符串前后满足特定模式的子串
    ///
    /// @param s 被处理的字符串
    /// @param pattern 指定字符串的环绕模式
    static auto surround(std::string_view s, std::string_view pattern = "#(#)") -> std::string;
    static auto surround_inplace(std::string_view s, std::string_view pattern = "#(#)") -> std::string;
    //
    static auto unsurround_view(std::string_view s, std::string_view mode = "#(#)") -> std::string_view;
    static auto unsurround(std::string_view s, std::string_view mode = "#(#)") -> std::string;
    static auto unsurround_inplace(std::string_view s, std::string_view mode = "#(#)") -> std::string;

    //! 反转：字符串逆序
    ///
    /// 将 s 中指定范围内的子串的前后字符串的诶个字符交换位置
    ///
    /// @param s 被反转的字符串
    /// @param pos 指定反转的起始位置
    /// @param max_n 指定从 pos 位置开始最多反转多少字符
    /// @return 返回颠倒位置后的字符串
    static auto invert(std::string_view s, size_type pos = 0, size_type max_n = npos) -> std::string;
    //
    static auto invert_inplace(std::string& s, size_type pos = 0, size_type max_n = npos) -> std::string&;

    //! 生成
    ///
    /// 生成字 s 或者 ch 的内容重复出现 times 次后的新字符串
    ///
    /// @param s 字符串模板
    /// @param ch 指定重复的字符
    static auto repeat(std::string_view s, size_type times) -> std::string;
    static auto repeat(value_type ch, size_type times) -> std::string;

    //! 生成：随机生成
    ///
    /// 按照特定的策略，随机生成字符串
    ///
    /// @param max_len, min_len 指定生成的字符串的长度的范围
    /// @param proc 指定随机数函数
    /// @param ch, charset 指生成的字符串中的字符的范围
    static auto random(size_type max_len, size_type min_len, const number_provider_proc& proc) -> std::string;
    static auto random(size_type max_len, size_type min_len, value_type ch, const number_provider_proc& proc) -> std::string;
    static auto random(size_type max_len, size_type min_len, std::string_view charset, const number_provider_proc& proc) -> std::string;
    static auto random(size_type max_len, size_type min_len, charset_type charset, const number_provider_proc& proc) -> std::string;
    static auto random(size_type max_len, const number_provider_proc& proc) -> std::string;
    static auto random(size_type max_len, value_type ch, const number_provider_proc& proc) -> std::string;
    static auto random(size_type max_len, std::string_view charset, const number_provider_proc& proc) -> std::string;
    static auto random(size_type max_len, charset_type charset, const number_provider_proc& proc) -> std::string;

    //! 生成：
    ///
    /// 自动随机填充指定的字符串（不改变大小）
    ///
    /// @param s 原始字符串
    /// @param charset 指定生成的字符串中的字符的范围
    /// @param proc 随机函数
    static auto random_fill(std::string& s, std::string_view charset, const number_provider_proc& proc) -> std::string&;
    static auto random_fill(std::string& s, charset_type charset, const number_provider_proc& proc) -> std::string&;
    static auto random_fill(std::string& s, const number_provider_proc& proc) -> std::string&;

    //! 重排：
    ///
    /// 现有字符串随机重排（不改变大小）
    ///
    /// @param s 指定被重拍的字符串
    /// @param proc 随机数生成器
    /// @return 将 s 重排后返回
    static auto random_reorder(std::string& s, const number_provider_proc& proc) -> std::string&;

    //! 空白操作
    ///
    /// @ref spaces 用于生成指定 width 宽度的空白字符序列
    /// @ref after_skip_spaces_view, after_skip_spaces, after_skip_spaces_inplace 返回跳过空白后剩余的内容
    ///
    /// @param witdh 指定待生成的空白串的宽度，这里的空白串采用 ASCII 码为 0x20 的字符填充
    /// @param s 用于去除空白字符的串
    /// @param pos 从指定的位置开始找查找并跳过空白
    /// @return 总是返回跳过空白后，右侧的部分。当未指定 pos 时，表示从字符串 s 的首字符开始处理。
    static auto make_spaces(size_type width) -> std::string;
    static auto after_skip_spaces_view(std::string_view s) -> std::string_view;
    static auto after_skip_spaces(std::string_view s) -> std::string;
    static auto after_skip_spaces_inplace(std::string& s) -> std::string&;
    static auto after_skip_spaces_view(std::string_view s, size_type pos) -> std::string_view;
    static auto after_skip_spaces(std::string_view s, size_type pos) -> std::string;
    static auto after_skip_spaces_inplace(std::string& s, size_type pos) -> std::string&;

    //! 字符串遮罩
    ///
    /// 使用指定的掩码字符串 mask 对原始字符串中的关键文字替换，以遮蔽原始字符串中的敏感信息。常用于
    /// 敏感信息脱敏，长串简略缩短等场景。其工作方式是，先根据 visible 来决定需要保留多少字符可见。
    /// 然后使用 mask 中的字符循环填充 width 不足的部分，直到达到需要的宽度。
    ///
    /// 关于返回字符串的宽度控制：
    ///
    /// * 如果 width 大于 visible，表示限定宽度；
    /// * 如果 visible 为 0，那么所有字符都被遮罩，其返回的数据由 mask 和 width 决定
    /// * 如果 width 和 visible 均为 0，那么直接返回 mask
    /// * 如果 mask 为空串，自动校正为缺省值 "***"
    ///
    /// 下面的代码演示了不同调用方式的结果：
    ///
    /// ```cpp
    /// cover_center("abc/mnop/def", "***", 4, 8) //  "ab****ef"
    /// cover_center("abc/mnop/def", "***", 3, 8) //  "a*****ef"
    /// cover_center("abc/mnop/def", "***", 3, 3) //  "a***ef"
    /// cover_center("abc/mnop/def", "***", 3, 0) //  "a***ef"
    /// cover_center("abc/mnop/def", "***", 0, 0) //  "***"
    /// cover_center("abc/mnop/def", "", 0, 0)    //  "***"
    /// ```
    ///
    /// @ref{cover_left, cover_left_inplace} 对 s 的左侧使用遮罩
    /// @ref{cover_right, cover_right_inplace} 对 s 的右侧使用遮罩
    /// @ref{cover_center, cover_center_inplace} 对 s 的中间部分使用遮罩
    /// @ref{cover_surrounding, cover_surrounding_inplace} 对 s 的两侧使用遮罩
    ///
    /// @param s 被遮罩的字符串
    /// @param mask 用于遮罩的字符串，如果 mask 为空，自动区默认值 "*"
    /// @param visible 保留可见的部分的长度
    /// @param width 限定宽度
    /// @return 返回遮罩后的字符串
    static auto cover_left(std::string_view s, std::string_view mask = "***", size_type visible = 3, size_type width = 10) -> std::string;
    static auto cover_right(std::string_view s, std::string_view mask = "***", size_type visible = 3, size_type width = 10) -> std::string;
    static auto cover_center(std::string_view s, std::string_view mask = "***", size_type visible = 3, size_type width = 10) -> std::string;
    static auto cover_surrounding(std::string_view s, std::string_view mask = "***", size_type visible = 3, size_type width = 10) -> std::string;
    //
    static auto cover_left_inplace(std::string& s, std::string_view mask = "***", size_type visible = 3, size_type width = 10) -> std::string&;
    static auto cover_right_inplace(std::string& s, std::string_view mask = "***", size_type visible = 3, size_type width = 10) -> std::string&;
    static auto cover_center_inplace(std::string& s, std::string_view mask = "***", size_type visible = 3, size_type width = 10) -> std::string&;
    static auto cover_surrounding_inplace(std::string& s, std::string_view mask = "***", size_type visible = 3, size_type width = 10) -> std::string&;

    //! 字符串拼接 @anchor join
    ///
    /// 用 s 作为分隔符，拼接多个字符串。输入串可以通过 proc 或者 items 供给。如果 proc 无法提供任何字符串或者 items 为空，返回空串。
    ///
    /// @param s 分隔符
    /// @param proc 用于通过回调函数的方式供给被拼接的字符串
    /// @param items 被字符串容器序列
    /// @return 返回合并后的字符串
    static auto join(std::string_view s, const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join(std::string_view s, const Sequence& items) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join(const Sequence& items) -> std::string;

    //! 拼接列表
    ///
    /// 使用逗号作为分隔符，拼接多个子串。输入串可以通过 proc 或者 items 供给。如果 proc 无法提供任何字符串或者 items 为空，返回空串。
    ///
    /// @param proc 用于通过回调函数的方式供给被拼接的字符串
    /// @param items 被字符串容器序列
    /// @return 返回合并后的字符串
    static auto join_list(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_list(const Sequence& items) -> std::string;

    //! 映射拼接
    ///
    /// 使用 sep_pair 和 sep_list 拼接多个 key-value 对。该函数拼接的结果接近 json 的字典的内部结构（没有外围的花括号）。
    ///
    /// @param sep_pair 用于拼接每个 key-value 对，当未指定该参数或者为空时，自动采用 `":"`
    /// @param sep_list 用于拼接多个拼接好的 key-value 对，当未指定该参数或者为空时，自动采用 `","`
    /// @param proc 用于供给 key-value 对， key-value 对由 `std::tuple<std::string_view, std::string_view>` 来表示
    /// @param items 用于存储 key-value 对的容器
    /// @return 返回拼接后的字符串
    static auto join_map(std::string_view sep_pair, std::string_view sep_list, const view_pair_provider_proc& proc) -> std::string;
    static auto join_map(const view_pair_provider_proc& proc) -> std::string;
    template <typename Map, typename = typename Map::const_iterator>
    static auto join_map(std::string_view sep_pair, std::string_view sep_list, const Map& items) -> std::string;
    template <typename Map, typename = typename Map::const_iterator>
    static auto join_map(const Map& items) -> std::string;

    //! 按行拼接
    ///
    /// 将每个字符串视作一行，然后用换行符拼接成一个字符串
    ///
    /// @param sep 指定换行符号，如果未指定，默认使用 @ref{sep_line_ends} 作为分隔符
    /// @param proc
    /// @param items
    static auto join_lines(std::string_view sep, const view_provider_proc& proc) -> std::string;
    static auto join_lines(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_lines(std::string_view sep, const Sequence& items) -> std::string;
    template <typename Sequence = std::initializer_list<std::string_view>, typename = typename Sequence::const_iterator>
    static auto join_lines(const Sequence& items) -> std::string;

    //! 拼接路径
    ///
    /// 使用指定的分隔符 sep 或者系统默认的路径分隔符，将不同来源的路径片段拼接成完整的文件路径
    ///
    /// @param sep 指定的路径分隔符，如果不带该参数，默认使用 @ref{sep_path}
    /// @param proc 供给路径片段
    /// @param items 供给路径片段的容器
    /// @return 返回拼接后的路径
    static auto join_path(std::string_view sep, const view_provider_proc& proc) -> std::string;
    static auto join_path(const view_provider_proc& proc) -> std::string;
    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_path(std::string_view sep, const Sequence& items) -> std::string;
    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_path(const Sequence& items) -> std::string;

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
    static auto join_search_path(std::string_view sep, const Sequence& items) -> std::string;

    template <typename Sequence = std::initializer_list<std::string>, typename = typename Sequence::const_iterator>
    static auto join_search_path(const Sequence& items) -> std::string;

    //! 字符串拆分
    ///
    /// 以指定的字符集为分隔符将字符串拆分为多个子串
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

    //! 以指定的字符串为分隔符将字符串拆分为两个部分
    ///
    /// split_pair 的功能等价于 split_list 在 max_n 等于 1 时的功能。
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
    ///
    /// @param keep_ends 是否保留行尾分隔符
    static auto split_lines(std::string_view s, bool keep_ends, const view_consumer_proc& proc) -> void;
    static auto split_lines(std::string_view s, bool keep_ends = false) -> std::vector<std::string>;
    static auto split_lines_view(std::string_view s, bool keep_ends = false) -> std::vector<std::string_view>;

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

    //! 分片
    ///
    /// @ref{partition_view, partition} 从 s 左侧查找首个 sep 分隔符或者满足 proc 的字符的位置，并将字符串分割为分隔符左侧，分隔符，分隔符右侧三个部分，并返回
    /// @ref{rpartition_view, rpartition} 从 s 右侧查找首个 sep 分隔符或者满足 proc 的字符的位置，并将字符串分割为分隔符左侧，分隔符，分隔符右侧三个部分，并返回
    ///
    /// @param s 输入字符串
    /// @param sep 分隔符。根据类型不同其分隔符含义不同：
    /// * `charset_type` : 只要某个字符在字符集范围内，即表示找到分隔符
    /// * `char_match_proc` : 只要某个字符使得函数返回true，则表示找到分隔符
    /// * `std::string_view` : 表示子串作为分隔符
    /// * `range_search_proc` : 同样以子串作为分隔符
    /// @return 返回依次由分隔符左侧的子串，分隔符自身，分隔符右侧子串组成的 tuple
    static auto partition_view(std::string_view s, charset_type sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto partition_view(std::string_view s, const char_match_proc& sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto partition_view(std::string_view s, std::string_view sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto partition_view(std::string_view s, const std::regex& sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto partition_view(std::string_view s, const range_search_proc& sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    //
    static auto partition(std::string_view s, charset_type sep) -> std::tuple<std::string, std::string, std::string>;
    static auto partition(std::string_view s, const char_match_proc& sep) -> std::tuple<std::string, std::string, std::string>;
    static auto partition(std::string_view s, std::string_view sep) -> std::tuple<std::string, std::string, std::string>;
    static auto partition(std::string_view s, const std::regex& sep) -> std::tuple<std::string, std::string, std::string>;
    static auto partition(std::string_view s, const range_search_proc& sep) -> std::tuple<std::string, std::string, std::string>;
    //
    static auto rpartition_view(std::string_view s, charset_type sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto rpartition_view(std::string_view s, const char_match_proc& sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto rpartition_view(std::string_view s, std::string_view sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto rpartition_view(std::string_view s, const std::regex& sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto rpartition_view(std::string_view s, const range_search_proc& sep) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    //
    static auto rpartition(std::string_view s, charset_type sep) -> std::tuple<std::string, std::string, std::string>;
    static auto rpartition(std::string_view s, const char_match_proc& sep) -> std::tuple<std::string, std::string, std::string>;
    static auto rpartition(std::string_view s, std::string_view sep) -> std::tuple<std::string, std::string, std::string>;
    static auto rpartition(std::string_view s, const std::regex& sep) -> std::tuple<std::string, std::string, std::string>;
    static auto rpartition(std::string_view s, const range_search_proc& sep) -> std::tuple<std::string, std::string, std::string>;

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

    // 基于窗口拆分字符串
    static auto windowed(std::string_view s, size_type width, size_type step, const view_consumer_proc& proc) -> void;
    static auto windowed_view(std::string_view s, size_type width, size_type step) -> std::vector<std::string_view>;
    static auto windowed(std::string_view s, size_type width, size_type step) -> std::vector<std::string>;

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

    //! 剔除
    ///
    /// * @ref{trim_left, trim_left_inplace} 去掉 s 左侧的空白
    /// * @ref{trim_right_view, trim_right, trim_right_inplace} 去掉 s 右侧的空白
    /// * @ref{trim_surrounding_view, trim_surrounding, trim_surrounding_inplace} 去掉 s 两端的空白
    /// * @ref{trim_anywhere, trim_anywhere_inplace} 去掉 s 中任何满足条件的字符
    ///
    /// @param s 被操作的字符串
    /// @param proc 字符匹配条件
    /// @param charset 表示可以满足条件的字符集
    /// @return 返回
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
    //
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
    //
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
    //
    static auto trim_anywhere(std::string_view s, const char_match_proc& proc) -> std::string;
    static auto trim_anywhere(std::string_view s, const charset_type& charset) -> std::string;
    static auto trim_anywhere(std::string_view s, std::string_view charset) -> std::string;
    static auto trim_anywhere(std::string_view s, value_type charset) -> std::string;
    static auto trim_anywhere(std::string_view s) -> std::string;
    //
    static auto trim_anywhere_inplace(std::string& s, const char_match_proc& proc) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s, const charset_type& charset) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s, std::string_view charset) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s, value_type charset) -> std::string&;
    static auto trim_anywhere_inplace(std::string& s) -> std::string&;

    //! 化简
    ///
    /// * @ref{simplified, simplified_inplace} 将 s 中连续匹配 proc 条件的字符替换为 fill，对于不带 fill 和 proc 形式
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
    static auto simplified(std::string_view s, std::string_view fill, const char_match_proc& proc) -> std::string;
    static auto simplified(std::string_view s) -> std::string;
    static auto simplified_inplace(std::string& s, std::string_view fill, const char_match_proc& proc) -> std::string&;
    static auto simplified_inplace(std::string& s) -> std::string&;
    //
    static auto simplified_integer(std::string_view s) -> std::string;
    static auto simplified_integer_inplace(std::string& s) -> std::string&;
    //
    static auto simplified_decimal(std::string_view s) -> std::string;
    static auto simplified_decimal_inplace(std::string& s) -> std::string&;

    //! 拷贝
    ///
    /// 将 s 中的数据拷贝到 `buffer` 中去，实际的字节数取决于 `s.size()` 与 `size` 的最小值
    ///
    /// @param buffer 缓冲区
    /// @param size 缓冲区尺寸
    /// @param s 原始字符串
    /// @return 返回实际拷贝的字节数
    static auto copy(pointer buffer, size_type size, std::string_view s) -> size_type;

    //! 展开 @anchor{expand}
    ///
    /// 将字符串 s 中 `$xxx` 和 `${xxx}` 形式的子串看做环境变量的占位子串，将其中的 `xxx` 视作环境变量的名字，将整个占位子串
    /// 替换为环境变量的值。本系列函数提供了多种方式获取环境变量值的方式。其中，如果没带任何获取环境变量值的方式的话，将自动
    /// 通过 getenv 函数，从当前系统环境变量表中获取。
    ///
    /// @param s 模板字符串
    /// @param keep_unexpanded 当无法通过 key 确定被替换的值时，由 keep_unexpanded 参数决定是保留原样。如果不原样保留会直接
    ///                        去除占位子串。
    /// @param proc 通过 proc 查询环境变量的值
    /// @param kvs 直接从一个指定的 map 中检索环境变量的值
    /// @param key, val 只将 key 替换为 val，其他的原样保留
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

    //! tab 扩展
    ///
    /// 将字符串中的 tab 符号(\t)按照 tab_size 宽度替换成空格（0x20）。每个 tab 实际被替换成的空格的数量，并不是固定的，
    /// 而是取决于 tab 字符在字符串 s 中的位置。该函数模拟了在编辑器中按 tab 键时的视觉效果。
    ///
    /// @param s 将被扩展的字符串
    /// @param tab_size 指定一个 tab 应该等价于多少个空格字符（0x20）。
    /// @return 返回扩展后的字符串
    static auto expand_tabs(std::string_view s, size_type tab_size = 8) -> std::string;
    static auto expand_tabs_inplace(std::string& s, size_type tab_size = 8) -> std::string&;

    //! HOME 字符扩展
    //
    /// 扩展字符串中的 `~` 前缀为 `${HOME}` 的值，该函数模拟了 Shell 的行为。
    ///
    /// @param s 将被扩展的字符串
    /// @return 返回扩展后的字符串
    static auto expand_user(std::string_view s) -> std::string;
    static auto expand_user_inplace(std::string& s) -> std::string&;

    //! 路径正常化
    ///
    /// @param s 被视作路径的字符串
    /// @return 返回正规化之后的路径
    static auto normpath(std::string_view s) -> std::string;
    static auto normpath_inplace(std::string& s) -> std::string&;

    //! 路径处理
    ///
    /// 将字符串 s 视作文件路径，并检查其是否是绝对路径的形式或者相对路径的形式
    static auto is_absolute(std::string_view s) -> bool;
    static auto is_relative(std::string_view s) -> bool;

    //! 路径处理函数：基础定位函数
    ///
    /// 将入参 s 视作文件路径：
    /// * @ref basename_ptr 返回文件名的基本名（basename）的起始位置
    /// * @ref extname_ptr 返回文件的扩展名的起始位置
    /// * @ref dirname_ptr 返回文件路径的尾部位置
    ///
    /// 为介绍三个函数之间的关系，假设 s 为字符串 "abc/def/ghi.txt"，那么：
    /// * `str::basename_ptr(s)` 返回的是指向 `g` 的位置的指针
    /// * `str::extname_ptr(s)` 返回的是指向 `.` 的位置的指针
    /// * `str::dirname_ptr(s)` 返回的是指向 `f` 的位置的指针
    ///
    /// @param s 作为路径的字符串
    /// @return 以指针的形式返回 s 中满足条件的位置
    static auto basename_ptr(std::string_view s) -> std::string::const_pointer;
    static auto extname_ptr(std::string_view s) -> std::string::const_pointer;
    static auto dirname_ptr(std::string_view s) -> std::string::const_pointer;

    //! 路径处理函数：目录
    ///
    /// @ref{dirname_view, dirname, dirname_inplace} 获取路径 s 的目录部分
    /// @ref{remove_dirname_view, remove_dirname, remove_dirname_inplace} 删除路径 s 的目录部分，返回剩余部分
    /// @ref{replace_dirname, replace_dirname_inplace} 使用 new_name 替换路径 s 中的目录部分
    /// @ref{split_diename} 将路径 s 拆分为目录部分和剩余部分
    ///
    /// @param s 路径字符串
    /// @param new_name 表示被替换成的新名字
    static auto dirname_view(std::string_view s) -> std::string_view;
    static auto dirname(std::string_view s) -> std::string;
    static auto remove_dirname_view(std::string_view s) -> std::string_view;
    static auto remove_dirname(std::string_view s) -> std::string;
    static auto replace_dirname(std::string_view s, std::string_view new_name) -> std::string;
    static auto split_dirname_view(std::string_view s) -> std::tuple<std::string_view, std::string_view>;
    static auto split_dirname(std::string_view s) -> std::tuple<std::string, std::string>;
    //
    static auto dirname_inplace(std::string& s) -> std::string&;
    static auto remove_dirname_inplace(std::string& s) -> std::string&;
    static auto replace_dirname_inplace(std::string& s, std::string_view new_name) -> std::string&;

    //! 路径处理函数：文件名
    ///
    /// @ref basename_view, basename, basename_inplace 获取路径 s 的文件名部分
    /// @ref remove_basename_view, remove_basename, remove_basename_inplace 删除路径 s 的文件名部分，返回剩余部分
    /// @ref replace_basename, replace_basename_inplace 使用 new_name 替换路径 s 中的文件名
    /// @ref split_basename 将路径 s 拆分为文件名和剩余部分
    ///
    /// @param s 路径字符串
    /// @param new_name 表示被替换成的新名字
    static auto basename_view(std::string_view s) -> std::string_view;
    static auto basename(std::string_view s) -> std::string;
    static auto remove_basename_view(std::string_view s) -> std::string_view;
    static auto remove_basename(std::string_view s) -> std::string;
    static auto replace_basename(std::string_view s, std::string_view new_name) -> std::string;
    static auto split_basename_view(std::string_view s) -> std::tuple<std::string_view, std::string_view>;
    static auto split_basename(std::string_view s) -> std::tuple<std::string, std::string>;
    //
    static auto basename_inplace(std::string& s) -> std::string&;
    static auto remove_basename_inplace(std::string& s) -> std::string&;
    static auto replace_basename_inplace(std::string& s, std::string_view new_name) -> std::string&;

    //! 路径处理函数：扩展名
    ///
    /// @ref extname_view, extname, extname_inplace 获取路径 s 的扩展名部分
    /// @ref remove_extname_view, remove_extname, remove_extname_inplace 删除路径 s 的扩展名，返回剩余部分
    /// @ref replace_extname, replace_extname_inplace 使用 new_name 替换路径 s 中的扩展名
    /// @ref split_basename 将路径 s 拆分为扩展名和剩余部分
    static auto extname_view(std::string_view s) -> std::string_view;
    static auto extname(std::string_view s) -> std::string;
    static auto remove_extname_view(std::string_view s) -> std::string_view;
    static auto remove_extname(std::string_view s) -> std::string;
    static auto replace_extname(std::string_view s, std::string_view name) -> std::string;
    static auto split_extname_view(std::string_view s) -> std::tuple<std::string_view, std::string_view>;
    static auto split_extname(std::string_view s) -> std::tuple<std::string, std::string>;
    //
    static auto extname_inplace(std::string& s) -> std::string&;
    static auto remove_extname_inplace(std::string& s) -> std::string&;
    static auto replace_extname_inplace(std::string& s, std::string_view new_name) -> std::string&;

    //! 路径处理函数：裸文件件名
    ///
    /// 裸文件件名是指不包含文件后缀名的部分
    ///
    /// @ref rawname_view, rawname, rawname_inplace 获取路径 s 的中裸文件名部分
    /// @ref replace_rawname, replace_rawname_inplace 使用 new_name 替换路径 s 中的裸文件名
    /// @ref split_rawname 将路径 s 拆分为目录、裸文件名，扩展名三个部分
    ///
    /// @param s 路径字符串
    /// @param new_name 表示被替换成的新名字
    static auto rawname_view(std::string_view s) -> std::string_view;
    static auto rawname(std::string_view s) -> std::string;
    static auto replace_rawname(std::string_view s, std::string_view new_name) -> std::string;
    static auto split_rawname_view(std::string_view s) -> std::tuple<std::string_view, std::string_view, std::string_view>;
    static auto split_rawname(std::string_view s) -> std::tuple<std::string, std::string, std::string>;
    //
    static auto rawname_inplace(std::string& s) -> std::string&;
    static auto replace_rawname_inplace(std::string& s, std::string_view name) -> std::string&;

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

    //! 转义：URL
    ///
    /// 将 s 中属于 url 的特殊字符，转义为 url 中可表示的形式，或者反过来
    ///
    /// @param s 被编码或者解码的字符串
    /// @param proc 用于接收转换后，生成的字符串
    /// @return 返回编码或者解码后的字符串
    static auto encode_url(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto decode_url(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto encode_url(std::string_view s) -> std::string;
    static auto decode_url(std::string_view s) -> std::string;
    //
    static auto encode_url_inplace(std::string& s) -> std::string&;
    static auto decode_url_inplace(std::string& s) -> std::string&;

    //! 转义：C语言字符串
    ///
    /// 将 s 中属于 C 语言字符串中的特殊字符，转义为 C 语言字符串的转义表示形式，或者反过来。
    ///
    /// @param s 被编码或者解码的字符串
    /// @param proc 用于接收转换后，生成的字符串
    /// @return 返回编码或者解码后的字符串
    static auto encode_cstr(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto decode_cstr(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto encode_cstr(std::string_view s) -> std::string;
    static auto decode_cstr(std::string_view s) -> std::string;
    //
    static auto encode_cstr_inplace(std::string& s) -> std::string;
    static auto decode_cstr_inplace(std::string& s) -> std::string;

    //! 编解码：base64
    ///
    /// 将 s 中属于 url 的特殊字符，转义为 url 中可表示的形式，或者反过来
    ///
    /// @param s 被编码或者解码的字符串
    /// @param proc 用于接收转换后，生成的字符串
    /// @return 返回编码或者解码后的字符串
    static auto encode_base64(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto decode_base64(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto encode_base64(std::string_view s) -> std::string;
    static auto decode_base64(std::string_view s) -> std::string;
    //
    static auto encode_base64_inplace(std::string& s) -> std::string&;
    static auto decode_base64_inplace(std::string& s) -> std::string&;

    // 编解码：base16
    ///
    /// 将 s 中属于 url 的特殊字符，转义为 url 中可表示的形式，或者反过来
    ///
    /// @param s 被编码或者解码的字符串
    /// @param proc 用于接收转换后，生成的字符串
    /// @param upper 转换出来的十六进制是否采用大写形式
    /// @return 返回编码或者解码后的字符串
    static auto encode_base16(std::string_view s, bool upper, const view_consumer_proc& proc) -> void;
    static auto decode_base16(std::string_view s, const view_consumer_proc& proc) -> void;
    static auto encode_base16(std::string_view s, bool upper = false) -> std::string;
    static auto decode_base16(std::string_view s) -> std::string;
    //
    static auto encode_base16_inplace(std::string& s, bool upper = false) -> std::string&;
    static auto decode_base16_inplace(std::string& s) -> std::string&;

    //! 格式化
    ///
    /// 按照十六进制编辑器的样式格式化数据
    ///
    /// @param data, len 被格式化的数据的地址和数据长度
    /// @param format 指定 dump_hex 时的格式化信息，参考 @ref{dump_hex_format}
    /// @param proc 用于接收格式化数据
    struct dump_hex_format {
        enum format {
            show_offset = 0x01,
            show_ascii = 0x02,
        };
        uint8_t flags{format::show_offset | format::show_ascii}; ///< 可选标记位
        uint8_t line_size{16};                                   ///< 每行格式化多少个字节
        uint8_t group_size{1};                                   ///< 多少字节一组，如果大于 line_size，自动校正为 line_size
        value_type ascii_mask{'.'};                              ///< 显示 ascii 时，对不打印字符显示的掩码字符
        std::string_view ascii_margin{" "};                      ///< 显示 ascii 时，在此之前显示的 margin 字符
        std::string_view offset_margin{":"};                     ///< 显示 offset 时，offset 右侧与文本段的分隔符
    };
    static auto dump_hex(void* data, size_type len, const dump_hex_format& format, const line_consumer_proc& proc) -> void;

    //! 求和
    ///
    /// 将字符串 s 中的每个字符先用 proc 做映射，然后做求和计算，并返回最终的结果。
    ///
    /// @param s 被映射的字符串
    /// @param proc 映射函数
    /// @return 返回求和后的结果
    template <typename T>
    static auto sum(std::string_view s, const mapping_proc<T>& proc) -> T;

    //! 字符集生成
    ///
    /// 用于将 s 中的所有字符设置到字符集中
    ///
    /// @param s 从 s 中的每个字符字作为字符集
    /// @param charset 向 charset 中添加 s 中的字符，用于修改场景
    /// @return 返回生成或者修改后的字符集
    static auto charset(std::string_view s) -> charset_type;
    static auto charset(std::string_view s, charset_type& charset) -> charset_type&;

    //! 读取文件内容
    ///
    /// * @ref{read_all} 用于读取文件所有内容
    /// * @ref{read_line} 用于读取文件中的一行，通过连续调用可以读取多行
    /// * @ref{read_lines} 用于连续读取多行
    ///
    /// @param filename, file, stream 指定计划读取的文件的文件名，或者一个已经打开的文件对象，或者文件输入流对象
    /// @param keeo_ends 是否保留行尾的续行符
    /// @param max_n 最多读取多少行
    /// @param proc 通过 proc 函数接收行输出
    static auto read_all(const std::string& filename) -> std::string;
    static auto read_all(const char* filename) -> std::string;
    //
    static auto read_line(FILE* file, bool keeo_ends = false) -> std::string;
    static auto read_line(std::istream& stream) -> std::string;
    //
    static auto read_lines(FILE* file, const line_consumer_proc& proc) -> void;
    static auto read_lines(FILE* file, size_type max_n = npos) -> std::vector<std::string>;
    static auto read_lines(std::istream& stream, const line_consumer_proc& proc) -> void;
    static auto read_lines(std::istream& stream, size_type max_n = npos) -> std::vector<std::string>;
    //
    static auto read_lines(const std::string& filename, const line_consumer_proc& proc) -> void;
    static auto read_lines(const char* filename, const line_consumer_proc& proc) -> void;
    static auto read_lines(const std::string& filename, size_type max_n = npos) -> std::vector<std::string>;
    static auto read_lines(const char* filename, size_type max_n = npos) -> std::vector<std::string>;

    //! 选项识别
    ///
    /// 将字符串 s 看做一个命令行选项序列，通过迭代的方式连续从 s 中拆分并识别出选项。需要注意 read_opt_view 和 read_opt
    /// 采用的是固定格式的方式解析的，不依赖于选项定义。其语法定义如下：
    ///
    /// * `-` 为选项识别符
    /// * `-key` 定义一个独立的、无 value 选项，常常用来定义开关型的选项
    /// * `-key=value` 定义一个名字为 key 值为 value 的选项
    /// * `value` 定义一个没有 key，但是有 value 的参数
    /// * `-- value` 用于对选项识别符号进行转义，用于处理位置参数本身已以 `-` 开头的情况
    ///
    /// ```
    /// std::string cmdline{"create_dirs -- workdir -when_exist=quit -i"};
    /// size_t pos = 0;
    /// while (pos >= cmdline.size()) {
    ///     auto [key, val] = str::read_opt_view(cmdline, pos);
    ///     std::cout << "{" << key << ":" << value << "}"<< std::endl;
    /// }
    ///
    /// //The output is:
    /// //{:create_dirs}
    /// //{:./workdir}
    /// //{-when_exist:quit}
    /// //{-i:}
    /// ```
    ///
    /// @param s 命令行字符串
    /// @param pos 从指定的位置开始读取下一个选项
    /// @return 以键值对的形式返回读取到的选项，并提前将 pos 移动到选项的结尾
    static auto read_opt_view(std::string_view s, size_type& pos) -> std::tuple<std::string_view, std::string_view>;
    static auto read_opt(std::string_view s, size_type& pos) -> std::tuple<std::string, std::string>;

    //! 符号识别
    ///
    /// 尝试识别指定类型的符号，并返回符号的范围
    ///
    /// @param s 被视作符号识别的缓冲区的字符串
    /// @param pos 作为输入参数是表示指定识别的起始位置，作为输出参数时表示下一个还未被识别的字符的位置
    /// @return 如果识别成功，将返回符号的范围，如果识别失败，返回的范围对象长度为 0，如果 pos 已经不在 s 的范围内，pos 的值
    ///         将大于或者等于 `s.size()`。因此，可以通过测试 `(pos >= s.size())` 来确定是否所有数据已经识别完。
    static auto skip_spaces(std::string_view s, size_type& pos) -> void;
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

    //! 映射
    ///
    /// 将字符串 s 中在 source 中的字符映射到 target 的对应位置的字符中去。
    ///
    /// @param s 被处理的字符串
    /// @param match_charset 匹配字符集
    /// @param replace_charset 替换字符集
    /// @param proc 将匹配的字符映射为其他字符集
    static auto mapping(std::string& s, std::string_view match_charset, std::string_view replace_charset) -> std::string;
    static auto mapping(std::string& s, const char_mapping_proc& proc) -> std::string;
};

#endif // TINY_STR_H
