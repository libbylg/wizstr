/**
* Copyright (c) 2021-2021 libbylg@126.com
* tiny is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
* See the Mulan PSL v2 for more details.
*/
#ifndef STR_ASSERT_H
#define STR_ASSERT_H

#include <cassert>

#if defined(NDEBUG)
#define ASSERT(x)
#else
#ifndef ASSERT
#define ASSERT(x) assert(x)
#endif
#endif

#endif // STR_ASSERT_H