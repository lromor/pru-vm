# pru-vm
PRU-ICSS virtual machine

# NOTES


## Binary output

 oooo oooo vvvv vvvv vvvv vvvv fffr rrrr

 where

 * o: opcode
 * v: argument value (0x12ab)
 * f: field (.b1, .b2)
 * r: register (r29)

when it needs more than 32 bits the field argument is obviously greeter

# OPCODES

if ARGTYPE_REGISTER, so it's like ADD r1, r1, r2
then the 24th bit is 0
else is 1


if( inst.Arg[2].Type == ARGTYPE_REGISTER )
{
    opcode |= inst.Arg[2].Value << 16;
    opcode |= inst.Arg[2].Field << 21;
    printf("opcode ARGTYPE REGISTER!: %x\n", opcode);
}
else
{
    opcode |= inst.Arg[2].Value << 16;
    opcode |= 1<<24;
    printf("opcode PORCO DIO: %x\n", opcode);
}


if it's not value but register then

/* Code as an AND */
inst.Op = OP_AND;
inst.Arg[2] = inst.Arg[1];
goto CODE_ARITHMETIC;
