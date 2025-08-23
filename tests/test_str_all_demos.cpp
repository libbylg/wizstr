/**
 * Copyright (c) 2021-2025  libbylg@126.com
 * wizstr is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "testing.hpp"
#include "test-utils.hpp"

#include "str.hpp"

#include <iostream>

#if defined(WIZSTR_NAMESPACE)
using str = WIZSTR_NAMESPACE::str;
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

TEST(test_demo, split_by_charset) {
    std::vector<std::string> result;
    std::string s = "A12BCd3";

    static auto charset_digits = str::charset(str::all_digits);
    static auto charset_alphas = str::charset(str::all_alphas);
    for (size_t pos = 0; pos < s.size();) {
        auto charset = (std::isdigit(s[pos])) ? &charset_digits : &charset_alphas;
        result.emplace_back(str::take_view(s, *str::accept(s, pos, *charset)));
    }

    ASSERT_EQ(result, (std::vector{"A", "12", "BCd", "3"}));
}