#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::xxx_next_eol") {
    SECTION("一般情况:LF") {
        std::string_view s{"a\nb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            REQUIRE(view::iter_next_eol(s, pos) == "\n");
            REQUIRE(pos == 2);
            REQUIRE(view::iter_next_eol(s, pos) == "\n");
            REQUIRE(pos == 4);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 5);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 5);
        }
        SECTION("view::find_next_eol") {
            REQUIRE(view::find_next_eol(s, 0) == "\n");
            REQUIRE(view::find_next_eol(s, 2) == "\n");
            REQUIRE(view::find_next_eol(s, 5) == "");
            REQUIRE(view::find_next_eol(s, 5) == "");
        }
    }
    SECTION("\\r+\\n") {
        std::string_view s{"a\r\nb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            REQUIRE(view::iter_next_eol(s, pos) == "\r\n");
            REQUIRE(pos == 3);
            REQUIRE(view::iter_next_eol(s, pos) == "\n");
            REQUIRE(pos == 5);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 6);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 6);
        }
        SECTION("view::find_next_eol") {
            REQUIRE(view::find_next_eol(s, 0) == "\r\n");
            REQUIRE(view::find_next_eol(s, 3) == "\n");
            REQUIRE(view::find_next_eol(s, 5) == "");
            REQUIRE(view::find_next_eol(s, 6) == "");
        }
    }
    SECTION("\\r+\\r") {
        std::string_view s{"a\r\rb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            REQUIRE(view::iter_next_eol(s, pos) == "\r");
            REQUIRE(pos == 2);
            REQUIRE(view::iter_next_eol(s, pos) == "\r");
            REQUIRE(pos == 3);
            REQUIRE(view::iter_next_eol(s, pos) == "\n");
            REQUIRE(pos == 5);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 6);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 6);
        }
        SECTION("view::find_next_eol") {
            REQUIRE(view::find_next_eol(s, 0) == "\r");
            REQUIRE(view::find_next_eol(s, 2) == "\r");
            REQUIRE(view::find_next_eol(s, 3) == "\n");
            REQUIRE(view::find_next_eol(s, 5) == "");
            REQUIRE(view::find_next_eol(s, 6) == "");
        }
    }
    SECTION("\\n+\\n") {
        std::string_view s{"a\n\nb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            REQUIRE(view::iter_next_eol(s, pos) == "\n");
            REQUIRE(pos == 2);
            REQUIRE(view::iter_next_eol(s, pos) == "\n");
            REQUIRE(pos == 3);
            REQUIRE(view::iter_next_eol(s, pos) == "\n");
            REQUIRE(pos == 5);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 6);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 6);
        }
        SECTION("view::find_next_eol") {
            REQUIRE(view::find_next_eol(s, 0) == "\n");
            REQUIRE(view::find_next_eol(s, 2) == "\n");
            REQUIRE(view::find_next_eol(s, 3) == "\n");
            REQUIRE(view::find_next_eol(s, 5) == "");
            REQUIRE(view::find_next_eol(s, 6) == "");
        }
    }
    SECTION("\\n+\\r") {
        std::string_view s{"a\n\rb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            REQUIRE(view::iter_next_eol(s, pos) == "\n");
            REQUIRE(pos == 2);
            REQUIRE(view::iter_next_eol(s, pos) == "\r");
            REQUIRE(pos == 3);
            REQUIRE(view::iter_next_eol(s, pos) == "\n");
            REQUIRE(pos == 5);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 6);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 6);
        }
        SECTION("view::find_next_eol") {
            REQUIRE(view::find_next_eol(s, 0) == "\n");
            REQUIRE(view::find_next_eol(s, 2) == "\r");
            REQUIRE(view::find_next_eol(s, 3) == "\n");
            REQUIRE(view::find_next_eol(s, 5) == "");
            REQUIRE(view::find_next_eol(s, 6) == "");
        }
    }
    SECTION("空串") {
        std::string_view s{""};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 0);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 0);
        }
        SECTION("view::find_next_eol") {
            REQUIRE(view::find_next_eol(s, 0) == "");
        }
    }
    SECTION("无任何换行符号") {
        std::string_view s{"abcdef"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 6);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 6);
        }
        SECTION("view::find_next_eol") {
            REQUIRE(view::find_next_eol(s, 0) == "");
            REQUIRE(view::find_next_eol(s, 6) == "");
        }
    }
    SECTION("单一\\r") {
        std::string_view s{"\r"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            REQUIRE(view::iter_next_eol(s, pos) == "\r");
            REQUIRE(pos == 1);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 1);
        }
        SECTION("view::find_next_eol") {
            REQUIRE(view::find_next_eol(s, 0) == "\r");
            REQUIRE(view::find_next_eol(s, 1) == "");
        }
    }
    SECTION("单一\\n") {
        std::string_view s{"\n"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            REQUIRE(view::iter_next_eol(s, pos) == "\n");
            REQUIRE(pos == 1);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 1);
        }
        SECTION("view::find_next_eol") {
            REQUIRE(view::find_next_eol(s, 0) == "\n");
            REQUIRE(view::find_next_eol(s, 1) == "");
        }
    }
    SECTION("综合") {
        std::string_view s{"\n\r\n\r\r\r\n"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            REQUIRE(view::iter_next_eol(s, pos) == "\n");
            REQUIRE(pos == 1);
            REQUIRE(view::iter_next_eol(s, pos) == "\r\n");
            REQUIRE(pos == 3);
            REQUIRE(view::iter_next_eol(s, pos) == "\r");
            REQUIRE(pos == 4);
            REQUIRE(view::iter_next_eol(s, pos) == "\r");
            REQUIRE(pos == 5);
            REQUIRE(view::iter_next_eol(s, pos) == "\r\n");
            REQUIRE(pos == 7);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 7);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 7);
        }
        SECTION("view::find_next_eol") {
            REQUIRE(view::find_next_eol(s, 0) == "\n");
            REQUIRE(view::find_next_eol(s, 1) == "\r\n");
            REQUIRE(view::find_next_eol(s, 3) == "\r");
            REQUIRE(view::find_next_eol(s, 4) == "\r");
            REQUIRE(view::find_next_eol(s, 5) == "\r\n");
            REQUIRE(view::find_next_eol(s, 7) == "");
        }
    }
    SECTION("pos超出字符串范围[1]") {
        std::string_view s{"a\nb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 5;
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 5);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 5);
        }
        SECTION("view::find_next_eol") {
            REQUIRE(view::find_next_eol(s, 5) == "");
        }
    }
    SECTION("pos超出字符串范围[2]") {
        std::string_view s{"a\nb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 10;
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 5);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 5);
        }
        SECTION("view::find_next_eol") {
            REQUIRE(view::find_next_eol(s, 10) == "");
        }
    }
    SECTION("pos超出字符串范围[3]") {
        std::string_view s{"a\nb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = view::npos;
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 5);
            REQUIRE(view::iter_next_eol(s, pos) == "");
            REQUIRE(pos == 5);
        }
        SECTION("view::find_next_eol") {
            REQUIRE(view::find_next_eol(s, view::npos) == "");
        }
    }
}