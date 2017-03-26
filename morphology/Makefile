ROOT=../bcl

CFLAGS=-Wall -Wextra -Werror -std=c99 -I$(ROOT)/include 
LDFLAGS=-L$(ROOT)/lib 
LDLIBS=-lm -lbcl

OBJ=morphology.o

BIN=\
	make-se\
	labeling\
	dilation\
	erosion\
	opening\
	closing\
	add\
	subtract\
	binarize\
	labeling-color

all: $(BIN)

make-se: morphology.o
dilation: morphology.o
erosion: morphology.o
opening: morphology.o
closing: morphology.o

clean:
	$(RM) $(OBJ) $(BIN) *.ppm

.PHONY: all clean 
