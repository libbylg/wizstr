#include "testing.hpp"

#include "str.hpp"

#include "test-compares.hpp"
#include "test-to_consumer.hpp"

TEST(test_str, split) {
    GROUP("分隔符缺省场景：按空白拆分(且多余空白忽略)") {
        SECTION("无次数限制") {
            ASSERT_EQ(str::split("a b c"), (std::vector<std::string>{"a", "b", "c"}));
            ASSERT_EQ(str::split("a\tb\tc"), (std::vector<std::string>{"a", "b", "c"}));
            ASSERT_EQ(str::split("a\rb\rc"), (std::vector<std::string>{"a", "b", "c"}));
            ASSERT_EQ(str::split("a\nb\nc"), (std::vector<std::string>{"a", "b", "c"}));
            ASSERT_EQ(str::split("a\vb\vc"), (std::vector<std::string>{"a", "b", "c"}));
            ASSERT_EQ(str::split("a\r\n\t\vb\r\n\t\vc"), (std::vector<std::string>{"a", "b", "c"}));
            ASSERT_EQ(str::split("\r\t\n\v a\r\t\n\v b\r\t\n\v c\r\t\n\v "), (std::vector<std::string>{"a", "b", "c"}));
            ASSERT_EQ(str::split(" \t\r\n\v"), (std::vector<std::string>{}));
            ASSERT_EQ(str::split("abc"), (std::vector<std::string>{"abc"}));
            ASSERT_EQ(str::split(" abc"), (std::vector<std::string>{"abc"}));
            ASSERT_EQ(str::split("\tabc"), (std::vector<std::string>{"abc"}));
            ASSERT_EQ(str::split("\rabc"), (std::vector<std::string>{"abc"}));
            ASSERT_EQ(str::split("\nabc"), (std::vector<std::string>{"abc"}));
            ASSERT_EQ(str::split("\vabc"), (std::vector<std::string>{"abc"}));
            ASSERT_EQ(str::split("abc "), (std::vector<std::string>{"abc"}));
            ASSERT_EQ(str::split("abc\t"), (std::vector<std::string>{"abc"}));
            ASSERT_EQ(str::split("abc\r"), (std::vector<std::string>{"abc"}));
            ASSERT_EQ(str::split("abc\n"), (std::vector<std::string>{"abc"}));
            ASSERT_EQ(str::split("abc\v"), (std::vector<std::string>{"abc"}));
            ASSERT_EQ(str::split(""), (std::vector<std::string>{}));
        }
        SECTION("限制次数") {
            ASSERT_EQ(str::split(" a\rb\tc\vd\n", 2), (std::vector<std::string>{"a", "b", "c\vd\n"}));
        }
        SECTION("被拆分字符串无对应分隔符") {
            ASSERT_EQ(str::split("abcdef#@#$"), (std::vector<std::string>{"abcdef#@#$"}));
        }
        SECTION("空串作为分隔符") {
            ASSERT_EQ(str::split("ab,cdef"), (std::vector<std::string>{"ab", "cdef"}));
        }
        SECTION("限制值为0") {
            ASSERT_EQ(str::split(",aa,,bb,cc,", 0), (std::vector<std::string>{",aa,,bb,cc,"}));
        }
        SECTION("限制值合适") {
            ASSERT_EQ(str::split(",aa,,bb,cc,", 4), (std::vector<std::string>{"", "aa", "", "bb", "cc,"}));
        }
        SECTION("限制值超过总段数") {
            ASSERT_EQ(str::split(",aa,,bb,cc,", 100), (std::vector<std::string>{"", "aa", "", "bb", "cc", ""}));
        }
    }

    GROUP("proc场景") {
        SECTION("简单场景1") {
            std::vector<std::string_view> result;
            str::split("HelloWorld,HelloWorld", to_consumer{result});
            ASSERT_EQ(result, (std::vector<std::string_view>{"HelloWorld", "HelloWorld"}));
        }
        SECTION("简单场景2") {
            std::vector<std::string_view> result;
            str::split("a,d,ee,", to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string_view>{"a", "d", "ee", ""}));
        }

        SECTION("刚好匹配一个") {
            std::vector<std::string_view> result;
            str::split(std::string(","), to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string_view>{"", ""}));
        }

        SECTION("前面字段") {
            std::vector<std::string_view> result;
            str::split("AAA,", to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string_view>{"AAA", ""}));
        }

        SECTION("后面字段") {
            std::vector<std::string_view> result;
            str::split(",AAA", to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string_view>{"", "AAA"}));
        }

        SECTION("拆分空串") {
            std::vector<std::string_view> result;
            str::split("", to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string_view>{""}));
        }

        SECTION("连续匹配") {
            std::vector<std::string_view> result;
            str::split("AA,,BB", to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string_view>{"AA", "", "BB"}));
        }

        SECTION("连续匹配2") {
            std::vector<std::string_view> result;
            str::split(",,", to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string_view>{"", "", ""}));
        }

        SECTION("无法匹配") {
            std::vector<std::string_view> result;
            str::split("defgui", to_consumer(result));
            ASSERT_EQ(result, (std::vector<std::string_view>{"defgui"}));
        }

        SECTION("提前结束:一般场景") {
            std::vector<std::string_view> result;
            str::split(",123,456,,def,ghi,", [&result](std::string_view item) -> int {
                result.emplace_back(item);
                if (result.size() == 4) {
                    return -1;
                }
                return 0;
            });

            ASSERT_EQ(result, (std::vector<std::string_view>{"", "123", "456", ""}));
        }

        SECTION("提前结束:但总数量并不够") {
            std::vector<std::string_view> result;
            str::split(",123,456,defghi", [&result](std::string_view item) -> int {
                result.emplace_back(item);
                if (result.size() == 4) {
                    return -1;
                }
                return 0;
            });

            ASSERT_EQ(result, (std::vector<std::string_view>{"", "123", "456", "defghi"}));
        }
    }

    // GROUP("regex场景") {
    //     SECTION("一般场景") {
    //         std::regex sep("[ \t]*(:|,|;)[ \t]*");
    //         ASSERT_EQ(str::split("aaa, bbb:ccc ;ddd ", sep), (std::vector<std::string>{"aaa", "bbb", "ccc", "ddd "}));
    //         ASSERT_EQ(str::split("aaa, bbb:ccc ;ddd ", sep, 0), (std::vector<std::string>{"aaa, bbb:ccc ;ddd "}));
    //         ASSERT_EQ(str::split("aaa, bbb:ccc ;ddd ", sep, 1), (std::vector<std::string>{"aaa", "bbb:ccc ;ddd "}));
    //         ASSERT_EQ(str::split("aaa, bbb:ccc ;ddd ", sep, 2), (std::vector<std::string>{"aaa", "bbb", "ccc ;ddd "}));
    //         ASSERT_EQ(str::split("aaa, bbb:ccc ;ddd ", sep, 3), (std::vector<std::string>{"aaa", "bbb", "ccc", "ddd "}));
    //         ASSERT_EQ(str::split("aaa, bbb:ccc ;ddd ", sep, 4), (std::vector<std::string>{"aaa", "bbb", "ccc", "ddd "}));
    //         ASSERT_EQ(str::split("aaa, bbb:ccc ;ddd ", sep, str::npos), (std::vector<std::string>{"aaa", "bbb", "ccc", "ddd "}));
    //     }
    //     SECTION("空串") {
    //         std::regex sep(":|,|;");
    //         ASSERT_EQ(str::split("", sep), (std::vector<std::string>{""}));
    //         ASSERT_EQ(str::split("", sep, 0), (std::vector<std::string>{""}));
    //         ASSERT_EQ(str::split("", sep, 3), (std::vector<std::string>{""}));
    //     }
    //     SECTION("proc:不限拆分次数") {
    //         std::regex sep("[ \t]*(:|,|;)[ \t]*");
    //         std::vector<std::string_view> result;
    //         str::split("aaa, bbb:ccc ;ddd ", sep, [&result](std::string_view item) -> int {
    //             result.emplace_back(item);
    //             return 0;
    //         });
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"aaa", "bbb", "ccc", "ddd "}));
    //     }
    //     SECTION("proc:限制拆分次数+提前返回") {
    //         std::regex sep("[ \t]*(:|,|;)[ \t]*");
    //         std::vector<std::string_view> result;
    //         str::split("aaa, bbb:ccc ;ddd ", sep, 2, [&result](std::string_view item) -> int {
    //             result.emplace_back(item);
    //             if (result.size() == 2) { // max_n 限制不起作用,这里的约束条件会提前触发,导致提前结束,所以只存两个,尾部的数据直接丢弃
    //                 return -1;
    //             }
    //             return 0;
    //         });
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"aaa", "bbb"}));
    //     }
    //     SECTION("proc:限制拆分次数+提前返回+限制次数较小") {
    //         std::regex sep("[ \t]*(:|,|;)[ \t]*");
    //         std::vector<std::string_view> result;
    //         str::split("aaa, bbb:ccc ;ddd,MNP", sep, 2, [&result](std::string_view item) -> int {
    //             result.emplace_back(item);
    //             if (result.size() == 4) { // max_n 提前返回,限制起作用，实际能存三个
    //                 return -1;
    //             }
    //             return 0;
    //         });
    //         ASSERT_EQ(result, (std::vector<std::string_view>{"aaa", "bbb", "ccc ;ddd,MNP"}));
    //     }
    //     SECTION("全都是分隔符") {
    //         std::regex sep(":|,|;");
    //         ASSERT_EQ(str::split("::,,;;", sep), (std::vector<std::string>{"", "", "", "", "", "", ""}));
    //     }
    //     SECTION("分隔符刚好在尾部") {
    //         std::regex sep(":|,|;");
    //         ASSERT_EQ(str::split("aaa:bbb,ccc;", sep), (std::vector<std::string>{"aaa", "bbb", "ccc", ""}));
    //     }
    //     SECTION("分隔符刚好在头部") {
    //         std::regex sep(":|,|;");
    //         ASSERT_EQ(str::split(":aaa:bbb,ccc", sep), (std::vector<std::string>{"", "aaa", "bbb", "ccc"}));
    //     }
    // }
}