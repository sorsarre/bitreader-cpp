#pragma once
#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace brcpp
{
    template<typename T>
    struct id_t
    {
        using type = T;
    };

    template<size_t Size>
    struct integral_by_size
    {
        using type = void;
    };

    template<> struct integral_by_size<1>: id_t<uint8_t> {};
    template<> struct integral_by_size<2>: id_t<uint16_t> {};
    template<> struct integral_by_size<4>: id_t<uint32_t> {};
    template<> struct integral_by_size<8>: id_t<uint64_t> {};

    template<typename Enum>
    using fitting_integral = typename integral_by_size<sizeof(Enum)>::type;

    struct binary_codec {};

    template<bool cond, typename R>
    using cond_if_t = std::enable_if_t<cond, R>;

    template<typename T>
    using check_unsigned_integral = std::bool_constant<std::is_unsigned_v<T> && std::is_integral_v<T>>;

    template<typename T>
    using check_signed_integral = std::bool_constant<std::is_signed_v<T> && std::is_integral_v<T>>;

    template<typename T>
    using check_integral = std::bool_constant<std::is_integral_v<T>>;

    template<typename T>
    using check_floating_point = std::bool_constant<std::is_floating_point_v<T>>;

    template<typename T>
    using check_bit_readable = std::bool_constant<std::is_floating_point_v<T> || std::is_integral_v<T>>;

    template<typename T>
    using check_binary_codec = std::bool_constant<std::is_base_of_v<binary_codec, T>>;

    template<typename T>
    using check_enum = std::bool_constant<std::is_enum_v<T>>;


    template<typename T>
    using if_unsigned_integral = std::enable_if_t<check_unsigned_integral<T>::value, T>;

    template<typename T>
    using if_signed_integral = std::enable_if_t<check_signed_integral<T>::value, T>;

    template<typename T>
    using if_integral = std::enable_if_t<check_integral<T>::value, T>;

    template<typename T>
    using if_floating_point = std::enable_if_t<check_floating_point<T>::value, T>;

    template<typename T>
    using if_bit_readable = std::enable_if_t<check_bit_readable<T>::value, T>;

    template<typename T>
    using if_binary_codec = std::enable_if_t<check_binary_codec<T>::value, typename T::value_type>;

    template<typename T>
    using if_enum = std::enable_if_t<check_enum<T>::value, T>;


    template<typename T>
    using if_unsigned_integral_void = std::enable_if_t<check_unsigned_integral<T>::value, void>;

    template<typename T>
    using if_signed_integral_void = std::enable_if_t<check_signed_integral<T>::value, void>;

    template<typename T>
    using if_integral_void = std::enable_if_t<check_integral<T>::value, void>;

    template<typename T>
    using if_floating_point_void = std::enable_if_t<check_floating_point<T>::value, void>;

    template<typename T>
    using if_bit_readable_void = std::enable_if_t<check_bit_readable<T>::value, void>;

    template<typename T>
    using if_binary_codec_void = std::enable_if_t<check_binary_codec<T>::value, void>;

    template<typename T>
    using if_enum_void = std::enable_if_t<check_enum<T>::value, void>;

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
