#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::find_next_regex") {
    SECTION("不同起始位置") {
        REQUIRE(view::find_next_regex("abc1234def", "[0-9]+") == std::string("1234"));
        REQUIRE(view::find_next_regex("abc1234def", "[0-9]+", 3) == std::string("1234"));
        REQUIRE(view::find_next_regex("1234def789", "[0-9]+", 3) == std::string("4"));
        REQUIRE(view::find_next_regex("1234def789", "[0-9]+", 4) == std::string("789"));
        REQUIRE(view::find_next_regex("", "[0-9]+") == std::nullopt);
        REQUIRE(view::find_next_regex("abc----def", "[0-9]+") == std::nullopt);
        REQUIRE(view::find_next_regex("abc----def", "[0-9]+", 10) == std::nullopt);
        REQUIRE(view::find_next_regex("abc----def", "[0-9]+", view::npos) == std::nullopt);
    }
}

TEST_CASE("view::itr_next_regex") {
    SECTION("不同起始位置:从0开始连续查找") {
        size_t pos = 0;
        REQUIRE(view::iter_next_regex("abc1234def", pos, "[0-9]+") == std::string("1234"));
        REQUIRE(pos == 7);
        REQUIRE(view::iter_next_regex("abc1234def", pos, "[0-9]+") == std::nullopt);
        REQUIRE(pos == 10);
    }
    SECTION("不同起始位置:从指定位置开始连续查找") {
        size_t pos = 3;
        REQUIRE(view::iter_next_regex("abc1234def", pos, "[0-9]+") == std::string("1234"));
        REQUIRE(pos == 7);
        REQUIRE(view::iter_next_regex("abc1234def", pos, "[0-9]+") == std::nullopt);
        REQUIRE(pos == 10);
    }
    SECTION("不同起始位置:能找到多个") {
        size_t pos = 3;
        REQUIRE(view::iter_next_regex("1234def789", pos, "[0-9]+") == std::string("4"));
        REQUIRE(pos == 4);
        REQUIRE(view::iter_next_regex("1234def789", pos, "[0-9]+") == std::string("789"));
        REQUIRE(pos == 10);
        REQUIRE(view::iter_next_regex("1234def789", pos, "[0-9]+") == std::nullopt);
        REQUIRE(pos == 10);
    }
    SECTION("空串") {
        size_t pos = 0;
        REQUIRE(view::iter_next_regex("", pos, "[0-9]+") == std::nullopt);
        REQUIRE(pos == 0);
        REQUIRE(view::iter_next_regex("", pos, "[0-9]+") == std::nullopt);
        REQUIRE(pos == 0);
    }
    SECTION("字符串中没有任何匹配子串") {
        size_t pos = 0;
        REQUIRE(view::iter_next_regex("abc----def", pos, "[0-9]+") == std::nullopt);
        REQUIRE(pos == 10);
        REQUIRE(view::iter_next_regex("abc----def", pos, "[0-9]+") == std::nullopt);
        REQUIRE(pos == 10);
    }
    SECTION("字符串中没有任何匹配子串:指定位置超出范围") {
        size_t pos = 10;
        REQUIRE(view::iter_next_regex("abc----def", pos, "[0-9]+") == std::nullopt);
        REQUIRE(pos == 10);
        REQUIRE(view::iter_next_regex("abc----def", pos, "[0-9]+") == std::nullopt);
        REQUIRE(pos == 10);
    }
    SECTION("字符串中没有任何匹配子串:指定位置超出范围-npos") {
        size_t pos = view::npos;
        REQUIRE(view::iter_next_regex("abc----def", pos, "[0-9]+") == std::nullopt);
        REQUIRE(pos == 10);
        REQUIRE(view::iter_next_regex("abc----def", pos, "[0-9]+") == std::nullopt);
        REQUIRE(pos == 10);
    }
}