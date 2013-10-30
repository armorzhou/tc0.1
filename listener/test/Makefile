CCC = g++
CXX = g++
BASICOPTS = -g
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 

TARGETDIR_test=.

all: test

OBJS_test =  \
	$(TARGETDIR_test)/test.o \
	../listener.o
	
USERLIBS_test = $(SYSLIBS_test) 
DEPLIBS_test =  
LDLIBS_test = $(USERLIBS_test)


$(TARGETDIR_test)/test: $(TARGETDIR_test) $(OBJS_test) $(DEPLIBS_test)
	$(LINK.cc) $(CCFLAGS_test) $(CPPFLAGS_test) -o $@ $(OBJS_test) $(LDLIBS_test) 


$(TARGETDIR_test)/test.o: $(TARGETDIR_test) test.cpp
	$(COMPILE.cc) $(CCFLAGS_test) $(CPPFLAGS_test) -o $@ test.cpp



clean:
	rm -f \
		$(TARGETDIR_test)/test \
		$(TARGETDIR_test)/test.o
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

