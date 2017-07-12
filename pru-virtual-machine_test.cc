
#include <stdio.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "pru-virtual-machine.h"

// MOV immediate value
TEST(OpCodes, mov_imm) {
  const uint32_t code[] = {
     0x241234c0, 0x24abcd80, // MOV r0, 0x1234abcd
     0x10a0a0e1 // MOV r1, r0.w1 (== AND r1, r0.w1, r0.w1)
  };

  PruVirtualMachine vm = PruVirtualMachine();
  vm.Load(code, sizeof(code));
  vm.Run();

  ASSERT_EQ(vm.IsRunning(), true);

  vm.Next();
  ASSERT_EQ(0x12340000, vm.ReadRegister(0));

  vm.Next();
  ASSERT_EQ(0x1234abcd, vm.ReadRegister(0));

  vm.Next();
  ASSERT_EQ(0x1234abcd, vm.ReadRegister(0));
  ASSERT_EQ(0x0034ab00, vm.ReadRegister(1));

  ASSERT_EQ(vm.IsRunning(), false);
}


int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
