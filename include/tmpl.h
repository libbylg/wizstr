//
// Created by luolijun on 2021/10/1.
//

#ifndef TMPL_H
#define TMPL_H

#include "str.h"

#include <functional>
#include <map>

class tmpl {
public:
    //  基于模板进行渲染
    static str render(const str& s, const std::map<str, str>& kvs);
    static str render(const str& s, std::function<int(const str& key, str& val)> provider);

    //  环境变量展开
    static str expand(const str& s, const std::map<str, str>& kvs);
    static str expand(const str& s, std::function<int(const str& key, str& val)> provider);

};

#endif //TMPL_H
