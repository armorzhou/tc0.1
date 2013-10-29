all:test_db test_schema acl_test 


acl_test:acl_test.cpp
	g++ -g -Wall -O3 -o $@ $< -lmysqlclient ../../connector/libconn.a \
	../libacl.a -Wl,-rpath -Wl,LIBDIR -lglog 
test_db:
	g++ -Wall -O3 -o test_db test_db.cpp ../acl_db.cpp ../acl_access.cpp \
        ../acl_convert.cpp ../../log/libjlog.a -Wl,-rpath -Wl,LIBDIR -lglog
test_schema:
	g++ -Wall -O3 -o test_schema test_schema.cpp -lmysqlclient \
	../acl_schema.cpp ../acl_db.cpp \
	../acl_access.cpp ../acl_convert.cpp ../../log/libjlog.a -Wl,-rpath -Wl,LIBDIR -lglog
clean:
	rm -f test_db test_schema acl_test
