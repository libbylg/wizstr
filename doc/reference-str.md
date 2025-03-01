[TOC]

# TINY

tiny 库实现了一个简单、好用、高效的字符串处理基础设施。主要由 `bytes`、`re` 几个类组成。用于解决 C++ 开发团队中尝尝面临不想依赖一些重型库，但又没有好用的字符串处理工具（类）的问题。tiny 库的几个主要类，参考了 C++STL、Qt、Folly、golang、Java、Python 这些库或者语言的类似的字符串处理工具的接口设计。我们力求做到 C++ 开发者所理解的好用。

# bytes 类


### 成员函数

#### 构造析构赋值

| 返回值 | 函数签名|
|---    |--- |
|       | byteses()                                |
|       | bytes()                                 |
|       | bytes(const_pointer s)               |
|       | bytes(const_pointer s, size_type n)  |
|       | bytes(value_type ch, size_type size)     |
|       | bytes(bytes&& other) noexcept             |
|       | bytes(const bytes& other)                 |
| bytes&  | operator=(bytes&& other) noexcept       |
| bytes&  | operator=(const bytes& other)           |

#### 迭代器

| 返回值 | 函数签名|
|---    |--- |
| bytes::iterator                 | begin()           |
| bytes::iterator                 | end()             |
| bytes::const_iterator           | begin() const     |
| bytes::const_iterator           | end() const       |
| bytes::reverse_iterator         | rbegin()          |
| bytes::reverse_iterator         | rend()            |
| bytes::const_reverse_iterator   | rbegin() const    |
| bytes::const_reverse_iterator   | rend() const      |
| value_type                     | at(pos_type rpos) const       |
| value_type                     | back() const      |
| value_type&                    | back()        |
| value_type                     | front() const     |
| value_type&                    | front()       |

#### 获取长度容量

| 返回值 | 函数签名|
|---    |--- |
| size_type | size() const |
| size_type | capacity() const |

//  清空
void clear();

#### 对接 C 接口

| 返回值 | 函数签名|
|---    |--- |
| const_pointer | data() const |
| pointer       | data()   |
| void          | attach(pointer buf, size_type len, size_type cap) |

#### 首尾修改

| 返回值 | 函数签名|
|---    |---     |
| bytes&      | append(const bytes& bytes)                        |
| bytes&      | append(value_type ch)                          |
| bytes&      | append(const_pointer bytes, size_type len)   |
| bytes&      | append(const_pointer bytes)                  |
| bytes&      | prepend(const bytes& bytes)                       |
| bytes&      | prepend(value_type ch)                         |
| bytes&      | prepend(const_pointer bytes, size_type len)  |
| bytes&      | prepend(const_pointer bytes)                 |
| void      | push_back(const bytes& other)                   |
| void      | push_back(value_type ch)                       |
| void      | push_front(const bytes& other)                  |
| void      | push_front(value_type ch)                      |
| value_type | pop_back()                                    |
| value_type | pop_front()                                   |

#### 中间插入

| 返回值 | 函数签名|
|---    |---     |
| bytes&   | insert(pos_type rpos, const bytes& bytes)                    |
| bytes&   | insert(pos_type rpos, value_type ch)                      |
| bytes&   | insert(pos_type rpos, const_pointer s, size_type n)   |
| bytes&   | insert(pos_type rpos, const_pointer s)                |


#### 删除数据

| 返回值 | 函数签名|
|---    |---     |
| bytes&      | remove(pos_type rpos)                                      |
| bytes&      | remove(pos_type rpos, size_type n)                         |
| bytes&      | remove(value_type ch)                                      |
| bytes&      | remove(const_pointer bytes)                              |
| bytes&      | remove(const bytes& bytes)                                    |
| bytes&      | remove(const re& rx)                                      |
| bytes&      | remove(std::function<int(value_type c, bool& match)> func) |
| bytes&      | erase(pos_type rpos = 0, pos_type len = npos)              |
| iterator  | erase(const_iterator p)                                   |
| iterator  | erase(const_iterator first, const_iterator last)          |

#### 移动子串

