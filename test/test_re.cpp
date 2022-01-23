/**
 * Copyright (c) 2021-2021 libbylg@126.com
 * tiny is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "tester.h"

#include "tiny/bytes.h"
#include "tiny/re.h"

#include <iostream>

#ifndef SECTION
#define SECTION(xx)
#endif

TEST(tiny_re, tiny_re_match) {
    SECTION("基本测试") {
        tiny::re rx("a+(b*)");
        bool ret = rx.match("xxx-aab-abb");
        EXPECT_FALSE(ret);
    }
}

TEST(tiny_re, tiny_re_find) {

    SECTION("基本测试") {
        tiny::re rx("a+(b*)");
        int cnt = 0;
        rx.find("xxx-aab-abbb", 0, [&cnt](const tiny::re::segment_type* segments, tiny::re::size_type n) -> int {
            for (uint32_t i = 0; i < n; i++) {
                printf("%d: [%d] [%.*s]\n", cnt, i, segments[i].len, segments[i].ptr);
            }

            cnt++;
            return 0;
        });
    }
}

TEST(tiny_re, tiny_re_split) {
    SECTION("基本测试") {
        tiny::re rx("(-+|[ \t]+)");
        int cnt = 0;
        rx.split("xxx-aab abb", 0, [&cnt](const tiny::re::segment_type& segment) -> int {
            printf("%d: [%.*s]\n", cnt, segment.len, segment.ptr);
            cnt++;
            return 0;
        });
    }
}