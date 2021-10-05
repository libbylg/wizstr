//
// Created by luolijun on 2021/10/1.
//
#ifndef RE_H
#define RE_H

#include <functional>

namespace tiny {

class str;

class re {
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

    struct segment {
        const pointer start;
        pos_type pos;
        uint32_t len;
    };

public:
    explicit re();
    explicit re(const char* pattern, uint32_t flags = 0);
    explicit re(const str& pattern, uint32_t flags = 0);

    bool ok() const;

    str error() const;
    str& error(str& s) const;

    uint32_t flags() const;

    size_type subexp_num() const;

    str string() const;

    bool compile(const_pointer pattern, uint32_t flags = 0);
    bool compile(const str& pattern, uint32_t flags = 0);

    bool match(const_pointer s);
    bool match(const str& s);

    void split(const_pointer s, std::function<int(const segment& segs)> func);
    void split(const str& s, std::function<int(const segment& segs)> func);

    void find(const_pointer s, std::function<int(const segment& segs)> func);
    void find(const str& s, std::function<int(const segment& segs)> func);

    void find_submatch(const_pointer s, std::function<int(const segment* segs, size_type n)> func);
    void find_submatch(const str& s, std::function<int(const segment* segs, size_type n)> func);

    void replace(const_pointer s, const_pointer repl);
    void replace(const str& s, const_pointer repl);
    void replace(const_pointer s, const str& repl);
    void replace(const str& s, const str& repl);
    //re.escape(pattern)¶

    //re.purge()¶

    //func (re *Regexp) String() string
    //func (re *Regexp) LiteralPrefix() (prefix string, complete bool)
    //func (re *Regexp) Longest()
    //func (re *Regexp) NumSubexp() int
    //func (re *Regexp) SubexpNames() []string

    //func (re *Regexp) Expand(dst []byte, template []byte, src []byte, match []int) []byte
    //func (re *Regexp) ExpandString(dst []byte, template string, src string, match []int) []byte

    //func (re *Regexp) ReplaceAllLiteral(src, repl []byte) []byte
    //func (re *Regexp) ReplaceAllLiteralString(src, repl string) string
    //func (re *Regexp) ReplaceAll(src, repl []byte) []byte
    //func (re *Regexp) ReplaceAllString(src, repl string) string
    //func (re *Regexp) ReplaceAllFunc(src []byte, repl func([]byte) []byte) []byte
    //func (re *Regexp) ReplaceAllStringFunc(src string, repl func(string) string) string
};

} // namespace tiny
#endif //RE_H
