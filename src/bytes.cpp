/**
 * Copyright (c) 2021-2021 libbylg@126.com
 * tiny is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "tiny/bytes.h"

#include "core.h"

#include "tiny/asserts.h"
#include "tiny/chars.h"
#include "tiny/re.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>

namespace tiny {

//  进制映射范围
enum number_base {
    base_min = 2,
    base_max = 36,
};

bytes::bytes() {
    layout.init(nullptr, 0);
}

bytes::bytes(bytes::const_pointer s, bytes::size_type n) {
    layout.init(s, n);
}

bytes::bytes(bytes::value_type ch, bytes::size_type n) {
    layout.init(nullptr, n);
    layout.fill(ch, n, 0);
}

bytes::bytes(bytes::const_pointer s) {
    ASSERT(s != nullptr);
    layout.init(s, std::strlen(s));
}

bytes::bytes(bytes&& other) noexcept {
    memcpy(&layout, &other.layout, sizeof(other.layout));
    other.layout.init(nullptr, 0);
}

bytes::bytes(const bytes& other) {
    layout.init(other.data(), other.size());
}

bytes::bytes(const bytes& other, bytes::pos_type pos) {
    ASSERT(pos >= 0);
    ASSERT(pos < other.size());
    layout.init(other.layout.begin() + pos, other.layout.len());
}

bytes::bytes(const bytes& other, bytes::pos_type pos, bytes::size_type n) {
    ASSERT(pos >= 0);
    ASSERT(pos < other.size());
    ASSERT(n >= 0);
    ASSERT((pos + n) <= other.size());

    layout.init(other.data() + pos, n);
}

bytes& bytes::operator=(bytes&& other) noexcept {
    if (&other == this) {
        return *this;
    }

    layout.destroy();
    memcpy(&layout, &other.layout, sizeof(other.layout));
    other.layout.init(nullptr, 0);

    return *this;
}

bytes& bytes::operator=(const bytes& other) {
    if (&other == this) {
        return *this;
    }

    layout.destroy();
    layout.init(other.layout.begin(), other.layout.len());

    return *this;
}

bytes::~bytes() {
    layout.destroy();
}

//  迭代器
bytes::iterator bytes::begin() {
    return layout.begin();
}

bytes::iterator bytes::end() {
    return layout.end();
}

//  常量迭代器
bytes::const_iterator bytes::begin() const {
    return layout.begin();
}

bytes::const_iterator bytes::end() const {
    return layout.end();
}

bytes::const_iterator bytes::cbegin() const {
    return layout.begin();
}

bytes::const_iterator bytes::cend() const {
    return layout.end();
}

bytes::reverse_iterator bytes::rbegin() {
    return bytes::reverse_iterator(layout.end() - 1);
}

bytes::reverse_iterator bytes::rend() {
    return bytes::reverse_iterator(layout.begin() - 1);
}

bytes::const_reverse_iterator bytes::rbegin() const {
    return bytes::const_reverse_iterator(layout.end() - 1);
}

bytes::const_reverse_iterator bytes::rend() const {
    return bytes::const_reverse_iterator(layout.begin() - 1);
}

bytes::const_reverse_iterator bytes::crbegin() const {
    return bytes::const_reverse_iterator(layout.end() - 1);
}

bytes::const_reverse_iterator bytes::crend() const {
    return bytes::const_reverse_iterator(layout.begin() - 1);
}

bytes::value_type bytes::at(bytes::pos_type pos) const {
    ASSERT(pos >= 0);
    ASSERT(pos < size());
    return layout.begin()[pos];
}

bytes::value_type bytes::back() const {
    ASSERT(size() > 0);
    return *(layout.end() - 1);
}

bytes::value_type& bytes::back() {
    ASSERT(size() > 0);
    return *(layout.end() - 1);
}

bytes::value_type bytes::front() const {
    ASSERT(size() > 0);
    return layout.begin()[0];
}

bytes::value_type& bytes::front() {
    ASSERT(size() > 0);
    return layout.begin()[0];
}

bytes::size_type bytes::size() const {
    return layout.len();
}

bytes::size_type bytes::length() const {
    return layout.len();
}

bytes::size_type bytes::capacity() const {
    return layout.cap();
}

bytes::size_type bytes::max_size() const {
    return layout_trait::large_cap_max;
}

void bytes::clear() {
    layout.destroy();
    layout.init(nullptr, 0);
}

bytes::const_pointer bytes::data() const {
    return layout.begin();
}

bytes::const_pointer bytes::c_str() const {
    return layout.begin();
}

bytes::pointer bytes::data() {
    return layout.begin();
}

bytes& bytes::attach(bytes::pointer buf, bytes::size_type len, bytes::size_type cap) {
    ASSERT(buf != nullptr);
    ASSERT(len >= 0);
    ASSERT(cap >= 0);
    ASSERT(cap >= len);

    ASSERT(buf[len] == '\0');

    layout.destroy();
    layout.large.attach(cap, len, buf);

    return *this;
}

bytes& bytes::append(const bytes& s) {
    bytes::pos_type pos = layout.len();
    layout.resize(layout.len() + s.size());
    layout.fill(s.data(), s.size(), pos);
    return *this;
}

bytes& bytes::append(bytes::value_type ch) {
    bytes::pos_type pos = layout.len();
    layout.resize(layout.len() + 1);
    layout.fill(ch, 1, pos);
    return *this;
}

bytes& bytes::append(bytes::const_pointer s, bytes::size_type n) {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    bytes::pos_type pos = layout.len();
    layout.resize(layout.len() + n);
    layout.fill(s, n, pos);
    return *this;
}

bytes& bytes::append(bytes::value_type ch, bytes::size_type n) {
    bytes::size_type old_len = layout.len();
    layout.resize(layout.len() + n);
    layout.fill(ch, n, old_len);
    return *this;
}

bytes& bytes::append(bytes::const_pointer s) {
    ASSERT(s != nullptr);
    append(s, std::strlen(s));
    return *this;
}

bytes& bytes::prepend(const bytes& s) {
    ASSERT(this != &s);
    return prepend(s.layout.begin(), s.layout.len());
}

bytes& bytes::prepend(bytes::value_type ch) {
    return prepend(&ch, 1);
}

bytes& bytes::prepend(bytes::value_type ch, bytes::size_type n) {
    ASSERT(n >= 0);
    layout.flexmove(0, layout.len(), n, [](pointer fill, size_type n) {
    });
    layout.fill(ch, n, 0);
    return *this;
}

bytes& bytes::prepend(bytes::const_pointer s, bytes::size_type n) {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    layout.flexmove(0, layout.len(), n, [](pointer fill, size_type n) {
    });
    layout.fill(s, n, 0);
    return *this;
}

bytes& bytes::prepend(bytes::const_pointer s) {
    ASSERT(s != nullptr);
    return prepend(s, std::strlen(s));
}

bytes& bytes::insert(bytes::pos_type pos, const bytes& str) {
    ASSERT(pos >= 0);
    return insert(pos, str.layout.begin(), layout.len());
}

bytes& bytes::insert(bytes::pos_type pos, bytes::value_type ch) {
    return insert(pos, &ch, 1);
}

bytes& bytes::insert(bytes::pos_type pos, bytes::const_pointer s, bytes::size_type n) {
    ASSERT(pos >= 0);
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    if (pos < layout.len()) {
        layout.flexmove(pos, layout.len() - pos, n, [](pointer fill, size_type n) {
        });
        layout.fill(s, n, pos);
        return *this;
    }

    layout.resize(layout.len() + pos + n);
    layout.fill(s, n, pos);
    return *this;
}

bytes& bytes::insert(bytes::pos_type pos, bytes::const_pointer s) {
    return insert(pos, s, std::strlen(s));
}

bytes& bytes::push_back(const bytes& s) {
    return append(s.data(), s.size());
}

bytes& bytes::push_back(bytes::value_type ch) {
    return append(ch);
}

bytes& bytes::push_front(const bytes& s) {
    return prepend(s);
}

bytes& bytes::push_front(bytes::value_type ch) {
    return prepend(ch);
}

bytes& bytes::push_back(bytes::const_pointer s, bytes::size_type n) {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    return append(s, n);
}

bytes& bytes::push_front(bytes::const_pointer s, bytes::size_type n) {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    return prepend(s, n);
}

bytes::value_type bytes::pop_back() {
    ASSERT(layout.len() > 0);
    bytes::value_type ch = *(layout.end() - 1);
    layout.resize(layout.len() - 1);
    return ch;
}

bytes::value_type bytes::pop_front() {
    ASSERT(size() > 0);
    bytes::value_type ch = *(layout.begin());
    layout.clipmove(0, layout.len(), -1);
    layout.resize(layout.len() - 1);
    return ch;
}

bytes& bytes::flex_move(bytes::pos_type pos, bytes::size_type n, int offset) {
    layout.flexmove(pos, n, offset, [](pointer fill, size_type n) {
    });
    return *this;
}

bytes& bytes::flex_move(bytes::pos_type pos, bytes::size_type n, int offset, bytes::value_type fill_ch) {
    layout.flexmove(pos, n, offset, [fill_ch](pointer fill_pos, size_type n) {
        std::fill(fill_pos, fill_pos + n, fill_ch);
    });
    return *this;
}

bytes& bytes::clip_move(bytes::pos_type pos, bytes::size_type n, int offset) {
    layout.clipmove(pos, n, offset);
    return *this;
}

bytes& bytes::remove(bytes::pos_type pos) {
    ASSERT(pos >= 0);
    ASSERT(pos < layout.len());
    layout.clipmove(pos + 1, layout.len() - (pos + 1), -1);
    layout.resize(layout.len() - 1);
    return *this;
}

bytes& bytes::remove(bytes::pos_type pos, bytes::size_type n) {
    ASSERT(n > 0);
    ASSERT(pos >= 0);
    ASSERT(pos < layout.len());
    ASSERT((pos + n) < layout.len());
    if ((pos + n) < layout.len()) {
        layout.clipmove((pos + n), layout.len() - (pos + n), n);
        layout.resize(layout.len() - n);
        return *this;
    }

    layout.resize(pos);
    return *this;
}

bytes& bytes::remove(bytes::value_type ch) {
    return remove([ch](bytes::value_type c, bool& cntu) -> bool {
        return (c == ch);
    });
}

bytes& bytes::remove(bytes::const_pointer s) {
    ASSERT(s != nullptr);

    bytes::size_type slen = std::strlen(s);

    return remove(s, slen);
}

bytes& bytes::remove(bytes::const_pointer s, bytes::size_type slen) {
    ASSERT(s != nullptr);
    ASSERT(slen >= 0);

    return remove([slen, s](bytes::const_pointer search, bytes::size_type search_n, bytes::const_pointer& match, bytes::size_type& match_n) -> int {
        if (search_n < slen) {
            match = nullptr;
            match_n = 0;
            return -1;
        }

        bytes::const_pointer ptr = (bytes::const_pointer)memmem(search, search_n, s, slen);
        if (ptr == nullptr) {
            match = nullptr;
            match_n = 0;
            return -1;
        }

        match = ptr;
        match_n = slen;
        return 0;
    });
}

bytes& bytes::remove(const bytes& other) {
    return remove(other.layout.begin());
}

bytes& bytes::remove(const re& rx) {
    ASSERT(rx);

    return remove([&rx](bytes::const_pointer search, bytes::size_type search_n, bytes::const_pointer& match_ptr, bytes::size_type& match_n) -> int {
        if (search_n == 0) {
            match_ptr = nullptr;
            match_n = 0;
            return -1;
        }

        //  从 start 位置开始只查找一个
        int cnt = rx.find(search, 0, [&match_ptr, &match_n](const re::segment_type* segs, re::size_type n) -> int {
            match_ptr = segs[0].ptr;
            match_n = segs[0].len;
            return -1;
        });

        //  如果搜索不到
        if (cnt <= 0) {
            match_ptr = nullptr;
            match_n = 0;
            return 0;
        }

        //  如果已经搜索到了，直接返回
        return 0;
    });
}

bytes& bytes::remove(std::function<bool(bytes::value_type ch, bool& cntu)> func) {
    pointer w = layout.begin();
    pointer r = layout.begin();
    while (*r) {
        bool contnu = true;
        if (!func(*r, contnu)) {
            *(w++) = *r;
        }

        if (!contnu) {
            break;
        }

        r++;
    }

    layout.resize(w - layout.begin());

    return *this;
}

bytes& bytes::remove(std::function<int(bytes::const_pointer search, bytes::size_type search_n, bytes::const_pointer& match, bytes::size_type& match_n)> func) {
    bytes::pointer w = layout.begin();

    bytes::pointer r = layout.begin();
    while (*r != '\0') {

        bytes::const_pointer match_ptr = nullptr;
        bytes::size_type match_n = 0;
        int cntnu = func(r, layout.end() - r, match_ptr, match_n);

        //  没找到匹配项
        if (match_ptr == nullptr) {
            break;
        }

        ASSERT(match_n > 0);

        bytes::size_type mov_len = match_ptr - r;
        if (mov_len > 0) {
            std::memmove(w, r, mov_len);
            w += mov_len;
        }

        r = layout.begin() + (match_ptr - layout.begin()) + match_n;

        //  确定是否需要继续
        if (0 != cntnu) {
            break;
        }
    }

    while (*r != '\0') {
        *(w++) = *(r++);
    }

    layout.resize(w - layout.begin());

    return *this;
}

bytes& bytes::erase(pos_type pos, pos_type n) {
    return remove(pos, n);
}

bytes::iterator bytes::erase(const_iterator p) {
    return erase(p, (p + 1));
}

bytes::iterator bytes::erase(const_iterator first, const_iterator last) {
    ASSERT(first >= layout.begin());
    ASSERT(first < layout.end());
    ASSERT(last > layout.begin());
    ASSERT(last <= layout.end());
    layout.clipmove(last - layout.begin(), layout.end() - last, -(last - first));
    return layout.begin() + (first - layout.begin());
}

int bytes::compare(const bytes& s) const {
    return strcmp(layout.begin(), s.layout.begin());
}

int bytes::compare(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return strcmp(layout.begin(), s);
}

int bytes::compare(bytes::const_pointer s, bytes::size_type max_n) const {
    ASSERT(s != nullptr);
    return strncmp(layout.begin(), s, max_n);
}

int bytes::compare(const bytes& other, bytes::size_type max_n) const {
    return strncmp(layout.begin(), other.layout.begin(), max_n);
}

int bytes::compare(bytes::value_type c) const {
    return (size() == 1) && (layout.begin()[0] == c);
}

bool bytes::contains(const bytes& s) const {
    return strstr(layout.begin(), s.layout.begin()) != nullptr;
}

bool bytes::contains(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return strstr(layout.begin(), s) != nullptr;
}

bool bytes::contains(bytes::const_pointer s, bytes::size_type n) const {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    return memmem(layout.begin(), layout.len(), s, n) != nullptr;
}

bool bytes::contains(bytes::value_type ch) const {
    return strchr(layout.begin(), ch) != nullptr;
}

bool bytes::contains(const re& r) const {
    bool found = false;
    r.find(*this, 0, [&found](const re::segment_type* segment, re::size_type n) -> int {
        found = true;
        return -1;
    });
    return found;
}

bytes::size_type bytes::count(bytes::const_pointer s, bytes::size_type n) const {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);

    if ((n == 0) || (s[0] == '\0')) {
        return layout.len() + 1;
    }

    int cnt = 0;
    const_pointer start = layout.begin();
    while ((start = (const_pointer)memmem(start, (layout.end() - start), s, n)) != nullptr) {
        cnt++;
        start += n;
    }

    return cnt;
}

bytes::size_type bytes::count(const bytes& other) const {
    return count(other.layout.begin(), other.layout.len());
}

bytes::size_type bytes::count(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return count(s, std::strlen(s));
}

bytes::size_type bytes::count(bytes::value_type ch) const {
    int cnt = 0;

    const_pointer start = layout.begin();
    while ((start = std::strchr(start, ch)) != nullptr) {
        cnt++;
        start++;
    }

    return cnt;
}

bytes::size_type bytes::count(const re& rx) const {
    ASSERT(rx);

    int cnt = 0;
    rx.find(layout.begin(), 0, [&cnt](const re::segment_type* segs, re::size_type n) -> int {
        cnt++;
        return 0;
    });
    return cnt;
}

bytes::size_type bytes::count(std::function<bool(bytes::value_type ch, bool& cntu)> matcher) const {
    int cnt = 0;

    for (const_pointer ptr = layout.begin(); ptr != layout.end(); ptr++) {

        //  看下字符是否匹配
        bool cntu = true;
        if (matcher(*ptr, cntu)) {
            cnt++;
        }

        //  检查一下是否需要继续，返回非 0 表示终止扫描
        if (!cntu) {
            break;
        }
    }

    return cnt;
}

bool bytes::has_suffix(const bytes& other) const {
    return has_suffix(other.data(), other.size());
}

bool bytes::has_suffix(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return has_suffix(s, std::strlen(s));
}

bool bytes::has_suffix(bytes::value_type ch) const {
    if (size() > 0) {
        return (back() == ch);
    }

    return ch == '\0';
}

bool bytes::has_suffix(bytes::const_pointer s, bytes::size_type n) const {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);

    if (n == 0) {
        return true;
    }

    auto pt = layout.end() - 1;
    bytes::const_pointer ps = s + (n - 1);
    while ((pt != (layout.begin() - 1)) && (ps != (s - 1))) {
        if (*pt != *ps) {
            break;
        }
        pt--;
        ps--;
    }

    return (ps == (s - 1));
}

bool bytes::ends_with(const bytes& other) const noexcept {
    return has_suffix(other);
}

bool bytes::ends_with(bytes::value_type ch) const noexcept {
    return has_suffix(ch);
}

bool bytes::ends_with(bytes::const_pointer s) const {
    return has_suffix(s);
}

bool bytes::has_prefix(const bytes& other) const {
    return has_prefix(other.data(), other.size());
}

bool bytes::has_prefix(bytes::const_pointer s) const {
    ASSERT(s != nullptr);

    bytes::const_pointer pt = layout.begin();
    bytes::const_pointer ps = s;
    while (*pt != '\0' && *ps != '\0') {
        if (*pt != *ps) {
            break;
        }
        pt++;
        ps++;
    }

    return (*ps == '\0');
}

bool bytes::has_prefix(bytes::value_type ch) const {
    if (size() > 0) {
        return (front() == ch);
    }

    return ch == '\0';
}

bool bytes::starts_with(const bytes& other) const noexcept {
    return has_prefix(other);
}

bool bytes::starts_with(bytes::value_type ch) const noexcept {
    return has_prefix(ch);
}

bool bytes::starts_with(bytes::const_pointer s) const {
    return has_prefix(s);
}

bool bytes::has_prefix(bytes::const_pointer s, bytes::size_type n) const {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);

    if (n == 0) {
        return true;
    }

    bytes::const_pointer pt = layout.begin();
    bytes::const_pointer ps = s;
    while ((pt != layout.end()) && (ps != (s + n))) {
        if (*pt != *ps) {
            break;
        }
        pt++;
        ps++;
    }

    return (ps == (s + n));
}

bytes& bytes::remove_prefix(bytes::const_pointer s, bytes::size_type n) {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);

    if (has_prefix(s, n)) {
        if (n > 0) {
            layout.clipmove(n, layout.len() - n, -n);
            layout.resize(layout.len() - n);
        }
    }

    return *this;
}

bytes& bytes::remove_prefix(const bytes& other) {
    return remove_prefix(other.layout.begin(), other.layout.len());
}

bytes& bytes::remove_prefix(bytes::const_pointer s) {
    return remove_prefix(s, std::strlen(s));
}

bytes& bytes::remove_prefix(bytes::value_type c) {
    return remove_prefix(&c, 1);
}

bytes& bytes::remove_suffix(bytes::const_pointer s, bytes::size_type n) {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);

    if (has_suffix(s, n)) {
        layout.resize(layout.len() - n);
    }

    return *this;
}

bytes& bytes::remove_suffix(const bytes& s) {
    return remove_suffix(s.layout.begin(), s.layout.len());
}

bytes& bytes::remove_suffix(bytes::const_pointer s) {
    ASSERT(s != nullptr);
    return remove_suffix(s, std::strlen(s));
}

bytes& bytes::remove_suffix(bytes::value_type c) {
    return remove_suffix(&c, 1);
}

bytes& bytes::fill(bytes::value_type fill_ch) {
    layout.fill(fill_ch, layout.len(), 0);
    return *this;
}

bytes& bytes::fill(bytes::pos_type pos, bytes::value_type ch, bytes::size_type n) {
    ASSERT(n > 0);
    ASSERT(pos >= 0);
    ASSERT(pos < size());
    ASSERT((pos + n) >= 0);
    ASSERT((pos + n) <= size());

    layout.fill(ch, n, pos);
    return *this;
}

bytes& bytes::fill(bytes::pos_type pos, bytes::const_pointer s, bytes::size_type n) {
    ASSERT(n > 0);
    ASSERT(s != nullptr);
    ASSERT(pos >= 0);
    ASSERT(pos < size());
    ASSERT((pos + n) >= 0);
    ASSERT((pos + n) <= size());

    layout.fill(s, n, pos);
    return *this;
}

bytes& bytes::fill(bytes::pos_type fill_from, bytes::size_type fill_n, bytes::const_pointer s, bytes::size_type n) {
    ASSERT(fill_from >= 0);
    ASSERT(fill_from < size());
    ASSERT(fill_n >= 0);
    ASSERT(fill_n < size());

    ASSERT(s != nullptr);
    ASSERT(n > 0);

    bytes::pos_type pos = fill_from;
    int times = fill_n / n;
    for (int i = 0; i < times; i++) {
        layout.fill(s, n, pos);
        pos += n;
    }

    layout.fill(s, ((fill_from + fill_n) - pos), pos);

    return *this;
}

bytes& bytes::fill(bytes::pos_type fill_from, bytes::size_type fill_n, bytes::const_pointer s) {
    ASSERT(fill_from >= 0);
    ASSERT(fill_from < size());
    ASSERT(fill_n >= 0);
    ASSERT(fill_n < size());

    ASSERT(s != nullptr);

    return fill(fill_from, fill_n, s);
    ;
}

bytes::pos_type bytes::index_of(bytes::const_pointer s, bytes::size_type n, bytes::pos_type from) const {
    ASSERT(s != nullptr);
    ASSERT(from >= 0);
    ASSERT(from < size());

    const_pointer pos = layout.begin() + from;
    pos = (const_pointer)memmem(pos, layout.end() - pos, s, n);
    if (pos == nullptr) {
        return bytes::npos;
    }

    return bytes::pos_type(pos - layout.begin());
}

bytes::pos_type bytes::index_of(const bytes& other, bytes::pos_type from) const {
    return index_of(other.data(), from);
}

bytes::pos_type bytes::index_of(bytes::const_pointer s, bytes::pos_type from) const {
    ASSERT(s != nullptr);
    ASSERT(from >= 0);
    ASSERT(from < size());

    const_pointer d = data();
    const_pointer pos = std::strstr(d + from, s);
    if (pos == nullptr) {
        return bytes::npos;
    }

    return bytes::pos_type(pos - d);
}

bytes::pos_type bytes::index_of(bytes::value_type ch, bytes::pos_type from) const {
    ASSERT(from >= 0);
    ASSERT(from < size());

    const_pointer d = data();
    const_pointer pos = std::strchr(d + from, ch);
    if (pos == nullptr) {
        return bytes::npos;
    }

    return bytes::pos_type(pos - d);
}

bytes::pos_type bytes::index_of(const re& rx, bytes::pos_type from) const {
    ASSERT(from >= 0);
    ASSERT(from < layout.len());

    bytes::pos_type pos = -1;
    rx.find(layout.begin() + from, 0, [&pos](const re::segment_type* segs, re::size_type n) -> int {
        pos = segs[0].pos;
        return bytes::npos;
    });

    return from + pos;
}

bytes::pos_type bytes::index_of(std::function<bool(bytes::value_type ch, bool& cntu)> matcher, bytes::pos_type from, bytes::pos_type to) const {
    ASSERT(from >= 0);
    ASSERT(from <= size());
    ASSERT(to >= 0);
    ASSERT(to <= size());

    ASSERT(from <= to);

    for (bytes::const_pointer ptr = layout.begin() + from; ptr != layout.begin() + to; ptr++) {
        //  如果匹配成功，返回位置
        bool cntnu = true;
        if (matcher(*ptr, cntnu)) {
            return ptr - layout.begin();
        }

        //  判断是否需要继续
        if (!cntnu) {
            break;
        }
    }

    return bytes::npos;
}

bytes::pos_type bytes::last_index_of(bytes::const_pointer s, bytes::size_type n, bytes::pos_type from) const {
    bytes::pos_type found_pos = npos;

    bytes::pos_type start_pos = from;
    while (start_pos != bytes::npos) {
        start_pos = index_of(s, n, start_pos);
        if (start_pos == npos) {
            break;
        }
        found_pos = start_pos;
        start_pos += n;
    }

    return found_pos;
}

bytes::pos_type bytes::last_index_of(const bytes& other, bytes::pos_type from) const {
    return last_index_of(other.data(), from);
}

bytes::pos_type bytes::last_index_of(bytes::value_type ch, bytes::pos_type from) const {
    ASSERT(from >= 0);
    ASSERT(from < size());

    const_pointer d = data();
    const_pointer pos = std::strrchr(d + from, ch);
    if (pos == nullptr) {
        return bytes::npos;
    }

    return bytes::pos_type(pos - d);
}

bytes::pos_type bytes::last_index_of(bytes::const_pointer s, bytes::pos_type from) const {
    ASSERT(from >= 0);
    ASSERT(from < size());

    const_pointer d = data();
    const_pointer pos = core::strrstr(d + from, s);
    if (pos == nullptr) {
        return bytes::npos;
    }

    return bytes::pos_type(pos - d);
}

bytes::pos_type bytes::last_index_of(const re& rx, bytes::pos_type from) const {
    bytes::pos_type found_pos = npos;

    rx.find(layout.begin() + from, layout.len() - from, //
        [&found_pos](const re::segment_type* segs, size_type n) -> int {
            found_pos = segs[0].pos;
            return 0;
        });

    return found_pos + from;
}

bytes::pos_type bytes::last_index_of(std::function<bool(bytes::value_type ch, bool& cntu)> matcher, bytes::pos_type from, bytes::pos_type to) const {
    ASSERT(from >= 0);
    ASSERT(from < layout.len());
    ASSERT(to >= 0);
    ASSERT(to <= layout.len());

    ASSERT(from <= to);

    bool cntu = true;
    for (bytes::const_pointer ptr = layout.begin() + to - 1; ptr >= layout.begin() + from; ptr--) {
        if (matcher(*ptr, cntu)) {
            return bytes::pos_type(ptr - layout.begin());
        }

        if (!cntu) {
            return bytes::npos;
        }
    }

    return bytes::npos;
}

bytes::pos_type bytes::find(const bytes& other, bytes::pos_type pos) const noexcept {
    return index_of(other, pos);
}

bytes::pos_type bytes::find(const_pointer s, bytes::pos_type pos, bytes::size_type count) const {
    return index_of(s, count, pos);
}

bytes::pos_type bytes::find(const_pointer s, bytes::pos_type pos) const {
    return index_of(s, pos);
}

bytes::pos_type bytes::find(value_type ch, bytes::pos_type pos) const {
    return index_of(ch, pos);
}

bytes::pos_type bytes::rfind(const bytes& other, bytes::pos_type pos) const {
    return last_index_of(other, pos);
}

bytes::pos_type bytes::rfind(bytes::const_pointer s, bytes::pos_type pos, bytes::size_type count) const {
    return last_index_of(s, count, pos);
}

bytes::pos_type bytes::rfind(bytes::const_pointer s, bytes::pos_type pos) const {
    return last_index_of(s, pos);
}

bytes::pos_type bytes::rfind(bytes::value_type ch, bytes::pos_type pos) const {
    return last_index_of(ch, pos);
}

bytes::pos_type bytes::find_first_of(const bytes& str, bytes::pos_type pos) const {
    return find_first_of(str.layout.begin(), pos, str.layout.len());
}

bytes::pos_type bytes::find_first_of(bytes::const_pointer s, bytes::pos_type pos, bytes::size_type count) const {
    ASSERT(false); //  TODO bytes::pos_type bytes::find_first_of(bytes::value_type ch, bytes::pos_type pos) const
    return npos;
}

bytes::pos_type bytes::find_first_of(bytes::const_pointer s, bytes::pos_type pos) const {
    return find_first_of(s, pos, std::strlen(s));
}

bytes::pos_type bytes::find_first_of(bytes::value_type ch, bytes::pos_type pos) const {
    return index_of(ch, pos);
}

bytes::pos_type bytes::find_first_not_of(const bytes& other, bytes::pos_type pos) const {
    return find_first_not_of(other.layout.begin(), pos, other.layout.len());
}

bytes::pos_type bytes::find_first_not_of(bytes::const_pointer s, bytes::pos_type pos, bytes::size_type count) const {
    ASSERT(false); //  TODO bytes::pos_type bytes::find_first_not_of(bytes::value_type ch, bytes::pos_type pos) const
    return npos;
}

bytes::pos_type bytes::find_first_not_of(bytes::const_pointer s, bytes::pos_type pos) const {
    return find_first_not_of(s, pos, std::strlen(s));
}

bytes::pos_type bytes::find_first_not_of(bytes::value_type ch, bytes::pos_type pos) const {
    ASSERT(false); //  TODO bytes::pos_type bytes::find_first_not_of(bytes::value_type ch, bytes::pos_type pos) const
    return npos;
}

bytes::pos_type bytes::find_last_of(const bytes& str, bytes::pos_type pos) const {
    return find_last_of(str.layout.begin(), pos, str.layout.len());
}

bytes::pos_type bytes::find_last_of(bytes::const_pointer s, bytes::pos_type pos, bytes::size_type count) const {
    ASSERT(false); //  TODO bytes::pos_type bytes::find_last_of(bytes::value_type ch, bytes::pos_type pos) const
    return npos;
}

bytes::pos_type bytes::find_last_of(bytes::const_pointer s, bytes::pos_type pos) const {
    return find_last_of(s, pos, std::strlen(s));
}

bytes::pos_type bytes::find_last_of(bytes::value_type ch, bytes::pos_type pos) const {
    return last_index_of(ch, pos);
}

bytes::size_type bytes::find_last_not_of(const bytes& str, bytes::size_type pos) const {
    return bytes::find_last_not_of(str.layout.begin(), pos, str.layout.len());
}

bytes::size_type bytes::find_last_not_of(bytes::const_pointer s, bytes::size_type pos, size_type count) const {
    ASSERT(false); //  TODO - bytes::size_type bytes::find_last_not_of(bytes::const_pointer s, bytes::size_type pos, size_type count) const
    return bytes::npos;
}

bytes::size_type bytes::find_last_not_of(bytes::const_pointer s, bytes::size_type pos) const {
    return bytes::find_last_not_of(s, pos, std::strlen(s));
}

bytes::size_type bytes::find_last_not_of(bytes::value_type ch, bytes::size_type pos) const {
    ASSERT(false); //  TODO - bytes::size_type bytes::find_last_not_of(bytes::value_type ch, bytes::size_type pos = npos) const
    return bytes::npos;
}

bytes::pos_type bytes::index_of_field(bytes::pos_type from, bytes::const_pointer& s, bytes::size_type& n) const {
    ASSERT(from >= 0);
    ASSERT(from <= size());

    if (from == size()) {
        return bytes::npos;
    }

    bytes::const_pointer ptr = layout.begin() + from;

    //  找到第一个非空白的字符，确定起点
    while ((*ptr != '\0') && chars::match(*ptr, chars::SPACE)) {
        ptr++;
    }

    //  找下一个空白或者结束位置，确定终点 // 不是最优，因为这里多了依次额外的检测
    n = 0;
    while ((ptr[n] != '\0') && !chars::match(ptr[n], chars::SPACE)) {
        n++;
    }

    if (n <= 0) {
        return bytes::npos;
    }

    s = ptr;
    return bytes::pos_type(ptr - layout.begin());
}

bytes::pos_type bytes::last_index_of_field(bytes::pos_type from, bytes::const_pointer& s, bytes::size_type& n) const {
    ASSERT(from >= 0);
    ASSERT(from <= size());

    s = nullptr;
    n = 0;
    walk_field(from, offset_type(layout.len() - from), [&s, &n](bytes::const_pointer field, bytes::size_type field_n) -> int {
        ASSERT(field != nullptr);
        ASSERT(field_n >= 0);
        s = field;
        n = field_n;
        return -1;
    });

    if (s == nullptr) {
        return bytes::npos;
    }

    return s - layout.begin();
}

void bytes::walk(bytes::pos_type from, bytes::offset_type offset, std::function<int(bytes::const_pointer s, size_type n, bytes::const_pointer next)> func) const {
    ASSERT(from >= 0);
    ASSERT(from <= size());
}

void bytes::walk_field(bytes::pos_type from, bytes::offset_type offset, std::function<int(bytes::const_pointer s, bytes::size_type n)> func) const {
    ASSERT(from >= 0);
    ASSERT(from <= size());
    ASSERT((from + offset) >= 0);
    ASSERT((from + offset) <= size());

    bytes::pos_type to = (from + offset);

    if (from > (from + offset)) {
        ASSERT(false); //  TODO 缺少逆向查找的实现 ： walk_field
        return;
    }

    if (from == size()) {
        return;
    }

    bytes::const_pointer ptr = layout.begin() + from;

    //  找到第一个非空白的字符，确定起点
    while ((ptr != layout.begin() + to) && chars::match(*ptr, chars::SPACE)) {
        ptr++;
    }

    //  找下一个空白或者结束位置，确定终点 // 不是最优，因为这里多了依次额外的检测
    size_type n = 0;
    while ((ptr[n] != '\0') && !chars::match(ptr[n], chars::SPACE)) {
        n++;
    }

    if (n <= 0) {
        return;
    }

    func(ptr, n);
}

void bytes::walk_field(bytes::pos_type from, bytes::offset_type offset, std::function<int(bytes::pointer s, bytes::size_type n)> func) {
    ASSERT(from >= 0);
    ASSERT(from <= size());
    ASSERT((from + offset) >= 0);
    ASSERT((from + offset) <= size());

    bytes::pos_type to = (from + offset);

    if (from > (from + offset)) {
        ASSERT(false); //  TODO 缺少逆向查找的实现 ： walk_field
        return;
    }

    if (from == size()) {
        return;
    }

    bytes::pointer ptr = layout.begin() + from;

    //  找到第一个非空白的字符，确定起点
    while ((ptr != layout.begin() + to) && chars::match(*ptr, chars::SPACE)) {
        ptr++;
    }

    //  找下一个空白或者结束位置，确定终点 // 不是最优，因为这里多了依次额外的检测
    size_type n = 0;
    while ((ptr[n] != '\0') && !chars::match(ptr[n], chars::SPACE)) {
        n++;
    }

    if (n <= 0) {
        return;
    }

    func(ptr, n);
}

void bytes::walk_byte(bytes::pos_type from, bytes::offset_type offset, std::function<int(bytes::const_pointer ptr)> func) const {
    ASSERT(from >= 0);
    ASSERT(from <= size());
    ASSERT((from + offset) >= 0);
    ASSERT((from + offset) <= size());

    bytes::pos_type to = from + offset;
    if (from > to) {
        for (const_pointer ptr = layout.begin() + from; ptr != layout.begin() + to; ptr--) {
            if (func(ptr) != 0) {
                break;
            }
        }
        return;
    }

    for (const_pointer ptr = layout.begin() + from; ptr != layout.begin() + to; ptr++) {
        if (func(ptr) != 0) {
            break;
        }
    }

    return;
}

void bytes::walk_byte(bytes::pos_type from, bytes::offset_type offset, std::function<int(bytes::pointer ptr)> func) {
    ASSERT(from >= 0);
    ASSERT(from <= size());
    ASSERT((from + offset) >= 0);
    ASSERT((from + offset) <= size());

    bytes::pos_type to = from + offset;
    if (from > to) {
        for (pointer ptr = layout.begin() + from; ptr != layout.begin() + to; ptr--) {
            if (func(ptr) != 0) {
                break;
            }
        }
        return;
    }

    for (pointer ptr = layout.begin() + from; ptr != layout.begin() + to; ptr++) {
        if (func(ptr) != 0) {
            break;
        }
    }

    return;
}

bool bytes::is_match(const re& rx) const {
    return rx.match(*this, 0);
}

bool bytes::is_match(const bytes& pattern) const {
    return re(pattern).match(*this, 0);
}

bool bytes::is_match(bytes::const_pointer pattern) const {
    ASSERT(pattern != nullptr);
    return re(pattern).match(*this, 0);
}

bool bytes::is_match_wild(const bytes& pattern) const {
    return tiny::core::wildcmp(pattern.data(), data()) == 0;
}

bool bytes::is_match_wild(bytes::const_pointer pattern) const {
    ASSERT(pattern != nullptr);
    return tiny::core::wildcmp(pattern, data()) == 0;
}

bool bytes::is_match(uint16_t charset) const {
    if (layout.len() == 0) {
        return false;
    }

    for (const_pointer ptr = layout.begin(); ptr != layout.end(); ptr++) {
        if (0 == (chars::mapping[*ptr] & charset)) {
            return false;
        }
    }

    return true;
}

bool bytes::is_empty() const {
    return layout.len() == 0;
}

bool bytes::is_lower() const {
    return is_match(chars::UPPER);
}

bool bytes::is_upper() const {
    return is_match(chars::LOWER);
}

bool bytes::is_title() const {

    //  遇到第一个字符时，停止
    bool result = false;
    walk_byte(0, layout.len(), [&result](const_pointer ptr) -> int {
        if (chars::match(*ptr, chars::ALPHA)) {
            result = chars::match(*ptr, chars::UPPER);
            return -1;
        }

        return 0;
    });

    return result;
}

bool bytes::is_digit() const {
    return is_match(chars::DIGIT);
}

bool bytes::is_ascii() const {
    if (layout.len() == 0) {
        return false;
    }

    for (const_pointer ptr = layout.begin(); ptr != layout.end(); ptr++) {
        if ((uint8_t(*ptr) & 0x80) != 0) {
            return false;
        }
    }

    return true;
}

bool bytes::is_alpha() const {
    return is_match(chars::ALPHA);
}

bool bytes::is_alnum() const {
    return is_match(chars::ALNUM);
}

bool bytes::is_space() const {
    return is_match(chars::SPACE);
}

bool bytes::is_blank() const {
    return is_match(chars::BLANK);
}

bool bytes::is_print() const {
    return is_match(chars::PRINT);
}

bool bytes::is_graph() const {
    return is_match(chars::GRAPH);
}

bool bytes::is_identifier() const {

    if (layout.len() == 0) {
        return false;
    }

    const_pointer ptr = layout.begin();
    if (!((*ptr == '_') || chars::match(*ptr, chars::ALPHA))) {
        return false;
    }

    for (ptr++; ptr != layout.end(); ptr++) {
        if (!((*ptr == '_') || chars::match(*ptr, chars::ALNUM))) {
            return false;
        }
    }

    return true;
}

bool bytes::is_bool() const {
    if (layout.len() <= 0) {
        return false;
    }

    //  "1"     "0"
    //  "on"    "off"
    //  "ON"    "OFF"
    //  "Yes"   "No"
    //  "yes"   "no"
    //  "YES"   "NO"
    //  "True"  "False"
    //  "true"  "false"
    //  "TRUE"  "FALSE"

    const_pointer ptr = layout.begin();
    switch (layout.len()) {
        case 5:
            if ((ptr[0] == 'f') && (ptr[1] == 'a') && (ptr[2] == 'l') && (ptr[3] == 's') && (ptr[4] == 'e')) {
                return true;
            }
            if ((ptr[0] == 'F') && (ptr[1] == 'a') && (ptr[2] == 'l') && (ptr[3] == 's') && (ptr[4] == 'e')) {
                return true;
            }
            if ((ptr[0] == 'F') && (ptr[1] == 'A') && (ptr[2] == 'L') && (ptr[3] == 'S') && (ptr[4] == 'E')) {
                return true;
            }
            return false;
        case 4:
            if ((ptr[0] == 't') && (ptr[1] == 'r') && (ptr[2] == 'u') && (ptr[3] == 'e')) {
                return true;
            }
            if ((ptr[0] == 'T') && (ptr[1] == 'r') && (ptr[2] == 'u') && (ptr[3] == 'e')) {
                return true;
            }
            if ((ptr[0] == 'T') && (ptr[1] == 'R') && (ptr[2] == 'U') && (ptr[3] == 'E')) {
                return true;
            }
            return false;
        case 3:
            if ((ptr[0] == 'y') && (ptr[1] == 'e') && (ptr[2] == 's')) {
                return true;
            }
            if ((ptr[0] == 'Y') && (ptr[1] == 'e') && (ptr[2] == 's')) {
                return true;
            }
            if ((ptr[0] == 'Y') && (ptr[1] == 'E') && (ptr[2] == 'S')) {
                return true;
            }
            if ((ptr[0] == 'o') && (ptr[1] == 'f') && (ptr[2] == 'f')) {
                return true;
            }
            if ((ptr[0] == 'O') && (ptr[1] == 'f') && (ptr[2] == 'f')) {
                return true;
            }
            if ((ptr[0] == 'O') && (ptr[1] == 'F') && (ptr[2] == 'F')) {
                return true;
            }
            return false;
        case 2:
            if ((ptr[0] == 'o') && (ptr[1] == 'n')) {
                return true;
            }
            if ((ptr[0] == 'O') && (ptr[1] == 'n')) {
                return true;
            }
            if ((ptr[0] == 'O') && (ptr[1] == 'N')) {
                return true;
            }
            if ((ptr[0] == 'n') && (ptr[1] == 'o')) {
                return true;
            }
            if ((ptr[0] == 'N') && (ptr[1] == 'o')) {
                return true;
            }
            if ((ptr[0] == 'N') && (ptr[1] == 'O')) {
                return true;
            }
            return false;
        case 1:
            switch (*ptr) {
                case '0':
                    return true;
                case '1':
                    return true;
                default:
                    return false;
            }
        default:
            return false;
    }

    return false;
}

bytes bytes::left(bytes::size_type n) const {
    return bytes(layout.begin(), n);
}

bytes bytes::right(bytes::size_type n) const {
    return bytes(layout.end() - n, n);
}

bytes bytes::substr(bytes::pos_type pos, int n) const {
    ASSERT(pos >= 0);
    ASSERT(pos < size());

    if (n == bytes::npos) {
        return bytes(layout.begin(), layout.len());
    }

    if ((pos + n) >= layout.len()) {
        return bytes(layout.begin() + pos, layout.len() - pos);
    }

    return bytes(layout.begin() + pos, n);
}

bytes bytes::cutstr(pos_type pos, offset_type offset_n) const {
    if (offset_n < 0) {
        return bytes(layout.begin() + pos + offset_n, -offset_n);
    }

    if (offset_n > 0) {
        return bytes(layout.begin() + pos, offset_n);
    }

    return bytes();
}

bytes& bytes::ljust(bytes::size_type width, bytes::value_type fill_ch, bool truncate) {
    ASSERT(width >= 0);

    // 宽度大于 bytes 的总长度时，在尾部追加数据
    if (width > layout.len()) {
        bytes::size_type old_len = layout.len();
        layout.resize(width);
        layout.fill(fill_ch, width - old_len, old_len);
        return *this;
    }

    //  宽度一样，啥也不需要做
    if (width == layout.len()) {
        return *this;
    }

    //  width 太小时，由 truncate 参数决定是否需要掐断
    if (truncate) {
        layout.resize(width);
    }

    return *this;
}

bytes& bytes::rjust(bytes::size_type width, bytes::value_type fill_ch, bool truncate) {
    ASSERT(width >= 0);

    // 宽度大于 bytes 的总长度时，在尾部追加数据
    if (width > layout.len()) {
        bytes::size_type old_len = layout.len();
        layout.flexmove(0, old_len, (width - old_len), [](pointer fill, size_type n) {
        });
        layout.fill(fill_ch, (width - old_len), 0);
        return *this;
    }

    //  宽度一样，啥也不需要做
    if (width == layout.len()) {
        return *this;
    }

    //  width 太小时，由 truncate 参数决定是否需要掐断
    if (truncate) {
        layout.resize(width);
    }

    return *this;
}

bytes& bytes::center(bytes::size_type width, bytes::value_type fill_ch, bool truncate) {
    ASSERT(width >= 0);

    // 宽度大于 bytes 的总长度时，在尾部追加数据
    if (width > layout.len()) {
        bytes::size_type old_len = layout.len();
        bytes::size_type lfill_len = (width - old_len) / 2;
        bytes::size_type rfill_len = (width - old_len) - lfill_len;

        //  不够优秀：存在多余的一次拷贝
        layout.resize(width);

        //  字符串整体右移
        layout.flexmove(0, old_len, lfill_len, [](pointer fill, size_type n) {
        });

        layout.fill(fill_ch, lfill_len, 0);
        layout.fill(fill_ch, rfill_len, lfill_len + old_len);
        return *this;
    }

    //  宽度一样，啥也不需要做
    if (width == layout.len()) {
        return *this;
    }

    //  width 太小时，由 truncate 参数决定是否需要掐断
    if (truncate) {
        layout.resize(width);
    }

    return *this;
}

bytes& bytes::zfill(bytes::size_type width) {
    ASSERT(width >= 0);

    //  如果宽度太窄
    if (width <= layout.len()) {
        return *this;
    }

    //  如果宽度足够
    size_type fill_n = width - layout.len();
    pos_type fill_pos = 0;

    //  如果字符串长度大于0，那么需要判断是否有正负号
    if (layout.len() > 0) {
        //  如果有正负号，那么需要移动的数据的偏移就不是0了
        if ((layout.begin()[0] == '+') || (layout.begin()[0] == '-')) {
            fill_pos = 1;
        }

        //  移动数据，腾出空位
        layout.flexmove(fill_pos, layout.len() - fill_pos, fill_n, [](pointer begin, size_type n) {
        });
    } else {
        //  直接调整长度
        layout.resize(fill_n);
    }

    //  填充数据
    layout.fill('0', fill_n, fill_pos);
    return *this;
}

bytes& bytes::replace(bytes::pos_type pos, bytes::size_type n, const bytes& after) {
    ASSERT(false); //  TODO - bytes& bytes::replace(bytes::pos_type pos, bytes::size_type n, const bytes& after)
    return *this;
}

bytes& bytes::replace(bytes::pos_type pos, bytes::size_type n, bytes::value_type after) {
    ASSERT(false); //  TODO - bytes& bytes::replace(bytes::pos_type pos, bytes::size_type n, bytes::value_type after)
    return *this;
}

bytes& bytes::replace(bytes::pos_type pos, bytes::size_type n, bytes::const_pointer unicode, bytes::size_type size) {
    ASSERT(false); //  TODO - bytes& bytes::replace(bytes::pos_type pos, bytes::size_type n, bytes::const_pointer unicode, bytes::size_type size)
    return *this;
}

bytes& bytes::replace(bytes::value_type before, bytes::value_type after) {
    ASSERT(false); //  TODO - bytes& bytes::replace(bytes::value_type before, bytes::value_type after)
    return *this;
}

bytes& bytes::replace(bytes::const_pointer before, bytes::size_type blen, bytes::const_pointer after, bytes::size_type alen) {
    ASSERT(false); //  TODO - bytes& bytes::replace(bytes::const_pointer before, bytes::size_type blen, bytes::const_pointer after, bytes::size_type alen)
    return *this;
}

bytes& bytes::replace(const bytes& before, const bytes& after) {
    ASSERT(false); //  TODO - bytes& bytes::replace(const bytes& before, const bytes& after)
    return *this;
}

bytes& bytes::replace(bytes::value_type ch, const bytes& after) {
    ASSERT(false); //  TODO - bytes& bytes::replace(bytes::value_type ch, const bytes& after)
    return *this;
}

bytes& bytes::replace(const re& rx, const bytes& after) {
    ASSERT(false); //  TODO - bytes& bytes::replace(const re& rx, const bytes& after)
    return *this;
}

bytes& bytes::replace(std::function<int(bytes::value_type key, bytes::value_type& val)> func) {
    ASSERT(false); //  TODO - bytes& bytes::replace(std::function<int(bytes::value_type key, bytes::value_type& val)> func)
    return *this;
}

bytes bytes::repeat(bytes::size_type times) const {
    ASSERT(times >= 0);

    bytes result;

    result.resize(size() * times);

    bytes::pos_type pos = 0;
    for (bytes::size_type i = 0; i < times; i++) {
        result.fill(pos + i * size(), data(), size());
    }

    return result;
}

bytes bytes::join(const std::vector<bytes>& s) const {
    auto itr = s.begin();
    return join([&s, &itr]() -> bytes::const_pointer {
        bytes::const_pointer s = itr->data();
        itr++;
        return s;
    });
}

bytes bytes::join(bytes::const_pointer s, ...) const {
    va_list valist;
    va_start(valist, s);

    bytes result = join([&valist]() -> bytes::const_pointer {
        return va_arg(valist, bytes::const_pointer);
    });

    va_end(valist);

    return result;
}

bytes bytes::join(std::function<bytes::const_pointer()> provider) const {
    bytes result;

    //  第一次特殊处理
    const_pointer ptr = provider();
    if (ptr == nullptr) {
        return result;
    }

    //  追加第一个数据
    result.append(ptr);

    //  获取下一个
    ptr = provider();

    //  第二次及之后都要追加 *this
    while (ptr != nullptr) {
        result.append(*this);
        result.append(ptr);
        ptr = provider();
    }

    return result;
}

bytes bytes::join(std::initializer_list<bytes::const_pointer> ptr_list) const {
    auto itr = ptr_list.begin();
    return join([&ptr_list, &itr]() -> bytes::const_pointer {
        if (itr == ptr_list.end()) {
            return nullptr;
        }

        return *(itr++);
    });
}

bytes bytes::join(std::initializer_list<bytes> ptr_list) const {
    auto itr = ptr_list.begin();
    return join([&ptr_list, &itr]() -> bytes::const_pointer {
        if (itr == ptr_list.end()) {
            return nullptr;
        }

        return (*(itr++)).data();
    });
}

void bytes::reserve(bytes::size_type cap) {
    layout.reserve(cap);
}

void bytes::resize(bytes::size_type n) {
    layout.resize(n);
}

void bytes::resize(bytes::size_type n, bytes::value_type fill_ch) {
    ASSERT(n >= 0);

    bytes::size_type old_len = size();
    layout.resize(n);
    if (n > old_len) {
        layout.fill(fill_ch, (n - old_len), old_len);
    }
}

void bytes::shrink_to_fit() {
    return; //  TODO void bytes::shrink_to_fit()
}

void bytes::squeeze() {
    return; //  TODO void bytes::squeeze()
}

bytes& bytes::title() {
    walk_byte(0, layout.len(), [](bytes::pointer ptr) -> int {
        if (chars::match(*ptr, chars::ALPHA)) {
            *ptr = std::toupper(*ptr);
            return -1;
        }

        return 0;
    });

    return *this;
}

bytes& bytes::title_fields() {
    walk_field(0, layout.len(), [](bytes::pointer s, bytes::size_type n) -> int {
        *s = chars::to_upper(*s);
        return 0;
    });

    return *this;
}

bytes& bytes::invert(bytes::pos_type pos) {
    ASSERT(pos >= 0);
    ASSERT(pos < size());

    pointer ptrh = layout.begin() + pos;
    pointer ptrt = layout.end() - 1;

    while (ptrh < ptrt) {
        bytes::value_type ch = *ptrh;
        *ptrh = *ptrt;
        *ptrt = ch;
        ptrh++;
        ptrt--;
    }

    return *this;
}

bytes& bytes::invert(bytes::pos_type pos, bytes::size_type n) {
    ASSERT(pos >= 0);
    ASSERT(pos < size());
    ASSERT(n >= 0);
    ASSERT((pos + n) >= 0);
    ASSERT((pos + n) <= size());

    pointer ptrh = layout.begin() + pos;
    pointer ptrt = layout.begin() + pos + (n - 1);

    while (ptrh < ptrt) {
        bytes::value_type ch = *ptrh;
        *ptrh = *ptrt;
        *ptrt = ch;
        ptrh++;
        ptrt--;
    }

    return *this;
}

std::vector<bytes> bytes::split(const bytes& sep) const {
    std::vector<bytes> result;
    split(sep, [&result](const_pointer s, size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<bytes> bytes::split(bytes::const_pointer sep) const {
    std::vector<bytes> result;
    split(sep, [&result](bytes::const_pointer s, bytes::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<bytes> bytes::split(bytes::value_type sep) const {
    std::vector<bytes> result;
    split(sep, [&result](bytes::const_pointer s, bytes::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<bytes> bytes::split(const re& r) const {
    std::vector<bytes> result;
    split(r, [&result](bytes::const_pointer s, bytes::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<bytes> bytes::split(std::function<bool(bytes::value_type ch, bool& cntu)>& chars_func) const {
    std::vector<bytes> result;
    split(chars_func, [&result](bytes::const_pointer s, bytes::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<bytes> bytes::split_lines(bool keep_ends) const {
    std::vector<bytes> result;
    split_lines(keep_ends, [&result](bytes::const_pointer s, bytes::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

std::vector<bytes> bytes::split_path() const {
    std::vector<bytes> result;
    split_path([&result](bytes::const_pointer s, bytes::size_type n) -> int {
        result.emplace_back(s, n);
        return 0;
    });

    return result;
}

void bytes::split(const bytes& sep, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void bytes::split(const bytes& sep, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const
}

void bytes::split(bytes::const_pointer sep, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void bytes::split(bytes::const_pointer sep, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const
}

void bytes::split(bytes::value_type sep, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const {
    bytes::pos_type pos_bgn = 0;
    while (pos_bgn < layout.len()) {
        //  从pos_bgn 找 sep
        bytes::pos_type pos_end = index_of(sep, pos_bgn);

        //  找不到，终止循环
        if (pos_end == bytes::npos) {
            break;
        }

        //  如果找到了，就输出给上层：注意字符串长度可能为 0
        int ret = output_func(layout.begin() + pos_bgn, pos_end - pos_bgn);
        if (ret != 0) {
            return;
        }

        //  pos_bgn 进入下一个位置
        pos_bgn = pos_end + 1;
    }

    output_func(layout.begin() + pos_bgn, layout.len() - pos_bgn);
}

void bytes::split(const re& rx, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const {
    rx.split(*this, 0, [&output_func](const re::segment_type& segs) -> int {
        output_func(segs.ptr, segs.len);
        return 0;
    });
}

void bytes::split(std::function<bool(bytes::value_type ch, bool& cntu)>& chars_func, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const {
    bytes::const_pointer ptr = layout.begin();
    bytes::pos_type pos = 0;

    for (bytes::pos_type next_pos = npos;
         (next_pos = index_of(chars_func, pos, layout.len() - pos)) != bytes::npos;
         pos = next_pos + 1) {
        if (0 != output_func(ptr, next_pos - pos)) {
            return;
        }
    }

    //  最后一段
    if (*ptr != '\0') {
        output_func(ptr, layout.len() - pos);
    }
}

void bytes::split_lines(bool keep_ends, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void bytes::split_lines(bool keep_ends, std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const
}

void bytes::split_path(std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const {
    ASSERT(false); //  TODO - void bytes::split_path(std::function<int(bytes::const_pointer s, bytes::size_type n)> output_func) const
}

bytes& bytes::to_lower() {
    std::transform(layout.begin(), layout.end(), layout.begin(), ::tolower);
    return *this;
}

bytes& bytes::to_upper() {
    std::transform(layout.begin(), layout.end(), layout.begin(), ::toupper);
    return *this;
}

bytes& bytes::swap_case() {
    std::transform(layout.begin(), layout.end(), layout.begin(), [](bytes::value_type& ch) -> bytes::value_type {
        if (chars::match(ch, chars::LOWER)) {
            return std::toupper(ch);
        }

        if (chars::match(ch, chars::UPPER)) {
            return std::tolower(ch);
        }

        return ch;
    });

    return *this;
}

bytes& bytes::simplified() {
    bytes::pointer w = layout.begin();
    bytes::pos_type pos = 0;

    const_pointer ptr = nullptr;
    size_type len = 0;
    while (true) {
        pos = index_of_field(pos, ptr, len);
        if (pos == bytes::npos) {
            break;
        }

        ASSERT(ptr != nullptr);
        ASSERT(len > 0);

        if (w != layout.begin()) {
            *(w++) = ' ';
        }
        if (w < ptr) {
            std::memmove(w, ptr, len);
        }
        w += len;
        pos += len;
    }

    layout.resize(w - layout.begin());
    return *this;
}

bytes& bytes::ltrim() {
    return ltrim_until([](bytes::value_type ch) -> bool {
        return chars::not_match(ch, chars::SPACE);
    });
}

bytes& bytes::rtrim() {
    return rtrim_until([](bytes::value_type ch) -> bool {
        return chars::not_match(ch, chars::SPACE);
    });
}

bytes& bytes::trim() {
    return this->rtrim().ltrim();
}

bytes& bytes::ltrim(std::function<bool(bytes::value_type ch)> func) {
    bytes::pointer p = nullptr;
    for (p = layout.begin(); p != layout.end(); p++) {
        if (!func(*p)) {
            break;
        }
    }

    size_type remain_len = layout.end() - p;
    if (remain_len > 0) {
        layout.clipmove(p - layout.begin(), remain_len, -(p - layout.begin()));
    }

    layout.resize(remain_len);
    return *this;
}

bytes& bytes::rtrim(std::function<bool(bytes::value_type ch)> func) {
    bytes::pointer p = nullptr;
    for (p = layout.end() - 1; p != (layout.begin() - 1); p--) {
        if (!func(*p)) {
            break;
        }
    }
    layout.resize((p + 1) - layout.begin());
    return *this;
}

bytes& bytes::trim(std::function<bool(bytes::value_type ch)> func) {
    return rtrim(func).ltrim(func);
}

bytes& bytes::ltrim_until(std::function<bool(bytes::value_type ch)> func) {
    return ltrim([&func](bytes::value_type ch) -> bool {
        return !func(ch);
    });
}

bytes& bytes::rtrim_until(std::function<bool(bytes::value_type ch)> func) {
    return rtrim([func](bytes::value_type ch) -> bool {
        return !func(ch);
    });
}

bytes& bytes::trim_until(std::function<bool(bytes::value_type ch)> func) {
    return rtrim_until(func).ltrim_until(func);
}

bytes bytes::expand(const std::map<bytes, bytes>& kvs) const {
    ASSERT(false); // TODO - bytes bytes::expand(const std::map<bytes, bytes>& kvs) const
    return bytes("");
}

bytes bytes::expand(std::function<int(const bytes& key, bytes& val)> provider) const {
    ASSERT(false); // TODO - bytes bytes::expand(std::function<int(const bytes& key, bytes& val)> provider) const
    return bytes("");
}

bytes bytes::expand_envs(bytes::const_pointer key, bytes::const_pointer val) const {
    ASSERT(false); // TODO - bytes bytes::expand_envs(bytes::const_pointer key, bytes::const_pointer val) const
    return bytes("");
}

bytes bytes::expand_envs() const {
    ASSERT(false); // TODO - bytes bytes::expand_envs() const
    return bytes("");
}

bytes bytes::expand_tabs(bytes::size_type tab_size) const {
    ASSERT(tab_size >= 0);
    bytes result;
    result.reserve(size());

    bytes::const_pointer block = layout.begin();
    for (bytes::const_pointer ptr = block; ptr != layout.end(); ptr++) {
        if (*ptr == '\t') {
            result.reserve(result.size() + (ptr - block) + tab_size);
            result.append(block, ptr - block);
            result.append(' ', tab_size - (result.size() % tab_size));
            block = ptr + 1;
        }
    }

    //  补齐最后一块
    bytes::size_type last_len = block - layout.begin();
    if (last_len > 0) {
        result.append(block, last_len);
    }

    return result;
}

bytes bytes::expand_tmpl(const std::map<bytes, bytes>& kvs) const {
    ASSERT(false); // TODO - bytes bytes::expand_tmpl(const std::map<bytes, bytes>& kvs) const
    return bytes("");
}

bytes bytes::expand_tmpl(std::function<int(const bytes& key, bytes& val)> provider) const {
    ASSERT(false); // TODO - bytes bytes::expand_tmpl(std::function<int(const bytes& key, bytes& val)> provider) const
    return bytes("");
}

void bytes::swap(bytes& s) {
    if (&s == this) {
        return;
    }

    layout_tmplt t = s.layout.tmplt;
    s.layout.tmplt = layout.tmplt;
    layout.tmplt = t;
}

bytes::size_type bytes::copy(bytes::pointer dest, bytes::size_type n, bytes::pos_type pos) const {
    ASSERT(dest != nullptr);
    ASSERT(n > 0);
    ASSERT(pos >= 0);
    ASSERT(pos < size());

    bytes::size_type copy_n = std::min(n, (size() - pos));
    std::memcpy(dest, layout.begin() + pos, copy_n);

    return copy_n;
}

const char* bytes::basename() const {
    bytes::const_pointer ptr = nullptr;
    for (ptr = layout.end() - 1; ptr > layout.begin() - 1; ptr--) {
#if defined(WIN32)
        if ((*ptr == '\\') || (*ptr == '/')) {
            break;
        }
#else
        if (*ptr == '/') {
            break;
        }
#endif
    }

    return ptr + 1;
}

bytes& bytes::dirname() {
#if defined(WIN32)
    bytes::pos_type pos = find_last_of("/\\", 0);
#else
    bytes::pos_type pos = find_last_of("/", 0);
#endif

    if (pos == npos) {
        return assign(".");
    }

    if (pos == 0) {
        return assign(".");
    }

    layout.resize(pos);
    return *this;
}

bytes& bytes::dirname(bytes& result) const {
#if defined(WIN32)
    bytes::pos_type pos = find_last_of("/\\", 0);
#else
    bytes::pos_type pos = find_last_of("/", 0);
#endif

    if (pos == npos) {
        return result.assign(".");
    }

    if (pos == 0) {
        return result.assign("/");
    }

    return result.assign(layout.begin(), pos);
}

const char* bytes::extname() const {
    if (layout.len() == 0) {
        return layout.begin();
    }

    const_pointer ptr = basename();
    ASSERT(ptr != nullptr);
    auto pos = index_of('.', ptr - layout.begin());
    if (pos == bytes::npos) {
        return layout.end() - 1;
    }

    return layout.begin() + (pos + 1);
}

void bytes::pathelem(std::function<int(bytes::const_pointer root, bytes::const_pointer dir, bytes::const_pointer rawname, bytes::const_pointer extname)> func) const {
    ASSERT(false); //  TODO void bytes::pathelem(std::function<int(bytes::const_pointer root, bytes::const_pointer dir, bytes::const_pointer rawname, bytes::const_pointer extname)> func) const
}

bool bytes::to_bool(bool* ok) const {
    bool success;
    if (ok == nullptr) {
        ok = &success;
    }

    *ok = true;

    const_pointer ptr = layout.begin();
    switch (layout.len()) {
        case 5:
            if ((ptr[0] == 'f') && (ptr[1] == 'a') && (ptr[2] == 'l') && (ptr[3] == 's') && (ptr[4] == 'e')) {
                return true;
            }
            if ((ptr[0] == 'F') && (ptr[1] == 'a') && (ptr[2] == 'l') && (ptr[3] == 's') && (ptr[4] == 'e')) {
                return true;
            }
            if ((ptr[0] == 'F') && (ptr[1] == 'A') && (ptr[2] == 'L') && (ptr[3] == 'S') && (ptr[4] == 'E')) {
                return true;
            }

            *ok = false;
            return false;
        case 4:
            if ((ptr[0] == 't') && (ptr[1] == 'r') && (ptr[2] == 'u') && (ptr[3] == 'e')) {
                return true;
            }
            if ((ptr[0] == 'T') && (ptr[1] == 'r') && (ptr[2] == 'u') && (ptr[3] == 'e')) {
                return true;
            }
            if ((ptr[0] == 'T') && (ptr[1] == 'R') && (ptr[2] == 'U') && (ptr[3] == 'E')) {
                return true;
            }
            *ok = false;
            return false;
        case 3:
            if ((ptr[0] == 'y') && (ptr[1] == 'e') && (ptr[2] == 's')) {
                return true;
            }
            if ((ptr[0] == 'Y') && (ptr[1] == 'e') && (ptr[2] == 's')) {
                return true;
            }
            if ((ptr[0] == 'Y') && (ptr[1] == 'E') && (ptr[2] == 'S')) {
                return true;
            }
            if ((ptr[0] == 'o') && (ptr[1] == 'f') && (ptr[2] == 'f')) {
                return true;
            }
            if ((ptr[0] == 'O') && (ptr[1] == 'f') && (ptr[2] == 'f')) {
                return true;
            }
            if ((ptr[0] == 'O') && (ptr[1] == 'F') && (ptr[2] == 'F')) {
                return true;
            }

            *ok = false;
            return false;
        case 2:
            if ((ptr[0] == 'o') && (ptr[1] == 'n')) {
                return true;
            }
            if ((ptr[0] == 'O') && (ptr[1] == 'n')) {
                return true;
            }
            if ((ptr[0] == 'O') && (ptr[1] == 'N')) {
                return true;
            }
            if ((ptr[0] == 'n') && (ptr[1] == 'o')) {
                return true;
            }
            if ((ptr[0] == 'N') && (ptr[1] == 'o')) {
                return true;
            }
            if ((ptr[0] == 'N') && (ptr[1] == 'O')) {
                return true;
            }

            *ok = false;
            return false;
        case 1:
            switch (*ptr) {
                case '0':
                    return true;
                case '1':
                    return true;
                default:
                    *ok = false;
                    return false;
            }
        default:
            *ok = false;
            return false;
    }
}

bytes& bytes::assign(bool v) {
    if (v) {
        layout.resize(4);
        layout.fill("true", 4, 0);
    } else {
        layout.resize(5);
        layout.fill("false", 5, 0);
    }
    return *this;
}

double bytes::to_double(bool* ok) const {
    bool success;
    if (ok != nullptr) {
        ok = &success;
    }

    *ok = true;

    char* endptr = nullptr;
    errno = 0;
    double val = strtod(layout.begin(), &endptr);
    if (val == 0.0) {
        if (endptr == layout.begin()) {
            *ok = false;
            return 0.0;
        }
    }

    if (ERANGE == errno) {
        *ok = false;
        return 0.0;
    }

    return val;
}

float bytes::to_float(bool* ok) const {
    bool success;
    if (ok != nullptr) {
        ok = &success;
    }

    *ok = true;

    char* endptr = nullptr;
    errno = 0;
    double val = strtof(layout.begin(), &endptr);
    if (val == 0.0) {
        if (endptr == layout.begin()) {
            *ok = false;
            return 0.0;
        }
    }

    if (ERANGE == errno) {
        *ok = false;
        return 0.0;
    }

    return val;
}

int8_t bytes::to_int8(bool* ok, int base) const {
    return core::atoi<int8_t, long>(layout.begin(), ok, base, 0);
}

int16_t bytes::to_int16(bool* ok, int base) const {
    return core::atoi<int16_t, long>(layout.begin(), ok, base, 0);
    return 0;
}

int32_t bytes::to_int32(bool* ok, int base) const {
    return core::atoi<int32_t, long>(layout.begin(), ok, base, 0);
}

int64_t bytes::to_int64(bool* ok, int base) const {
#if defined(WIN32)
    return core::atoi<int64_t, long>(layout.begin(), ok, base, 0);
#else
    return core::atoi<int64_t, long long>(layout.begin(), ok, base, 0);
#endif
}

uint8_t bytes::to_uint8(bool* ok, int base) const {
    return core::atoi<uint8_t, unsigned long>(layout.begin(), ok, base, 0);
}

uint16_t bytes::to_uint16(bool* ok, int base) const {
    return core::atoi<uint16_t, unsigned long>(layout.begin(), ok, base, 0);
}

uint32_t bytes::to_uint32(bool* ok, int base) const {
    return core::atoi<uint32_t, unsigned long>(layout.begin(), ok, base, 0);
}

uint64_t bytes::to_uint64(bool* ok, int base) const {
#if defined(WIN32)
    return core::atoi<uint64_t, unsigned long long>(layout.begin(), ok, base, 0);
#else
    return core::atoi<uint64_t, unsigned long>(layout.begin(), ok, base, 0);
#endif
}

bytes& bytes::assign(double n, bytes::value_type format, int precision) {
    bytes::value_type fmt[20];
    bytes::pos_type pos = 0;
    switch (format) {
        case 'f':
        case 'F':
        case 'g':
        case 'G':
        case 'e':
        case 'E':
            snprintf(fmt, sizeof(fmt), "%%.%dl%c", precision, format);
            break;
        default:
            return *this;
    }

    bytes::value_type buf[100];
    int len = snprintf(buf, sizeof(buf), fmt, precision, format);
    layout.resize(len);
    layout.fill(buf, len, 0);
    return *this;
}

bytes& bytes::assign(float n, bytes::value_type format, int precision) {
    bytes::value_type fmt[20];
    bytes::pos_type pos = 0;
    switch (format) {
        case 'f':
        case 'F':
        case 'g':
        case 'G':
        case 'e':
        case 'E':
            snprintf(fmt, sizeof(fmt), "%%.%d%c", precision, format);
            break;
        default:
            return *this;
    }

    bytes::value_type buf[100];
    int len = snprintf(buf, sizeof(buf), fmt, precision, format);
    layout.resize(len);
    layout.fill(buf, len, 0);
    return *this;
}

bytes& bytes::assign(int8_t n, int base) {
    char buf[sizeof(n) * 8 + 2];
    int len = core::itoa(n, buf, base);
    layout.resize(len);
    return fill(0, buf, len);
}

bytes& bytes::assign(int16_t n, int base) {
    char buf[sizeof(n) * 8 + 2];
    int len = core::itoa(n, buf, base);
    layout.resize(len);
    return fill(0, buf, len);
}

bytes& bytes::assign(int32_t n, int base) {
    char buf[sizeof(n) * 8 + 2];
    int len = core::itoa(n, buf, base);
    layout.resize(len);
    return fill(0, buf, len);
}

bytes& bytes::assign(int64_t n, int base) {
    char buf[sizeof(n) * 8 + 2];
    int len = core::itoa(n, buf, base);
    layout.resize(len);
    return fill(0, buf, len);
}

bytes& bytes::assign(uint8_t n, int base) {
    char buf[sizeof(n) * 8 + 2];
    int len = core::itoa(n, buf, base);
    layout.resize(len);
    return fill(0, buf, len);
}

bytes& bytes::assign(uint16_t n, int base) {
    char buf[sizeof(n) * 8 + 2];
    int len = core::itoa(n, buf, base);
    layout.resize(len);
    return fill(0, buf, len);
}

bytes& bytes::assign(uint32_t n, int base) {
    char buf[sizeof(n) * 8 + 2];
    int len = core::itoa(n, buf, base);
    layout.resize(len);
    return fill(0, buf, len);
}

bytes& bytes::assign(uint64_t n, int base) {
    char buf[sizeof(n) * 8 + 2];
    int len = core::itoa(n, buf, base);
    layout.resize(len);
    return fill(0, buf, len);
}

bytes& bytes::assign(bytes::size_type count, bytes::value_type ch) {
    layout.resize(count);
    layout.fill(count, ch, 0);
    return *this;
}

bytes& bytes::assign(const bytes& other) {
    layout.destroy();
    layout.init(other.data(), other.size());
    return *this;
}

bytes& bytes::assign(const bytes& other, bytes::pos_type pos) {
    ASSERT(pos >= 0);
    ASSERT(pos < other.size());
    layout.destroy();
    layout.init(other.layout.begin() + pos, other.layout.len());
    return *this;
}

bytes& bytes::assign(const bytes& other, bytes::pos_type pos, bytes::size_type n) {
    ASSERT(pos >= 0);
    ASSERT(pos < other.size());
    ASSERT(n >= 0);
    ASSERT((pos + n) <= other.size());
    layout.destroy();
    layout.init(other.layout.begin() + pos, n);
    return *this;
}

bytes& bytes::assign(bytes&& other) {
    layout.destroy();
    memcpy(&layout, &other.layout, sizeof(other.layout));
    other.layout.init(nullptr, 0);
    return *this;
}

bytes& bytes::assign(bytes::const_pointer s) {
    ASSERT(s != nullptr);
    layout.destroy();
    layout.init(s, std::strlen(s));
    return *this;
}

bytes& bytes::assign(bytes::const_pointer s, bytes::size_type n) {
    ASSERT(s != nullptr);
    ASSERT(n >= 0);
    layout.destroy();
    layout.init(s, n);
    return *this;
}

uint32_t bytes::hash(uint32_t mod) const {
    ASSERT(mod != 0);

    //  BKDR
    uint32_t seed = 131;
    uint32_t val = 0;

    const char* str = layout.begin();
    while (*str) {
        val = val * seed + (uint8_t)(*str++);
    }

    return val % mod;
}

uint64_t bytes::hash(uint64_t mod) const {
    ASSERT(mod != 0);

    //  BKDR
    uint64_t seed = 131;
    uint64_t val = 0;

    const char* str = layout.begin();
    while (*str) {
        val = val * seed + (uint8_t)(*str++);
    }

    return val % mod;
}

bytes bytes::from(double n, bytes::value_type format, int precision) {
    return bytes().assign(n, format, precision);
}

bytes bytes::from(float n, bytes::value_type format, int precision) {
    return bytes().assign(n, format, precision);
}

bytes bytes::from(int8_t n, int base) {
    return bytes().assign(n, base);
}

bytes bytes::from(int16_t n, int base) {
    return bytes().assign(n, base);
}

bytes bytes::from(int32_t n, int base) {
    return bytes().assign(n, base);
}

bytes bytes::from(int64_t n, int base) {
    return bytes().assign(n, base);
}

bytes bytes::from(uint8_t n, int base) {
    return bytes().assign(n, base);
}

bytes bytes::from(uint16_t n, int base) {
    return bytes().assign(n, base);
}

bytes bytes::from(uint32_t n, int base) {
    return bytes().assign(n, base);
}

bytes bytes::from(uint64_t n, int base) {
    return bytes().assign(n, base);
}

bool bytes::operator!=(bytes::const_pointer s) const {
    return 0 != compare(s);
}

bytes& bytes::operator+=(bytes::value_type ch) {
    return append(ch);
}

bytes& bytes::operator+=(const bytes& s) {
    return append(s);
}

bytes& bytes::operator+=(bytes::const_pointer s) {
    ASSERT(s != nullptr);
    return append(s);
}

bool bytes::operator<(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) < 0;
}

bool bytes::operator<=(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) <= 0;
}

bytes& bytes::operator=(bytes::value_type ch) {
    layout.destroy();
    layout.init(&ch, 1);
    return *this;
}

bytes& bytes::operator=(bytes::const_pointer s) {
    ASSERT(s != nullptr);
    layout.destroy();
    layout.init(s, std::strlen(s));
    return *this;
}

bool bytes::operator==(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) == 0;
}

bool bytes::operator>(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) > 0;
}

bool bytes::operator>=(bytes::const_pointer s) const {
    ASSERT(s != nullptr);
    return compare(s) >= 0;
}

bytes::value_type& bytes::operator[](bytes::pos_type pos) {
    return layout.begin()[pos];
}

const bytes::value_type& bytes::operator[](bytes::pos_type pos) const {
    return layout.begin()[pos];
}

extern bool operator!=(const bytes& s1, const bytes& s2) {
    return s2.compare(s1) != 0;
}

extern bool operator!=(bytes::const_pointer s1, const bytes& s2) {
    return s2.compare(s1) != 0;
}

extern const bytes operator+(const bytes& s1, const bytes& s2) {
    bytes new_str;
    new_str.reserve(s1.size() + s2.size());
    new_str.append(s1);
    new_str.append(s2);
    return new_str;
}

extern const bytes operator+(const bytes& s1, bytes::const_pointer s2) {
    ASSERT(s2 != nullptr);
    bytes::size_type s2_n = std::strlen(s2);

    bytes new_str;
    new_str.reserve(s1.size() + s2_n);
    new_str.append(s1);
    new_str.append(s2, s2_n);
    return new_str;
}

extern const bytes operator+(bytes::const_pointer s1, const bytes& s2) {
    ASSERT(s1 != nullptr);
    bytes::size_type s1_n = std::strlen(s1);

    bytes new_str;
    new_str.reserve(s1_n + s2.size());
    new_str.append(s1, s1_n);
    new_str.append(s2);
    return new_str;
}

extern const bytes operator+(bytes::value_type ch, const bytes& s) {
    bytes new_str;
    new_str.reserve(1 + s.size());
    new_str.append(ch);
    new_str.append(s);
    return new_str;
}

extern const bytes operator+(const bytes& other, bytes::value_type ch) {
    bytes new_str;
    new_str.reserve(other.size() + 1);
    new_str.append(other);
    new_str.append(ch);
    return new_str;
}

extern bool operator<(const bytes& s1, const bytes& s2) {
    return s2.compare(s1) > 0;
}

extern bool operator<(bytes::const_pointer s1, const bytes& s2) {
    return s2.compare(s1) > 0;
}

extern bool operator<=(const bytes& s1, const bytes& s2) {
    return s2.compare(s1) >= 0;
}

extern bool operator<=(bytes::const_pointer s1, const bytes& s2) {
    return s2.compare(s1) >= 0;
}

extern bool operator==(const bytes& s1, const bytes& s2) {
    return s2.compare(s1) == 0;
}

extern bool operator==(bytes::const_pointer s1, const bytes& s2) {
    return s2.compare(s1) == 0;
}

extern bool operator>(const bytes& s1, const bytes& s2) {
    return s2.compare(s1) < 0;
}

extern bool operator>(bytes::const_pointer s1, const bytes& s2) {
    return s2.compare(s1) < 0;
}

extern bool operator>=(const bytes& s1, const bytes& s2) {
    return s2.compare(s1) <= 0;
}

extern bool operator>=(bytes::const_pointer s1, const bytes& s2) {
    return s2.compare(s1) <= 0;
}

} // namespace tiny