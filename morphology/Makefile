ROOT=../bcl

CFLAGS=-Wall -Wextra -Werror -std=c99 -I$(ROOT)/include 
LDFLAGS=-L$(ROOT)/lib 
LDLIBS=-lm -lbcl

OBJ=morphology.o

BIN=\
	make-se\
	labeling

all: $(BIN)

make-se: morphology.o

clean:
	$(RM) $(OBJ) $(BIN) *.ppm

.PHONY: all clean 
