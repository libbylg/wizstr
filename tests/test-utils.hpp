/**
 * Copyright (c) 2021-2024 libbylg@126.com
 * str is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#ifndef STR_TEST_TO_PROC_HPP
#define STR_TEST_TO_PROC_HPP

class scope_guard {
public:
    explicit scope_guard(const std::function<void()>& proc): proc_{proc} {
    }

    ~scope_guard() {
        if (proc_) {
            proc_();
        }
    }

private:
    const std::function<void()>& proc_;
};

template<typename ContainerType>
class to_proc {
public:
    explicit to_proc(const ContainerType& container)
        : container_{container} {
        itr = container_.begin();
    }

    auto operator()() -> std::optional<std::string_view> {
        if (itr == container_.end()) {
            return std::nullopt;
        }

        return *(itr++);
    }

private:
    const ContainerType& container_;
    typename ContainerType::const_iterator itr;
};

class to_search_proc {
public:
    explicit to_search_proc(std::string_view sep)
        : sep_{sep} {
    }

    auto operator()(std::string_view search_range) -> std::optional<std::string_view> {
        auto pos = search_range.find(sep_);
        if (pos == std::string::npos) {
            return std::nullopt;
        }

        return search_range.substr(pos, sep_.size());
    }

private:
    std::string sep_;
};


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

template<typename ContainerType, typename ElemType = typename ContainerType::value_type>
class to_consumer {
public:
    explicit to_consumer(ContainerType& result,
                         typename ContainerType::size_type max_n = std::numeric_limits<typename ContainerType::size_type>::max())
        : result_{result}
    , max_n_{max_n} {
    }

    inline auto operator()(ElemType&& elem) -> int {
        result_.push_back(std::move(elem));
        if (result_.size() >= max_n_) {
            return -1;
        }
        return 0;
    }

private:
    ContainerType& result_;
    typename ContainerType::size_type max_n_{0};
};

#endif // STR_TEST_TO_PROC_HPP
