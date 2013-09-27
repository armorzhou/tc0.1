CCC = g++
CXX = g++
BASICOPTS = -g -Wall -O3
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


TARGETDIR_liblogin.a=.
TARGETDIR_test=.


all: liblogin.a test

OBJS_liblogin.a =  \
	$(TARGETDIR_liblogin.a)/password.o \
	$(TARGETDIR_liblogin.a)/sha1.o \
	$(TARGETDIR_liblogin.a)/login.o


ARCPP = $(AR) $(ARFLAGS) $@ 
$(TARGETDIR_liblogin.a)/liblogin.a: $(TARGETDIR_liblogin.a) $(OBJS_liblogin.a)
	$(ARCPP) $(OBJS_liblogin.a)


$(TARGETDIR_liblogin.a)/password.o: $(TARGETDIR_liblogin.a) password.cpp
	$(COMPILE.cc) $(CCFLAGS_liblogin.a) $(CPPFLAGS_liblogin.a) -o $@ password.cpp

$(TARGETDIR_liblogin.a)/sha1.o: $(TARGETDIR_liblogin.a) sha1.cpp
	$(COMPILE.cc) $(CCFLAGS_liblogin.a) $(CPPFLAGS_liblogin.a) -o $@ sha1.cpp

$(TARGETDIR_liblogin.a)/login.o: $(TARGETDIR_liblogin.a) login.cpp
	$(COMPILE.cc) $(CCFLAGS_liblogin.a) $(CPPFLAGS_liblogin.a) -o $@ login.cpp



OBJS_test =  \
	$(TARGETDIR_test)/main.o
USERLIBS_test = ../log/libjlog.a -lcrypto $(SYSLIBS_test) 
DEPLIBS_test = ../log/libjlog.a    
LDLIBS_test = $(USERLIBS_test) /usr/local/lib/libmysqlclient_r.a -lmysqlclient 


$(TARGETDIR_test)/test: $(TARGETDIR_test) $(OBJS_test) $(DEPLIBS_test)
	$(LINK.cc) $(CCFLAGS_test) $(CPPFLAGS_test) -o $@ $(OBJS_test) $(LDLIBS_test)

$(TARGETDIR_test)/main.o: $(TARGETDIR_test) main.cpp
	$(COMPILE.cc) $(CCFLAGS_test) $(CPPFLAGS_test) -o $@ main.cpp



clean:
	rm -f \
		$(TARGETDIR_liblogin.a)/liblogin.a \
		$(TARGETDIR_liblogin.a)/password.o \
		$(TARGETDIR_liblogin.a)/sha1.o \
		$(TARGETDIR_liblogin.a)/login.o \
		$(TARGETDIR_test)/test \
		$(TARGETDIR_test)/password.o \
		$(TARGETDIR_test)/sha1.o \
		$(TARGETDIR_test)/login.o \
		$(TARGETDIR_test)/main.o \
		$(TARGETDIR_test)/tc_main.o
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

