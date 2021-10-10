//
// Created by luolijun on 2021/10/10.
//

#ifndef CORE_H
#define CORE_H

#include <cctype>

// memmove
// memcpy
// strlen
// std::fill
// strstr
// strrstr
// strchr
// toupper
// tolower
// memmem
namespace tiny {
namespace core {
    extern const char* strrstr(const char* s1, const char* s2);
    extern const char* memrstr(const char* begin, const char* end, const char* ptn, size_t n);
    extern int wildcmp(const char* wild, const char* s);
} // namespace core
} // namespace tiny
#endif // CORE_H
