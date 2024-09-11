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

class MemoryLineProvider {
public:
    MemoryLineProvider(std::initializer_list<std::string_view> lines) {
        for (auto item : lines) {
            lines_.emplace_back(item);
        }
    }

    auto operator()(std::string& line) -> bool {
        if (lineno_ >= lines_.size()) {
            return false;
        }

        line = lines_[lineno_++];
        return true;
    }

private:
    size_t lineno_{0};
    std::vector<std::string> lines_;
};

class FileLineProvider {
public:
    explicit FileLineProvider(const std::string& filename) {
        file_ = fopen(filename.c_str(), "r");
        assert(file_ != nullptr);
    }

    auto operator()(std::string& line) -> bool {
        line = str::read_line(file_, true);
        if (line.empty()) {
            return false;
        }

        return true;
    }

private:
    size_t lineno_{0};
    FILE* file_{nullptr};
};

class XSLineReader {
public:
    XSLineReader(std::function<bool(std::string&)>&& provider)
        : provider_{std::move(provider)} {
    }

    auto ReadLine(std::string& resultLine) -> int32_t {
        resultLine.clear();

        std::string currLine = cachedLine_;
        Status currStatus = cachedStatus_;
        int32_t currLineno = cachedLineno_;

        int32_t startLineno = currLineno;
        while (provider_(currLine)) {
            // 递增行号
            currLineno++;

            // 去掉首尾空白
            std::string_view tidyLine = view::trim_surrounding(currLine);

            // 初始状态
            if (currStatus == ST_INITED) {
                assert(cachedLine_.empty());
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
                    currStatus = ST_SUFFIX; // 维持当前状态
                    continue;
                }

                // "  xxxxx " 无行尾标识符:续行结束了
                resultLine.append(" ");
                resultLine.append(tidyLine.data(), tidyLine.size());
                currStatus = ST_PREFIX; // 转换到行首续行状态

                continue;
            }

            // 首续行状态
            assert(currStatus == ST_PREFIX);

            // 在首续行状态，如果遇到是空行或者注释行:忽略该行
            if (tidyLine.empty() || view::starts_with(tidyLine, "//")) {
                continue;
            }

            if (view::starts_with(tidyLine, '+')) {
                // "+  xxxxx \ " 同时有行首和行尾标识符:追加到当前行,并且继续续行
                if (view::ends_with(tidyLine, '\\')) {
                    resultLine.append(" ");
                    resultLine.append(tidyLine.data() + 1, tidyLine.size() - 2);
                    currStatus = ST_SUFFIX; // 转换到尾续行状态
                    continue;
                }

                // "+ xxxx" 只有行首标识符:
                resultLine.append(" ");
                resultLine.append(tidyLine.data() + 1, tidyLine.size() - 1);
                currStatus = ST_PREFIX; // 维持当前首续行状态
                continue;
            }

            // " xxxxx \ " 只有行尾标识符: 当前状态结束，下一条语句开始
            if (view::ends_with(tidyLine, '\\')) {
                cachedLine_ = std::string_view{tidyLine.data(), tidyLine.size() - 1};
                cachedStatus_ = ST_SUFFIX;
                return lineno_; // RET
            }

            // "  xxxxx " 行首、行尾标识符都不存在
            cachedLine_ = std::string_view{tidyLine.data(), tidyLine.size() - 1};
            cachedStatus_ = ST_PREFIX;
            return lineno_; // RET
        }

        // 如果遇到文件结束，无论如何将当前数据提交
        if (resultLine.empty()) {
            return 0; // 文件结束
        }

        return lineno_;
    }

private:
    enum Status {
        ST_INITED = 0,
        ST_SUFFIX = 1,
        ST_PREFIX = 2,
    };

    std::function<bool(std::string&)> provider_;
    std::string cachedLine_;
    Status cachedStatus_{ST_INITED};
    int32_t cachedLineno_{0};

    //int32_t lineno_{0};
};

