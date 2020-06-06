



SRC := $(wildcard test_*.c)
OBJ := $(patsubst %.c, bin/%, $(SRC))


all: init $(OBJ)


bin/test_%: test_%.c bin/block_handler.o bin/file_handler.o bin/block_cache.o bin/get_hash.o
	gcc -g -o $@ bin/block_handler.o bin/file_handler.o bin/block_cache.o bin/get_hash.o $< -lcrypto
bin/test_static_%: test_static_%.c
	gcc -g -o $@ $<




bin/block_handler.o: ../block_handler.h ../block_handler.c
	gcc -g -o bin/block_handler.o -c ../block_handler.c

bin/file_handler.o: ../file_handler.h ../file_handler.c
	gcc -g -o bin/file_handler.o -c ../file_handler.c

bin/block_cache.o: ../block_cache.h ../block_cache.c
	gcc -g -o bin/block_cache.o -c ../block_cache.c

bin/get_hash.o: ../get_hash.h ../get_hash.c
	gcc -g -o bin/get_hash.o -c ../get_hash.c

init:
	mkdir -p bin/fs

clean:
	rm -rf bin/*
