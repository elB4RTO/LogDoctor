#ifndef LOGDOCTOR__ARRAYOPS_H
#define LOGDOCTOR__ARRAYOPS_H


#include <array>
#include <tuple>


template <typename T>
struct is_std_array : std::false_type {};

template <typename T, std::size_t N>
struct is_std_array<std::array<T,N>> : std::true_type {};

template <typename T, std::size_t N>
struct is_std_array<const std::array<T,N>> : std::true_type {};

template <typename A>
concept IsStdArray = is_std_array<A>::value;


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


//! ArrayOps
/*!
    Utilities for the arrays
*/
namespace ArrayOps
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

} // namespace ArrayOps


#endif // LOGDOCTOR__ARRAYOPS_H
