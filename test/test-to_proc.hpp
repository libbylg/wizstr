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

template <typename ContainerType>
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

#endif // STR_TEST_TO_PROC_HPP
