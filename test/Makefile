CC = gcc
CXX = g++
FC = gfortran
CXXFLAGS = -std=c++14 -O3 -lgfortran
FFLAGS = -cpp
INCLUDE = -I../src/include -I../eigen
INCLUDE += -I../cppformat -I../Catch/single_include

ASTERISK = "*********************************************"
COMMENT = @echo -ne "\033[32m"$(ASTERISK)"\033[0m\n"

SRC = ../cppformat/fmt/format.o ../src/core/coord_tran.o
SRC += ../src/solution/modal_analysis.o ../src/io/simple_convert.o

SRC_F90 = ../src/fortran/cdb_reader.o 

%.o: %.cc
	$(COMMENT)
	@echo -ne "\033[41;33mBegin compile\033[0m\t\033[35m"$<"\033[0m\n"
	$(COMMENT)
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $< $(args)

%.o: %.f90
	$(COMMENT)
	@echo -ne "\033[47;31mBegin compile\033[0m\t\033[35m"$<"\033[0m\n"
	$(COMMENT)
	$(FC) -c $(FFLAGS) $< $(args)

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
	
test_cdb_cpp: $(SRC) ../src/fortran/cdb_reader.o ./ansys/cdb_test_simple.o
	$(COMMENT)
	@echo "Begin cdb test in cpp"
	$(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) $(INCLUDE) -o test_cdb
	ls ./ansys | sed 's:^:./ansys/:' | ./test_cdb

test_solution: $(SRC) $(SRC_F90) ./solution_basic_test.o
	$(COMMENT)
	@echo "Begin solution test in c++"
	$(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) $(INCLUDE) -o test_solution
	./test_solution
	
clean:
	@echo "Clean files"
	- $(RM) $(CLEAN) *.o *.mod test_*
	
all:
	$(MAKE) test_node
	./test_node
	$(MAKE) clean
	
.PHONY: clean test_node test_cdb test_cdb_cpp test_solution all
