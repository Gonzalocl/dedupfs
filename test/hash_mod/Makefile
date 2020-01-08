



hash.o: hash.c get_hash.o
	gcc hash.c get_hash.o -o hash.o -lcrypto

get_hash.o: get_hash.c get_hash.h
	gcc -o get_hash.o -c get_hash.c -lcrypto

clean:
	rm -f get_hash.o hash.o
