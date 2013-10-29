CCC = g++
CXX = g++
BASICOPTS = -g -Wall -O3
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 

TARGETDIR_libconn.a=.
all: protocol_test

protocol_test: protocol_test.cpp 
	${CCC}  ${CXXFLAGS} -o $@ $< ../protocol_10.cpp ../net_serv.cpp ../../listener/listener.o

clean:
	rm -f \
		protocol_test

.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

