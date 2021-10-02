#include "gtest/gtest.h"

extern "C" {
#include "../value.h"
}

TEST(TestValueArray, InitValueArray) {
  ValueArray xs;
  initValueArray(&xs);
  EXPECT_EQ(xs.count, 0);
  EXPECT_EQ(xs.capacity, 0);
}

TEST(TestValueArray, FreeValueArray) {
  ValueArray xs;
  initValueArray(&xs);

  writeValueArray(&xs, 123);
  writeValueArray(&xs, 123);
  writeValueArray(&xs, 123);

  freeValueArray(&xs);
  EXPECT_EQ(xs.count, 0);
  EXPECT_EQ(xs.capacity, 0);
  EXPECT_TRUE(xs.values == NULL);
}

TEST(TestValueArray, WriteSingle) {
  ValueArray xs;
  initValueArray(&xs);

  writeValueArray(&xs, 123);
  EXPECT_EQ(xs.count, 1);
  EXPECT_EQ(xs.values[0], 123);

  freeValueArray(&xs);
}

TEST(TestValueArray, WriteSequence) {
  ValueArray xs;
  initValueArray(&xs);
  size_t n = 23456;
  for (int i = 0; i < n; ++i) {
    writeValueArray(&xs, uint8_t{i % 255});
  }
  EXPECT_EQ(xs.count, n);
  for (int i = 0; i < n; ++i) {
    EXPECT_EQ(xs.values[i], i % 255);
  }
  freeValueArray(&xs);
}
