#include "tester.hpp"

#include "str.hpp"


TEST(test_str, xxx_next_eol) {
    SECTION("一般情况:LF") {
        std::string_view s{"a\nb\nc"};
        SECTION("str::iter_next_eol") {
            str::size_type pos = 0;
            ASSERT_EQ(str::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 4);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
        }
        SECTION("str::find_next_eol") {
            ASSERT_EQ(str::find_next_eol(s, 0), (str::range_type{1, 1}));
            ASSERT_EQ(str::find_next_eol(s, 2), (str::range_type{3, 1}));
            ASSERT_EQ(str::find_next_eol(s, 5), (str::range_type{}));
            ASSERT_EQ(str::find_next_eol(s, 5), (str::range_type{}));
        }
    }
    SECTION("\\r+\\n") {
        std::string_view s{"a\r\nb\nc"};
        SECTION("str::iter_next_eol") {
            str::size_type pos = 0;
            ASSERT_EQ(str::iter_next_eol(s, pos), "\r\n");
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
        }
        SECTION("str::find_next_eol") {
            ASSERT_EQ(str::find_next_eol(s, 0), (str::range_type{1, 2}));
            ASSERT_EQ(str::find_next_eol(s, 3), (str::range_type{4, 1}));
            ASSERT_EQ(str::find_next_eol(s, 5), (str::range_type{}));
            ASSERT_EQ(str::find_next_eol(s, 6), (str::range_type{}));
        }
    }
    SECTION("\\r+\\r") {
        std::string_view s{"a\r\rb\nc"};
        SECTION("str::iter_next_eol") {
            str::size_type pos = 0;
            ASSERT_EQ(str::iter_next_eol(s, pos), "\r");
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::iter_next_eol(s, pos), "\r");
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
        }
        SECTION("str::find_next_eol") {
            ASSERT_EQ(str::find_next_eol(s, 0), (str::range_type{1, 1}));
            ASSERT_EQ(str::find_next_eol(s, 2), (str::range_type{2, 1}));
            ASSERT_EQ(str::find_next_eol(s, 3), (str::range_type{4, 1}));
            ASSERT_EQ(str::find_next_eol(s, 5), (str::range_type{}));
            ASSERT_EQ(str::find_next_eol(s, 6), (str::range_type{}));
        }
    }
    SECTION("\\n+\\n") {
        std::string_view s{"a\n\nb\nc"};
        SECTION("str::iter_next_eol") {
            str::size_type pos = 0;
            ASSERT_EQ(str::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
        }
        SECTION("str::find_next_eol") {
            ASSERT_EQ(str::find_next_eol(s, 0), (str::range_type{1, 1}));
            ASSERT_EQ(str::find_next_eol(s, 2), (str::range_type{2, 1}));
            ASSERT_EQ(str::find_next_eol(s, 3), (str::range_type{4, 1}));
            ASSERT_EQ(str::find_next_eol(s, 5), (str::range_type{}));
            ASSERT_EQ(str::find_next_eol(s, 6), (str::range_type{}));
        }
    }
    SECTION("\\n+\\r") {
        std::string_view s{"a\n\rb\nc"};
        SECTION("str::iter_next_eol") {
            str::size_type pos = 0;
            ASSERT_EQ(str::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 2);
            ASSERT_EQ(str::iter_next_eol(s, pos), "\r");
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
        }
        SECTION("str::find_next_eol") {
            ASSERT_EQ(str::find_next_eol(s, 0), (str::range_type{1, 1}));
            ASSERT_EQ(str::find_next_eol(s, 2), (str::range_type{2, 1}));
            ASSERT_EQ(str::find_next_eol(s, 3), (str::range_type{4, 1}));
            ASSERT_EQ(str::find_next_eol(s, 5), (str::range_type{}));
            ASSERT_EQ(str::find_next_eol(s, 6), (str::range_type{}));
        }
    }
    SECTION("空串") {
        std::string_view s{""};
        SECTION("str::iter_next_eol") {
            str::size_type pos = 0;
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 0);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 0);
        }
        SECTION("str::find_next_eol") {
            ASSERT_EQ(str::find_next_eol(s, 0), (str::range_type{}));
        }
    }
    SECTION("无任何换行符号") {
        std::string_view s{"abcdef"};
        SECTION("str::iter_next_eol") {
            str::size_type pos = 0;
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 6);
        }
        SECTION("str::find_next_eol") {
            ASSERT_EQ(str::find_next_eol(s, 0), (str::range_type{}));
            ASSERT_EQ(str::find_next_eol(s, 6), (str::range_type{}));
        }
    }
    SECTION("单一\\r") {
        std::string_view s{"\r"};
        SECTION("str::iter_next_eol") {
            str::size_type pos = 0;
            ASSERT_EQ(str::iter_next_eol(s, pos), "\r");
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 1);
        }
        SECTION("str::find_next_eol") {
            ASSERT_EQ(str::find_next_eol(s, 0), (str::range_type{0, 1}));
            ASSERT_EQ(str::find_next_eol(s, 1), (str::range_type{}));
        }
    }
    SECTION("单一\\n") {
        std::string_view s{"\n"};
        SECTION("str::iter_next_eol") {
            str::size_type pos = 0;
            ASSERT_EQ(str::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 1);
        }
        SECTION("str::find_next_eol") {
            ASSERT_EQ(str::find_next_eol(s, 0), (str::range_type{0, 1}));
            ASSERT_EQ(str::find_next_eol(s, 1), (str::range_type{}));
        }
    }
    SECTION("综合") {
        std::string_view s{"\n\r\n\r\r\r\n"};
        SECTION("str::iter_next_eol") {
            str::size_type pos = 0;
            ASSERT_EQ(str::iter_next_eol(s, pos), "\n");
            ASSERT_EQ(pos, 1);
            ASSERT_EQ(str::iter_next_eol(s, pos), "\r\n");
            ASSERT_EQ(pos, 3);
            ASSERT_EQ(str::iter_next_eol(s, pos), "\r");
            ASSERT_EQ(pos, 4);
            ASSERT_EQ(str::iter_next_eol(s, pos), "\r");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(str::iter_next_eol(s, pos), "\r\n");
            ASSERT_EQ(pos, 7);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 7);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 7);
        }
        SECTION("str::find_next_eol") {
            ASSERT_EQ(str::find_next_eol(s, 0), (str::range_type{0, 1}));
            ASSERT_EQ(str::find_next_eol(s, 1), (str::range_type{1, 2}));
            ASSERT_EQ(str::find_next_eol(s, 3), (str::range_type{3, 1}));
            ASSERT_EQ(str::find_next_eol(s, 4), (str::range_type{4, 1}));
            ASSERT_EQ(str::find_next_eol(s, 5), (str::range_type{5, 2}));
            ASSERT_EQ(str::find_next_eol(s, 7), (str::range_type{}));
        }
    }
    SECTION("pos超出字符串范围[1]") {
        std::string_view s{"a\nb\nc"};
        SECTION("str::iter_next_eol") {
            str::size_type pos = 5;
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
        }
        SECTION("str::find_next_eol") {
            ASSERT_EQ(str::find_next_eol(s, 5), (str::range_type{}));
        }
    }
    SECTION("pos超出字符串范围[2]") {
        std::string_view s{"a\nb\nc"};
        SECTION("str::iter_next_eol") {
            str::size_type pos = 10;
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
        }
        SECTION("str::find_next_eol") {
            ASSERT_EQ(str::find_next_eol(s, 10), (str::range_type{}));
        }
    }
    SECTION("pos超出字符串范围[3]") {
        std::string_view s{"a\nb\nc"};
        SECTION("str::iter_next_eol") {
            str::size_type pos = str::npos;
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
            ASSERT_EQ(str::iter_next_eol(s, pos), "");
            ASSERT_EQ(pos, 5);
        }
        SECTION("str::find_next_eol") {
            ASSERT_EQ(str::find_next_eol(s, str::npos), (str::range_type{}));
        }
    }
}