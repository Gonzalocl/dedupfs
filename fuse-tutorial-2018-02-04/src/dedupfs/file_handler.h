#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H






int file_init();

int file_getattr(const char *path, struct stat *stat_buf);
int file_create(const char *path, mode_t mode);
int file_unlink(const char *path);
//int file_rename(const char *path, const char *new_path);
//int file_link(const char *path, const char *new_path);
int file_truncate(const char *path, off_t new_size);
int file_open(const char *path, char *mode);




#endif //FILE_HANDLER_H
