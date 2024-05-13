#ifndef LOGDOCTOR__WORKAROUNDS__LIB_H
#define LOGDOCTOR__WORKAROUNDS__LIB_H


#include <array>


template <typename T>
struct is_std_array : std::false_type {};

template <typename T, std::size_t N>
struct is_std_array<std::array<T,N>> : std::true_type {};

template <typename T, std::size_t N>
struct is_std_array<const std::array<T,N>> : std::true_type {};

template <typename A>
concept IsStdArray = is_std_array<A>::value;


#endif // LOGDOCTOR__WORKAROUNDS__LIB_H
