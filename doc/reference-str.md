# TINY

tiny 库实现了一个简单、好用、高效的字符串处理基础设施。主要由 `str`、`re` 几个类组成。用于解决 C++ 开发团队中尝尝面临不想依赖一些重型库，但又没有好用的字符串处理工具（类）的问题。tiny 库的几个主要类，参考了 C++STL、Qt、Folly、golang、Java、Python 这些库或者语言的类似的字符串处理工具的接口设计。我们力求做到 C++ 开发者所理解的好用。

# str 类


### 成员函数

#### 构造析构赋值

| 返回值 | 函数签名|
|---    |--- |
|       | ~str()                                |
|       | str()                                 |
|       | str(const_pointer s)               |
|       | str(const_pointer s, size_type n)  |
|       | str(value_type ch, size_type size)     |
|       | str(str&& other) noexcept             |
|       | str(const str& other)                 |
| str&  | operator=(str&& other) noexcept       |
| str&  | operator=(const str& other)           |

#### 迭代器

| 返回值 | 函数签名|
|---    |--- |
| str::iterator                 | begin()           |
| str::iterator                 | end()             |
| str::const_iterator           | begin() const     |
| str::const_iterator           | end() const       |
| str::reverse_iterator         | rbegin()          |
| str::reverse_iterator         | rend()            |
| str::const_reverse_iterator   | rbegin() const    |
| str::const_reverse_iterator   | rend() const      |
| value_type                     | at(pos_type pos) const       |
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
| str&      | append(const str& str)                        |
| str&      | append(value_type ch)                          |
| str&      | append(const_pointer str, size_type len)   |
| str&      | append(const_pointer str)                  |
| str&      | prepend(const str& str)                       |
| str&      | prepend(value_type ch)                         |
| str&      | prepend(const_pointer str, size_type len)  |
| str&      | prepend(const_pointer str)                 |
| void      | push_back(const str& other)                   |
| void      | push_back(value_type ch)                       |
| void      | push_front(const str& other)                  |
| void      | push_front(value_type ch)                      |
| value_type | pop_back()                                    |
| value_type | pop_front()                                   |

#### 中间插入

| 返回值 | 函数签名|
|---    |---     |
| str&   | insert(pos_type pos, const str& str)                    |
| str&   | insert(pos_type pos, value_type ch)                      |
| str&   | insert(pos_type pos, const_pointer s, size_type n)   |
| str&   | insert(pos_type pos, const_pointer s)                |


#### 删除数据

| 返回值 | 函数签名|
|---    |---     |
| str&      | remove(pos_type pos)                                      |
| str&      | remove(pos_type pos, size_type n)                         |
| str&      | remove(value_type ch)                                      |
| str&      | remove(const_pointer str)                              |
| str&      | remove(const str& str)                                    |
| str&      | remove(const re& rx)                                      |
| str&      | remove(std::function<int(value_type c, bool& match)> func) |
| str&      | erase(pos_type pos = 0, pos_type len = npos)              |
| iterator  | erase(const_iterator p)                                   |
| iterator  | erase(const_iterator first, const_iterator last)          |

#### 移动子串

| 返回值 | 函数签名|
|---    |---     |
| str&  | flex_move(pos_type pos, size_type n, int offset)                      |
| str&  | flex_move(pos_type pos, size_type n, int offset, value_type fill_ch)   |
| str&  | clip_move(pos_type pos, size_type n, int offset)                      |
| str&  | clip_move(pos_type pos, size_type n, int offset, value_type fill_ch)   |


#### 简单字符串比较

| 返回值 | 函数签名|
|---    |---     |
| int    | compare(const str& other) const      |
| int    | compare(const_pointer s) const    |            
| int    | compare(value_type c) const           |
| int    | compare(const_pointer s, size_type max_n) const    |            

#### 子串包含

| 返回值 | 函数签名|
|---    |---     |
| bool  | contains(const str& s) const          |
| bool  | contains(const_pointer s) const    |
| bool  | contains(value_type ch) const          |
| bool  | contains(const re& rx) const          |
| bool  | contains(re& re) const                |

#### 子串统计

| 返回值 | 函数签名|
|---    |---     |
| int   | count(const str& s) const             |
| int   | count(const_pointer s) const          |           
| int   | count(value_type ch) const            |
| int   | count(const re& rx) const             |

#### 前后缀操作

