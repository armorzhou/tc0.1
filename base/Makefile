CCC = g++
CXX = g++
BASICOPTS = -g -Wall -O3 
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


TARGETDIR_libbase.a=.
TARGETDIR_test=.


all: libbase.a

OBJS_libbase.a =  \
	$(TARGETDIR_libbase.a)/cluster.o \
	$(TARGETDIR_libbase.a)/sql_column.o \
	$(TARGETDIR_libbase.a)/sql_db.o \
	$(TARGETDIR_libbase.a)/sql_table.o \
	$(TARGETDIR_libbase.a)/base.o

ARCPP = $(AR) $(ARFLAGS) $@ 
$(TARGETDIR_libbase.a)/libbase.a: $(TARGETDIR_libbase.a) $(OBJS_libbase.a)
	$(ARCPP) $(OBJS_libbase.a)

$(TARGETDIR_libbase.a)/cluster.o: $(TARGETDIR_libbase.a) cluster.cpp
	$(COMPILE.cc) $(CCFLAGS_libbase.a) $(CPPFLAGS_libbase.a) -o $@ cluster.cpp

$(TARGETDIR_libbase.a)/sql_column.o: $(TARGETDIR_libbase.a) sql_column.cpp
	$(COMPILE.cc) $(CCFLAGS_libbase.a) $(CPPFLAGS_libbase.a) -o $@ sql_column.cpp

$(TARGETDIR_libbase.a)/sql_db.o: $(TARGETDIR_libbase.a) sql_db.cpp
	$(COMPILE.cc) $(CCFLAGS_libbase.a) $(CPPFLAGS_libbase.a) -o $@ sql_db.cpp

$(TARGETDIR_libbase.a)/sql_table.o: $(TARGETDIR_libbase.a) sql_table.cpp
	$(COMPILE.cc) $(CCFLAGS_libbase.a) $(CPPFLAGS_libbase.a) -o $@ sql_table.cpp

$(TARGETDIR_libbase.a)/base.o: $(TARGETDIR_libbase.a) base.cpp
	$(COMPILE.cc) $(CCFLAGS_libbase.a) $(CPPFLAGS_libbase.a) -o $@ base.cpp


clean:
	rm -f \
		$(TARGETDIR_libbase.a)/libbase.a \
		$(TARGETDIR_libbase.a)/cluster.o \
		$(TARGETDIR_libbase.a)/sql_column.o \
		$(TARGETDIR_libbase.a)/sql_db.o \
		$(TARGETDIR_libbase.a)/sql_table.o \
		$(TARGETDIR_libbase.a)/base.o

.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

