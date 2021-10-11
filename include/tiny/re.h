//
// Created by luolijun on 2021/10/1.
//
#ifndef RE_H
#define RE_H

#include <cstdint>
#include <functional>

#include "tiny/segment.h"

namespace tiny {

class bytes;

class re {
public:
    enum : uint32_t {
        POSIX = 0x00000001,
    };

    typedef char value_type;

    typedef char* pointer;
    typedef const char* const_pointer;

    typedef char& reference;
    typedef const char& const_reference;

    typedef int32_t pos_type;

    typedef uint32_t size_type;

    struct error_type {
        int error{ 0 };
        int offset{ -1 };
        char message[128]{ 0 };
    };

    struct segment_type {
        char* ptr{ nullptr };
        int32_t pos{ -1 };
        uint32_t len{ 0 };
    };

public:
    explicit re(const char* pattern, uint32_t flags = 0, error_type* error = nullptr);
    explicit re(const bytes& pattern, uint32_t flags = 0, error_type* error = nullptr);

    operator bool() const;

    //  校验
    bool match(const_pointer s, uint32_t options = 0) const;
    bool match(const bytes& s, uint32_t options = 0) const;
    //    results match(const_pointer s, uint32_t options = 0);

    //  分割
    int split(const_pointer s, uint32_t options, std::function<int(const segment_type& segs)> func) const;
    int split(const bytes& s, uint32_t options, std::function<int(const segment_type& segs)> func) const;

    //  查找
    int find(const_pointer s, uint32_t options, std::function<int(const segment_type* segs, size_type n)> func) const;
    int find(const bytes& s, uint32_t options, std::function<int(const segment_type* segs, size_type n)> func) const;

    //  替换
    int replace(const_pointer s, const_pointer repl, std::function<int(const_pointer s, size_type n)> func) const;
    int replace(const bytes& s, const_pointer repl, std::function<int(const_pointer s, size_type n)> func) const;

public:
    static re email;
    static re ipv4;
    static re real;
    static re integer;
    static re kvpair;
    static re blanks;
    static re date;
    static re numeric;

private:
    void* compile(const_pointer pattern, uint32_t flags, error_type* error);

private:
    void* regex_code{ nullptr };
};

} // namespace tiny
#endif // RE_H
