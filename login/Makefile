CCC = g++
CXX = g++
BASICOPTS = -g -Wall -O3
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 

OBJ = login55.o server_connection_cnf.o

liblogin.a:${OBJ}
	ar rv $@ $<
	
login55.o:login55.cpp
	${CCC} ${BASICOPTS} -c -o $@ $<

server_connection_cnf.o:server_connection_cnf.cpp
	${CCC} ${BASICOPTS} -c -o $@ $<

	
clean:
	rm -f *.[ao]
	 