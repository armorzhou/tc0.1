## -*- Makefile -*-
##
## �û�: tangchao
## ʱ��: 2013-10-24 14:58:46
## Makefile created by Oracle Solaris Studio.
##
## ���ļ����Զ���ɵġ�
##


#### �����й���Ŀ�깲��ı������͹��߶��� #####
CCC = g++
CXX = g++
BASICOPTS = -g
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 

TARGETDIR_test=.


all: test

OBJS_test =  \
	$(TARGETDIR_test)/test.o
USERLIBS_test = $(SYSLIBS_test) -Wl,-rpath -Wl,LIBDIR -lglog  \
	../../log/libjlog.a  ../libbase.a
DEPLIBS_test =  
LDLIBS_test = $(USERLIBS_test)


$(TARGETDIR_test)/test: $(TARGETDIR_test) $(OBJS_test) $(DEPLIBS_test)
	$(LINK.cc) $(CCFLAGS_test) $(CPPFLAGS_test) -o $@ $(OBJS_test) \
	$(LDLIBS_test)  


$(TARGETDIR_test)/test.o: $(TARGETDIR_test) test.cpp
	$(COMPILE.cc) $(CCFLAGS_test) $(CPPFLAGS_test) -o $@ test.cpp


clean:
	rm -f \
		$(TARGETDIR_test)/test \
		$(TARGETDIR_test)/test.o

.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

