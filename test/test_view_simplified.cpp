#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::simplified") {
    SECTION("一般场景") {
        REQUIRE(view::simplified("\r\n\t   A abbCC中华人\r\n\t   民共\r\n\t   和 \t国\r\n\t   ") == "A abbCC中华人 民共 和 国");
        REQUIRE(view::simplified("\r\n\t   A abbCC中华人\r\n\t   民共\r\n\t   和 \t国\r\n\t   ", "|", [](view::value_type ch) -> bool {
            return std::isspace(ch);
        }) == "A|abbCC中华人|民共|和|国");
    }

    SECTION("没什么可化简的") {
        REQUIRE(view::simplified("AabbCC中华人民共和国") == "AabbCC中华人民共和国");
    }

    SECTION("空串") {
        REQUIRE(view::simplified("") == "");
    }

    SECTION("全空白") {
        REQUIRE(view::simplified("   \t  \n \r \v") == "");
    }
}