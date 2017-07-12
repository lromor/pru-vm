
#include <stdint.h>

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

void imov(uint32_t *dst, FieldType dst_field, uint32_t src,
          FieldType src_field) {

  const FieldInfo dst_info = fields_info[dst_field];
  const FieldInfo src_info = fields_info[src_field];

  // Clear with unwanted bits of src
  src &= src_info.mask;

  // Shift the mask to match the target mask
  int shift = dst_info.offset - src_info.offset;
  if (shift < 0) src >>= (-1 * shift);
  else src <<= shift;

  // This time apply the destination mask
  src &= dst_info.mask;

  // Write operation
  *dst &= ~dst_info.mask; // Place zeroes on where we wish to write.
  *dst |= src;
}

void iand(uint32_t *dst, FieldType dst_field, uint32_t arg1,
          FieldType arg1_field, uint32_t arg2, FieldType arg2_field) {}

void iadd(uint32_t *dst, FieldType dst_field, uint32_t arg1,
          FieldType arg1_field, uint32_t arg2, FieldType arg2_field) {}
