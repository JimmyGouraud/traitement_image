ROOT=../bcl

CFLAGS=-Wall -Wextra -Werror -std=c99 -I$(ROOT)/include 
LDFLAGS=-L$(ROOT)/lib 
LDLIBS=-lm -lbcl

OBJ=morphology.o

BIN=\
	dilation\
	erosion

all: $(BIN)

dilation: $(OBJ)
erosion:  $(OBJ)

clean:
	$(RM) $(OBJ) $(BIN) *.ppm

.PHONY: all clean 
