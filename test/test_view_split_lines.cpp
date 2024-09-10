#include "catch2/catch_all.hpp"

#include "str.h"
#include "test-compares.h"
#include "view.h"

#include <cstdio>

TEST_CASE("view::split_lines:keep_ends=false") {
    SECTION("简单场景") {
        std::string s{"\na\r\nb\ncccc\n"};
        std::vector<std::string_view> expect = {"", "a", "b", "cccc"};

        std::vector<std::string_view> result;
        view::split_lines(s, false, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, false) == expect);
    }
    SECTION("空串") {
        std::string s{""};
        std::vector<std::string_view> expect = {};

        std::vector<std::string_view> result;
        view::split_lines(s, false, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(expect == result);
        REQUIRE(view::split_lines(s, false) == expect);
    }
    SECTION("各种换行场景") {
        std::string s{"\n\r\r\n\r"};
        std::vector<std::string_view> expect = {"", "", "", ""};

        std::vector<std::string_view> result;
        view::split_lines(s, false, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, false) == expect);
    }
    SECTION("综合测试") {
        std::string s{"a\n\ryyy\r\r\nb\n\nc\n\r"};
        std::vector<std::string_view> expect = {"a", "", "yyy", "", "b", "", "c", ""};

        std::vector<std::string_view> result;
        view::split_lines(s, false, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, false) == expect);
    }
}

TEST_CASE("view::split_lines:keep_ends=true") {
    SECTION("简单场景") {
        std::string s{"\na\r\nb\ncccc\n"};
        std::vector<std::string_view> expect = {"\n", "a\r\n", "b\n", "cccc\n"};

        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("空串") {
        std::string s{""};
        std::vector<std::string_view> expect = {};

        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("纯换行") {
        std::string s{"\n"};
        std::vector<std::string_view> expect = {"\n"};

        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, true) == expect);
    }

    SECTION("综合测试") {
        std::string s{"a\n\ryyy\r\r\nb\n\nc\n\r"};
        std::vector<std::string_view> expect = {"a\n", "\r", "yyy\r", "\r\n", "b\n", "\n", "c\n", "\r"};

        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("最后一行没有换行符号") {
        std::string s{"a\nyyy\nb\n\ncdef"};
        std::vector<std::string_view> expect = {"a\n", "yyy\n", "b\n", "\n", "cdef"};

        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("单行无换行符") {
        std::string s{"ayyynbncdef"};
        std::vector<std::string_view> expect = {"ayyynbncdef"};

        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("提前终止") {
        std::string s{"a\nyyy\nb\n\ncdef"};
        std::vector<std::string_view> expect = {"a\n", "yyy\n", "b\n"};

        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return (result.size() < 3 ? 0 : -1);
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, true) == std::vector<std::string_view>{"a\n", "yyy\n", "b\n", "\n", "cdef"});
    }
}

enum Status {
    ST_INITED = 0,
    ST_SUFFIX = 1,
    ST_PREFIX = 2,
};

class XSLineReader {
public:
    XSLineReader(std::function<bool(std::string&)>&& provider)
        : provider_{std::move(provider)} {
    }

    auto ReadLine(std::string& resultLine) -> int32_t {
        resultLine.clear();

        std::string currline = cachedLine_;
        Status currStatus = cachedStatus_;

        while (provider_(currline)) {
            // 递增行号
            lineno_++;

            // 去掉首尾空白
            std::string_view tidyLine = view::trim_surrounding(currline);

            // 初始状态
            if (currStatus == ST_INITED) {
                // 初始状态如果遇到空行或者注释行
                if (tidyLine.empty() || view::starts_with(tidyLine, "//")) {
                    continue; // 维持当前状态
                }

                // " xxxxx \ " 遇到行尾续行状态
                if (view::ends_with(tidyLine, '\\')) {
                    resultLine.append(tidyLine.data(), tidyLine.size() - 1);
                    currStatus = ST_SUFFIX; // 转到行尾续行状态
                    continue;
                }

                // 结束状态
                resultLine.append(tidyLine.data(), tidyLine.size());
                currStatus = ST_PREFIX;
                continue;
            }

            // 尾续行状态
            if (currStatus == ST_SUFFIX) {
                // " xxxxx \ " 有行尾标识符:继续续行
                if (view::ends_with(tidyLine, '\\')) {
                    resultLine.append(" ");
                    resultLine.append(tidyLine.data(), tidyLine.size() - 1);
                    resultLine.append(" ");
                    currStatus = ST_SUFFIX; // 维持当前状态
                    continue;
                }

                // "  xxxxx " 无行尾标识符:续行结束了
                resultLine.append(" ");
                resultLine.append(tidyLine.data(), tidyLine.size());
                resultLine.append(" ");
                currStatus = ST_PREFIX; // 转换到行首续行状态

                continue;
            }

            // 首续行状态
            assert(currStatus == ST_PREFIX);
            if (view::starts_with(tidyLine, '+')) {
                //                // 首续行之前没有任何前缀，这种语法错误的
                //                if (resultLine.empty()) {
                //                    return -1;
                //                }

                // "+  xxxxx \ " 同时有行首和行尾标识符:追加到当前行,并且继续续行
                if (view::ends_with(tidyLine, '\\')) {
                    resultLine.append(" ");
                    resultLine.append(tidyLine.data() + 1, tidyLine.size() - 2);
                    resultLine.append(" ");
                    currStatus = ST_SUFFIX; // 转换到尾续行状态
                    continue;
                }

                // "+ xxxx" 只有行首标识符:
                resultLine.append(" ");
                resultLine.append(tidyLine.data() + 1, tidyLine.size() - 1);
                resultLine.append(" ");
                currStatus = ST_PREFIX; // 维持当前首续行状态
                continue;
            }

            // " xxxxx \ " 只有行尾标识符: 当前状态结束，下一条语句开始
            if (view::ends_with(tidyLine, '\\')) {
                cachedLine_ = std::string_view{tidyLine.data(), tidyLine.size() - 1};
                cachedStatus_ = ST_SUFFIX;
                return 0; // RET
            }

            // "  xxxxx " 行首、行尾标识符都不存在
            cachedLine_ = std::string_view{tidyLine.data(), tidyLine.size() - 1};
            cachedStatus_ = ST_PREFIX;
            return 0; // RET
        }

        // 如果遇到文件结束，无论如何将当前数据提交
        if (resultLine.empty()) {
            return -1;
        }

        return 0;
    }

private:
    std::function<bool(std::string&)> provider_;
    std::string cachedLine_;
    Status cachedStatus_{ST_INITED};

    int32_t lineno_{0};
};

TEST_CASE("view::read_lines:xs") {
    std::vector<std::string> originLines{
        "r1 (1 2) resistor r=1\n",
        "+ p1=1\n",
    };

    size_t lineno = 0;
    XSLineReader reader([&originLines, &lineno](std::string& line) -> bool {
        if (lineno >= originLines.size()) {
            return false;
        }

        line = originLines[lineno++];
        return true;
    });

    std::string line;
    REQUIRE(reader.ReadLine(line) == 0);
    REQUIRE(line == "r1 (1 2) resistor r=1 p1=1");
}