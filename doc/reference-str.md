[TOC]

# 简介

str 库实现了一个简单、好用的字符串处理基础设施。用于解决 C++ 开发团队中尝尝面临不想依赖一些重型库，但又没有好用的字符串处理工具（类）的问题。str 库全库只有一个类，由一系列静态成员函数组成。其主要接口，参考了 C++STL、Qt、Folly、golang、Java、Python、Kotlin 等库或者语言中的字符串处理工具的接口设计。我们力求做到 C++ 开发者所理解的好用。

# 字符串操作分类

* 字符串比较、匹配、统计
* 字符串内的子串查找和定位
* 字符串整形、变形
* 字符串拆分、合并
* 字符串的生成
* 不同应用场景的特殊诉求：
  - 字符串作为文件路径
  - 不同场景下的编码和解码

### 字符串拆分

#### 以单一字符为分隔符拆分字符串

|声明| 备注 | 版本 |
|--- |---- |----  |
|`auto split(std::string_view s, const char_match_proc& sepset, size_type max_n, const view_consumer_proc& proc) -> void` | (1) | |
|`auto split(std::string_view s, const charset_type& sepset, size_type max_n, const view_consumer_proc& proc) -> void`    | (2) | |
|`auto split(std::string_view s, const charset_type& sepset, const view_consumer_proc& proc) -> void`                     | (3) | |
|`auto split(std::string_view s, const charset_type& sepset, size_type max_n = npos) -> std::vector<std::string_view>`    | (4) | |
|`auto split(std::string_view s, std::string_view sepset, size_type max_n, const view_consumer_proc& proc) -> void`       | (5) | |
|`auto split(std::string_view s, std::string_view sepset, const view_consumer_proc& proc) -> void`                        | (6) | |
|`auto split(std::string_view s, std::string_view sepset, size_type max_n = npos) -> std::vector<std::string_view>`       | (7) | |
|`auto split(std::string_view s, const view_consumer_proc& proc) -> void`                                                 | (8) | |
|`auto split(std::string_view s, size_type max_n = str::npos) -> std::vector<std::string_view>`                           | (0) | |



|参数名|含义|
|---|---|
| `s` |被拆分的原始字符串。|
| `sepset` |可用作分隔符的字符集。|
| `max_n` |执行最大拆分次数。如果为 0，直接返回原始字符串。如果为 npos 表示。|
| `proc` |用于执行接受输出。|

