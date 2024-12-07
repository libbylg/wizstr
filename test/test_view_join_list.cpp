#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

#include <array>
#include <list>
#include <vector>

TEST(test_view, join_list) {
    SECTION("一般场景") {
        ASSERT_EQ(view::join_list(std::vector{"A", "B", "C"}), "A,B,C");
        ASSERT_EQ(view::join_list(";", std::vector{"A", "B"}), "A;B");
        ASSERT_EQ(view::join_list("#", std::vector{"A"}), "A");
        ASSERT_EQ(view::join_list("|", std::vector{"A", "B", "C"}), "A|B|C");
        ASSERT_EQ(view::join_list("", std::vector{"A", "B", "C"}), "ABC");
        ASSERT_EQ(view::join_list("|", std::vector<view::const_pointer>{}), "");
        ASSERT_EQ(view::join_list("|", std::vector{"A", "", "C"}), "A||C");
        ASSERT_EQ(view::join_list("/", std::list{"A", "B", "C"}), "A/B/C");
        ASSERT_EQ(view::join_list("*", std::array{"A", "B", "C"}), "A*B*C");
        ASSERT_EQ(view::join_list("*", std::array{"", "", ""}), "**");
    }
    SECTION("使用 initialize_list 形式") {
        ASSERT_EQ(view::join_list({"A", "B", "C"}), "A,B,C");
        ASSERT_EQ(view::join_list(";", {"A", "B"}), "A;B");
    }
    SECTION("proc使用供给数据") {
        std::array items{"A", "B", "C"};
        size_t i = 0;
        ASSERT_TRUE(view::join_list([&i, &items]() -> std::optional<std::string_view> {
            if (i >= items.size()) {
                return std::nullopt;
            }

            return items[i++];
        }) == "A,B,C");
    }
}
