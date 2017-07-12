
#include <stdio.h>
#include <stdint.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "pru-virtual-instructions.h"

TEST(OpCodes, mov) {

  uint32_t dst = 0;
  const uint32_t src = 0x1234abcd;

  pru_mov(&dst, FIELDTYPE_7_0, src, FIELDTYPE_31_16);
  ASSERT_EQ(dst, 0x34);
}

TEST(OpCodes, and) {

  uint32_t dst = 0;
  const uint32_t src1 = 0x1234abcd;
  const uint32_t src2 = 0xf0f00f0f;

  pru_and(&dst, FIELDTYPE_31_24, src1, FIELDTYPE_31_16, src2, FIELDTYPE_15_0);
  ASSERT_EQ(dst, (0x1234 & 0x00f) << 24);
}

TEST(OpCodes, add) {

  uint32_t dst = 0;
  const uint32_t src1 = 0x000f0000;
  const uint32_t src2 = 0x00000001;

  pru_add(&dst, FIELDTYPE_31_24, src1, FIELDTYPE_31_16, src2, FIELDTYPE_15_0);
  ASSERT_EQ(dst, 0x10000000);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
