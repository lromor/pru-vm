
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "pru-virtual-machine.h"
#include "pru-virtual-instructions.h"

union PruInstr {
  uint32_t value;
  struct {
    unsigned b0 : 8;
    unsigned b1 : 8;
    unsigned b2 : 8;
    unsigned b3 : 8;
  } fields;
};

#ifdef _DEBUG
static void instr_pretty_printer(const char* op, const union PruInstr instr) {
  fprintf(stdout, "{ %s 0x%x\t0x%x\t0x%x\t0x%x }\n", op, instr.fields.b3,
          instr.fields.b2, instr.fields.b1, instr.fields.b0);
}
#endif

PruVirtualMachine::PruVirtualMachine() : iram_(NULL), registers_{0},
                                         is_running_(false), ip_(0) {}

// This may be useful to start the execution at a certain address with some
// specific conditions
void PruVirtualMachine::Run() { is_running_ = true; }

void PruVirtualMachine::Next() {
  assert(iram_ != NULL);

  // Pick n instructions associated with a single opcode
  const uint32_t instr =  ((uint32_t *) iram_)[ip_];

  // Map it back to PRU_INST
  // PRU_INST instruction;
  ExecuteInstruction(instr);

  // Increase the ip
  ip_++;

  if (ip_ >= isize_) is_running_ = false;

}


void PruVirtualMachine::Load(const void *instructions, int size) {
  iram_ = (uint32_t *) instructions;
  isize_ = size / sizeof(uint32_t);
}

bool PruVirtualMachine::IsRunning() {
  return is_running_;
}

uint32_t PruVirtualMachine::ReadRegister(int reg) {
  return registers_[reg];
}

int PruVirtualMachine::ExecuteInstruction(uint32_t instruction) {
  union PruInstr instr;
  instr.value = instruction;

  char op_text[10];

  switch (instr.fields.b3) {
    case OP_MOV_CONST: {
      // e.g: MOV r1, 0xffffffff
      // Pick first five bits that contain the target register.
      strcpy(op_text, "MOV");
      uint32_t *dst = &registers_[instr.fields.b0 & 0x1f];
      FieldType dst_field = (FieldType) (0x7 & (instr.fields.b0 >> 5));

      uint32_t src = (0x00ffff00 & instruction);
      FieldType src_field = FIELDTYPE_23_8;
      imov(dst, dst_field, src, src_field);
      break;
    }
    case OP_AND: {
      // Used for bitwise AND and for MOV reg, reg
      // 0b: dst, 1b: source, 2b: source, 3b:op
      strcpy(op_text, "AND");
      break;
    }
    case OP_ADD:
      strcpy(op_text, "ADD");
      break;
    default:
      // Failed on interpreting the instruction
      return -1;
  }

  #ifdef _DEBUG
    instr_pretty_printer(op_text, instr);
  #endif

  return 0;

}
