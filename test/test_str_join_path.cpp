#include "testing.hpp"

#include "str.hpp"


#include <list>

TEST(test_str, join_path) {
    SECTION("一般场景") {
        ASSERT_EQ(str::join_path(std::vector{"A"}), "A");
        ASSERT_EQ(str::join_path(std::array{"."}), ".");
        ASSERT_EQ(str::join_path(std::list{"A", "B", "C"}), "A/B/C");
    }
    SECTION("空") {
        ASSERT_EQ(str::join_path(std::vector<std::string_view>{}), "");
        ASSERT_EQ(str::join_path(std::vector<std::string_view>{""}), "");
        ASSERT_EQ(str::join_path(std::vector<std::string_view>{"", "", "abc"}), "abc");
        ASSERT_EQ(str::join_path(std::vector<std::string_view>{"", "", "abc", ""}), "abc");
        ASSERT_EQ(str::join_path(std::vector<std::string_view>{"abc", "", "", "def"}), "abc/def");
    }
    SECTION("绝对路径") {
        ASSERT_EQ(str::join_path(std::list<std::string_view>{"/"}), "/");
        ASSERT_EQ(str::join_path(std::list<std::string_view>{"/", ""}), "/");
        ASSERT_EQ(str::join_path(std::list<std::string_view>{"aaa", "/bbb", "ccc"}), "/bbb/ccc");
        ASSERT_EQ(str::join_path(std::list<std::string_view>{"/", "abc", "/def", "ghi"}), "/def/ghi");
    }
    SECTION("initialize_list") {
        ASSERT_EQ(str::join_path({"aaa", "bbb", "ccc"}), "aaa/bbb/ccc");
    }
}