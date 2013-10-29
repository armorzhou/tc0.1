CCC = g++
CXX = g++
BASICOPTS = -g
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


TARGETDIR_libjlog.a=.
TARGETDIR_test=.


all: libjlog.a #test

CPPFLAGS_libjlog.a = -I/usr/local/include

OBJS_libjlog.a =  \
	$(TARGETDIR_libjlog.a)/jlog.o


ARCPP = $(AR) $(ARFLAGS) $@ 
$(TARGETDIR_libjlog.a)/libjlog.a: $(TARGETDIR_libjlog.a) $(OBJS_libjlog.a)
	$(ARCPP) $(OBJS_libjlog.a)


$(TARGETDIR_libjlog.a)/jlog.o: $(TARGETDIR_libjlog.a) jlog.cpp
	$(COMPILE.cc) $(CCFLAGS_libjlog.a) $(CPPFLAGS_libjlog.a) -o $@ jlog.cpp



CPPFLAGS_test = -I/usr/local/include -L/usr/local/lib

OBJS_test =  \
	$(TARGETDIR_test)/jlog.o \
	$(TARGETDIR_test)/main.o
SYSLIBS_test = -Wl,-rpath -Wl,LIBDIR -lglog -llog4c
USERLIBS_test =  $(SYSLIBS_test) 
DEPLIBS_test =    
LDLIBS_test = $(USERLIBS_test) libjlog.a  /usr/local/lib/liblog4c.a \
	/usr/local/lib/libglog.a


$(TARGETDIR_test)/test: $(TARGETDIR_test) $(OBJS_test) $(DEPLIBS_test)
	$(LINK.cc) $(CCFLAGS_test) $(CPPFLAGS_test) -o $@ $(OBJS_test) $(LDLIBS_test)

$(TARGETDIR_test)/main.o: $(TARGETDIR_test) main.cpp
	$(COMPILE.cc) $(CCFLAGS_test) $(CPPFLAGS_test) -o $@ main.cpp



clean:
	rm -f \
		$(TARGETDIR_libjlog.a)/libjlog.a \
		$(TARGETDIR_libjlog.a)/jlog.o \
		$(TARGETDIR_test)/test \
		$(TARGETDIR_test)/main.o
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

