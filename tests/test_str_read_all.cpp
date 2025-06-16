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

TEST(test_str, read_all) {
    SECTION("Gernial") {
        std::string filename{str::dirname(__FILE__) + "/data/test-general.txt"};
        std::string expected{"ABCDE12345\nABCDE\n12345"};
        ASSERT_EQ(str::read_all(filename), expected);
    }
    SECTION("File-Not-Exist") {
        std::string filename{str::dirname(__FILE__) + "/data/NOTEXIST.txt"};
        std::string expected{};
        ASSERT_EQ(str::read_all(filename), expected);
    }
    SECTION("EmptyFile") {
        std::string filename{str::dirname(__FILE__) + "/data/test-empty.txt"};
        std::string expected{};
        ASSERT_EQ(str::read_all(filename), expected);
    }
}