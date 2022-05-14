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
        const char* cp = s;
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

    //    // __str_find_first_of
    //    template <class _CharT, class _SizeT, class _Traits, _SizeT __npos>
    //    inline _SizeT
    //    __str_find_first_of(const _CharT* __p, _SizeT __sz,
    //        const _CharT* __s, _SizeT __pos, _SizeT __n) _NOEXCEPT {
    //        if (__pos >= __sz || __n == 0)
    //            return __npos;
    //        const _CharT* __r = __find_first_of_ce(__p + __pos, __p + __sz, __s, __s + __n, _Traits::eq);
    //        if (__r == __p + __sz)
    //            return __npos;
    //        return static_cast<_SizeT>(__r - __p);
    //    }
    //
    //    // __str_find_last_of
    //    template <class _CharT, class _SizeT, class _Traits, _SizeT __npos>
    //    inline _SizeT _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    //    __str_find_last_of(const _CharT* __p, _SizeT __sz,
    //        const _CharT* __s, _SizeT __pos, _SizeT __n) _NOEXCEPT {
    //        if (__n != 0) {
    //            if (__pos < __sz)
    //                ++__pos;
    //            else
    //                __pos = __sz;
    //            for (const _CharT* __ps = __p + __pos; __ps != __p;) {
    //                const _CharT* __r = _Traits::find(__s, __n, *--__ps);
    //                if (__r)
    //                    return static_cast<_SizeT>(__ps - __p);
    //            }
    //        }
    //        return __npos;
    //    }
    //
    //    // __str_find_first_not_of
    //    template <class _CharT, class _SizeT, class _Traits, _SizeT __npos>
    //    inline _SizeT _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    //    __str_find_first_not_of(const _CharT* __p, _SizeT __sz,
    //        const _CharT* __s, _SizeT __pos, _SizeT __n) _NOEXCEPT {
    //        if (__pos < __sz) {
    //            const _CharT* __pe = __p + __sz;
    //            for (const _CharT* __ps = __p + __pos; __ps != __pe; ++__ps)
    //                if (_Traits::find(__s, __n, *__ps) == 0)
    //                    return static_cast<_SizeT>(__ps - __p);
    //        }
    //        return __npos;
    //    }
    //
    //    template <class _CharT, class _SizeT, class _Traits, _SizeT __npos>
    //    inline _SizeT _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    //    __str_find_first_not_of(const _CharT* __p, _SizeT __sz,
    //        _CharT __c, _SizeT __pos) _NOEXCEPT {
    //        if (__pos < __sz) {
    //            const _CharT* __pe = __p + __sz;
    //            for (const _CharT* __ps = __p + __pos; __ps != __pe; ++__ps)
    //                if (!_Traits::eq(*__ps, __c))
    //                    return static_cast<_SizeT>(__ps - __p);
    //        }
    //        return __npos;
    //    }
    //
    //    // __str_find_last_not_of
    //    template <class _CharT, class _SizeT, class _Traits, _SizeT __npos>
    //    inline _SizeT _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    //    __str_find_last_not_of(const _CharT* __p, _SizeT __sz,
    //        const _CharT* __s, _SizeT __pos, _SizeT __n) _NOEXCEPT {
    //        if (__pos < __sz)
    //            ++__pos;
    //        else
    //            __pos = __sz;
    //        for (const _CharT* __ps = __p + __pos; __ps != __p;)
    //            if (_Traits::find(__s, __n, *--__ps) == 0)
    //                return static_cast<_SizeT>(__ps - __p);
    //        return __npos;
    //    }
    //
    //    template <class _CharT, class _SizeT, class _Traits, _SizeT __npos>
    //    inline _SizeT _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    //    __str_find_last_not_of(const _CharT* __p, _SizeT __sz,
    //        _CharT __c, _SizeT __pos) _NOEXCEPT {
    //        if (__pos < __sz)
    //            ++__pos;
    //        else
    //            __pos = __sz;
    //        for (const _CharT* __ps = __p + __pos; __ps != __p;)
    //            if (!_Traits::eq(*--__ps, __c))
    //                return static_cast<_SizeT>(__ps - __p);
    //        return __npos;
    //    }
    //
    //    template <class _Ptr>
    //    inline _LIBCPP_INLINE_VISIBILITY
    //        size_t
    //        __do_string_hash(_Ptr __p, _Ptr __e) {
    //        typedef typename iterator_traits<_Ptr>::value_type value_type;
    //        return __murmur2_or_cityhash<size_t>()(__p, (__e - __p) * sizeof(value_type));
    //    }

} // namespace core
} // namespace tiny