#ifndef VECOPS_H
#define VECOPS_H

#include <string>
#include <vector>


//! VecOps
/*!
    Utilities for the vectors
*/
class VecOps
{
public:
    VecOps();

    //! Checks if a vector of integers contains an element
    /*!
        \param list The target vector
        \param flag The element to search for
        \return Whether the list does contain the flag or not
    */
    static const bool contains( const std::vector<int>& list, const int& flag );

    //! Checks if a vector of strings contains an element
    /*!
        \overload static const bool contains( const std::vector<int>& list, const int& flag )
        \param list The target vector
        \param flag The element to search for
        \return Whether the list does contain the flag or not
    */
    static const bool contains( const std::vector<std::string>& list, const std::string& flag );
};

#endif // VECOPS_H
