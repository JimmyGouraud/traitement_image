ROOT=.
LIBNAME=bcl
LIBFILENAME=lib$(LIBNAME).a

CC= gcc
CFLAGS= -g -Wall -std=c99
CPPFLAGS=

HEADERS= \
	src/bcl.h \
	src/exception.h \
	src/memory.h \
	src/message.h \
	src/str.h \
	src/pnm.h

OBJ= \
	src/bcl.o \
	src/exception.o \
	src/memory.o \
	src/message.o \
	src/str.o \
	src/pnm.o

$(LIBFILENAME) : $(OBJ)
	rm -f $(LIBFILENAME)
	ar -qc $(LIBFILENAME) $(OBJ)

TEST_str: TEST_str.o $(OBJ)
	$(CC) TEST_str.o $(OBJ) -o TEST_str
TEST_str.o: TEST_str.c str.c bcl.h

bcl.o: bcl.c $(HEADERS)
exception.o: exception.c exception.h memory.h
memory.o: memory.c  exception.h
message.o: message.c memory.h exception.h
str.o: str.c str.h memory.h
pnm.o: pnm.c pnm.h memory.h

install : checkdirs $(ROOT)/lib/$(LIBFILENAME) 

checkdirs :
	[ -d $(ROOT)/lib ] || mkdir $(ROOT)/lib
	[ -d $(ROOT)/include ] || mkdir $(ROOT)/include

$(ROOT)/lib/$(LIBFILENAME) : $(LIBFILENAME)
	cp $(HEADERS) $(ROOT)/include
	cp $(LIBFILENAME) $(ROOT)/lib

clean :
	rm -f $(OBJ) $(LIBFILENAME)

cleanall : clean
	rm -rf $(ROOT)/lib $(ROOT)/include


