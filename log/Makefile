CC = gcc
BASICOPTS = -g
CFLAGS = $(BASICOPTS) -Wall -O3

lib =liblog.a

all:${lib} test
	
${lib}:log.o
	ar rv $@ $<

log.o:log.c log.h
	${CC} ${CFLAGS} -c -o $@ $<

test:main.o
	${CC} ${CFLAGS} -o $@ $< ${lib}

main.o:main.c
	${CC} ${CFLAGS} -c -o $@ $<
	
clean:
	rm -f *.[oa] test
	
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