| 返回值 | 函数签名|
|---    |---     |
| bool  | has_suffix(const str& s) const        |
| bool  | has_suffix(const_pointer str) const   |
| bool  | has_suffix(value_type c) const        |
| bool  | has_prefix(const str& s) const        |
| bool  | has_prefix(const_pointer s) const     |
| bool  | has_prefix(value_type c) const        |
| bool  | remove_prefix(const str& s)           |
| bool  | remove_prefix(const_pointer s)        |
| bool  | remove_prefix(value_type c)           |
| bool  | remove_suffix(const str& s)           |
| bool  | remove_suffix(const_pointer s)        |
| bool  | remove_suffix(value_type c)           |

#### 填充

| 返回值 | 函数签名|
|---    |---     |
| str&  | fill(value_type ch)                                |
| str&  | fill(pos_type pos, value_type ch, size_type n)     |

#### 查找

| 返回值 | 函数签名|
|---    |---     |
| pos_type | index_of(const str& str, pos_type from = 0) const                                          |
| pos_type | index_of(const_pointer s, pos_type from = 0) const                                      |
| pos_type | index_of(value_type ch, pos_type from = 0) const                                            |
| pos_type | index_of(const re& rx, pos_type from = 0) const                                            |
| pos_type | index_of(re& rx, pos_type from = 0) const                                                  |
| pos_type | index_of(std::function<int(value_type c, bool& match)> func, pos_type from = 0) const       |
| pos_type | last_index_of(const str& str, pos_type from = -1) const                                    |
| pos_type | last_index_of(value_type ch, pos_type from = -1) const                                      |
| pos_type | last_index_of(const_pointer str, pos_type from = -1) const                              |
| pos_type | last_index_of(const re& rx, pos_type from = -1) const                                      |
| pos_type | last_index_of(re& rx, pos_type from = -1) const                                            |
| pos_type | last_index_of(std::function<int(value_type c, bool& match)> func, pos_type from = -1) const |

#### 匹配检测

| 返回值 | 函数签名|
|---    |---     |
| bool  | is_match(re& rx) const                         |
| bool  | is_match(const str& pattern) const             |
| bool  | is_match(const_pointer& pattern) const      |
| bool  | is_match_wild(const str& pattern) const        |
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
| bool  | is_printable() const      |
| bool  | is_identifier() const     |             
| bool  | is_numeric() const        |
| bool  | is_bool() const        |

#### 提取子串

| 返回值 | 函数签名|
|---    |---     |
| str   | left(size_type n) const                       |
| str   | right(size_type n) const                      |
| str   | substr(pos_type pos, int offset_n = -1) const |

#### 限宽对齐

| 返回值 | 函数签名|
|---    |---     |
| str&  | ljust(size_type width, value_type fill = ' ', bool truncate = false)  |
| str&  | rjust(size_type width, value_type fill = ' ', bool truncate = false)  |
| str&  | center(size_type width, value_type fill = ' ', bool truncate = false) |
| str&  | zfill(size_type width, value_type fill = ' ', bool truncate = false)  |

#### 子串替换

| 返回值 | 函数签名|
|---    |---     |
| str&  | replace(pos_type pos, size_type n, const str& after)                               |
| str&  | replace(pos_type pos, size_type n, value_type after)                               |
| str&  | replace(pos_type pos, size_type n, const_pointer unicode, size_type size)          |
| str&  | replace(value_type before, value_type after)                                       |
| str&  | replace(const_pointer before, size_type blen, const_pointer after, size_type alen) |
| str&  | replace(const str& before, const str& after)                                       |
| str&  | replace(value_type ch, const str& after)                                           |
| str&  | replace(const re& rx, const str& after)                                            |
| str&  | replace(std::function<int(value_type key, value_type& val)> func)                  |

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
| str&  | inversion() |

#### 分割子串

| 返回值 | 函数签名|
|---    |---     |
| std::vector<str> | split(const str& sep) const                                                |
| std::vector<str> | split(const_pointer sep) const                                             |
| std::vector<str> | split(value_type sep) const                                                |
| std::vector<str> | split(const re& r) const                                                   |
| std::vector<str> | split(std::function<int(value_type c, bool& match)>& chars_func) const     |
| std::vector<str> | split_lines(bool keep_ends = false) const                                  |
| std::vector<str> | split_path() const                                                         |
| void             | split(const str& sep, std::function<int(const_pointer s, size_type n)> output_func) const      |
| void             | split(const_pointer sep, std::function<int(const_pointer s, size_type n)> output_func) const   |
| void             | split(value_type sep, std::function<int(const_pointer s, size_type n)> output_func) const      |
| void             | split(const re& r, std::function<int(const_pointer s, size_type n)> output_func) const         |
| void             | split(std::function<int(value_type c, bool& match)>& chars_func, std::function<int(const_pointer s, size_type n)> output_func) const    |
| void             | split_lines(bool keep_ends, std::function<int(const_pointer s, size_type n)> output_func) const    |
| void             | split_path(std::function<int(const_pointer s, size_type n)> output_func) const                     |

