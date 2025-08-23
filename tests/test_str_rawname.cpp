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

TEST(test_str, rawname) {
    SECTION("一般情况") {
        ASSERT_EQ(str::rawname("/aaa/bbb/ccc.txt"), "ccc");   // str::range(9, 3));
        ASSERT_EQ(str::rawname("/aaa/bbb/ccc"), "ccc");       // str::range(9, 3));
        ASSERT_EQ(str::rawname("/aaa/bbb/"), "");             // str::range(9, 0));
        ASSERT_EQ(str::rawname("/aaa/bbb/.ccc"), ".ccc");     // str::range(9, 4));
        ASSERT_EQ(str::rawname("/aaa/bbb/.ccc.xml"), ".ccc"); // str::range(9, 4));
        ASSERT_EQ(str::rawname("/"), "");                     // str::range(1, 0));
        ASSERT_EQ(str::rawname("."), "");                     // str::range(1, 0));
        ASSERT_EQ(str::rawname(".."), "");                    // str::range(2, 0));
        ASSERT_EQ(str::rawname("a"), "a");                    // str::range(0, 1));
        ASSERT_EQ(str::rawname("a."), "a");                   // str::range(0, 1));
        ASSERT_EQ(str::rawname(".a"), ".a");                  // str::range(0, 2));
        ASSERT_EQ(str::rawname(".abc.txt"), ".abc");          // str::range(0, 4));
        ASSERT_EQ(str::rawname(""), "");                      // str::range(0, 0));
        ASSERT_EQ(str::rawname(".a..def"), ".a.");            // str::range(0, 2));
    }
}
