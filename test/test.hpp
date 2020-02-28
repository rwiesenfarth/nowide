//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//  Copyright (c) 2019 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef NOWIDE_LIB_TEST_H_INCLUDED
#define NOWIDE_LIB_TEST_H_INCLUDED

#include <cstdlib>
#include <sstream>
#include <stdexcept>

#if defined(_MSC_VER) && defined(_CPPLIB_VER) && defined(_DEBUG)
#include <crtdbg.h>
#endif


namespace nowide {
    struct test_monitor
    {
        test_monitor()
        {
#if defined(_MSC_VER) && (_MSC_VER > 1310)
            // disable message boxes on assert(), abort()
            ::_set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#endif
#if defined(_MSC_VER) && defined(_CPPLIB_VER) && defined(_DEBUG)
            // disable message boxes on iterator debugging violations
            _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
            _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif
        }
    };
} // namespace nowide


inline nowide::test_monitor& test_mon()
{
    static nowide::test_monitor instance;
    return instance;
}

/// Function called when a test failed to be able set a breakpoint for debugging
inline void test_failed(const std::string& msg)
{
    throw std::runtime_error(msg);
}

#ifdef _MSC_VER
#define DISABLE_CONST_EXPR_DETECTED __pragma(warning(push)) __pragma(warning(disable : 4127))
#define DISABLE_CONST_EXPR_DETECTED_POP __pragma(warning(pop))
#else
#define DISABLE_CONST_EXPR_DETECTED
#define DISABLE_CONST_EXPR_DETECTED_POP
#endif

#define TEST(x)                                                                         \
    do                                                                                  \
    {                                                                                   \
        test_mon();                                                                     \
        if(x)                                                                           \
            break;                                                                      \
        std::ostringstream ss;                                                          \
        ss << "Error " #x " in " << __FILE__ << ':' << __LINE__ << " " << __FUNCTION__; \
        test_failed(ss.str());                                                          \
        DISABLE_CONST_EXPR_DETECTED                                                     \
    } while(0) DISABLE_CONST_EXPR_DETECTED_POP

#endif // #ifndef NOWIDE_LIB_TEST_H_INCLUDED
