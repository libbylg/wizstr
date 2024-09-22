#include "catch2/catch_all.hpp"

#include "str.h"
#include "test-compares.h"
#include "view.h"

TEST_CASE("view::split_path") {
    SECTION("绝对路径") {
        REQUIRE(view::split_path("/") == std::vector<std::string_view>{"/"});
        REQUIRE(view::split_path("//") == std::vector<std::string_view>{"/"});
        REQUIRE(view::split_path("//abc/def") == std::vector<std::string_view>{"/", "abc", "def"});
        REQUIRE(view::split_path("//abc/def/") == std::vector<std::string_view>{"/", "abc", "def"});
    }
    SECTION("空串路径") {
        REQUIRE(view::split_path("") == std::vector<std::string_view>{});
    }
    SECTION("相对路径") {
        REQUIRE(view::split_path("abc/mpq/def.txt") == std::vector<std::string_view>{"abc", "mpq", "def.txt"});
        REQUIRE(view::split_path("abc/mpq/def.txt/") == std::vector<std::string_view>{"abc", "mpq", "def.txt"});
        REQUIRE(view::split_path("abc//mpq//def.txt//") == std::vector<std::string_view>{"abc", "mpq", "def.txt"});
    }
    SECTION("proc:1") {
        std::vector<std::string_view> result;
        view::split_path("abc/def/ghi/mpq//def.txt//", [&result](std::string_view item) -> int {
            result.emplace_back(item);
            if (result.size() >= 3) {
                return -1;
            }

            return 0;
        });
        REQUIRE(result == std::vector<std::string_view>{"abc", "def", "ghi"});
    }
    SECTION("proc:2") {
        std::vector<std::string_view> result;
        view::split_path("/abc/def/def.txt", [&result](std::string_view item) -> int {
            result.emplace_back(item);
            if (result.size() >= 1) {
                return -1;
            }

            return 0;
        });
        REQUIRE(result == std::vector<std::string_view>{"/"});
    }
}