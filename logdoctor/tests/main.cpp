
#include "white_box.h"

#include <iostream>


int main()
{
    std::cout << "LOGDOCTOR TESTER" << std::endl;
    bool passed{ true };

    passed &= Testing::testOperators();

    passed &= Testing::testUtilities();

    passed &= Testing::testCraplogModules();

    passed &= Testing::testCrapviewModules();

    if (passed)
        std::cout << "\n\nALL TESTS PASSED" << std::endl;
    else
        std::cout << "\n\nSOME TEST FAILED" << std::endl;

    return passed;
}
