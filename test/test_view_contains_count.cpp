
//
// TEST(tiny_bytes, contains) {
//     SECTION("删除中间数据") {
//         std::string a("3bc1233");
//         EXPECT_TRUE(a.contains("12"));
//         EXPECT_TRUE(a.contains("33"));
//         EXPECT_FALSE(a.contains("31"));
//     }
// }
//


//
// TEST(tiny_bytes, count) {
//     SECTION("检查是否匹配某个正则表达式") {
//         std::string a("3bc12def1233");
//         EXPECT_EQ(a.count("12"), 2);
//         EXPECT_EQ(a.count(char('3')), 3);
//         EXPECT_EQ(a.count("A"), 0);
//     }
//
//     SECTION("统计子串数量") {
//         std::string a("333433343333334");
//         EXPECT_EQ(a.count("33"), 5);
//         EXPECT_EQ(a.count(""), 16);
//         EXPECT_EQ(a.count("333"), 4);
//     }
// }