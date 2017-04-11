ROOT=../bcl

CFLAGS= -Wextra -Werror -std=c99 -I$(ROOT)/include 
LDFLAGS=-L$(ROOT)/lib
LDLIBS=-lm -lfftw3 -lbcl

OBJ=fft.o
BIN=	heat-equation\
	butterworth\
	anisotropic-diffusion\
	median\
	bilateral\
	nlmeans

all: $(BIN)

butterworth: fft.o
nlmeans: nl-means.c
	gcc $(CFLAGS) $(LDFLAGS) nl-means.c $(LDLIBS) -o nlmeans

clean:
	$(RM) $(OBJ) $(BIN) *.ppm

.PHONY: all clean 
