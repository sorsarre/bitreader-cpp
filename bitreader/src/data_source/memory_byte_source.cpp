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
size_t memory_byte_source::get_n(uint64_t& buf, size_t bytes)
{
    if (_current == _data.end()) {
        throw std::runtime_error("Access beyond data buffer boundaries");
    }

    auto to_shift = std::min(bytes, available());
    for (size_t iter = 0; iter < to_shift; ++iter) {
        buf <<= 8;
        buf |= *_current;
        ++_current;
    }

    return to_shift;
}

//----------------------------------------------------------------------
bool memory_byte_source::depleted()
{
    return true; // Memory buffer source is always depleted
    // I.e. never gets any additional data
}

//----------------------------------------------------------------------
uint64_t memory_byte_source::available()
{
    return static_cast<uint64_t>(_data.end() - _current);
}

//----------------------------------------------------------------------
uint64_t memory_byte_source::position()
{
    return static_cast<uint64_t>(_current - _data.begin());
}

//----------------------------------------------------------------------
void memory_byte_source::seek(uint64_t position)
{
    if (position > _data.size()) {
        throw std::range_error("Position outside of the data buffer");
    }

    _current = _data.begin() + position;
}

//----------------------------------------------------------------------
void memory_byte_source::skip(uint64_t bytes)
{
    if (bytes > available()) {
        throw std::range_error("Cannot skip beyond the boundaries of the data buffer");
    }

    _current += bytes;
}

//----------------------------------------------------------------------
std::shared_ptr<memory_byte_source> memory_byte_source::clone()
{
    auto ret = std::make_shared<memory_byte_source>();
    ret->_data = shared_buffer::clone(_data);
    ret->_current = ret->_data.begin() + position();
    return ret;
}
