CCC = g++
CXX = g++
BASICOPTS = -g -Wall -O3
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 

	
testlogin:testlogin.cpp
	${CCC} ${BASICOPTS} -o $@ $< ../server_connection_cnf.cpp ../login55.cpp \
	-Wl,-rpath -Wl,LIBDIR -lglog ../../log/libjlog.a ../../acl/libacl.a \
	-lmysqlclient ../../connector/mysql_con.o ../../protocol/libpl.a \
	../../listener/listener.o

	
clean:
	rm -f *.[ao] testlogin
	 
