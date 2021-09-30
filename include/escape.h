//
// Created by luolijun on 2021/9/30.
//

#ifndef ESCAPE_H
#define ESCAPE_H

class escape {
public:
    static str html(const str& s);

    static str uri(const str& s);

    static str& cstr(const str& s);

    static str& sql(const str& s);
};

#endif //ESCAPE_H
