#pragma once
#include <cstdint>
#include <cstddef>
#include <memory>
#include "bitreader/common/file_reader.hpp"
#include "bitreader/common/shared_buffer.hpp"

namespace brcpp
{
    class file_byte_source
    {
    public:
        explicit file_byte_source(std::shared_ptr<file_reader> reader);
        uint8_t get();
        void next();
        bool depleted();
        size_t available();
        void seek(size_t position);
        void skip(size_t bytes);

    private:
        void load_buffer();

        std::shared_ptr<file_reader> _reader;
        shared_buffer _buffer;
        uint64_t _position;
        uint64_t _last;
    };
}
