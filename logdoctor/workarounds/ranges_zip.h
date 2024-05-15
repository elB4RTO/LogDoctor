#ifndef LOGDOCTOR__WORKAROUNDS__RANGES_ZIP_H
#define LOGDOCTOR__WORKAROUNDS__RANGES_ZIP_H


#include"lib.h"

#include <tuple>


template <typename Array>
struct Zipperator
{
    using array_value       = typename Array::value_type;
    using array_iterator    = typename Array::iterator;

    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = std::tuple<array_value&, array_value&>;
    using pointer           = std::tuple<array_value&, array_value&>;
    using reference         = std::tuple<array_value&, array_value&>;

    explicit Zipperator( array_iterator l, array_iterator r ) noexcept
    : lit{l},rit{r} {}

    inline reference operator*()
    { return std::tuple<array_value&, array_value&>(*lit,*rit); }

    inline Zipperator& operator++() noexcept
    { lit++; rit++; return *this; }

    friend bool operator!=( const Zipperator& lhs, const Zipperator& rhs ) noexcept
    { return lhs.lit != rhs.lit and lhs.rit != rhs.rit; }

private:
    array_iterator lit;
    array_iterator rit;
};



template <typename Array>
class ZippedArrays
{
    Array& larr;
    Array& rarr;

public:
    explicit ZippedArrays( Array& l, Array& r )
    : larr{l},rarr{r} {}

    inline auto begin() noexcept
    { return Zipperator<Array>(larr.begin(),rarr.begin()); }

    inline auto end() noexcept
    { return Zipperator<Array>(larr.end(),rarr.end()); }
};


//! Workarounds
/*!
    Workarounds for compilers missing features
*/
namespace Workarounds
{

//! Zips two arrays
/*!
    \param l_array The left-side array
    \param r_array The right-side array
    \return An iterator over the two arrays
    \todo Replace with std::views::zip when clang will fully support it
*/
template <typename Array>
requires IsStdArray<Array>
inline auto zip( Array& l_array, Array& r_array )
{
    return ZippedArrays<Array>( l_array, r_array );
}

} // namespace Workarounds


#endif // LOGDOCTOR__WORKAROUNDS__RANGES_ZIP_H
