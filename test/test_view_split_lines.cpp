#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

template <typename elem1_type>
static auto eq(const std::vector<elem1_type>& a, const std::vector<elem1_type>& b) -> bool {
    if (a.size() != b.size()) {
        return false;
    }

    for (view::size_type i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

// TEST_CASE("view::split_list") {
//     SECTION("单个字符作为分隔符") {
//         REQUIRE(eq(view::split_list("a,b,c,", ','), std::vector<std::string_view>{"a", "b", "c", ""}));
//     }

TEST_CASE("view::split_lines") {
    SECTION("简单场景") {
        std::string s{"\na\r\nb\ncccc\n"};
        std::vector<std::string_view> fields = {
            std::string{""},
            std::string{"a"},
            std::string{"b"},
            std::string{"cccc"},
            std::string{""},
        };

        std::vector<std::string_view> result;
        view::split_lines(s, false, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(eq(result, fields));
    }
//    SECTION("空串") {
//        std::string s{""};
//        std::vector<std::string> fields = {
//            std::string{""},
//        };
//
//        std::vector<std::string> result;
//        s.split_lines([&result](std::string::const_pointer p, std::string::size_type n) -> int {
//            result.emplace_back(p, n);
//            return 0;
//        });
//
//        ASSERT_EQ(result, fields);
//    }
//    SECTION("纯换行") {
//        std::string s{"\n"};
//        std::vector<std::string> fields = {
//            std::string{""},
//            std::string{""},
//        };
//
//        std::vector<std::string> result;
//        s.split_lines([&result](std::string::const_pointer p, std::string::size_type n) -> int {
//            result.emplace_back(p, n);
//            return 0;
//        });
//
//        ASSERT_EQ(result, fields);
//    }
//
//    SECTION("综合测试") {
//        std::string s{"a\n\ryyy\r\r\nb\n\nc\n\r"};
//        std::vector<std::string> fields = {
//            std::string{"a"},
//            std::string{""},
//            std::string{"yyy"},
//            std::string{""},
//            std::string{"b"},
//            std::string{""},
//            std::string{"c"},
//            std::string{""},
//            std::string{""},
//        };
//
//        std::vector<std::string> result;
//        s.split_lines([&result](std::string::const_pointer p, std::string::size_type n) -> int {
//            result.emplace_back(p, n);
//            return 0;
//        });
//
//        ASSERT_EQ(result, fields);
//    }
}

// TEST(tiny_bytes, split_lines_keepends) {
//     SECTION("简单场景") {
//         std::string s{ "\na\r\nb\ncccc\n" };
//         std::vector<std::string> fields = {
//             std::string{ "\n" },
//             std::string{ "a\r\n" },
//             std::string{ "b\n" },
//             std::string{ "cccc\n" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//     SECTION("空串") {
//         std::string s{ "" };
//         std::vector<std::string> fields = {
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//     SECTION("纯换行") {
//         std::string s{ "\n" };
//         std::vector<std::string> fields = {
//             std::string{ "\n" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//
//     SECTION("综合测试") {
//         std::string s{ "a\n\ryyy\r\r\nb\n\nc\n\r" };
//         std::vector<std::string> fields = {
//             std::string{ "a\n" },
//             std::string{ "\r" },
//             std::string{ "yyy\r" },
//             std::string{ "\r\n" },
//             std::string{ "b\n" },
//             std::string{ "\n" },
//             std::string{ "c\n" },
//             std::string{ "\r" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
// }
