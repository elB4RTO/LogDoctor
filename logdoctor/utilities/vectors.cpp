
#include "vectors.h"


template <typename T>
VecOps<T>::VecOps()
{

}


template <typename T>
const bool VecOps<T>::contains(const std::vector<T>& list, const T& flag )
{
    bool result = false;
    for ( const T& item : list ) {
        if ( item == flag ) {
            result = true;
            break;
        }
    }
    return result;
}


template class VecOps<int>;
template class VecOps<std::string>;
