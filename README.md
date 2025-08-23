<a href='https://gitee.com/libbylg/wizstr/stargazers'>
<img src='https://gitee.com/libbylg/wizstr/badge/star.svg?theme=dark' alt='star'/>
</a>

* 源码仓库地址：

  - `GitHub`: [https://github.com/libbylg/wizstr](https://github.com/libbylg/wizstr)
  - `Gitee`: [https://gitee.com/libbylg/wizstr](https://gitee.com/libbylg/wizstr)

* 下载地址：

  - `GitHub`: [https://github.com/libbylg/wizstr/releases](https://github.com/libbylg/wizstr/releases)
  - `Gitee`: [https://gitee.com/libbylg/wizstr/releases](https://gitee.com/libbylg/wizstr/releases)

# wizstr

## 简介

wizstr 库提供了一系列字符串处理函数算法，wizstr 的目标是希望能成为 C++ 中，功能最丰富的字符串处理函数库。

wizstr 库聚焦丰富的功能、完善的测试、完备的接口设计。

当前 wizstr 库基于 `std::string` 和 `std::string_view`，提供了下面的功能。（详情可参考文档 [MANUAL.html](https://libbylg.github.io/wizstr/docs/MANUAL.html)）

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

## 使用示例

```c++ 
#include <iostream>

#include "str.hpp"

int main() {
    // split&join
    auto items = str::split("Welcome to use wizstr library");
    std::cout << "items: [" << str::join_list(items) << "]" << std::endl;

    // path
    auto dirname = str::dirname("/home/sam/project/main.cpp");
    auto basename = str::basename("/home/sam/project/main.cpp");
    auto fullpath = str::join_path({dirname, basename});
    std::cout << "dirname: " << dirname << std::endl;
    std::cout << "basename: " << basename << std::endl;
    std::cout << "fullpath: " << fullpath << std::endl;

    // trim
    std::string_view s = "  Hello  World  !  ";
    std::cout << "trim_left: [" << str::trim_left(s) << "]" << std::endl;
    std::cout << "trim_right: [" << str::trim_right(s) << "]" << std::endl;
    std::cout << "trim_surrounding: [" << str::trim_surrounding(s) << "]" << std::endl;
    std::cout << "simplified: [" << str::simplified(s) << "]" << std::endl;

    // dump_hex
    std::string k = "This function is used to dump binnary data to hex string like xxd";
    str::dump_hex(k.data(), k.size(), str::dump_hex_format{.flags = (str::show_ascii | str::show_offset)},
        [](size_t, std::string_view line_text) {
            std::cout << line_text << std::endl;
            return 0;
        });

    return 0;
}
```

输出结果如下：

```text
items: [Welcome,to,use,wizstr,library]
dirname: /home/sam/project
basename: main.cpp
fullpath: /home/sam/project/main.cpp
trim_left: [Hello  World  !  ]
trim_right: [  Hello  World  !]
trim_surrounding: [Hello  World  !]
simplified: [Hello World !]
00: 54686973 2066756e 6374696f 6e206973 This function is
10: 20757365 6420746f 2064756d 70206269  used to dump bi
20: 6e6e6172 79206461 74612074 6f206865 nnary data to he
30: 78207374 72696e67 206c696b 65207878 x string like xx
40: 64                                  d               
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
make install
````

如果需要指定安装路径可以为 cmake 命令指定 `CMAKE_INSTALL_PREFIX` 参数：

```shell
cmake .. -DCMAKE_INSTALL_PREFIX=<InstallTargetDirectory>
```

其中，`<InstallTargetDirectory>` 这里需要替换给您自己的安装目录。

**注意：** 关于 `CMAKE_INSTALL_PREFIX` 的含义和用法用法可以参考 cmake 官方文档。

* 方式2：通过 `build.sh` (适合 `*nix` 系统，含 `msys2` 和 `mingw`)

```shell
sh build.sh compile
sh build.sh install
```

如果需要指定安装路径可以通过调整 install 阶段的命令:

```shell
sh build.sh install <InstallTargetDirectory>`
```

其中，`<InstallTargetDirectory>` 这里需要替换给您自己预期的安装目录。

## 集成

#### 方式1：直接源码集成

这种方式就是直接将 `include/str_config.hpp`, `include/str.hpp` 和 `src/str.cpp` 这三个文件直接拷贝到您的项目中去，
并随项目的其他代码一起编译。需要注意，`include/str_config.hpp` 在源码仓中并不存在，如果采用源码方式集成，这个文件
需要手工生成。其内容可参照下面的代码修改：

```c++
#ifndef STR_CONFIG_H
#define STR_CONFIG_H
// #define WIZSTR_NAMESPACE MyStr  
#endif
```

其中，如果取消 `#define WIZSTR_NAMESPACE MyStr` 这一行的注释，表示您想为本 wizstr 库指定一个名字空间。你可以将 `MyStr` 
替换为您喜欢的名字空间名字即可。

**注意：** 这种方式并不是很推荐，主要是一旦采用这种方式，很容易对 wizstr 的这部分代码造成侵入式修改，
最终会导致丧失简单即可升级 wizstr 库到新版本的能力。 
考虑到某些特殊的闭源项目或者存在网络隔离环境的项目必须这样做，那么我们仍然建议您做好代码隔离，
当需要对 wizstr 的代码做修改时，也尽可能用打 patch 的方式来修改，而非直接修改 wizstr 的源码。

#### 方式2：链接 wizstr 的静态库或者动态库

这种方式，需要您先完成编译和安装（参见 "构建" 小节）。
剩余的就是配置 Makefile 使得编译器和连接器能正确找到并使用 wizstr 库的头文件和动态或者静态库。

下面，我们假设您已经将 wizstr 库安装到了 ${HOME}/cpp/depends/wizstr 下了，
那么，接着您还需要在您自己的软件的编译和连接脚本中增加下面的处理：

* 指定 wizstr 库的头文件的搜索路径：

gcc 和 clang 中一般是 `-I${HOME}/cpp/depends/wizstr/include`

* 指定 wizstr 库的库文件的搜索路径：

gcc 和 clang 中一般是 `-L${HOME}/cpp/depends/wizstr/lib`

* 指定链接的 wizstr 库的名称：

gcc 和 clang 中，链接动态库一般是直接 `-lwizstr`，如果连接静态库一般是 `-llibwizstr.a`


#### 方式3：使用 cmake 自动发现和引入 wizstr 库

这种方式在您使用 cmake 才是可行的，假设您的项目的的某个名字为 MyApp 的构建目标需要用到 wizstr 库。
那么，配置方式如下：

* 用想使用动态库：

```cmake
find_package(wizstr)
# ... 
target_link_libraries(MyApp PRIVATE wizstr::wizstr-shared)
```

* 用想使用静态库：

```cmake
find_package(wizstr)
# ... 
target_link_libraries(MyApp PRIVATE wizstr::wizstr-static)
```

## 关于名字空间

wizstr 库中所有的函数都是 `struct str` 的静态成员函数。这种方式的优点是：你无法绕过 `str` 类名来调用其任何成员函数。
相对于 `namespace str` 来说更加严格。这主要是因为 wizstr 库中有不少函数已知与一些其他的 C/C++ 库存在名字冲突。
然而，`str` 本身也可能与用户的某些类名相同。因此，权衡考虑，允许 `wizstr` 库的用户在编译阶段指定一个自定义的名字空间
将有助于更灵活地解决类名冲突问题。具体方法如下：

* 如果采用源码集成，此时不涉及对 wizstr 库的单独编译，此时可以自行通过 `include/str_config.hpp` 文件类指定；
* 如果采用 cmake 命令来编译，可以通过增加 cmake 命令行参数 `-DWIZSTR_NAMESPACE=xxx` 的方式来指定名字空间名称（注意 xxx 需要替换成您期望的名字空间名）；
* 如果采用 build.sh 命令来编译，当前不支持定制名字空间名称。

## LICENSE

BSD-like License ： [木兰宽松许可v2（MulanPSL2）](LICENSE)

## 文档&试玩

#### Playground 试玩

正在装修中...

#### 阅读文档

wizstr 库的文档在 `docs` 目录下。如果想在线查看，可以参考页面：https://libbylg.github.io/wizstr/docs/MANUAL.html

**注意：** 如果您发现文档中的问题，欢迎提 issue 或者 PR。

## 如何求助和参与贡献

#### 寻求帮助?

- 首先，如果只是不知道某个函数的用法，wizstr 库的用例十分丰富，没有什么查看 wizstr 的用例更好的办法了；
- 其次，如果您想自己上手写一些试用代码，wizstr 的 playgroud 应该是你需要的；不过 playground 当前正在装修，可能还要等一段时间；
- 接收，您可以在 [FAQ](https://libbylg.github.io/wizstr/docs/FAQ.html) 中检索你遇到的问题，看看是否有现成的解决方案；
- 最后，您可以通过提 [github-issue](https://github.com/libbylg/wizstr/issues) 或者 [gitee-issue](https://gitee.com/libbylg/wizstr/issues) 来直接向维护人员寻求帮助；

#### 发现 bug?

恳请提 [github-issue](https://github.com/libbylg/wizstr/issues) 或者 [gitee-issue](https://gitee.com/libbylg/wizstr/issues)，并附上 **最小代码示例**
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

装修中...

<!--
TODO
-->

## 关联项目：

* [Framework-Less-Testing](https://gitee.com/libbylg/Framework-Less-Testing)
