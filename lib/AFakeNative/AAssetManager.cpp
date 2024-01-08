#include "AAssetManager.h"
#include "AFakeNative_Utils.h"

#include <pthread.h>
#include <malloc.h>
#include <cstring>
#include <cstdio>
#include <libc_bridge/libc_bridge.h>
#include <string>

typedef struct assetManager {
    int dummy = 0; // TODO: mb we will need to store something here in future
    pthread_mutex_t mLock;
} assetManager;

typedef struct aAsset {
    char * filename;
    FILE* f;
} asset;

static AAssetManager * g_AAssetManager = nullptr;

AAssetManager * AAssetManager_create() {
    if (g_AAssetManager) return g_AAssetManager;

    assetManager am;

    pthread_mutex_init(&am.mLock, nullptr);

    g_AAssetManager = (AAssetManager *) malloc(sizeof(assetManager));
    memcpy(g_AAssetManager, &am, sizeof(assetManager));

    return g_AAssetManager;
}

AAsset* AAssetManager_open(AAssetManager* mgr, const char* filename, int mode) {
    std::string realp = std::string(DATA_PATH) + std::string("assets/") + std::string(filename);

    auto * a = (aAsset *) malloc(sizeof(aAsset));
    a->filename = (char *) malloc(realp.length() + 1);
    strcpy(a->filename, realp.c_str());

#ifdef USE_SCELIBC_IO
    a->f = sceLibcBridge_fopen((const char *)a->filename, "r");
#else
    a->f = fopen((cost char *)a->filename, "r");
#endif

    if (!a->f) {
        free(a->filename);
        free(a);
        a = nullptr;
    }

    ALOGD("[AAssetManager] AAssetManager_open(%p, %s, %i): %p", mgr, realp.c_str(), mode, a);
    return (AAsset *) a;
}

void AAsset_close(AAsset* asset) {
    //ALOGD("AAsset_close(%p)", asset);

    if (asset) {
        auto * a = (aAsset *) asset;
        free(a->filename);
        fclose(a->f);
        free(a);
    }
}

int AAsset_read(AAsset* asset, void* buf, size_t count) {
    //ALOGD("AAsset_read(%p, %p, %i)", asset, buf, count);

    if (!asset) {
        return -1;
    }

    auto * a = (aAsset *) asset;

#ifdef USE_SCELIBC_IO
    size_t ret = sceLibcBridge_fread(buf, 1, count, a->f);
#else
    size_t ret = fread(buf, 1, count, a->f);
#endif

    if (ret > 0) {
        return (int) ret;
    } else {
#ifdef USE_SCELIBC_IO
        if (ret == 0 || sceLibcBridge_feof(a->f)) {
#else
        if (ret == 0 || feof(a->f)) {
#endif
            return 0;
        } else {
            return -1;
        }
    }
}

off_t AAsset_seek(AAsset* asset, off_t offset, int whence) {
    //ALOGD("AAsset_seek(%p, %d, %i)", asset, offset, whence);

    if (!asset) {
        return (off_t) -1;
    }

    auto * a = (aAsset *) asset;

#ifdef USE_SCELIBC_IO
    auto ret = (off_t) sceLibcBridge_fseek(a->f, offset, whence);
#else
    auto ret = (off_t) fseek(a->f, offset, whence);
#endif

    return ret;
}
