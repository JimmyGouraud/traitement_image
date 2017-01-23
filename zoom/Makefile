ROOT=../bcl

CFLAGS=-Wall -Wextra -Werror -std=c99 -I$(ROOT)/include 
LDFLAGS=-L$(ROOT)/lib -lbcl 
LDLIBS=-lm -lfftw3 -lbcl 

OBJ=fft.o 
BIN=\
	copy\
	padding\
	filter 

all: $(BIN)

padding: fft.o

clean:
	$(RM) $(OBJ) $(BIN) *.ppm

.PHONY: all clean 
