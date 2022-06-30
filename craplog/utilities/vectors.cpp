
#include "vectors.h"

using std::string, std::vector;

VecOps::VecOps()
{

}


bool VecOps::contains( const vector<string>& list, const string& flag )
{
    bool result = false;
    for ( const string& item : list ) {
        if ( item == flag ) {
            result = true;
            break;
        }
    }
    return result;
}
