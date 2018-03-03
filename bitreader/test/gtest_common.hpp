#pragma once
#include "bitreader/common/file_reader.hpp"
#include "bitreader/common/shared_buffer.hpp"

using namespace brcpp;

namespace {
    //--------------------------------------------------------------------------
    uint8_t* generate_test_data(size_t size)
    {
        if (size == 0) {
            return nullptr;
        }

        auto ret = new uint8_t[size];
        for (size_t iter = 0; iter < size; ++iter) {
            ret[iter] = static_cast<uint8_t>(iter+1);
        }
        return ret;
    }

    //--------------------------------------------------------------------------
    class fake_file_reader: public file_reader
    {
    public:
        //----------------------------------------------------------------------
        explicit fake_file_reader(size_t size)
        {
            _data = shared_buffer::wrap_mem(
                generate_test_data(size),
                size
            );
        }

        //----------------------------------------------------------------------
        size_t read(uint8_t* dest, uint64_t position, size_t bytes) override
        {
            if (position > _data.size()) {
                throw std::runtime_error("Cannot seek beyond the end of the buffer");
            }

            size_t to_copy = std::min(_data.size() - position, bytes);
            auto begin = _data.begin() + position;
            auto end = begin + to_copy;
            std::copy(begin, end, dest);
            return to_copy;
        }

        //----------------------------------------------------------------------
        uint64_t size() override
        {
            return _data.size();
        }

        //----------------------------------------------------------------------
        bool depleted() override
        {
            return true;
        }

        //----------------------------------------------------------------------
        ~fake_file_reader() override = default;

    private:
        shared_buffer _data;
    };
}
