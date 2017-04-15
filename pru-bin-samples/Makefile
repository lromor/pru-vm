

AM335_BASE:=../am335x_pru_package
PASM=$(AM335_BASE)/pru_sw/utils/pasm

# Comme the line below to compile on ARM
export ARM_COMPILE_FLAGS?=

ASSEMBLY_SOURCES=$(wildcard *.p)
PRU_BIN=$(ASSEMBLY_SOURCES:.p=_bin.h)

all: $(PRU_BIN)

%_bin.h : %.p $(PASM)
	$(PASM) -I$(CAPE_INCLUDE) -V3 -c $<

$(PASM):
	make -C $(AM335_BASE)

clean:
	rm -f $(PRU_BIN)
	# rm -f $(PASM)
	# make -C $(AM335_BASE)/pru_sw/app_loader/interface clean
