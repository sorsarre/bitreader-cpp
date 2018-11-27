cmake_minimum_required(VERSION 3.7)

enable_testing()

############# Bitreader gtest
add_executable(bitreader_gtest bitreader_gtest.cpp)

target_include_directories(bitreader_gtest PRIVATE ${GTEST_INCLUDE_DIRS})

target_link_libraries(bitreader_gtest bitreadercpp)
target_link_libraries(bitreader_gtest ${GTEST_BOTH_LIBRARIES})
target_link_libraries(bitreader_gtest pthread)
set_target_properties(bitreader_gtest PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

add_test(AllTestsBitreader bitreader_gtest)

############## Common gtest
add_executable(common_gtest
        shared_buffer_gtest.cpp
        memory_byte_source_gtest.cpp
        file_byte_source_gtest.cpp
        gtest_common_gtest.cpp
        gtest_common.hpp)

target_include_directories(common_gtest PRIVATE ${GTEST_INCLUDE_DIRS})

target_link_libraries(common_gtest bitreadercpp)
target_link_libraries(common_gtest ${GTEST_BOTH_LIBRARIES})
target_link_libraries(common_gtest pthread)
set_target_properties(common_gtest PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

add_test(AllTestsCommon common_gtest)
