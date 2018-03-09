#include <gtest/gtest.h>
#include "bitreader/data_source/memory_byte_source.hpp"
#include "gtest_common.hpp"

using namespace brcpp;

//------------------------------------------------------------------------------
TEST(memoryByteSourceTest, emptyCtor)
{
    memory_byte_source src;
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
TEST(memoryByteSourceTest, basic)
{
    const size_t size = 10;
    auto data = generate_test_data(size);
    memory_byte_source src(data, size);

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

    delete[] data;
}

//------------------------------------------------------------------------------
TEST(memoryByteSourceTest, skip)
{
    const size_t size = 10;
    auto data = generate_test_data(size);
    memory_byte_source src(data, size);

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
TEST(memoryByteSourceTest, seek)
{
    const size_t size = 10;
    auto data = generate_test_data(size);
    memory_byte_source src(data, size);

    EXPECT_EQ(1, src.get());
    EXPECT_EQ(size, src.available());
    EXPECT_NO_THROW(src.seek(size/2));
    EXPECT_EQ(size/2 + 1, src.get());
    EXPECT_EQ(size/2, src.available());

    EXPECT_ANY_THROW(src.seek(size+1));
    EXPECT_EQ(size/2 + 1, src.get());
    EXPECT_EQ(size/2, src.available());
}

//------------------------------------------------------------------------------
TEST(memoryByteSourceTest, clone)
{
    const size_t size = 10;
    auto data = generate_test_data(size);
    memory_byte_source src(data, size);

    EXPECT_NO_THROW(src.seek(size/2));
    auto clone = src.clone();
    EXPECT_EQ(src.position(), clone->position());
    EXPECT_EQ(src.available(), clone->available());
    EXPECT_EQ(src.get(), clone->get());
}
