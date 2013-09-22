bin=tc
all:${bin}

${bin}:main.o tc.o
	gcc -g -Wall -O3 -o $@ $< tc.o init/libinit.a log/liblog.a listener/liblsn.a
	
clean:
	rm -f *.[oa]
	rm -f */*.[oa]
	rm -f */test
	rm -f tc