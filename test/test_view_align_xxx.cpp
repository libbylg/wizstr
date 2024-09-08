//
// TEST_CASE("str::ljust") {
//    SECTION("一般情况：字符串左对齐，右边填充") {
//        std::string s("HelloWorld");
//        REQUIRE(str::ljust(s, 13, '*') == "HelloWorld***");
//    }
//
//    SECTION("宽度和原字符串相同：字符串不变") {
//        std::string s("HelloWorld");
//        REQUIRE(str::ljust(s, 10, '*') == "HelloWorld");
//    }
//
//    SECTION("宽度太窄时保持原样") {
//        std::string s("HelloWorld");
//        REQUIRE(str::ljust(s, 9, '*') == "HelloWorld");
//    }
//}
//
// TEST_CASE("str::rjust") {
//    SECTION("简单测试") {
//        std::string s("HelloWorld");
//        REQUIRE(str::rjust(s, 13, '*') == "***HelloWorld");
//    }
//
//    SECTION("简单测试") {
//        std::string s("HelloWorld");
//        REQUIRE(str::rjust(s, 10, '*') == "HelloWorld");
//    }
//
//    SECTION("简单测试") {
//        std::string s("HelloWorld");
//        REQUIRE(str::rjust(s, 9, '*') == "HelloWorld");
//    }
//
//    SECTION("简单测试") {
//        std::string s("HelloWorld");
//        REQUIRE(str::rjust(s, 9, '*') == "HelloWorld");
//    }
//}
//
// TEST_CASE("str::center") {
//    SECTION("简单测试") {
//        std::string s("HelloWorld");
//        REQUIRE(str::center(s, 13, '*') == "*HelloWorld**");
//    }
//
//    SECTION("简单测试") {
//        std::string s("HelloWorld");
//        REQUIRE(str::center(s, 10, '*') == "HelloWorld");
//    }
//
//    SECTION("简单测试") {
//        std::string s("HelloWorld");
//        REQUIRE(str::center(s, 9, '*') == "HelloWorld");
//    }
//
//    SECTION("简单测试") {
//        std::string s("HelloWorld");
//        REQUIRE(str::center(s, 9, '*') == "HelloWorld");
//    }
//}