TEST_CASE("view::read_lines:xs") {
    SECTION("一般首续行场景") {
        XSLineReader reader(MemoryLineProvider{
            "r1 (1 2) resistor r=1\n",
            "+p1=1\n",
            "+p2=2\n",
        });

        std::string line;

        REQUIRE(reader.ReadLine(line) == 1);
        REQUIRE(line == "r1 (1 2) resistor r=1 p1=1 p2=2");

        REQUIRE(reader.ReadLine(line) == 0);
        REQUIRE(line.empty());
    }
    SECTION("一般尾续行场景") {
        XSLineReader reader(MemoryLineProvider{
            "r1 (1 2) resistor r=1\\\n",
            "p1=1\\\n",
            "p2=2\n",
        });

        std::string line;

        REQUIRE(reader.ReadLine(line) == 1);
        REQUIRE(line == "r1 (1 2) resistor r=1 p1=1 p2=2");

        REQUIRE(reader.ReadLine(line) == 0);
        REQUIRE(line.empty());
    }
    SECTION("尾续行遇到首续行：首续行字符失效") {
        XSLineReader reader(MemoryLineProvider{
            "r1 (1 2) resistor r=1\\\n",
            "+ p1=1\n",
        });

        std::string line;

        REQUIRE(reader.ReadLine(line) == 1);
        REQUIRE(line == "r1 (1 2) resistor r=1 + p1=1");

        REQUIRE(reader.ReadLine(line) == 0);
        REQUIRE(line.empty());
    }
    SECTION("尾续行遇到尾续行：简单场景") {
        XSLineReader reader(MemoryLineProvider{
            "r1 (1 2) resistor r=1\\\n",
            "p1=1\\\n",
            "p2=2\n", // <<<
        });

        std::string line;

        REQUIRE(reader.ReadLine(line) == 1);
        REQUIRE(line == "r1 (1 2) resistor r=1 p1=1 p2=2");

        REQUIRE(reader.ReadLine(line) == 0);
        REQUIRE(line.empty());
    }
    SECTION("尾续行后面遇到文件结束：尾续行不跨文件") {
        XSLineReader reader(MemoryLineProvider{
            "r1 (1 2) resistor r=1\\\n",
            "p1=1\\\n",
            "p2=2\\\n", //
        });

        std::string line;

        REQUIRE(reader.ReadLine(line) == 1);
        REQUIRE(line == "r1 (1 2) resistor r=1 p1=1 p2=2");

        REQUIRE(reader.ReadLine(line) == 0);
        REQUIRE(line.empty());
    }

    SECTION("首续行遇到首续行：简单场景") {
        XSLineReader reader(MemoryLineProvider{
            "r1 (1 2) resistor r=1\n",
            "+p1=1\n", //
            "+p2=2\n", //
            "+p3=3\n", //
        });

        std::string line;

        REQUIRE(reader.ReadLine(line) == 1);
        REQUIRE(line == "r1 (1 2) resistor r=1 p1=1 p2=2 p3=3");

        REQUIRE(reader.ReadLine(line) == 0);
        REQUIRE(line.empty());
    }
    SECTION("首续行遇到尾续行：尾续行结束后，仍然尝试首续行") {
        XSLineReader reader(MemoryLineProvider{
            "r1 (1 2) resistor r=1\n",
            "+p1=1\\\n",
            "p2=2\n",  //
            "+p3=3\n", //
        });

        std::string line;

        REQUIRE(reader.ReadLine(line) == 1);
        REQUIRE(line == "r1 (1 2) resistor r=1 p1=1 p2=2 p3=3");

        REQUIRE(reader.ReadLine(line) == 0);
        REQUIRE(line.empty());
    }
    SECTION("首续行后面遇到文件结束：首续行不跨文件") {
        XSLineReader reader(MemoryLineProvider{
            "r1 (1 2) resistor r=1\n",
            "+p1=1\n",
            "+p2=2\n", //
        });

        std::string line;

        REQUIRE(reader.ReadLine(line) == 1);
        REQUIRE(line == "r1 (1 2) resistor r=1 p1=1 p2=2");

        REQUIRE(reader.ReadLine(line) == 0);
        REQUIRE(line.empty());
    }
    SECTION("首续行中间插入注释行：不影响首续行") {
        XSLineReader reader(MemoryLineProvider{
            "r1 (1 2) resistor r=1\n",
            "+p1=1\n",
            "//+p2=2\n",
            "+p3=3\n",
        });

        std::string line;

        REQUIRE(reader.ReadLine(line) == 1);
        REQUIRE(line == "r1 (1 2) resistor r=1 p1=1 p3=3");

        REQUIRE(reader.ReadLine(line) == 0);
        REQUIRE(line.empty());
    }
    SECTION("首续行遇到普通行：当前首续行结束，但是开启新的语句") {
        XSLineReader reader(MemoryLineProvider{
            "r1 (1 2) resistor r=1\n",
            "+p1=1\n",
            "r2 (2 3) resistor p2=2\n",
            "+p3=3\n",
        });

        std::string line;

        REQUIRE(reader.ReadLine(line) == 1);
        REQUIRE(line == "r1 (1 2) resistor r=1 p1=1");

        REQUIRE(reader.ReadLine(line) == 2);
        REQUIRE(line == "r2 (2 3) resistor p2=2 p3=3");

        REQUIRE(reader.ReadLine(line) == 0);
        REQUIRE(line.empty());
    }
    SECTION("首续行遇到空白行：空白行不影响首续行，直接被忽略") {
        XSLineReader reader(MemoryLineProvider{
            "r1 (1 2) resistor r=1\n",
            "+p1=1\n",
            " \t \n",
            " \t \n",
            "+p3=3\n",
        });

        std::string line;

        REQUIRE(reader.ReadLine(line) == 1);
        REQUIRE(line == "r1 (1 2) resistor r=1 p1=1 p3=3");

        REQUIRE(reader.ReadLine(line) == 0);
        REQUIRE(line.empty());
    }
}