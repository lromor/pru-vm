
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "pru-virtual-machine.h"
#include "pru-virtual-instructions.h"

void GetBurstLen(char *tempstr, unsigned int BurstLen)
{
  if (BurstLen < 124) {
    sprintf(tempstr, "%u", BurstLen+1);
  } else if (BurstLen == 124) {
     sprintf(tempstr, "b0");
  } else if (BurstLen == 125) {
    sprintf(tempstr, "b1");
  } else if (BurstLen == 126) {
    sprintf(tempstr, "b2");
  } else if (BurstLen == 127) {
    sprintf(tempstr, "b3");
  } else {
    sprintf(tempstr, "XX");
  }
}

static const char *f1_inst[] = {
  "ADD", "ADC", "SUB", "SUC", "LSL", "LSR", "RSB", "RSC", "AND", "OR", "XOR",
  "NOT", "MIN", "MAX", "CLR", "SET"
};

static const char *f2_inst[] = {
  "JMP", "JAL", "LDI", "LMBD", "SCAN", "HALT", "RESERVED", "RESERVED",
  "RESERVED", "RESERVED", "RESERVED", "RESERVED", "RESERVED", "RESERVED",
  "RESERVED", "SLP"
};

static const char *f4_inst[] = {
  "xx", "LT", "EQ", "LE", "GT", "NE", "GE", "A"
};

static const char  *f5_inst[] = {
  "xx", "BC", "BS", "xx"
};

static const char  *f6_7_inst[] = { "SBBO", "LBBO" };

static const char  *f6_4_inst[] = { "SBCO", "LBCO" };

static const char  *sis[] = {
  ".b0", ".b1", ".b2", ".b3", ".w0", ".w1", ".w2", ""
};

static const char*  bytenum[] = {"", ".b1", ".b2", ".b3"};

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

union PruInstr {
  uint32_t value;
  struct {
    unsigned b0 : 8;
    unsigned b1 : 8;
    unsigned b2 : 8;
    unsigned b3 : 8;
  } bytes;
};

