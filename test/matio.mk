##############################################
# Build MATIO dynamic library.               #
##############################################
# Note: Only for MSYS2 now.                  #
##############################################
# Requirement: hdf5 zlib                     #
##############################################

# Compiler.
CC := gcc
CFLAGS := -D_GNU_SOURCE=1
# Source path.
SRC_DIR := ../matio/src
# Add header files.
CFLAGS += -I../src/include/matio -I$(SRC_DIR)
# Add library path.
CFLAGS += -lhdf5 -lz -lm -fPIC -shared
# Source files.
SRC := inflate.o endian.o snprintf.o mat.o mat4.o mat5.o mat73.o
SRC += matvar_struct.o io.o matvar_cell.o read_data.o
# Check OS version.
ifeq ($(OS), Windows_NT)
	uname_s := Windows
else
	uname_s := $(shell uname -s)
endif
# Dynamic library name.
ifeq ($(uname_s), Windows)
	target = libmatio.dll
endif
ifeq ($(uname_s), Linux)
	target = libmatio.so
endif

all:
	$(MAKE) libs
	$(MAKE) clean

%.o: %.c
	@echo "MATIO: Complie c files."
	$(CC) -c $(CFLAGS) $<

libs: $(addprefix $(SRC_DIR)/, $(SRC))
	@echo "MATIO: Build dynamic library."
	$(CC) $(notdir $^) $(CFLAGS) -o $(target)

clean:
	@echo "MATIO: Clean files"
	- $(RM) $(CLEAN) *.o *.exe *.mat

.PHONY: clean all
