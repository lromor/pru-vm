# PRU-VM
PRU-ICSS virtual machine

Usage:

    make all
    ./vm pru-firmware.bin

# NOTES

OPCODES Implemented:
* MOV
* AND


# CREDITS

Due to the lack of PRU op codes documentation I found really useful
reading the [pasm sources](https://github.com/beagleboard/am335x_pru_package/blob/master/pru_sw/utils/pasm_source/pasmop.c)
and [prudebug](https://github.com/poopgiggle/prudebug/blob/master/da.c) sources.
