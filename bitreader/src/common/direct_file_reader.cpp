#include "bitreader/common/direct_file_reader.hpp"

using namespace brcpp;

//----------------------------------------------------------------------
size_t direct_file_reader::read(uint8_t* dest, uint64_t position, size_t bytes) {
    if (fseeko64(_file, position, SEEK_SET) < 0) {
        throw std::runtime_error("Could not seek to designated position");
    }

    return fread(dest, 1, bytes, _file);
}

//----------------------------------------------------------------------
uint64_t direct_file_reader::size() {
    fseeko64(_file, 0, SEEK_END);
    auto result = ftello64(_file);
    if (result < 0) {
        throw std::runtime_error("Could not seek to the end of the file");
    }

    return static_cast<uint64_t>(result);
}

//----------------------------------------------------------------------
bool direct_file_reader::depleted() {
    return true;
}

//----------------------------------------------------------------------
direct_file_reader::~direct_file_reader() {
    fclose(_file);
}

//----------------------------------------------------------------------
std::shared_ptr<file_reader> direct_file_reader::open(const std::string& path) {
    return std::shared_ptr<file_reader>(new direct_file_reader(path));
}


//----------------------------------------------------------------------
direct_file_reader::direct_file_reader(const std::string& path)
    : _path(path)
{
    _file = fopen64(path.c_str(), "rb");
    if (!_file) {
        throw std::runtime_error("Could not open file for reading");
    }
}

//----------------------------------------------------------------------
std::shared_ptr<file_reader> direct_file_reader::clone()
{
    auto ret = new direct_file_reader(_path);
    return std::shared_ptr<file_reader>(ret);
}
