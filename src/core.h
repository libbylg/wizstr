//
// Created by luolijun on 2021/10/10.
//

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
