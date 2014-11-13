all: exec

exec: cuckoo_search.o RngStream.o ConsFunctions.o StRnk.o
	gcc RngStream.o cuckoo_search.o ConsFunctions.o StRnk.o -O2  -o exec -lm -Wall

RngStream.o: RngStream.c RngStream.h
	gcc -c RngStream.c

cuckoo_search.o: cuckoo_search.c cuckoo_search.h
	gcc -c cuckoo_search.c

ConsFunctions.o: ConsFunctions.c ConsFunctions.h
	gcc -c ConsFunctions.c

StRnk.o: StRnk.c StRnk.h RngStream.h ConsFunctions.h common.h
	gcc -c StRnk.c

clean:
	rm -rf *o exec
