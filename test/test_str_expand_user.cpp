#include "tester.hpp"

#include "str.hpp"


TEST(test_str, expand_user) {
    SECTION("简单场景") {
        ASSERT_EQ(str::expand_user("~"), std::string{getenv("HOME")});
        ASSERT_EQ(str::expand_user("~/"), std::string{getenv("HOME")} + "/");
        ASSERT_EQ(str::expand_user("~/abc"), std::string{getenv("HOME")} + "/abc");
    }
    SECTION("空串") {
        ASSERT_EQ(str::expand_user(""), "");
    }
    SECTION("起始点不匹配") {
        ASSERT_EQ(str::expand_user("~ "), "~ ");
        ASSERT_EQ(str::expand_user(" ~/abc"), " ~/abc");
    }
}
