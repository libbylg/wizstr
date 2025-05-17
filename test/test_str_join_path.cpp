#include "testing.hpp"

#include "str.hpp"
#include "test-to_proc.hpp"

#include <list>


TEST(test_str, join_path) {
    SECTION("容器模式：一般场景") {
        ASSERT_EQ(str::join_path(std::vector{"A"}), "A");
        ASSERT_EQ(str::join_path(std::array{"."}), ".");
        ASSERT_EQ(str::join_path(std::list{"A", "B", "C"}), "A/B/C");
    }
    SECTION("容器模式：空") {
        ASSERT_EQ(str::join_path(std::vector<std::string_view>{}), "");
        ASSERT_EQ(str::join_path(std::vector<std::string_view>{""}), "");
        ASSERT_EQ(str::join_path(std::vector<std::string_view>{"", "", "abc"}), "abc");
        ASSERT_EQ(str::join_path(std::vector<std::string_view>{"", "", "abc", ""}), "abc");
        ASSERT_EQ(str::join_path(std::vector<std::string_view>{"abc", "", "", "def"}), "abc/def");
    }
    SECTION("容器模式：绝对路径") {
        ASSERT_EQ(str::join_path(std::list<std::string_view>{"/"}), "/");
        ASSERT_EQ(str::join_path(std::list<std::string_view>{"/", ""}), "/");
        ASSERT_EQ(str::join_path(std::list<std::string_view>{"aaa", "/bbb", "ccc"}), "/bbb/ccc");
        ASSERT_EQ(str::join_path(std::list<std::string_view>{"/", "abc", "/def", "ghi"}), "/def/ghi");
    }
    SECTION("容器模式：initialize_list") {
        ASSERT_EQ(str::join_path({"aaa", "bbb", "ccc"}), "aaa/bbb/ccc");
    }
    SECTION("容器模式：重复斜杠问题") {
        ASSERT_EQ(str::join_path({"aaa/", "bbb/", "ccc/"}), "aaa/bbb/ccc/");
        ASSERT_EQ(str::join_path({"/aaa/", "/bbb/", "/ccc/"}), "/ccc/");
        ASSERT_EQ(str::join_path({"/aaa/", "/bbb/", "ccc/", "ddd"}), "/bbb/ccc/ddd");
        ASSERT_EQ(str::join_path({"/aaa/", "bbb//ccc/", "ddd///eee"}), "/aaa/bbb//ccc/ddd///eee");
        ASSERT_EQ(str::join_path({"/", "", ""}), "/");
        ASSERT_EQ(str::join_path({"/", "", "/"}), "/");
    }

    // ================

    SECTION("容器模式：一般场景") {
        ASSERT_EQ(str::join_path(to_proc{std::vector{"A"}}), "A");
        ASSERT_EQ(str::join_path(to_proc{std::array{"."}}), ".");
        ASSERT_EQ(str::join_path(to_proc{std::list{"A", "B", "C"}}), "A/B/C");
    }
    SECTION("容器模式：空") {
        ASSERT_EQ(str::join_path(to_proc{std::vector<std::string_view>{}}), "");
        ASSERT_EQ(str::join_path(to_proc{std::vector<std::string_view>{""}}), "");
        ASSERT_EQ(str::join_path(to_proc{std::vector<std::string_view>{"", "", "abc"}}), "abc");
        ASSERT_EQ(str::join_path(to_proc{std::vector<std::string_view>{"", "", "abc", ""}}), "abc");
        ASSERT_EQ(str::join_path(to_proc{std::vector<std::string_view>{"abc", "", "", "def"}}), "abc/def");
    }
    SECTION("容器模式：绝对路径") {
        ASSERT_EQ(str::join_path(to_proc{std::list<std::string_view>{"/"}}), "/");
        ASSERT_EQ(str::join_path(to_proc{std::list<std::string_view>{"/", ""}}), "/");
        ASSERT_EQ(str::join_path(to_proc{std::list<std::string_view>{"aaa", "/bbb", "ccc"}}), "/bbb/ccc");
        ASSERT_EQ(str::join_path(to_proc{std::list<std::string_view>{"/", "abc", "/def", "ghi"}}), "/def/ghi");
    }
    SECTION("容器模式：initialize_list") {
        ASSERT_EQ(str::join_path(to_proc{std::initializer_list{"aaa", "bbb", "ccc"}}), "aaa/bbb/ccc");
    }
    SECTION("容器模式：重复斜杠问题") {
        ASSERT_EQ(str::join_path(to_proc{std::vector{"aaa/", "bbb/", "ccc/"}}), "aaa/bbb/ccc/");
        ASSERT_EQ(str::join_path(to_proc{std::vector{"/aaa/", "/bbb/", "/ccc/"}}), "/ccc/");
        ASSERT_EQ(str::join_path(to_proc{std::vector{"/aaa/", "/bbb/", "ccc/", "ddd"}}), "/bbb/ccc/ddd");
        ASSERT_EQ(str::join_path(to_proc{std::vector{"/aaa/", "bbb//ccc/", "ddd///eee"}}), "/aaa/bbb//ccc/ddd///eee");
        ASSERT_EQ(str::join_path(to_proc{std::vector{"/", "", ""}}), "/");
        ASSERT_EQ(str::join_path(to_proc{std::vector{"/", "", "/"}}), "/");
    }
}