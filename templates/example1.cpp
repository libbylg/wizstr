/**
 * Copyright (c) 2021-2025  libbylg@126.com
 * str is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */

/// @block readme_example
#include <iostream>

#include "str.hpp"

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

int main() {
    // split&join
    auto items = str::split("Welcome to use str library");
    std::cout << "items: [" << str::join_list(items) << "]" << std::endl;

    // path
    auto dirname = str::dirname("/home/sam/project/main.cpp");
    auto basename = str::basename("/home/sam/project/main.cpp");
    auto fullpath = str::join_path({dirname, basename});
    std::cout << "dirname: " << dirname << std::endl;
    std::cout << "basename: " << basename << std::endl;
    std::cout << "fullpath: " << fullpath << std::endl;

    // trim
    std::string_view s = "  Hello  World  !  ";
    std::cout << "trim_left: [" << str::trim_left(s) << "]" << std::endl;
    std::cout << "trim_right: [" << str::trim_right(s) << "]" << std::endl;
    std::cout << "trim_surrounding: [" << str::trim_surrounding(s) << "]" << std::endl;
    std::cout << "simplified: [" << str::simplified(s) << "]" << std::endl;

    // dump_hex
    std::string k = "This function is used to dump binary data to hex string like xxd";
    str::dump_hex(k.data(), k.size(), str::dump_hex_format{.flags = (str::show_ascii | str::show_offset)},
        [](size_t, std::string_view line_text) {
            std::cout << line_text << std::endl;
            return 0;
        });

    return 0;
}
/// @end readme_example
