
//
// TEST(tiny_bytes, is_match_wild) {
//     SECTION("检查是否匹配某个正则表达式") {
//         std::string a("3bc12def33");
//         EXPECT_TRUE(a.is_match_wild("3*"));
//         EXPECT_TRUE(a.is_match_wild("3bc12def33"));
//         EXPECT_TRUE(a.is_match_wild("3*de?33"));
//     }
// }

// TEST_CASE("str::is_bool") {
//    SECTION("简单场景") {
//        REQUIRE(str::is_bool("1"));
//        REQUIRE(str::is_bool("0"));
//        REQUIRE(str::is_bool("on"));
//        REQUIRE(str::is_bool("off"));
//        REQUIRE(str::is_bool("ON"));
//        REQUIRE(str::is_bool("OFF"));
//        REQUIRE(str::is_bool("Yes"));
//        REQUIRE(str::is_bool("No"));
//        REQUIRE(str::is_bool("yes"));
//        REQUIRE(str::is_bool("no"));
//        REQUIRE(str::is_bool("YES"));
//        REQUIRE(str::is_bool("NO"));
//        REQUIRE(str::is_bool("True"));
//        REQUIRE(str::is_bool("False"));
//        REQUIRE(str::is_bool("true"));
//        REQUIRE(str::is_bool("false"));
//        REQUIRE(str::is_bool("TRUE"));
//        REQUIRE(str::is_bool("FALSE"));
//    }
//}