| 返回值 | 函数签名|
|---    |---     |
| bytes&  | flex_move(pos_type rpos, size_type n, int offset)                      |
| bytes&  | flex_move(pos_type rpos, size_type n, int offset, value_type fill_ch)   |
| bytes&  | clip_move(pos_type rpos, size_type n, int offset)                      |
| bytes&  | clip_move(pos_type rpos, size_type n, int offset, value_type fill_ch)   |


#### 简单字符串比较

| 返回值 | 函数签名|
|---    |---     |
| int    | compare(const bytes& other) const      |
| int    | compare(const_pointer s) const    |            
| int    | compare(value_type c) const           |
| int    | compare(const_pointer s, size_type max_n) const    |            

#### 子串包含

| 返回值 | 函数签名|
|---    |---     |
| bool  | contains(const bytes& s) const          |
| bool  | contains(const_pointer s) const    |
| bool  | contains(value_type ch) const          |
| bool  | contains(const re& rx) const          |
| bool  | contains(re& re) const                |

#### 子串统计

| 返回值 | 函数签名|
|---    |---     |
| int   | count(const bytes& s) const             |
| int   | count(const_pointer s) const          |           
| int   | count(value_type ch) const            |
| int   | count(const re& rx) const             |

#### 前后缀操作

| 返回值 | 函数签名|
|---    |---     |
| bool  | has_suffix(const bytes& s) const        |
| bool  | has_suffix(const_pointer bytes) const   |
| bool  | has_suffix(value_type c) const        |
| bool  | has_prefix(const bytes& s) const        |
| bool  | has_prefix(const_pointer s) const     |
| bool  | has_prefix(value_type c) const        |
| bool  | remove_prefix(const bytes& s)           |
| bool  | remove_prefix(const_pointer s)        |
| bool  | remove_prefix(value_type c)           |
| bool  | remove_suffix(const bytes& s)           |
| bool  | remove_suffix(const_pointer s)        |
| bool  | remove_suffix(value_type c)           |

#### 填充

| 返回值 | 函数签名|
|---    |---     |
| bytes&  | fill(value_type ch)                                |
| bytes&  | fill(pos_type rpos, value_type ch, size_type n)     |

#### 查找

| 返回值 | 函数签名|
|---    |---     |
| pos_type | index_of(const bytes& bytes, pos_type from = 0) const                                          |
| pos_type | index_of(const_pointer s, pos_type from = 0) const                                      |
| pos_type | index_of(value_type ch, pos_type from = 0) const                                            |
| pos_type | index_of(const re& rx, pos_type from = 0) const                                            |
| pos_type | index_of(re& rx, pos_type from = 0) const                                                  |
| pos_type | index_of(std::function<int(value_type c, bool& match)> func, pos_type from = 0) const       |
| pos_type | last_index_of(const bytes& bytes, pos_type from = -1) const                                    |
| pos_type | last_index_of(value_type ch, pos_type from = -1) const                                      |
| pos_type | last_index_of(const_pointer bytes, pos_type from = -1) const                              |
| pos_type | last_index_of(const re& rx, pos_type from = -1) const                                      |
| pos_type | last_index_of(re& rx, pos_type from = -1) const                                            |
| pos_type | last_index_of(std::function<int(value_type c, bool& match)> func, pos_type from = -1) const |

#### 匹配检测

| 返回值 | 函数签名|
|---    |---     |
| bool  | is_match(re& rx) const                         |
| bool  | is_match(const bytes& pattern) const             |
| bool  | is_match(const_pointer& pattern) const      |
| bool  | is_match_wild(const bytes& pattern) const        |
| bool  | is_match_wild(const_pointer& pattern) const |

#### 字符串特征检测

| 返回值 | 函数签名|
|---    |---     |
| bool  | is_empty() const          |
| bool  | is_lower() const          |
| bool  | is_upper() const          |
| bool  | is_title() const          |
| bool  | is_digit() const          |
| bool  | is_ascii() const          |
| bool  | is_alpha() const          |
| bool  | is_alnum() const          |
| bool  | is_space() const          |
| bool  | is_print() const      |
| bool  | is_identifier() const     |             
| bool  | is_numeric() const        |
| bool  | is_bool() const        |

#### 提取子串

