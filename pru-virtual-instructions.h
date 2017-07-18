

#ifndef _PRU_VIRTUAL_INSTRUCTIONS_H
#define _PRU_VIRTUAL_INSTRUCTIONS_H

#include <stdint.h>

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

void pru_ldi(uint32_t *dst, int dst_field, uint32_t src, int src_field);

void pru_and(uint32_t *dst, int dst_field, uint32_t arg1, int arg1_field,
             uint32_t arg2, int arg2_field);

void pru_add(uint32_t *dst, int dst_field, uint32_t arg1, int arg1_field,
             uint32_t arg2, int arg2_field);

#endif // _PRU_VIRTUAL_INSTRUCTIONS_H
