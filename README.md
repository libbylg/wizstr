<a href='https://gitee.com/libbylg/str/stargazers'><img src='https://gitee.com/libbylg/str/badge/star.svg?theme=dark' alt='star'></img></a>

# str

#### 介绍

str 库提供了一系列字符串处理函数算法，目标是成为 C++ 中功能最丰富的字符串处理函数库。

#### 安装教程

**对外依赖**

* C++标准要求：至少为 `C++17`
* 操作系统：
  绝大多数函数适用于任何操作系统，部分路径处理函数只支持 `*nix` 系统，含 `msys2` 和 `mingw`
* 编译器：
  * gcc：支持
  * clang：支持
  * msvc：支持
  * 其他：不确定，因无条件测试验证，所以如果您使用更低版本 cmake 成功通过编译和测试，恳请告诉我（可以提 issue）。
* cmake：
  至少为 `3.24`，理论上更低版本也可行，但并未在更低版本测试过。
  如果您使用更低版本 cmake 成功通过编译和测试，恳请告诉我（可以提 issue）。 

**如何编译**

* 方式1：通过 cmake (适合任何环境)

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

* 方式2：通过 build.sh (适合 `*nix` 系统，含 `msys2` 和 `mingw`)

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

#### 使用说明

<!--
**快速入门**

本项目已经被集成到 xxx，可以在在线编辑器中直接使用
-->

**文档：**

当前文档正在装修，现阶段可直接参考 [include/str.hpp](include/str.hpp) 中的文档注释。

如果您发现文档中的问题，欢迎提 issue 或者 PR。

#### 如何求助和参与贡献

* 求助?

欢迎通过 xxx 的 xxx 频道提问，让我们互相帮助。

* 发现 bug?

恳请提 issue，并附上**最小代码示例**，以及问题现象以及您的预期。
如果能附上您的解决思路的话，那更是不胜感激！

* 新想法?

可以通过 issue 反馈您的新想法，并附上**您期望的接口设计**，
以及 **最小用法示例**。

需要注意：新想法不一定能采纳，这主要取决于这个想法是否与
当前项目的目标一致以及与现有接口是否冲突。以及项目维护者
的业余时间是否充沛。很多时候如果能 **直接贡献代码** 将更
受欢迎^_^!

* 直接贡献代码?

如果您解决了某个问题或者实现了新的功能，并希望您的解决方法能合并到
主线版本，您可以发起新的 PR。

#### 贡献者名单

* libbylg@126.com 
* 