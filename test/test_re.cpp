//
// Created by luolijun on 2021/10/7.
//
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
            for (int i = 0; i < n; i++) {
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