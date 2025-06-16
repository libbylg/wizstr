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
#include "testing.hpp"

#include "str.hpp"

#include <complex>

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, next_opt1) {
    GROUP("容器模式") {
        SECTION("正常情况") {
            str::size_type pos = 0;
            auto argv = std::vector{"-a=va", "-b", "--", "-vvv", "ccc"};
            ASSERT_EQ(str::next_opt1(pos, argv), (std::tuple{"-a", "va"}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt1(pos, argv), (std::tuple{"-b", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt1(pos, argv), (std::tuple{"", "-vvv"}));
            ASSERT_EQ(pos, 4);
            ASSERT_EQ(str::next_opt1(pos, argv), (std::tuple{"", "ccc"}));
            ASSERT_EQ(pos, 5);
            ASSERT_FALSE(str::next_opt1(pos, argv));
            ASSERT_EQ(pos, 5);
            ASSERT_FALSE(str::next_opt1(pos, argv));
            ASSERT_EQ(pos, 5);
        }
        SECTION("起点超出范围") {
            str::size_type pos = 0;
            auto argv = std::vector{"-a=va", "-b", "--", "-vvv", "ccc"};
            ASSERT_FALSE(str::next_opt1(pos = 5, argv));
            ASSERT_EQ(pos, 5);
            ASSERT_FALSE(str::next_opt1(pos = 6, argv));
            ASSERT_EQ(pos, 5);
            ASSERT_FALSE(str::next_opt1(pos = str::npos, argv));
            ASSERT_EQ(pos, 5);
        }
        SECTION("空容器") {
            str::size_type pos = 0;
            auto argv = std::vector<std::string>{};
            ASSERT_FALSE(str::next_opt1(pos = 0, argv));
            ASSERT_EQ(pos, 0);
        }
        SECTION("空选项") {
            str::size_type pos = 0;
            auto argv = std::vector{"-a=va", ""};
            ASSERT_EQ(str::next_opt1(pos = 0, argv), (std::tuple{"-a", "va"}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt1(pos, argv), (std::tuple{"", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_FALSE(str::next_opt1(pos, argv));
            ASSERT_EQ(pos, 2);
        }
        SECTION("转义:转义后为空") {
            str::size_type pos = 0;
            auto argv = std::vector{"-k", "--"};
            ASSERT_EQ(str::next_opt1(pos = 0, argv), (std::tuple{"-k", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt1(pos, argv), (std::tuple{"", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_FALSE(str::next_opt1(pos, argv));
            ASSERT_EQ(pos, 2);
        }
    }
    GROUP("迭代器模式") {
        SECTION("正常情况") {
            auto argv = std::vector{"-a=va", "-b", "--", "-vvv", "ccc"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt1(itr, argv.end()), (std::tuple{"-a", "va"}));
            ASSERT_EQ(str::next_opt1(itr, argv.end()), (std::tuple{"-b", ""}));
            ASSERT_EQ(str::next_opt1(itr, argv.end()), (std::tuple{"", "-vvv"}));
            ASSERT_EQ(str::next_opt1(itr, argv.end()), (std::tuple{"", "ccc"}));
            ASSERT_FALSE(str::next_opt1(itr, argv.end()));
            ASSERT_FALSE(str::next_opt1(itr, argv.end()));
        }
        SECTION("起点就是end") {
            auto argv = std::vector{"-a=va", "-b", "--", "-vvv", "ccc"};
            auto itr = argv.end();
            ASSERT_FALSE(str::next_opt1(itr, argv.end()));
            ASSERT_FALSE(str::next_opt1(itr, argv.end()));
        }
        SECTION("空容器") {
            auto argv = std::vector<std::string>{};
            auto itr = argv.begin();
            ASSERT_FALSE(str::next_opt1(itr, argv.end()));
            ASSERT_FALSE(str::next_opt1(itr, argv.end()));
        }
    }
    GROUP("数组+char*") {
        SECTION("一般情况:数组+char*") {
            int pos = 0;
            auto items = std::vector<std::string>{"-a=va", "-b", "--", "-vvv", "ccc"};
            char* argv[5]{items[0].data(), items[1].data(), items[2].data(), items[3].data(), items[4].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt1(pos, argc, argv), (std::tuple{"-a", "va"}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt1(pos, argc, argv), (std::tuple{"-b", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt1(pos, argc, argv), (std::tuple{"", "-vvv"}));
            ASSERT_EQ(pos, 4);
            ASSERT_EQ(str::next_opt1(pos, argc, argv), (std::tuple{"", "ccc"}));
            ASSERT_EQ(pos, 5);
            ASSERT_FALSE(str::next_opt1(pos, argc, argv));
            ASSERT_EQ(pos, 5);
            ASSERT_FALSE(str::next_opt1(pos, argc, argv));
            ASSERT_EQ(pos, 5);
        }
        SECTION("起点超出范围") {
            int pos = 0;
            auto items = std::vector<std::string>{"-a=va", "-b", "--", "-vvv", "ccc"};
            char* argv[5]{items[0].data(), items[1].data(), items[2].data(), items[3].data(), items[4].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_FALSE(str::next_opt1(pos = 5, argc, argv));
            ASSERT_EQ(pos, 5);
            ASSERT_FALSE(str::next_opt1(pos = 6, argc, argv));
            ASSERT_EQ(pos, 5);
            ASSERT_FALSE(str::next_opt1(pos = std::numeric_limits<int>::max(), argc, argv));
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(str::next_opt1(pos = -1, argc, argv), (std::tuple{"-a", "va"}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt1(pos = std::numeric_limits<int>::min(), argc, argv), (std::tuple{"-a", "va"}));
            ASSERT_EQ(pos, 1);
        }
        SECTION("指定数组长度为0和负值") {
            int pos = 0;
            auto items = std::vector<std::string>{"-a=va", "-b", "--", "-vvv", "ccc"};
            char* argv[5]{items[0].data(), items[1].data(), items[2].data(), items[3].data(), items[4].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_FALSE(str::next_opt1(pos = 0, argc=0, argv));
            ASSERT_EQ(pos, 0);
            ASSERT_FALSE(str::next_opt1(pos = -15, argc=-10, argv));
            ASSERT_EQ(pos, 0);
            ASSERT_FALSE(str::next_opt1(pos = 0, argc=-1, argv));
            ASSERT_EQ(pos, 0);
        }
    }
    GROUP("数组+const char*") {
        SECTION("一般情况") {
            int pos = 0;
            const char* argv[5] = {"-a=va", "-b", "--", "-vvv", "ccc"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt1(pos, argc, argv), (std::tuple{"-a", "va"}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt1(pos, argc, argv), (std::tuple{"-b", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt1(pos, argc, argv), (std::tuple{"", "-vvv"}));
            ASSERT_EQ(pos, 4);
            ASSERT_EQ(str::next_opt1(pos, argc, argv), (std::tuple{"", "ccc"}));
            ASSERT_EQ(pos, 5);
        }
        SECTION("起点超出范围") {
            int pos = 0;
            const char* argv[5] = {"-a=va", "-b", "--", "-vvv", "ccc"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_FALSE(str::next_opt1(pos = 5, argc, argv));
            ASSERT_EQ(pos, 5);
            ASSERT_FALSE(str::next_opt1(pos = 6, argc, argv));
            ASSERT_EQ(pos, 5);
            ASSERT_FALSE(str::next_opt1(pos = std::numeric_limits<int>::max(), argc, argv));
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(str::next_opt1(pos = -1, argc, argv), (std::tuple{"-a", "va"}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt1(pos = std::numeric_limits<int>::min(), argc, argv), (std::tuple{"-a", "va"}));
            ASSERT_EQ(pos, 1);
        }
        SECTION("指定数组长度为0和负值") {
            int pos = 0;
            const char* argv[5] = {"-a=va", "-b", "--", "-vvv", "ccc"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_FALSE(str::next_opt1(pos = 0, argc=0, argv));
            ASSERT_EQ(pos, 0);
            ASSERT_FALSE(str::next_opt1(pos = -15, argc=-10, argv));
            ASSERT_EQ(pos, 0);
            ASSERT_FALSE(str::next_opt1(pos = 0, argc=-1, argv));
            ASSERT_EQ(pos, 0);
        }
        SECTION("null数组") {
            int pos = 0;
            const char** argv = nullptr;
            int argc = 5;
            ASSERT_FALSE(str::next_opt1(pos = 0, argc=5, argv));
            ASSERT_EQ(pos, 0);
        }
    }
}
