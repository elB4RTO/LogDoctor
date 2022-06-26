#include "vectors.h"

VecOps::VecOps()
{

}


bool VecOps::contains( std::vector<std::string> list, std::string flag )
{
    bool result = false;
    for ( std::string& item : list ) {
        if ( item == flag ) {
            result = true;
            break;
        }
    }
    return result;
}
