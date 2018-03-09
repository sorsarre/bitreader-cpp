#pragma once
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <cassert>
#include <stdexcept>

namespace brcpp {
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

    //--------------------------------------------------------------------------
    class bitreader {
    public:
        bitreader();

        /**
         * Set the buffer to read from
         * @param data Pointer to the data
         * @param length Length of the input buffer
         */
        void set_data(const uint8_t* data, size_t length);

        /**
         * @return Current position in the input stream (in bits)
         */
        size_t position() const;

        /**
         * @return The number of bits available for reading
         */
        size_t available() const;

        /**
         * @brief Set the current position in the input stream
         * @param bitpos Position to seek towards
         */
        void seek(size_t bitpos);

        /**
         * @brief Skips the necessary number of bits to ensure alignment
         * @param bits Number of bits to align for
         */
        void align(size_t bits);

        /**
         * @brief Skip the specified number of bits in the stream
         * @param bits Number of bits to skip
         */
        void skip(size_t bits);

        //----------------------------------------------------------------------
        /**
         * @brief       Reads the binary data.
         * @tparam T    Type to read into (uintXX_t/intXX_t)
         * @param bits  Number of bits to read
         * @return      The data read from the stream
         */
        template<typename T>
        if_integral<T> read(size_t bits)
        {
            _validate_read_dynamic<T>(bits);
            T ret = T(0);
            _read(_state, bits, ret);
            return _sign_extend(ret, bits);
        }

        //----------------------------------------------------------------------
        /**
         * @brief       Read the binary data without advancing the position.
         * @tparam T    Type to read into (uintXX_t/intXX_t)
         * @param bits  Number of bits to read
         * @return      The data read from the stream
         */
        template<typename T>
        if_integral<T> peek(size_t bits)
        {
            _validate_read_dynamic<T>(bits);
            T ret = T(0);
            _peek(_state, bits, ret);
            return _sign_extend(ret, bits);
        }

    private:
        //----------------------------------------------------------------------
        struct internal_state {
            uint64_t buffer = 0;
            size_t shift = 0;
            const uint8_t* ptr = nullptr;
        };

        //----------------------------------------------------------------------
        template<typename T>
        if_signed_integral<T> _sign_extend(T raw, size_t bits)
        {
            T m = 1U << (bits - 1);
            return (raw ^ m) - m;
        }

        //----------------------------------------------------------------------
        template<typename T>
        if_unsigned_integral<T> _sign_extend(T raw, size_t bits)
        {
            return raw;
        }

        //----------------------------------------------------------------------
        void _next(internal_state& state) const;
        void _skip(internal_state& state, size_t bits) const;
        size_t _position(const internal_state& state) const;
        size_t _available(const internal_state& state) const;
        void _align(internal_state& state, size_t bits) const;

        //----------------------------------------------------------------------
        template<typename T>
        static constexpr T _mask(size_t bits)
        {
            return (bits == sizeof(T)*8) ? (~0) : ((T(1) << bits) - 1);
        }

        //----------------------------------------------------------------------
        template<typename T>
        void _elementary_read(internal_state& state, size_t bits, T& ret) const
        {
            state.shift -= bits;
            ret |= (state.buffer >> state.shift) & _mask<T>(bits);
        }

        //----------------------------------------------------------------------
        template<typename T>
        void _read(internal_state& state, size_t bits, T& ret) const
        {
            if (_available(state) < bits) {
                throw std::runtime_error("Cannot read beyond the bitstream data");
            }

            if (bits < state.shift) {
                _elementary_read(state, bits, ret);
            } else if (bits == state.shift) {
                _elementary_read(state, bits, ret);
                _next(state);
            } else {
                bits -= state.shift;
                _elementary_read(state, state.shift, ret);
                ret <<= bits;
                _next(state);
                _elementary_read(state, bits, ret);
            }
        }

        //----------------------------------------------------------------------
        template<typename T>
        void _peek(internal_state& state, size_t bits, T& ret) const
        {
            internal_state temporary = state;
            _read(temporary, bits, ret);
        }

        //----------------------------------------------------------------------
        template <typename T, size_t Size>
        constexpr void _validate_read_static() const
        {
            static_assert(Size <= bit_read_helper<T>::max_bits);
            static_assert(Size >= bit_read_helper<T>::min_bits);
        }

        //----------------------------------------------------------------------
        template <typename T>
        void _validate_read_dynamic(size_t size) const
        {
            assert(size <= bit_read_helper<T>::max_bits);
            assert(size >= bit_read_helper<T>::min_bits);
        }

        internal_state _state;
        const uint8_t* _data;
        const uint8_t* _data_end;
    };

}
