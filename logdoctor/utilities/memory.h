#ifndef LOGDOCTOR__UTILITIES__MEMORY_H
#define LOGDOCTOR__UTILITIES__MEMORY_H


//! MemOps
/*!
    Utilities for system's memory infos
*/
namespace MemOps
{

namespace{ using size_t = unsigned long; }

//! Returns the available (free) memory in the system
/*!
    \return The amount of memory in Bytes
*/
const size_t availableMemory();

} // namespace MemOps


#endif // LOGDOCTOR__UTILITIES__MEMORY_H
