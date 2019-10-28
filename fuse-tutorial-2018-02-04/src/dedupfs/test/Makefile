

bin/test_block: test_block.c bin/block_handler.o
	gcc -o bin/test_block bin/block_handler.o test_block.c

bin/block_handler.o: ../block_handler.h ../block_handler.c
	gcc -o bin/block_handler.o -c ../block_handler.c

bin/test_read_conf: test_read_conf.c ../block_handler.h
	gcc -o bin/test_read_conf test_read_conf.c

bin/test_write_conf: test_write_conf.c ../block_handler.h
	gcc -o bin/test_write_conf test_write_conf.c


clean:
	rm -rf bin/*
	mkdir bin/fs