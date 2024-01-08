#pragma once

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int pseudo_pipe(int pipefd[2]);

bool is_pipe(int fd);
ssize_t pseudo_pipe_read(int fd, void *buf, size_t count);
ssize_t pseudo_pipe_write(int fd, const void *buf, size_t count);
void pseudo_pipe_status(int fd, bool *is_readable, bool *is_writeable);

#ifdef __cplusplus
};
#endif
