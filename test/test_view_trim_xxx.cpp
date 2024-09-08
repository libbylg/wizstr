
// TEST(tiny_bytes, trims) {
//     SECTION("前后没有空白") {
//         std::string a("3bc1233");
//         EXPECT_TRUE(a.trim() == "3bc1233");
//     }
//
//     SECTION("前后都有空白") {
//         std::string b("  \tc1233\t  \t  ");
//         EXPECT_TRUE(b.trim() == "c1233");
//     }
//
//     SECTION("指定字符") {
//         std::string b("33c1233");
//         EXPECT_TRUE(b.trim([](std::string::value_type ch) -> bool {
//             return ch == '3';
//         }) == "c12");
//     }
// }