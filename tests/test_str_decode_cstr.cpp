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

#include <string>

#if defined(WIZSTR_NAMESPACE)
using str = WIZSTR_NAMESPACE::str;
#endif

TEST(test_str, decode_cstr) {
    SECTION("一般情况") {
        ASSERT_EQ(str::decode_cstr(R"()"), (std::tuple{0u, R"()"}));
        ASSERT_EQ(str::decode_cstr(R"(\'\"\?\a\b\f\n\r\t\v\\)"), (std::tuple{22, "\'\"\?\a\b\f\n\r\t\v\\"}));
        ASSERT_EQ(str::decode_cstr(R"(abc\77Kdef)"), (std::tuple{10, std::string_view{"abc\77Kdef", 8}}));
        ASSERT_EQ(str::decode_cstr(R"(abc\7  Kdef)"), (std::tuple{11, std::string_view{"abc\7  Kdef", 10}}));
        ASSERT_EQ(str::decode_cstr(R"(abc\x00Kdef)"), (std::tuple{11, std::string_view{"abc\0Kdef", 8}}));
        ASSERT_EQ(str::decode_cstr(R"(abc\xA1Kdef)"), (std::tuple{11, std::string_view{"abc\xA1Kdef", 8}}));
        ASSERT_EQ(str::decode_cstr(R"(abc\xf1Kdef)"), (std::tuple{11, std::string_view{"abc\xf1Kdef", 8}}));
        ASSERT_EQ(str::decode_cstr(R"(abc\x00000f1Kdef)"), (std::tuple{16, std::string_view{"abc\x00000f1Kdef", 8}}));
        ASSERT_EQ(str::decode_cstr(R"(\aMNPc\)"), (std::tuple{7, std::string_view{"\aMNPc", 5}}));
        ASSERT_EQ(str::decode_cstr(R"(\aMNPc\777)"), (std::tuple{7, std::string_view{"\aMNPc", 5}}));
        ASSERT_EQ(str::decode_cstr(R"(\aMNPc\x)"), (std::tuple{7, std::string_view{"\aMNPc", 5}}));
        ASSERT_EQ(str::decode_cstr(R"(\aMNPc\xZZ)"), (std::tuple{7, std::string_view{"\aMNPc", 5}}));
        ASSERT_EQ(str::decode_cstr(R"(\aMNPc\x99999)"), (std::tuple{7, std::string_view{"\aMNPc", 5}}));
        ASSERT_EQ(str::decode_cstr(R"(\q)"), (std::tuple{1, std::string_view{"", 0}}));
    }
    SECTION("proc形式:提前结束") {
        std::string s;
        str::decode_cstr(R"(abc\ndef\nmnp)", [&s](std::string_view seg) -> int {
            s.append(seg);
            if (s.size() >= 5) {
                return -1;
            }

            return 0;
        });
        ASSERT_EQ(s, "abc\ndef");
    }
    SECTION("proc形式:提前结束2") {
        std::string s;
        str::decode_cstr(R"(abc\ndef)", [&s](std::string_view seg) -> int {
            s.append(seg);
            if (s.size() >= 5) {
                return -1;
            }

            return 0;
        });
        ASSERT_EQ(s, "abc\ndef");
    }
    SECTION("proc形式:提前结束3") {
        std::string s;
        str::decode_cstr(R"(abc\n\n\n\n\n\n)", [&s](std::string_view seg) -> int {
            s.append(seg);
            if (s.size() >= 5) {
                return -1;
            }

            return 0;
        });
        ASSERT_EQ(s, "abc\n\n");
    }
    // SECTION("proc形式:提前结束2") {
    //     std::string s;
    //     str::decode_cstr(R"(abc\"'?)", [&s](std::string_view seg) -> int {
    //         s.append(seg);
    //         if (s.size() >= 6) {
    //             return -1;
    //         }
    //
    //         return 0;
    //     });
    //     ASSERT_EQ(s, R"(abc\\\")");
    // }
}