CC = gcc
CXX = g++
FC = gfortran
CXXFLAGS = -std=c++14 -O3 -lgfortran
FFLAGS = -cpp
INCLUDE = -I../src/include -I../eigen
INCLUDE += -I../cppformat -I../Catch/single_include

ASTERISK4 = "****"
ASTERISK16 = $(ASTERISK4)$(ASTERISK4)$(ASTERISK4)$(ASTERISK4)
ASTERISK = $(ASTERISK16)$(ASTERISK16)$(ASTERISK16)$(ASTERISK16)$(ASTERISK16)
COMMENT = @echo -ne "\033[32m"$(ASTERISK)"\033[0m\n"

SRC = ../cppformat/fmt/format.o ../src/core/coord_tran.o
SRC += ../src/base/material.o ../src/base/section.o
SRC += ../src/solution/modal_analysis.o ../src/io/simple_convert.o
SRC += ../src/fortran/cdb_reader.o 

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
	
clean:
	@echo "Clean files"
	- $(RM) $(CLEAN) *.o *.mod test_*
	
all:
	$(MAKE) test_node
	./test_node
	$(MAKE) clean
	
.PHONY: clean test_node test_cdb test_cdb_cpp test_solution all
