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

    struct binary_codec_base {};

    template<typename T>
    concept binary_codec = 
        std::derived_from<T, binary_codec_base> &&
        requires { typename T::value_type; };

    template<typename T>
    concept enumeration = std::is_enum_v<T>;

    using std::integral;
    using std::unsigned_integral;
    using std::signed_integral;
    using std::floating_point;

    template<typename T>
    struct bit_read_helper {};

    //--------------------------------------------------------------------------
    template<integral T>
    struct bit_read_helper<T>
    {
        static constexpr const size_t max_bits = 8 * sizeof(T);
        static constexpr const size_t min_bits = 0;
        static constexpr const bool is_signed = std::is_signed_v<T>;
    };

    //--------------------------------------------------------------------------
    template<floating_point T>
    struct bit_read_helper<T>
    {
        static constexpr const size_t max_bits = 8 * sizeof(T);
        static constexpr const size_t min_bits = 8 * sizeof(T);
        static constexpr const bool is_signed = std::is_signed_v<T>;
    };
}
