
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
  } bytes;
};

#ifdef _DEBUG
static void instr_pretty_printer(const char* op, const union PruInstr instr) {
  fprintf(stdout, "{ %s 0x%x\t0x%x\t0x%x\t0x%x }\n", op, instr.bytes.b3,
          instr.bytes.b2, instr.bytes.b1, instr.bytes.b0);
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
  const unsigned op = instr.bytes.b3;

  switch (op) {
    case OP_MOV_CONST: {
      // e.g: MOV r1, 0xffffffff
      // Pick first five bits that contain the target register.
      strcpy(op_text, "MOV");
      uint32_t *dst = &registers_[instr.bytes.b0 & 0x1f];
      FieldType dst_field = (FieldType) (0x7 & (instr.bytes.b0 >> 5));

      uint32_t src = (0x00ffff00 & instruction);
      FieldType src_field = FIELDTYPE_23_8;
      pru_mov(dst, dst_field, src, src_field);
      break;
    }
    case OP_AND: {
      // Used for bitwise AND and for MOV reg, reg
      // 0b: dst, 1b: source, 2b: source, 3b:op
      strcpy(op_text, "AND");

      uint32_t *dst = &registers_[instr.bytes.b0 & 0x1f];
      FieldType dst_field = (FieldType) (0x7 & (instr.bytes.b0 >> 5));

      uint32_t arg2 = registers_[instr.bytes.b1 & 0x1f];
      FieldType arg2_field = (FieldType) (0x7 & (instr.bytes.b1 >> 5));

      uint32_t arg1;
      FieldType arg1_field;

      if (op & 0x01) {
        arg1 = instr.bytes.b2;
        arg1_field = FIELDTYPE_7_0;
      } else {
        arg1 = registers_[instr.bytes.b2 & 0x1f];
        arg1_field = (FieldType) (0x7 & (instr.bytes.b2 >> 5));
      }
      pru_and(dst, dst_field, arg1, arg1_field, arg2, arg2_field);
      break;
    }
    case OP_ADD: {
      strcpy(op_text, "ADD");
      uint32_t *dst = &registers_[instr.bytes.b0 & 0x1f];
      FieldType dst_field = (FieldType) (0x7 & (instr.bytes.b0 >> 5));

      uint32_t arg2 = registers_[instr.bytes.b1 & 0x1f];
      FieldType arg2_field = (FieldType) (0x7 & (instr.bytes.b1 >> 5));

      uint32_t arg1;
      FieldType arg1_field;

      if (op & 0x01) {
        arg1 = instr.bytes.b2;
        arg1_field = FIELDTYPE_7_0;
      } else {
        arg1 = registers_[instr.bytes.b2 & 0x1f];
        arg1_field = (FieldType) (0x7 & (instr.bytes.b2 >> 5));
      }
      pru_and(dst, dst_field, arg1, arg1_field, arg2, arg2_field);
      break;
    }
    default:
      // Failed on interpreting the instruction
      return -1;
  }

  #ifdef _DEBUG
    instr_pretty_printer(op_text, instr);
  #endif

  return 0;

}
