;; -*- asm -*-

.origin 0
.entrypoint INIT

INIT:
  MOV r0, 0x80000000
  MOV r1, 0x80000000
  ADD r0, r0, r1
  ADC r0, r0, 1 
