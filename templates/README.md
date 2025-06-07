#!tools/gendoc -o README.md
<a href='https://gitee.com/libbylg/str/stargazers'>
<img src='https://gitee.com/libbylg/str/badge/star.svg?theme=dark' alt='star'/>
</a>

# str

## 简介

str 库提供了一系列字符串处理函数算法，str 的目标是希望能成为 C++ 中，功能最丰富的字符串处理函数库。

str 库聚焦丰富的功能、完善的测试、完备的接口设计。

当前 str 库基于 `std::string` 和 `std::string_view`，提供了下面的功能。（详情可参考文档 [docs/MANUAL.md](#)）

* 批量追加插入（append、insert）
* 大小写不敏感的比较（icompare）
* 通配符匹配（wild）
* 查找及包含关系检测（next_xxx、prev_xxx、contains）
* 特征字符串统计（count）
* 前后缀操作（prefix、suffix）
* 特征测试（is_xxx）
* 子串提取（take、drop）
* 修剪和整形（trim、surround、align、invert、simplified）
* 按多行处理（lines）
* 按词处理（word）
* 字符串生成（make）
* 空白串处理（space）
* 字符串遮罩（cover）
* 字符串拆分（split）
* 字符串拼接（join）
* 大小写转换（to_xxx）
* 变量或者特殊符号展开（expand）
* 文件名路径操作（dirname、basename、extname、rawname）
* 字符串转义（encode、decode）
* 文本文件逐行或者全量读取（read_xxx）
* 字符分组和筛选（grouping、filter）
* 十六进制数据dump（dump_hex）

## 示例

```c++ 
@embded(file:templates/example1.cpp:readme-example)
```

输出结果如下：

```text
@embded(exec-stdout:"templates/example1")
```

## 构建

**对外依赖**

* C++标准要求：至少为 `C++17`
* 操作系统：
  绝大多数函数适用于任何操作系统，部分路径处理函数只支持 `*nix` 系统，含 `msys2` 和 `mingw`
* 编译器：
  * gcc：支持
  * clang：支持
  * msvc：支持
  * 其他：不确定，因无条件测试验证，所以如果您使用更低版本 cmake 成功通过编译和测试，恳请告诉我（可以提 issue 反馈）。
* cmake：
  至少为 `3.24`，理论上更低版本也可行，但并未在更低版本测试过。
  如果您使用更低版本 cmake 成功通过编译和测试，恳请告诉我（可以提 issue）。 

**如何编译**

* 方式1：通过 `cmake` 编译 (适合任何环境)

```shell
mkdir build
cd build
cmake ..
make test
make install
````

如果需要指定安装路径可以为 cmake 命令指定 `CMAKE_INSTALL_PREFIX` 参数：

```shell
cmake .. -DCMAKE_INSTALL_PREFIX=<InstallTargetDirectory>
```

其中，`<InstallTargetDirectory>` 这里需要替换给您自己的安装目录。

关于 `CMAKE_INSTALL_PREFIX` 的含义和用法用法可以参考 cmake 官方文档。

* 方式2：通过 `build.sh` (适合 `*nix` 系统，含 `msys2` 和 `mingw`)

```shell
sh build.sh compile
sh build.sh test
sh build.sh install
```

如果需要指定安装路径可以通过调整 install 阶段的命令:

```shell
sh build.sh install -i <InstallTargetDirectory>`
```

其中，`<InstallTargetDirectory>` 这里需要替换给您自己的安装目录。

## 文档

<!--
**快速入门**

本项目已经被集成到 xxx，可以在在线编辑器中直接使用
-->

**阅读文档：**

当前文档正在装修，现阶段可直接参考 [include/str.hpp](include/str.hpp) 中的文档注释。

如果您发现文档中的问题，欢迎提 issue 或者 PR。

**手动编译文档：**

有时候，没有办法在线阅读文档，但如果您在本地有源码，可以自行在本地编译出文档。str 的文档需要用基于 str 库写的文档编译工具来生成。这都可以通过命令完成。具体方式如下：

```shell
sh build.sh doc
```

编译完成之后，您可以直接在查看 `docs/MANUAL.md` 文件。

**示例代码：**

str 中每个已经提供的函数都经历了尽可能充分的测试，所以没有什么比看测试用例更好的办法了。参见 [test](tests) 目录，基本上每个文件都对应一个函数，你很容易找到对应的使用案例，甚至一些极其生僻的调用方式。

## 如何求助和参与贡献

#### 寻求帮助?

  - 首先，您可以在 [docs/FAQ.md](#) 中检索你遇到的问题，看看是否有现成的解决方案。
  - 其次，您可以通过 StackOverflow 提问，但注意带上 `gitee-str` 标签；
  - 最后，您可以通过提 [issue](https://gitee.com/libbylg/str/issues) 来直接向维护人员寻求帮助，但请带上 `help` 标签的；

#### 发现 bug?

恳请提 [issue](https://gitee.com/libbylg/str/issues)，并附上 **最小代码示例** 
以及问题现象以及您的预期。 如果能附上您的解决思路的话，那更不胜感激！

#### 新想法?

可以通过 issue 反馈您的新想法，并附上**您期望的接口设计**，以及 **最小用法示例**。

> 需要注意：新想法不一定能采纳，这主要取决于这个想法是否与当前项目的目标一致以及与现有接口是否会产生冲突。最后，更关键的是项目维护者的业余时间是否充沛。很多时候如果能 **直接贡献代码** 将更受欢迎^_^! 

#### 直接贡献代码?

如果您解决了某个问题或者实现了新的功能，并希望您的解决方法能合并到主线版本，您可以发起新的 PR。

## 贡献者名单

虚位以待

## 用户荣誉榜

| 公司  | 产品  | 备注 |
|-----|-----|----|
| Hxx | Pxx |    |

## 请作者喝杯奶茶

<!--
TODO
-->

## 关联项目：

* [Framework-Less-Testing](https://gitee.com/libbylg/Framework-Less-Testing)
