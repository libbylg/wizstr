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
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring>

#include <functional>
#include <type_traits>

#include "tiny/asserts.h"

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

    template <typename T>
    struct atoi_mapping_trait {
        //  type
        //  VALUE_MAX
        //  VALUE_MIN
    };

    template <typename T>
    struct atoi_mapped_trait {
        //  type
        //  VALUE_MAX
        //  VALUE_MIN
        //  type map(const char* s, char** endptr, int base);
    };

    template <typename T, typename M>
    struct atoi_trait {
        typedef atoi_mapping_trait<M> mapping;
        typedef atoi_mapped_trait<T> mapped;
        static_assert(std::is_signed<T>::value == std::is_signed<M>::value, "确保 T 和 M 同时为有符号数或者无符号数");
        static_assert(sizeof(T) <= sizeof(M), "确保 T 和 M 在类型转换时，数据的内存宽度是兼容的，避免丢失数据位");
    };

    template <>
    struct atoi_mapping_trait<long> {
        typedef long type;
        static const type VALUE_MAX = LONG_MAX;
        static const type VALUE_MIN = LONG_MIN;
        static inline type map(const char* s, char** endptr, int base) {
            return std::strtol(s, endptr, base);
        }
    };

    template <>
    struct atoi_mapping_trait<long long> {
        typedef long long type;
        static const type VALUE_MAX = LLONG_MAX;
        static const type VALUE_MIN = LLONG_MIN;
        static inline type map(const char* s, char** endptr, int base) {
            return std::strtoll(s, endptr, base);
        }
    };

    template <>
    struct atoi_mapping_trait<unsigned long> {
        typedef unsigned long type;
        static const type VALUE_MAX = ULONG_MAX;
        static const type VALUE_MIN = 0UL;
        static inline type map(const char* s, char** endptr, int base) {
            return std::strtoul(s, endptr, base);
        }
    };

    template <>
    struct atoi_mapping_trait<unsigned long long> {
        typedef unsigned long long type;
        static const type VALUE_MAX = ULLONG_MAX;
        static const type VALUE_MIN = 0ULL;
        static inline type map(const char* s, char** endptr, int base) {
            return std::strtoull(s, endptr, base);
        }
    };

    template <>
    struct atoi_mapped_trait<int8_t> {
        typedef int8_t type;
        static const type VALUE_MAX{ INT8_MAX };
        static const type VALUE_MIN{ INT8_MIN };
    };

    template <>
    struct atoi_mapped_trait<int16_t> {
        typedef int16_t type;
        static const type VALUE_MAX = SHRT_MAX;
        static const type VALUE_MIN = SHRT_MIN;
    };

    template <>
    struct atoi_mapped_trait<int32_t> {
        typedef int32_t type;
        static const type VALUE_MAX = INT32_MAX;
        static const type VALUE_MIN = INT32_MIN;
    };

    template <>
    struct atoi_mapped_trait<int64_t> {
        typedef int64_t type;
        static const type VALUE_MAX = INT64_MAX;
        static const type VALUE_MIN = INT64_MIN;
    };

    template <>
    struct atoi_mapped_trait<uint8_t> {
        typedef uint8_t type;
        static const type VALUE_MAX = 1;
        static const type VALUE_MIN = 0;
    };

    template <>
    struct atoi_mapped_trait<uint16_t> {
        typedef uint16_t type;
        static const type VALUE_MAX = USHRT_MAX;
        static const type VALUE_MIN = 0;
    };

    template <>
    struct atoi_mapped_trait<uint32_t> {
        typedef uint32_t type;
        static const type VALUE_MAX = UINT32_MAX;
        static const type VALUE_MIN = 0;
    };

    template <>
    struct atoi_mapped_trait<uint64_t> {
        typedef uint64_t type;
        static const type VALUE_MAX = UINT64_MAX;
        static const type VALUE_MIN = 0ULL;
    };

    //  字符串转整数模板，如果失败总是返回 defval，
    template <typename T, typename M, typename TRAIT = atoi_trait<T, M>>
    T atoi(const char* s, bool* ok, int base, T defval) {
        ASSERT(base >= 2);
        ASSERT(base <= 36);

        bool success = false;
        if (ok == nullptr) {
            ok = &success;
        }

        //  执行数据映射
        char* endptr = nullptr;
        errno = 0;
        typename TRAIT::mapping::type val = TRAIT::mapping::map(s, &endptr, base);

        //  返回 0，可能只是完全没转换，需要判断下
        if (val == 0) {
            //  映射失败
            if (endptr == s) {
                *ok = false;
                return defval;
            }

            //  映射成功
            *ok = true;
            return val;
        }

        //  超出映射类型范围
        if ((TRAIT::mapping::VALUE_MAX == val) || (TRAIT::mapping::VALUE_MIN == val)) {
            if (errno == ERANGE) {
                *ok = false;
                return defval;
            }
        }

        ASSERT(errno == 0);

        //  超出范围
        if ((val > TRAIT::mapped::VALUE_MAX) || (val < TRAIT::mapped::VALUE_MIN)) {
            *ok = false;
            return defval;
        }

        //  转换成功
        *ok = true;
        return val;
    }

    template <typename T>
    int itoa(T val, char* buf, int base) {
        ASSERT(buf != nullptr);
        ASSERT(base >= 0);
        ASSERT(base <= 36);

        //  特殊场景
        if (val == 0) {
            buf[0] = '0';
            buf[1] = '\0';
            return 1;
        }

        //  只有 10 进制是需要处理特殊的负号前缀
        auto numr = (typename std::make_unsigned<T>::type)(val);
        char mark = '\0';
        if (base == 10) {
            if (val < 0) {
                numr = (typename std::make_unsigned<T>::type)(-val);
                mark = '-';
            }
        }

        //  数据格式化时的映射
        // static const char number_map_upper[] = "0123456789ABCDEFGHIJKLMNOPQRETUVWXYZ";
        static const char number_map_lower[] = "0123456789abcdefghijklmnopqretuvwxyz";

        int len = 0;
        while (numr != 0) {
            typename std::make_unsigned<T>::type rem = typename std::make_unsigned<T>::type(numr % base);
            buf[len++] = number_map_lower[rem];
            numr = numr / base;
        }

        //  10 进制特殊前缀处理
        if (mark != '\0') {
            buf[len++] = mark;
        }

        buf[len] = '\0';

        int low = 0;
        int high = len - 1;
        while (low < high) {
            char tmp = buf[high];
            buf[high] = buf[low];
            buf[low] = tmp;
            high--;
            low++;
        }

        return len;
    }

} // namespace core
} // namespace tiny
#endif // CORE_H
