# Compilers.
CC := gcc
FC := gfortran
CXX := g++
# Compile options.
CXXFLAGS := -O3# -g
# Include.
CXXFLAGS += -I../fmt -I../Catch/single_include -I../eigen -I../src/include \
-I../src/include/matio -I../matio/src
# Color definitions.
# Regular colors.
BLACK  = '\033[0;30m'
RED    = '\033[0;31m'
GREEN  = '\033[0;32m'
ORANGE = '\033[0;33m'
BLUE   = '\033[0;34m'
PURPLE = '\033[0;35m'
CYAN   = '\033[0;36m'
WHITE  = '\033[0;37m'
# Reset.
COLOR_OFF    = '\033[0m'
# Comment line.
ASTERISK36 = "************************************"
COMMENT = $(GREEN)$(ASTERISK36)$(ASTERISK36)"********"$(COLOR_OFF)
BLANK = "      "

all: a01 a02 a03 a04 a05 a06 b01 b02 b03
	@echo "[Basic] Test all cases."
	$(MAKE) -f basic.mk clean

%.o: %.cc
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(ORANGE)"[Basic] Compile c++ files."$(COLOR_OFF)
	@echo -e $(COMMENT)
	$(CXX) -c $(CXXFLAGS) $<

a01: ../fmt/fmt/format.o ./basic/a01.o
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(RED)"[Basic] ObjectBase test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) -o test_$@
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(PURPLE)"Execute test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	./test_$@

a02: ../fmt/fmt/format.o ../src/base/load.o ./basic/a02.o
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(RED)"[Basic] Load test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) -o test_$@
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(PURPLE)"Execute test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	./test_$@

a03: ../fmt/fmt/format.o ./basic/a03.o
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(RED)"[Basic] Boundary test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) -o test_$@
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(PURPLE)"Execute test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	./test_$@

a04: ../fmt/fmt/format.o ../src/base/material.o ./basic/a04.o
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(RED)"[Basic] Material test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) -o test_$@
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(PURPLE)"Execute test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	./test_$@

a05: ../fmt/fmt/format.o ../src/base/section.o ./basic/a05.o
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(RED)"[Basic] Section test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) -o test_$@
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(PURPLE)"Execute test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	./test_$@

a06: ../fmt/fmt/format.o ../src/base/dof_handler.o ./basic/a06.o
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(RED)"[Basic] Dof handler test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) -o test_$@
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(PURPLE)"Execute test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	./test_$@

b01: ../fmt/fmt/format.o ../src/base/dof_handler.o ../src/element/element_attr.o \
../src/base/node.o ../src/core/coord_tran.o ./basic/b01.o
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(RED)"[Basic] Node test 01."$(COLOR_OFF)
	@echo -e $(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) -o test_$@
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(PURPLE)"Execute test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	./test_$@

b02: ../fmt/fmt/format.o ../src/base/dof_handler.o ../src/element/element_attr.o \
../src/base/node.o ../src/core/coord_tran.o ./basic/b02.o
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(RED)"[Basic] Node test 02."$(COLOR_OFF)
	@echo -e $(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) -o test_$@
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(PURPLE)"Execute test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	./test_$@

b03: ../fmt/fmt/format.o ../src/base/dof_handler.o ../src/element/element_attr.o \
../src/base/node.o ../src/base/load.o ../src/base/material.o ../src/base/section.o \
../src/core/coord_tran.o ../src/element/element.o ../src/element/pipe.o ../src/element/additional.o \
./basic/b03.o
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(RED)"[Basic] Element test 01."$(COLOR_OFF)
	@echo -e $(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) -o test_$@
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(PURPLE)"Execute test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	./test_$@

clean:
	@echo "[Basic] Clean files."
	- $(RM) $(CLEAN) *.o test_*

.PHONY: all clean
