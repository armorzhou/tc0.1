CCC = g++
CXX = g++
BASICOPTS = -g -Wall -O3
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 

OBJ = testsend

	
testsend.o:testsend.cpp
	${CCC} ${BASICOPTS} -o $@ $< ../send_server_handshake.o

	
clean:
	rm -f *.[ao]
	 
