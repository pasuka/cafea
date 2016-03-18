CC = gcc
CXX = g++
FC = gfortran
CXXFLAGS = -std=c++14 -O3
INCLUDE = -I../src/include -I../eigen
INCLUDE += -I../cppformat -I../Catch/single_include

ASTERISK = "*******************************************************************"
COMMENT = @echo -ne "\033[32m"$(ASTERISK)"\033[0m\n"

SRC = ../cppformat/cppformat/format.o ../src/core/coord_tran.o

%.o: %.cc
	$(COMMENT)
	@echo -ne "\033[41;33mBegin compile\033[0m\t\033[35m"$<"\033[0m\n"
	$(COMMENT)
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $<

test_node: $(SRC) ./node_basic_test.o 
	$(COMMENT)
	@echo "Begin node test"
	$(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) $(INCLUDE) -o test_node
	
clean:
	@echo "Clean files"
	- $(RM) $(CLEAN) *.o test_*
	
all:
	$(MAKE) test_node
	./test_node
	$(MAKE) clean
	
.PHONY: clean test_node all
