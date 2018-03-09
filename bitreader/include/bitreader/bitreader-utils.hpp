#pragma once
#include <cstdint>
#include <type_traits>

namespace brcpp
{
    template<typename T>
    using if_unsigned_integral = std::enable_if_t<std::is_unsigned_v<T> && std::is_integral_v<T>, T>;

    template<typename T>
    using if_signed_integral = std::enable_if_t<std::is_signed_v<T> && std::is_integral_v<T>, T>;

    template<typename T>
    using if_integral = std::enable_if_t<std::is_integral_v<T>, T>;

    template<typename T>
    using if_floating_point = std::enable_if_t<std::is_floating_point_v<T>, T>;

    template<typename T>
    using if_bit_readable = std::enable_if_t<std::is_floating_point_v<T> || std::is_integral_v<T>, T>;

    struct binary_codec {};

    template<typename T>
    using if_binary_codec = std::enable_if_t<std::is_base_of_v<binary_codec, T>, typename T::value_type>;

    //--------------------------------------------------------------------------
    template<typename T>
    struct bit_read_helper
    {
        static constexpr const size_t max_bits = 8 * sizeof(T);
        static constexpr const size_t min_bits = 0;
        static constexpr const bool is_signed = std::is_signed_v<T>;
    };

    //--------------------------------------------------------------------------
    template<typename T>
    struct floating_point_bit_read_helper
    {
        static constexpr const size_t max_bits = 8 * sizeof(T);
        static constexpr const size_t min_bits = 8 * sizeof(T);
        static constexpr const bool is_signed = std::is_signed_v<T>;
    };

    //--------------------------------------------------------------------------
    template<> struct bit_read_helper<float>: floating_point_bit_read_helper<float> {};
    template<> struct bit_read_helper<double>: floating_point_bit_read_helper<double> {};
    template<> struct bit_read_helper<long double>: floating_point_bit_read_helper<long double> {};
}
