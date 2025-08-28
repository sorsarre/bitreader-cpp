cmake_minimum_required(VERSION 3.10)

set(BRCPP_SANITIZER "none" CACHE STRING "Compile with a specific Clang sanitizer (address,undefined,leak,memory,type)")

function(brcpp_add_cxx_flags FLAGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")
endfunction()

function(brcpp_add_exe_link_flags FLAGS)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${FLAGS}")
endfunction()

function(brcpp_enable_sanitizer SAN)
    if (NOT SAN MATCHES "none")
        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            add_compile_options(-fsanitize=${SAN})
            add_link_options(-fsanitize=${SAN})
        else()
            message(FATAL_ERROR "Sanitizers can only be used with Clang. Current compiler is ${CMAKE_CXX_COMPILER_ID}.")
        endif()
    endif()
endfunction()

brcpp_enable_sanitizer(${BRCPP_SANITIZER})

if (NOT WIN32)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Wshorten-64-to-32)
    endif()
    add_compile_options(-Wall -Wconversion -Wsign-conversion -Werror -pedantic)
else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

set(BITREADER_SOURCES
        src/common/direct_file_reader.cpp
        src/common/shared_buffer.cpp
        src/data_source/file_byte_source.cpp
        src/data_source/memory_byte_source.cpp)

set(BITREADER_HEADERS
        include/bitreader/bitreader.hpp
        include/bitreader/bitwriter.hpp
        include/bitreader/common/shared_buffer.hpp
        include/bitreader/common/direct_file_reader.hpp
        include/bitreader/common/file_reader.hpp
        include/bitreader/data_source/memory_byte_source.hpp
        include/bitreader/data_source/file_byte_source.hpp
    )

add_library(bitreadercpp STATIC ${BITREADER_SOURCES} ${BITREADER_HEADERS})
target_include_directories(bitreadercpp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_include_directories(bitreadercpp PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
set_target_properties(bitreadercpp PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

enable_testing()
find_package(GTest)

if (NOT GTEST_FOUND)
    message(WARNING "Unit tests are disabled - gtest not found")
else()
    add_subdirectory(test)
endif()
