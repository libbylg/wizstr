#include "tester.hpp"

#include "str.hpp"


#include <array>
#include <list>

TEST(test_str, join_map) {
    SECTION("一般情况") {
        std::map<std::string_view, std::string_view> items = {
            {"A", "a"},
            {"B", "b"},
            {"C", "c"},
        };
        ASSERT_EQ(str::join_map(items), "A=a,B=b,C=c");
        ASSERT_EQ(str::join_map(":", ";", items), "A:a;B:b;C:c");
    }
    // SECTION("非map:STL容器") {
    //     std::vector<std::tuple<std::string, std::string>> items = {
    //         {"A", "a"},
    //         {"B", "b"},
    //         {"C", "c"},
    //     };
    //     ASSERT_EQ(str::join_map(items), "A=a,B=b,C=c");
    //     ASSERT_EQ(str::join_map(":", ";", items), "A:a;B:b;C:c");
    // }
    // SECTION("非map:普通struct") {
    //     struct KY {
    //         std::string key;
    //         std::string val;
    //     };
    //     std::vector<KY> items = {
    //         {"A", "a"},
    //         {"B", "b"},
    //         {"C", "c"},
    //     };
    //     ASSERT_EQ(str::join_map(items), "A=a,B=b,C=c");
    //     ASSERT_EQ(str::join_map(":", ";", items), "A:a;B:b;C:c");
    // }
    SECTION("空映射") {
        ASSERT_TRUE(str::join_map([]() -> std::optional<std::tuple<std::string_view, std::string_view>> {
            return std::nullopt;
        }) == "");
    }

    SECTION("proc使用供给数据") {
        std::map<std::string_view, std::string_view> items = {
            {"A", "a"},
            {"B", "b"},
            {"C", "c"},
        };
        auto itr = items.cbegin();
        ASSERT_TRUE(str::join_map([&itr, end = items.cend()]() -> std::optional<std::tuple<std::string_view, std::string_view>> {
            if (itr == end) {
                return std::nullopt;
            }

            auto [key, val] = *(itr++);
            return std::tuple{key, val};
        }) == "A=a,B=b,C=c");
    }
}
