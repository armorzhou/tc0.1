CCC = g++
CXX = g++
BASICOPTS = -g -Wall -O3
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


TARGETDIR_liblsn.a=.
TARGETDIR_test=.


all: listener.o

$(TARGETDIR_liblsn.a)/listener.o: $(TARGETDIR_liblsn.a) listener.cpp
	$(COMPILE.cc) $(CCFLAGS_liblsn.a) $(CPPFLAGS_liblsn.a) -o $@ listener.cpp


clean:
	rm -f \
		$(TARGETDIR_liblsn.a)/listener.o \

.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

