CC = gcc
BASICOPTS = -g
CFLAGS = $(BASICOPTS) -Wall -O3

lib =liblsn.a

all:${lib} test
	
${lib}:listener.o
	ar rv $@ $<

listener.o:listener.c listener.h
	${CC} ${CFLAGS} -c -o $@ $<

test:main.o
	${CC} ${CFLAGS} -o $@ $< ${lib} ../init/libinit.a

main.o:main.c
	${CC} ${CFLAGS} -c -o $@ $<
	
clean:
	rm -f *.[oa] test ${lib}
	
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

