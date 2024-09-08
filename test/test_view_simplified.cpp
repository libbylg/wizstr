
//
// TEST_CASE("simplified-reference", "") {
//    SECTION("简单测试") {
//        std::string a("\t  A abbCC中华人   民共  和 \t国   ");
//        str::simplified(a);
//        REQUIRE(a == "A abbCC中华人 民共 和 国");
//    }
//
//    SECTION("没什么可以替代的") {
//        std::string a("AabbCC中华人民共和国");
//        REQUIRE(str::simplified(a) == "AabbCC中华人民共和国");
//    }
//
//    SECTION("空串") {
//        std::string a("");
//        REQUIRE(str::simplified(a) == "");
//    }
//
//    SECTION("全空白") {
//        std::string a("   \t  \n \r \v");
//        REQUIRE(str::simplified(a) == "");
//    }
//}


// TEST_CASE("simplified-copy", "") {
//    SECTION("简单测试") {
//        REQUIRE(str::simplified("\t  A abbCC中华人   民共  和 \t国   ") == "A abbCC中华人 民共 和 国");
//    }
//
//    SECTION("没什么可以替代的") {
//        REQUIRE(str::simplified("AabbCC中华人民共和国") == "AabbCC中华人民共和国");
//    }
//
//    SECTION("空串") {
//        REQUIRE(str::simplified("") == "");
//    }
//
//    SECTION("全空白") {
//        REQUIRE(str::simplified("   \t  \n \r \v") == "");
//    }
//}