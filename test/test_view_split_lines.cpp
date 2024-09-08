//
// TEST(tiny_bytes, split_lines_notkeepends) {
//     SECTION("简单场景") {
//         std::string s{ "\na\r\nb\ncccc\n" };
//         std::vector<std::string> fields = {
//             std::string{ "" },
//             std::string{ "a" },
//             std::string{ "b" },
//             std::string{ "cccc" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines([&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//     SECTION("空串") {
//         std::string s{ "" };
//         std::vector<std::string> fields = {
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines([&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//     SECTION("纯换行") {
//         std::string s{ "\n" };
//         std::vector<std::string> fields = {
//             std::string{ "" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines([&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//
//     SECTION("综合测试") {
//         std::string s{ "a\n\ryyy\r\r\nb\n\nc\n\r" };
//         std::vector<std::string> fields = {
//             std::string{ "a" },
//             std::string{ "" },
//             std::string{ "yyy" },
//             std::string{ "" },
//             std::string{ "b" },
//             std::string{ "" },
//             std::string{ "c" },
//             std::string{ "" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines([&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
// }
//
// TEST(tiny_bytes, split_lines_keepends) {
//     SECTION("简单场景") {
//         std::string s{ "\na\r\nb\ncccc\n" };
//         std::vector<std::string> fields = {
//             std::string{ "\n" },
//             std::string{ "a\r\n" },
//             std::string{ "b\n" },
//             std::string{ "cccc\n" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//     SECTION("空串") {
//         std::string s{ "" };
//         std::vector<std::string> fields = {
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//     SECTION("纯换行") {
//         std::string s{ "\n" };
//         std::vector<std::string> fields = {
//             std::string{ "\n" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
//
//     SECTION("综合测试") {
//         std::string s{ "a\n\ryyy\r\r\nb\n\nc\n\r" };
//         std::vector<std::string> fields = {
//             std::string{ "a\n" },
//             std::string{ "\r" },
//             std::string{ "yyy\r" },
//             std::string{ "\r\n" },
//             std::string{ "b\n" },
//             std::string{ "\n" },
//             std::string{ "c\n" },
//             std::string{ "\r" },
//             std::string{ "" },
//         };
//
//         std::vector<std::string> result;
//         s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
//             result.emplace_back(p, n);
//             return 0;
//         });
//
//         ASSERT_EQ(result, fields);
//     }
// }
