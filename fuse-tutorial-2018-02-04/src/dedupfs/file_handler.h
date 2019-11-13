#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#define DEFAULT_FILES_PATH "files"

struct file_handler_conf {
    char fs_path[PATH_MAX];
    char files_path[PATH_MAX];
    char blocks_path[PATH_MAX];
    int block_size;
    int hash_type;
    int hash_length;
    int hash_split;
    int hash_split_size;
    int bytes_link_counter;
};



int file_handler_init(struct file_handler_conf *conf);

int file_getattr(struct file_handler_conf *conf, const char *path, struct stat *stat_buf);
int file_mknod(struct file_handler_conf *conf, const char *path, mode_t mode);
int file_unlink(struct file_handler_conf *conf, const char *path);
//int file_rename(struct file_handler_conf *conf, const char *path, const char *new_path);
//int file_link(struct file_handler_conf *conf, const char *path, const char *new_path);
int file_truncate(struct file_handler_conf *conf, const char *path, off_t new_size);
int file_open(struct file_handler_conf *conf, const char *path, int flags);
int file_read(struct file_handler_conf *conf, int fd, void *buf, size_t size, off_t offset);
int file_write(struct file_handler_conf *conf, int fd, const void *buf, size_t size, off_t offset);
//int file_flush(struct file_handler_conf *conf, int fd);
int file_release(struct file_handler_conf *conf, int fd);
int file_fsync(struct file_handler_conf *conf, int fd);
int file_ftruncate(struct file_handler_conf *conf, int fd, off_t offset);
int file_fgetattr(struct file_handler_conf *conf, int fd, struct stat *stat_buf);

#endif //FILE_HANDLER_H
