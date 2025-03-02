#include "tester.hpp"

#include "str.hpp"


#include <list>

TEST(test_str, join_search_path) {
    SECTION("一般场景") {
        ASSERT_EQ(str::join_search_path({"A", "B", "C"}), "A:B:C");
        ASSERT_EQ(str::join_search_path({"A", "B"}), "A:B");
        ASSERT_EQ(str::join_search_path({"A"}), "A");
    }
    SECTION("空串") {
        ASSERT_EQ(str::join_search_path({""}), "");
        ASSERT_EQ(str::join_search_path({"", ""}), "");
        ASSERT_EQ(str::join_search_path({"", "", "AAA"}), "AAA");
        ASSERT_EQ(str::join_search_path({"", "A", "B", "", "C", ""}), "A:B:C");
    }
}

TEST(test_str, split_search_path) {
    SECTION("空串") {
        ASSERT_EQ(str::split_search_path(""), (std::vector<std::string_view>{}));
    }
    SECTION("空路径") {
        ASSERT_EQ(str::split_search_path(":::"), (std::vector<std::string_view>{}));
        ASSERT_EQ(str::split_search_path(":::", true), (std::vector<std::string_view>{"", "", "", ""}));

        ASSERT_EQ(str::split_search_path(":  : :"), (std::vector<std::string_view>{"  ", " "}));
        ASSERT_EQ(str::split_search_path(":  : :", true), (std::vector<std::string_view>{"", "  ", " ", ""}));

        ASSERT_EQ(str::split_search_path(":aaa"), (std::vector<std::string_view>{"aaa"}));
        ASSERT_EQ(str::split_search_path(":aaa", true), (std::vector<std::string_view>{"", "aaa"}));

        ASSERT_EQ(str::split_search_path("abc:def:hij"), (std::vector<std::string_view>{"abc", "def", "hij"}));
        ASSERT_EQ(str::split_search_path("abc:def:hij", true), (std::vector<std::string_view>{"abc", "def", "hij"}));

        ASSERT_EQ(str::split_search_path("abc::hij"), (std::vector<std::string_view>{"abc", "hij"}));
        ASSERT_EQ(str::split_search_path("abc::hij", true), (std::vector<std::string_view>{"abc", "", "hij"}));
    }
}
