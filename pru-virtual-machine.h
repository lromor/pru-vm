
#ifndef _PRU_VIRTUAL_MACHINE_H
#define _PRU_VIRTUAL_MACHINE_H

#include <stdint.h>

class PruVirtualMachine {
public:
  PruVirtualMachine() {}

  // Load the binary file
  void LoadBinary() {}

  // Crunch next instruction
  void Next() {}

  // Get Pru register value
  uint32_t GetRegisterValue(short reg) { return 0; }

private:
  uint32_t pru_dram_[2048];
};

#endif // _PRU_VIRTUAL_MACHINE_H
