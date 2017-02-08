# Compilers.
CC := gcc
FC := gfortran
CXX := g++
# Compile options.
CXXFLAGS := -O3#-g
# Include.
CXXFLAGS += -I../fmt -I../Catch/single_include -I../src/include
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
COMMENT = $(GREEN)$(ASTERISK36)$(ASTERISK36)$(COLOR_OFF)
BLANK = "      "

all: a01 a02 a03
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
	$(CXX) $(notdir $^) $(CXXFLAGS) -o test_a01
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(PURPLE)"Execute test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	./test_a01

a02: ../fmt/fmt/format.o ../src/base/load.o ./basic/a02.o
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(RED)"[Basic] Load test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) -o test_a02
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(PURPLE)"Execute test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	./test_a02

a03: ../fmt/fmt/format.o ./basic/a03.o
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(RED)"[Basic] Boundary test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) -o test_a03
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(PURPLE)"Execute test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	./test_a03

clean:
	@echo "[Basic] Clean files."
	- $(RM) $(CLEAN) *.o test_*

.PHONY: all clean
