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

TEST(test_str, next_opt2) {
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
    SECTION("场景2：-key value") {
        int pos = 0;
        auto items = std::vector<std::string>{"-a", "va"};
        char* argv[]{items[0].data(), items[1].data()};
        ASSERT_EQ(str::next_opt2(pos, 2, argv), (std::tuple{"-a", "va"}));
        ASSERT_EQ(pos, 2);
        ASSERT_EQ(str::next_opt2(pos, 2, argv), std::nullopt);
        ASSERT_EQ(pos, 2);
        ASSERT_EQ(str::next_opt2(pos, 2, argv), std::nullopt);
        ASSERT_EQ(pos, 2);
    }
    SECTION("场景：-key|") {
        int pos = 0;
        const char* argv[] = {"-a"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", ""}));
        ASSERT_EQ(pos, 1);
        ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
        ASSERT_EQ(pos, 1);
    }
    SECTION("场景：-key -|") {
        int pos = 0;
        const char* argv[] = {"-a", "-"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", ""}));
        ASSERT_EQ(pos, 2);
        ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
        ASSERT_EQ(pos, 2);
    }
    SECTION("场景：-key --|") {
        int pos = 0;
        const char* argv[] = {"-a", "--"};
        int argc = sizeof(argv) / sizeof(argv[0]);
        ASSERT_EQ(str::next_opt2(pos, argc, argv), (std::tuple{"-a", ""}));
        ASSERT_EQ(pos, 2);
        ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
        ASSERT_EQ(pos, 2);
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
    SECTION("场景：空列表") {
        int pos = 0;
        const char* argv[] = {};
        int argc = sizeof(argv) / sizeof(argv[0]);
        ASSERT_EQ(str::next_opt2(pos, argc, argv), std::nullopt);
        ASSERT_EQ(pos, 0);
    }
}
