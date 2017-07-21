
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
  ASSERT_EQ(0x000034ab, vm.ReadRegister(1));

  ASSERT_EQ(vm.IsRunning(), false);
}

// Test add and adc with carry
TEST(OpCodes, add_and_carry) {
  const uint32_t code[] = {
    0x248000c0, 0x24000080, // MOV r0, 0x80000000
    0x248000c1, 0x24000081, // MOV r1, 0x80000000
    0x00e1e0e0, // ADD r0, r0, r1
    0x0301e0e0, // ADC r0, r0, 1
  };

  PruVirtualMachine vm = PruVirtualMachine();
  vm.Load(code, sizeof(code));
  vm.Run();

  vm.Next();
  ASSERT_EQ(0x80000000, vm.ReadRegister(0));

  vm.Next();
  ASSERT_EQ(0x80000000, vm.ReadRegister(0));

  vm.Next();
  ASSERT_EQ(0x80000000, vm.ReadRegister(1));

  vm.Next();
  ASSERT_EQ(0x80000000, vm.ReadRegister(1));

  vm.Next();
  ASSERT_EQ(0x00000000, vm.ReadRegister(0));
  vm.Next();
  ASSERT_EQ(0x00000002, vm.ReadRegister(0));

}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
