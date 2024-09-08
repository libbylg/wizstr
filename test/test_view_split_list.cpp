#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

template <typename elem1_type, typename elem2_type>
static auto eq(const std::vector<elem1_type>& a, const std::vector<elem2_type>& b) -> bool {
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

TEST_CASE("view::split_list") {
    SECTION("单个字符作为分隔符") {
        REQUIRE(eq(view::split_list("a,b,c,", ','), std::vector<std::string_view>{"a", "b", "c", ""}));
    }
    SECTION("字符串作为分隔符") {
        REQUIRE(eq(view::split_list("a,b,c,", ","), std::vector<std::string_view>{"a", "b", "c", ""}));
    }
    SECTION("空串") {
        REQUIRE(eq(view::split_list("", ","), std::vector<std::string_view>{""}));
    }
}

//
// // TEST_CASE("split_list", "as vector") {
// TEST_CASE(split_list, as_vector) {
//     SECTION("一般情况") {
//         auto items = str::split_list("a,b,c", ',');
//         REQUIRE(items.size() == 3u);
//         REQUIRE(items[0] == "a");
//         REQUIRE(items[1] == "b");
//         REQUIRE(items[2] == "c");
//     }
//     SECTION("全空情况") {
//         auto items = str::split_list(",,", ',');
//         REQUIRE(items.size() == 3u);
//         REQUIRE(items[0] == "");
//         REQUIRE(items[1] == "");
//         REQUIRE(items[2] == "");
//     }
//     SECTION("没有替换分个符") {
//         auto items = str::split_list("abc", ',');
//         REQUIRE(items.size() == 1u);
//         REQUIRE(items[0] == "abc");
//     }
//     SECTION("分隔符号在开头") {
//         auto items = str::split_list(",abc", ',');
//         REQUIRE(items.size() == 2u);
//         REQUIRE(items[0] == "");
//         REQUIRE(items[1] == "abc");
//     }
//     SECTION("分隔符号在结尾") {
//         auto items = str::split_list("abc,", ',');
//         REQUIRE(items.size() == 2u);
//         REQUIRE(items[0] == "abc");
//         REQUIRE(items[1] == "");
//     }
// }

// TEST(tiny_bytes, split_by_char) {
//     std::string a("HelloWorld-HelloWorld");
//     std::vector<std::string> v;
//     a.split('-', [&v](std::string::const_pointer s, std::string::size_type n) -> int {
//         v.emplace_back(s, n);
//         return 0;
//     });
//     EXPECT_EQ(v.size(), 2);
//     EXPECT_EQ(v[0], "HelloWorld");
//     EXPECT_EQ(v[1], "HelloWorld");
//     std::vector<std::string> p{ std::string{ "HelloWorld" }, std::string{ "HelloWorld" } };
//     EXPECT_EQ(v, p);
// }

// TEST(tiny_bytes, split_by_string) {
//     SECTION("简单场景") {
//         std::string s{ "abcdbceebc" };
//         std::vector<std::string> fileds = {
//             std::string{ "a" },
//             std::string{ "d" },
//             std::string{ "ee" },
//             std::string{ "" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("刚好匹配一个") {
//         std::string s{ "bc" };
//         std::vector<std::string> fileds = {
//             std::string{ "" },
//             std::string{ "" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("前面字段") {
//         std::string s{ "AAAbc" };
//         std::vector<std::string> fileds = {
//             std::string{ "AAA" },
//             std::string{ "" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("后面字段") {
//         std::string s{ "bcAAA" };
//         std::vector<std::string> fileds = {
//             std::string{ "" },
//             std::string{ "AAA" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("空串") {
//         std::string s{ "" };
//         std::vector<std::string> fileds = {
//             std::string{ "" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("连续匹配") {
//         std::string s{ "AAbcbcBB" };
//         std::vector<std::string> fileds = {
//             std::string{ "AA" },
//             std::string{ "" },
//             std::string{ "BB" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("连续匹配2") {
//         std::string s{ "bcbc" };
//         std::vector<std::string> fileds = {
//             std::string{ "" },
//             std::string{ "" },
//             std::string{ "" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
//
//     SECTION("无法匹配") {
//         std::string s{ "defgui" };
//         std::vector<std::string> fileds = {
//             std::string{ "defgui" },
//         };
//         std::vector<std::string> result;
//         s.split(std::string("bc"), [&result](std::string::const_pointer s, std::string::size_type n) -> int {
//             result.emplace_back(s, n);
//             return 0;
//         });
//
//         ASSERT_EQ(fileds, result);
//     }
// }