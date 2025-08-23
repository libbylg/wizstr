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

TEST(test_str, decode_base16) {
    ASSERT_EQ(str::decode_base16(""), "");
    ASSERT_EQ(str::decode_base16("61"), "a");
    ASSERT_EQ(str::decode_base16("6162"), "ab");
    ASSERT_EQ(str::decode_base16("616263"), "abc");
    ASSERT_EQ(str::decode_base16("61626358595A"), "abcXYZ");
    ASSERT_EQ(str::decode_base16("61626358595a"), "abcXYZ");
    ASSERT_EQ(str::decode_base16("78797AFF807C414243"), "xyz\xFF\x80|ABC");
    ASSERT_EQ(str::decode_base16("78797aff807c414243"), "xyz\xFF\x80|ABC");
}
