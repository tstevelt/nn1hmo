PROG = nn1hmo
LIB = nn1hmo.a
XLIB = -lm 
DEF = /usr/local/include
PRG = /usr/local/bin/$(PROG)

CFLAGS = -g -fsanitize=address -Wall -Wformat-truncation=0 -I../../include -I/usr/local/include

FILES = \
	$(LIB)(nn1hmo.o)	\
	$(LIB)(shslib.o)	\
	$(LIB)(getargs.o)	\
	$(LIB)(genparms.o)	\
	$(LIB)(getparms.o)	\
	$(LIB)(init.o)		\
	$(LIB)(train.o)		\
	$(LIB)(algo.o)		\
	$(LIB)(test.o)		\
	$(LIB)(predict.o)	\
	$(LIB)(miscfuncs.o)	\
	$(LIB)(modelfuncs.o) \
	$(LIB)(SignalFuncs.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc -fsanitize=address -o $(PRG) $(LIB) $(XLIB)

$(LIB): $(FILES)

$(FILES): nn1hmo.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

