CC = gcc
BASICOPTS = -g
CFLAGS = $(BASICOPTS) -Wall -O3

lib =libinit.a

all:${lib} test
	
${lib}:init.o
	ar rv $@ $<

init.o:init.c init.h
	${CC} ${CFLAGS} -c -o $@ $<

test:main.o
	${CC} ${CFLAGS} -o $@ $< ${lib}

main.o:main.c
	${CC} ${CFLAGS} -c -o $@ $<
	
clean:
	rm -f *.[oa] test ${lib}
	
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

