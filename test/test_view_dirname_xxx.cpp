
//
// TEST_CASE("dirname") {
//    SECTION("全路径") {
//        REQUIRE(str::dirname("/aaa/bbb/ccc") == "/aaa/bbb");
//    }
//    SECTION("相对路径") {
//        REQUIRE(str::dirname("aaa/bbb/ccc") == "aaa/bbb");
//        REQUIRE(str::dirname("../bbb/ccc") == "../bbb");
//        REQUIRE(str::dirname("../ccc") == "..");
//        REQUIRE(str::dirname("./ccc") == ".");
//    }
//    SECTION(".和..") {
//        REQUIRE(str::dirname(".") == ".");
//        REQUIRE(str::dirname("..") == ".");
//    }
//    SECTION("无路径分隔符") {
//        REQUIRE(str::dirname("abc") == ".");
//    }
//    SECTION("./和../") {
//        REQUIRE(str::dirname("./") == ".");
//        REQUIRE(str::dirname("../") == ".");
//        REQUIRE(str::dirname("./aa") == ".");
//        REQUIRE(str::dirname("../aa") == "..");
//    }
//    SECTION("绝对路径 /") {
//        REQUIRE(str::dirname("/") == "/");
//    }
//    SECTION("空串") {
//        REQUIRE(str::dirname("") == ".");
//    }
//    SECTION("多余的路径元素") {
//        REQUIRE(str::dirname("///ccc") == "/");
//        REQUIRE(str::dirname(".///ccc") == ".");
//    }
//}