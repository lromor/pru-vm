

AM335_BASE:=../am335x_pru_package
PASM=$(AM335_BASE)/pru_sw/utils/pasm

PRU_BIN=pru-sum_bin.h

all: $(PRU_BIN)

%_bin.h : %.p $(PASM)
	$(PASM) -I$(CAPE_INCLUDE) -V3 -c $<

$(PASM):
	make -C $(AM335_BASE)

clean:
	rm -f $(PASM) $(PRU_BIN)
	make -C $(AM335_BASE)/pru_sw/app_loader/interface clean
