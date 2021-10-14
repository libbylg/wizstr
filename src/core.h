/**
 * Copyright (c) 2021-2021 libbylg@126.com
 * tiny is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#ifndef CORE_H
#define CORE_H

#include <cctype>

// std::memmove
// std::memcpy
// std::strlen
// std::fill
// std::strstr
// strrstr
// std::strchr
// std::toupper
// std::tolower
// memmem
// std::transform
// std::min

namespace tiny {
namespace core {
    extern const char* strrstr(const char* s1, const char* s2);
    extern const char* memrstr(const char* begin, const char* end, const char* ptn, size_t n);
    extern int wildcmp(const char* wild, const char* s);
} // namespace core
} // namespace tiny
#endif // CORE_H
