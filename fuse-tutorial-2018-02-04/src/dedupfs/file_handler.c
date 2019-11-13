#include "file_handler.h"

struct file_handler_conf *handler_conf = NULL;
char fs_path[PATH_MAX];


int file_handler_init(const char *fs, struct file_handler_conf *conf) {
    return 0;
}

int file_getattr(const char *path, struct stat *stat_buf);
int file_mknod(const char *path, mode_t mode);
int file_unlink(const char *path);
int file_truncate(const char *path, off_t new_size);
int file_open(const char *path, int flags);
int file_read(int fd, void *buf, size_t size, off_t offset);
int file_write(int fd, const void *buf, size_t size, off_t offset);
int file_release(int fd);
int file_fsync(int fd);
int file_ftruncate(int fd, off_t offset);
int file_fgetattr(int fd, struct stat *stat_buf);


