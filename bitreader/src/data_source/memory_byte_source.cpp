#include "bitreader/data_source/memory_byte_source.hpp"

using namespace brcpp;

//----------------------------------------------------------------------
memory_byte_source::memory_byte_source()
{
    _current = nullptr;
}

//----------------------------------------------------------------------
memory_byte_source::memory_byte_source(const uint8_t* data, size_t size)
        : _data(shared_buffer::copy_mem(data, size))
{
    _current = _data.begin();
}

//----------------------------------------------------------------------
uint8_t memory_byte_source::get()
{
    if (_current == _data.end()) {
        throw std::runtime_error("Access beyond data buffer boundaries");
    }

    return *_current;
}

//----------------------------------------------------------------------
void memory_byte_source::next()
{
    if (_current != _data.end()) {
        ++_current;
    }
}

//----------------------------------------------------------------------
bool memory_byte_source::depleted()
{
    return true; // Memory buffer source is always depleted
    // I.e. never gets any additional data
}

//----------------------------------------------------------------------
size_t memory_byte_source::available()
{
    return _data.end() - _current;
}

//----------------------------------------------------------------------
void memory_byte_source::seek(size_t position)
{
    if (position > _data.size()) {
        throw std::range_error("Position outside of the data buffer");
    }

    _current = _data.begin() + position;
}

//----------------------------------------------------------------------
void memory_byte_source::skip(size_t bytes)
{
    if (bytes > available()) {
        throw std::range_error("Cannot skip beyond the boundaries of the data buffer");
    }

    _current += bytes;
}
