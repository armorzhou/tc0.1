CCC = g++
CXX = g++
BASICOPTS = -g -Wall -O3
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 

all:libpl.a

libpl.a:protocol_10.o net_serv.o
	ar rv libpl.a protocol_10.o net_serv.o

protocol_10.o:protocol_10.cpp
	${CCC} ${CXXFLAGS} -c -o $@ $<

net_serv.o:net_serv.cpp
	${CCC} ${CXXFLAGS} -c -o $@ $<

clean:
	rm *.[oa]