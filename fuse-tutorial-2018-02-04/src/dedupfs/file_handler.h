#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#define DEFAULT_FILES_PATH "files"

struct file_handler_conf {
    char files_path[PATH_MAX];
    char blocks_path[PATH_MAX];
    int block_size;
    int hash_type;
    int hash_length;
    int hash_split;
    int hash_split_size;
    int bytes_link_counter;
};



int file_handler_init(const char *fs, struct file_handler_conf *conf);

int file_getattr(const char *path, struct stat *stat_buf);
int file_mknod(const char *path, mode_t mode);
int file_unlink(const char *path);
//int file_rename(const char *path, const char *new_path);
//int file_link(const char *path, const char *new_path);
int file_truncate(const char *path, off_t new_size);
int file_open(const char *path, const char *mode);
int file_read(int fd, void *buf, size_t size, off_t offset);
int file_write(int fd, const void *buf, size_t size, off_t offset);
//int file_flush(int fd);
int file_release(int fd);
int file_fsync(int fd);
int file_ftruncate(int fd, off_t offset);
int bb_fgetattr(int fd, struct stat *stat_buf);

#endif //FILE_HANDLER_H
