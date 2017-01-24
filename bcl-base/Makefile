ROOT=../bcl

CFLAGS= -Wall -Wextra -Werror -std=c99 -I$(ROOT)/include
LDFLAGS= -L$(ROOT)/lib
LDLIBS= -lbcl
BIN=\
	print-ppm\
	test1\
	extract-subimage\
	extract-channel\
	gray2color\
	color2mean\
	normalize

all: $(BIN)

clean:
	$(RM) $(BIN) *.ppm

.PHONY:
	clean all test


