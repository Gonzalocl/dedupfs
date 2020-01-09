#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#include "block_handler.h"

#define DEFAULT_FILES_PATH "files"
#define DEFAULT_FILE_OPEN_MAX 4096

struct file_handler_conf {
    char fs_path[PATH_MAX];
    char files_path[PATH_MAX];
    char full_files_path[PATH_MAX];
    int hash_type;
    struct block_handler_conf block_handler;
    int file_open_max;
    struct file_descriptor **file_descriptors;
    int fd_counter;
    unsigned char *zero_block_hash;
};



int file_handler_init(struct file_handler_conf *conf);
// TODO
int file_handler_end(struct file_handler_conf *conf);

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

int file_get_block_hash(struct file_handler_conf *conf, int fd, long block, unsigned char *hash);
int file_set_block_hash(struct file_handler_conf *conf, int fd, long block, const unsigned char *hash);

// TODO public?
int file_get_size(struct file_handler_conf *conf, int fd, long *file_size);
int file_set_size(struct file_handler_conf *conf, int fd, const long file_size);

#endif //FILE_HANDLER_H
