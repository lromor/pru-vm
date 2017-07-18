;; -*- asm -*-

.origin 0
.entrypoint INIT

INIT:
  MOV r0, 0x1234abcd

  ZERO 0, 124
  LBBO &r1, r1, r2.w0, 4
  JMP r1
  MOV r1.b0, r0.w1
  AND r1, r0, r0
  SUB r0, r1, r1
  SUB r0, r1, 0xff
  ADD r0, r1, r1
TEST:
  ADD r0, r1, 0x16
