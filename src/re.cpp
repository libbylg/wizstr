//
// Created by luolijun on 2021/10/4.
//
#include "tiny/re.h"
#include "tiny/str.h"

#define PCRE2_STATIC
#define PCRE2_CODE_UNIT_WIDTH 8
#include "pcre2.h"

namespace tiny {

re::re(const_pointer pattern, uint32_t flags, error_type* error) {
    ASSERT(pattern != nullptr);
    regex_code = compile(pattern, flags, error);
}

re::re(const str& pattern, uint32_t flags, error_type* error) {
    regex_code = compile(pattern.data(), flags, error);
}

re::operator bool() {
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

re::results re::match(const_pointer s, uint32_t options) {
    ASSERT(regex_code != nullptr);

    re::results result;

    pcre2_match_data* match_data = pcre2_match_data_create_from_pattern((pcre2_code*)(regex_code), nullptr);
    if (match_data == nullptr) {
        return result;
    }

    int ret = pcre2_match((pcre2_code*)(regex_code), PCRE2_SPTR(s), PCRE2_ZERO_TERMINATED, 0, options, match_data, nullptr);
    if (ret < 0) {
        if (ret == PCRE2_ERROR_NOMATCH) {
            pcre2_match_data_free(match_data);
            return result;
        }

        return result;
    }

    //  匹配成功才返回有意义的结果
    result.regex_result = match_data;
    return result;
}

//bool re::match(const_pointer s, uint32_t options) {
//    ASSERT(regex_code != nullptr);
//
//    pcre2_match_data* match_data = pcre2_match_data_create_from_pattern((pcre2_code*)(regex_code), nullptr);
//    if (match_data == nullptr) {
//        return false;
//    }
//
//    int ret = pcre2_match((pcre2_code*)(regex_code), PCRE2_SPTR(s), PCRE2_ZERO_TERMINATED, 0, options, match_data, nullptr);
//    if (ret < 0) {
//        if (ret == PCRE2_ERROR_NOMATCH) {
//            pcre2_match_data_free(match_data);
//            return false;
//        }
//
//        return false;
//    }
//
//    return true;
//}
//
//bool re::match(const str& s, uint32_t options) {
//    return match(s.data(), options);
//}
//
//void re::split(const_pointer s, std::function<int(const segment_type& segs)> func) {
//    ASSERT(regex_code != nullptr);
//
//    pcre2_match_data* match_data = pcre2_match_data_create_from_pattern((pcre2_code*)(regex_code), nullptr);
//    if (match_data == nullptr) {
//        return false;
//    }
//
//    int ret = pcre2_match((pcre2_code*)(regex_code), PCRE2_SPTR(s), PCRE2_ZERO_TERMINATED, 0, options, match_data, nullptr);
//    if (ret < 0) {
//        if (ret == PCRE2_ERROR_NOMATCH) {
//            pcre2_match_data_free(match_data);
//            return false;
//        }
//
//        return false;
//    }
//
//    return true;
//}

void re::split(const str& s, std::function<int(const segment_type& segs)> func) {
}

void re::find(const_pointer s, std::function<int(const segment_type& segs)> func) {
}

void re::find(const str& s, std::function<int(const segment_type& segs)> func) {
}

void re::find_submatch(const_pointer s, std::function<int(const segment_type* segs, size_type n)> func) {
}

void re::find_submatch(const str& s, std::function<int(const segment_type* segs, size_type n)> func) {
}

void re::replace(const_pointer s, const_pointer repl, std::function<int(const_pointer s, size_type n)> func) {
}

void re::replace(const str& s, const_pointer repl, std::function<int(const_pointer s, size_type n)> func) {
}

} // namespace tiny