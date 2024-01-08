#include <psp2/kernel/threadmgr.h>
#include <cerrno>
#include "pseudo_pipe.h"
#include "AFakeNative/AFakeNative_Utils.h"

#define PIPEFD_MARGIN 384
#define PIPEFD_MAX 64

#define MSGPIPE_MEMTYPE_USER_MAIN 0x40
#define MSGPIPE_THREAD_ATTR_PRIO (0x8 | 0x4)

typedef struct pipefd_internal {
    int readfd; // >=0 indicates that it's in use
    int writefd;
    int msgpipe;
    bool readable;
    bool writeable;
} pipefd_internal;

static pipefd_internal pipefd_pool[PIPEFD_MAX];
SceKernelLwMutexWork pipefd_pool_mutex = {{0xFEE1DEAD}};

int pseudo_pipe(int pipefd[2]) {
#ifdef DEBUG_PIPEFD
    ALOGD("pseudo_pipe: called\n");
#endif

    if (pipefd_pool_mutex.data[0] == 0xFEE1DEAD) {
        sceKernelCreateLwMutex(&pipefd_pool_mutex, "pipefd_pool_mutex", 0, 0, nullptr);
        sceKernelLockLwMutex(&pipefd_pool_mutex, 1, nullptr);

        for (int i = 0; i < PIPEFD_MAX; ++i) {
            pipefd_pool[i].readfd = -1;
            pipefd_pool[i].writefd = -1;
            pipefd_pool[i].msgpipe = -1;
            pipefd_pool[i].readable = false;
            pipefd_pool[i].writeable = false;
        }

        #ifdef DEBUG_PIPEFD
            ALOGD("pseudo_pipe: initialized the pool\n");
        #endif
    } else {
        sceKernelLockLwMutex(&pipefd_pool_mutex, 1, nullptr);
    }

    int ret = sceKernelCreateMsgPipe("pseudo_pipe", MSGPIPE_MEMTYPE_USER_MAIN, MSGPIPE_THREAD_ATTR_PRIO, 4 * 4096, NULL);
    if (ret < 0) {
        #ifdef DEBUG_PIPEFD
            ALOGD("pseudo_pipe: sceKernelCreateMsgPipe failed\n");
        #endif
        sceKernelUnlockLwMutex(&pipefd_pool_mutex, 1);
        return -1;
    }

    pipefd_internal * pipe = nullptr;
    for (int i = 0, u = 0; u < PIPEFD_MAX; i++, u+=2) {
        if (pipefd_pool[i].readfd == -1) {
            pipe = &pipefd_pool[i];

            pipe->readfd = u + PIPEFD_MARGIN;
            pipe->writefd = u + 1 + PIPEFD_MARGIN;
            pipe->readable = false;
            pipe->writeable = true;
            pipe->msgpipe = ret;

            break;
        }
    }

    if (!pipe) {
        sceKernelDeleteMsgPipe(ret);
        sceKernelUnlockLwMutex(&pipefd_pool_mutex, 1);
        errno = EMFILE;
        return -1;
    }

    pipefd[0] = pipe->readfd;
    pipefd[1] = pipe->writefd;

#ifdef DEBUG_PIPEFD
    ALOGD("pseudo_pipe: pipe<%i, %i> initialized", pipe->readfd, pipe->writefd);
#endif

    sceKernelUnlockLwMutex(&pipefd_pool_mutex, 1);
    return 0;
}

