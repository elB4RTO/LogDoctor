#ifndef LOGDOCTOR__WORKAROUNDS__LIB_H
#define LOGDOCTOR__WORKAROUNDS__LIB_H


#include <array>
#include <ranges>


template <typename T>
struct is_std_array : std::false_type {};

template <typename T, std::size_t N>
struct is_std_array<std::array<T,N>> : std::true_type {};

template <typename T, std::size_t N>
struct is_std_array<const std::array<T,N>> : std::true_type {};

template <typename A>
concept IsStdArray = is_std_array<A>::value;


template <typename Cont>
concept HasRangesEnumerate = requires(Cont c) {
    std::views::enumerate(c);
};


template <typename Cont1, typename Cont2>
concept HasRangesZip = requires(Cont1 c1, Cont2 c2) {
    std::views::zip(c1, c2);
};


#endif // LOGDOCTOR__WORKAROUNDS__LIB_H
