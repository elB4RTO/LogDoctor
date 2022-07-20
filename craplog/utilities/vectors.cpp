
#include "vectors.h"


VecOps::VecOps()
{

}



bool VecOps::contains( const std::vector<int>& list, const int flag )
{
    bool result = false;
    for ( const int& item : list ) {
        if ( item == flag ) {
            result = true;
            break;
        }
    }
    return result;
}


bool VecOps::contains( const std::vector<std::string>& list, const std::string& flag )
{
    bool result = false;
    for ( const std::string& item : list ) {
        if ( item == flag ) {
            result = true;
            break;
        }
    }
    return result;
}
