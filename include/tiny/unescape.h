//
// Created by luolijun on 2021/10/1.
//

#ifndef UNESCAPE_H
#define UNESCAPE_H

class unescape {
public:
    static str html(const str& s);

    static str uri(const str& s);

    static str& cstr(const str& s);

    static str& sql(const str& s);
}
#endif //UNESCAPE_H
