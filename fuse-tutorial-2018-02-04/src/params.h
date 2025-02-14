/*
  Copyright (C) 2012 Joseph J. Pfeiffer, Jr., Ph.D. <pfeiffer@cs.nmsu.edu>

  This program can be distributed under the terms of the GNU GPLv3.
  See the file COPYING.

  There are a couple of symbols that need to be #defined before
  #including all the headers.
*/

#ifndef _PARAMS_H_
#define _PARAMS_H_

// The FUSE API has been changed a number of times.  So, our code
// needs to define the version of the API that we assume.  As of this
// writing, the most current API version is 26
#define FUSE_USE_VERSION 26

// need this to get pwrite().  I have to use setvbuf() instead of
// setlinebuf() later in consequence.
#define _XOPEN_SOURCE 500

#define _POSIX_C_SOURCE 200809L

// maintain bbfs state in here
#include <limits.h>
#include <stdio.h>
#include <sys/types.h>
struct bb_state {
    FILE *logfile;
    char *rootdir;
    struct file_handler_conf *file_handler;
    char file_permissions_path[PATH_MAX];
    mode_t file_permissions_mode;
    pid_t file_permissions_pid;
};
#define BB_DATA ((struct bb_state *) fuse_get_context()->private_data)

#endif
