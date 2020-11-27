#Set the shell to bash because it has more features
shell := /bin/bash

gcc=gcc -Wall
lib=-lpthread
bin=bin/

#Set up debug configuration
ifdef $(dbg)
	gcc=$(gcc) -g
	Run=gdb
	lvl=-Og
else
	Run=
	lvl=-O3
endif

RunEnd= "encodedmessage.txt"
def=
option=$(lvl) $(opt) $(def)

obj=.o
out=pbreak$(obj)

#define obj files
ofiles=$(bin)pbreak$(obj) $(bin)queue$(obj) \
$(bin)keygen$(obj) $(bin)thread$(obj)
#define c files for compile
cfiles=pbreak.c queue.c keygen.c thread.c
#define h files to rquire
hfiles=keygen.h queue.h thread.h

all: build run

build: $(cfiles) $(hfiles)
	$(gcc) $(cfiles) -o $(out) $(option) $(lib)

run: build
	$(Run) ./$(out) $(RunEnd)

clean:
	rm $(out)
	#rm $(ofiles)


$(bin)%$(obj): %.c $(hfiles)
	$(gcc) $< -c -o $@ $(option) $(lib)

link: $(ofiles)
	$(gcc) $^ -o $(out) $(option) $(lib)
