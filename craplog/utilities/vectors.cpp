
#include "vectors.h"

using std::string, std::vector;

VecOps::VecOps()
{

}


bool VecOps::contains( vector<string> list, string flag )
{
    bool result = false;
    for ( string& item : list ) {
        if ( item == flag ) {
            result = true;
            break;
        }
    }
    return result;
}
