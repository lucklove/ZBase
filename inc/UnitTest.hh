#pragma once
#include <functional>
#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <csignal>

struct BaseSuite
{
    virtual void run() = 0;
    virtual ~BaseSuite() = default;
};

struct CheckFailed : std::exception {};

class UnitTest
{
private:
    UnitTest() : last_checked_line{0}, error_num{0} {}
    std::vector<BaseSuite*> test_suites_;

public:
    std::string last_checked_file;
    size_t last_checked_line;
    size_t error_num;

    static UnitTest& getInstance()
    {
        static UnitTest instance;
        return instance;
    }

    void runAll()
    {
        std::cout << "running " << test_suites_.size() << " tests..." << std::endl;
        for(BaseSuite *test : test_suites_)
            test->run();
        std::cout << error_num << " errors detected." << std::endl;
    }

    void registerTestSuite(BaseSuite *test)
    {
        test_suites_.push_back(test);
    }
};

struct TestSuite : BaseSuite
{
public:
    TestSuite(std::function<void()> method, const std::string& name, const std::string& file, size_t line) 
        : method_{method}, case_name_{name}, defined_file_{file}, defined_line_{line}
    {
        UnitTest::getInstance().registerTestSuite(this);
    }
   
    void run() override
    {
        try
        {
            UnitTest::getInstance().last_checked_file = defined_file_;
            UnitTest::getInstance().last_checked_line = defined_line_;
            method_(); 
        }
        catch(CheckFailed&)
        {
            /** do nothing */
        }
        catch(std::exception& e)
        {
            ++UnitTest::getInstance().error_num;
            std::cout << "unknown location(0): fatal error: in \"" << case_name_ << "\": "
                << typeid(e).name() << ": " << e.what() << std::endl;
            std::cout << UnitTest::getInstance().last_checked_file << "("
                << UnitTest::getInstance().last_checked_line << ")" << ": last checkpoint" << std::endl;
        }
        catch(...)
        {
            ++UnitTest::getInstance().error_num;
            std::cout << "unknown location(0): fatal error: in \"" << case_name_ << "\": unknown type" << std::endl;
            std::cout << UnitTest::getInstance().last_checked_file << "("
                << UnitTest::getInstance().last_checked_line << ")" << ": last checkpoint" << std::endl;
        }
    }

    ~TestSuite() override = default;
    
private:
    std::function<void()> method_;
    std::string case_name_;
    std::string defined_file_;
    size_t defined_line_; 
};


#ifdef TEST_MAIN
int main()
{
    UnitTest::getInstance().runAll();    
    return UnitTest::getInstance().error_num;    
}
#endif

#define TEST_CASE(test_name)                                                                    \
void test_name();                                                                               \
TestSuite test_name##_suite{test_name, #test_name, __FILE__, __LINE__};                         \
void test_name()

#define G_CHECK(cond, strict)                                                                   \
do {                                                                                            \
    UnitTest::getInstance().last_checked_file = __FILE__;                                       \
    UnitTest::getInstance().last_checked_line = __LINE__;                                       \
    if(!(cond)) {                                                                               \
        ++UnitTest::getInstance().error_num;                                                    \
        if(strict) {                                                                            \
            std::cout << "critical error at " __FILE__ "(" << __LINE__ << ")." << std::endl;    \
            std::cout << "check \"" << #cond << "\" failed." << std::endl;                      \
            throw CheckFailed{};                                                                \
        } else {                                                                                \
            std::cout << "check \"" << #cond << "\" failed " << "at "                           \
                << __FILE__ << "(" << __LINE__ << ")" << std::endl;                             \
        }                                                                                       \
    }                                                                                           \
} while(0)

#define TEST_CHECK(cond)                                                                        \
G_CHECK(cond, false)

#define TEST_REQUIRE(cond)                                                                      \
G_CHECK(cond, true)
            
