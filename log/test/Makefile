## -*- Makefile -*-
##
## �û�: tangchao
## ʱ��: 2014-1-5 11:47:21
## Makefile created by Oracle Solaris Studio.
##
## ���ļ����Զ���ɵġ�
##


#### �����й���Ŀ�깲��ı������͹��߶��� #####
CCC = g++
CXX = g++
BASICOPTS = -g -Wall -m64
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


# ����Ŀ��Ŀ¼��
TARGETDIR_logtest=.


all: logtest

## Ŀ��: logtest
CPPFLAGS_logtest = 
OBJS_logtest =  \
	$(TARGETDIR_logtest)/logtest.o
SYSLIBS_logtest = -static 
USERLIBS_logtest = $(SYSLIBS_logtest) ../libtclog.a
DEPLIBS_logtest =  
LDLIBS_logtest = $(USERLIBS_logtest)


# ���ӻ�鵵
$(TARGETDIR_logtest)/logtest: $(TARGETDIR_logtest) $(OBJS_logtest) $(DEPLIBS_logtest)
	$(LINK.cc) $(CCFLAGS_logtest) $(CPPFLAGS_logtest) -o $@ $(OBJS_logtest) $(LDLIBS_logtest)


# ��Դ�ļ�����Ϊ .o �ļ�
$(TARGETDIR_logtest)/logtest.o: $(TARGETDIR_logtest) logtest.cpp
	$(COMPILE.cc) $(CCFLAGS_logtest) $(CPPFLAGS_logtest) -o $@ logtest.cpp



#### ����Ŀ�꽫��ɾ��������ɵ��ļ� ####
clean:
	rm -f \
		$(TARGETDIR_logtest)/logtest \
		$(TARGETDIR_logtest)/logtest.o
# ����������ϵ���
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

