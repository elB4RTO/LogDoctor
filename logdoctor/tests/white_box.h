#ifndef LOGDOCTESTER__WHITE_BOX_H
#define LOGDOCTESTER__WHITE_BOX_H


//! Testing
/*!
    Collection of whitebox tests
*/
namespace Testing
{

//! A Test suit for the defined operators
bool testOperators();

//! A Test suit for the utilities
bool testUtilities();

//! A Test suit for the workarounds
bool testWorkarounds();

//! A Test suit for craplog's modules
bool testCraplogModules();

//! A Test suit for crapview's modules
bool testCrapviewModules();

} // namespace Testing

#endif // LOGDOCTESTER__WHITE_BOX_H
