#!tools/gendoc -o docs/FAQ.md

#### 为什么没有用 googletest 等测试框架？

测试框架也是自己写的，参见项目 

#### 为什么有时候 `str::dirname` 与 `std::dirname_view` 的返回值不同？

这其实是一个很无奈的设计：
`str::dirname_view(s)` 函数返回的数据类型是 `std::string_view`。
而 `str::dirname(s)` 函数返回值是 `std::string` 类型。
`str::dirname_view(s)` 必须确保返回值必须是输入串的子串，但 `str::dirname(s)` 却没有这个限制。 

假设表示路径的字符串 s 被赋值为 `std::string s = "foo.txt";` ， 
显然其目录名（即 dirname）应该是 `"."`。
然而 `"."` 并不是 `"foo.txt"` 的子串， 这导致 `str::dirname_view(s)` 没有办法返回 `"."`。

所以，`str::dirname` 与 `std::dirname_view` 的功能并非总是一致的。
大部分场景下，我们应该使用 `str::dirname` 而非 `std::dirname_view`，`str::dirname` 
总是确保返回一个有意义的字符串路径，它不仅更安全，也不容易遇到一些边角场景的错误。
