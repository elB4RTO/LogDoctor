#ifndef LOGDOCTESTER__LIB_H
#define LOGDOCTESTER__LIB_H

#include <iostream>
#include <sstream>

#include <QtSystemDetection>


#if defined( Q_OS_MACOS )
#define C_DEFAULT ""
#define C_TESTING ""
#define C_FAILED  ""
#define C_PASSED  ""
#define C_WARN    ""
#define C_SUITE   ""
#elif defined( Q_OS_WINDOWS )
#define C_DEFAULT ""
#define C_TESTING ""
#define C_FAILED  ""
#define C_PASSED  ""
#define C_WARN    ""
#define C_SUITE   ""
#elif defined( Q_OS_LINUX ) || defined( Q_OS_BSD4 )
#define C_DEFAULT "\x1b[0m"
#define C_TESTING "\x1b[1;30m"
#define C_FAILED  "\x1b[1;31m"
#define C_PASSED  "\x1b[1;32m"
#define C_WARN    "\x1b[1;33m"
#define C_SUITE   "\x1b[1;34m"
#else
#error "System not supported"
#endif


#define T_INIT(ARG)\
    unsigned n_tested{0}, n_passed{0}, n_failed{0};\
    bool successful;\
    std::cout << "\n\n"\
              << "┏(" C_SUITE "TEST_SUITE" C_DEFAULT ") " ARG << std::endl;

#define T_TEST_START(ARG)\
    ++n_tested;\
    successful = true;\
    std::cout << "┃\n"\
              << "┠┄┬[" C_TESTING "TESTING" C_DEFAULT "] " ARG "()" << std::endl;

#define T_TEST_RESULT()\
    std::cout << "┃ │" << std::endl;\
    if (successful) {\
        ++n_passed;\
        std::cout << "┃ └┄─[" C_PASSED "PASSED" C_DEFAULT "]" << std::endl;\
    } else {\
        ++n_failed;\
        std::cout << "┃ └┄─[" C_FAILED "FAILED" C_DEFAULT "]" << std::endl;\
    }

#define T_FIN()\
    std::cout << "┃\n"\
              << "┗━┳━━━━━━━━┯━━━━━━━━┯━━━━━━━━┓\n"\
              << "  ┃ TESTED ┆ PASSED ┆ FAILED ┃\n"\
              << "  ┃ " << std::setw(6) << n_tested << " ┆ " << std::setw(6) << n_passed << " ┆ " << std::setw(6) << n_failed << " ┃\n"\
              << "  ┗━━━━━━━━┷━━━━━━━━┷━━━━━━━━┛"\
              << std::endl;\
    return n_failed == 0 && n_passed == n_tested;

#define T_ASSERT(ARG)\
    if (!(ARG)) {\
        std::cout << "┃ │\n"\
                  << "┃ ├─┄{" C_WARN "ASSERTION_FAILED" C_DEFAULT "}@" C_WARN << __LINE__ << C_DEFAULT " " #ARG << std::endl;\
        successful = false;\
    }

#define T_ASSERT_NOT(ARG)\
    if (ARG) {\
        std::cout << "┃ │\n"\
                  << "┃ ├─┄{" C_WARN "ASSERTION_FAILED" C_DEFAULT "}@" C_WARN << __LINE__ << C_DEFAULT " ! " #ARG << std::endl;\
        successful = false;\
    }

#define T_EXPECT(TARGET,EXPECTED)\
    if (TARGET != EXPECTED) {\
        std::cout << "┃ │\n"\
                  << "┃ ├─┄{" C_WARN "EXPECTATION_FAILED" C_DEFAULT "}@" C_WARN << __LINE__ << C_DEFAULT " " #TARGET << " == " << #EXPECTED << std::endl;\
        successful = false;\
    }

#define T_EXPECT_NOT(TARGET,EXPECTED)\
    if (TARGET == EXPECTED) {\
        std::cout << "┃ │\n"\
                  << "┃ ├─┄{" C_WARN "EXPECTATION_FAILED" C_DEFAULT "}@" C_WARN << __LINE__ << C_DEFAULT " " #TARGET << " != " << #EXPECTED << std::endl;\
        successful = false;\
    }


#define REDIRECT_STDOUT()\
    std::streambuf* out{ std::cout.rdbuf() };\
    std::stringstream out_;\
    std::cout.rdbuf( out_.rdbuf() );

#define RESTORE_STDOUT()\
    std::cout.rdbuf( out );

#define REDIRECT_STDERR()\
    std::streambuf* err{ std::cerr.rdbuf() };\
    std::stringstream err_;\
    std::cerr.rdbuf( err_.rdbuf() );

#define RESTORE_STDERR()\
    std::cerr.rdbuf( err );


#endif // LOGDOCTESTER__LIB_H
