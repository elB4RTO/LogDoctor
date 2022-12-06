#ifndef VECOPS_H
#define VECOPS_H

#include <string>
#include <vector>


//! VecOps
/*!
    Utilities for the vectors
*/
template <typename T>
class VecOps
{
public:
    VecOps();

    //! Checks if a vector contains an element
    /*!
        \param list The target vector
        \param flag The element to search for
        \return Whether the list does contain the flag or not
    */
    static const bool contains( const std::vector<T>& list, const T& flag );
};

#endif // VECOPS_H
