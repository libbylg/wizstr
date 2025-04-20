#include "tester.hpp"

#include "str.hpp"


#include <list>

TEST(test_str, join_searchpath) {
    SECTION("一般场景") {
        ASSERT_EQ(str::join_searchpath({"A", "B", "C"}), "A:B:C");
        ASSERT_EQ(str::join_searchpath({"A", "B"}), "A:B");
        ASSERT_EQ(str::join_searchpath({"A"}), "A");
    }
    SECTION("空串") {
        ASSERT_EQ(str::join_searchpath({""}), "");
        ASSERT_EQ(str::join_searchpath({"", ""}), "");
        ASSERT_EQ(str::join_searchpath({"", "", "AAA"}), "AAA");
        ASSERT_EQ(str::join_searchpath({"", "A", "B", "", "C", ""}), "A:B:C");
    }
}

TEST(test_str, split_searchpath) {
    SECTION("空串") {
        ASSERT_EQ(str::split_searchpath(""), (std::vector<std::string_view>{}));
    }
    SECTION("空路径") {
        ASSERT_EQ(str::split_searchpath(":::"), (std::vector<std::string_view>{}));
        ASSERT_EQ(str::split_searchpath(":::", true), (std::vector<std::string_view>{"", "", "", ""}));

        ASSERT_EQ(str::split_searchpath(":  : :"), (std::vector<std::string_view>{"  ", " "}));
        ASSERT_EQ(str::split_searchpath(":  : :", true), (std::vector<std::string_view>{"", "  ", " ", ""}));

        ASSERT_EQ(str::split_searchpath(":aaa"), (std::vector<std::string_view>{"aaa"}));
        ASSERT_EQ(str::split_searchpath(":aaa", true), (std::vector<std::string_view>{"", "aaa"}));

        ASSERT_EQ(str::split_searchpath("abc:def:hij"), (std::vector<std::string_view>{"abc", "def", "hij"}));
        ASSERT_EQ(str::split_searchpath("abc:def:hij", true), (std::vector<std::string_view>{"abc", "def", "hij"}));

        ASSERT_EQ(str::split_searchpath("abc::hij"), (std::vector<std::string_view>{"abc", "hij"}));
        ASSERT_EQ(str::split_searchpath("abc::hij", true), (std::vector<std::string_view>{"abc", "", "hij"}));
    }
}
