#ifndef LOGDOCTOR__UTILITIES__MEMORY_H
#define LOGDOCTOR__UTILITIES__MEMORY_H


#include <cstddef>


//! MemOps
/*!
    Utilities for system's memory infos
*/
namespace MemOps
{

//! Returns the available (free) memory in the system
/*!
    \return The amount of memory in Bytes
    \throw DoNotCatchException
*/
std::size_t availableMemory();

} // namespace MemOps


#endif // LOGDOCTOR__UTILITIES__MEMORY_H
