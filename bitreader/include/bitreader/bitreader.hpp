#pragma once
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <stdexcept>
#include <memory>

#include "bitreader/bitreader-utils.hpp"
#include "common/numeric.hpp"

namespace brcpp {


    //--------------------------------------------------------------------------
    template<typename Source>
    class bitreader {
    public:
        bitreader(std::shared_ptr<Source> source)
        {
            _state.source = source;
            _next(_state);
        }

        bitreader(const bitreader&) = delete;
        bitreader& operator=(const bitreader&) = delete;

        /**
         * @return Current position in the input stream (in bits)
         */
        size_t position() const
        {
            return _position(_state);
        }

        /**
         * @return The number of bits available for reading
         */
        size_t available() const
        {
            return _available(_state);
        }

        /**
         * @brief Set the current position in the input stream
         * @param bitpos Position to seek towards
         */
        void seek(size_t bitpos)
        {
            uint64_t byte_pos = bitpos / 8;
            uint64_t bits_to_skip = bitpos % 8;

            _state.source->seek(byte_pos);
            _state.shift = 0;
            _skip(_state, bits_to_skip);
        }

        /**
         * @brief Skips the necessary number of bits to ensure alignment
         * @param bits Number of bits to align for
         */
        void align(size_t bits)
        {
            _align(_state, bits);
        }

        /**
         * @brief Skip the specified number of bits in the stream
         * @param bits Number of bits to skip
         */
        void skip(size_t bits)
        {
            _skip(_state, bits);
        }

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

        template<typename T>
        if_binary_codec<T> read()
        {
            return T::read(*this);
        }

        template<typename T>
        if_enum<T> read(size_t bits)
        {
            using value_type = std::underlying_type_t<T>;
            auto val = static_cast<T>(read<value_type>(bits));
            return val;
        }

        //----------------------------------------------------------------------
        template<typename T>
        void read(size_t bits, T& out)
        {
            out = this->read<T>(bits);
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
            std::shared_ptr<Source> source;

            internal_state clone()
            {
                return internal_state{
                    buffer,
                    shift,
                    source->clone()
                };
            }
        };

        //----------------------------------------------------------------------
        template<typename T>
        if_signed_integral<T> _sign_extend(T raw, size_t bits)
        {
            const auto m = static_cast<T>(one<T> << (bits - 1));
            return (raw ^ m) - m;
        }

        //----------------------------------------------------------------------
        template<typename T>
        if_unsigned_integral<T> _sign_extend(T raw, size_t bits)
        {
            return raw;
        }

        //----------------------------------------------------------------------
        void _next(internal_state& state) const
        {
            size_t available = std::min<uint64_t>(
                    sizeof(state.buffer),
                    state.source->available());

            size_t to_read = available;
            size_t done_read = state.source->get_n(state.buffer, to_read);
            state.shift = 8 * done_read;
        }

        //----------------------------------------------------------------------
        void _skip(internal_state& state, size_t bits) const
        {
            if (_available(state) < bits) {
                throw std::runtime_error("Cannot skip beyond end of bitstream");
            }

            if (bits < state.shift) {
                state.shift -= bits;
            } else if (bits == state.shift) {
                _next(state);
            } else {
                size_t to_skip = bits - state.shift;
                state.shift = 0;
                state.source->skip(to_skip / 8);
                _next(state);
                state.shift -= to_skip % 8;
            }
        }

        //----------------------------------------------------------------------
        size_t _position(const internal_state& state) const
        {
            return state.source->position() * 8 - state.shift;
        }

        //----------------------------------------------------------------------
        size_t _available(const internal_state& state) const
        {
            return state.source->available() * 8 + state.shift;
        }

        //----------------------------------------------------------------------
        void _align(internal_state& state, size_t bits) const
        {
            size_t advance = (bits - (_position(state) % bits)) % bits;
            if (advance > 0) {
                _skip(state, advance);
            }
        }

        //----------------------------------------------------------------------
        template<typename T>
        static constexpr T _mask(size_t bits)
        {
            if (bits >= sizeof(T)*8)
            {
                return static_cast<T>(~zero<T>);
            }
            else
            {
                return static_cast<T>(one<T> << bits) - one<T>;
            }
        }

        //----------------------------------------------------------------------
        template<typename T>
        void _elementary_read(internal_state& state, size_t bits, T& ret) const
        {
            state.shift -= bits;
            const auto shifted = static_cast<T>(state.buffer >> state.shift);
            const auto masked = static_cast<T>(shifted & _mask<T>(bits));
            ret |= masked;
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
                //_next(state);
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
            internal_state temporary = state.clone();
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
    };

}
