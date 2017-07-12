
#include <stdint.h>
#include <stdio.h>
#include "pru-virtual-instructions.h"

typedef struct {
  int offset; // field offset in byte
  int size; // field size in byte
  uint32_t mask; // Bit mask
} FieldInfo;


static const FieldInfo fields_info[] {
  { 0, 1, 0x000000ff },
  { 1, 1, 0x0000ff00 },
  { 2, 1, 0x00ff0000 },
  { 3, 1, 0xff000000 },
  { 0, 2, 0x0000ffff },
  { 1, 2, 0x00ffff00 },
  { 2, 2, 0xffff0000 },
  { 0, 4, 0xffffffff }
};

inline void clear_and_shift(uint32_t *value, FieldInfo field) {
  // Clear unwanted bits
  *value &= field.mask;

  // Shift to LSB
  *value >>= (field.offset * 8);
}

void pru_mov(uint32_t *dst, FieldType dst_field, uint32_t src,
          FieldType src_field) {

  const FieldInfo dst_info = fields_info[dst_field];
  const FieldInfo src_info = fields_info[src_field];

  // Clear with unwanted bits of src
  src &= src_info.mask;

  // Shift the mask to match the target mask
  int shift = (dst_info.offset - src_info.offset) * 8;
  if (shift < 0) src >>= (-1 * shift);
  else src <<= shift;

  // This time apply the destination mask
  src &= dst_info.mask;

  // Write operation
  *dst |= src;
}

void pru_and(uint32_t *dst, FieldType dst_field, uint32_t arg1,
          FieldType arg1_field, uint32_t arg2, FieldType arg2_field) {

  const FieldInfo arg1_info = fields_info[arg1_field];
  const FieldInfo arg2_info = fields_info[arg2_field];

  // Get enum with biggest field
  FieldType widest_field = \
    (arg1_info.size > arg2_info.size) ? arg1_field : arg2_field;

  FieldInfo widest = fields_info[widest_field];

  clear_and_shift(&arg1, arg1_info);
  clear_and_shift(&arg2, arg2_info);

  // And
  arg1 &= arg2;
  // Revert back to the widest one position
  arg1 <<= (widest.offset * 8);
  pru_mov(dst, dst_field, arg1, widest_field);
}

void pru_add(uint32_t *dst, FieldType dst_field, uint32_t arg1,
          FieldType arg1_field, uint32_t arg2, FieldType arg2_field) {

  const FieldInfo arg1_info = fields_info[arg1_field];
  const FieldInfo arg2_info = fields_info[arg2_field];

  // Get enum with biggest field
  FieldType widest_field = \
    (arg1_info.size > arg2_info.size) ? arg1_field : arg2_field;

  FieldInfo widest = fields_info[widest_field];

  clear_and_shift(&arg1, arg1_info);
  clear_and_shift(&arg2, arg2_info);

  arg1 += arg2;
  arg1 <<= (widest.offset * 8);
  pru_mov(dst, dst_field, arg1, widest_field);
}
