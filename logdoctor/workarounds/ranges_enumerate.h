#ifndef LOGDOCTOR__WORKAROUNDS__RANGES_ENUMERATE_H
#define LOGDOCTOR__WORKAROUNDS__RANGES_ENUMERATE_H


#include"lib.h"

#include <tuple>


template <typename Array>
struct Enumerator
{
    using array_size_t      = typename Array::size_type;
    using array_value       = typename Array::value_type;
    using array_iterator    = typename Array::const_iterator;

    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = std::tuple<array_size_t, array_value>;
    using pointer           = std::tuple<array_size_t, array_value>;
    using reference         = std::tuple<array_size_t, array_value>;

    explicit Enumerator( array_iterator a ) noexcept
    : idx{0},iter{a} {}

    inline reference operator*()
    { return std::make_tuple(idx,*iter); }

    inline Enumerator& operator++() noexcept
    { idx++; iter++; return *this; }

    friend bool operator!=( const Enumerator& lhs, const Enumerator& rhs ) noexcept
    { return lhs.iter != rhs.iter; }

private:
    array_size_t idx;
    array_iterator iter;
};


template <typename Array>
class EnumeratedArray
{
    const Array& arr;

public:
    explicit EnumeratedArray( const Array& a )
    : arr{a} {}

    inline auto begin() noexcept
    { return Enumerator<Array>(arr.cbegin()); }

    inline auto end() noexcept
    { return Enumerator<Array>(arr.cend()); }
};


//! Workarounds
/*!
    Workarounds for compilers missing features
*/
namespace Workarounds
{

//! Enumerates an array
/*!
    \param array The array
    \return An iterator over the two array along with the index
    \todo Replace with std::views::enumerate when clang will fully support it
*/
template <typename Array>
requires IsStdArray<Array>
inline auto enumerate( const Array& array )
{
    return EnumeratedArray<Array>( array );
}

} // namespace Workarounds


#endif // LOGDOCTOR__WORKAROUNDS__RANGES_ENUMERATE_H
