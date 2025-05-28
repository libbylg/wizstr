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
#ifndef STR_TO_CONSUMER_H
#define STR_TO_CONSUMER_H

#include <limits>
#include <utility>

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

#endif // STR_TO_CONSUMER_H
