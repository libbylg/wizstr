#ifndef TESTING_H
#define TESTING_H

#include <cassert>
#include <csetjmp>
#include <cstdarg>
#include <cstdint>
#include <cstdio>

namespace testing {
//! 几个终端颜色助记符
static constexpr auto tag_blue_enter = "\033[1;33m";
static constexpr auto tag_blue_leave = "\033[0m";
static constexpr auto tag_pass_enter = "\033[1;32m";
static constexpr auto tag_pass_leave = "\033[0m";
static constexpr auto tag_fail_enter = "\033[1;31m";
static constexpr auto tag_fail_leave = "\033[0m";

//! 测试用例头
struct testcase_head {
    testcase_head* next;
    testcase_head* prev;

    //! 测试用例头
    explicit testcase_head()
        : next{this}
        , prev{this} {
    }

    //! 向用例尾部插入元素
    static inline void insert(testcase_head* item, testcase_head* prev, testcase_head* next) {
        item->prev = prev;
        item->next = next;
        prev->next = item;
        next->prev = item;
    }
};

//! 测试用例
struct testcase : public testcase_head {
    // 测试用例函数类型定义
    using testproc = void (*)();

    // 测试用例函数地址
    testproc proc;

    // 测试套名，测试用例名
    const char* nsuite;
    const char* ncase;

    // 用例定义在哪个文件
    const char* file;
    size_t lineno;

    //! 所有测试用例表
    static inline testcase_head all_testcases{};
    static inline testcase* curr_testcase{nullptr};
    static jmp_buf curr_assertion_env;

    //! 测试用例构造函数
    explicit testcase(testproc proc, const char* nsuite, const char* ncase, const char* file, size_t lineno)
        : proc{proc}
        , nsuite{nsuite}
        , ncase{ncase}
        , file{file}
        , lineno{lineno} {
        insert(this, all_testcases.prev, &all_testcases);
    }

    //! 执行一个测试用例
    static inline bool exec(testcase* tcase, jmp_buf& tenv, bool& abort) {
        // 每个用例执行前打印下当前正在执行那个任务
        std::printf("%s[%s - %s] %s%s\n", tag_blue_enter, tcase->nsuite, tcase->ncase, "....", tag_blue_leave);

        // 设置 longjmp 的会跳点，以便于断言宏在必要时中断用例执行
        if (setjmp(tenv)) {
            std::printf("%s[%s - %s] %s%s\n", tag_fail_enter, tcase->nsuite, tcase->ncase, "fail", tag_fail_leave);
            abort = true;
            return false;
        }

        // 执行用例
        tcase->proc();

        // 如果执行成功，proc 函数会自动返回
        std::printf("%s[%s - %s] %s%s\n", tag_pass_enter, tcase->nsuite, tcase->ncase, "pass", tag_pass_leave);
        return true;
    }

    //! 断言处理函数
    static inline void make_assert(bool pass, bool abort, const char* file, size_t lineno, const char* format, ...) {
        // 如果执行未触发断言，不需要做什么
        if (pass) {
            return;
        }

        // 如果断言的条件无法满足，输出行号（这里的格式大部分 IDE 能自动解析）
        std::printf("%s:%lu\n", file, lineno);

        // 输出断言信息（这个实现得并不好，但是对于大多数项目来说够用了）
        va_list valist;
        va_start(valist, format);
        std::vprintf(format, valist);
        va_end(valist);

        // 补一个换行
        std::printf("\n");

        // 如果指定了立即结束标记，那么通过 longjmp 回到 setjmp 的位置
        if (abort) {
            std::longjmp(curr_assertion_env, abort);
        }
    }

    //! 执行所有的测试用例
    static inline int exec_all(int argc, char* argv[]) {
        // 记录用例数量
        size_t count_total = 0;
        size_t count_pass = 0;
        size_t count_fail = 0;

        // 按照注册顺序遍历用例
        for (testcase_head* ptr = testcase::all_testcases.next; ptr != &testcase::all_testcases; ptr = ptr->next) {
            count_total++;
            bool abort = false;

            // 如果用例执行失败，需要特殊处理
            curr_testcase = reinterpret_cast<testcase*>(ptr);
            if (!exec(curr_testcase, curr_assertion_env, abort)) {

                // 递增失败用例数量
                count_fail++;

                // 如果指定了退出标记，那么立即退出
                if (abort) {
                    break;
                }

                // 如果不需要退出，那么继续执行下一个用例
                continue;
            }

            // 如果执行成功就继续执行下一个用例
            count_pass++;
        }

        // 输出用例总结报告
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
};

} // namespace testing

//! 测试用例类
#ifndef TESTING_TESTCACSE_CLASS
#define TESTING_TESTCACSE_CLASS testing::testcase
#endif // TESTING_TESTCACSE_CLASS

//! 生成断言
#ifndef TESTING_MAKE_ASSERT
#define TESTING_MAKE_ASSERT TESTING_TESTCACSE_CLASS::make_assert
#endif // TESTING_MAKE_ASSERT

//! 定义一个测试用例的宏（很关键，有点啰嗦有点长）
#ifndef TEST
#define TEST(SuiteName_, CaseName_)                                                                                      \
    static void testing_##SuiteName_##__##CaseName_();                                                                   \
    static TESTING_TESTCACSE_CLASS testing_register__##SuiteName_##__##CaseName_(&(testing_##SuiteName_##__##CaseName_), \
        (#SuiteName_), (#CaseName_), (__FILE__), (__LINE__));                                                            \
    static void testing_##SuiteName_##__##CaseName_()
#endif // TEST

//! 测试用例的断言宏（基础断言宏）
#ifndef TESTING_ASSERT
#define TESTING_ASSERT(Expr_) (TESTING_MAKE_ASSERT((Expr_), true, __FILE__, __LINE__, #Expr_))
#endif // TESTING_ASSERT
#ifndef TESTING_EXPECT
#define TESTING_EXPECT(Expr_) (TESTING_MAKE_ASSERT((Expr_), false, __FILE__, __LINE__, #Expr_))
#endif // TESTING_EXPECT

//! 扩展断言宏（模仿 gtest）
#define EXPECT_EQ(Expr1_, Expr2_) (TESTING_EXPECT(((Expr1_) == (Expr2_))))
#define ASSERT_EQ(Expr1_, Expr2_) (TESTING_ASSERT(((Expr1_) == (Expr2_))))
#define EXPECT_TRUE(Expr1_) (TESTING_EXPECT(!!(Expr1_)))
#define ASSERT_TRUE(Expr1_) (TESTING_ASSERT(!!(Expr1_)))
#define EXPECT_FALSE(Expr1_) (TESTING_EXPECT(!(Expr1_)))
#define ASSERT_FALSE(Expr1_) (TESTING_ASSERT(!(Expr1_)))

//! 用例分段宏（模仿 catch2）
#define SECTION(Desc_) if (true)

#endif // TESTING_H
