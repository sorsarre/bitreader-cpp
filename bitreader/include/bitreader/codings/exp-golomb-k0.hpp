#pragma once

#include <bitreader/bitreader-utils.hpp>

namespace brcpp::ext
{
    template<typename T>
    struct exp_golomb_k0: public brcpp::binary_codec_base
    {
        using value_type = T;

        template<typename Reader>
        static T read(Reader& br)
        {
            size_t counter = 0;
            T result = zero<T>;
            while (br.template read<uint8_t>(1) == 0) {
                ++counter;
            }

            result = static_cast<T>(one<T> << counter);
            if (counter > 0) {
                result |= br.template read<T>(counter);
            }

            return result-1;
        }

        template<typename Writer>
        static void write(Writer& w, T value)
        {
            if (value == T{0}) {
                w.template write<uint8_t>(1, 1);
                return;
            }
            // x = 1'abcd
            // encoded(x) = 00001abcd
            size_t counter = 0;
            while ((value >> counter) != 1) {
                ++counter;
            }

            w.template write<T>(0, counter);
            w.template write<uint8_t>(1, 1);
            w.template write<T>(value, counter);
        }
    };
}
