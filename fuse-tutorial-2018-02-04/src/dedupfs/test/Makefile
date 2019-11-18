



SRC := $(wildcard test_*.c)
OBJ := $(patsubst %.c, bin/%, $(SRC))


all: $(OBJ)


bin/test_%: test_%.c bin/block_handler.o bin/file_handler.o bin/block_cache.o
	gcc -o $@ bin/block_handler.o bin/file_handler.o bin/block_cache.o $<
bin/test_static_%: test_static_%.c
	gcc -o $@ $<




bin/block_handler.o: ../block_handler.h ../block_handler.c
	gcc -o bin/block_handler.o -c ../block_handler.c

bin/file_handler.o: ../file_handler.h ../file_handler.c
	gcc -o bin/file_handler.o -c ../file_handler.c

bin/block_cache.o: ../block_cache.h ../block_cache.c
	gcc -o bin/block_cache.o -c ../block_cache.c



clean:
	rm -rf bin/*
	mkdir bin/fs