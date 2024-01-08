#pragma once

#include <cstdint>
#include <psp2/kernel/threadmgr.h>
#include <malloc.h>
#include <cerrno>
#include <cstdio>
#include <sys/unistd.h>
#include "AFakeNative/PseudoEpoll.h"

#ifdef __cplusplus
extern "C" {
#endif

int pseudo_eventfd(unsigned int initval, int flags);

bool is_eventfd(int fd);
ssize_t pseudo_eventfd_read(int fd, void *buf, size_t count);
ssize_t pseudo_eventfd_write(int fd, const void *buf, size_t count);
void pseudo_eventfd_status(int fd, bool *is_readable, bool *is_writeable);

#ifdef __cplusplus
};
#endif