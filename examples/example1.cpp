#include <iostream>

#include "str.h"

int main() {
    // split
    auto items = str::split("Welcome to use str library");
    std::cout << "items: [";
    for (auto& item : items) {
        std::cout << item << ",";
    }
    std::cout << "]" << std::endl;

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

    return 0;
}
