
// TEST_CASE(last_extname_ptr, last_extname_ptr) {
//     SECTION("简单裸名字") {
//         REQUIRE(str::last_extname("abc") == std::string(""));
//     }
//     SECTION("简单裸名字带扩展名") {
//         REQUIRE(str::last_extname("abc.ext") == std::string(".ext"));
//     }
//     SECTION("裸名字带路径") {
//         REQUIRE(str::last_extname("abc/def") == std::string(""));
//     }
//     SECTION("只有路径无basename") {
//         REQUIRE(str::last_extname("abc/") == std::string(""));
//     }
//     SECTION("裸名字带路径路径中有点") {
//         REQUIRE(str::last_extname("/abc.def/basename.ext") == std::string(".ext"));
//     }
//     SECTION("裸名字带路径路径中有点+") {
//         REQUIRE(str::last_extname("/abc.def/") == std::string(""));
//     }
//     SECTION("basename为隐藏文件，隐藏文件点开头不是只有扩展名") {
//         REQUIRE(str::last_extname("abc/.hidefile") == std::string(""));
//     }
//     SECTION("basename为隐藏文件，隐藏文件点开头，但是还有额外扩展名") {
//         REQUIRE(str::last_extname("abc/.hidefile.ext") == std::string(".ext"));
//     }
//     SECTION("basename带多个点") {
//         REQUIRE(str::last_extname("abc/normal-file.xxx.ext") == std::string(".ext"));
//     }
//     SECTION("basename位置只有点1") {
//         REQUIRE(str::last_extname("abc/.") == std::string(""));
//     }
//     SECTION("basename位置只有点2") {
//         REQUIRE(str::last_extname("abc/..") == std::string(""));
//     }
//     SECTION("basename位置只有点3") {
//         REQUIRE(str::last_extname("...") == std::string(""));
//     }
//     SECTION("basename位置,点在最后1") {
//         REQUIRE(str::last_extname(".abc.") == std::string("."));
//     }
//     SECTION("basename位置,点在最后2") {
//         REQUIRE(str::last_extname(".abc..") == std::string("."));
//     }
//     SECTION("basename位置,多点前缀1") {
//         REQUIRE(str::last_extname("..abc") == std::string(""));
//     }
//     SECTION("basename位置,多点前缀1") {
//         REQUIRE(str::last_extname("...abc") == std::string(""));
//     }
//     SECTION("空串") {
//         REQUIRE(str::last_extname("") == std::string(""));
//     }
//     SECTION("全空白字符") {
//         REQUIRE(str::last_extname(" \t ") == std::string(""));
//     }
// }
