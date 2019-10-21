

bin/test_block: test_block.c bin/block_handler.o
	gcc -o bin/test_block bin/block_handler.o test_block.c

bin/block_handler.o: ../block_handler.h ../block_handler.c
	gcc -o bin/block_handler.o -c ../block_handler.c



clean:
	rm -rf bin/*