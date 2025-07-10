<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>str - 一个字符串函数库</title>
<link rel="stylesheet" href="cppreference.css"/>
</head>
<body>
<main>
<!--
-->
<section class="chapter-3">
<h3>
简介
</h3>
<div class="chapter-3-body">
<p>
str 库提供了一系列字符串处理函数算法，目标是成为 C++ 语言功能最丰富的字符串处理函数库。
</p>
<p>
当前主要提供了下列算法：
</p>
<ul>
<li>
批量追加：<a href="#append">append</a>、<a href="#prepend">prepend</a>
</li>
<li>
大小写不敏感的比较：<a href="#icompare">icompare</a> 和 <a href="#iequals">iequals</a>
</li>
<li>
基于通配符匹配：<a href="#wildcmp">wildcmp</a>
</li>
<li>
两字符串之间的关系：<a href="#contains">contains</a>
</li>
<li>
特征字符串统计：<a href="#count">count</a>
</li>
<li>
前后缀操作：<a href="#prefix">prefix</a>、<a href="#suffix">suffix</a>、
</li>
<li>
查找： <a href="#next_char">next_char</a>、<a href="#prev_char">prev_char</a>、<a href="#next_string">next_string</a>、<a href="#prev_string">prev_string</a>、<a href="#next_spaces">next_spaces</a>、<a href="#next_eol">next_eol</a>
</li>
<li>
特征测试：<a href="#is_xx">is_xx</a>、<a href="#is_literal">is_literal</a>、<a href="#is_identifier">is_identifier</a>、<a href="#is_all_in">is_all_in</a>、<a href="#has_any_one">has_any_one</a>
</li>
<li>
子串提取：<a href="#take">take</a>、<a href="#drop">drop</a>
</li>
<li>
修剪和整形：<a href="#align">align</a>、<a href="#surround">surround</a>、<a href="#unsurround">unsurround</a>、<a href="#invert">invert</a>、<a href="#simplified">simplified</a>、<a href="#trim">trim</a>
</li>
<li>
按多行处理：<a href="#lines">lines</a>
</li>
<li>
按单词处理：<a href="#words">words</a>
</li>
<li>
字符串生成：<a href="#repeat">repeat</a>
</li>
<li>
空白串处理：<a href="#spaces">spaces</a>
</li>
<li>
字符串遮罩：<a href="#cover">cover</a>
</li>
<li>
字符串拆分：<a href="#split">split</a>、<a href="#partition">partition</a>、<a href="#chunked">chunked</a>、<a href="#windowed">windowed</a>
</li>
<li>
字符串拼接：<a href="#join">join</a>
</li>
<li>
大小写转换：<a href="#case_conversion">case_conversion</a>
</li>
<li>
变量或者特殊符号展开：<a href="#expand_tabs">expand_tabs</a>, <a href="#expand_envs">expand_envs</a>, <a href="#expand_user">expand_user</a>
</li>
<li>
文件名路径操作：<a href="#basename">basename</a>、<a href="#extname">extname</a>、<a href="#dirname">dirname</a>、<a href="#rawname">rawname</a>、<a href="#home">home</a>
</li>
<li>
字符串转义：<a href="#encode_cstr">encode_cstr</a>、<a href="#decode_cstr">decode_cstr</a>、<a href="#encode_base16">encode_base16</a>、<a href="#decode_base16">decode_base16</a>、<a href="#encode_base64">encode_base64</a>、<a href="#decode_base64">decode_base64</a>
</li>
<li>
文本文件读取：<a href="#read_all">read_all</a>、<a href="#read_lines">read_lines</a>、<a href="#read_next_line">read_next_line</a>、<a href="#with_file">with_file</a>
</li>
<li>
简单词法识别：<a href="#accept">accept</a>、<a href="#skip">skip</a>、<a href="#skip_spaces">skip_spaces</a>
</li>
<li>
数据块格式化：<a href="#dump_hex">dump_hex</a>
</li>
<li>
多行文本处理：<a href="#lines">lines</a>
</li>
<li>
空白处理：<a href="#next_spaces">next_spaces</a>、<a href="#skip_spaces">skip_spaces</a>、<a href="#spaces">spaces</a>、<a href="#trim">trim</a>、<a href="#simplified">simplified</a>、<a href="#spaces_margin">spaces_margin</a>
</li>
<li>
换行符检测和处理：<a href="#ends_with_eol">ends_with_eol</a>、<a href="#eol_suffix">eol_suffix</a>、<a href="#next_eol">next_eol</a>、<a href="#prev_eol">prev_eol</a>、<a href="#remove_eol">remove_eol</a>
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
关于函数的返回值及其使用注意事项：
</h1>
<div class="section-1-body">
<p>
str 中提供的函数根据返回值的不同可以分为三种不同的形式，使用者需要根据情况合理地选择。
</p>
<ul>
<li>
<code class="source">xxx_view</code> 形式：
<p>
通常意味着该函数的返回值是输入参数的一个（或多个）视图，该函数不会发生任何分配行为（返回存放
容器的 <code class="source">std::string_view</code>，如 <code class="source">std::vector&lt;std::string_view&gt;</code> 类似的除外）。但这种形式的接口
是的时也需要格外注意，其返回值可能会因为输入参数提前析构，导致失效。所以在调用这些
接口时，需要确保在使用前其输入参数的地址仍然是有效的。
</p>
</li>
<li>
<code class="source">xxx_inplace</code> 形式：
<p>
这类函数通常意味着该函数返回的是输入参数自身，返回值也通常是 <code class="source">std::string&amp;</code>。该函数在执行
过程中通常会修改输入参数，并返回。如果使用这类函数，需要确保原始输入串是可以被改写的，否则
建议使用 <code class="source">xxx_view 形式</code>或者 <code class="source">xxx 形式</code> 的函数代替。
</p>
</li>
<li>
<code class="source">xxx_range</code> 形式：
<p>
这类函数返回的并不是某种形式的子串，而是子串在原始串中的范围，在子串定位场景很常见。
</p>
</li>
<li>
<code class="source">xxx</code> 形式：
<p>
与前面几种对应，这类不带 <code class="source">_view</code> 或者 <code class="source">_inplace</code> 后缀的函数，其返回值不是原始输入的视图，而是一个新的字符串拷贝。
因此，这类函数既没有类似 <code class="source">_view</code> 系列函数那样的返回值依赖于输入参数的生存期的问题，也没有类似 <code class="source">xxx_inplace</code> 那样会修改
原始输入参数的问题。但这类函数由于总是会拷贝原始输入字符串的，所以如果返回的字符串无法充分利用字符串的 SSO 特性，
那么性能会比 <code class="source">xxx_view</code> 和 <code class="source">xxx_inplace</code> 系列要低一些。当然这类函数的优点也是显而易见的，就是更<code class="source">安全</code>。
</p>
</li>
</ul>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
几个操作系统强相关的常量 <a id="sep_searchpath"></a><a id="sep_path"></a><a id="system_var"></a><a id="sep_searchpath_char"></a><a id="sep_path_char"></a><a id="sep_line_ends"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static constexpr std::string_view sep_searchpath = &quot;:&quot;;
    static constexpr value_type sep_searchpath_char = &apos;:&apos;;
    static constexpr std::string_view sep_path = &quot;/&quot;;
    static constexpr value_type sep_path_char = &apos;/&apos;;
    static constexpr std::string_view sep_line_ends = &quot;\n&quot;;
