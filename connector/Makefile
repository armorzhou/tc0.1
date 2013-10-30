CCC = g++
CXX = g++
BASICOPTS = -g
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


TARGETDIR_libconn.a=.


all: mysql_con.o


mysql_con.o: $(TARGETDIR_libconn.a) mysql_con.cpp
	$(COMPILE.cc) $(CCFLAGS_libconn.a) $(CPPFLAGS_libconn.a) -o $@ mysql_con.cpp

clean:
	rm -f \
		mysql_con.o

.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

