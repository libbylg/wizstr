#ifndef TESTING_H
#define TESTING_H

#include <cassert>
#include <string_view>
#include <cstdint>
#include <string>
#include <vector>
#include <cstdarg>
#include <cstdio>
#include <csetjmp>

template<typename T, typename Y>
auto operator==(const std::vector<T> &a, const std::vector<Y> &b) -> bool {
    if (a.size() != b.size()) {
        return false;
    }

    for (size_t index = 0; index < a.size(); index++) {
        if (a[index] != b[index]) {
            return false;
        }
    }

    return true;
}

//! 测试用例头
struct testcase_head {
    testcase_head *next;
    testcase_head *prev;

    testcase_head() : next{this}, prev{this} {
    }

    static inline void insert(testcase_head *item, testcase_head *prev, testcase_head *next) {
        item->prev = prev;
        item->next = next;
        prev->next = item;
        next->prev = item;
    }
};

//! 测试用例
struct testcase : public testcase_head {
    using testproc = void (*)(testcase *tcase, std::jmp_buf tenv);
    testproc proc{nullptr};
    const char *nsuite;
    const char *ncase;

    // 用例定义在哪个文件的那一行
    const char *file;
    size_t lineno;

    enum testcase_flags : uint8_t {
        flag_pass = 0x01,
        flag_abort = 0x02,
    };

    //! 执行所有的测试用例
    static inline int exec_all(int argc, char *argv[]) {
        size_t count_total = 0;
        size_t count_pass = 0;
        size_t count_fail = 0;
        for (testcase_head *ptr = all_testcases.next; ptr != &all_testcases; ptr = ptr->next) {
            count_total++;
            bool abort = false;
            if (!(reinterpret_cast<testcase *>(ptr))->exec(abort)) {
                count_fail++;
                if (abort) {
                    break;
                }
                continue;
            }

            count_pass++;
        }

        static const char *tag_pass_enter = "\033[1;32m";
        static const char *tag_pass_leave = "\033[0m";
        static const char *tag_fail_enter = "\033[1;31m";
        static const char *tag_fail_leave = "\033[0m";

        std::printf("----------------------------\n");
        if (count_fail == 0) {
            std::printf("%sTotal : %lu%s\n", tag_pass_enter, count_total, tag_pass_leave);
            std::printf("%sPass  : %lu%s\n", tag_pass_enter, count_pass, tag_pass_leave);
            std::printf("%sFail  : %lu%s\n", tag_pass_enter, count_fail, tag_pass_leave);
            return 0;
        } else {
            std::printf("%sTotal : %lu%s\n", tag_fail_enter, count_total, tag_fail_leave);
            std::printf("%sPass  : %lu%s\n", tag_fail_enter, count_pass, tag_fail_leave);
            std::printf("%sFail  : %lu%s\n", tag_fail_enter, count_fail, tag_fail_leave);
            return 1;
        }
    }

    explicit testcase(testproc proc, const char *nsuite, const char *ncase, const char *file, size_t lineno)
        : proc{proc}, nsuite{nsuite}, ncase{ncase}, file{file}, lineno{lineno} {
        insert(this, all_testcases.prev, &all_testcases);
    }

    inline void make_assert(bool pass, bool abort, std::jmp_buf env, const char *f, size_t n, const char *format, ...) {
        if (pass) {
            return;
        }

        std::printf("%s:%lu\n", f, n);

        va_list valist;
        va_start(valist, format);
        std::vprintf(format, valist);
        va_end(valist);

        std::printf("\n");

        if (abort) {
            std::longjmp(env, abort);
        }
    }

    //! 执行一个测试用例
    bool exec(bool &abort) {
        static const char *tag_blue_enter = "\033[1;33m";
        static const char *tag_blue_leave = "\033[0m";
        static const char *tag_pass_enter = "\033[1;32m";
        static const char *tag_pass_leave = "\033[0m";
        static const char *tag_fail_enter = "\033[1;31m";
        static const char *tag_fail_leave = "\033[0m";

        std::printf("%s[%s - %s] %s%s\n", tag_blue_enter, nsuite, ncase, "....", tag_blue_leave);

        // 设置回调点
        std::jmp_buf assert_env;
        if (setjmp(assert_env)) {
            std::printf("%s[%s - %s] %s%s\n", tag_fail_enter, nsuite, ncase, "fail", tag_fail_leave);
            abort = true;
            return false;
        }

        proc(this, assert_env);

        std::printf("%s[%s - %s] %s%s\n", tag_pass_enter, nsuite, ncase, "pass", tag_pass_leave);
        return true;
    }

    //! 所有测试用例表
    static inline testcase_head all_testcases{};
};

//! 定义一个测试用例的宏
#define TEST(SuiteName_, CaseName_) \
    static void test_##SuiteName_##CaseName_(testcase* tcase, std::jmp_buf tenv); \
    static testcase testcase_##SuiteName_##CaseName(&(test_##SuiteName_##CaseName_), #SuiteName_, #CaseName_, __FILE__, __LINE__); \
    static void test_##SuiteName_##CaseName_(testcase* TESTCASE_, std::jmp_buf TEST_ASSERT_ENV_) \
    /* (end) */

//! 测试用例的断言宏
#define TEST_ASSERT(Expr_) ((TESTCASE_)->make_assert((Expr_), true, (TEST_ASSERT_ENV_), __FILE__, __LINE__, #Expr_))
#define TEST_EXPECT(Expr_) ((TESTCASE_)->make_assert((Expr_), true, (TEST_ASSERT_ENV_), __FILE__, __LINE__, #Expr_))
//
#define EXPECT_EQ(Expr1_, Expr2_) (TEST_EXPECT(((Expr1_) == (Expr2_))))
#define ASSERT_EQ(Expr1_, Expr2_) (TEST_ASSERT(((Expr1_) == (Expr2_))))
#define EXPECT_TRUE(Expr1_) (TEST_EXPECT((Expr1_)))
#define ASSERT_TRUE(Expr1_) (TEST_ASSERT((Expr1_)))
#define EXPECT_FALSE(Expr1_) (TEST_EXPECT(!(Expr1_)))
#define ASSERT_FALSE(Expr1_) (TEST_ASSERT(!(Expr1_)))

//! 用例分段宏
#define SECTION(Desc_) if (true)

#endif//TESTING_H
