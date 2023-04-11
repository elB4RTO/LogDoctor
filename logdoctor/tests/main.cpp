
#include "white_box.h"

#include <iostream>


int main()
{
    std::cout << "STARTING TEST SUITE" << std::endl;

    std::cout << "\nTESTING: utilities" << std::endl;
    Testing::testUtilities();

    std::cout << "\nTESTING: craplog modules" << std::endl;
    Testing::testCraplogModules();

    std::cout << "\nALL TESTS PASSED" << std::endl;
    return 0;
}
