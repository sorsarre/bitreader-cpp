#include <gtest/gtest.h>
#include <bitreader/data_source/file_byte_source.hpp>
#include "gtest_common.hpp"

using namespace brcpp;

//------------------------------------------------------------------------------
TEST(fileByteSourceTest, emptyCtor)
{
    const size_t size = 0;
    auto data = std::make_shared<fake_file_reader>(size);
    file_byte_source src(data);

    EXPECT_ANY_THROW(src.get());
    EXPECT_NO_THROW(src.next());
    EXPECT_TRUE(src.depleted());
    EXPECT_EQ(0, src.available());
    EXPECT_NO_THROW(src.seek(0));
    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.seek(1));
    EXPECT_EQ(0, src.available());
    EXPECT_NO_THROW(src.skip(0));
    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.skip(1));
    EXPECT_EQ(0, src.available());
}

//------------------------------------------------------------------------------
TEST(fileByteSourceTest, basic)
{
    const size_t size = 10;
    auto data = std::make_shared<fake_file_reader>(size);
    file_byte_source src(data);

    for (size_t iter = 0; iter < size; ++iter) {
        EXPECT_EQ(size-iter, src.available());
        EXPECT_EQ(true, src.depleted());
        EXPECT_EQ(iter+1, src.get());
        EXPECT_NO_THROW(src.next());
    }

    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.get());
    EXPECT_NO_THROW(src.seek(size));
    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.seek(size+1));
    EXPECT_EQ(0, src.available());
    EXPECT_NO_THROW(src.skip(0));
    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.skip(1));
    EXPECT_EQ(0, src.available());

    EXPECT_NO_THROW(src.next()); // Should we expect no throw, really?

    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.get());
    EXPECT_NO_THROW(src.seek(size));
    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.seek(size+1));
    EXPECT_EQ(0, src.available());
    EXPECT_NO_THROW(src.skip(0));
    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.skip(1));
    EXPECT_EQ(0, src.available());
}

//------------------------------------------------------------------------------
TEST(fileByteSourceTest, skip)
{
    const size_t size = 10;
    auto data = std::make_shared<fake_file_reader>(size);
    file_byte_source src(data);

    EXPECT_EQ(1, src.get());
    EXPECT_EQ(size, src.available());
    EXPECT_NO_THROW(src.skip(size/2));
    EXPECT_EQ(size/2 + 1, src.get());
    EXPECT_EQ(size/2, src.available());

    EXPECT_ANY_THROW(src.skip(size/2 + 2));
    EXPECT_EQ(size/2 + 1, src.get());
    EXPECT_EQ(size/2, src.available());
}

//------------------------------------------------------------------------------
TEST(fileByteSourceTest, seek)
{
    const size_t size = 10;
    auto data = std::make_shared<fake_file_reader>(size);
    file_byte_source src(data);

    EXPECT_EQ(1, src.get());
    EXPECT_EQ(size, src.available());
    EXPECT_NO_THROW(src.seek(size/2));
    EXPECT_EQ(size/2 + 1, src.get());
    EXPECT_EQ(size/2, src.available());

    EXPECT_ANY_THROW(src.seek(size+1));
    EXPECT_EQ(size/2 + 1, src.get());
    EXPECT_EQ(size/2, src.available());
}
