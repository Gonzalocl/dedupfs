#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include "file_handler.h"

#define TRUE (0==0)
#define FALSE (!TRUE)

#define FILE_CONF "file_conf"

// TODO
static int check_conf(struct file_handler_conf *conf);
static int write_conf_file(struct file_handler_conf *conf);
static int read_conf_file(struct file_handler_conf *conf, char *blocks_path);

// TODO
int file_handler_init(struct file_handler_conf *conf) {

    return block_handler_init(conf->fs_path, &conf->block_handler);

//    char path[PATH_MAX];
//    int ret_value = 0;

//    if (snprintf(path, PATH_MAX, "%s/%s", conf->fs_path, FILE_CONF) < 0) {
//        return -EIO;
//    }

//    if (access(path, F_OK) == 0) {
//
//    }
//    else {
//
//    }



//    return ret_value;
}

int file_getattr(struct file_handler_conf *conf, const char *path, struct stat *stat_buf);
int file_mknod(struct file_handler_conf *conf, const char *path, mode_t mode);
int file_unlink(struct file_handler_conf *conf, const char *path);
int file_truncate(struct file_handler_conf *conf, const char *path, off_t new_size);
int file_open(struct file_handler_conf *conf, const char *path, int flags);
int file_read(struct file_handler_conf *conf, int fd, void *buf, size_t size, off_t offset);
int file_write(struct file_handler_conf *conf, int fd, const void *buf, size_t size, off_t offset);
int file_release(struct file_handler_conf *conf, int fd);
int file_fsync(struct file_handler_conf *conf, int fd);
int file_ftruncate(struct file_handler_conf *conf, int fd, off_t offset);
int file_fgetattr(struct file_handler_conf *conf, int fd, struct stat *stat_buf);


