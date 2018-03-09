cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

set(BITREADER_SOURCES
        src/common/direct_file_reader.cpp
        src/common/shared_buffer.cpp
        src/data_source/file_byte_source.cpp
        src/data_source/memory_byte_source.cpp)

set(BITREADER_HEADERS
        include/bitreader/bitreader.hpp
        include/bitreader/common/shared_buffer.hpp
        include/bitreader/common/direct_file_reader.hpp
        include/bitreader/common/file_reader.hpp
        include/bitreader/data_source/memory_byte_source.hpp
        include/bitreader/data_source/file_byte_source.hpp
    )

add_library(bitreadercpp STATIC ${BITREADER_SOURCES} ${BITREADER_HEADERS})
target_include_directories(bitreadercpp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_include_directories(bitreadercpp PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

enable_testing()
find_package(GTest)

if (NOT GTEST_FOUND)
    message(WARNING "Unit tests are disabled - gtest not found")
else()
    add_subdirectory(test)
endif()
