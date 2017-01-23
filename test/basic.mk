# Compilers.
CC := gcc
FC := gfortran
CXX := g++
# Compile options.
CXXFLAGS := -O3#-g
# Include.
CXXINC := -I../fmt -I../Catch/single_include -I../src/include
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

all:
	@echo "Nothing yet!"

%.o: %.cc
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(ORANGE)"Compile c++ files."$(COLOR_OFF)
	@echo -e $(COMMENT)
	$(CXX) -c $(CXXFLAGS) $(CXXINC) $<

a01: ../fmt/fmt/format.o ./basic/a01.o
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(RED)"ObjectBase test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	$(CXX) $(notdir $^) $(CXXFLAGS) $(CXXINC) -o test_a01
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(PURPLE)"Execute test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	./test_a01

a02:
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(RED)"Node test."$(COLOR_OFF)
	@echo -e $(COMMENT)
	@echo -e $(COMMENT)
	@echo -e $(BLANK)$(PURPLE)"Execute test."$(COLOR_OFF)
	@echo -e $(COMMENT)

clean:
	@echo "Clean files."
	- $(RM) $(CLEAN) *.o test_*

.PHONY: all clean