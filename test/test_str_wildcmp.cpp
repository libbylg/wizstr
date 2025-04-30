#include "tester.hpp"

#include "str.hpp"


TEST(test_str, wildcmp_cstr) {
    SECTION("检查是否匹配通配符") {
        ASSERT_TRUE(str::wildcmp("3bc12def33", "3*"));
        ASSERT_TRUE(str::wildcmp("3abc", "3abc"));
        ASSERT_TRUE(str::wildcmp("3", "?"));
        ASSERT_TRUE(str::wildcmp("3abc", "????"));
        ASSERT_FALSE(str::wildcmp("3abc", ""));
        ASSERT_TRUE(str::wildcmp("", ""));
        ASSERT_TRUE(str::wildcmp("3b33KK33", "*33"));
        ASSERT_TRUE(str::wildcmp("3bc12def33", "*3?"));
        ASSERT_TRUE(str::wildcmp("3bc12def33", "*?3"));
        ASSERT_TRUE(str::wildcmp("3bc12def33", "3bc12def33"));
        ASSERT_TRUE(str::wildcmp("3bc12def33", "3*de?33"));
        ASSERT_TRUE(str::wildcmp("3bc12def33", "*"));
        ASSERT_TRUE(str::wildcmp("", "*"));
        ASSERT_FALSE(str::wildcmp("abc", "3bc12def33"));
        ASSERT_TRUE(str::wildcmp("3bc12def33", "*3**3***3"));
        ASSERT_TRUE(str::wildcmp("3bc12def33", "*3**3***3***"));
        ASSERT_TRUE(str::wildcmp("abc", "abc***"));
        ASSERT_FALSE(str::wildcmp("abc", "abc***d"));
    }
}

TEST(test_str, wildcmp_string_view) {
    SECTION("检查是否匹配通配符") {
        ASSERT_TRUE(str::wildcmp(std::string_view{"3bc12def33"}, std::string_view{"3*"}));
        ASSERT_TRUE(str::wildcmp(std::string_view{"3abc"}, std::string_view{"3abc"}));
        ASSERT_TRUE(str::wildcmp(std::string_view{"3"}, std::string_view{"?"}));
        ASSERT_TRUE(str::wildcmp(std::string_view{"3abc"}, std::string_view{"????"}));
        ASSERT_FALSE(str::wildcmp(std::string_view{"3abc"}, std::string_view{""}));
        ASSERT_TRUE(str::wildcmp(std::string_view{""}, std::string_view{""}));
        ASSERT_TRUE(str::wildcmp(std::string_view{"3bc12def33"}, std::string_view{"*3"}));
        ASSERT_TRUE(str::wildcmp(std::string_view{"3bc12def33"}, std::string_view{"*3?"}));
        ASSERT_TRUE(str::wildcmp(std::string_view{"3bc12def33"}, std::string_view{"*?3"}));
        ASSERT_TRUE(str::wildcmp(std::string_view{"3bc12def33"}, std::string_view{"3bc12def33"}));
        ASSERT_TRUE(str::wildcmp(std::string_view{"3bc12def33"}, std::string_view{"3*de?33"}));
        ASSERT_TRUE(str::wildcmp(std::string_view{"3bc12def33"}, std::string_view{"*"}));
        ASSERT_TRUE(str::wildcmp(std::string_view{""}, std::string_view{"*"}));
        ASSERT_FALSE(str::wildcmp(std::string_view{"abc"}, std::string_view{"3bc12def33"}));
        ASSERT_TRUE(str::wildcmp(std::string_view{"3bc12def33"}, std::string_view{"*3**3***3"}));
        ASSERT_TRUE(str::wildcmp(std::string_view{"3bc12def33"}, std::string_view{"*3**3***3***"}));
        ASSERT_TRUE(str::wildcmp(std::string_view{"abc"}, std::string_view{"abc***"}));
        ASSERT_FALSE(str::wildcmp(std::string_view{"abc"}, std::string_view{"abc***d"}));
    }
}