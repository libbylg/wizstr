//
// Created by root on 9/9/24.
//

#ifndef STR_TEST_COMPARES_H
#define STR_TEST_COMPARES_H

template <typename elem1_type>
static inline auto eq(const std::vector<elem1_type>& a, const std::vector<elem1_type>& b) -> bool {
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

static inline auto operator==(const std::vector<std::string_view>& a, const std::vector<std::string_view>& b) -> bool {
    return eq(a, b);
}

static inline auto operator==(const std::vector<std::string>& a, const std::vector<std::string>& b) -> bool {
    return eq(a, b);
}

#endif // STR_TEST_COMPARES_H
