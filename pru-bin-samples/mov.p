;; -*- asm -*-

.origin 0
.entrypoint INIT

INIT:
  MOV r0, 0x1234abcd
  MOV r1, r0
  AND r1, r0, r0
  SUB r0, r1, r1
  SUB r0, r1, 0xff
  ADD r0, r1, r1
  ADD r0, r1, 0x16
