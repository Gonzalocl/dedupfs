#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "file_handler.h"
#include "block_cache.h"

#define TRUE (0==0)
#define FALSE (!TRUE)

#define FILE_CONF "file_conf"
#define FILE_SIZE_BYTES 8

#define HASHES_SUPPORTED 2
static int hash_length[HASHES_SUPPORTED] = {16, 20};


struct file_descriptor {
    int index_fd;
    struct block_cache *cache;
};


static void get_full_path(struct file_handler_conf *conf, char *full_path, const char *relative_path) {
    snprintf(full_path, PATH_MAX, "%s%s", conf->full_files_path, relative_path);
}

// TODO test this
static int next_fd(struct file_handler_conf *conf) {
    int fd = conf->fd_counter;
    if (conf->file_descriptors[fd] == NULL) {
        conf->fd_counter = (fd + 1) % conf->file_open_max;
        return fd;
    }

    fd = (fd + 1) % conf->file_open_max;

    while (conf->file_descriptors[fd] != NULL && fd != conf->fd_counter)
        fd = (fd + 1) % conf->file_open_max;

    if (fd == conf->fd_counter) {
        return -1;
    }

    conf->fd_counter = (fd + 1) % conf->file_open_max;
    return fd;
}

// TODO
static int check_conf(struct file_handler_conf *conf);
static int write_conf_file(struct file_handler_conf *conf);
static int read_conf_file(struct file_handler_conf *conf, char *blocks_path);

// TODO
int file_handler_init(struct file_handler_conf *conf) {
    if (snprintf(conf->full_files_path, PATH_MAX, "%s/%s", conf->fs_path, conf->files_path) < 0) {
        return -EIO;
    }
    mkdir(conf->full_files_path, 0755);
    conf->block_handler.hash_type = conf->hash_type;
    conf->block_handler.hash_length = hash_length[conf->hash_type-1];
    conf->block_handler.bytes_link_counter = DEFAULT_BYTES_LINK_COUNTER;
    conf->file_open_max = DEFAULT_FILE_OPEN_MAX;
    conf->fd_counter = 0;
    if ((conf->file_descriptors = malloc(conf->file_open_max * sizeof(struct file_descriptor *))) == NULL) {
        return -errno;
    }
    memset(conf->file_descriptors, 0, conf->file_open_max * sizeof(struct block_cache *));
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

    if ((fd = open(full_path, O_RDONLY)) == -1) {
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
    // TODO ?
    stat_buf->st_blksize = conf->block_handler.block_size;
    stat_buf->st_blocks = (file_size % stat_buf->st_blksize) == 0 ? file_size/stat_buf->st_blksize : file_size/stat_buf->st_blksize + 1;
//    stat_buf->st_blksize = 512;
//    stat_buf->st_blocks = (file_size % 512) == 0 ? file_size/512 : file_size/512 + 1;

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
int file_open(struct file_handler_conf *conf, const char *path, int flags) {
    char full_path[PATH_MAX];
    int fd;
    int ret_value = 0;

    get_full_path(conf, full_path, path);

    fd = next_fd(conf);

    conf->file_descriptors[fd] = malloc(sizeof(struct file_descriptor));

    // TODO flags?
    if ((conf->file_descriptors[fd]->index_fd = open(full_path, O_RDWR)) == -1) {
        return -errno;
    }

    //TODO size
    conf->file_descriptors[fd]->cache = cache_init(fd, 0, conf);

    return fd;
}

int file_read(struct file_handler_conf *conf, int fd, void *buf, size_t size, off_t offset) {

}

int file_write(struct file_handler_conf *conf, int fd, const void *buf, size_t size, off_t offset) {

}

int file_release(struct file_handler_conf *conf, int fd) {
    close(conf->file_descriptors[fd]->index_fd);
    cache_end(conf->file_descriptors[fd]->cache);
    free(conf->file_descriptors[fd]);
}

int file_fsync(struct file_handler_conf *conf, int fd);
int file_ftruncate(struct file_handler_conf *conf, int fd, off_t offset);
int file_fgetattr(struct file_handler_conf *conf, int fd, struct stat *stat_buf);

int file_get_block_hash(struct file_handler_conf *conf, int fd, long block, unsigned char *hash) {
    lseek(conf->file_descriptors[fd]->index_fd, FILE_SIZE_BYTES + (block*hash_length[conf->hash_type-1]), SEEK_SET);
    read(conf->file_descriptors[fd]->index_fd, hash, hash_length[conf->hash_type-1]);
    return 0;
}

// TODO check errors
int file_set_block_hash(struct file_handler_conf *conf, int fd, long block, const unsigned char *hash) {
    lseek(conf->file_descriptors[fd]->index_fd, FILE_SIZE_BYTES + (block*hash_length[conf->hash_type-1]), SEEK_SET);
    write(conf->file_descriptors[fd]->index_fd, hash, hash_length[conf->hash_type-1]);
    return 0;
}

