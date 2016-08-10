CC := gcc
CFLAGS := -D_GNU_SOURCE=1

SRC_DIR := ../matio/src
##############################################
#      Note: Only for MSYS2 now.             #
##############################################
# Add header files.
CFLAGS += -I/mingw64/include -I../src/include/matio -I$(SRC_DIR)
# Add library path.
CFLAGS += -lhdf5 -lz -lm -fPIC -shared

SRC := $(SRC_DIR)/inflate.o $(SRC_DIR)/endian.o $(SRC_DIR)/snprintf.o
SRC += $(SRC_DIR)/mat.o $(SRC_DIR)/mat4.o $(SRC_DIR)/mat5.o 
SRC += $(SRC_DIR)/mat73.o $(SRC_DIR)/matvar_struct.o $(SRC_DIR)/io.o
SRC += $(SRC_DIR)/matvar_cell.o $(SRC_DIR)/read_data.o

%.o: %.c
	$(CC) -c $(CFLAGS) $<
	
libs: $(SRC)
	$(CC) $(notdir $^) $(CFLAGS) -o libmatio.dll

clean:
	@echo "MATIO: Clean files"
	- $(RM) $(CLEAN) *.o *.exe *.mat
	
all:
	$(MAKE) libs
	$(MAKE) clean
	
.PHONY: clean libs all


