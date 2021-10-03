#ifndef STR_ASSERT_H
#define STR_ASSERT_H

#if defined(NDEBUG)
#define ASSERT(x)
#else
#ifndef ASSERT
#define ASSERT(x) assert(x)
#endif
#endif

#endif //STR_ASSERT_H