#pragma once
#include <cstdio>
#include <cstdint>
#include <cstddef>

#include <stdexcept>
#include <string>
#include <memory>

#include "bitreader/common/file_reader.hpp"

namespace brcpp
{
    class direct_file_reader: public file_reader
    {
    public:
        size_t read(uint8_t* dest, uint64_t position, size_t bytes) override;
        uint64_t size() override;
        bool depleted() override;
        ~direct_file_reader() override;
        std::shared_ptr<file_reader> clone() override;
        static std::shared_ptr<file_reader> open(const std::string& path);

    private:
        explicit direct_file_reader(const std::string& path);

        const std::string _path;
        FILE* _file;
    };
}
