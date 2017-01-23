ROOT=../bcl

CFLAGS=-Wall -Wextra -Werror -std=c99 -I$(ROOT)/include 
LDFLAGS=-L$(ROOT)/lib
LDLIBS=-lm -lfftw3 -lbcl

BIN=test-fft
OBJ=fft.o 

all: $(BIN)

fft.o: fft.c fft.h
test-fft: fft.o

clean:
	$(RM) $(OBJ) $(BIN) *.ppm

.PHONY: clean all


