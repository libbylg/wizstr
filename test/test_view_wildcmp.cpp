#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, wildcmp) {
    SECTION("检查是否匹配某个正则表达式") {
        ASSERT_TRUE(view::wildcmp("3*", "3bc12def33"));
        ASSERT_TRUE(view::wildcmp("3abc", "3abc"));
        ASSERT_TRUE(view::wildcmp("?", "3"));
        ASSERT_TRUE(view::wildcmp("????", "3abc"));
        ASSERT_FALSE(view::wildcmp("", "3abc"));
        ASSERT_TRUE(view::wildcmp("", ""));
        ASSERT_TRUE(view::wildcmp("*3", "3bc12def33"));
        ASSERT_TRUE(view::wildcmp("*3?", "3bc12def33"));
        ASSERT_TRUE(view::wildcmp("*?3", "3bc12def33"));
        ASSERT_TRUE(view::wildcmp("3bc12def33", "3bc12def33"));
        ASSERT_TRUE(view::wildcmp("3*de?33", "3bc12def33"));
        ASSERT_TRUE(view::wildcmp("*", "3bc12def33"));
        ASSERT_TRUE(view::wildcmp("*", ""));
        ASSERT_FALSE(view::wildcmp("3bc12def33", "abc"));
    }
}
