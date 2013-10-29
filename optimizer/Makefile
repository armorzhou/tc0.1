CCC = g++
CXX = g++
BASICOPTS = -g -Wall -O3
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 
DOS2UNIX=

TARGETDIR_libopt.a=.
TARGETDIR_test=.


all: ${DOS2UNIX} libopt.a #test

${DOS2UNIX}:
	dos2unix *.cpp
	dos2unix *.h
	
OBJS_libopt.a =  \
	$(TARGETDIR_libopt.a)/cs_db.o \
	$(TARGETDIR_libopt.a)/cs_field.o \
	$(TARGETDIR_libopt.a)/cs_table.o \
	$(TARGETDIR_libopt.a)/optimizer_mysql.o \
	$(TARGETDIR_libopt.a)/stmt_template.o \
	$(TARGETDIR_libopt.a)/object_select.o 


ARCPP = $(AR) $(ARFLAGS) $@ 
$(TARGETDIR_libopt.a)/libopt.a: $(TARGETDIR_libopt.a) $(OBJS_libopt.a)
	$(ARCPP) $(OBJS_libopt.a)

$(TARGETDIR_libopt.a)/object_select.o: $(TARGETDIR_libopt.a) object_select.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ object_select.cpp

$(TARGETDIR_libopt.a)/cs_db.o: $(TARGETDIR_libopt.a) cs_db.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ cs_db.cpp

$(TARGETDIR_libopt.a)/cs_field.o: $(TARGETDIR_libopt.a) cs_field.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ cs_field.cpp

$(TARGETDIR_libopt.a)/cs_table.o: $(TARGETDIR_libopt.a) cs_table.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ cs_table.cpp

$(TARGETDIR_libopt.a)/execute_plan.o: $(TARGETDIR_libopt.a) execute_plan.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ execute_plan.cpp

$(TARGETDIR_libopt.a)/optimizer.o: $(TARGETDIR_libopt.a) optimizer.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ optimizer.cpp

$(TARGETDIR_libopt.a)/optimizer_mysql.o: $(TARGETDIR_libopt.a) optimizer_mysql.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ optimizer_mysql.cpp

$(TARGETDIR_libopt.a)/statistics.o: $(TARGETDIR_libopt.a) statistics.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ statistics.cpp

$(TARGETDIR_libopt.a)/stmt_template.o: $(TARGETDIR_libopt.a) stmt_template.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ stmt_template.cpp

$(TARGETDIR_libopt.a)/op_select_to_mysql.o: $(TARGETDIR_libopt.a) op_select_to_mysql.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ op_select_to_mysql.cpp

$(TARGETDIR_libopt.a)/op_select_to_oracle.o: $(TARGETDIR_libopt.a) op_select_to_oracle.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ op_select_to_oracle.cpp
	
$(TARGETDIR_libopt.a)/op_select_to_mssql.o: $(TARGETDIR_libopt.a) op_select_to_mssql.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ op_select_to_mssql.cpp
	
$(TARGETDIR_libopt.a)/op_update_to_mysql.o: $(TARGETDIR_libopt.a) op_update_to_mysql.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ op_update_to_mysql.cpp	
	
$(TARGETDIR_libopt.a)/op_insert_to_mysql.o: $(TARGETDIR_libopt.a) op_insert_to_mysql.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ op_insert_to_mysql.cpp	
	
$(TARGETDIR_libopt.a)/op_delete_to_mysql.o: $(TARGETDIR_libopt.a) op_delete_to_mysql.cpp
	$(COMPILE.cc) $(CCFLAGS_libopt.a) $(CPPFLAGS_libopt.a) -o $@ op_delete_to_mysql.cpp
	
OBJS_test = 	
USERLIBS_test = ../base/libbase.a ../log/libjlog.a ../route/libroute.a \
	../connector/libconn.a libopt.a ../conf/libconf.a \
	-lmysqlclient -lmysqlcppconn-static ../sql/libsql.a \
	-Wl,-rpath -Wl,LIBDIR -lglog -llog4c \
	$(SYSLIBS_test) 
DEPLIBS_test = ../base/libbase.a  ../log/libjlog.a  ../connector/libconn.a  libopt.a  ../conf/libconf.a     
LDLIBS_test = $(USERLIBS_test)


$(TARGETDIR_test)/test: $(TARGETDIR_test) $(OBJS_test) $(DEPLIBS_test)
	$(LINK.cc) $(CCFLAGS_test) $(CPPFLAGS_test) -o $@ $(OBJS_test) $(LDLIBS_test)

$(TARGETDIR_test)/main.o: $(TARGETDIR_test) main.cpp
	$(COMPILE.cc) $(CCFLAGS_test) $(CPPFLAGS_test) -o $@ main.cpp



clean:
	rm -f \
		$(TARGETDIR_libopt.a)/libopt.a \
		$(TARGETDIR_libopt.a)/cs_db.o \
		$(TARGETDIR_libopt.a)/cs_field.o \
		$(TARGETDIR_libopt.a)/cs_table.o \
		$(TARGETDIR_libopt.a)/execute_plan.o \
		$(TARGETDIR_libopt.a)/optimizer.o \
		$(TARGETDIR_libopt.a)/optimizer_mysql.o \
		$(TARGETDIR_libopt.a)/statistics.o \
		$(TARGETDIR_libopt.a)/stmt_template.o \
		$(TARGETDIR_test)/test \
		$(TARGETDIR_test)/cs_db.o \
		$(TARGETDIR_test)/cs_field.o \
		$(TARGETDIR_test)/cs_table.o \
		$(TARGETDIR_test)/execute_plan.o \
		$(TARGETDIR_test)/main.o \
		$(TARGETDIR_libopt.a)/op_select_to_mysql.o \
		$(TARGETDIR_libopt.a)/op_select_to_mssql.o \
		$(TARGETDIR_libopt.a)/op_select_to_oracle.o \
		$(TARGETDIR_libopt.a)/op_insert_to_mysql.o \
		$(TARGETDIR_libopt.a)/op_update_to_mysql.o \
		$(TARGETDIR_libopt.a)/op_delete_to_mysql.o \
		$(TARGETDIR_libopt.a)/object_select.o 

.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

