#include "bitreader/data_source/file_byte_source.hpp"
#include <stdexcept>

using namespace brcpp;

static constexpr const size_t InitialBufferSize = 32 * 1024;

//----------------------------------------------------------------------
file_byte_source::file_byte_source(std::shared_ptr<file_reader> reader)
        : _reader(std::move(reader)),
          _buffer(shared_buffer::allocate(InitialBufferSize))
        , _position(0), _last(0) {

}

//----------------------------------------------------------------------
size_t file_byte_source::get_n(uint64_t& buf, size_t bytes) {
    if (bytes == 0) {
        return 0;
    }

    if (
            _position < _last ||
            _position + bytes >= _last + _buffer.size() ||
            _buffer.size() == 0)
    {
        load_buffer();
    }

    if (_buffer.size() == 0 && available() == 0) {
        throw std::runtime_error("Cannot read beyond the end of the file");
    }

    auto to_shift = std::min(available(), bytes);
    for (size_t iter = 0; iter < to_shift; ++iter) {
        buf <<= 8;
        buf |= _buffer.get()[_position - _last];
        ++_position;
    }

    return to_shift;
}

//----------------------------------------------------------------------
bool file_byte_source::depleted() {
    return _reader->depleted();
}

//----------------------------------------------------------------------
uint64_t file_byte_source::available() {
    return _reader->size() - _position;
}

//----------------------------------------------------------------------
uint64_t file_byte_source::position() {
    return _position;
}

//----------------------------------------------------------------------
void file_byte_source::seek(uint64_t position) {
    if (position > _reader->size()) {
        throw std::range_error("Cannot seek beyond file size");
    }

    _position = position;
}

//----------------------------------------------------------------------
void file_byte_source::skip(uint64_t bytes) {
    if (bytes > available()) {
        throw std::range_error("Cannot skip beyond file size");
    }

    _position += bytes;
}

//----------------------------------------------------------------------
void file_byte_source::load_buffer()
{
    auto read = _reader->read(_buffer.get(), _position, _buffer.capacity());
    _buffer.resize(read);
    _last = _position;
}

//----------------------------------------------------------------------
std::shared_ptr<file_byte_source> file_byte_source::clone()
{
    auto ret = std::make_shared<file_byte_source>(_reader->clone());
    ret->_buffer = shared_buffer::clone(_buffer);
    ret->_position = _position;
    ret->_last = _last;
    return ret;
}
