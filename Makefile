
CPPC=g++
CFLAGS=-g -O0 -D_DEBUG

INCDIR_APP_LOADER?=am335x_pru_package/pru_sw/app_loader/include

GTEST_SOURCE?=/usr/src/gtest
GMOCK_SOURCE?=/usr/src/gmock

OBJECTS=pru-virtual-machine.o pru-virtual-instructions.o
MAIN_OBJECTS=vm.o
TEST_FRAMEWORK_OBJECTS=gtest-all.o gmock-all.o
BINARIES=vm virtual-prussdrv.a
UNITTEST_BINARIES=pru-virtual-machine_test pru-virtual-instructions_test

CFLAGS+=-Wall -Werror -I$(INCDIR_APP_LOADER)

DEPENDENCY_RULES=$(OBJECTS:=.d) $(MAIN_OBJECTS:=.d) $(UNITTEST_BINARIES:=.o.d)

all: vm pru-binaries

test: $(UNITTEST_BINARIES)
	for test_bin in $(UNITTEST_BINARIES) ; do echo ; echo $$test_bin; ./$$test_bin || exit 1 ; done

virtual-prussdrv.a: virtual-prussdrv.o pru-virtual-machine.o
	ar rvs $@ $^

vm: vm.cc $(OBJECTS)
	$(CPPC) $(CFLAGS) -o $@ $< $(OBJECTS)

%_test: %_test.o $(OBJECTS) $(TEST_FRAMEWORK_OBJECTS)
	$(CPPC) -lpthread -o $@ $< $(OBJECTS) $(TEST_FRAMEWORK_OBJECTS)

%.o: %.cc
	$(CPPC) $(CFLAGS) -c $< -o $@
	@$(CPPC) $(CFLAGS) -MM $< > $@.d

%_test.o: %_test.cc
	$(CPPC)  $(GTEST_INCLUDE) $(CFLAGS) -c  $< -o $@
	@$(CPPC) $(GTEST_INCLUDE) $(CFLAGS) -MM $< > $@.d

-include $(DEPENDENCY_RULES)

# Test and Mock framework.
gtest-all.o : $(GTEST_SOURCE)/src/gtest-all.cc
	$(CPPC) $(CFLAGS) $(GTEST_INCLUDE) -I$(GTEST_SOURCE) -I$(GTEST_SOURCE)/include -c  $< -o $@

gmock-all.o : $(GMOCK_SOURCE)/src/gmock-all.cc
	$(CPPC) $(CFLAGS) $(GTEST_INCLUDE) -I$(GMOCK_SOURCE) -I$(GMOCK_SOURCE)/include -c  $< -o $@

pru-binaries:
	make -C pru-bin-samples all

clean:
	make -C pru-bin-samples clean
	rm -rf $(BINARIES) $(UNITTEST_BINARIES) *.o *.d

.PHONY: pru-binaries
