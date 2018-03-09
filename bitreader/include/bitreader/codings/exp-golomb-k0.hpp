#pragma once

#include <bitreader/bitreader-utils.hpp>

namespace brcpp::ext
{
    template<typename T>
    struct exp_golomb_k0: public brcpp::binary_codec
    {
        using value_type = T;

        template<typename Reader>
        static T read(Reader& br)
        {
            size_t counter = 0;
            T result = T(0);
            while (br.template read<uint8_t>(1) == 0) {
                ++counter;
            }

            result = 1 << counter;
            if (counter > 0) {
                result |= br.template read<T>(counter);
            }

            return result-1;
        }
    };
}
