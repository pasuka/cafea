CC = gcc
CXX = g++
FC = gfortran
CXXFLAGS = -std=c++14 -O3
INCLUDE = -I../src/include -I../eigen
INCLUDE += -I../cppformat -I../Catch/single_include

ASTERISK = "*******************************************************************"
COMMENT = @echo -ne "\033[32m"$(ASTERISK)"\033[0m\n"

%.o: %.cc
	$(COMMENT)
	@echo -ne "\033[41;33mBegin compile\033[0m\t\033[35m"$<"\033[0m\n"
	$(COMMENT)
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $<

test_node: ../cppformat/cppformat/format.o ./node_basic_test.o
	$(COMMENT)
	@echo "Begin node test"
	$(CXX) $(notdir $^) $(CXXFLAGS) $(INCLUDE) -o test_node
clean:
	@echo "Clean files"
	- $(RM) $(CLEAN) *.o test_*
	
.PHONY: clean test_node