</pre>
<ul>
<li>
<code class="source">sep_searchpath</code>, <code class="source">sep_searchpath_char</code> 搜索路径分隔符
</li>
<li>
<code class="source">sep_path</code>, <code class="source">sep_path_char</code> 文件路径分隔符
</li>
<li>
<code class="source">sep_line_ends</code> 换行符
</li>
</ul>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
字符分类 <a id="builtin_charsets"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static constexpr std::string_view all_uppers = &quot;ABCDEFGHIJKLMNOPQRSTUVWXYZ&quot;;
    static constexpr std::string_view all_lowers = &quot;abcdefghijklmnopqrstuvwxyz&quot;;
    static constexpr std::string_view all_leters = &quot;ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz&quot;;
    static constexpr std::string_view all_alphas = &quot;ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz&quot;;
    static constexpr std::string_view all_digits = &quot;0123456789&quot;;
    static constexpr std::string_view all_xdigits = &quot;0123456789ABCDEFabcdef&quot;;
    static constexpr std::string_view all_alnums = &quot;ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789&quot;;
    static constexpr std::string_view all_alnumuls = &quot;ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_&quot;;
    static constexpr std::string_view all_aluls = &quot;ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_&quot;;
    static constexpr std::string_view all_spaces = &quot;\x09\x0A\x0B\x0C\x0d\x20&quot;;
    static constexpr std::string_view all_hex_upper = &quot;0123456789ABCDEF&quot;;
    static constexpr std::string_view all_hex_lower = &quot;0123456789abcdef&quot;;
    static constexpr std::string_view all_puncts = R&quot;(!&quot;#$%&amp;&apos;()*+,-./:;&lt;=&gt;?@[\]^_`{|}~)&quot;;
</pre>
<ul>
<li>
<code class="source">all_uppers</code> 所有大写字母集合 <a id="all_uppers"></a>
</li>
<li>
<code class="source">all_lowers</code> 所有小写字母集合 <a id="all_lowers"></a>
</li>
<li>
<code class="source">all_leters</code> 所有字母集合 <a id="all_leters"></a>
</li>
<li>
<code class="source">all_alphas</code> 所有字母集合 <a id="all_alphas"></a>
</li>
<li>
<code class="source">all_digits</code> 所有数字字符 <a id="all_digits"></a>
</li>
<li>
<code class="source">all_xdigits</code> 所有十六进制数字表示的字符集合 <a id="all_xdigits"></a>
</li>
<li>
<code class="source">all_alnums</code> 所有的字母和数字集合 <a id="all_alnums"></a>
</li>
<li>
<code class="source">all_alnumuls</code> 所有的字母、数字、下划线的集合 <a id="all_alnumuls"></a>
</li>
<li>
<code class="source">all_aluls</code> 所有字母和下滑线的集合 <a id="all_aluls"></a>
</li>
<li>
<code class="source">all_spaces</code> 所有空白字符 <a id="all_spaces"></a>
</li>
<li>
<code class="source">all_hex_upper</code> 所有大写字母形式的十六进制字符集 <a id="all_hex_upper"></a>
</li>
<li>
<code class="source">all_hex_lower</code> 所有小写字母形式的十六进制字符集 <a id="all_hex_lower"></a>
</li>
<li>
<code class="source">all_puncts</code> 所有标点符号 <a id="all_puncts"></a>
</li>
</ul>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
在尾部追加 <a id="append"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto append(std::string_view s, std::string_view other, size_type times_n = 1) -&gt; std::string;
    static auto append(std::string_view s, const view_provider_proc&amp; proc) -&gt; std::string;
    template &lt;typename Sequence = std::initializer_list&lt;std::string_view&gt;, typename = typename Sequence::const_iterator&gt;
    static auto append(std::string_view s, const Sequence&amp; items) -&gt; std::string;
    /// -
    static auto append_inplace(std::string&amp; s, std::string_view other, size_type times_n = 1) -&gt; std::string&amp;;
    static auto append_inplace(std::string&amp; s, const view_provider_proc&amp; proc) -&gt; std::string&amp;;
    template &lt;typename Sequence = std::initializer_list&lt;std::string_view&gt;, typename = typename Sequence::const_iterator&gt;
    static auto append_inplace(std::string&amp; s, const Sequence&amp; items) -&gt; std::string&amp;;
</pre>
<p>
提供了向指定字符尾部追加一个或者多个字符串的能力。实际上，STL 中已经提供了比较丰富的追加字符串，这里针对
大量字符串拼接提供了相对简便的方法。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定向哪个字符串后添加新串。  
</td></tr>
<tr class="tr-param"><td class="td-param-names">other</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被追加的字符串。  
</td></tr>
<tr class="tr-param"><td class="td-param-names">times_n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 重复追加多少次，如果指定为 0，则实际不会做任何追加操作。  
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 由 proc 函数提供被追加的字符串，如果 proc 返回 <code class="source">std::nullopt</code>，表示后续无更多字符串需要追加。  
</td></tr>
<tr class="tr-param"><td class="td-param-names">items</td><td class="td-param-sep">-</td><td class="td-param-desc"> 从容器 items 中获取被追加的字符串。
</td></tr>
</tbody></table>
<p>
@notice{1} 对于 <code class="source">append_inplace</code> 函数，如果 s 与 被插入字符串存在重叠时，函数的行为是不确定的，应该避免出现这种
情况。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
向头部追加 <a id="prepend"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto prepend(std::string_view s, std::string_view other, size_type times_n = 1) -&gt; std::string;
    static auto prepend(std::string_view s, const view_provider_proc&amp; proc) -&gt; std::string;
    template &lt;typename Sequence = std::initializer_list&lt;std::string_view&gt;, typename = typename Sequence::const_iterator&gt;
    static auto prepend(std::string_view s, const Sequence&amp; items) -&gt; std::string;
    /// -
    static auto prepend_inplace(std::string&amp; s, std::string_view other, size_type times_n = 1) -&gt; std::string&amp;;
    static auto prepend_inplace(std::string&amp; s, const view_provider_proc&amp; proc) -&gt; std::string&amp;;
    template &lt;typename Sequence = std::initializer_list&lt;std::string_view&gt;, typename = typename Sequence::const_iterator&gt;
    static auto prepend_inplace(std::string&amp; s, const Sequence&amp; items) -&gt; std::string&amp;;
</pre>
<p>
將一个或者多个字符串追加到指定字符串的前面。实际上，STL 中已经提供了比较丰富的字符串插入函数，这里针对
大量字符串拼接提供了相对简便的方法。需要注意，对于通过 proc 和 items 来提供被追加串的函数，字符串总是以倒
序的方式被追加。比如，<code class="source">str::prepend(&quot;abc&quot;, {&quot;123&quot;, &quot;456&quot;, &quot;789&quot;})</code> 返回的结果是 &quot;789456123abc&quot;。
对于 prepend_inplace 函数，如果 s 与 被插入字符串存在重叠时，函数的行为是不确定的，应该避免出现这种情况。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 所有字符串都追加到该字符串之前
</td></tr>
<tr class="tr-param"><td class="td-param-names">other</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被追加的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">times_n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 重复追加多少次，如果指定为 0，则实际不会做任何追加操作。
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 由 proc 函数提供被追加的字符串，如果 proc 返回 std::nullopt，表示后续无更多字符串需要追加。
</td></tr>
<tr class="tr-param"><td class="td-param-names">items</td><td class="td-param-sep">-</td><td class="td-param-desc"> 从容器 items 中获取被追加的字符串。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
不区分大小写的比较 <a id="icompare"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto icompare(std::string_view s, std::string_view other) -&gt; int;
    static auto icompare(std::string_view s, std::string_view other, size_type max_n) -&gt; int;
</pre>
<p>
<code class="source">icompare</code> 提供了不区分大小写比较的能力，其中 max_n 用于限制最多比较字符数量。特别的，如果 max_n 等于 0，
返回 0；
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 参与比较的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">other</td><td class="td-param-sep">-</td><td class="td-param-desc"> 另一个参与比较的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">max_n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 表示最多比较前 max_n 个字符
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回正数，表示 s 大于 other；返回负值，表示 s 小于 other；返回 0，表示 s 和 other 相等。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
不区分大小写的相等測試 <a id="iequals"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto iequals(std::string_view s, std::string_view other) -&gt; bool;
    static auto iequals(std::string_view s, std::string_view other, size_type max_n) -&gt; bool;
</pre>
<p>
<code class="source">iequals</code> 提供了不区分大小写的相等比较，其中 <code class="source">max_n</code> 用于限制最多比较字符数量。特别的，如果
<code class="source">max_n</code> 等于 0。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 参与比较的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">other</td><td class="td-param-sep">-</td><td class="td-param-desc"> 另一个参与比较的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">max_n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 表示最多比较前 <code class="source">max_n</code> 个字符
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">如果相等，返回 true，否则返回 <code class="source">false</code>
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
基于通配符的匹配检测 <a id="wildcmp"></a><a id="iwildcmp"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto wildcmp(const_pointer s, const_pointer pattern) -&gt; bool;
    static auto wildcmp(std::string_view s, std::string_view pattern) -&gt; bool;
    /// -
    static auto iwildcmp(const_pointer s, const_pointer pattern) -&gt; bool;
    static auto iwildcmp(std::string_view s, std::string_view pattern) -&gt; bool;
</pre>
<p>
测试字符串 s 是否匹配通配符 pattern，<a href="#wildcmp">wildcmp</a> 表示区分大小写，<a href="#iwildcmp">iwildcmp</a> 表示不区分大小写。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被测试的字符串。
</td></tr>
<tr class="tr-param"><td class="td-param-names">pattern</td><td class="td-param-sep">-</td><td class="td-param-desc"> 通配串。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">如果 s 字符串匹配 pattern，返回 true，否则返回 false。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
判断两个字符串的包含关系 <a id="contains"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto contains(std::string_view s, std::string_view other) -&gt; bool;
    static auto contains(std::string_view s, value_type ch) -&gt; bool;
    static auto contains(std::string_view s, const char_match_proc&amp; proc) -&gt; bool;
    static auto contains(std::string_view s, const charset_type&amp; charset) -&gt; bool;
    static auto contains(std::string_view s, const std::regex&amp; pattern) -&gt; bool;
    /// -
    static auto icontains(std::string_view s, std::string_view other) -&gt; bool;
    static auto icontains(std::string_view s, value_type ch) -&gt; bool;
</pre>
<p>
等价于在字符串 s 中查找是否存在指定的字符或者字符串。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 在该字符串查找目标子串
</td></tr>
<tr class="tr-param"><td class="td-param-names">other</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被查找的目标子串
</td></tr>
<tr class="tr-param"><td class="td-param-names">ch</td><td class="td-param-sep">-</td><td class="td-param-desc"> 在 s 中查找是否存在指定的字符
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> s 中的每个字符都会触发 proc 函数，proc 返回 true，表示当前字符是正在被查找的字符；
</td></tr>
<tr class="tr-param"><td class="td-param-names">ignore_case</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定是否采用不区分大小写的方式来查找子串
</td></tr>
<tr class="tr-param"><td class="td-param-names">charset</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定一个字符集，s 中只要有任意一个字符在 charset 中就表示 s 中包含 charset
</td></tr>
<tr class="tr-param"><td class="td-param-names">pattern</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定一个正则表达式，只要 s 中有任意子串匹配 pattern，表示 s 中包含 pattern
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">如果 s 包含指定的字符或者字符串或者某种模式，返回 true，否则返回 false。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
子串统计 <a id="count"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto count(std::string_view s, std::string_view other) -&gt; size_type;
    static auto count(std::string_view s, value_type ch) -&gt; size_type;
    static auto count(std::string_view s, const char_match_proc&amp; proc) -&gt; size_type;
    static auto count(std::string_view s, const charset_type&amp; charset) -&gt; size_type;
    static auto count(std::string_view s, const std::regex&amp; pattern) -&gt; size_type;
</pre>
<p>
本函数用于统计 s 串中是否包含特定模式的子串的数量。需要注意，count 函数统计的子串是不重叠的子串。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 在该字符串中查找
</td></tr>
<tr class="tr-param"><td class="td-param-names">other, ch, charset</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被统计的子串或者字符或者字符集
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于表示满足特定条件字符，是更抽象的字符查找形式
</td></tr>
<tr class="tr-param"><td class="td-param-names">pattern</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于统计满足表达式的子串的数量，多个子串之间可以重叠。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回满足条件的子串或者字符的数量。特别的，当 s 或者 other 为空时，总是返回 0
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
前缀操作 <a id="prefix"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto prefix(std::string_view s, std::string_view other) -&gt; size_type;
    static auto has_prefix(std::string_view s, value_type ch) -&gt; bool;
    static auto has_prefix(std::string_view s, std::string_view prefix) -&gt; bool;
    static auto starts_with(std::string_view s, value_type ch) -&gt; bool;
    static auto starts_with(std::string_view s, std::string_view prefix) -&gt; bool;
    static auto starts_with(std::string_view s, size_type pos, value_type ch) -&gt; bool;
    static auto starts_with(std::string_view s, size_type pos, std::string_view prefix) -&gt; bool;
    static auto remove_prefix_view(std::string_view s, std::string_view prefix) -&gt; std::string_view;
    static auto remove_prefix_view(std::string_view s, value_type prefix) -&gt; std::string_view;
    static auto remove_prefix(std::string_view s, std::string_view prefix) -&gt; std::string;
    static auto remove_prefix(std::string_view s, value_type prefix) -&gt; std::string;
    /// -
    static auto remove_prefix_inplace(std::string&amp; s, std::string_view prefix) -&gt; std::string&amp;;
    static auto remove_prefix_inplace(std::string&amp; s, value_type prefix) -&gt; std::string&amp;;
</pre>
<p>
本组函数提供了常见的前缀操作：
</p>
<ul>
<li>
<code class="source">prefix</code> 提供了计算两个字符串的公共前缀的能力；
</li>
<li>
<code class="source">has_prefix</code>, <code class="source">starts_with</code> 功能一致，都用于测试字符串 s 是否有指定的前缀。
</li>
<li>
<code class="source">remove_prefix_view</code>, <code class="source">remove_prefix</code>,  <code class="source">remove_prefix_inplace</code> 会返回从字符串 s 中去除两个字符串的共同前缀后剩余的部分。
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 目标字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">other</td><td class="td-param-sep">-</td><td class="td-param-desc"> 参与公共前缀计算的另一个字符串，用于 <a href="#prefix">prefix</a>
</td></tr>
<tr class="tr-param"><td class="td-param-names">prefix, ch</td><td class="td-param-sep">-</td><td class="td-param-desc"> 前缀字符串或者字符
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
后缀操作 <a id="suffix"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto suffix(std::string_view s, std::string_view other) -&gt; size_type;
    static auto has_suffix(std::string_view s, value_type ch) -&gt; bool;
    static auto has_suffix(std::string_view s, std::string_view suffix) -&gt; bool;
    static auto ends_with(std::string_view s, value_type ch) -&gt; bool;
    static auto ends_with(std::string_view s, std::string_view suffix) -&gt; bool;
    static auto remove_suffix_view(std::string_view s, std::string_view suffix) -&gt; std::string_view;
    static auto remove_suffix_view(std::string_view s, value_type ch) -&gt; std::string_view;
    static auto remove_suffix(std::string_view s, std::string_view suffix) -&gt; std::string;
    static auto remove_suffix(std::string_view s, value_type ch) -&gt; std::string;
    /// -
    static auto remove_suffix_inplace(std::string&amp; s, std::string_view suffix) -&gt; std::string&amp;;
    static auto remove_suffix_inplace(std::string&amp; s, value_type ch) -&gt; std::string&amp;;
</pre>
<p>
本组函数提供了常见的前缀操作:
</p>
<ul>
<li>
<code class="source">suffix</code> 提供了计算两个字符串 s 和 other 的公共前缀的长度，返回 0 表示无前缀；
</li>
<li>
<code class="source">has_suffix</code>, <code class="source">ends_with</code> 功能一致，都用于测试字符串 s 是否有指定的后缀。
</li>
<li>
<code class="source">remove_suffix_view</code>, <code class="source">remove_suffix</code>, <code class="source">remove_suffix_inplace</code> 会返回从字符串 s 中去除两个字符串的共同后剩 余的部分。
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 目标字符串。
</td></tr>
<tr class="tr-param"><td class="td-param-names">other</td><td class="td-param-sep">-</td><td class="td-param-desc"> 参与公共前缀计算的另一个字符串，用于 <a href="#suffix">suffix</a>。
</td></tr>
<tr class="tr-param"><td class="td-param-names">suffix, ch</td><td class="td-param-sep">-</td><td class="td-param-desc"> 后缀字符串或者字符。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
检测是否以特定的模式开头和结束 <a id="spaces_margin"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto starts_with_spaces(std::string_view s) -&gt; bool;
    static auto ends_with_spaces(std::string_view s) -&gt; bool;
    static auto starts_with_margin(std::string_view s, value_type margin) -&gt; bool;
</pre>
<ul>
<li>
<code class="source">starts_with_spaces</code> 如果字符串 <code class="source">s</code> 以空白开头，返回 <code class="source">true</code>，否则返回 <code class="source">false</code>。
</li>
<li>
<code class="source">ends_with_spaces</code> 如果字符串 <code class="source">s</code> 以空白结尾，返回 <code class="source">true</code>，否则返回 <code class="source">false</code>。
</li>
<li>
<code class="source">starts_with_margin</code> 如果字符串 <code class="source">s</code> 的首个非空白字母为 <code class="source">margin</code> 字符，返回 <code class="source">true</code>，否则返回 <code class="source">false</code>。
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被检查的参数。
</td></tr>
<tr class="tr-param"><td class="td-param-names">margin</td><td class="td-param-sep">-</td><td class="td-param-desc"> 边界字符。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
定位字符位置 <a id="next_char"></a><a id="prev_char"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto next_char(std::string_view s, size_type&amp; pos, value_type ch) -&gt; std::optional&lt;size_type&gt;;
    static auto next_char(std::string_view s, size_type&amp; pos, const charset_type&amp; charset) -&gt; std::optional&lt;size_type&gt;;
    static auto next_char(std::string_view s, size_type&amp; pos, std::string_view charset) -&gt; std::optional&lt;size_type&gt;;
    static auto next_char(std::string_view s, size_type&amp; pos, const char_match_proc&amp; proc) -&gt; std::optional&lt;size_type&gt;;
    /// -
    static auto prev_char(std::string_view s, size_type&amp; pos, value_type ch) -&gt; std::optional&lt;size_type&gt;;
    static auto prev_char(std::string_view s, size_type&amp; pos, const charset_type&amp; charset) -&gt; std::optional&lt;size_type&gt;;
    static auto prev_char(std::string_view s, size_type&amp; pos, std::string_view charset) -&gt; std::optional&lt;size_type&gt;;
    static auto prev_char(std::string_view s, size_type&amp; pos, const char_match_proc&amp; proc) -&gt; std::optional&lt;size_type&gt;;
</pre>
<p>
在字符串 s 从特定的位置 <code class="source">pos</code> 开始向前或者向后查找、定位特定的字符；如果找到，返回该字符在字符串中的位置；
否则返回 <code class="source">std::nullopt</code> 。
</p>
<ul>
<li>
<code class="source">next_char</code> 总是从 <code class="source">pos</code> 开始向字符串尾部查找特定的字符，并返回找到的该字符的位置。
</li>
<li>
<code class="source">prev_char</code> 总是从 <code class="source">pos - 1</code> 开始向字符串首部查找特定的字符，并返回找到的该字符的位置。
</li>
</ul>
<p>
@notice{1} 需要特别注意，next_xxx 总是从 <code class="source">pos - 1</code> 开始，向字符串首部查找（而 <code class="source">next_char</code> 总是从
<code class="source">pos</code> 开始向字符串尾部查找）。因此，如果希望从 <code class="source">s</code> 的最后一个字符开始向前查找时，<code class="source">pos</code> 应该指定为 <code class="source">s.size()</code>。
当调用 <code class="source">prev_xxx</code> 系列函数且 <code class="source">pos</code> 参数指定的值为 <code class="source">0</code> 时， <code class="source">prev_xxx</code> 系列函数已无法继续向前查找，
此时将返回 <code class="source">std::nullopt</code>。同理，当调用 next_xxx 系列函数，且 <code class="source">pos</code> 参数大于或者等于 <code class="source">s.size()</code> 时，
<code class="source">prev_xxx</code> 系列函数同样无法继续查找，此时也将返回 <code class="source">std::nullopt</code>。
</p>
<p>
@notice{2} 当找到特定的字符时，<code class="source">next_char</code> 的 <code class="source">pos</code> 总是为该字符的下一个字符，而 <code class="source">pref_char</code> 的
<code class="source">pos</code> 总是指向该字符所在的位置。这个设计使得 <code class="source">next_char</code> 和 <code class="source">prev_char</code> 可以配合使用。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 在该字符串中查找
</td></tr>
<tr class="tr-param"><td class="td-param-names">pos</td><td class="td-param-sep">-</td><td class="td-param-desc"> 查找的起始位置，需要注意该字段对于 <code class="source">next_xxx</code> 和 <code class="source">prev_xxx</code> 具有不同的含义（参见 @ref{#notice{2}}）。
</td></tr>
<tr class="tr-param"><td class="td-param-names">ch, charset</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于定位的字符或者字符集。
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于测试某个字符是否满足定位条件，常用于定制场景。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
定位子串 <a id="next_string"></a><a id="prev_string"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto next_string_range(std::string_view s, size_type&amp; pos, std::string_view substr) -&gt; std::optional&lt;range_type&gt;;
    static auto next_string_view(std::string_view s, size_type&amp; pos, std::string_view substr) -&gt; std::optional&lt;std::string_view&gt;;
    static auto next_string(std::string_view s, size_type&amp; pos, std::string_view substr) -&gt; std::optional&lt;std::string&gt;;
    /// -
    static auto prev_string_range(std::string_view s, size_type&amp; pos, std::string_view substr) -&gt; std::optional&lt;range_type&gt;;
    static auto prev_string_view(std::string_view s, size_type&amp; pos, std::string_view substr) -&gt; std::optional&lt;std::string_view&gt;;
    static auto prev_string(std::string_view s, size_type&amp; pos, std::string_view substr) -&gt; std::optional&lt;std::string&gt;;
</pre>
<p>
在字符串 <code class="source">s</code> 中，查找（定位）子串 <code class="source">substr</code> 的位置。如果找到返回找到的该子串的位置（<code class="source">range_type</code>）、
视图（<code class="source">std::string_view</code>）、或者子串本身（<code class="source">std::string</code>）；否则，返回 <code class="source">std::nullopt</code>。
</p>
<p>
@notice{1} 需要特别注意，next_xxx 总是从 <code class="source">pos - 1</code> 开始，向字符串首部查找（而 next_char 总是从
<code class="source">pos</code> 开始向字符串尾部查找）。因此，如果希望从 <code class="source">s</code> 的最后一个字符开始向前查找时，<code class="source">pos</code> 应该指定为 <code class="source">s.size()</code>。
当调用 prev_xxx 系列函数且 <code class="source">pos</code> 参数指定的值为 <code class="source">0</code> 时， prev_xxx 系列函数已无法继续向前查找，此时
将返回 <code class="source">std::nullopt</code>。同理，当调用 next_xxx 系列函数，且 <code class="source">pos</code> 参数大于或者等于 <code class="source">s.size()</code>
时，prev_xxx 系列函数同样无法继续查找，此时也将返回 <code class="source">std::nullopt</code>。
</p>
<p>
@notice{2} 当找到特定的字符时，next_xxx 的 <code class="source">pos</code> 总是为该字符的下一个字符，而 pref_xxx 的 <code class="source">pos</code>
总是指向该字符所在的位置。这个设计使得 next_xxx 和 prev_xxx 可以配合使用。
</p>
<ul>
<li>
<code class="source">next_string_range</code>, <code class="source">next_string_view</code>, <code class="source">next_string</code> 从字符串 <code class="source">s</code> 的 <code class="source">pos</code> 位置开始向字符串尾部，查找 <code class="source">substr</code>。
</li>
<li>
<code class="source">prev_string_range</code>, <code class="source">prev_string_view</code>, <code class="source">prev_string</code> 从字符串 <code class="source">s</code> 的 <code class="source">pos - 1</code> 位置开始向字符串首部，查找 <code class="source">substr</code>。
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 在该字符串中查找（定位）子串。
</td></tr>
<tr class="tr-param"><td class="td-param-names">pos</td><td class="td-param-sep">-</td><td class="td-param-desc"> 查找的起始位置，需要注意该字段对于 <code class="source">next_xxx</code> 和 <code class="source">prev_xxx</code> 具有不同的含义（参见 @ref{#notice{2}}）。
</td></tr>
<tr class="tr-param"><td class="td-param-names">substr</td><td class="td-param-sep">-</td><td class="td-param-desc"> 待查找（定位）的子串。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
定位行结束符（换行符） <a id="next_eol"></a><a id="prev_eol"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto next_eol_range(std::string_view s, size_type&amp; pos) -&gt; std::optional&lt;range_type&gt;;
    static auto next_eol_view(std::string_view s, size_type&amp; pos) -&gt; std::optional&lt;std::string_view&gt;;
    static auto next_eol(std::string_view s, size_type&amp; pos) -&gt; std::optional&lt;std::string&gt;;
    /// -
    static auto prev_eol_range(std::string_view s, size_type&amp; pos) -&gt; std::optional&lt;range_type&gt;;
    static auto prev_eol_view(std::string_view s, size_type&amp; pos) -&gt; std::optional&lt;std::string_view&gt;;
    static auto prev_eol(std::string_view s, size_type&amp; pos) -&gt; std::optional&lt;std::string&gt;;
</pre>
<p>
从 pos 开始查找（定位）行结束符（End-Of-Line）的位置，可用于按行拆分字符串场景。
</p>
<p>
 由于操作系统的差异，行结束符并非总是 <code class="source">\n</code>，本系列函数采用下面的策略来识别行结束符
</p>
<ul>
<li>
如果当前字符为 <code class="source">\n</code>，那么将当前字符 <code class="source">\n</code> 识别为行结束符；
</li>
<li>
如果当前字符为 <code class="source">\r</code>，那么还需要额外查看下一个字符，如果下一个字符为 <code class="source">\n</code>，那么将 <code class="source">\r\n</code> 整体视作行结束符；否则将 <code class="source">\r</code>
<p>
  视作行结束符。
</p>
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 在该字符串内查找行结束符
</td></tr>
<tr class="tr-param"><td class="td-param-names">pos</td><td class="td-param-sep">-</td><td class="td-param-desc"> 查找的起始位置，需要注意该字段对于 <code class="source">next_xxx</code> 和 <code class="source">prev_xxx</code> 具有不同的含义（参见 @ref{#notice{2}}）。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
行结束符检测 <a id="eol"></a><a id="ends_with_eol"></a><a id="eol_suffix"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto ends_with_eol(std::string_view s) -&gt; bool;
    static auto has_eol_suffix(std::string_view s) -&gt; bool;
    static auto eol_suffix(std::string_view s) -&gt; size_type;
</pre>
<ul>
<li>
<code class="source">ends_with_eol</code>, <code class="source">has_eol_suffix</code> 检查字符串 <code class="source">s</code> 是否具有行结束符
</li>
<li>
<code class="source">eol_suffix</code> 检查字符串 <code class="source">s</code> 是否以行结束符结尾，如果有返回行结束符的长度，否则返回 0
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被检测的字符
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
移除行结束符 <a id="remove_eol"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto remove_eol_suffix_range(std::string_view s) -&gt; range_type;
    static auto remove_eol_suffix_view(std::string_view s) -&gt; std::string_view;
    static auto remove_eol_suffix(std::string_view s) -&gt; std::string;
    static auto remove_eol_suffix_inplace(std::string&amp; s) -&gt; std::string&amp;;
</pre>
<p>
返回去除字符串 <code class="source">s</code> 尾部的行结束符后的新串。需要注意，不同函数返回的数据类型有差别。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 待移出行结束符的原始字符串。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
定位空白块 <a id="next_spaces"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto next_spaces_pos(std::string_view s, size_type&amp; pos) -&gt; size_type;
    static auto next_spaces(std::string_view s, size_type&amp; pos) -&gt; std::optional&lt;size_type&gt;;
    static auto next_spaces_range(std::string_view s, size_type&amp; pos) -&gt; std::optional&lt;range_type&gt;;
    static auto next_spaces_view(std::string_view s, size_type&amp; pos) -&gt; std::optional&lt;std::string_view&gt;;
    /// -
    static auto prev_spaces_pos(std::string_view s, size_type&amp; pos) -&gt; size_type;
    static auto prev_spaces(std::string_view s, size_type&amp; pos) -&gt; std::optional&lt;size_type&gt;;
    static auto prev_spaces_range(std::string_view s, size_type&amp; pos) -&gt; std::optional&lt;range_type&gt;;
    static auto prev_spaces_view(std::string_view s, size_type&amp; pos) -&gt; std::optional&lt;std::string_view&gt;;
</pre>
<p>
<code class="source">next_spaces_pos</code> 在字符串 <code class="source">s</code> 中，从 <code class="source">pos</code> 位置开始查找下一个空白块的位置，并返回该空白块的起始位置。
如果未找到，返回 <code class="source">std::nullopt</code>。
</p>
<p>
@notice{1} “空白块”是指由 <a href="#all_spaces">all_spaces</a> 中的字符组成的连续的子串。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 在该字符串中查找空白块
</td></tr>
<tr class="tr-param"><td class="td-param-names">pos</td><td class="td-param-sep">-</td><td class="td-param-desc"> 作为输入参数时，表示查找空白块的起始位置；作为输出参数时，表示找到的空白块最后一个空白字符的之后的位置。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
特征测试 <a id="is_xx"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto is_lower(std::string_view s) -&gt; bool;
    static auto is_upper(std::string_view s) -&gt; bool;
    static auto is_title(std::string_view s) -&gt; bool;
    static auto is_capitalize(std::string_view s) -&gt; bool;
    static auto is_digit(std::string_view s) -&gt; bool;
    static auto is_xdigit(std::string_view s) -&gt; bool;
    static auto is_ascii(std::string_view s) -&gt; bool;
    static auto is_alpha(std::string_view s) -&gt; bool;
    static auto is_alnum(std::string_view s) -&gt; bool;
    static auto is_alnumul(std::string_view s) -&gt; bool;
    static auto is_space(std::string_view s) -&gt; bool;
    static auto is_blank(std::string_view s) -&gt; bool;
    static auto is_print(std::string_view s) -&gt; bool;
    static auto is_graph(std::string_view s) -&gt; bool;
</pre>
<ul>
<li>
<code class="source">s_lower</code>: 检测 <code class="source">s</code> 中的所有字母都是小写（参考 <code class="source">std::islower</code>）。
</li>
<li>
<code class="source">s_upper</code>: 检测 <code class="source">s</code> 中的所有字母都是大写字母（参考 <code class="source">std::isupper</code>）。
</li>
<li>
<code class="source">s_capitalize</code>: 检测 <code class="source">s</code> 的首个字符是否为大写字母。
</li>
<li>
<code class="source">s_title</code>:  对于给定的字符串 <code class="source">s</code> 中，以空白分割的每个子串(单词)，如果其每个子串的首字符都是非字母或者是大写字母返回
<p>
  <code class="source">true</code>。
</p>
</li>
<li>
<code class="source">s_digit</code>: 检测 <code class="source">s</code> 否所有的字符都是数字或者十六进制字符（参考 <code class="source">std::xdigit</code>）。
</li>
<li>
<code class="source">s_xdigit</code>: 检测 <code class="source">s</code> 否所有的字符都是数字或者十六进制字符（参考 <code class="source">std::xdigit</code>）。
</li>
<li>
<code class="source">s_ascii</code>: 检测 <code class="source">s</code> 中的所有字符是否都在 ASCII 范围内。
</li>
<li>
<code class="source">s_alpha</code>: 检测 <code class="source">s</code> 是否全都为字母（参考 <code class="source">std::isalpha</code>）。
</li>
<li>
<code class="source">s_alnum</code>: 检测 <code class="source">s</code> 是否全都为字母或者数字（参考 <code class="source">std::isalnum</code>）。
</li>
<li>
<code class="source">s_alnumul</code>: 检测 <code class="source">s</code> 是否全都为字母或者数字或者下划线。
</li>
<li>
<code class="source">s_space</code>: 检测 <code class="source">s</code> 是否全都为空白字符（参考 <code class="source">std::isspace</code>）。
</li>
<li>
<code class="source">s_blank</code>: 检测 <code class="source">s</code> 是否全都为空格字符（参考 <code class="source">std::isblank</code>）。
</li>
<li>
<code class="source">s_print</code>: 检测 <code class="source">s</code> 是否全都为可打印字符（参考 <code class="source">std::isprint</code>）。
</li>
<li>
<code class="source">s_graph</code>: 检测 <code class="source">s</code> 是否全都为图形符（参考 <code class="source">std::isgraph</code>）。
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被测试的字符串
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">所有的字符串都必须按组共同的特征，才会返回 <code class="source">true</code>，否则，（包括 <code class="source">s</code> 为空串场景）均返回 <code class="source">false</code>。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
特征测试：常见词法特征类 <a id="is_literal"></a><a id="is_identifier"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto is_identifier(std::string_view s) -&gt; bool;
    static auto is_literal_bool(std::string_view s) -&gt; bool;
    static auto is_literal_true(std::string_view s) -&gt; bool;
    static auto is_literal_false(std::string_view s) -&gt; bool;
    static auto is_literal_integer(std::string_view s) -&gt; bool;
    static auto is_literal_real(std::string_view s) -&gt; bool;
</pre>
<ul>
<li>
<code class="source">is_identifier</code> 检查字符串 <code class="source">s</code> 是否满足一个标识符的特征，即以大小写字母或者下划线开头且后续字符为字母数字或者下划线。
</li>
<li>
<code class="source">is_literal_bool</code> 检查字符串 <code class="source">s</code> 是否是 bool 值的特征，等价于 <code class="source">(is_literal_true(s) || is_literal_false(s))</code>。
</li>
<li>
<code class="source">is_literal_true</code> 检查字符串 <code class="source">s</code> 是否可以被视作 <code class="source">true</code>。
</li>
<li>
<code class="source">is_literal_false</code> 检查字符串 <code class="source">s</code> 是否可以被视作 <code class="source">false</code>。
</li>
<li>
<code class="source">is_literal_integer</code> 检查字符串 s 是否可以被视作整数。
</li>
<li>
<code class="source">is_literal_real</code> 检查字符串 s 是否可以被视作浮点数，需要注意整数本身也可以被视作浮点数。
</li>
</ul>
<p>
@notice{1} 针对各种字符特征，下面为更具体的解释：
</p>
<ul>
<li>
被视作 <code class="source">true</code> 的字符串包括 <code class="source">&quot;1&quot;</code>, <code class="source">&quot;on&quot;</code>, <code class="source">&quot;ON&quot;</code>, <code class="source">&quot;Yes&quot;</code>, <code class="source">&quot;yes&quot;</code>, <code class="source">&quot;YES&quot;</code>, <code class="source">&quot;True&quot;</code>, <code class="source">&quot;true&quot;</code>, <code class="source">&quot;TRUE&quot;</code>
</li>
<li>
被视作 <code class="source">false</code> 的字符串包括 <code class="source">&quot;0&quot;</code>, <code class="source">&quot;off&quot;</code>, <code class="source">&quot;OFF&quot;</code>, <code class="source">&quot;No&quot;</code>, <code class="source">&quot;no&quot;</code>, <code class="source">&quot;NO&quot;</code>, <code class="source">&quot;False&quot;</code>, <code class="source">&quot;false&quot;</code>, <code class="source">&quot;FALSE&quot;</code>
</li>
<li>
被视作 <code class="source">real</code> 的字符串，等价于匹配正则表达式 <code class="source">[+-]?(([0-9]+)|([0-9]+\.)|(\.[0-9]+))([Ee][+-]?[0-9]+)?</code>
</li>
<li>
被视作 <code class="source">integer</code> 的字符串，等价于匹配正则表达式 <code class="source">[+-]?[0-9]+</code>
</li>
<li>
被视作 <code class="source">identifier</code> 的字符串，等价于正则表达式 <code class="source">[A-Za-z_][0-9A-Za-z_]*</code>
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被测试的字符串。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">所有的字符串都必须按组共同的特征，才会返回 <code class="source">true</code>，否则，（包括 <code class="source">s</code> 为空串场景）均返回 <code class="source">false</code>。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
特征测试：指定字符集类 <a id="is_all_in"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">

    //! 是否空白或者控
    static auto is_space_or_empty(std::string_view s) -&gt; bool;
</pre>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被测试的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于测试 s 中的每个字符是否满足给定条件的函数
</td></tr>
<tr class="tr-param"><td class="td-param-names">charset</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定需要满足条件的字符集
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">所有的字符串都必须按组共同的特征，才会返回 true，否则，（包括 s 为空串场景）均返回 false。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
特征测试：单一条件类 <a id="has_any_one"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
</pre>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被测试的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于测试 <code class="source">s</code> 中的每个字符是否满足给定条件的函数
</td></tr>
<tr class="tr-param"><td class="td-param-names">charset</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定需要满足条件的字符集
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">与 <code class="source">is_xxx</code> 系列函数需要“所有字符必须全部满足指定特征”不同，<code class="source">has_xxx</code> 系列函数只需要有任意一个字符满足特征，立即返回
</p>
<p>
<code class="source">true</code>。唯一的特例是空串总是返回 <code class="source">false</code>。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
提取子串：基于位置 <a id="take"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto take_left_view(std::string_view s, size_type n) -&gt; std::string_view;
    static auto take_right_view(std::string_view s, size_type n) -&gt; std::string_view;
    static auto take_view(std::string_view s, size_type pos, size_type n) -&gt; std::string_view;
    static auto take_view(std::string_view s, size_type pos) -&gt; std::string_view;
    static auto take_view(std::string_view s, range_type range) -&gt; std::string_view;
    static auto take_view(std::string_view s, interval_type inter) -&gt; std::string_view;
    static auto take_view(std::string_view s, shifter_type shifter) -&gt; std::string_view;
    /// -
    static auto take_left(std::string_view s, size_type n) -&gt; std::string;
    static auto take_right(std::string_view s, size_type n) -&gt; std::string;
    static auto take(std::string_view s, size_type pos, size_type n) -&gt; std::string;
    static auto take(std::string_view s, size_type pos) -&gt; std::string;
    static auto take(std::string_view s, range_type range) -&gt; std::string;
    static auto take(std::string_view s, interval_type inter) -&gt; std::string;
    static auto take(std::string_view s, shifter_type slider) -&gt; std::string;
    /// -
    static auto take_left_inplace(std::string&amp; s, size_type n) -&gt; std::string&amp;;
    static auto take_right_inplace(std::string&amp; s, size_type n) -&gt; std::string&amp;;
    static auto take_inplace(std::string&amp; s, size_type pos, size_type n) -&gt; std::string&amp;;
    static auto take_inplace(std::string&amp; s, range_type range) -&gt; std::string&amp;;
    static auto take_inplace(std::string&amp; s, size_type pos) -&gt; std::string&amp;;
    static auto take_inplace(std::string&amp; s, interval_type inter) -&gt; std::string&amp;;
    static auto take_inplace(std::string&amp; s, shifter_type slider) -&gt; std::string&amp;;
</pre>
<ul>
<li>
<code class="source">take_left_view</code>, <code class="source">take_left</code>, <code class="source">take_left_inplace</code>: 返回字符串 s 的最左边前 n 个字符的子串
</li>
<li>
<code class="source">take_right_view</code>, <code class="source">take_right</code>, <code class="source">take_right_inplace</code>: 返回字符串 s 的最右边前 n 个字符的子串
</li>
<li>
<code class="source">take_view</code>, <code class="source">take</code>, <code class="source">take_inplace</code>: 返回字符串 s 中，从pos 位置开始的 n个字符组成的子串
</li>
<li>
<code class="source">take_view</code>, <code class="source">take</code>, <code class="source">take_inplace</code>: 返回字符串 s 中，range 范围的子串。
</li>
<li>
<code class="source">take_view</code>, <code class="source">take</code>, <code class="source">take_inplace</code>: 返回字符串 s 中，从 pos 开始偏移 shifter 的字符串。
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 原始字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定提取的子串的最大长度。当 n 为 0 时，总是返回空串。当按照指定的方式无法获得 n 个字符的长度
</td></tr>
</tbody></table>
<p>
时，相关函数总是试图返回尽可能多的字符串。
</p>
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">pos</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于指定待提取的子串的起始位置
</td></tr>
<tr class="tr-param"><td class="td-param-names">range</td><td class="td-param-sep">-</td><td class="td-param-desc">  指定用于需要提取的子串的范围，用于 <code class="source">pos</code> 和 <code class="source">len</code> 形式的范围。
</td></tr>
<tr class="tr-param"><td class="td-param-names">inter</td><td class="td-param-sep">-</td><td class="td-param-desc">  指定用于需要提取的子串的范围，用于 <code class="source">begin</code> 和 <code class="source">end</code> 形式的范围。
</td></tr>
<tr class="tr-param"><td class="td-param-names">shifter</td><td class="td-param-sep">-</td><td class="td-param-desc">  指定用于需要提取的子串的范围，用于 <code class="source">ppos</code> 和 <code class="source">offset</code> 形式的范围。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
删除子串：基于位置 <a id="drop"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto drop_left_view(std::string_view s, size_type n) -&gt; std::string_view;
    static auto drop_right_view(std::string_view s, size_type n) -&gt; std::string_view;
    static auto drop_left(std::string_view s, size_type n) -&gt; std::string;
    static auto drop_right(std::string_view s, size_type n) -&gt; std::string;
    static auto drop(std::string_view s, size_type pos, size_type n) -&gt; std::string;
    static auto drop(std::string_view s, size_type pos) -&gt; std::string;
    static auto drop(std::string_view s, range_type range) -&gt; std::string;
    static auto drop(std::string_view s, interval_type inter) -&gt; std::string;
    static auto drop(std::string_view s, shifter_type shifter) -&gt; std::string;
    static auto drop(std::string_view s, const char_match_proc&amp; proc) -&gt; std::string;
    static auto drop(std::string_view s, const charset_type&amp; charset) -&gt; std::string;
    //
    static auto drop_left_inplace(std::string&amp; s, size_type n) -&gt; std::string&amp;;
    static auto drop_right_inplace(std::string&amp; s, size_type n) -&gt; std::string&amp;;
    static auto drop_inplace(std::string&amp; s, size_type pos, size_type n) -&gt; std::string&amp;;
    static auto drop_inplace(std::string&amp; s, size_type pos) -&gt; std::string&amp;;
    static auto drop_inplace(std::string&amp; s, range_type range) -&gt; std::string&amp;;
    static auto drop_inplace(std::string&amp; s, interval_type inter) -&gt; std::string&amp;;
    static auto drop_inplace(std::string&amp; s, shifter_type shifter) -&gt; std::string&amp;;
    static auto drop_inplace(std::string&amp; s, const char_match_proc&amp; proc) -&gt; std::string&amp;;
    static auto drop_inplace(std::string&amp; s, const charset_type&amp; charset) -&gt; std::string&amp;;
</pre>
<ul>
<li>
<code class="source">drop_left_view</code>, <code class="source">drop_left</code>, <code class="source">drop_left_inplace</code>: 返回去掉字符串 s 的最左边前 n 个字符后的子串
</li>
<li>
<code class="source">drop_right_view</code>, <code class="source">drop_right</code>, <code class="source">drop_right_inplace</code>: 返回去掉字符串 s 的最右边的 n 个字符后的子串
</li>
<li>
<code class="source">drop</code>, <code class="source">drop_inplace</code>: 返回从字符串 s 中剔除指定范围或者模式的字符后的剩余的字符串。
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 原始字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定删除的子串的最大长度。
</td></tr>
<tr class="tr-param"><td class="td-param-names">pos</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于指定需要剔除的子串的起始位置。
</td></tr>
<tr class="tr-param"><td class="td-param-names">range</td><td class="td-param-sep">-</td><td class="td-param-desc">  指定用于需要剔除的子串的范围，用于 <code class="source">pos</code> 和 <code class="source">len</code> 形式的范围。
</td></tr>
<tr class="tr-param"><td class="td-param-names">inter</td><td class="td-param-sep">-</td><td class="td-param-desc">  指定用于需要剔除的子串的范围，用于 <code class="source">begin</code> 和 <code class="source">end</code> 形式的范围。
</td></tr>
<tr class="tr-param"><td class="td-param-names">shifter</td><td class="td-param-sep">-</td><td class="td-param-desc">  指定用于需要剔除的子串的范围，用于 <code class="source">ppos</code> 和 <code class="source">offset</code> 形式的范围。
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于指定需要剔除的字符。
</td></tr>
<tr class="tr-param"><td class="td-param-names">charset</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于指定需要剔除的字符集。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回剔除特定字符或者子串后的剩余部分。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
对齐 <a id="align"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto align_left(std::string_view s, size_type width, value_type ch = &apos; &apos;) -&gt; std::string;
    static auto align_right(std::string_view s, size_type width, value_type ch = &apos; &apos;) -&gt; std::string;
    static auto align_center(std::string_view s, size_type width, value_type ch = &apos; &apos;) -&gt; std::string;
    static auto align_zfill(std::string_view s, size_type width) -&gt; std::string;
    static auto align_left_inplace(std::string&amp; s, size_type width, value_type ch = &apos; &apos;) -&gt; std::string&amp;;
    static auto align_right_inplace(std::string&amp; s, size_type width, value_type ch = &apos; &apos;) -&gt; std::string&amp;;
    static auto align_center_inplace(std::string&amp; s, size_type width, value_type ch = &apos; &apos;) -&gt; std::string&amp;;
    static auto align_zfill_inplace(std::string&amp; s, size_type width) -&gt; std::string&amp;;
</pre>
<ul>
<li>
<code class="source">align_left</code>, <code class="source">align_left_inplace</code>: 在字符串 s 尾部填充 ch，直到字符串长度达到 width，以使得字符串看起来是左对齐的效果
</li>
<li>
<code class="source">align_right</code>, <code class="source">align_right_inplace</code>: 在字符串 s 头部追加 ch，直到字符串长度达到 width，以使得字符串看起来是左对齐的效果
</li>
<li>
<code class="source">align_center</code>, <code class="source">align_center_inplace</code>: 在字符串 s 首尾添加 ch，直到字符串长度达到 width，以使得字符串看起来是居中对齐的效果
</li>
<li>
<code class="source">align_zfill</code>, <code class="source">align_zfill_inplace</code>: 在字符串 s 头部添加字符 &apos;0&apos;，使得字符串看起来被补齐了前缀 0，这通常用于全是数字的字符串的场景
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被对齐的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">widht</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定新生成的字符串的宽度，如果 width 小于或者等于 s 的长度，不会追加 ch，也即对齐前后字符串内容不变
</td></tr>
<tr class="tr-param"><td class="td-param-names">ch</td><td class="td-param-sep">-</td><td class="td-param-desc"> 当 width 大于 s 的长度时，所采用的填充字符
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
多行文本处理 <a id="lines"></a><a id="foreach_lines"></a><a id="count_lines"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto foreach_lines(std::string_view s, bool keep_ends, const line_consumer_proc&amp; proc) -&gt; void;
    static auto count_lines(std::string_view s) -&gt; size_type;
</pre>
<ul>
<li>
<code class="source">foreach_lines</code> 用于按行遍历。
</li>
<li>
<code class="source">count_lines</code> 字符串 <code class="source">s</code> 中实际有多少行。
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 包含多行文本而串。
</td></tr>
<tr class="tr-param"><td class="td-param-names">keep_ends</td><td class="td-param-sep">-</td><td class="td-param-desc"> 是否保留行结束符。
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于接收每个遍历的行。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
以单词为单位的处理算法 <a id="words"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto foreach_words(std::string_view s, size_type pos, const range_consumer_proc&amp; proc) -&gt; void;
    static auto foreach_words(std::string_view s, size_type pos, const view_consumer_proc&amp; proc) -&gt; void;
    static auto foreach_words(std::string_view s, const range_consumer_proc&amp; proc) -&gt; void;
    static auto foreach_words(std::string_view s, const view_consumer_proc&amp; proc) -&gt; void;
    /// -
    static auto count_words(std::string_view s) -&gt; size_type;
    /// -
    static auto next_word_view(std::string_view s, size_type&amp; pos) -&gt; std::string_view;
    static auto next_word_range(std::string_view s, size_type&amp; pos) -&gt; range_type;
    static auto next_word(std::string_view s, size_type&amp; pos) -&gt; std::string;
    /// -
    static auto prev_word_view(std::string_view s, size_type&amp; pos) -&gt; std::string_view;
    static auto prev_word_range(std::string_view s, size_type&amp; pos) -&gt; range_type;
    static auto prev_word(std::string_view s, size_type&amp; pos) -&gt; std::string;
    /// -
    static auto split_words(std::string_view s, size_type max_n = npos) -&gt; std::vector&lt;std::string&gt;;
    /// -
    static auto starts_with_word(std::string_view s, std::string_view word) -&gt; bool;
    static auto ends_with_word(std::string_view s, std::string_view word) -&gt; bool;
</pre>
<p>
@notice{1} 这里的单词（word）是指连续的非空白字符序列。
</p>
<ul>
<li>
@ref #foreach_words: 用于遍历字符串中的每个单词
</li>
<li>
@ref #count_words: 用于统计字符串中的单词的数量
</li>
<li>
@ref #next_word_view, #next_word_range, #next_word: 用于从指定的位置开始向字符串的尾部查找下一个单词
</li>
<li>
@ref #prev_word_view, #prev_word_range, #prev_word: 用于从指定的位置开始向字符串的首部查找前一个单词
</li>
<li>
@ref #split_words: 以空格为分隔符从字符串 s 中拆分出多个单词
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被查找或者统计的原始字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">pos</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定起始位置，需要注意在 next_xxx 和 prev_xxx 函数中，pos 的含义的区别。
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定一个函数，用来接受遍历每一个被视作单词的子串
</td></tr>
<tr class="tr-param"><td class="td-param-names">max_n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于 <code class="source">split_words</code> 函数，用于限制拆分出来的单词的数量（注意并非拆分次数）
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
用指定的模式串环绕字符串 <a id="surround"></a><a id="unsurround"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto surround(std::string_view s, std::string_view left = &quot;(&quot;, std::string_view right = &quot;)&quot;) -&gt; std::string;
    static auto surround_inplace(std::string&amp; s, std::string_view left = &quot;(&quot;, std::string_view right = &quot;)&quot;) -&gt; std::string&amp;;
    /// -
    static auto unsurround_view(std::string_view s, std::string_view left = &quot;(&quot;, std::string_view right = &quot;)&quot;) -&gt; std::string_view;
    static auto unsurround(std::string_view s, std::string_view left = &quot;(&quot;, std::string_view right = &quot;)&quot;) -&gt; std::string;
    static auto unsurround_inplace(std::string&amp; s, std::string_view left = &quot;(&quot;, std::string_view right = &quot;)&quot;) -&gt; std::string&amp;;
</pre>
<p>
<code class="source">surround</code> 和 <code class="source">unsurround</code> 系列函数，同时在字符串两端操作，常用于添加括号和去掉括号场景。
其中，<code class="source">left</code> 和 <code class="source">right</code> 表示字符串首尾需要添加或者去掉的子串。对于 unsurround 系列函数，如果字符串的首
或者尾，分别无法匹配 <code class="source">left</code> 或者 <code class="source">right</code> 子串，那么不进行任何操作。
</p>
<ul>
<li>
<code class="source">surround</code>, <code class="source">surround_inplace</code> 向给定的字符串的首位添加特定的子串
</li>
<li>
<code class="source">unsurround</code>, <code class="source">unsurround_view</code>, <code class="source">unsurround_inplace</code> 同时去掉指定字符串前后满足特定模式的子串
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被处理的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">left, right</td><td class="td-param-sep">-</td><td class="td-param-desc"> 表示在字符串首尾的需要添加或者去掉的子串
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
反转：字符串逆序 <a id="invert"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto invert(std::string_view s, size_type pos = 0, size_type max_n = npos) -&gt; std::string;
    static auto invert_inplace(std::string&amp; s, size_type pos = 0, size_type max_n = npos) -&gt; std::string&amp;;
</pre>
<p>
将 <code class="source">s</code> 中指定范围内的子串的前后字符串的逐个字符交换位置。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被反转的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">pos</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定反转的起始位置
</td></tr>
<tr class="tr-param"><td class="td-param-names">max_n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定从 pos 位置开始最多反转多少字符
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回颠倒位置后的字符串
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
重复序列生成 <a id="repeat"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto repeat(std::string_view s, size_type times) -&gt; std::string;
    static auto repeat(value_type ch, size_type times) -&gt; std::string;
</pre>
<p>
生成字 <code class="source">s</code> 或者 <code class="source">ch</code> 的内容重复出现 <code class="source">times</code> 次后的新字符串。如果是生成空白字符的重复序列，
可考虑用 <a href="#spaces">spaces</a>。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">ch, s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定重复的字符或者字符串模板。
</td></tr>
<tr class="tr-param"><td class="td-param-names">times</td><td class="td-param-sep">-</td><td class="td-param-desc"> 重复次数。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
空白串生成 <a id="spaces"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto spaces(uint8_t width) -&gt; std::string_view;
    static auto make_spaces(size_type width) -&gt; std::string;
    static auto make_spaces_inplace(std::string&amp; s, size_type width) -&gt; std::string&amp;;
</pre>
<ul>
<li>
<code class="source">spaces</code> 用于生成指定宽度 <code class="source">width</code> 的空白字符序列。
</li>
<li>
<code class="source">make_spaces</code>，<code class="source">make_spaces_inplace</code> 只要内存允许，支持生成任意长度的空白串。
</li>
</ul>
<p>
@notice{1} <code class="source">spaces</code>，<code class="source">make_spaces</code>，<code class="source">make_spaces_inplace</code> 这三个函数的功能类似。
但主要差别 是 <code class="source">space</code> 利用的是预生成的字符串常量，所以速度会更快，但最大长度有限（<code class="source">UNIT8_MAX</code>），
大部分情况下 <code class="source">space</code> 函数都是够用的。当需要生成超过 <code class="source">space</code> 支持空白串时，使用 <code class="source">make_spaces</code>
或者 <code class="source">make_spaces_inplace</code> 是更合适的选择。但显然 <code class="source">make_xxx</code> 系列函数更慢。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 在 make_spaces_inplace 中表示用于接收数据的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">witdh</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定待生成的空白串的宽度，这里的空白串采用 ASCII 码为 0x20 的字符填充
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return"><code class="source">spaces</code> 函数返回内部预生成的空白串的视图；<code class="source">make_space</code> 为返回新生成的字符串（内存重新分配）；
</p>
<p>
而 <code class="source">make_spaces_inplace</code> 函数会将生成的空白串直接原位替换原始输入字符串。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
字符串遮罩 <a id="cover"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto cover(std::string_view s, std::string_view mask = &quot;***&quot;, size_type left_n = 1, size_type right_n = 3) -&gt; std::string;
    static auto cover_inplace(std::string&amp; s, std::string_view mask = &quot;***&quot;, size_type left_n = 1, size_type right_n = 3) -&gt; std::string&amp;;
</pre>
<p>
使用指定的掩码字符串 mask 对原始字符串中的关键文字替换，以遮蔽原始字符串中的敏感信息。常用于
敏感信息脱敏，长串简略缩短等场景。
</p>
<p>
<code class="source">cover</code> 和 <code class="source">cover_inplace</code> 对 <code class="source">s</code> 的中间部分使用遮罩字符串 <code class="source">mask</code> 替换，其中 <code class="source">left_n</code> 和 <code class="source">right_n</code> 用于
控住遮罩之后，首尾各自保留多少长度的明文子串。
</p>
<p>
@notice{1} 当 <code class="source">left_n</code> 或者 <code class="source">right_n</code> 或者 <code class="source">left_n + right_n</code> 大于或者等于原始串 s 的长度时，本函数将失去
遮罩作用。对于这种情况 <code class="source">cover</code> 和 <code class="source">cover_inplace</code> 函数将返回空串，以避免无意间的信息泄露。因此，
在使用本函数之前，最好确保字符串 s 的长度达到本算法的最小值 <code class="source">left_n + right_n</code>。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被遮罩的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">mask</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于遮罩的字符串，特别的，mask 为空也是允许的，但效果等价于去掉被遮罩的字符。
</td></tr>
<tr class="tr-param"><td class="td-param-names">left_n, right_n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于指定字符串 <code class="source">s</code> 的首尾两端分别保留多长的明文串
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回遮罩后的字符串
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
字符串拼接 <a id="join"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto join(std::string_view s, const view_provider_proc&amp; proc) -&gt; std::string;
    template &lt;typename Sequence = std::initializer_list&lt;std::string_view&gt;, typename = typename Sequence::const_iterator&gt;
    static auto join(std::string_view s, const Sequence&amp; items) -&gt; std::string;
</pre>
<p>
以 <code class="source">s</code> 作为分隔符，拼接多个字符串。输入串可以通过 <code class="source">proc</code> 或者 <code class="source">items</code> 供给。如果 <code class="source">proc</code> 无法提供任何字符串
或者 <code class="source">items</code> 为空，返回空串。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 分隔符。
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于通过回调函数的方式供给被拼接的字符串。
</td></tr>
<tr class="tr-param"><td class="td-param-names">items</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被字符串容器序列。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回拼接后的字符串。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
拼接列表 <a id="join_list"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto join_list(const view_provider_proc&amp; proc) -&gt; std::string;
    template &lt;typename Sequence = std::initializer_list&lt;std::string_view&gt;, typename = typename Sequence::const_iterator&gt;
    static auto join_list(const Sequence&amp; items) -&gt; std::string;
</pre>
<p>
使用逗号作为分隔符，拼接多个子串。输入串可以通过 proc 或者 items 供给。如果 proc 无法提供任何字符串或者 items 为空，返回空串。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于通过回调函数的方式供给被拼接的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">items</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被字符串容器序列
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回合并后的字符串
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
映射拼接 <a id="join_map"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto join_map(std::string_view sep_pair, std::string_view sep_list, const view_pair_provider_proc&amp; proc) -&gt; std::string;
    static auto join_map(const view_pair_provider_proc&amp; proc) -&gt; std::string;
    template &lt;typename Map, typename = typename Map::const_iterator&gt;
    static auto join_map(std::string_view sep_pair, std::string_view sep_list, const Map&amp; items) -&gt; std::string;
    template &lt;typename Map, typename = typename Map::const_iterator&gt;
    static auto join_map(const Map&amp; items) -&gt; std::string;
</pre>
<p>
使用 <code class="source">sep_pair</code> 拼接每个 key-value 对的 key 和 value 部分，并使用 <code class="source">sep_list</code> 拼接多个 key-value 对。
该函数拼接的结果接近 json 的字典的内部结构（没有外围的花括号）。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">sep_pair</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于拼接每个 key-value 对，当未指定该参数或者为空时，自动采用 <code class="source">&quot;:&quot;</code>
</td></tr>
<tr class="tr-param"><td class="td-param-names">sep_list</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于拼接多个拼接好的 key-value 对，当未指定该参数或者为空时，自动采用 <code class="source">&quot;,&quot;</code>
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于供给 key-value 对， key-value 对由 <code class="source">std::tuple&lt;std::string_view, std::string_view&gt;</code> 来表示
</td></tr>
<tr class="tr-param"><td class="td-param-names">items</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于存储 key-value 对的容器
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回拼接后的字符串
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
按行拼接 <a id="join_lines"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto join_lines(std::string_view line_ends, const view_provider_proc&amp; proc) -&gt; std::string;
    static auto join_lines(const view_provider_proc&amp; proc) -&gt; std::string;
    template &lt;typename Sequence = std::initializer_list&lt;std::string_view&gt;, typename = typename Sequence::const_iterator&gt;
    static auto join_lines(std::string_view line_ends, const Sequence&amp; items) -&gt; std::string;
    template &lt;typename Sequence = std::initializer_list&lt;std::string_view&gt;, typename = typename Sequence::const_iterator&gt;
    static auto join_lines(const Sequence&amp; items) -&gt; std::string;
</pre>
<p>
将每个字符串视作一行，然后用换行符拼接成一个字符串，并返回。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">line_ends</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定行结束符，如果未指定，默认使用 <a href="#sep_line_ends">sep_line_ends</a> 作为分隔符
</td></tr>
<tr class="tr-param"><td class="td-param-names">items</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定存储行的容器
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 通过 proc 回调函数供给值，当 proc 返回 <code class="source">std::nullopt</code> 时，终止拼接过程
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
拼接路径 <a id="join_path"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto join_path(std::string_view sep, const view_provider_proc&amp; proc) -&gt; std::string;
    static auto join_path(const view_provider_proc&amp; proc) -&gt; std::string;
    template &lt;typename Sequence = std::initializer_list&lt;std::string&gt;, typename = typename Sequence::const_iterator&gt;
    static auto join_path(std::string_view sep, const Sequence&amp; items) -&gt; std::string;
    template &lt;typename Sequence = std::initializer_list&lt;std::string&gt;, typename = typename Sequence::const_iterator&gt;
    static auto join_path(const Sequence&amp; items) -&gt; std::string;
</pre>
<p>
使用指定的分隔符 <code class="source">sep</code> 或者系统默认的路径分隔符，将不同来源的路径片段拼接成完整的文件路径。
</p>
<p>
@notice{1} 需要注意：<a href="#join_path">join_path</a> 会主动去除路径片段间的路径分隔符 &apos;/&apos;，但只会去除一次。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">sep</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定的路径分隔符，如果不带该参数，默认使用 <a href="#sep_path">sep_path</a>
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 通过回调函数指定路径片段，当 proc 返回 <code class="source">std::nullopt</code> 时，拼接过程终止。
</td></tr>
<tr class="tr-param"><td class="td-param-names">items</td><td class="td-param-sep">-</td><td class="td-param-desc"> 存储路径片段的容器
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回拼接后的路径
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
拼接搜索路径 <a id="join_searchpath"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto join_searchpath(std::string_view sep, const view_provider_proc&amp; proc) -&gt; std::string;
    static auto join_searchpath(const view_provider_proc&amp; proc) -&gt; std::string;
    template &lt;typename Sequence = std::initializer_list&lt;std::string&gt;, typename = typename Sequence::const_iterator&gt;
    static auto join_searchpath(std::string_view sep, const Sequence&amp; items) -&gt; std::string;
    template &lt;typename Sequence = std::initializer_list&lt;std::string&gt;, typename = typename Sequence::const_iterator&gt;
    static auto join_searchpath(const Sequence&amp; items) -&gt; std::string;
</pre>
<p>
使用搜索路径分隔符拼接由 <code class="source">proc</code> 或者 <code class="source">items</code> 供给的字符串，并返回拼接后的结果。
路径分隔符可以通过 <code class="source">sep</code> 参数指定，当调用没有该参数的形式的函数时，自动使用系统默认的分隔
符（参见 <a href="#sep_searchpath">sep_searchpath</a>）。对于提供 <code class="source">proc</code> 参数的接口，<code class="source">proc</code> 会持续调用该哈数获得数据直到该函数
返回 <code class="source">std::nullopt</code>。如果 <code class="source">proc</code> 在第一次调用时就返回 <code class="source">std::nullopt</code>，返回的搜索路径为空串。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">sep</td><td class="td-param-sep">-</td><td class="td-param-desc"> 搜索路径分隔符。对于未指定该参数的函数，自动采用 <a href="#sep_searchpath">sep_searchpath</a> 作为分隔符。
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 提供搜素路径片段的函数。
</td></tr>
<tr class="tr-param"><td class="td-param-names">items</td><td class="td-param-sep">-</td><td class="td-param-desc"> 存放路径片段的容器。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回以当前系统的搜索路径分隔符拼接好的字符串。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
通用字符串拆分 <a id="split"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto split(std::string_view s, const substr_search_proc&amp; search_proc, size_type max_n, const range_consumer_proc&amp; proc) -&gt; void;
    static auto split(std::string_view s, const charset_type&amp; sep_charset, size_type max_n, const view_consumer_proc&amp; proc) -&gt; void;
    static auto split(std::string_view s, const charset_type&amp; sep_charset, const view_consumer_proc&amp; proc) -&gt; void;
    static auto split(std::string_view s, const charset_type&amp; sep_charset, size_type max_n = npos) -&gt; std::vector&lt;std::string&gt;;
    static auto split(std::string_view s, std::string_view sep_str, size_type max_n, const view_consumer_proc&amp; proc) -&gt; void;
    static auto split(std::string_view s, std::string_view sep_str, const view_consumer_proc&amp; proc) -&gt; void;
    static auto split(std::string_view s, std::string_view sep_str, size_type max_n = npos) -&gt; std::vector&lt;std::string&gt;;
    static auto split(std::string_view s, size_type max_n, const view_consumer_proc&amp; proc) -&gt; void;
    static auto split(std::string_view s, const view_consumer_proc&amp; proc) -&gt; void;
    static auto split(std::string_view s, size_type max_n = npos) -&gt; std::vector&lt;std::string&gt;;
    static auto split_view(std::string_view s, const charset_type&amp; sep_charset, size_type max_n = npos) -&gt; std::vector&lt;std::string_view&gt;;
    static auto split_view(std::string_view s, std::string_view sep_str, size_type max_n = npos) -&gt; std::vector&lt;std::string_view&gt;;
    static auto split_view(std::string_view s, size_type max_n = npos) -&gt; std::vector&lt;std::string_view&gt;;
</pre>
<p>
将字符串 <code class="source">s</code> 以指定的分隔符（比如，字符集 <code class="source">sep_charset</code>、字符串 <code class="source">sep_str</code>、某抽象函数 <code class="source">search_proc</code>）拆分为
多个子串。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被拆分的字符串。
</td></tr>
<tr class="tr-param"><td class="td-param-names">search_proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 更抽象的分隔符定位方法，需要返回找到的分隔符的范围，或者返回 <code class="source">std::nullopt</code> 表示找不到分隔符。
</td></tr>
<tr class="tr-param"><td class="td-param-names">sep_charset</td><td class="td-param-sep">-</td><td class="td-param-desc"> 分隔符集合，可以有多种形式组成，集合中的每个字符都是分隔符。
</td></tr>
<tr class="tr-param"><td class="td-param-names">sep_str</td><td class="td-param-sep">-</td><td class="td-param-desc"> 以字符串为分隔符，如果该参数指定为空串，自动以连续的空白符为分隔符。
</td></tr>
<tr class="tr-param"><td class="td-param-names">max_n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 最多拆分多少次。如果为 0 表示不做任何拆分，返回原始字符串。如果为 npos 表示不限制拆分次数。需要注意，当字符串
</td></tr>
</tbody></table>
<p>
<code class="source">s</code> 中实际的分隔符的数量大于 <code class="source">max_n</code> 时，会返回 <code class="source">max_n + 1</code> 个子串。
</p>
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定如何接受拆分出来的字符串。需要注意，不同函数 proc 的原型是不同的。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">当未指定 proc 参数时，会返回字符串列表。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
按逗号拆分 <a id="split_list"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto split_list(std::string_view s, size_type max_n, const view_consumer_proc&amp; proc) -&gt; void;
    static auto split_list(std::string_view s, const view_consumer_proc&amp; proc) -&gt; void;
    static auto split_list(std::string_view s, size_type max_n = npos) -&gt; std::vector&lt;std::string&gt;;
    static auto split_list_view(std::string_view s, size_type max_n = npos) -&gt; std::vector&lt;std::string_view&gt;;
</pre>
<p>
以逗号 <code class="source">,</code> 为分隔符，将字符串 <code class="source">s</code> 拆分成多个子串。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被拆分的字符串。
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定如何接受拆分出来的字符串。
</td></tr>
<tr class="tr-param"><td class="td-param-names">max_n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 最多拆分多少次。如果为 0 表示不做任何拆分，将返回原始字符串。如果为 <code class="source">npos</code> 表示不限制拆分次数。
</td></tr>
</tbody></table>
<p>
需要注意，当字符串 <code class="source">s</code> 中实际的分隔符的数量大于或者等于 <code class="source">max_n</code> 时，会返回 <code class="source">max_n + 1</code> 个子串。
</p>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">当未指定 <code class="source">proc</code> 参数时，会返回字符串列表。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
拆分 key-value 对 <a id="split_pair"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto split_pair(std::string_view s, std::string_view sep = &quot;:&quot;) -&gt; pair&lt;std::string&gt;;
    static auto split_pair_view(std::string_view s, std::string_view sep = &quot;:&quot;) -&gt; pair&lt;std::string_view&gt;;
</pre>
<ul>
<li>
<a href="#split_pair">split_pair</a> 等价于 <a href="#split">split</a> 系列函数，指定 <code class="source">max_n</code> 参数为 <code class="source">1</code> 时的功能。
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被拆分的字符串。
</td></tr>
<tr class="tr-param"><td class="td-param-names">sep</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用作分隔符的字符串，当 <code class="source">sep</code> 被指定为空串时，自动以 <code class="source">&quot;:&quot;</code> 为分隔符。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回被拆分处理的字符串。如果字符串中未找到分隔符，整个字符串作为返回值的第一个字符串，而第二个字符串为空。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
拆分多个 key-value 对 <a id="split_map"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto split_map(std::string_view s, std::string_view sep_list, std::string_view sep_pair, const view_pair_consumer_proc&amp; proc) -&gt; void;
    static auto split_map(std::string_view s, std::string_view sep_list = &quot;,&quot;, std::string_view sep_pair = &quot;:&quot;, size_type max_n = npos) -&gt; std::map&lt;std::string, std::string&gt;;
</pre>
<p>
<code class="source">split_map</code> 会对字符串做两轮拆分：
</p>
<p>
第一轮先以 <code class="source">sep_list</code> 为分隔符，将字符串拆分成一组字串；
</p>
<p>
第二轮再以 <code class="source">sep_pair</code> 为分隔符将前一轮拆分出来的每个字串拆分成键值对，并将该该键值对存入 map 对象返回，
或者通过 proc 输出。
</p>
<p>
总之，<code class="source">split_map</code> 是拆分的是类型下面的数据格式的算法（以 <code class="source">sep_list</code> 和 <code class="source">sep_pair</code> 为缺省值时为例）：
</p>
<pre  class="source">
  item1:value1,item2:value2,item3:value3 ...</pre>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被拆分的字符串。
</td></tr>
<tr class="tr-param"><td class="td-param-names">sep_list</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用作第一轮拆分的分隔符。
</td></tr>
<tr class="tr-param"><td class="td-param-names">sep_pair</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用作第二轮拆分的分隔符。
</td></tr>
<tr class="tr-param"><td class="td-param-names">max_n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 最多拆分多少次。<code class="source">max_n</code> 主要用于控制第一轮拆分的次数，如果指定为 0 将返回空 map 或者不
</td></tr>
</tbody></table>
<p>
触发 proc。当次数达到后，后续的数据会被舍弃，且不会被放入 map 中，也不会再触发 proc。由于调用方无法感
知是否有剩余数据未拆分完，因此，<code class="source">max_n</code> 通常只用在舍弃剩余字符串是无关紧要的情况下使用。
</p>
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 输出拆分出来的每个键值对。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回组合成的 map，对于返回值为 void 的形式，数据通过 proc 返回。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
按行拆分 <a id="split_lines"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto split_lines(std::string_view s, bool keep_ends, const view_consumer_proc&amp; proc) -&gt; void;
    static auto split_lines(std::string_view s, bool keep_ends = false) -&gt; std::vector&lt;std::string&gt;;
    static auto split_lines_view(std::string_view s, bool keep_ends = false) -&gt; std::vector&lt;std::string_view&gt;;
</pre>
<p>
以行结束符为分隔符，将字符串 <code class="source">s</code> 拆分成多行。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 待拆分字符串。
</td></tr>
<tr class="tr-param"><td class="td-param-names">keep_ends</td><td class="td-param-sep">-</td><td class="td-param-desc"> 是否保留行尾的分隔符。
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于接收拆分出的子串。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">通过 proc 接收拆分后的行或者返回存储拆分后的所有行的容器。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
路径拆分 <a id="split_path"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto split_path(std::string_view s, const view_consumer_proc&amp; proc) -&gt; void;
    static auto split_path(std::string_view s) -&gt; std::vector&lt;std::string&gt;;
    static auto split_path_view(std::string_view s) -&gt; std::vector&lt;std::string_view&gt;;
</pre>
<p>
将字符串 <code class="source">s</code> 视作文件或者目录的路径，按照路径分隔符 <a href="#sep_path_char">sep_path_char</a>，拆分成多个组成部分。
</p>
<p>
@notice{1} 本函数并不支持 windows 下同时可以使用 <code class="source">&apos;/&apos;</code> 和 <code class="source">&apos;\\&apos;</code> 为分隔符的场景。如果有必要，可以使用
更加强大的 <a href="#split">split</a> 函数代替。
</p>
<p>
@notice{2} 对于以 &apos;/&apos; 字符开头的绝对路径，拆分出来的第一个字符串，总是 &apos;/&apos;。
</p>
<p>
@notice{3} 对于重复的路径分隔符（比如，&quot;ab&quot; 中的 &quot;&quot;），会自动视作一个路径分隔符。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 待拆分的路径。
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于接收被拆分出来的字符串。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">当返回值类型为 <code class="source">void</code> 时，可以通过 proc 函数接收输出数据；否则，返回值表示拆分出来的多个子串。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
拆分搜索路径 <a id="split_searchpath"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto split_searchpath(std::string_view s, bool keep_empty, value_type sep, const view_consumer_proc&amp; proc) -&gt; void;
    static auto split_searchpath(std::string_view s, bool keep_empty = false, value_type sep = &apos;:&apos;) -&gt; std::vector&lt;std::string&gt;;
    static auto split_searchpath_view(std::string_view s, bool keep_empty = false, value_type sep = &apos;:&apos;) -&gt; std::vector&lt;std::string_view&gt;;
</pre>
<p>
将字符串 <code class="source">s</code> 视作搜索目录（可以以 Linux 系统下的 <code class="source">$PATH</code> 环境变量为参考），按照搜索路径分隔
符 <a href="#sep_searchpath_char">sep_searchpath_char</a> 将 <code class="source">s</code> 拆分成多个路径。
</p>
<p>
@notice{1} 需要注意 POSIX 系统标准并不允许文件路径中存在冒号的情况，
参考: https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap08.html#tag_08_03。
</p>
<p>
@notice{2} 需要注意本函数默认支持的是 <code class="source">*nix</code> 系统的默认的搜索路径分隔符，对于 Windows 系统，可以
指定 <code class="source">sep</code> 参数为 <code class="source">&quot;;&quot;</code>。
</p>
<p>
@notice{3} 本系列函数并不会主动去除路径前后的空白，也对于重复路径也不去重。当 <code class="source">keep_empty</code> 指定
为 <code class="source">true</code> 时，空串会被传递给 proc 或者返回。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 待拆分的路径
</td></tr>
<tr class="tr-param"><td class="td-param-names">keep_empty</td><td class="td-param-sep">-</td><td class="td-param-desc"> 是否保留空路径（注意：POSIX 中空搜索路径通常表示当前工作路径，是有意义的）
</td></tr>
<tr class="tr-param"><td class="td-param-names">sep</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定搜索路径分隔符。对于 Windows 下的搜索路径，需要额外指定该参数。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">当返回值类型为 <code class="source">void</code> 时，可以通过 proc 函数接收输出数据；否则，返回值表示拆分出来的多个子串。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
分片 <a id="partition"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto partition_range(std::string_view s, const charset_type&amp; charset) -&gt; ternary&lt;range_type&gt;;
    static auto partition_range(std::string_view s, const char_match_proc&amp; proc) -&gt; ternary&lt;range_type&gt;;
    static auto partition_range(std::string_view s, std::string_view sep) -&gt; ternary&lt;range_type&gt;;
    /// -
    static auto partition_view(std::string_view s, const charset_type&amp; charset) -&gt; ternary&lt;std::string_view&gt;;
    static auto partition_view(std::string_view s, const char_match_proc&amp; proc) -&gt; ternary&lt;std::string_view&gt;;
    static auto partition_view(std::string_view s, std::string_view sep) -&gt; ternary&lt;std::string_view&gt;;
    static auto partition_view(std::string_view s, const view_search_proc&amp; proc) -&gt; ternary&lt;std::string_view&gt;;
    /// -
    static auto partition(std::string_view s, const charset_type&amp; charset) -&gt; ternary&lt;std::string&gt;;
    static auto partition(std::string_view s, const char_match_proc&amp; proc) -&gt; ternary&lt;std::string&gt;;
    static auto partition(std::string_view s, std::string_view sep) -&gt; ternary&lt;std::string&gt;;
    static auto partition(std::string_view s, const view_search_proc&amp; proc) -&gt; ternary&lt;std::string&gt;;
</pre>
<p>
<code class="source">partition_range</code>, <code class="source">partition_view</code>, <code class="source">partition</code> 从 s 左侧查找首个 sep 分隔符或者满足 proc 的字符的位置，
并将字符串分割为分隔符左侧，分隔符，分隔符右侧三个部分，并返回。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 输入字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">charset, proc, sep</td><td class="td-param-sep">-</td><td class="td-param-desc"> 不同形式的分隔符。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回依次由分隔符左侧的子串，分隔符自身，分隔符右侧子串组成的三元组（ternary）。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
字符串分块 <a id="next_chunk"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto next_chunk_range(std::string_view s, size_type&amp; pos, size_type max_n) -&gt; std::optional&lt;range_type&gt;;
    static auto next_chunk_view(std::string_view s, size_type&amp; pos, size_type max_n) -&gt; std::optional&lt;std::string_view&gt;;
    static auto next_chunk(std::string_view s, size_type&amp; pos, size_type max_n) -&gt; std::optional&lt;std::string&gt;;
</pre>
<p>
从字符串 <code class="source">s</code> 的 <code class="source">pos</code> 位置开始，最多提取 <code class="source">max_n</code> 长度的子串。本函数支持迭代。本函数可多次调用，
实现迭代式提取子串。当 <code class="source">pos</code> 大于或者等于 <code class="source">s</code> 的长度时，返回 <code class="source">std::nullopt</code>。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc">
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
指定宽度拆分字符串 <a id="chunked"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto chunked(std::string_view s, size_type width, const view_consumer_proc&amp; proc) -&gt; void;
    static auto chunked(std::string_view s, size_type width) -&gt; std::vector&lt;std::string&gt;;
    static auto chunked_view(std::string_view s, size_type width) -&gt; std::vector&lt;std::string_view&gt;;
</pre>
<p>
将字符串 s 拆分成宽度为 width 的多个子串
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 将被拆分的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">width</td><td class="td-param-sep">-</td><td class="td-param-desc"> 执行拆分宽度，如果 width 为 0 自动校正为 1；当 width 大于 s 的长度时，等价于不拆分
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 数据输出函数
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">通过 proc 或者返回值返回拆分后的子串列表
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
字符串分块 <a id="next_window"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto next_window_range(std::string_view s, size_type&amp; pos, size_type max_n, size_type step) -&gt; std::optional&lt;range_type&gt;;
    static auto next_window_view(std::string_view s, size_type&amp; pos, size_type max_n, size_type step) -&gt; std::optional&lt;std::string_view&gt;;
    static auto next_window(std::string_view s, size_type&amp; pos, size_type max_n, size_type step) -&gt; std::optional&lt;std::string&gt;;
</pre>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
基于窗口拆分字符串 <a id="windowed"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto windowed(std::string_view s, size_type width, size_type step, const view_consumer_proc&amp; proc) -&gt; void;
    static auto windowed(std::string_view s, size_type width, size_type step) -&gt; std::vector&lt;std::string&gt;;
    static auto windowed_view(std::string_view s, size_type width, size_type step) -&gt; std::vector&lt;std::string_view&gt;;
</pre>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
大小写转换 <a id="case_conversion"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto to_lower(std::string_view s) -&gt; std::string;
    static auto to_upper(std::string_view s) -&gt; std::string;
    static auto to_title(std::string_view s) -&gt; std::string;
    static auto to_capitalize(std::string_view s) -&gt; std::string;
    static auto swap_case(std::string_view s) -&gt; std::string;
    //
    static auto to_lower_inplace(std::string&amp; s) -&gt; std::string&amp;;
    static auto to_upper_inplace(std::string&amp; s) -&gt; std::string&amp;;
    static auto to_title_inplace(std::string&amp; s) -&gt; std::string&amp;;
    static auto to_capitalize_inplace(std::string&amp; s) -&gt; std::string&amp;;
    static auto swap_case_inplace(std::string&amp; s) -&gt; std::string&amp;;
</pre>
<ul>
<li>
<code class="source">to_lower</code>, <code class="source">to_lower_inplace</code> 将 s 中的所有大写字母转换为小写字母
</li>
<li>
<code class="source">to_upper</code>, <code class="source">to_upper_inplace</code> 将 s 中的所有小写字母转换为大写字母
</li>
<li>
<code class="source">to_title</code>, <code class="source">to_title_inplace</code> 将 s 每个单词的首字母转换为大写形式
</li>
<li>
<code class="source">to_capitalize</code>, <code class="source">to_capitalize_inplace</code> 将 s 首字母转换为大写形式
</li>
<li>
<code class="source">swap_case</code>, <code class="source">swap_case_inplace</code> 将 s 中的所有大写字母转换为小写字母，同时将消息字母转换为大写字母
</li>
</ul>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
剔除 <a id="trim"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
</pre>
<ul>
<li>
<code class="source">trim_left</code>, <code class="source">trim_left_inplace</code> 去掉 <code class="source">s</code> 左侧的满足条件的字符
</li>
<li>
<code class="source">trim_right_view</code>, <code class="source">trim_right</code>, <code class="source">trim_right_inplace</code> 去掉 <code class="source">s</code> 右侧的满足条件的字符
</li>
<li>
<code class="source">trim_surrounding_view</code>, <code class="source">trim_surrounding</code>, <code class="source">trim_surrounding_inplace</code> 去掉 <code class="source">s</code> 两端的满足条件的字符
</li>
<li>
<code class="source">trim_anywhere</code>, <code class="source">trim_anywhere_inplace</code> 去掉 <code class="source">s</code> 中任何满足条件的满足条件的字符
</li>
</ul>
<p>
@notice{1} 对于未指定 <code class="source">proc</code> 或者 <code class="source">charset</code> 等定制条件的回到函数，表示去除空白字符。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 从该字符串剔除自定的字符或者子串。
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 字符匹配条件，所有满足条件的字符都将剔除。
</td></tr>
<tr class="tr-param"><td class="td-param-names">charset</td><td class="td-param-sep">-</td><td class="td-param-desc"> 表示可以满足条件的字符集，<code class="source">s</code> 中的任何字符集中的字符都将被剔除。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回剔除指定字符后的剩余部分。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
化简 <a id="simplified"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto simplified(std::string_view s, std::string_view sep, const char_match_proc&amp; proc) -&gt; std::string;
    static auto simplified(std::string_view s) -&gt; std::string;
    static auto simplified_inplace(std::string&amp; s, std::string_view sep, const char_match_proc&amp; proc) -&gt; std::string&amp;;
    static auto simplified_inplace(std::string&amp; s) -&gt; std::string&amp;;
    //
    static auto simplified_integer(std::string_view s) -&gt; std::string;
    static auto simplified_integer_inplace(std::string&amp; s) -&gt; std::string&amp;;
</pre>
<ul>
<li>
<code class="source">simplified</code>, <code class="source">simplified_inplace</code> 将 s 中连续匹配 proc 条件的字符替换为 sep，对于不带 fill 和 proc 形式
<p>
  的函数，表示将所有的空白字符替换成单个空格字符（0x20）。
</p>
</li>
<li>
<code class="source">simplified_integer</code>, <code class="source">simplified_integer_inplace</code> 将字符串 s 视作整数（integer）在不影响其值的前提
<p>
  下，清除多余的前缀正号和前缀 0。
</p>
</li>
<li>
<code class="source">simplified_decimal</code>, <code class="source">simplified_decimal_inplace</code> 将字符串 s 视作小数（decimal）在不影响其值的前提
<p>
  下，清除多余的前缀正号、整数部分的前缀0、小数部分尾部的多余的0。如果 s 是指数形式，指数中的多余 0 以及指数形式本身
  也会被纳入化简考虑。
</p>
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 将被化简的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">fill</td><td class="td-param-sep">-</td><td class="td-param-desc"> 如果可以化简，那么化简后改用什么填充
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 连续字符匹配条件
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回化简后的字符串
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
展开 <a id="expand_envs"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto expand_envs(std::string_view s, bool keep_unexpanded, const string_mapping_proc&amp; proc) -&gt; std::string;
    static auto expand_envs(std::string_view s, bool keep_unexpanded = false) -&gt; std::string;
    static auto expand_envs(std::string_view s, bool keep_unexpanded, const std::map&lt;std::string, std::string&gt;&amp; kvs) -&gt; std::string;
    static auto expand_envs(std::string_view s, const std::map&lt;std::string, std::string&gt;&amp; kvs) -&gt; std::string;
    static auto expand_envs(std::string_view s, std::string_view key, std::string_view val) -&gt; std::string;
    //
    static auto expand_envs_inplace(std::string&amp; s, bool keep_unexpanded, const string_mapping_proc&amp; proc) -&gt; std::string&amp;;
    static auto expand_envs_inplace(std::string&amp; s, bool keep_unexpanded = false) -&gt; std::string&amp;;
    static auto expand_envs_inplace(std::string&amp; s, bool keep_unexpanded, const std::map&lt;std::string, std::string&gt;&amp; kvs) -&gt; std::string&amp;;
    static auto expand_envs_inplace(std::string&amp; s, const std::map&lt;std::string, std::string&gt;&amp; kvs) -&gt; std::string&amp;;
    static auto expand_envs_inplace(std::string&amp; s, std::string_view key, std::string_view val) -&gt; std::string&amp;;
</pre>
<p>
将字符串 <code class="source">s</code> 中 <code class="source">$xxx</code> 和 <code class="source">${xxx}</code> 形式的子串看做环境变量的占位子串，将其中的 <code class="source">xxx</code> 视作环境变量的名字，
将整个占位子串替换为环境变量的值。本系列函数提供了多种方式获取环境变量值的方式。其中，如果没带任何获取环
境变量值的方式的话，将自动通过 <code class="source">getenv</code> 函数，从当前系统环境变量表中获取。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 模板字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">keep_unexpanded</td><td class="td-param-sep">-</td><td class="td-param-desc"> 当无法通过 <code class="source">key</code> 确定被替换的值时，由 <code class="source">keep_unexpanded</code> 参数决定是保留原样。如果
</td></tr>
</tbody></table>
<p>
不原样保留会直接去除占位子串。
</p>
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 通过 <code class="source">proc</code> 查询环境变量的值。
</td></tr>
<tr class="tr-param"><td class="td-param-names">kvs</td><td class="td-param-sep">-</td><td class="td-param-desc"> 直接从一个指定的 map 中检索环境变量的值。
</td></tr>
<tr class="tr-param"><td class="td-param-names">key, val</td><td class="td-param-sep">-</td><td class="td-param-desc"> 只将 <code class="source">key</code> 替换为 <code class="source">val</code>，其他的原样保留。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回替换后的新串。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
tab 扩展 <a id="expand_tabs"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto expand_tabs(std::string_view s, size_type tab_size = 8) -&gt; std::string;
    static auto expand_tabs_inplace(std::string&amp; s, size_type tab_size = 8) -&gt; std::string&amp;;
</pre>
<p>
将字符串中的 tab 符号（<code class="source">\t</code>）按照 <code class="source">tab_size</code> 宽度替换成空格（对 应ASCII 码 0x20）。每个 tab 实际被替换成
的空格的数量，并不是固定的，而是取决于 tab 字符在字符串 s 中的位置。该函数模拟了在编辑器中按 tab 键时的视
觉效果。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 将被扩展的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">tab_size</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定一个 tab 应该等价于多少个空格字符（0x20）。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回扩展后的字符串
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
HOME 字符扩展 <a id="expand_user"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto expand_user(std::string_view s) -&gt; std::string;
    static auto expand_user_inplace(std::string&amp; s) -&gt; std::string&amp;;
</pre>
<p>
扩展字符串中的 <code class="source">~</code> 前缀为 <code class="source">${HOME}</code> 的值，该函数模拟了 Shell 的行为。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 将被扩展的字符串
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回扩展后的字符串
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
路径正规化 <a id="normpath"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto normpath(std::string_view s) -&gt; std::string;
    static auto normpath_inplace(std::string&amp; s) -&gt; std::string&amp;;
</pre>
<p>
将输入字符串 <code class="source">s</code> 视作文件或者目录的路径，消除路径中的相对路径（<code class="source">.</code> 和 <code class="source">..</code>）。
</p>
<p>
@notice{1} 需要注意，本算法（函数）只是做字符串的操作，并不会访问文件系统。但这也意味着，在部分场景下
并不能完全去掉相对路径。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被视作路径的字符串。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回正规化之后的路径。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
路径的基本部分的长度 <a id="basename_pos"></a><a id="extname_pos"></a><a id="dirname_pos"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto basename_pos(std::string_view s) -&gt; size_type;
    static auto dirname_pos(std::string_view s) -&gt; size_type;
    static auto extname_pos(std::string_view s) -&gt; size_type;
</pre>
<ul>
<li>
<code class="source">basename_pos</code> 返回路径 <code class="source">s</code> 中的文件的基本名（basename）的长度。
</li>
<li>
<code class="source">dirname_pos</code> 返回路径 <code class="source">s</code> 中的文件的目录部分（dirname）的长度，如果 <code class="source">s</code> 是目录，返回其父目录。
</li>
<li>
<code class="source">extname_pos</code> 返回路径 <code class="source">s</code> 中的文件的的基本名中的扩展名部分（extname）。
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被视作路径的字符串
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回路径几个基本部分的路径的长度
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
路径处理函数：目录 <a id="dirname"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto dirname_range(std::string_view s) -&gt; range_type;
    static auto dirname_view(std::string_view s) -&gt; std::string_view;
    static auto dirname(std::string_view s) -&gt; std::string;
    static auto remove_dirname_view(std::string_view s) -&gt; std::string_view;
    static auto remove_dirname(std::string_view s) -&gt; std::string;
    static auto replace_dirname(std::string_view s, std::string_view new_dir) -&gt; std::string;
    static auto split_dirname_view(std::string_view s) -&gt; pair&lt;std::string_view&gt;;
    static auto split_dirname(std::string_view s) -&gt; pair&lt;std::string&gt;;
    //
    static auto dirname_inplace(std::string&amp; s) -&gt; std::string&amp;;
</pre>
<ul>
<li>
<code class="source">dirname_view</code>, <code class="source">dirname</code>, <code class="source">dirname_inplace</code> 获取路径 <code class="source">s</code> 的目录部分
</li>
<li>
<code class="source">remove_dirname_view</code>, <code class="source">remove_dirname</code>, <code class="source">remove_dirname_inplace</code> 删除路径 <code class="source">s</code> 的目录部分，返回剩余部分
</li>
<li>
<code class="source">replace_dirname</code>, <code class="source">replace_dirname_inplace</code> 使用 <code class="source">new_dir</code> 替换路径 <code class="source">s</code> 中的目录部分
</li>
<li>
<code class="source">split_diename</code> 将路径 <code class="source">s</code> 拆分为目录部分和剩余部分
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 路径字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">new_dir</td><td class="td-param-sep">-</td><td class="td-param-desc"> 表示被替换成的新名字，用于 replace_xxx 函数
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
路径处理函数：文件名 <a id="basename"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto basename_range(std::string_view s) -&gt; range_type;
    static auto basename_view(std::string_view s) -&gt; std::string_view;
    static auto basename(std::string_view s) -&gt; std::string;
    static auto remove_basename_view(std::string_view s) -&gt; std::string_view;
    static auto remove_basename(std::string_view s) -&gt; std::string;
    static auto replace_basename(std::string_view s, std::string_view new_name) -&gt; std::string;
    static auto split_basename_view(std::string_view s) -&gt; pair&lt;std::string_view&gt;;
    static auto split_basename(std::string_view s) -&gt; pair&lt;std::string&gt;;
    /// -
    static auto basename_inplace(std::string&amp; s) -&gt; std::string&amp;;
</pre>
<ul>
<li>
<code class="source">basename_view</code>, <code class="source">basename</code>, <code class="source">basename_inplace</code>: 获取路径 <code class="source">s</code> 的文件名部分
</li>
<li>
<code class="source">remove_basename_view</code>, <code class="source">remove_basename</code>, <code class="source">remove_basename_inplace</code>: 删除路径 <code class="source">s</code> 的文件名部分，返回剩余部分
</li>
<li>
<code class="source">replace_basename</code>, <code class="source">replace_basename_inplace</code>: 使用 <code class="source">new_name</code> 替换路径 <code class="source">s</code> 中的文件名
</li>
<li>
<code class="source">split_basename</code>: 将路径 <code class="source">s</code> 拆分为文件名和剩余部分
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 路径字符串。
</td></tr>
<tr class="tr-param"><td class="td-param-names">new_name</td><td class="td-param-sep">-</td><td class="td-param-desc"> 表示被替换成的新名字。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
路径处理函数：扩展名 <a id="extname"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto extname_range(std::string_view s) -&gt; range_type;
    static auto extname_view(std::string_view s) -&gt; std::string_view;
    static auto extname(std::string_view s) -&gt; std::string;
</pre>
<ul>
<li>
<code class="source">extname_view</code>, <code class="source">extname</code>, <code class="source">extname_inplace</code>: 获取路径 s 的扩展名部分。
</li>
<li>
<code class="source">remove_extname_view</code>, <code class="source">remove_extname</code>, <code class="source">remove_extname_inplace</code>: 删除路径 s 的扩展名，返回剩余部分。
</li>
<li>
<code class="source">replace_extname</code>, <code class="source">replace_extname_inplace</code>: 使用 new_name 替换路径 s 中的扩展名。
</li>
<li>
<code class="source">split_extname</code>: 将路径 s 拆分为扩展名和剩余部分。
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 路径字符串。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
路径处理函数：裸文件名 <a id="rawname"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto rawname_range(std::string_view s) -&gt; range_type;
    static auto rawname_view(std::string_view s) -&gt; std::string_view;
    static auto rawname(std::string_view s) -&gt; std::string;
</pre>
<p>
@notice{1} 这里的“裸文件名”是指不包含文件后缀名的部分，等价于 basename 中去掉 extname 之后的部分。
</p>
<ul>
<li>
<code class="source">rawname_view</code>, <code class="source">rawname</code>, <code class="source">rawname_inplace</code>: 获取路径 s 的中裸文件名部分
</li>
<li>
<code class="source">replace_rawname</code>, <code class="source">replace_rawname_inplace</code>: 使用 new_name 替换路径 s 中的裸文件名
</li>
<li>
<code class="source">split_rawname</code>: 将路径 s 拆分为目录、裸文件名，扩展名三个部分
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 路径字符串
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
转义：C语言字符串 <a id="encode_cstr"></a><a id="decode_cstr"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto encode_cstr(std::string_view s, const view_consumer_proc&amp; proc) -&gt; void;
    static auto encode_cstr(std::string_view s) -&gt; std::string;
    static auto decode_cstr(std::string_view s, const view_consumer_proc&amp; proc) -&gt; size_type;
    static auto decode_cstr(std::string_view s) -&gt; std::tuple&lt;size_type, std::string&gt;;
    /// -
    static auto encode_cstr_inplace(std::string&amp; s) -&gt; std::string;
    static auto decode_cstr_inplace(std::string&amp; s) -&gt; size_type;
</pre>
<p>
将 s 中属于 C 语言字符串中的特殊字符，转义为 C 语言字符串的转义表示形式，或者反过来。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被编码或者解码的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于接收转换后，生成的字符串
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回编码或者解码后的字符串
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
编解码：base64 <a id="encode_base64"></a><a id="decode_base64"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto encode_base64(std::string_view s, const view_consumer_proc&amp; proc) -&gt; void;
    static auto encode_base64(std::string_view s) -&gt; std::string;
    static auto decode_base64(std::string_view s, const view_consumer_proc&amp; proc) -&gt; void;
    static auto decode_base64(std::string_view s) -&gt; std::string;
    //
    static auto encode_base64_inplace(std::string&amp; s) -&gt; std::string&amp;;
    static auto decode_base64_inplace(std::string&amp; s) -&gt; std::string&amp;;
</pre>
<p>
将 s 中属于 url 的特殊字符，转义为 url 中可表示的形式，或者反过来
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被编码或者解码的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于接收转换后，生成的字符串
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回编码或者解码后的字符串
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
编解码：base16 <a id="encode_base16"></a><a id="decode_base16"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto encode_base16(std::string_view s, bool upper, const view_consumer_proc&amp; proc) -&gt; void;
    static auto encode_base16(std::string_view s, bool upper = false) -&gt; std::string;
    static auto decode_base16(std::string_view s, const view_consumer_proc&amp; proc) -&gt; void;
    static auto decode_base16(std::string_view s) -&gt; std::string;
    /// -
    static auto encode_base16_inplace(std::string&amp; s, bool upper = false) -&gt; std::string&amp;;
    static auto decode_base16_inplace(std::string&amp; s) -&gt; std::string&amp;;
</pre>
<p>
将 s 中属于 url 的特殊字符，转义为 url 中可表示的形式，或者反过来
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被编码或者解码的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于接收转换后，生成的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">upper</td><td class="td-param-sep">-</td><td class="td-param-desc"> 转换出来的十六进制是否采用大写形式
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回编码或者解码后的字符串
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
数据按十六进制 dump <a id="dump_hex"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto dump_hex(const void* data, size_type len, const dump_hex_format&amp; format, const line_consumer_proc&amp; proc) -&gt; void;
</pre>
<p>
按照十六进制编辑器的样式格式化数据
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">data, len</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被格式化的数据的地址和数据长度
</td></tr>
<tr class="tr-param"><td class="td-param-names">format</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定 dump_hex 时的格式化信息，参考 <a href="#dump_hex_format">dump_hex_format</a>
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于接收格式化数据
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
求和 <a id="sum"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    template &lt;typename T&gt;
    static auto sum(std::string_view s, const mapping_proc&lt;T&gt;&amp; proc) -&gt; T;
</pre>
<p>
将字符串 <code class="source">s</code> 中的每个字符先用 <code class="source">proc</code> 做映射，然后做求和计算，并返回最终的结果。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被映射的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 映射函数
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回求和后的结果
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
字符集对象生成 <a id="charset"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto charset(std::string_view s) -&gt; charset_type;
    static auto charset() -&gt; charset_type;
</pre>
<p>
<code class="source">charset</code> 用于将 <code class="source">s</code> 中的所有字符设置到字符集对象(charset_type)中。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 从 <code class="source">s</code> 中的每个不重复字符作为字符集的一部分
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回生成的字符集
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
范围对象生成 <a id="range"></a><a id="interval"></a><a id="shifter"></a><a id="range_interval_shifter"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto range(size_type pos, size_type n) -&gt; range_type;
    static auto range() -&gt; range_type;
    static auto range(std::string_view s, size_type pos = 0) -&gt; range_type;
    static auto interval(size_type begin, size_type end) -&gt; interval_type;
    static auto shifter(size_type pos, ssize_type offset) -&gt; shifter_type;
</pre>
<ul>
<li>
range 用于根据指定的 <code class="source">pos</code> 和 <code class="source">n</code> 生成一个 <a href="#range_type">range_type</a> 类型的范围对象。
</li>
<li>
interval 用于根据指定的 <code class="source">begin</code> 和 <code class="source">end</code> 生成一个 <a href="#interval_type">interval_type</a> 类型的范围对象。
</li>
<li>
shifter 用于给定起始点 <code class="source">pos</code> 以及偏移量 <code class="source">offset</code> 来数据的范围，其返回的是 <a href="#shifter_type">shifter_type</a> 类型的对象。
</li>
</ul>
<p>
@notice{1} 需要特别注意 range 在某些边界场景下并不是完全等价的。比如:
<code class="source">str::range(pos, n)</code> 并非总是等价于 <code class="source">str::interval(pos, (pos + n))</code>，因为考虑到 <code class="source">pos</code> 或者 <code class="source">n</code>
的值可能为 <code class="source">str::npos</code>，此时简单地用 <code class="source">(pos + n)</code> 做等价表示。
</p>
<p>
@notice{2} <a href="#shifter">shifter</a> 函数相对比较特殊，其 <code class="source">offset</code> 参数可以为正值也可以为负值。如果 <code class="source">offset</code> 为负值，
可以理解为 <code class="source">(pos + offset)</code> 到 <code class="source">pos</code> 的范围（不包含 <code class="source">pos</code>）。如果 <code class="source">offset</code> 为正值，可以理解为 <code class="source">pos</code>
到 <code class="source">(pos + offset)</code> 的范围。当然在遇到具体字符串时，会根据具体字符串的长度调整。
</p>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
读取一行 <a id="read_next_line"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto read_next_line(FILE* file, bool keep_ends, std::string&amp; line_text) -&gt; bool;
    static auto read_next_line(FILE* file, bool keep_ends = false) -&gt; std::optional&lt;std::string&gt;;
    static auto read_next_line(std::istream&amp; file, std::string&amp; line_text) -&gt; bool;
    static auto read_next_line(std::istream&amp; file) -&gt; std::optional&lt;std::string&gt;;
</pre>
<p>
以迭代的方式，从指定的文件读取一行文字并返回，本函数可以连续调用，以实现逐行读取效果。但如果需要逐行
读取文件的功能，可参考 <a href="#read_lines">read_lines</a>。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">filename, file</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定计划读取的文件的文件名，或者一个已经打开的文件对象，或者文件输入流对象
</td></tr>
<tr class="tr-param"><td class="td-param-names">keeo_ends</td><td class="td-param-sep">-</td><td class="td-param-desc"> 是否保留行尾的续行符
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
读取文件的全部内容 <a id="read_all"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto read_all(const std::string&amp; filename) -&gt; std::string;
    static auto read_all(const char* filename) -&gt; std::string;
</pre>
<p>
将文本文件 filename 中的所有内容读取出来并返回。需要注意，读取大文件很容易触发内存分配失败。
因此，read_all 主要为尺寸比较小的文本文件提供了简便的函数。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">filename</td><td class="td-param-sep">-</td><td class="td-param-desc"> 待读取文件内容的文件名
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">返回读取的文本文件的内容。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
按行读取多行 <a id="read_lines"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto read_lines(FILE* file, bool keep_ends, const line_consumer_proc&amp; proc) -&gt; void;
    static auto read_lines(FILE* file, bool keep_ends = false, size_type max_n = npos) -&gt; std::vector&lt;std::string&gt;;
    static auto read_lines(std::istream&amp; file, const line_consumer_proc&amp; proc) -&gt; void;
    static auto read_lines(std::istream&amp; file, size_type max_n = npos) -&gt; std::vector&lt;std::string&gt;;
    /// -
    static auto read_lines(const std::string&amp; filename, bool keep_ends, const line_consumer_proc&amp; proc) -&gt; void;
    static auto read_lines(const char* filename, bool keep_ends, const line_consumer_proc&amp; proc) -&gt; void;
    static auto read_lines(const std::string&amp; filename, bool keep_ends = false, size_type max_n = npos) -&gt; std::vector&lt;std::string&gt;;
    static auto read_lines(const char* filename, bool keep_ends = false, size_type max_n = npos) -&gt; std::vector&lt;std::string&gt;;
</pre>
<p>
从指定的文件（<code class="source">FILE*</code> 或者 <code class="source">std::istream</code> 或者 <code class="source">filename</code>）中连续读取多行文本。
</p>
<p>
@notice{1} 本系列函数和 <a href="#read_all">read_all</a> 类型，只适合文件内容较少的文本文件的读取，并不适合大型文件。
如果需要按行读取大型文件内容，可考虑基于 <a href="#read_next_line">read_next_line</a> 系列函数自行实现。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">file, filename</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定读取的数据的来源，如果是<code class="source">FILE*</code> 或者 <code class="source">std::istream</code> 类型的形式，表示从该文
</td></tr>
</tbody></table>
<p>
件的当前位置开始读取。如果是 <code class="source">std::string</code> 或者 <code class="source">const char*</code> 形式，将自动打开文件，并逐行读取文件所
有内容。
</p>
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">keep_ends</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定指定是否需要保留每行行位的结束符（如果有的话）。
</td></tr>
<tr class="tr-param"><td class="td-param-names">max_n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于限制读取行数，当年读取的行数达到 max_n 时，自动停止读取。
</td></tr>
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 通过 proc 函数接收行输出。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">通过 proc 或者返回值返回读取到的多行。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
打开文件并自动关闭文件 <a id="with_file"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto with_file(const std::string&amp; filepath, const char* mode, FILE* repl, const std::function&lt;void(FILE* f)&gt;&amp; proc) -&gt; void;
    static auto with_file(const std::string&amp; filepath, const char* mode, const std::function&lt;void(FILE* f)&gt;&amp; proc) -&gt; void;
</pre>
<p>
<code class="source">with_file</code> 根据指定的路径 <code class="source">filepath</code> 和 <code class="source">mode</code> 打开一个文件，并将这个文件交个 <code class="source">proc</code> 函数使用。
当 <code class="source">proc</code> 函数使用完毕后，<a href="#with_file">with_file</a> 函数会自动关闭该文件。本函数设计的相对特殊，其核心目的是
避免用户去处理打开和关闭文件的操作。在一定程度上可以避免文件忘记关闭问题。
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">filepath</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定文件路径
</td></tr>
<tr class="tr-param"><td class="td-param-names">mode</td><td class="td-param-sep">-</td><td class="td-param-desc"> 文件打开的模式
</td></tr>
<tr class="tr-param"><td class="td-param-names">repl</td><td class="td-param-sep">-</td><td class="td-param-desc"> 当遇到任何问题导致打开文件失败时，自动采用 repl 参数指定的文件对象代替。需要
</td></tr>
</tbody></table>
<p>
注意，@with_file 函数并不会自动关闭 <code class="source">repl</code> 文件。
</p>
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于接收并使用被 <a href="#with_file">with_file</a> 打开的文件对象的函数。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
命令行选项识别 <a id="next_opt1"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto next_opt1(int&amp; next_index, int argc, const char* argv[]) -&gt; std::optional&lt;pair&lt;std::string_view&gt;&gt;;
    static auto next_opt1(int&amp; next_index, int argc, char* argv[]) -&gt; std::optional&lt;pair&lt;std::string_view&gt;&gt;;
    template &lt;typename Container, typename SizeType = typename Container::size_type&gt;
    static auto next_opt1(SizeType&amp; next_index, const Container&amp; items) -&gt; std::optional&lt;pair&lt;std::string_view&gt;&gt;;
    template &lt;typename Iterator&gt;
    static auto next_opt1(Iterator&amp; itr, Iterator end) -&gt; std::optional&lt;pair&lt;std::string_view&gt;&gt;;
    template &lt;typename IterProc&gt;
    static auto next_opt1(const IterProc&amp; proc) -&gt; std::optional&lt;pair&lt;std::string_view&gt;&gt;;
</pre>
<p>
将一组字符串列表视作命令行的参数（选项）序列，<a href="#next_opt1">next_opt1</a> 函数从该序列中按照特定的模式读取和识别
出单个的命令行选项。
</p>
<p>
@notice{1} 与很多其他的命令行参数识别库不同，<a href="#next_opt1">next_opt1</a> 函数所支持的命令行参数的识别模式是一
种无 schema 的模式，其优点是不需要在参数识别前定义一堆的 schema 信息（比如，需要定义有哪些命令行参
数，每个参数的类型），所以使用起来相对方便快捷。但，相应的，<a href="#next_opt1">next_opt1</a> 无法实现很复杂的命令
行参数设计。<a href="#next_opt1">next_opt1</a> 很适合具有少量命令行参数的情况。下面是 <a href="#next_opt1">next_opt1</a> 函数的识别算
法：
</p>
<ul>
<li>
<code class="source">-</code> 为选项识别符，所有以 <code class="source">-</code> 开头的串均会作为键值对形式的参数，除非在此之前用 <code class="source">--</code> 转义；
</li>
<li>
<code class="source">-key</code> 定义一个独立的、无 <code class="source">value</code> 选项，常常用来定义开关型的选项；<code class="source">key</code> 部分为空也是允许的；
</li>
<li>
<code class="source">-key=value</code> 识别为名字为 <code class="source">key</code> 且值为 <code class="source">value</code> 的键值对参数；若 <code class="source">value</code> 部分为空，与 <code class="source">-key</code> 等价；
</li>
<li>
<code class="source">value</code> 识别为一个没有 <code class="source">key</code>，但是有 <code class="source">value</code> 的参数；
</li>
<li>
<code class="source">-- value</code> 用于对选项识别符号进行转义，用于处理 value 部分本身已以 <code class="source">-</code> 开头的情况；
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">next_index</td><td class="td-param-sep">-</td><td class="td-param-desc"> 从该位置的字符串开始识别下一个选项。
</td></tr>
<tr class="tr-param"><td class="td-param-names">argc, argv</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定命令行参数序列的大小和起始地址，常用于匹配 <code class="source">main</code> 函数的参数。
</td></tr>
<tr class="tr-param"><td class="td-param-names">items</td><td class="td-param-sep">-</td><td class="td-param-desc"> 存放命令行参数的容器，用于代替 <code class="source">argc</code> 与 <code class="source">argv</code> 的组合。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">以键值对的形式返回读取到的选项，并提前将 next_index 移动到选项的结尾。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
命令行选项识别 <a id="next_opt2"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto next_opt2(int&amp; next_index, int argc, const char* argv[]) -&gt; std::optional&lt;pair&lt;std::string_view&gt;&gt;;
    static auto next_opt2(int&amp; next_index, int argc, char* argv[]) -&gt; std::optional&lt;pair&lt;std::string_view&gt;&gt;;
    template &lt;typename Container, typename SizeType = typename Container::size_type&gt;
    static auto next_opt2(SizeType&amp; next_index, const Container&amp; items) -&gt; std::optional&lt;pair&lt;std::string_view&gt;&gt;;
    template &lt;typename Iterator&gt;
    static auto next_opt2(Iterator&amp; itr, Iterator end) -&gt; std::optional&lt;pair&lt;std::string_view&gt;&gt;;
</pre>
<p>
将一组字符串列表视作命令行的参数（选项）序列，<a href="#next_opt2">next_opt2</a> 函数从该序列中按照特定的模式读取和识别
出单个的命令行选项。
</p>
<p>
@notice{1} 与很多其他的命令行参数识别库不同，<a href="#next_opt2">next_opt2</a> 函数所支持的命令行参数的识别模式是一
种无 schema 的模式，其优点是不需要在参数识别前定义一堆的 schema 信息（比如，需要定义有哪些命令行参
数，每个参数的类型），所以使用起来相对方便快捷。但，相应的，<a href="#next_opt2">next_opt2</a> 无法实现很复杂的命令
行参数设计 <a href="#next_opt2">next_opt2</a> 很适合具有少量命令行参数的情况。下面是 <a href="#next_opt2">next_opt2</a> 函数的识别算
法：
</p>
<ul>
<li>
<code class="source">-</code> 为选项识别符，所有以 <code class="source">-</code> 开头的串均会作为键值对形式的参数，除非在此之前用 <code class="source">--</code> 转义；
</li>
<li>
<code class="source">-key value</code> 识别为名字为 <code class="source">key</code> 且值为 <code class="source">value</code> 的键值对参数；若 <code class="source">value</code> 部分为空，与 <code class="source">-key</code> 等价；
</li>
<li>
<code class="source">-key -</code> 定义一个独立的、无 <code class="source">value</code> 选项，常常用来定义开关型的选项；<code class="source">key</code> 部分为空也是允许的；
<p>
  如果 <code class="source">-key</code> 后面是另一个 key，那么，<code class="source">-</code> 可以省略
</p>
</li>
<li>
<code class="source">value</code> 识别为一个没有 <code class="source">key</code>，但是有 <code class="source">value</code> 的参数；
</li>
<li>
<code class="source">-- value</code> 用于对选项识别符号进行转义，用于处理 value 部分本身已以 <code class="source">-</code> 开头的情况；
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">next_index</td><td class="td-param-sep">-</td><td class="td-param-desc"> 从该位置的字符串开始识别下一个选项。
</td></tr>
<tr class="tr-param"><td class="td-param-names">argc, argv</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定命令行参数序列的大小和起始地址，常用于匹配 <code class="source">main</code> 函数的参数。
</td></tr>
<tr class="tr-param"><td class="td-param-names">items</td><td class="td-param-sep">-</td><td class="td-param-desc"> 存放命令行参数的容器，用于代替 <code class="source">argc</code> 与 <code class="source">argv</code> 的组合。
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">以键值对的形式返回读取到的选项，并提前将 next_index 移动到选项的结尾。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
简单词法识别 <a id="accept"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto accept_until(std::string_view s, size_type&amp; pos, value_type guard_ch) -&gt; std::optional&lt;range_type&gt;;
    static auto accept_until(std::string_view s, size_type&amp; pos, value_type escape, value_type guard_ch) -&gt; std::optional&lt;range_type&gt;;
    static auto accept_until(std::string_view s, size_type&amp; pos, std::string_view guard_token) -&gt; std::optional&lt;range_type&gt;;
    static auto accept_until(std::string_view s, size_type&amp; pos, const std::regex&amp; guard_pattern) -&gt; std::optional&lt;range_type&gt;;
    static auto accept_until(std::string_view s, size_type&amp; pos, const charset_type&amp; guard_charset) -&gt; std::optional&lt;range_type&gt;;
    static auto accept_until(std::string_view s, size_type&amp; pos, value_type escape, const charset_type&amp; guard_charset) -&gt; std::optional&lt;range_type&gt;;
    static auto accept_until(std::string_view s, size_type&amp; pos, const char_match_proc&amp; guard_proc) -&gt; std::optional&lt;range_type&gt;;
    static auto accept_until(std::string_view s, size_type&amp; pos, value_type escape, const char_match_proc&amp; guard_proc) -&gt; std::optional&lt;range_type&gt;;
    /// -
    static auto accept(std::string_view s, size_type&amp; pos, value_type expect_ch) -&gt; std::optional&lt;range_type&gt;;
    static auto accept(std::string_view s, size_type&amp; pos, std::string_view expect_token) -&gt; std::optional&lt;range_type&gt;;
    static auto accept(std::string_view s, size_type&amp; pos, const std::regex&amp; expect_pattern) -&gt; std::optional&lt;range_type&gt;;
    static auto accept(std::string_view s, size_type&amp; pos, const charset_type&amp; expect_charset) -&gt; std::optional&lt;range_type&gt;;
    static auto accept(std::string_view s, size_type&amp; pos, const char_match_proc&amp; expect_proc) -&gt; std::optional&lt;range_type&gt;;
</pre>
<ul>
<li>
<code class="source">accept_until</code> 从 <code class="source">pos</code> 位置开始逐个扫描 <code class="source">s</code> 中的每个字符，如果遇到哨兵字符或者字符串
<p>
  或者其他形式的哨兵，则返回截止到哨兵起始位置的字符范围。当指定 <code class="source">escape</code> 字符时，表示扫描到 <code class="source">escape</code> 字符时，
  自动忽略下一个字符，即使这个字符就是哨兵字符本身。最终，<code class="source">accept_until</code> 会返回哨兵之前的字符序列的范围。
</p>
</li>
<li>
<code class="source">accept</code> 从 <code class="source">pos</code> 位置开始逐个扫描 <code class="source">s</code> 中的每个字符，收集满足条件的字符序列，
<p>
  直到下一个字符无法满足条件为止。最终，accept 会返回满足条件的字符序列的范围。
</p>
</li>
</ul>
<p>
@notice{0} 如果扫描失败，那么所有函数都会返回 <code class="source">std::nullopt</code>，且输出参数 <code class="source">pos</code> 不会发生改变。
</p>
<p>
@notice{1} accept 和 accept_until 这两组函数的行为的重要区别是：accept 系列函数总是 &apos;盯着&apos; 满足
条件的字符，而 accept_until 总是 &apos;盯着&apos; 不满足条件的字符。
</p>
<p>
@notice{2} 虽然 accept 和 accept_until 这两组函数都是可以连续调用的，但是需要注意 accept_until 总是
会自动跳过已经识别出来的哨兵。因此，如果连续调用 accept_until，总是不可能得到哨兵序列的范围。如果确
实有必要获得的哨兵字符串，可以通过 pos 与返回的 range_type 来组合计算出来。如下示例：
</p>
<pre  class="source">
auto range = str::accept_until(s, pos, token);auto token_range = str::range(range-&gt;end_pos(), pos - range-&gt;end_pos());</pre>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 待扫描的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">pos</td><td class="td-param-sep">-</td><td class="td-param-desc"> 扫描的起始位置，该参数为输入输出参数。如果符号识别成功，对于 accept_until 来说，pos 总是
</td></tr>
</tbody></table>
<p>
位于哨兵序列最后一个字符的下一个字符；而对于 accept 由于并没有哨兵，它总是指向找到的第一个不满足条件
的字符的位置。
</p>
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">guard_ch</td><td class="td-param-sep">-</td><td class="td-param-desc"> 以单个字符作为哨兵
</td></tr>
<tr class="tr-param"><td class="td-param-names">guard_charset</td><td class="td-param-sep">-</td><td class="td-param-desc"> 以字符集（多个可选的字符）为哨兵
</td></tr>
<tr class="tr-param"><td class="td-param-names">guard_proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 更抽象的单字符的哨兵，在扫描时只要字符满足条件即表示遇到哨兵
</td></tr>
<tr class="tr-param"><td class="td-param-names">guard_token</td><td class="td-param-sep">-</td><td class="td-param-desc"> 以一个字符序列为哨兵
</td></tr>
<tr class="tr-param"><td class="td-param-names">guard_pattern</td><td class="td-param-sep">-</td><td class="td-param-desc"> 以一个正则表达式为哨兵（需要确保不匹配到空串）
</td></tr>
<tr class="tr-param"><td class="td-param-names">escape</td><td class="td-param-sep">-</td><td class="td-param-desc"> 扫描过程中如果遇到 escape 字符，将自动忽略下一个字符，以实现字符转义的效果
</td></tr>
<tr class="tr-param"><td class="td-param-names">expect_ch</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于 accept 函数，表示待识别的字符
</td></tr>
<tr class="tr-param"><td class="td-param-names">expect_token</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于 accept 函数，表示待识别的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">expect_pattern</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于 accept 函数，表示需要识别的字符序列的模式
</td></tr>
<tr class="tr-param"><td class="td-param-names">expect_charset</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于 accept 函数，表示待识别的满足条件的字符集
</td></tr>
<tr class="tr-param"><td class="td-param-names">expect_proc</td><td class="td-param-sep">-</td><td class="td-param-desc"> 用于 accept 函数，表示待识别的字符需要满足的条件
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">accept_until 系列函数总是返回找到的哨兵字符序列之前的字符序列的范围；而 accept 系列
</p>
<p>
函数总是返回满足条件的字符序列的范围。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
跳过满足条件的字符 <a id="skip"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
    static auto skip_n(std::string_view s, size_type&amp; pos, size_type n) -&gt; bool;
    static auto skip_max(std::string_view s, size_type&amp; pos, size_type max_n) -&gt; std::optional&lt;size_type&gt;;
    static auto skip_spaces(std::string_view s, size_type&amp; pos) -&gt; void;
    static auto skip_spaces(std::string_view s, size_type&amp; pos, size_type min_n) -&gt; bool;
</pre>
<ul>
<li>
<code class="source">skip_n</code> 从 <code class="source">pos</code> 开始跳过 <code class="source">n</code> 个字符，并返回 <code class="source">true</code>；如果长度不够，返回 <code class="source">false</code>；
</li>
<li>
<code class="source">skip_max</code> 从 <code class="source">pos</code> 开始跳过最多 <code class="source">max_n</code> 个字符，跳过实际跳过的长度；
</li>
<li>
<code class="source">skip_spaces</code> 从 <code class="source">pos</code> 开始，跳过所有空白字符；如果指定了 <code class="source">min_n</code> 参数，那么空白数量不够 min_n 时，会返回 <code class="source">false</code>；
</li>
</ul>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被视作符号识别的缓冲区的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">pos</td><td class="td-param-sep">-</td><td class="td-param-desc"> 作为输入参数是表示指定识别的起始位置，作为输出参数时表示下一个还未被识别的字符的位置
</td></tr>
<tr class="tr-param"><td class="td-param-names">n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 跳过 n 个字符
</td></tr>
<tr class="tr-param"><td class="td-param-names">max_n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 最多跳过的字符的数量，如果剩余字符数量不够将自动返回
</td></tr>
<tr class="tr-param"><td class="td-param-names">min_n</td><td class="td-param-sep">-</td><td class="td-param-desc"> 指定至少需要跳过的字符数量
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">如果识别成功，将返回符号的范围，如果识别失败，返回的范围对象长度为 0，如果 pos 已经不在 s 的范围内，pos 的值
</p>
<p>
将大于或者等于 <code class="source">s.size()</code>。因此，可以通过测试 <code class="source">(pos &gt;= s.size())</code> 来确定是否所有数据已经识别完。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
符号识别 <a id="skip_spaces"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
</pre>
<p>
尝试识别指定类型的符号，并返回符号的范围
</p>
<section class="section-1">
<h1 class="section-1-head">
参数
</h1>
<div class="section-1-body">
<table class="table-param">
<tbody class="tbody-param">
<tr class="tr-param"><td class="td-param-names">s</td><td class="td-param-sep">-</td><td class="td-param-desc"> 被视作符号识别的缓冲区的字符串
</td></tr>
<tr class="tr-param"><td class="td-param-names">pos</td><td class="td-param-sep">-</td><td class="td-param-desc"> 作为输入参数是表示指定识别的起始位置，作为输出参数时表示下一个还未被识别的字符的位置
</td></tr>
</tbody></table>
</div>
<div class="sectionend-1"></div></section>
<section class="section-1">
<h1 class="section-1-head">
返回值
</h1>
<div class="section-1-body">
<p class="return">如果识别成功，将返回符号的范围，如果识别失败，返回的范围对象长度为 0，如果 pos 已经不在 s 的范围内，pos 的值将大于或者等于
</p>
<p>
<code class="source">s.size()</code>。因此，可以通过测试 <code class="source">(pos &gt;= s.size())</code> 来确定是否所有数据已经识别完。
</p>
</div>
<div class="sectionend-1"></div></section>
</div>
<div class="chapterend-3"></div></section>
<section class="chapter-3">
<h3>
用户根目录 <a id="home"></a>
</h3>
<div class="chapter-3-body">
<pre  class="source">
</pre>
<p>
返回当前用户的根目录，等价于 <code class="source">*nix</code> 下的 <code class="source">${HOME}</code> 环境变量的值，主要被用于 <a href="#expand_user">expand_user</a> 函数。
</p>
<ul>
<li>
@notice{1} 由于 Windows 下并无严格意义上的与 <code class="source">*nix</code> 下对等的用户根目录的概念，因此，
<p>
  在 Windows 下会以 <code class="source">USERPROFILE</code> 环境变量的值来作为 <code class="source">${HOME}</code> 的值。
</p>
</li>
</ul>
</div>
<div class="chapterend-3"></div></section>
<div class="articleend"></div></main>
</body>
</html>
