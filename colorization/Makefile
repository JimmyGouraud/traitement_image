ROOT=../bcl

CFLAGS= -Wall -Wextra -std=c99 -I$(ROOT)/include
LDFLAGS= -L$(ROOT)/lib
LDLIBS= -lbcl -lm
BIN=\
	colorization

all: $(BIN)

clean:
	$(RM) $(BIN) *.ppm

.PHONY:
	clean


