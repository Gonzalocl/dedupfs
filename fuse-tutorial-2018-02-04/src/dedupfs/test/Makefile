



SRC := $(wildcard test_*.c)
OBJ := $(patsubst %.c, bin/%, $(SRC))


all: $(OBJ)

bin/%: %.c bin/block_handler.o bin/file_handler.o
	gcc -o $@ bin/block_handler.o bin/file_handler.o $<



bin/block_handler.o: ../block_handler.h ../block_handler.c
	gcc -o bin/block_handler.o -c ../block_handler.c

bin/file_handler.o: ../file_handler.h ../file_handler.c
	gcc -o bin/file_handler.o -c ../file_handler.c



clean:
	rm -rf bin/*
	mkdir bin/fs