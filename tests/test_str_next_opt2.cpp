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

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, next_opt2) {
    GROUP("容器模式") {
        SECTION("场景：-key value") {
            str::size_type pos = 0;
            auto argv = std::vector{"-a", "va"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-a", "va"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-key") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"-a"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-a", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
        }
        SECTION("场景：-key1 -key2") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"-key1", "-key2"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-key1", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-key2", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-key -") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"-a", "-"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-a", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-key --") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"-a", "--"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-a", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-key - value") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"-a", "-", "-va"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-a", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-va", ""}));
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
        }
        SECTION("场景：-key -- value") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"-a", "--", "-va"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-a", "-va"}));
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
        }
        SECTION("场景：value1") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"va", "vb"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"", "va"}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"", "vb"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-- value") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"--", "va"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"", "va"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：- value") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"-", "va"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-", "va"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：value --") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"va", "--"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"", "va"}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：- value") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"-", "va"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-", "va"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：- -") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"-", "-", "v1"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"", "v1"}));
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
        }
        SECTION("场景：-- --") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"--", "--"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"", "--"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：- --") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"-", "--"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-- -") {
            int pos = 0;
            auto argv = std::vector<std::string>{"--", "-"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"", "-"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"-"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"-", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
        }
        SECTION("场景：--") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"--"};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
        }
        SECTION("场景：字段为空") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"", ""};
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argv), (std::tuple{"", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：空列表") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{};
            ASSERT_EQ(str::next_opt2(pos, argv), std::nullopt);
            ASSERT_EQ(pos, 0);
        }
        SECTION("场景：起始位置超过容量") {
            size_t pos = 0;
            auto argv = std::vector<std::string>{"aa", "bb"};
            ASSERT_EQ(str::next_opt2(pos=2, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos=3, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos=str::npos, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            // ASSERT_EQ(str::next_opt2(pos=-1, argv), (std::tuple{"", "aa"}));
            // ASSERT_EQ(pos, 1);
            // ASSERT_EQ(str::next_opt2(pos=std::numeric_limits<decltype(argc)>::min(), argc, argv), (std::tuple{"", "aa"}));
            // ASSERT_EQ(pos, 1);
        }
    }
    GROUP("迭代模式") {
        SECTION("场景：-key value") {
            auto argv = std::vector{"-a", "va"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"-a", "va"}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：-key") {
            auto argv = std::vector<std::string>{"-a"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"-a", ""}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：-key1 -key2") {
            auto argv = std::vector<std::string>{"-key1", "-key2"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"-key1", ""}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"-key2", ""}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：-key -") {
            auto argv = std::vector<std::string>{"-a", "-"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"-a", ""}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：-key --") {
            auto argv = std::vector<std::string>{"-a", "--"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"-a", ""}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：-key - value") {
            auto argv = std::vector<std::string>{"-a", "-", "-va"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"-a", ""}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"-va", ""}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：-key -- value") {
            auto argv = std::vector<std::string>{"-a", "--", "-va"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"-a", "-va"}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：value1") {
            auto argv = std::vector<std::string>{"va", "vb"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"", "va"}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"", "vb"}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：-- value") {
            auto argv = std::vector<std::string>{"--", "va"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"", "va"}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：- value") {
            auto argv = std::vector<std::string>{"-", "va"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"-", "va"}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：value --") {
            auto argv = std::vector<std::string>{"va", "--"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"", "va"}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"", ""}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：- value") {
            auto argv = std::vector<std::string>{"-", "va"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"-", "va"}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：- -") {
            auto argv = std::vector<std::string>{"-", "-", "v1"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"-", ""}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"", "v1"}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：-- --") {
            auto argv = std::vector<std::string>{"--", "--"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"", "--"}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：- --") {
            auto argv = std::vector<std::string>{"-", "--"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"-", ""}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：-- -") {
            auto argv = std::vector<std::string>{"--", "-"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"", "-"}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：-") {
            auto argv = std::vector<std::string>{"-"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"-", ""}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：--") {
            auto argv = std::vector<std::string>{"--"};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"", ""}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：字段为空") {
            auto argv = std::vector<std::string>{"", ""};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"", ""}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), (std::tuple{"", ""}));
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：空列表") {
            auto argv = std::vector<std::string>{};
            auto itr = argv.begin();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
        SECTION("场景：起始位置超过容量") {
            auto argv = std::vector<std::string>{"aa", "bb"};
            auto itr = argv.end();
            ASSERT_EQ(str::next_opt2(itr, argv.end()), std::nullopt);
        }
    }
    GROUP("数组+const char*") {
        SECTION("场景：-key value") {
            int pos = 0;
            const char* argv[] = {"-a", "va"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", "va"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-key") {
            int pos = 0;
            const char* argv[] = {"-a"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
        }
        SECTION("场景：-key1 -key2") {
            int pos = 0;
            const char* argv[] = {"-key1", "-key2"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-key1", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-key2", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-key -") {
            int pos = 0;
            const char* argv[] = {"-a", "-"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-key --") {
            int pos = 0;
            const char* argv[] = {"-a", "--"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-key - value") {
            int pos = 0;
            const char* argv[] = {"-a", "-", "-va"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-va", ""}));
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
        }
        SECTION("场景：-key -- value") {
            int pos = 0;
            const char* argv[] = {"-a", "--", "-va"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", "-va"}));
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
        }
        SECTION("场景：value1") {
            int pos = 0;
            const char* argv[] = {"va", "vb"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", "va"}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", "vb"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-- value") {
            int pos = 0;
            const char* argv[] = {"--", "va"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", "va"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：- value") {
            int pos = 0;
            const char* argv[] = {"-", "va"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-", "va"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：value --") {
            int pos = 0;
            const char* argv[] = {"va", "--"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", "va"}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：- value") {
            int pos = 0;
            const char* argv[] = {"-", "va"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-", "va"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：- -") {
            int pos = 0;
            const char* argv[] = {"-", "-", "v1"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", "v1"}));
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
        }
        SECTION("场景：-- --") {
            int pos = 0;
            const char* argv[] = {"--", "--"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", "--"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：- --") {
            int pos = 0;
            const char* argv[] = {"-", "--"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-- -") {
            int pos = 0;
            const char* argv[] = {"--", "-"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", "-"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-") {
            int pos = 0;
            const char* argv[] = {"-"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
        }
        SECTION("场景：--") {
            int pos = 0;
            const char* argv[] = {"--"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
        }
        SECTION("场景：字段为空") {
            int pos = 0;
            const char* argv[] = {"", ""};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：空列表") {
            int pos = 0;
            const char* argv[] = {};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 0);
        }
        SECTION("场景：起始位置超过容量") {
            int pos = 0;
            const char* argv[] = {"aa", "bb"};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos=2, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos=3, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos=std::numeric_limits<decltype(argc)>::max(), argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos=-1, argc, argv), (std::tuple{"", "aa"}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos=std::numeric_limits<decltype(argc)>::min(), argc, argv), (std::tuple{"", "aa"}));
            ASSERT_EQ(pos, 1);
        }
        SECTION("场景：argv为nullptr") {
            int pos = 0;
            const char** argv = nullptr;
            int argc = 1;
            ASSERT_EQ(str::next_opt2(pos=0, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 0);
        }
        SECTION("场景：argc为负数") {
            int pos = 0;
            const char* argv[] = {"aa", "bb"};
            int argc = 0;
            ASSERT_EQ(str::next_opt2(pos=0, argc=-1, argv), std::nullopt);
            ASSERT_EQ(pos, 0);
        }
    }
    GROUP("数组+char*") {
        SECTION("场景：-key value") {
            int pos = 0;
            auto items = std::vector<std::string>{"-a", "va"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", "va"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-key") {
            int pos = 0;
            auto items = std::vector<std::string>{"-a"};
            char* argv[]{items[0].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
        }
        SECTION("场景：-key1 -key2") {
            int pos = 0;
            auto items = std::vector<std::string>{"-key1", "-key2"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-key1", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-key2", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-key -") {
            int pos = 0;
            auto items = std::vector<std::string>{"-a", "-"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-key --") {
            int pos = 0;
            auto items = std::vector<std::string>{"-a", "--"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-key - value") {
            int pos = 0;
            auto items = std::vector<std::string>{"-a", "-", "-va"};
            char* argv[]{items[0].data(), items[1].data(), items[2].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-va", ""}));
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
        }
        SECTION("场景：-key -- value") {
            int pos = 0;
            auto items = std::vector<std::string>{"-a", "--", "-va"};
            char* argv[]{items[0].data(), items[1].data(), items[2].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", "-va"}));
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
        }
        SECTION("场景：value1") {
            int pos = 0;
            auto items = std::vector<std::string>{"va", "vb"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", "va"}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", "vb"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-- value") {
            int pos = 0;
            auto items = std::vector<std::string>{"--", "va"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", "va"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：- value") {
            int pos = 0;
            auto items = std::vector<std::string>{"-", "va"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-", "va"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：value --") {
            int pos = 0;
            auto items = std::vector<std::string>{"va", "--"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", "va"}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：- value") {
            int pos = 0;
            auto items = std::vector<std::string>{"-", "va"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-", "va"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：- -") {
            int pos = 0;
            auto items = std::vector<std::string>{"-", "-", "v1"};
            char* argv[]{items[0].data(), items[1].data(), items[2].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", "v1"}));
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 3);
        }
        SECTION("场景：-- --") {
            int pos = 0;
            auto items = std::vector<std::string>{"--", "--"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", "--"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：- --") {
            int pos = 0;
            auto items = std::vector<std::string>{"-", "--"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-- -") {
            int pos = 0;
            auto items = std::vector<std::string>{"--", "-"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", "-"}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：-") {
            int pos = 0;
            auto items = std::vector<std::string>{"-"};
            char* argv[]{items[0].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
        }
        SECTION("场景：--") {
            int pos = 0;
            auto items = std::vector<std::string>{"--"};
            char* argv[]{items[0].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 1);
        }
        SECTION("场景：字段为空") {
            int pos = 0;
            auto items = std::vector<std::string>{"", ""};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", ""}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"", ""}));
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
        }
        SECTION("场景：空列表") {
            int pos = 0;
            auto items = std::vector<std::string>{"a", "b"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos, argc = 0, argv), std::nullopt);
            ASSERT_EQ(pos, 0);
        }
        SECTION("场景：起始位置超过容量") {
            int pos = 0;
            auto items = std::vector<std::string>{"aa", "bb"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = sizeof(argv) / sizeof(argv[0]);
            ASSERT_EQ(str::next_opt2(pos=2, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos=3, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos=std::numeric_limits<decltype(argc)>::max(), argc, argv), std::nullopt);
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::next_opt2(pos=-1, argc, argv), (std::tuple{"", "aa"}));
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::next_opt2(pos=std::numeric_limits<decltype(argc)>::min(), argc, argv), (std::tuple{"", "aa"}));
            ASSERT_EQ(pos, 1);
        }
        SECTION("场景：argv为nullptr") {
            int pos = 0;
            const char** argv = nullptr;
            int argc = 1;
            ASSERT_EQ(str::next_opt2(pos=0, argc, argv), std::nullopt);
            ASSERT_EQ(pos, 0);
        }
        SECTION("场景：argc为负数") {
            int pos = 0;
            auto items = std::vector<std::string>{"aa", "bb"};
            char* argv[]{items[0].data(), items[1].data()};
            int argc = 0;
            ASSERT_EQ(str::next_opt2(pos=0, argc=-1, argv), std::nullopt);
            ASSERT_EQ(pos, 0);
        }
    }
}
