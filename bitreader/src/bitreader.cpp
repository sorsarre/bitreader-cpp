#include "bitreader/bitreader.hpp"

using namespace brcpp;

//------------------------------------------------------------------------------
bitreader::bitreader(): _data(nullptr), _data_end(nullptr)
{

}

//------------------------------------------------------------------------------
void bitreader::set_data(const uint8_t* data, size_t length)
{
    _data = data;
    _data_end = data + length;
    _state.ptr = data;
    _next(_state);
}

//------------------------------------------------------------------------------
size_t bitreader::position() const
{
    return _position(_state);
}

//------------------------------------------------------------------------------
size_t bitreader::available() const
{
    return _available(_state);
}

//----------------------------------------------------------------------
void bitreader::seek(size_t bitpos)
{
    size_t diff = _data_end - _data;
    if (diff*8 < bitpos) {
        throw std::runtime_error("Can't seek beyond bitstream size");
    }

    _state.ptr = _data;
    _skip(_state, bitpos);
}

//----------------------------------------------------------------------
void bitreader::align(size_t bits)
{
    _align(_state, bits);
}

//----------------------------------------------------------------------
void bitreader::skip(size_t bits)
{
    _skip(_state, bits);
}

//----------------------------------------------------------------------
void bitreader::_next(internal_state& state) const
{
    size_t available = std::min<size_t>(
            sizeof(state.buffer),
            _data_end - state.ptr);

    size_t to_read = available;
    while (to_read) {
        state.buffer <<= 8;
        state.buffer |= *state.ptr;
        ++state.ptr;
        --to_read;
    }
    state.shift = 8 * available;
}

//----------------------------------------------------------------------
void bitreader::_skip(internal_state& state, size_t bits) const
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
        state.ptr += to_skip / 8;
        _next(state);
        state.shift -= to_skip % 8;
    }
}

//----------------------------------------------------------------------
size_t bitreader::_position(const internal_state& state) const
{
    return (_state.ptr - _data) * 8 - state.shift;
}

//----------------------------------------------------------------------
size_t bitreader::_available(const internal_state& state) const
{
    return (_data_end - state.ptr) * 8 + state.shift;
}

//----------------------------------------------------------------------
void bitreader::_align(internal_state& state, size_t bits) const
{
    // TODO: Rewrite
    size_t advance = (bits - (_position(state) % bits)) % bits;
    if (advance) {
        _skip(state, advance);
    }
}
