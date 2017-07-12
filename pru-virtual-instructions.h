

#ifndef _PRU_VIRTUAL_INSTRUCTIONS_H
#define _PRU_VIRTUAL_INSTRUCTIONS_H

#include <stdint.h>

// 32-bit instruction set for emulating PRU ones.

#define OP_MOV_CONST                0x24

#define OP_GENERIC(x)               ((x - 1) << 1)

#define OP_ADD                      OP_GENERIC(1)
#define OP_AND                      OP_GENERIC(9)

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


void imov(uint32_t *dst, FieldType dst_field, uint32_t src,
          FieldType src_field);

void iand(uint32_t *dst, FieldType dst_field, uint32_t arg1,
          FieldType arg1_field, uint32_t arg2, FieldType arg2_field);

void iadd(uint32_t *dst, FieldType dst_field, uint32_t arg1,
          FieldType arg1_field, uint32_t arg2, FieldType arg2_field);


#endif // _PRU_VIRTUAL_INSTRUCTIONS_H
