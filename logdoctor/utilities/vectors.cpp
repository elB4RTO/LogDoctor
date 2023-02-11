
#include "vectors.h"

#include <string>
#include <algorithm>


template <typename T>
VecOps<T>::VecOps()
{

}


template <typename T>
const bool VecOps<T>::contains(const std::vector<T>& list, const T& flag )
{
    return std::any_of(
        list.cbegin(), list.cend(),
        [&flag]( const T& item )
               { return item == flag; } );
}


template class VecOps<int>;
template class VecOps<std::string>;
