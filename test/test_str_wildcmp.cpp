#include "tester.hpp"

#include "str.hpp"


TEST(test_str, wildcmp) {
    SECTION("检查是否匹配通配符") {
        ASSERT_TRUE(str::wildcmp("3*", "3bc12def33"));
        ASSERT_TRUE(str::wildcmp("3abc", "3abc"));
        ASSERT_TRUE(str::wildcmp("?", "3"));
        ASSERT_TRUE(str::wildcmp("????", "3abc"));
        ASSERT_FALSE(str::wildcmp("", "3abc"));
        ASSERT_TRUE(str::wildcmp("", ""));
        ASSERT_TRUE(str::wildcmp("*3", "3bc12def33"));
        ASSERT_TRUE(str::wildcmp("*3?", "3bc12def33"));
        ASSERT_TRUE(str::wildcmp("*?3", "3bc12def33"));
        ASSERT_TRUE(str::wildcmp("3bc12def33", "3bc12def33"));
        ASSERT_TRUE(str::wildcmp("3*de?33", "3bc12def33"));
        ASSERT_TRUE(str::wildcmp("*", "3bc12def33"));
        ASSERT_TRUE(str::wildcmp("*", ""));
        ASSERT_FALSE(str::wildcmp("3bc12def33", "abc"));
    }
}

TEST(test_str, wildcmp2) {
    SECTION("检查是否匹配通配符") {
        ASSERT_TRUE(str::wildcmp("3*", "3bc12def33"));
//        ASSERT_TRUE(str::wildcmp("3abc", "3abc"));
//        ASSERT_TRUE(str::wildcmp("?", "3"));
//        ASSERT_TRUE(str::wildcmp("????", "3abc"));
//        ASSERT_FALSE(str::wildcmp("", "3abc"));
//        ASSERT_TRUE(str::wildcmp("", ""));
//        ASSERT_TRUE(str::wildcmp("*3", "3bc12def33"));
//        ASSERT_TRUE(str::wildcmp("*3?", "3bc12def33"));
//        ASSERT_TRUE(str::wildcmp("*?3", "3bc12def33"));
//        ASSERT_TRUE(str::wildcmp("3bc12def33", "3bc12def33"));
//        ASSERT_TRUE(str::wildcmp("3*de?33", "3bc12def33"));
//        ASSERT_TRUE(str::wildcmp("*", "3bc12def33"));
//        ASSERT_TRUE(str::wildcmp("*", ""));
//        ASSERT_FALSE(str::wildcmp("3bc12def33", "abc"));
    }
}