



SRC := $(wildcard test_block_*.c)
OBJ := $(patsubst %.c, bin/%, $(SRC))


all: $(OBJ)

bin/%: %.c bin/block_handler.o
	gcc -o $@ bin/block_handler.o $<



bin/block_handler.o: ../block_handler.h ../block_handler.c
	gcc -o bin/block_handler.o -c ../block_handler.c

bin/test_read_conf: test_read_conf.c ../block_handler.h
	gcc -o bin/test_read_conf test_read_conf.c

bin/test_write_conf: test_write_conf.c ../block_handler.h
	gcc -o bin/test_write_conf test_write_conf.c


clean:
	rm -rf bin/*
	mkdir bin/fs