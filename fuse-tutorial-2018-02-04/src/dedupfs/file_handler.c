#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "file_handler.h"

#define TRUE (0==0)
#define FALSE (!TRUE)

#define FILE_CONF "file_conf"
#define FILE_SIZE_BYTES 8

#define HASHES_SUPPORTED 2
static int hash_length[HASHES_SUPPORTED] = {16, 20};

static void get_full_path(struct file_handler_conf *conf, char *full_path, const char *relative_path) {
    snprintf(full_path, PATH_MAX, "%s%s", conf->full_files_path, relative_path);
}

// TODO
static int check_conf(struct file_handler_conf *conf);
static int write_conf_file(struct file_handler_conf *conf);
static int read_conf_file(struct file_handler_conf *conf, char *blocks_path);

// TODO
int file_handler_init(struct file_handler_conf *conf) {
    snprintf(conf->full_files_path, PATH_MAX, "%s/%s", conf->fs_path, conf->files_path);
    mkdir(conf->full_files_path, 0755);
    conf->block_handler.hash_type = conf->hash_type;
    conf->block_handler.hash_length = hash_length[conf->hash_type-1];
    conf->block_handler.bytes_link_counter = DEFAULT_BYTES_LINK_COUNTER;
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

// TODO check errors
int file_getattr(struct file_handler_conf *conf, const char *path, struct stat *stat_buf) {

    char full_path[PATH_MAX];
    int fd;
    int ret_value = 0;

    get_full_path(conf, full_path, path);

    if ((fd = open(full_path, O_CREAT | O_EXCL | O_WRONLY, mode)) == -1) {
        return -errno;
    }

    long file_size;
    if ((ret_value = read(fd, &file_size, FILE_SIZE_BYTES)) < FILE_SIZE_BYTES) {
        if (ret_value < 0) {
            ret_value = -errno;
        }
        else {
            // TODO
            ret_value = -EIO;
        }
    }

    if (close(fd) == -1) {
        return -errno;
    }

    if (lstat(full_path, stat_buf) == -1) {
        return -errno;
    }

    stat_buf->st_size = file_size;
    stat_buf->st_blksize = conf->block_handler.block_size;
    // TODO
//    stat_buf->st_blocks = (file_size/stat_buf->st_blksize) == 0 ? file_size/stat_buf->st_blksize : file_size/stat_buf->st_blksize + 1;
    stat_buf->st_blocks = (file_size/512) == 0 ? file_size/512 : file_size/512 + 1;

    return ret_value;
}

int file_mknod(struct file_handler_conf *conf, const char *path, mode_t mode) {

    // TODO delete if error?
    char full_path[PATH_MAX];
    int fd;
    int ret_value = 0;

    get_full_path(conf, full_path, path);

    if ((fd = open(full_path, O_CREAT | O_EXCL | O_WRONLY, mode)) == -1) {
        return -errno;
    }

    long file_size = 0;
    if ((ret_value = write(fd, &file_size, FILE_SIZE_BYTES)) < FILE_SIZE_BYTES) {
        if (ret_value < 0) {
            ret_value = -errno;
        }
        else {
            // TODO
            ret_value = -EIO;
        }
    }

    if (close(fd) == -1) {
        return -errno;
    }

    return ret_value;
}

int file_unlink(struct file_handler_conf *conf, const char *path);
int file_truncate(struct file_handler_conf *conf, const char *path, off_t new_size);
int file_open(struct file_handler_conf *conf, const char *path, int flags);
int file_read(struct file_handler_conf *conf, int fd, void *buf, size_t size, off_t offset);
int file_write(struct file_handler_conf *conf, int fd, const void *buf, size_t size, off_t offset);
int file_release(struct file_handler_conf *conf, int fd);
int file_fsync(struct file_handler_conf *conf, int fd);
int file_ftruncate(struct file_handler_conf *conf, int fd, off_t offset);
int file_fgetattr(struct file_handler_conf *conf, int fd, struct stat *stat_buf);


