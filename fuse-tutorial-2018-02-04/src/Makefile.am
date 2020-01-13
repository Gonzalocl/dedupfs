bin_PROGRAMS = bbfs
bbfs_SOURCES = bbfs.c log.c log.h  params.h dedupfs/file_handler.c dedupfs/file_handler.h dedupfs/block_handler.c dedupfs/block_handler.h dedupfs/get_hash.c dedupfs/get_hash.h dedupfs/block_cache.c dedupfs/block_cache.h
AM_CFLAGS = @FUSE_CFLAGS@
LDADD = @FUSE_LIBS@ -lcrypto
