// TEST(tiny_bytes, expand) {
//     SECTION("简单扩展") {
//         std::string b("${HOME}/${NOTEXIST}");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, std::string(getenv("HOME")) + "/");
//     }
//
//     SECTION("简单扩展") {
//         std::string b("$HOME/${NOTEXIST}");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, std::string(getenv("HOME")) + "/");
//     }
//
//     SECTION("简单扩展") {
//         std::string b("abcd$HOME/${NOTEXIST}");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, "abcd" + std::string(getenv("HOME")) + "/");
//     }
//
//     SECTION("简单扩展") {
//         std::string b("abcd${HOME}");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, "abcd" + std::string(getenv("HOME")));
//     }
//
//     SECTION("简单扩展") {
//         std::string b("$HOME");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, std::string(getenv("HOME")));
//     }
//
//     SECTION("简单扩展") {
//         std::string b("$$$");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, "$$$");
//     }
//
//     SECTION("简单扩展") {
//         std::string b("");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, "");
//     }
//
//     SECTION("简单扩展") {
//         std::string b("$");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, "$");
//     }
//
//     SECTION("简单扩展") {
//         std::string b("abc$");
//         std::string result = b.expand_envs();
//         ASSERT_EQ(result, "abc$");
//     }
// }
