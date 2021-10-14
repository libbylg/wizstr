/**
 * Copyright (c) 2021-2021 libbylg@126.com
 * tiny is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "tiny/re.h"
#include "tiny/bytes.h"

#define PCRE2_STATIC
#define PCRE2_CODE_UNIT_WIDTH 8
#include "pcre2.h"

namespace tiny {

re::re(const_pointer pattern, uint32_t flags, error_type* error) {
    ASSERT(pattern != nullptr);
    regex_code = compile(pattern, flags, error);
}

re::re(const bytes& pattern, uint32_t flags, error_type* error) {
    regex_code = compile(pattern.data(), flags, error);
}

re::operator bool() const {
    return regex_code != nullptr;
}

void* re::compile(const_pointer pattern, uint32_t flags, error_type* error) {
    int err = 0;
    PCRE2_SIZE offset = -1;
    pcre2_code* code = pcre2_compile(PCRE2_SPTR(pattern), PCRE2_ZERO_TERMINATED, flags, &err, &offset, nullptr);
    if (code != nullptr) {
        if (error != nullptr) {
            error->error = err;
            error->offset = int(offset);
            pcre2_get_error_message(err, (PCRE2_UCHAR*)(error->message), sizeof(error->message));
        }
        return code;
    }

    if (error != nullptr) {
        error->error = 0;
        error->offset = -1;
        error->message[0] = '\0';
    }

    return code;
}

// re::results re::match(const_pointer s, uint32_t options) {
//     ASSERT(regex_code != nullptr);
//
//     re::results result;
//
//     pcre2_match_data* match_data = pcre2_match_data_create_from_pattern((pcre2_code*)(regex_code), nullptr);
//     if (match_data == nullptr) {
//         return result;
//     }
//
//     int ret = pcre2_match((pcre2_code*)(regex_code), PCRE2_SPTR(s), PCRE2_ZERO_TERMINATED, 0, options, match_data, nullptr);
//     if (ret < 0) {
//         if (ret == PCRE2_ERROR_NOMATCH) {
//             pcre2_match_data_free(match_data);
//             return result;
//         }
//
//         return result;
//     }
//
//     //  匹配成功才返回有意义的结果
//     result.regex_result = match_data;
//     return result;
// }

bool re::match(const_pointer s, uint32_t options) const {
    ASSERT(regex_code != nullptr);

    //  创建一个匹配数据
    pcre2_match_data* match_data = pcre2_match_data_create_from_pattern((pcre2_code*)(regex_code), nullptr);
    if (match_data == nullptr) {
        return false;
    }

    //  执行匹配
    int rc = pcre2_match((pcre2_code*)(regex_code), PCRE2_SPTR(s), PCRE2_ZERO_TERMINATED, 0, options, match_data, nullptr);
    if (rc < 0) {
        if (rc == PCRE2_ERROR_NOMATCH) {
            pcre2_match_data_free(match_data);
            return false;
        }

        return false;
    }

    //  获取成功匹配的位置
    PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(match_data);
    ASSERT(ovector != nullptr);

    //  获取匹配的串的位置
    //  ovector 中的数据的存储方式是：
    //  0:[start,len] 1:[start,len] ... rc:[start:len]
    int match_pos = ovector[0];
    int match_length = ovector[1];

    //  如果刚好匹配完整个字符串才认为是匹配，否则认为匹配失败
    return ((match_pos == 0) && s[match_length] == '\0');
}

bool re::match(const bytes& s, uint32_t options) const {
    return match(s.data(), options);
}

int re::find(const_pointer s, uint32_t options, std::function<int(const segment_type* segs, size_type n)> func) const {
    ASSERT(regex_code != nullptr);

    //  创建一个匹配数据
    pcre2_match_data* match_data = pcre2_match_data_create_from_pattern((pcre2_code*)(regex_code), nullptr);
    if (match_data == nullptr) {
        return 0;
    }

    //  分配 segments
    uint32_t segments_count = pcre2_get_ovector_count(match_data);
    ASSERT(segments_count > 0);
    segment_type* segments = nullptr;
    if (segments_count > 20) {
        segments = (segment_type*)malloc(sizeof(segment_type) * segments_count);
    } else {
        segments = (segment_type*)alloca(sizeof(segment_type) * segments_count);
    }
    ASSERT(segments != nullptr);

    //  统计总共找到多少个
    int match_count = 0;

    //  确定起点
    PCRE2_SPTR start = PCRE2_SPTR(s);

    while (*start != '\0') {
        //  执行匹配
        int rc = pcre2_match((pcre2_code*)(regex_code), PCRE2_SPTR(start), PCRE2_ZERO_TERMINATED, 0, options, match_data, nullptr);
        if (rc < 0) {
            break;
        }
        ASSERT(rc <= segments_count);

        //  匹配成功，增加匹配次数
        match_count++;

        //  获取匹配的串的位置
        PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(match_data);
        ASSERT(ovector != nullptr);
        for (int i = 0; i < rc; i++) {
            //  ovector 中的数据的存储方式是：
            //  0:[start,len] 1:[start,len] ... rc:[start:len]
            segments[i].pos = PCRE2_SIZE(start - PCRE2_SPTR(s)) + ovector[2 * i];
            segments[i].len = ovector[(2 * i) + 1] - ovector[2 * i];
            segments[i].ptr = const_cast<bytes::pointer>(s + segments[i].pos);
        }

        //  触发回调函数
        if (func(segments, rc) != 0) {
            break;
        }

        //  移动到下一个匹配位置
        start += ovector[2];
    }

    //
    ASSERT(match_data != nullptr);
    pcre2_match_data_free(match_data);
    if (segments_count > 20) {
        ASSERT(segments != nullptr);
        free(segments);
    }

    return match_count;
}

int re::find(const bytes& s, uint32_t options, std::function<int(const segment_type* segs, size_type n)> func) const {
    return find(s.data(), options, func);
}

int re::replace(const_pointer s, const_pointer repl, std::function<int(const_pointer s, size_type n)> func) const {
    return 0;
}

int re::replace(const bytes& s, const_pointer repl, std::function<int(const_pointer s, size_type n)> func) const {
    return 0;
}

int re::split(const_pointer s, uint32_t options, std::function<int(const segment_type& segs)> func) const {
    ASSERT(regex_code != nullptr);

    //  创建一个匹配数据
    pcre2_match_data* match_data = pcre2_match_data_create_from_pattern((pcre2_code*)(regex_code), nullptr);
    if (match_data == nullptr) {
        return 0;
    }

    //  分配 segments
    segment_type segment;

    //  统计总共找到多少个
    int match_count = 0;

    //  确定起点
    PCRE2_SPTR start = PCRE2_SPTR(s);

    while (*start != '\0') {
        //  执行匹配
        int rc = pcre2_match((pcre2_code*)(regex_code), PCRE2_SPTR(start), PCRE2_ZERO_TERMINATED, 0, options, match_data, nullptr);
        if (rc < 0) {
            break;
        }

        //  匹配成功，增加匹配次数
        match_count++;

        //  获取匹配的串的位置
        PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(match_data);
        ASSERT(ovector != nullptr);
        //  ovector 中的数据的存储方式是：
        //  0:[start,len] 1:[start,len] ... rc:[start:len]
        segment.pos = start - PCRE2_SPTR(s);
        segment.len = ovector[0];
        segment.ptr = const_cast<bytes::pointer>((const_pointer)start);

        //  触发回调函数
        if (func(segment) != 0) {
            break;
        }

        //  移动到下一个匹配位置
        start += ovector[1];
    }

    if (*start != '\0') {
        segment.pos = pos_type(start - PCRE2_SPTR(s));
        segment.len = strlen(const_pointer(start));
        segment.ptr = const_cast<bytes::pointer>((bytes::const_pointer)start);
    }

    //  释放 match_data
    ASSERT(match_data != nullptr);
    pcre2_match_data_free(match_data);

    return match_count;
}

int re::split(const bytes& s, uint32_t options, std::function<int(const segment_type& segs)> func) const {
    return split(s.data(), options, func);
}

} // namespace tiny