int PruVirtualMachine::ExecuteInstruction(uint32_t instr) {

  unsigned char op;
  unsigned short imm;
  unsigned char imm2;
  unsigned char aluop, rs2_field, rs2, rs1_field, rs1, rd_field, rd, io;
  unsigned char loadstore, burstlen, rxbyteaddr, rx, ro, ro_field, rb;
  unsigned char subop, test;
  short broff;
  char tempstr[50];
  char str[100];

  op = (instr & 0xE0000000) >> 29;

  switch (op) {
    case 0: // format 1
      aluop = (instr & 0x1E000000) >> 25;
      io = (instr & 0x01000000) >> 24;
      rs1_field = (instr & 0x0000E000) >> 13;
      rs1 = (instr & 0x00001F00) >> 8;
      rd_field = (instr & 0x000000E0) >> 5;
      rd = (instr & 0x0000001F);
      if (io) {
          imm2 = (instr & 0x00FF0000) >> 16;
          sprintf(str, "%s R%u%s, R%u%s, 0x%02x", f1_inst[aluop], rd,
                  sis[rd_field], rs1, sis[rs1_field], imm2);
      } else {
          rs2_field = (instr & 0x00E00000) >> 21;
          rs2 = (instr & 0x001F0000) >> 16;
          sprintf(str, "%s R%u%s, R%u%s, R%u%s", f1_inst[aluop], rd,
                  sis[rd_field], rs1, sis[rs1_field], rs2, sis[rs2_field]);
      }
      break;
    case 1: // format 2
      subop = (instr & 0x1E000000) >> 25;
      switch (subop) {
        case 0: // JMP & JAL
        case 1:
          io = (instr & 0x01000000) >> 24;
          rd_field = (instr & 0x000000E0) >> 5;
          rd = (instr & 0x0000001F);
          if (io) {
            imm = (instr & 0x00FFFF00) >> 8;
            if (subop == 0)
              sprintf(str, "%s 0x%04x", f2_inst[subop], imm);
            else
              sprintf(str, "%s R%u%s, 0x%04x", f2_inst[subop],
                      rd, sis[rd_field], imm);
          } else {
            rs2_field = (instr & 0x00E00000) >> 21;
            rs2 = (instr & 0x001F0000) >> 16;
            if (subop == 0)
              sprintf(str, "%s R%u%s", f2_inst[subop], rs2,
                      sis[rs2_field]);
            else
              sprintf(str, "%s R%u%s, R%u%s", f2_inst[subop], rd,
                      sis[rd_field], rs2, sis[rs2_field]);
          }
          break;
        case 2:  // LDI
          imm = (instr & 0x00FFFF00) >> 8;
          rd_field = (instr & 0x000000E0) >> 5;
          rd = (instr & 0x0000001F);
          sprintf(str, "%s R%u%s, 0x%04x", f2_inst[subop], rd,
                  sis[rd_field], imm);
          break;
        case 3:  // LMBD
          io = (instr & 0x01000000) >> 24;
          rs1_field = (instr & 0x0000E000) >> 13;
          rs1 = (instr & 0x00001F00) >> 8;
          rd_field = (instr & 0x000000E0) >> 5;
          rd = (instr & 0x0000001F);
          rs2_field = (instr & 0x00E00000) >> 21;
          rs2 = (instr & 0x001F0000) >> 16;
          imm2 = (instr & 0x00FF0000) >> 16;

          if (io) {
            sprintf(str, "%s R%u%s, R%u%s, 0x%04x", f2_inst[subop], rd,
                    sis[rd_field], rs1, sis[rs1_field], imm2);
          } else {
            sprintf(str, "%s R%u%s, R%u%s, R%u%s", f2_inst[subop], rd,
                    sis[rd_field], rs1, sis[rs1_field], rs2, sis[rs2_field]);
          }
          break;
        case 4:  // SCAN
          // let's not support this for now.
          return -1;
        case 5:  // HALT
          sprintf(str, "%s", f2_inst[subop]);
          break;
        case 15:  // SLP
          imm = (instr & 0x00800000) >> 23;
          sprintf(str, "%s %u", f2_inst[subop], imm);
          break;
        default:
          sprintf(str, "UNKNOWN-F2");
          break;
        }
        break;
    case 2:  // Format 4a & 4b - Quick Arithmetic Test and Branch
    case 3:
      test = (instr & 0x38000000) >> 27;
      io = (instr & 0x01000000) >> 24;
      rs2_field = (instr & 0x00E00000) >> 21;
      rs2 = (instr & 0x001F0000) >> 16;
      rs1_field = (instr & 0x0000E000) >> 13;
      rs1 = (instr & 0x00001F00) >> 8;
      imm = (instr & 0x00FF0000) >> 16;
      broff = ((instr & 0x06000000) >> 17) | (instr & 0x000000FF);
      if (broff & 0x0200) broff |= 0xFC00;

      if (test == 7) {
        sprintf(str, "QBA %d", broff);
      } else {
        if (io) {
          sprintf(str, "QB%s %d, R%u%s, %u", f4_inst[test], broff, rs1,
                  sis[rs1_field], imm);
        } else {
          sprintf(str, "QB%s %d, R%u%s, R%u%s", f4_inst[test], broff,
                  rs1, sis[rs1_field], rs2, sis[rs2_field]);
        }
      }
      break;
    case 6:  // Format 5 - Quick bit test and banch instructions
      test = (instr & 0x18000000) >> 27;
      io = (instr & 0x01000000) >> 24;
      rs2_field = (instr & 0x00E00000) >> 21;
      rs2 = (instr & 0x001F0000) >> 16;
      rs1_field = (instr & 0x0000E000) >> 13;
      rs1 = (instr & 0x00001F00) >> 8;
      imm = (instr & 0x001F0000) >> 16;
      broff = ((instr & 0x06000000) >> 17) | (instr & 0x000000FF);
      if (broff & 0x0200) broff |= 0xFC00;

      if (io) {
        sprintf(str, "QB%s %d, R%u%s, %u", f5_inst[test], broff, rs1,
                sis[rs1_field], imm);
      } else {
        sprintf(str, "QB%s %d, R%u%s, R%u%s", f5_inst[test], broff, rs1,
                sis[rs1_field], rs2, sis[rs2_field]);
      }
      break;
    case 4:
    case 7:  // Format 6 - LBBO/SBBO/LBCO/SBCO instructions
      loadstore = (instr & 0x10000000) >> 28;
      burstlen = ((instr & 0x0E000000) >> 21) |
        ((instr & 0x0000E000) >> 12) |
        ((instr & 0x00000080) >> 7);

      io = (instr & 0x01000000) >> 24;
      rxbyteaddr = (instr & 0x00000060) >> 5;
      rx = (instr & 0x0000001F);
      ro_field = (instr & 0x00E00000) >> 21;
      ro = (instr & 0x001F0000) >> 16;
      rb = (instr & 0x00001F00) >> 8;
      imm = (instr & 0x00FF0000) >> 16;
      GetBurstLen(tempstr, burstlen);

      if (op == 7) {
        if (io) {
          sprintf(str, "%s R%u%s, R%u, %u, %s", f6_7_inst[loadstore],
                  rx, bytenum[rxbyteaddr], rb, imm, tempstr);
        } else {
          sprintf(str, "%s R%u%s, R%u, R%u%s, %s", f6_7_inst[loadstore],
                  rx, bytenum[rxbyteaddr], rb, ro, sis[ro_field],
                  tempstr);
        }
      } else {  // OP==4
        if (io) {
          sprintf(str, "%s R%u%s, C%u, %u, %s", f6_4_inst[loadstore],
                  rx, bytenum[rxbyteaddr], rb, imm, tempstr);
        } else {
          sprintf(str, "%s R%u%s, C%u, R%u%s, %s", f6_4_inst[loadstore],
                  rx, bytenum[rxbyteaddr], rb, ro, sis[ro_field], tempstr);
        }
      }
      break;
    default:
      return -1;
  }

#ifdef _DEBUG
  fprintf(stdout, "%s\n", str);
#endif
  return 0;
}
