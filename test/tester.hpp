#ifndef TESTER_H
#define TESTER_H

#define UT_WITH_GTEST 0
#define UT_WITH_CATCH2 1

#if TEST_FRAMEWORK == UT_WITH_GTEST
#include "gtest/gtest.h"

#define SECTION(text)

template<typename T, typename Y>
auto operator==(const std::vector<T>& a, const std::vector<Y>& b) -> bool {
    if (a.size() != b.size()) {
        return false;
    }

    for (size_t index = 0; index < a.size(); index++) {
        if (a[index] != b[index]) {
            return false;
        }
    }

    return true;
}
#elif TEST_FRAMEWORK == UT_WITH_CATCH2
#include "catch2/catch_all.hpp"
#define TEST(SuiteName_, CaseName_) TEST_CASE(#SuiteName_ "__" #CaseName_)
#define ASSERT_EQ(a, b) REQUIRE((a) == (b))
#define ASSERT_TRUE(a) REQUIRE(a)
#define ASSERT_FALSE(a) REQUIRE(!(a))
#else
#error "Unsupported test framework7"
#endif

#endif//TESTER_H
