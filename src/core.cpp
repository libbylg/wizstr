//
// Created by luolijun on 2021/10/10.
//
#include "core.h"

#include <cstdlib>
#include <cstring>

namespace tiny {
namespace core {
    //  反向查找字符串
    extern const char* strrstr(const char* s1, const char* s2) {

        const char* sc1;
        const char* sc2;
        const char* psc1;
        const char* ps1;

        if (*s2 == '\0') {
            return s1;
        }

        ps1 = s1 + std::strlen(s1);

        while (ps1 != s1) {
            --ps1;
            for (psc1 = ps1, sc2 = s2;;) {
                if (*(psc1++) != *(sc2++)) {
                    break;
                }

                if (*sc2 == '\0') {
                    return ps1;
                }
            }
        }

        return nullptr;
    }

    extern const char* memrstr(const char* begin, const char* end, const char* ptn, size_t n) {
        return nullptr;
    }

    extern int wildcmp(const char* wild, const char* s) {
        const char* cp = nullptr;
        const char* mp = nullptr;
        while ((*s) && (*wild != '*')) {
            if ((*wild != *s) && (*wild != '?')) {
                return 1;
            }
            wild++;
            s++;
        }

        while (*s) {
            if (*wild == '*') {
                if (!*(++wild)) {
                    return 0;
                }
                mp = wild;
                cp = s + 1;
            } else if ((*wild == *s) || (*wild == '?')) {
                wild++;
                s++;
            } else {
                wild = mp;
                s = cp++;
            }
        }

        while (*wild == '*') {
            wild++;
        }

        return *wild;
    }

} // namespace core
} // namespace tiny