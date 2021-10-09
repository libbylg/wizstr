//
// Created by luolijun on 2021/10/9.
//

#ifndef SEGMENT_H
#define SEGMENT_H

namespace tiny {

struct segment_t {
    char* ptr{ nullptr };
    int32_t pos{ -1 };
    uint32_t len{ 0 };
};

} // namespace tiny

#endif // SEGMENT_H
