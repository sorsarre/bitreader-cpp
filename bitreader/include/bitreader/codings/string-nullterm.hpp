#pragma once

#include <string>
#include <bitreader/bitreader-utils.hpp>

namespace brcpp::ext
{
    struct string_nullterm: public brcpp::binary_codec_base
    {
        using value_type = std::string;

        template<typename Reader>
        static std::string read(Reader& br)
        {
            std::string ret;
            char current = br.template read<char>(8);
            while (current != '\0') {
                ret.push_back(current);
                current = br.template read<char>(8);
            }

            return ret;
        }

        template<typename Writer>
        static void write(Writer& w, const std::string& value)
        {
            for (char c: value) {
                w.write(c, 8);
            }
            w.write('\0', 8);
        }
    };
}