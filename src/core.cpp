/**
 * Copyright (c) 2021-2021 libbylg@126.com
 * tiny is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "core.h"

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