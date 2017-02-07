# Compiler.
CC := gcc
CXX := g++
FC := gfortran
# Compiler flags.
FFLAGS := -cpp
CXXFLAGS := -lgfortran -lboost_system-mt -lboost_filesystem-mt -L./ -lmatio -O3
# Check version of gcc.
GCCVERSIONEQ5 := $(shell expr `gcc -dumpversion | cut -f1-2 -d.` \>= 5.0)
GCCVERSIONEQ6 := $(shell expr `gcc -dumpversion | cut -f1-2 -d.` \>= 6.0)
ifeq "$(GCCVERSIONEQ6)" "0"
ifeq "$(GCCVERSIONEQ5)" "1"
CXXFLAGS += -std=c++14
else
endif
endif
# Include.
INCLUDE := -I../fmt -I../src/include/matio -I../matio/src -I../eigen
INCLUDE += -I../Catch/single_include
INCLUDE += -I../src/include

ASTERISK4 := "****"
ASTERISK16 := $(ASTERISK4)$(ASTERISK4)$(ASTERISK4)$(ASTERISK4)
ASTERISK := $(ASTERISK16)$(ASTERISK16)$(ASTERISK16)$(ASTERISK16)$(ASTERISK16)
COMMENT = @echo -ne "\033[32m"$(ASTERISK)"\033[0m\n"

SRC = ../fmt/fmt/format.o ../fmt/fmt/printf.o
SRC += ../src/core/coord_tran.o ../src/core/integration.o
SRC += ../src/core/sparse_matrix.o ../src/core/eigensolver.o
SRC += ../src/base/material.o ../src/base/section.o ../src/base/node.o ../src/base/load.o
SRC += ../src/base/dof_handler.o ../src/base/string_operation.o
SRC += ../src/element/element_attr.o ../src/element/pipe.o
SRC += ../src/element/additional.o ../src/element/element.o
SRC += ../src/solution/static_analysis.o
SRC += ../src/solution/modal_analysis.o
SRC += ../src/solution/harmonic_full_analysis.o
SRC += ../src/io/simple_convert.o ../src/io/bcy_reader.o
SRC += ../src/fortran/common_reader.o
SRC += ../src/fortran/cdb_reader.o ../src/fortran/bcy_reader_2.o

all:
	@echo "This is nothing yet!"
	$(MAKE) clean

%.o: %.cc
	$(COMMENT)
	@echo -ne "\033[41;33mCompile C++ file\033[0m\t\033[35m"$<"\033[0m\n"
	$(COMMENT)
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $< $(EXTRA_ARGS)

%.o: %.f90
	$(COMMENT)
	@echo -ne "\033[47;31mCompile F90 file\033[0m\t\033[35m"$<"\033[0m\n"
	$(COMMENT)
	$(FC) -c $(FFLAGS) $< $(EXTRA_ARGS)

test_node: $(SRC) ./node_basic_test.o
	$(COMMENT)
	@echo "Begin node test"
	$(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) $(INCLUDE) -o test_node

test_cdb: ../src/fortran/cdb_reader.o ./ansys/cdb_file_test.o
	$(COMMENT)
	@echo "Begin cdb test in fortran"
	$(COMMENT)
	$(FC) $(notdir $^) -o test_cdb
	ls ./ansys | sed 's:^:./ansys/:' | ./test_cdb

test_cdb_cpp: $(SRC) ./ansys/cdb_test_simple.o
	$(COMMENT)
	@echo "Begin cdb test in cpp"
	$(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) $(INCLUDE) -o test_cdb
	$(COMMENT)
	@echo "Begin test."
	$(COMMENT)
	ls ./ansys | sed 's:^:./ansys/:' | ./test_cdb

test_solution: $(SRC) ./solution_basic_test.o
	$(COMMENT)
	@echo -ne "\033[33m"Test: Solution Case"\033[0m\n"
	$(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) $(INCLUDE) -o test_solution
	$(COMMENT)
	@echo -ne "\033[35m"Run test"\033[0m\n"
	$(COMMENT)
	./test_solution

test_modal: $(SRC) ./solution_modal_test.o
	$(COMMENT)
	@echo -ne "\033[33m"Test: Solution Case"\033[0m\n"
	$(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) $(INCLUDE) -o test_modal
	$(COMMENT)
	@echo -ne "\033[35m"Run test"\033[0m\n"
	$(COMMENT)
	./test_modal

test_static: $(SRC) ./solution_static_test.o
	$(COMMENT)
	@echo -ne "\033[33m"Test: Solution Case"\033[0m\n"
	$(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) $(INCLUDE) -o test_static
	$(COMMENT)
	@echo -ne "\033[35m"Run test"\033[0m\n"
	$(COMMENT)
	./test_static

matio_lib:
	@echo "MATIO Library"
	$(MAKE) -f matio.mk

test_basic:
	@echo "Basic Test"
	$(MAKE) -f basic.mk $(varargin)

compile_modal: $(SRC) ./modal_main.o
	$(COMMENT)
	@echo -ne "\033[33m"Modal Analysis Exectue"\033[0m\n"
	$(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) $(INCLUDE) -o modal_analysis
	$(COMMENT)
	@echo -ne "\033[35m"Run test"\033[0m\n"
	$(COMMENT)

compile_harmonic: $(SRC) ./harmonic_main.o
	$(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) $(INCLUDE) -o harmonic_analysis
	$(COMMENT)

clean:
	@echo "Clean files"
	- $(RM) $(CLEAN) *.o *.mod *.mat test_*

.PHONY: clean all
