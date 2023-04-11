#ifndef VECOPS_H
#define VECOPS_H

#include <vector>
#include <algorithm>


//! VecOps
/*!
    Utilities for the vectors
*/
namespace VecOps
{

//! Checks if a vector contains an element
/*!
    \param list The target vector
    \param flag The element to search for
    \return Whether the list does contain the flag or not
*/
template <typename T>
inline const bool contains( const std::vector<T>& list, const T& flag )
{
    return std::any_of( list.cbegin(), list.cend(),
                        [&flag]( const T& item )
                               { return item == flag; } );
}

}

#endif // VECOPS_H
