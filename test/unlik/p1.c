#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {

    int fd;
    char c;
    int wwrite, rread, ret;

    if (argc != 2) {
        fprintf(stderr, "ERROR:%s:%d param\n", __FILE__, __LINE__);
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    if ((fd = open(argv[1], O_RDWR)) < 0) {
        fprintf(stderr, "ERROR:%s:%d :: %d\n", __FILE__, __LINE__, fd);
        return EXIT_FAILURE;
    }

    scanf("%c", &c);
    while (c != 'c') {
        wwrite = rand();
        if ((ret = lseek(fd, 0, SEEK_SET)) < 0) {
            fprintf(stderr, "ERROR:%s:%d :: %d\n", __FILE__, __LINE__, ret);
            return EXIT_FAILURE;
        }
        if ((ret = write(fd, &wwrite, 4)) < 0) {
            fprintf(stderr, "ERROR:%s:%d :: %d\n", __FILE__, __LINE__, ret);
            return EXIT_FAILURE;
        }
        printf("write : %d\n", wwrite);
        scanf("%c", &c);
        if ((ret = lseek(fd, 0, SEEK_SET)) < 0) {
            fprintf(stderr, "ERROR:%s:%d :: %d\n", __FILE__, __LINE__, ret);
            return EXIT_FAILURE;
        }
        if ((ret = read(fd, &rread, 4)) < 0) {
            fprintf(stderr, "ERROR:%s:%d :: %d\n", __FILE__, __LINE__, ret);
            return EXIT_FAILURE;
        }
        printf("read  : %d\n", rread);
        scanf("%c", &c);
        if (wwrite != rread) {
            fprintf(stderr, "ERROR:%s:%d:ERROR\n", __FILE__, __LINE__);
            return EXIT_FAILURE;
        }
    }

    if ((ret = close(fd)) < 0) {
        fprintf(stderr, "ERROR:%s:%d :: %d\n", __FILE__, __LINE__, ret);
        return EXIT_FAILURE;
    }


}