#### 大小写转换

| 返回值 | 函数签名|
|---    |---     |
| str&  | to_lower()    |
| str&  | to_upper()    |
| str&  | swap_case()   |
| str&  | title()       |

#### 空白处理

| 返回值 | 函数签名|
|---    |---     |
| str&  | simplified()  |
| str&  | ltrim()       |
| str&  | rtrim()       |
| str&  | trim()        |

#### 模板展开

| 返回值 | 函数签名|
|---    |---     |
| str   | expand(const std::map<str, str>& kvs) const                               |
| str   | expand(std::function<int(const str& key, str& val)> provider) const       |
| str   | expand_envs(const_pointer key, const_pointer val) const                   |
| str   | expand_envs() const                                                       |
| str   | expand_tabs(size_type tab_size = 8) const                                 |
| str   | expand_tmpl(const std::map<str, str>& kvs) const                          |
| str   | expand_tmpl(std::function<int(const str& key, str& val)> provider) const  |


#### 路径处理

| 返回值 | 函数签名|
|---    |---     |
| str   | basename() const      |
| str&  | basename()            |
| str   | dirname() const       |
| str&  | dirname()             |

#### 类型互转

| 返回值 | 函数签名|
|---    |---     |
| bool  | to_bool(bool* ok = nullptr) const |
| str&  | assign(bool v)                    |
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
| str&      | assign(double n, value_type format = 'g', int precision = 6)      |
| str&      | assign(float n, value_type format = 'g', int precision = 6)       |
| str&      | assign(int8_t n, int base = 10)                                   |
| str&      | assign(int16_t n, int base = 10)                                  |
| str&      | assign(int32_t n, int base = 10)                                  |
| str&      | assign(int64_t n, int base = 10)                                  |
| str&      | assign(uint8_t n, int base = 10)                                  |
| str&      | assign(uint16_t n, int base = 10)                                 |
| str&      | assign(uint32_t n, int base = 10)                                 |
| str&      | assign(uint64_t n, int base = 10)                                 |

#### 计算 hash

| 返回值 | 函数签名|
|---    |---     |
| int32_t | hash_code() const |

#### 拷贝和交换

| 返回值 | 函数签名|
|---    |---     |
| size_type  | copy(pointer dest, size_type n, pos_type pos = 0) const |
| void       | swap(str& other)       |

#### 新串生成

| 返回值 | 函数签名|
|---    |---     |
| str   | repeated(size_type times) const           |
| str   | join(const std::vector<str>& s) const     |
| str   | join(const str& s, ...) const             |
| str   | join(const_pointer s, ...) const          |

#### 数字转换为字符串

| 返回值 | 函数签名|
|---    |---     |
| static str | number(double n, value_type format = 'g', int precision = 6)     |
| static str | number(float n, value_type format = 'g', int precision = 6)      |
| static str | number(int8_t n, int base = 10)                                  |
| static str | number(int16_t n, int base = 10)                                 |
| static str | number(int32_t n, int base = 10)                                 |
| static str | number(int64_t n, int base = 10)                                 |
| static str | number(uint8_t n, int base = 10)                                 |
| static str | number(uint16_t n, int base = 10)                                |
| static str | number(uint32_t n, int base = 10)                                |
| static str | number(uint64_t n, int base = 10)                                |

####  运算符重载

| 返回值 | 函数签名|
|---    |---     |
| str& | operator=(value_type ch)                       |
| str& | operator=(const_pointer str)                   |
| str& | operator+=(const_pointer str)                  |
| str& | operator+=(value_type ch)                      |
| str& | operator+=(const str& other)                   |
| bool | operator!=(const_pointer other) const          |
| bool | operator<(const_pointer other) const           |
| bool | operator<=(const_pointer other) const          |
| bool | operator==(const_pointer other) const          |
| bool | operator>(const_pointer other) const           |
| bool | operator>=(const_pointer other) const          |
| str::value_type&       | operator[](pos_type pos)         |
| const str::value_type& | operator[](pos_type pos) const   |



