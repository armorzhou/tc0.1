## -*- Makefile -*-
##
## �û�: tangchao
## ʱ��: 2014-1-5 11:44:29
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
TARGETDIR_libtclog.a=.


all: libtclog.a

## Ŀ��: libtclog.a
OBJS_libtclog.a =  \
	$(TARGETDIR_libtclog.a)/log.o


# ���ӻ�鵵
ARCPP = $(AR) $(ARFLAGS) $@ 
$(TARGETDIR_libtclog.a)/libtclog.a: $(TARGETDIR_libtclog.a) $(OBJS_libtclog.a)
	$(ARCPP) $(OBJS_libtclog.a)


# ��Դ�ļ�����Ϊ .o �ļ�
$(TARGETDIR_libtclog.a)/log.o: $(TARGETDIR_libtclog.a) log.cpp
	$(COMPILE.cc) $(CCFLAGS_libtclog.a) $(CPPFLAGS_libtclog.a) -o $@ log.cpp



#### ����Ŀ�꽫��ɾ��������ɵ��ļ� ####
clean:
	rm -f \
		$(TARGETDIR_libtclog.a)/libtclog.a \
		$(TARGETDIR_libtclog.a)/log.o
# ����������ϵ���
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

