#include <cstdlib>
#include "ANativeActivity.h"

#include <falso_jni/FalsoJNI.h>

ANativeActivity * ANativeActivity_create() {
    auto * ret = (ANativeActivity *) malloc(sizeof(ANativeActivity));
    ret->callbacks = (ANativeActivityCallbacks *) malloc(sizeof(ANativeActivityCallbacks));
    ret->env = &jni;
    ret->vm = &jvm;
    ret->clazz = (jclass) 0x42424242;
    ret->internalDataPath = DATA_PATH"assets/";
    ret->externalDataPath = DATA_PATH"assets/";
    ret->sdkVersion = 14;
    ret->instance = nullptr;

    return ret;
}

void ANativeActivity_setWindowFlags(ANativeActivity* activity, uint32_t addFlags, uint32_t removeFlags) {
    // see Android's window.h for flags reference.
    // they are pretty much useless for us because we are always fullscreen, focusable, etc.
}

void ANativeActivity_finish(ANativeActivity* activity) {
    free(activity);
}
