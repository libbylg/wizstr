#include "testing.hpp"

#include "str.hpp"

TEST(test_str, simplified_inplace) {
    std::string s;
    SECTION("一般场景") {
        ASSERT_EQ(str::simplified_inplace(s = "\r\n\t   A abbCC中华人\r\n\t   民共\r\n\t   和 \t国\r\n\t   "), "A abbCC中华人 民共 和 国");
        ASSERT_TRUE(str::simplified_inplace(s = "\r\n\t   A abbCC中华人\r\n\t   民共\r\n\t   和 \t国\r\n\t   ", "|", [](str::value_type ch) -> bool {
            return std::isspace(ch);
        }) == "A|abbCC中华人|民共|和|国");
    }

    SECTION("没什么可化简的") {
        ASSERT_EQ(str::simplified_inplace(s = "AabbCC中华人民共和国"), "AabbCC中华人民共和国");
    }

    SECTION("空串") {
        ASSERT_EQ(str::simplified_inplace(s = ""), "");
    }

    SECTION("全空白") {
        ASSERT_EQ(str::simplified_inplace(s = "   \t  \n \r \v"), "");
    }

    SECTION("proc模式") {
        ASSERT_EQ(str::simplified_inplace(s = "   \t  \n \r \v"), "");
    }
}