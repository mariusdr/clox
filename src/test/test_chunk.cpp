#include "gtest/gtest.h"

extern "C" {
#include "../chunk.h"
}


TEST(TestChunk, InitChunk) 
{
    Chunk xs;
    initChunk(&xs);
    EXPECT_EQ(xs.count, 0);
    EXPECT_EQ(xs.capacity, 0);
}

TEST(TestChunk, FreeChunk) 
{
    Chunk xs; 
    initChunk(&xs);

    writeChunk(&xs, 123);
    writeChunk(&xs, 123);
    writeChunk(&xs, 123);
    
    freeChunk(&xs);
    EXPECT_EQ(xs.count, 0);
    EXPECT_EQ(xs.capacity, 0);
    EXPECT_TRUE(xs.code == NULL);
}

TEST(TestChunk, WriteSingle) 
{
    Chunk xs;
    initChunk(&xs);

    writeChunk(&xs, 123);
    EXPECT_EQ(xs.count, 1);
    EXPECT_EQ(xs.code[0], 123);

    freeChunk(&xs);
}

TEST(Testchunk, WriteSequence) 
{
    Chunk xs;
    initChunk(&xs);
    size_t n = 23456;
    for (int i = 0; i < n; ++i) 
    {
        writeChunk(&xs, uint8_t{i % 255});
    }
    EXPECT_EQ(xs.count, n);
    for (int i = 0; i < n; ++i) 
    {
        EXPECT_EQ(xs.code[i], i % 255);
    }
    freeChunk(&xs);
}
