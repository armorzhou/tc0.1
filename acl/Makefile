CCC = g++
CXX = g++
BASICOPTS = -g -Wall -O3
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


TARGETDIR_libacl.a=.


all: libacl.a

OBJS_libacl.a =  \
	$(TARGETDIR_libacl.a)/acl_access.o \
	$(TARGETDIR_libacl.a)/acl_convert.o \
	$(TARGETDIR_libacl.a)/acl_db.o \
	$(TARGETDIR_libacl.a)/acl_schema.o \
	$(TARGETDIR_libacl.a)/sql_acl.o


ARCPP = $(AR) $(ARFLAGS) $@ 
$(TARGETDIR_libacl.a)/libacl.a: $(TARGETDIR_libacl.a) $(OBJS_libacl.a)
	$(ARCPP) $(OBJS_libacl.a)


$(TARGETDIR_libacl.a)/acl_access.o: $(TARGETDIR_libacl.a) acl_access.cpp
	$(COMPILE.cc) $(CCFLAGS_libacl.a) $(CPPFLAGS_libacl.a) -o $@ acl_access.cpp

$(TARGETDIR_libacl.a)/acl_convert.o: $(TARGETDIR_libacl.a) acl_convert.cpp
	$(COMPILE.cc) $(CCFLAGS_libacl.a) $(CPPFLAGS_libacl.a) -o $@ acl_convert.cpp

$(TARGETDIR_libacl.a)/acl_db.o: $(TARGETDIR_libacl.a) acl_db.cpp
	$(COMPILE.cc) $(CCFLAGS_libacl.a) $(CPPFLAGS_libacl.a) -o $@ acl_db.cpp

$(TARGETDIR_libacl.a)/acl_schema.o: $(TARGETDIR_libacl.a) acl_schema.cpp
	$(COMPILE.cc) $(CCFLAGS_libacl.a) $(CPPFLAGS_libacl.a) -o $@ acl_schema.cpp

$(TARGETDIR_libacl.a)/sql_acl.o: $(TARGETDIR_libacl.a) sql_acl.cpp
	$(COMPILE.cc) $(CCFLAGS_libacl.a) $(CPPFLAGS_libacl.a) -o $@ sql_acl.cpp

clean:
	rm -f \
		$(TARGETDIR_libacl.a)/libacl.a \
		$(TARGETDIR_libacl.a)/acl_access.o \
		$(TARGETDIR_libacl.a)/acl_convert.o \
		$(TARGETDIR_libacl.a)/acl_db.o \
		$(TARGETDIR_libacl.a)/acl_schema.o \
		$(TARGETDIR_libacl.a)/sql_acl.o \

.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-x86-Linux

