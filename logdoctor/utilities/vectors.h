#ifndef VECOPS_H
#define VECOPS_H

#include <vector>


//! VecOps
/*!
    Utilities for the vectors
*/
template <typename T>
class VecOps
{
public:
    //! Checks if a vector contains an element
    /*!
        \param list The target vector
        \param flag The element to search for
        \return Whether the list does contain the flag or not
    */
    static inline const bool contains( const std::vector<T>& list, const T& flag )
    {
        return std::any_of(
            list.cbegin(), list.cend(),
            [&flag]( const T& item )
                   { return item == flag; } );
    }
};

#endif // VECOPS_H
