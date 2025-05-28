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

#endif // STR_TEST_TO_PROC_HPP
