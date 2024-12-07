#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::expand_envs") {
    SECTION("${xxx}形式") {
        REQUIRE(view::expand_envs("${HOME}/${NOTEXIST}") == std::string(getenv("HOME")) + "/");
        REQUIRE(view::expand_envs("${HOME}/${NOTEXIST}", true) == std::string(getenv("HOME")) + "/${NOTEXIST}");

    }

    SECTION("$xxx形式") {
        REQUIRE(view::expand_envs("$HOME/$NOTEXIST") == std::string(getenv("HOME")) + "/");
        REQUIRE(view::expand_envs("$HOME/$NOTEXIST", true) == std::string(getenv("HOME")) + "/$NOTEXIST");
    }

    SECTION("字符串中间") {
        REQUIRE(view::expand_envs("abcd$HOME/${NOTEXIST}") == "abcd" + std::string(getenv("HOME")) + "/");
        REQUIRE(view::expand_envs("abcd$HOME/${NOTEXIST}", true) == "abcd" + std::string(getenv("HOME")) + "/${NOTEXIST}");
    }

    SECTION("${xxx}在字符串尾部") {
        REQUIRE(view::expand_envs("abcd${HOME}") == "abcd" + std::string(getenv("HOME")));
        REQUIRE(view::expand_envs("abcd${HOME}", true) == "abcd" + std::string(getenv("HOME")));
    }

    SECTION("一个字符串只有 $xxx") {
        REQUIRE(view::expand_envs("$HOME") == std::string(getenv("HOME")));
        REQUIRE(view::expand_envs("$HOME", true) == std::string(getenv("HOME")));
    }

    SECTION("错误变量") {
        REQUIRE(view::expand_envs("$$$") == "$$$");
        REQUIRE(view::expand_envs("$$$", true) == "$$$");
        REQUIRE(view::expand_envs("${HOME aaa") == "${HOME aaa");
        REQUIRE(view::expand_envs("${HOME aaa", true) == "${HOME aaa");
        REQUIRE(view::expand_envs("abc$") == "abc$");
        REQUIRE(view::expand_envs("abc$", true) == "abc$");
    }

    SECTION("空串") {
        REQUIRE(view::expand_envs("") == "");
        REQUIRE(view::expand_envs("", true) == "");
    }

    SECTION("无可替换变量") {
        REQUIRE(view::expand_envs("Hello world") == "Hello world");
        REQUIRE(view::expand_envs("Hello world", true) == "Hello world");
    }

    SECTION("通过map提供数据") {
        std::map<std::string, std::string> items {
            {"HOME", "XXX"},
        };
        REQUIRE(view::expand_envs("${HOME}/${NOTEXIST}", items) == "XXX/");
        REQUIRE(view::expand_envs("${HOME}/${NOTEXIST}", true, items) == "XXX/${NOTEXIST}");
    }

    SECTION("通过key-value参数提供数据") {
        REQUIRE(view::expand_envs("${HOME}/${NOTEXIST}", "HOME", "XXX") == "XXX/${NOTEXIST}");
    }

}
