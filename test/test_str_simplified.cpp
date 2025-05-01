#include "testing.hpp"

#include "str.hpp"


TEST(test_str, simplified) {
    SECTION("一般场景") {
        ASSERT_EQ(str::simplified("\r\n\t   A abbCC中华人\r\n\t   民共\r\n\t   和 \t国\r\n\t   "), "A abbCC中华人 民共 和 国");
        ASSERT_TRUE(str::simplified("\r\n\t   A abbCC中华人\r\n\t   民共\r\n\t   和 \t国\r\n\t   ", "|", [](str::value_type ch) -> bool {
            return std::isspace(ch);
        }) == "A|abbCC中华人|民共|和|国");
    }

    SECTION("没什么可化简的") {
        ASSERT_EQ(str::simplified("AabbCC中华人民共和国"), "AabbCC中华人民共和国");
    }

    SECTION("空串") {
        ASSERT_EQ(str::simplified(""), "");
    }

    SECTION("全空白") {
        ASSERT_EQ(str::simplified("   \t  \n \r \v"), "");
    }
}