ssize_t pseudo_pipe_read(int fd, void *buf, size_t count) {
    if (pipefd_pool_mutex.data[0] == 0xFEE1DEAD) {
        return -1;
    }
    sceKernelLockLwMutex(&pipefd_pool_mutex, 1, NULL);

    pipefd_internal * pipe = nullptr;
    for (int i = 0; i < PIPEFD_MAX; i++) {
        if (pipefd_pool[i].readfd == fd) {
#ifdef DEBUG_PIPEFD
            ALOGD("pseudo_pipe_read: found pipe<%i, %i> for reading", pipefd_pool[i].readfd, pipefd_pool[i].writefd);
#endif
            pipe = &pipefd_pool[i];
            break;
        }
    }

    if (!pipe) {
        sceKernelUnlockLwMutex(&pipefd_pool_mutex, 1);
        errno = EINVAL;
        return -1;
    }
    ssize_t rlen = count;
    if (rlen > 4 * 4096) rlen = 4 * 4096;
    size_t pResult;
    ssize_t ret = sceKernelReceiveMsgPipe(pipe->msgpipe, buf, rlen, 1, &pResult, NULL);
    if (ret == 0) { ret = rlen; }

    if (pResult == 0) {
#ifdef DEBUG_PIPEFD
        ALOGD("pseudo_pipe_read: pipe<%i, %i> set as NOT readable", pipe->readfd, pipe->writefd);
#endif
        pipe->readable = false;
    }

#ifdef DEBUG_PIPEFD
    ALOGD("pseudo_pipe_read: pipe<%i, %i>, count %i, ret %i", pipe->readfd, pipe->writefd, count, ret);
#endif
    sceKernelUnlockLwMutex(&pipefd_pool_mutex, 1);
    return ret;
}

#define SCE_KERNEL_MSG_PIPE_MODE_FULL 0x00000001U

ssize_t pseudo_pipe_write(int fd, const void *buf, size_t count) {
    if (pipefd_pool_mutex.data[0] == 0xFEE1DEAD) {
        return -1;
    }
    sceKernelLockLwMutex(&pipefd_pool_mutex, 1, NULL);

    pipefd_internal * pipe = nullptr;
    for (int i = 0; i < PIPEFD_MAX; ++i) {
        if (pipefd_pool[i].writefd == fd) {
#ifdef DEBUG_PIPEFD
            ALOGD("pseudo_pipe_write: found pipe<%i, %i> for writing", pipefd_pool[i].readfd, pipefd_pool[i].writefd);
#endif
            pipe = &pipefd_pool[i];
            break;
        }
    }

    if (!pipe) {
        sceKernelUnlockLwMutex(&pipefd_pool_mutex, 1);
        errno = EINVAL;
        return -1;
    }

    size_t len = count;
    if (len > 4 * 4096) len = 4 * 4096;

    ssize_t ret = sceKernelSendMsgPipe(pipe->msgpipe, (void *)buf, len, SCE_KERNEL_MSG_PIPE_MODE_FULL, NULL, NULL);
    if (ret == 0) {
        ret = len;

#ifdef DEBUG_PIPEFD
        ALOGD("pseudo_pipe_write: pipe<%i, %i> set as readable", pipe->readfd, pipe->writefd);
#endif

        pipe->readable = true;
    }

    sceKernelUnlockLwMutex(&pipefd_pool_mutex, 1);
#ifdef DEBUG_PIPEFD
    ALOGD("pseudo_pipe_write: pipe<%i, %i>, count %i, ret %i", pipe->readfd, pipe->writefd, count, ret);
#endif
    return ret;
}

void pseudo_pipe_status(int fd, bool * is_readable, bool * is_writeable) {
    if (pipefd_pool_mutex.data[0] == 0xFEE1DEAD) {
        return;
    }
    sceKernelLockLwMutex(&pipefd_pool_mutex, 1, NULL);

    for (int u = 0; u < PIPEFD_MAX; u++) {
        if (pipefd_pool[u].writefd == fd || pipefd_pool[u].readfd == fd) {
            *is_readable = pipefd_pool[u].readable;
            *is_writeable = pipefd_pool[u].writeable;
            pipefd_pool[u].readable = false;
            pipefd_pool[u].writeable = false;
            break;
        }
    }

    sceKernelUnlockLwMutex(&pipefd_pool_mutex, 1);
}

bool is_pipe(int fd) {
    pipefd_internal * p = nullptr;

    sceKernelLockLwMutex(&pipefd_pool_mutex, 1, NULL);

    for (int i = 0; i < PIPEFD_MAX; ++i) {
        if (pipefd_pool[i].readfd == fd || pipefd_pool[i].writefd == fd) {
            p = &pipefd_pool[i];
            break;
        }
    }

    sceKernelUnlockLwMutex(&pipefd_pool_mutex, 1);

    return p != nullptr;
}
