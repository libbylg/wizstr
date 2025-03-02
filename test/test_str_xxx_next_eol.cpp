#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, xxx_next_eol) {
    SECTION("一般情况:LF") {
        std::string_view s{"a\nb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            ASSERT_EQ(view::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(view::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 4);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
        }
        SECTION("view::find_next_eol") {
            ASSERT_EQ(view::find_next_eol(s, 0), "\n");
            ASSERT_EQ(view::find_next_eol(s, 2), "\n");
            ASSERT_EQ(view::find_next_eol(s, 5), "");
            ASSERT_EQ(view::find_next_eol(s, 5), "");
        }
    }
    SECTION("\\r+\\n") {
        std::string_view s{"a\r\nb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            ASSERT_EQ(view::iter_next_eol(s, pos), "\r\n");
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(view::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
        }
        SECTION("view::find_next_eol") {
            ASSERT_EQ(view::find_next_eol(s, 0), "\r\n");
            ASSERT_EQ(view::find_next_eol(s, 3), "\n");
            ASSERT_EQ(view::find_next_eol(s, 5), "");
            ASSERT_EQ(view::find_next_eol(s, 6), "");
        }
    }
    SECTION("\\r+\\r") {
        std::string_view s{"a\r\rb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            ASSERT_EQ(view::iter_next_eol(s, pos), "\r");
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(view::iter_next_eol(s, pos), "\r");
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(view::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
        }
        SECTION("view::find_next_eol") {
            ASSERT_EQ(view::find_next_eol(s, 0), "\r");
            ASSERT_EQ(view::find_next_eol(s, 2), "\r");
            ASSERT_EQ(view::find_next_eol(s, 3), "\n");
            ASSERT_EQ(view::find_next_eol(s, 5), "");
            ASSERT_EQ(view::find_next_eol(s, 6), "");
        }
    }
    SECTION("\\n+\\n") {
        std::string_view s{"a\n\nb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            ASSERT_EQ(view::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(view::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(view::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
        }
        SECTION("view::find_next_eol") {
            ASSERT_EQ(view::find_next_eol(s, 0), "\n");
            ASSERT_EQ(view::find_next_eol(s, 2), "\n");
            ASSERT_EQ(view::find_next_eol(s, 3), "\n");
            ASSERT_EQ(view::find_next_eol(s, 5), "");
            ASSERT_EQ(view::find_next_eol(s, 6), "");
        }
    }
    SECTION("\\n+\\r") {
        std::string_view s{"a\n\rb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            ASSERT_EQ(view::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(view::iter_next_eol(s, pos), "\r");
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(view::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
        }
        SECTION("view::find_next_eol") {
            ASSERT_EQ(view::find_next_eol(s, 0), "\n");
            ASSERT_EQ(view::find_next_eol(s, 2), "\r");
            ASSERT_EQ(view::find_next_eol(s, 3), "\n");
            ASSERT_EQ(view::find_next_eol(s, 5), "");
            ASSERT_EQ(view::find_next_eol(s, 6), "");
        }
    }
    SECTION("空串") {
        std::string_view s{""};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 0);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 0);
        }
        SECTION("view::find_next_eol") {
            ASSERT_EQ(view::find_next_eol(s, 0), "");
        }
    }
    SECTION("无任何换行符号") {
        std::string_view s{"abcdef"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
        }
        SECTION("view::find_next_eol") {
            ASSERT_EQ(view::find_next_eol(s, 0), "");
            ASSERT_EQ(view::find_next_eol(s, 6), "");
        }
    }
    SECTION("单一\\r") {
        std::string_view s{"\r"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            ASSERT_EQ(view::iter_next_eol(s, pos), "\r");
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 1);
        }
        SECTION("view::find_next_eol") {
            ASSERT_EQ(view::find_next_eol(s, 0), "\r");
            ASSERT_EQ(view::find_next_eol(s, 1), "");
        }
    }
    SECTION("单一\\n") {
        std::string_view s{"\n"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            ASSERT_EQ(view::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 1);
        }
        SECTION("view::find_next_eol") {
            ASSERT_EQ(view::find_next_eol(s, 0), "\n");
            ASSERT_EQ(view::find_next_eol(s, 1), "");
        }
    }
    SECTION("综合") {
        std::string_view s{"\n\r\n\r\r\r\n"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 0;
            ASSERT_EQ(view::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(view::iter_next_eol(s, pos), "\r\n");
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(view::iter_next_eol(s, pos), "\r");
            ASSERT_EQ(pos, 4);
            ASSERT_EQ(view::iter_next_eol(s, pos), "\r");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(view::iter_next_eol(s, pos), "\r\n");
            ASSERT_EQ(pos, 7);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 7);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 7);
        }
        SECTION("view::find_next_eol") {
            ASSERT_EQ(view::find_next_eol(s, 0), "\n");
            ASSERT_EQ(view::find_next_eol(s, 1), "\r\n");
            ASSERT_EQ(view::find_next_eol(s, 3), "\r");
            ASSERT_EQ(view::find_next_eol(s, 4), "\r");
            ASSERT_EQ(view::find_next_eol(s, 5), "\r\n");
            ASSERT_EQ(view::find_next_eol(s, 7), "");
        }
    }
    SECTION("pos超出字符串范围[1]") {
        std::string_view s{"a\nb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 5;
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
        }
        SECTION("view::find_next_eol") {
            ASSERT_EQ(view::find_next_eol(s, 5), "");
        }
    }
    SECTION("pos超出字符串范围[2]") {
        std::string_view s{"a\nb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = 10;
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
        }
        SECTION("view::find_next_eol") {
            ASSERT_EQ(view::find_next_eol(s, 10), "");
        }
    }
    SECTION("pos超出字符串范围[3]") {
        std::string_view s{"a\nb\nc"};
        SECTION("view::iter_next_eol") {
            view::size_type pos = view::npos;
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(view::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
        }
        SECTION("view::find_next_eol") {
            ASSERT_EQ(view::find_next_eol(s, view::npos), "");
        }
    }
}