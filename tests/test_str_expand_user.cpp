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

#include "str.hpp"

#if defined(WIZSTR_NAMESPACE)
using str = WIZSTR_NAMESPACE::str;
#endif

TEST(test_str, expand_user) {
    SECTION("简单场景") {
        ASSERT_EQ(str::expand_user("~"), std::string{str::home()});
        ASSERT_EQ(str::expand_user("~/"), std::string{str::home()} + "/");
        ASSERT_EQ(str::expand_user("~/abc"), std::string{str::home()} + "/abc");
    }
    SECTION("空串") {
        ASSERT_EQ(str::expand_user(""), "");
    }
    SECTION("起始点不匹配") {
        ASSERT_EQ(str::expand_user("~ "), "~ ");
        ASSERT_EQ(str::expand_user(" ~/abc"), " ~/abc");
    }
}
