#include <csignal>
#include "gtest/gtest.h"
#include "gtest_common.hpp"

//------------------------------------------------------------------------------
TEST(testData, zeroSize)
{
    auto data = generate_test_data(0);
    EXPECT_EQ(nullptr, data);
}

//------------------------------------------------------------------------------
TEST(testData, contents)
{
    const size_t size = 10;
    auto data = generate_test_data(size);
    EXPECT_NE(nullptr, data);
    for (size_t iter = 0; iter < size; ++iter) {
        EXPECT_EQ(iter+1, data[iter]);
    }
    delete[] data;
}

//------------------------------------------------------------------------------
TEST(fakeFileReaderTest, zeroSize)
{
    fake_file_reader reader(0);
    EXPECT_EQ(0, reader.size());
    EXPECT_TRUE(reader.depleted());

    size_t read = 1;
    EXPECT_NO_THROW(read = reader.read(nullptr, 0, 0));
    EXPECT_EQ(0, read);
    EXPECT_NO_THROW(read = reader.read(nullptr, 0, 1));
    EXPECT_EQ(0, read);
    EXPECT_ANY_THROW(reader.read(nullptr, 1, 1));
    EXPECT_ANY_THROW(reader.read(nullptr, 1, 0));
}

//------------------------------------------------------------------------------
TEST(fakeFileReaderTest, nonZeroSize)
{
    const size_t size = 10;
    fake_file_reader reader(size);
    EXPECT_EQ(size, reader.size());
    EXPECT_TRUE(reader.depleted());

    size_t read = 1;
    uint8_t buffer[size];
    EXPECT_NO_THROW(read = reader.read(buffer, 0, 0));
    EXPECT_EQ(0, read);

    EXPECT_NO_THROW(read = reader.read(buffer, 0, size/2));
    EXPECT_EQ(size/2, read);
    for (size_t iter = 0; iter < size/2; ++iter) {
        EXPECT_EQ(iter+1, buffer[iter]);
    }

    EXPECT_NO_THROW(read = reader.read(buffer, size/2, size/2));
    EXPECT_EQ(size/2, read);
    for (size_t iter = size/2; iter < size/2; ++iter) {
        EXPECT_EQ(iter+1, buffer[iter]);
    }

    EXPECT_NO_THROW(read = reader.read(buffer, size, size/2));
    EXPECT_EQ(0, read);

    EXPECT_ANY_THROW(reader.read(buffer, size+1, size/2));
    EXPECT_ANY_THROW(reader.read(buffer, size+1, 0));
}
