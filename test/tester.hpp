#ifndef TESTER_H
#define TESTER_H

#include "gtest/gtest.h"

#define SECTION(text)

template<typename T>
auto operator==(const std::vector<T>& a, const std::vector<T>& b) -> bool {
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

#endif//TESTER_H
