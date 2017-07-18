
#ifndef _PRU_VIRTUAL_MACHINE_H
#define _PRU_VIRTUAL_MACHINE_H

#include <stdint.h>

class PruVirtualMachine {
public:
  PruVirtualMachine();

  // Load the binary file
  void Load(const void *instructions, int size);

  // Crunch next instruction
  void Next();

  // Turns on the PRU
  void Run(int start = 0x0);

  // Tells if the PRU has been started and if has new instructions to execute.
  bool IsRunning();

  // Get Pru register value
  uint32_t ReadRegister(int reg);

  // Set Pru register value
  void WriteRegister(int reg, uint32_t value);

  long GetCycles();

private:

  int ExecuteInstruction(uint32_t instr);

  void exe_format_1();

  // Pru is 32-bit
  uint32_t *iram_;

  // Number of instructions
  int isize_;

  // 0 - 31 PRU registers
  uint32_t registers_[32];

  bool is_running_;

  // Instruction pointer
  int ip_;

  // Number of execution cycles from the start
  long cycles_;
};

#endif // _PRU_VIRTUAL_MACHINE_H
