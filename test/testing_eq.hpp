#ifndef TESTING_EQ_H
#define TESTING_EQ_H

#include "str.hpp"

template<typename T, typename Y>
auto operator==(const std::vector<T> &a, const std::vector<Y> &b) -> bool {
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


#endif//TESTING_EQ_H
