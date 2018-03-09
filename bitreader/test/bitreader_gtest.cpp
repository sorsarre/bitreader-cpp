#include <gtest/gtest.h>
#include <bitreader/data_source/memory_byte_source.hpp>
#include "bitreader/bitreader.hpp"

using namespace brcpp;

using source_t = memory_byte_source;

//------------------------------------------------------------------------------
TEST(bitreaderTest, setData)
{
    const uint8_t data[] = {0xFF, 0x11, 0x22, 0x33};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0, br.position());
    EXPECT_EQ(32, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_aligned_nonfull)
{
    const uint8_t data[] = {0xFF, 0x11, 0x22, 0x33};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0xFF, br.read<uint8_t>(8));
    EXPECT_EQ(8, br.position());
    EXPECT_EQ(24, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_aligned_full)
{
    const uint8_t data[] = {0xFF, 0x11, 0x22, 0x33};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0xFF112233, br.read<uint32_t>(32));
    EXPECT_EQ(32, br.position());
    EXPECT_EQ(0, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_nonaligned_nonfull)
{
    const uint8_t data[] = {0xFF, 0x11, 0x22, 0x33};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0xF, br.read<uint8_t>(4));
    EXPECT_EQ(4, br.position());
    EXPECT_EQ(28, br.available());
    EXPECT_EQ(0xF11223, br.read<uint32_t>(24));
    EXPECT_EQ(28, br.position());
    EXPECT_EQ(4, br.available());
    EXPECT_EQ(0x3, br.read<uint16_t>(4));
    EXPECT_EQ(32, br.position());
    EXPECT_EQ(0, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_aligned_64)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0x1122334455667788, br.read<uint64_t>(64));
    EXPECT_EQ(64, br.position());
    EXPECT_EQ(8, br.available());
    EXPECT_EQ(0x99, br.read<uint8_t>(8));
    EXPECT_EQ(72, br.position());
    EXPECT_EQ(0, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_aligned_64_cross)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0x11, br.read<uint8_t>(8));
    EXPECT_EQ(8, br.position());
    EXPECT_EQ(64, br.available());
    EXPECT_EQ(0x2233445566778899, br.read<uint64_t>(64));
    EXPECT_EQ(72, br.position());
    EXPECT_EQ(0, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_nonaligned_64_cross)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0x01, br.read<uint8_t>(4));
    EXPECT_EQ(4, br.position());
    EXPECT_EQ(68, br.available());
    EXPECT_EQ(0x1223344556677889, br.read<uint64_t>(64));
    EXPECT_EQ(68, br.position());
    EXPECT_EQ(4, br.available());
    EXPECT_EQ(0x09, br.read<uint16_t>(4));
    EXPECT_EQ(72, br.position());
    EXPECT_EQ(0, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_signed_aligned)
{
    const uint8_t data[] = {0xFE, 0x3F};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(-2, br.read<int8_t>(8));
    EXPECT_EQ(0x3F, br.read<int8_t>(8));
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_signed_nonaligned)
{
    const uint8_t data[] = {0xFE, 0x3F};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(10));
    EXPECT_EQ(-1, br.read<int8_t>(6));
    EXPECT_EQ(16, br.position());
    EXPECT_EQ(0, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, skip_non_cross_aligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(3*8));
    EXPECT_EQ(24, br.position());
    EXPECT_EQ(48, br.available());
    EXPECT_EQ(0x445, br.read<uint16_t>(12));
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, skip_non_cross_unaligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(7));
    EXPECT_EQ(7, br.position());
    EXPECT_EQ(33, br.available());
    EXPECT_EQ(0x122, br.peek<uint16_t>(9));
    EXPECT_EQ(7, br.position());
    EXPECT_EQ(33, br.available());
    EXPECT_NO_THROW(br.skip(21));
    EXPECT_EQ(28, br.position());
    EXPECT_EQ(12, br.available());
    EXPECT_EQ(0x455, br.read<uint16_t>(12));
    EXPECT_EQ(40, br.position());
    EXPECT_EQ(0, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, skip_cross_aligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(72));
    EXPECT_EQ(72, br.position());
    EXPECT_EQ(8, br.available());
    EXPECT_EQ(0xAA, br.read<uint8_t>(8));
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, skip_cross_unaligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(74));
    EXPECT_EQ(74, br.position());
    EXPECT_EQ(6, br.available());
    EXPECT_EQ(0b10'1010, br.read<uint8_t>(6));
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, peek_non_cross_aligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0x1122, br.peek<uint16_t>(16));
    EXPECT_EQ(0, br.position());
    EXPECT_EQ(0x1122, br.read<uint16_t>(16));
    EXPECT_EQ(16, br.position());
    EXPECT_EQ(0x3344, br.peek<uint16_t>(16));
    EXPECT_EQ(16, br.position());
    EXPECT_EQ(0x3344, br.read<uint16_t>(16));
    EXPECT_EQ(32, br.position());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, peek_non_cross_unaligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0x112, br.peek<uint16_t>(12));
    EXPECT_EQ(0, br.position());
    EXPECT_EQ(0x112, br.read<uint16_t>(12));
    EXPECT_EQ(12, br.position());
    EXPECT_EQ(0x233, br.peek<uint16_t>(12));
    EXPECT_EQ(12, br.position());
    EXPECT_EQ(0x23344, br.read<uint32_t>(20));
    EXPECT_EQ(32, br.position());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, peek_cross_aligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(48));
    EXPECT_EQ(48, br.position());
    EXPECT_EQ(0x778899, br.peek<uint32_t>(24));
    EXPECT_EQ(48, br.position());
    EXPECT_EQ(0x778899AA, br.read<uint32_t>(32));
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, peek_cross_unaligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(52));
    EXPECT_EQ(52, br.position());
    EXPECT_EQ(0x78899A, br.peek<uint32_t>(24));
    EXPECT_EQ(52, br.position());
    EXPECT_EQ(0x78899AA, br.read<uint32_t>(28));
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, align_lt_byte_aligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.align(5));
    EXPECT_EQ(0, br.position());
    EXPECT_NO_THROW(br.skip(10));
    EXPECT_EQ(10, br.position());
    EXPECT_NO_THROW(br.align(5));
    EXPECT_EQ(10, br.position());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, align_lt_byte_unaligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(2));
    EXPECT_EQ(2, br.position());
    EXPECT_NO_THROW(br.align(5));
    EXPECT_EQ(5, br.position());
}
