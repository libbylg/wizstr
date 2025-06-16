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

#include <iostream>

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_demo, align) {

    std::string result;
    for (int i = 11; i >= 1; i -= 2) {
        result.append(str::align_center(str::repeat('*', i), 11)).append("\n");
        // std::cout << str::align_center(str::repeat('*', i), 11) << std::endl;
    }

    std::string expect = //
        "***********\n"  //
        " ********* \n"  //
        "  *******  \n"  //
        "   *****   \n"  //
        "    ***    \n"  //
        "     *     \n";
    ASSERT_EQ(result, expect);
}
