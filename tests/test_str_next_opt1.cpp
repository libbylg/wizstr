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

TEST(test_str, next_opt1) {
    SECTION("一般情况:容器") {
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
    }SECTION("一般情况:数组") {
        int pos = 0;
        auto items = std::vector<std::string>{"-a=va", "-b", "--", "-vvv", "ccc"};
        char* argv[5]{items[0].data(), items[1].data(), items[2].data(), items[3].data(), items[4].data()};
        ASSERT_EQ(str::next_opt1(pos, 5, argv), (std::tuple{"-a", "va"}));
        ASSERT_EQ(pos, 1);
        ASSERT_EQ(str::next_opt1(pos, 5, argv), (std::tuple{"-b", ""}));
        ASSERT_EQ(pos, 2);
        ASSERT_EQ(str::next_opt1(pos, 5, argv), (std::tuple{"", "-vvv"}));
        ASSERT_EQ(pos, 4);
        ASSERT_EQ(str::next_opt1(pos, 5, argv), (std::tuple{"", "ccc"}));
        ASSERT_EQ(pos, 5);
    }
    SECTION("一般情况3") {
        int pos = 0;
        const char* argv[5] = {"-a=va", "-b", "--", "-vvv", "ccc"};
        ASSERT_EQ(str::next_opt1(pos, sizeof(argv) / sizeof(argv[0]), argv), (std::tuple{"-a", "va"}));
        ASSERT_EQ(pos, 1);
        ASSERT_EQ(str::next_opt1(pos, sizeof(argv) / sizeof(argv[0]), argv), (std::tuple{"-b", ""}));
        ASSERT_EQ(pos, 2);
        ASSERT_EQ(str::next_opt1(pos, sizeof(argv) / sizeof(argv[0]), argv), (std::tuple{"", "-vvv"}));
        ASSERT_EQ(pos, 4);
        ASSERT_EQ(str::next_opt1(pos, sizeof(argv) / sizeof(argv[0]), argv), (std::tuple{"", "ccc"}));
        ASSERT_EQ(pos, 5);
    }
}
