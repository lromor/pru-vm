

#ifndef _PRU_VIRTUAL_INSTRUCTIONS_H
#define _PRU_VIRTUAL_INSTRUCTIONS_H

#include <stdint.h>

// 32-bit instruction set for emulating PRU ones.

#define OP_MOV_CONST                0x24


// If the 8th MSB bit is 0 the arg type is register, else immediate.
#define OP_GENERIC(x)               ((x - 1) << 1)

#define OP_ADD                      OP_GENERIC(1)
#define OP_AND                      OP_GENERIC(9)
#define OP_JMP                      0x20

// #define OP_ADC                      2
// #define OP_SUB                      3
// #define OP_SUC                      4
// #define OP_LSL                      5
// #define OP_LSR                      6
// #define OP_RSB                      7
// #define OP_RSC                      8
// #define OP_OR                       10
// #define OP_XOR                      11
// #define OP_NOT                      12
// #define OP_MIN                      13
// #define OP_MAX                      14
// #define OP_CLR                      15
// #define OP_SET                      16
// #define OP_LDI                      17
// #define OP_LBBO                     18
// #define OP_LBCO                     19
// #define OP_SBBO                     20
// #define OP_SBCO                     21
// #define OP_LFC                      22
// #define OP_STC                      23
// #define OP_JAL                      24
// #define OP_JMP                      25
// #define OP_QBGT                     26
// #define OP_QBLT                     27
// #define OP_QBEQ                     28
// #define OP_QBGE                     29
// #define OP_QBLE                     30
// #define OP_QBNE                     31
// #define OP_QBA                      32
// #define OP_QBBS                     33
// #define OP_QBBC                     34
// #define OP_LMBD                     35
// #define OP_CALL                     36
// #define OP_WBC                      37
// #define OP_WBS                      38
// #define OP_MOV                      39
// #define OP_MVIB                     40
// #define OP_MVIW                     41
// #define OP_MVID                     42
// #define OP_SCAN                     43
// #define OP_HALT                     44
// #define OP_SLP                      45
// #define OP_RET                      46
// #define OP_ZERO                     47
// #define OP_FILL                     48
// #define OP_XIN                      49
// #define OP_XOUT                     50
// #define OP_XCHG                     51
// #define OP_SXIN                     52
// #define OP_SXOUT                    53
// #define OP_SXCHG                    54
// #define OP_LOOP                     55
// #define OP_ILOOP                    56
// #define OP_NOP0                     57
// #define OP_NOP1                     58
// #define OP_NOP2                     59
// #define OP_NOP3                     60
// #define OP_NOP4                     61
// #define OP_NOP5                     62
// #define OP_NOP6                     63
// #define OP_NOP7                     64
// #define OP_NOP8                     65
// #define OP_NOP9                     66
// #define OP_NOPA                     67
// #define OP_NOPB                     68
// #define OP_NOPC                     69
// #define OP_NOPD                     70
// #define OP_NOPE                     71
// #define OP_NOPF                     72
// #define OP_MAXIDX                   72

typedef enum {
  FIELDTYPE_7_0,
  FIELDTYPE_15_8,
  FIELDTYPE_23_16,
  FIELDTYPE_31_24,
  FIELDTYPE_15_0,
  FIELDTYPE_23_8,
  FIELDTYPE_31_16,
  FIELDTYPE_31_0
} FieldType;


void pru_mov(uint32_t *dst, FieldType dst_field, uint32_t src,
          FieldType src_field);

void pru_and(uint32_t *dst, FieldType dst_field, uint32_t arg1,
          FieldType arg1_field, uint32_t arg2, FieldType arg2_field);

void pru_add(uint32_t *dst, FieldType dst_field, uint32_t arg1,
          FieldType arg1_field, uint32_t arg2, FieldType arg2_field);


#endif // _PRU_VIRTUAL_INSTRUCTIONS_H
