#pragma once
#include <string>
#include <stdexcept>

#include "bitreader/common/shared_buffer.hpp"

namespace brcpp
{
    class memory_byte_source
    {
    public:
        memory_byte_source();
        memory_byte_source(const uint8_t* data, size_t size);
        uint8_t get();
        void next();
        bool depleted();
        size_t available();
        void seek(size_t position);
        void skip(size_t bytes);
    private:
        shared_buffer::iterator _current;
        shared_buffer _data;
    };
}
