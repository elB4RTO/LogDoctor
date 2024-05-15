#ifndef LOGDOCTOR__WORKAROUNDS__RANGES_JOIN_H
#define LOGDOCTOR__WORKAROUNDS__RANGES_JOIN_H


#include <vector>


//! Workarounds
/*!
    Workarounds for compilers missing features
*/
namespace Workarounds
{

//! Flattens a vector of vectors
/*!
    \param nvecs The nested vectors to join
    \return A flattened vector
    \todo Replace with std::views::join when MacOS will support it
*/
template <typename F>
inline auto join( const std::vector<std::vector<F>>& nvecs )
{
    std::vector<const F*> flat;
    for ( const std::vector<F>& vec : nvecs ) {
        for ( const F& v : vec ) {
            flat.push_back( &v );
        }
    }
    return flat;
}

//! Flattens a vector of vectors
/*!
    \param nvecs The nested vectors to join
    \return A flattened vector
    \todo Replace with std::views::join when MacOS will support it
*/
template <typename F>
inline auto join( const std::vector<const std::vector<F>*>& nvecs )
{
    std::vector<const F*> flat;
    for ( const std::vector<F>* vec : nvecs ) {
        for ( const F& v : *vec ) {
            flat.push_back( &v );
        }
    }
    return flat;
}

} // namespace Workarounds


#endif // LOGDOCTOR__WORKAROUNDS__RANGES_JOIN_H
