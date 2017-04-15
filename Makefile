
CXX=g++
CFLAGS=-g -O0

# Comme the line below to compile on ARM
export ARM_COMPILE_FLAGS?=

pru-virtual-machine: pru-virtual-machine.cc pru-binaries
	$(CXX) $(CFLAGS) -o $@ $<

pru-binaries:
	make -C pru-bin-samples all

clean:
	make -C pru-bin-samples clean
	rm -f pru-virtual-machine

.PHONY: pru-binaries
