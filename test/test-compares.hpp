/**
 * Copyright (c) 2021-2024 libbylg@126.com
 * tiny is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#ifndef STR_TEST_COMPARES_H
#define STR_TEST_COMPARES_H

#include <string>
#include <string_view>
#include <vector>

template <typename elem1_type>
inline auto eq(const std::vector<elem1_type>& a, const std::vector<elem1_type>& b) -> bool {
    if (a.size() != b.size()) {
        return false;
    }

    for (std::string::size_type i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

inline auto operator==(const std::vector<std::string_view>& a, const std::vector<std::string_view>& b) -> bool {
    return eq(a, b);
}

inline auto operator==(const std::vector<std::string>& a, const std::vector<std::string>& b) -> bool {
    return eq(a, b);
}

template <typename T, typename Y>
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

template <typename T, typename Y, size_t N>
auto operator==(const std::array<T, N>& a, const std::array<Y, N>& b) -> bool {
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

template <typename A1, typename A2, typename B1, typename B2>
auto operator==(const std::tuple<A1, A2>& a, const std::tuple<B1, B2>& b) -> bool {
    return (std::get<0>(a) == std::get<0>(b)) && (std::get<1>(a) == std::get<1>(b));
}

#endif // STR_TEST_COMPARES_H
