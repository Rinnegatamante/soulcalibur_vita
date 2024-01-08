#include <cstdlib>
#include <cstring>
#include "AConfiguration.h"

typedef struct configuration {
    char country[2] = {'\0', '\0'};
    char language[2] = {'\0', '\0'};
    int32_t orientation = ACONFIGURATION_ORIENTATION_ANY;
    int32_t density = ACONFIGURATION_DENSITY_DEFAULT;
    int32_t keyboard = ACONFIGURATION_KEYBOARD_ANY;
    int32_t keyshidden = ACONFIGURATION_KEYSHIDDEN_ANY;
    int32_t mcc = 0;
    int32_t mnc = 0;
    int32_t navhidden = ACONFIGURATION_NAVHIDDEN_ANY;
    int32_t navigation = ACONFIGURATION_NAVIGATION_ANY;
    int32_t screenlong = ACONFIGURATION_SCREENLONG_ANY;
    int32_t screensize = ACONFIGURATION_SCREENSIZE_ANY;
    int32_t sdk = SDK_CUPCAKE;
    int32_t touchscreen = ACONFIGURATION_TOUCHSCREEN_ANY;
    int32_t uimodenight = ACONFIGURATION_UI_MODE_NIGHT_ANY;
    int32_t uimodetype = ACONFIGURATION_UI_MODE_TYPE_ANY;
} configuration;

AConfiguration* AConfiguration_new() {
    configuration conf;

    // TODO: proper country code
    conf.country[0] = '\0';
    conf.country[1] = '\0';

    // TODO: proper language code
    conf.language[0] = '\0';
    conf.language[1] = '\0';

    conf.orientation = ACONFIGURATION_ORIENTATION_LAND;
    conf.density = ACONFIGURATION_DENSITY_HIGH;
    conf.keyboard = ACONFIGURATION_KEYBOARD_QWERTY;
    conf.keyshidden = ACONFIGURATION_KEYSHIDDEN_NO;
    conf.navhidden = ACONFIGURATION_NAVHIDDEN_YES;
    conf.navigation = ACONFIGURATION_NAVIGATION_DPAD;
    conf.sdk = SDK_HONEYCOMB;
    conf.screenlong = ACONFIGURATION_SCREENLONG_YES;
    conf.screensize = ACONFIGURATION_SCREENSIZE_LARGE;
    conf.touchscreen = ACONFIGURATION_TOUCHSCREEN_FINGER;
    conf.uimodenight = ACONFIGURATION_UI_MODE_NIGHT_NO;
    conf.uimodetype = ACONFIGURATION_UI_MODE_TYPE_NORMAL;

    auto ret = (AConfiguration *) malloc(sizeof(configuration));
    memcpy(ret, &conf, sizeof(configuration));

    return ret;
};

void AConfiguration_delete(AConfiguration* config) {
    if (config) free(config);
}

void AConfiguration_fromAssetManager(AConfiguration* out, AAssetManager* am) {
    // do nothing (we wrote all values we wanted in _new())?
}

int32_t AConfiguration_getOrientation(AConfiguration* config) {
    if (!config) return ACONFIGURATION_ORIENTATION_ANY;
    auto * conf = (configuration *) config;
    return conf->orientation;
}

void AConfiguration_getCountry(AConfiguration* config, char* outCountry) {
    if (!config) {
        outCountry[0] = '\0';
        outCountry[1] = '\0';
    }
    auto * conf = (configuration *) config;
    outCountry[0] = conf->country[0];
    outCountry[1] = conf->country[1];
}

int32_t AConfiguration_getDensity(AConfiguration* config) {
    if (!config) return ACONFIGURATION_DENSITY_DEFAULT;
    auto * conf = (configuration *) config;
    return conf->density;
}

int32_t AConfiguration_getKeyboard(AConfiguration* config) {
    if (!config) return ACONFIGURATION_KEYBOARD_ANY;
    auto * conf = (configuration *) config;
    return conf->keyboard;
}

int32_t AConfiguration_getKeysHidden(AConfiguration* config) {
    if (!config) return ACONFIGURATION_KEYSHIDDEN_ANY;
    auto * conf = (configuration *) config;
    return conf->keyshidden;
}

void AConfiguration_getLanguage(AConfiguration* config, char* outLanguage) {
    if (!config) {
        outLanguage[0] = '\0';
        outLanguage[1] = '\0';
    }
    auto * conf = (configuration *) config;
    outLanguage[0] = conf->language[0];
    outLanguage[1] = conf->language[1];
}

int32_t AConfiguration_getMcc(AConfiguration* config) {
    if (!config) return 0;
    auto * conf = (configuration *) config;
    return conf->mcc;
}

int32_t AConfiguration_getMnc(AConfiguration* config) {
    if (!config) return 0;
    auto * conf = (configuration *) config;
    return conf->mnc;
}

int32_t AConfiguration_getNavHidden(AConfiguration* config) {
    if (!config) return ACONFIGURATION_NAVHIDDEN_ANY;
    auto * conf = (configuration *) config;
    return conf->navhidden;
}

int32_t AConfiguration_getNavigation(AConfiguration* config) {
    if (!config) return ACONFIGURATION_NAVIGATION_ANY;
    auto * conf = (configuration *) config;
    return conf->navigation;
}

int32_t AConfiguration_getScreenLong(AConfiguration* config) {
    if (!config) return ACONFIGURATION_SCREENLONG_ANY;
    auto * conf = (configuration *) config;
    return conf->screenlong;
}

int32_t AConfiguration_getScreenSize(AConfiguration* config) {
    if (!config) return ACONFIGURATION_SCREENSIZE_ANY;
    auto * conf = (configuration *) config;
    return conf->screensize;
}

int32_t AConfiguration_getSdkVersion(AConfiguration* config) {
    if (!config) return SDK_CUPCAKE;
    auto * conf = (configuration *) config;
    return conf->sdk;
}

int32_t AConfiguration_getTouchscreen(AConfiguration* config) {
    if (!config) return ACONFIGURATION_TOUCHSCREEN_ANY;
    auto * conf = (configuration *) config;
    return conf->touchscreen;
}

int32_t AConfiguration_getUiModeNight(AConfiguration* config) {
    if (!config) return ACONFIGURATION_UI_MODE_NIGHT_ANY;
    auto * conf = (configuration *) config;
    return conf->uimodenight;
}

int32_t AConfiguration_getUiModeType(AConfiguration* config) {
    if (!config) return ACONFIGURATION_UI_MODE_TYPE_ANY;
    auto * conf = (configuration *) config;
    return conf->uimodetype;
}

void AConfiguration_setOrientation(AConfiguration* config, int32_t orientation) {
    if (!config) return;
    auto * conf = (configuration *) config;
    conf->orientation = orientation;
}