| 返回值 | 函数签名|
|---    |---     |
| bytes   | left(size_type n) const                       |
| bytes   | right(size_type n) const                      |
| bytes   | cutstr(pos_type rpos, int offset_n = -1) const |

#### 限宽对齐

| 返回值 | 函数签名|
|---    |---     |
| bytes&  | ljust(size_type width, value_type fill = ' ', bool truncate = false)  |
| bytes&  | rjust(size_type width, value_type fill = ' ', bool truncate = false)  |
| bytes&  | center(size_type width, value_type fill = ' ', bool truncate = false) |
| bytes&  | zfill(size_type width, value_type fill = ' ', bool truncate = false)  |

#### 子串替换

| 返回值 | 函数签名|
|---    |---     |
| bytes&  | replace(pos_type rpos, size_type n, const bytes& after)                               |
| bytes&  | replace(pos_type rpos, size_type n, value_type after)                               |
| bytes&  | replace(pos_type rpos, size_type n, const_pointer unicode, size_type size)          |
| bytes&  | replace(value_type before, value_type after)                                       |
| bytes&  | replace(const_pointer before, size_type blen, const_pointer after, size_type alen) |
| bytes&  | replace(const bytes& before, const bytes& after)                                       |
| bytes&  | replace(value_type ch, const bytes& after)                                           |
| bytes&  | replace(const re& rx, const bytes& after)                                            |
| bytes&  | replace(std::function<int(value_type key, value_type& val)> func)                  |

#### 容量、内存管理

| 返回值 | 函数签名|
|---    |---     |
| void  | reserve(size_type size)                       |
| void  | resize(size_type size)                        |
| void  | resize(size_type size, value_type fill_ch)    |
| void  | shrink_to_fit()                               |
| void  | squeeze()                                     |


#### 逆序

| 返回值 | 函数签名|
|---    |---     |
| bytes&  | inversion() |

#### 分割子串

| 返回值 | 函数签名|
|---    |---     |
| std::vector<bytes> | split(const bytes& sep) const                                                |
| std::vector<bytes> | split(const_pointer sep) const                                             |
| std::vector<bytes> | split(value_type sep) const                                                |
| std::vector<bytes> | split(const re& r) const                                                   |
| std::vector<bytes> | split(std::function<int(value_type c, bool& match)>& chars_func) const     |
| std::vector<bytes> | split_lines(bool keep_ends = false) const                                  |
| std::vector<bytes> | split_path() const                                                         |
| void             | split(const bytes& sep, std::function<int(const_pointer s, size_type n)> output_func) const      |
| void             | split(const_pointer sep, std::function<int(const_pointer s, size_type n)> output_func) const   |
| void             | split(value_type sep, std::function<int(const_pointer s, size_type n)> output_func) const      |
| void             | split(const re& r, std::function<int(const_pointer s, size_type n)> output_func) const         |
| void             | split(std::function<int(value_type c, bool& match)>& chars_func, std::function<int(const_pointer s, size_type n)> output_func) const    |
| void             | split_lines(bool keep_ends, std::function<int(const_pointer s, size_type n)> output_func) const    |
| void             | split_path(std::function<int(const_pointer s, size_type n)> output_func) const                     |

#### 大小写转换

| 返回值 | 函数签名|
|---    |---     |
| bytes&  | to_lower()    |
| bytes&  | to_upper()    |
| bytes&  | swap_case()   |
| bytes&  | title()       |

#### 空白处理

| 返回值 | 函数签名|
|---    |---     |
| bytes&  | simplified()  |
| bytes&  | ltrim()       |
| bytes&  | rtrim()       |
| bytes&  | trim()        |

#### 模板展开

| 返回值 | 函数签名|
|---    |---     |
| bytes   | expand(const std::map<bytes, bytes>& kvs) const                               |
| bytes   | expand(std::function<int(const bytes& key, bytes& val)> provider) const       |
| bytes   | expand_envs(const_pointer key, const_pointer val) const                   |
| bytes   | expand_envs() const                                                       |
| bytes   | expand_tabs(size_type tab_size = 8) const                                 |
| bytes   | expand_tmpl(const std::map<bytes, bytes>& kvs) const                          |
| bytes   | expand_tmpl(std::function<int(const bytes& key, bytes& val)> provider) const  |


