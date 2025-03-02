#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, expand_user) {
    SECTION("简单场景") {
        ASSERT_EQ(view::expand_user("~"), std::string{getenv("HOME")});
        ASSERT_EQ(view::expand_user("~/"), std::string{getenv("HOME")} + "/");
        ASSERT_EQ(view::expand_user("~/abc"), std::string{getenv("HOME")} + "/abc");
    }
    SECTION("空串") {
        ASSERT_EQ(view::expand_user(""), "");
    }
    SECTION("起始点不匹配") {
        ASSERT_EQ(view::expand_user("~ "), "~ ");
        ASSERT_EQ(view::expand_user(" ~/abc"), " ~/abc");
    }
}
