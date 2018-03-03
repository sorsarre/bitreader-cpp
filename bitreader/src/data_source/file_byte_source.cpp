#include "bitreader/data_source/file_byte_source.hpp"

using namespace brcpp;

//----------------------------------------------------------------------
file_byte_source::file_byte_source(std::shared_ptr<file_reader> reader)
        : _reader(std::move(reader)),
          _buffer(shared_buffer::allocate(32 * 1024)) // TODO: Magic number, replace it
        , _position(0), _last(0) {

}

//----------------------------------------------------------------------
uint8_t file_byte_source::get() {
    if (
            _position < _last ||
            _position >= _last + _buffer.size() ||
            _buffer.size() == 0)
    {
        load_buffer();
    }

    if (_buffer.size() == 0 && available() == 0) {
        throw std::runtime_error("Cannot read beyond the end of the file");
    }
    return _buffer.get()[_position - _last];
}

//----------------------------------------------------------------------
void file_byte_source::next() {
    if (available() > 0) {
        ++_position;
    }
}

//----------------------------------------------------------------------
bool file_byte_source::depleted() {
    return _reader->depleted();
}

//----------------------------------------------------------------------
size_t file_byte_source::available() {
    return _reader->size() - _position;
}

//----------------------------------------------------------------------
void file_byte_source::seek(size_t position) {
    if (position > _reader->size()) {
        throw std::range_error("Cannot seek beyond file size");
    }

    _position = position;
}

//----------------------------------------------------------------------
void file_byte_source::skip(size_t bytes) {
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
}
