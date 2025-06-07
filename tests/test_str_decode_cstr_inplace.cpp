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

#include <string>

TEST(test_str, decode_cstr_inplace) {
    SECTION("一般情况") {
        std::string s;
        size_t pos = 0;

        pos = str::decode_cstr_inplace(s = R"()");
        ASSERT_TRUE((pos == 0u) && (s == R"()"));
        pos = str::decode_cstr_inplace(s = R"(\'\"\?\a\b\f\n\r\t\v\\)");
        ASSERT_TRUE((pos == 22) && (s == "\'\"\?\a\b\f\n\r\t\v\\"));
        pos = str::decode_cstr_inplace(s = R"(abc\77Kdef)");
        ASSERT_TRUE((pos == 10) && (s == std::string_view{"abc\77Kdef", 8}));
        pos = str::decode_cstr_inplace(s = R"(abc\7  Kdef)");
        ASSERT_TRUE((pos == 11) && (s == std::string_view{"abc\7  Kdef", 10}));
        pos = str::decode_cstr_inplace(s = R"(abc\x00Kdef)");
        ASSERT_TRUE((pos == 11) && (s == std::string_view{"abc\0Kdef", 8}));
        pos = str::decode_cstr_inplace(s = R"(abc\xA1Kdef)");
        ASSERT_TRUE((pos == 11) && (s == std::string_view{"abc\xA1Kdef", 8}));
        pos = str::decode_cstr_inplace(s = R"(abc\xf1Kdef)");
        ASSERT_TRUE((pos == 11) && (s == std::string_view{"abc\xf1Kdef", 8}));
        pos = str::decode_cstr_inplace(s = R"(abc\x00000f1Kdef)");
        ASSERT_TRUE((pos == 16) && (s == std::string_view{"abc\x00000f1Kdef", 8}));
        pos = str::decode_cstr_inplace(s = R"(\aMNPc\)");
        ASSERT_TRUE((pos == 7) && (s == std::string_view{"\aMNPc", 5}));
        pos = str::decode_cstr_inplace(s = R"(\aMNPc\777)");
        ASSERT_TRUE((pos == 7) && (s == std::string_view{"\aMNPc", 5}));
        pos = str::decode_cstr_inplace(s = R"(\aMNPc\x)");
        ASSERT_TRUE((pos == 7) && (s == std::string_view{"\aMNPc", 5}));
        pos = str::decode_cstr_inplace(s = R"(\aMNPc\xZZ)");
        ASSERT_TRUE((pos == 7) && (s == std::string_view{"\aMNPc", 5}));
        pos = str::decode_cstr_inplace(s = R"(\aMNPc\x99999)");
        ASSERT_TRUE((pos == 7) && (s == std::string_view{"\aMNPc", 5}));
        pos = str::decode_cstr_inplace(s = R"(\q)");
        ASSERT_TRUE((pos == 1) && (s == std::string_view{"", 0}));
    }
}