#### 路径处理

| 返回值 | 函数签名|
|---    |---     |
| bytes   | basename() const      |
| bytes&  | basename()            |
| bytes   | dirname() const       |
| bytes&  | dirname()             |

#### 类型互转

| 返回值 | 函数签名|
|---    |---     |
| bool  | to_bool(bool* ok = nullptr) const |
| bytes&  | assign(bool v)                    |
| double    | to_double(bool* ok = nullptr) const                   |
| float     | to_float(bool* ok = nullptr) const                    |
| int8_t    | to_int8(bool* ok = nullptr, int base = 10) const      |
| int16_t   | to_int16(bool* ok = nullptr, int base = 10) const     |
| int32_t   | to_int32(bool* ok = nullptr, int base = 10) const     |
| int64_t   | to_int64(bool* ok = nullptr, int base = 10) const     |
| uint8_t   | to_uint8(bool* ok = nullptr, int base = 10) const     |
| uint16_t  | to_uint16(bool* ok = nullptr, int base = 10) const    |
| uint32_t  | to_uint32(bool* ok = nullptr, int base = 10) const    |
| uint64_t  | to_uint64(bool* ok = nullptr, int base = 10) const    |
| bytes&      | assign(double n, value_type format = 'g', int precision = 6)      |
| bytes&      | assign(float n, value_type format = 'g', int precision = 6)       |
| bytes&      | assign(int8_t n, int base = 10)                                   |
| bytes&      | assign(int16_t n, int base = 10)                                  |
| bytes&      | assign(int32_t n, int base = 10)                                  |
| bytes&      | assign(int64_t n, int base = 10)                                  |
| bytes&      | assign(uint8_t n, int base = 10)                                  |
| bytes&      | assign(uint16_t n, int base = 10)                                 |
| bytes&      | assign(uint32_t n, int base = 10)                                 |
| bytes&      | assign(uint64_t n, int base = 10)                                 |

#### 计算 hash

| 返回值 | 函数签名|
|---    |---     |
| int32_t | hash_code() const |

#### 拷贝和交换

| 返回值 | 函数签名|
|---    |---     |
| size_type  | copy(pointer dest, size_type n, pos_type rpos = 0) const |
| void       | swap(bytes& other)       |

#### 新串生成

| 返回值 | 函数签名|
|---    |---     |
| bytes   | repeated(size_type times) const           |
| bytes   | join(const std::vector<bytes>& s) const     |
| bytes   | join(const bytes& s, ...) const             |
| bytes   | join(const_pointer s, ...) const          |

#### 数字转换为字符串

| 返回值 | 函数签名|
|---    |---     |
| static bytes | from(double n, value_type format = 'g', int precision = 6)     |
| static bytes | from(float n, value_type format = 'g', int precision = 6)      |
| static bytes | from(int8_t n, int base = 10)                                  |
| static bytes | from(int16_t n, int base = 10)                                 |
| static bytes | from(int32_t n, int base = 10)                                 |
| static bytes | from(int64_t n, int base = 10)                                 |
| static bytes | from(uint8_t n, int base = 10)                                 |
| static bytes | from(uint16_t n, int base = 10)                                |
| static bytes | from(uint32_t n, int base = 10)                                |
| static bytes | from(uint64_t n, int base = 10)                                |

####  运算符重载

| 返回值 | 函数签名|
|---    |---     |
| bytes& | operator=(value_type ch)                       |
| bytes& | operator=(const_pointer bytes)                   |
| bytes& | operator+=(const_pointer bytes)                  |
| bytes& | operator+=(value_type ch)                      |
| bytes& | operator+=(const bytes& other)                   |
| bool | operator!=(const_pointer other) const          |
| bool | operator<(const_pointer other) const           |
| bool | operator<=(const_pointer other) const          |
| bool | operator==(const_pointer other) const          |
| bool | operator>(const_pointer other) const           |
| bool | operator>=(const_pointer other) const          |
| bytes::value_type&       | operator[](pos_type rpos)         |
| const bytes::value_type& | operator[](pos_type rpos) const   |



