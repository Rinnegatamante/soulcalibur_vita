#include <falso_jni/FalsoJNI_Impl.h>
#include <falso_jni/FalsoJNI_Logger.h>
#include <string.h>
#include <vitasdk.h>
#include <AFakeNative/keycodes.h>

// System-wide constant that's often used to determine Android version
// https://developer.android.com/reference/android/os/Build.VERSION.html#SDK_INT
// Possible values: https://developer.android.com/reference/android/os/Build.VERSION_CODES
const int SDK_INT = 19; // Android 4.4 / KitKat

/*
 * JNI Methods
*/

NameToMethodID nameToMethodId[] = {
	{ 1, "stringCatcher", METHOD_TYPE_VOID },
	{ 2, "licenseCheck", METHOD_TYPE_VOID }, 
	{ 3, "getLicenseResult", METHOD_TYPE_INT }, 
	{ 4, "expansionIsValid", METHOD_TYPE_BOOLEAN }, 
	{ 5, "getExpansionPath", METHOD_TYPE_OBJECT }, 
	{ 6, "gameHelperReady", METHOD_TYPE_VOID }, 
	{ 7, "playGameBoot", METHOD_TYPE_VOID }, 
	{ 8, "getLocale", METHOD_TYPE_OBJECT }, 
	{ 9, "getTouchScreenNum", METHOD_TYPE_INT }, 
	{ 10, "hasJoyStickMethods", METHOD_TYPE_BOOLEAN }, 
	{ 11, "getApiLevel", METHOD_TYPE_INT }, 
	{ 12, "getDataPath", METHOD_TYPE_OBJECT }, 
	{ 13, "getPadNum", METHOD_TYPE_INT }, 
	{ 14, "openPubData", METHOD_TYPE_VOID },
	{ 15, "closePubData", METHOD_TYPE_VOID },
	{ 16, "timeStampRequest", METHOD_TYPE_VOID },
	{ 17, "hasStartButton", METHOD_TYPE_BOOLEAN },
	{ 18, "getVersionName", METHOD_TYPE_OBJECT },
	{ 19, "confirmFinish", METHOD_TYPE_VOID },
	{ 20, "isJoyStick", METHOD_TYPE_BOOLEAN },
	{ 21, "getButtonList", METHOD_TYPE_INT },
	{ 22, "getConfirmResult", METHOD_TYPE_INT },
	{ 23, "disposeConfirmDialog", METHOD_TYPE_VOID },
	{ 24, "playGameIsSignedIn", METHOD_TYPE_BOOLEAN },
	{ 25, "getPubData", METHOD_TYPE_OBJECT },
	{ 26, "getPubLink", METHOD_TYPE_OBJECT },
	{ 27, "playGameSignInReq", METHOD_TYPE_VOID },
	{ 28, "webToMoreApp", METHOD_TYPE_VOID },
};

void stringCatcher(jmethodID id, va_list args) {
	jint arg1 = va_arg(args, jint);
	jstring arg2 = va_arg(args, jstring);
	fjni_logv_info("stringCatcher with %i, %s", arg1, arg2);
}

void dummy(jmethodID id, va_list args) {
	fjni_log_info("dummy");
}

jint getLicenseResult(jmethodID id, va_list args) {
	return 1;
}

jint getTouchScreenNum(jmethodID id, va_list args) {
	return 1;
}

jint getConfirmResult(jmethodID id, va_list args) {
	return 1;
}

/*
 A = 0x10
 B = 0x20
 X = 0x40
 Y = 0x80
 START = 0x100
 SELECT = 0x200
 HOME = 0x800
 BACK = 0x400
*/

jint getPadNum(jmethodID id, va_list args) {
	return 1;
}

jint getButtonList(jmethodID id, va_list args) {
	jint arg1 = va_arg(args, jint);
	printf("getButtonList(%d)\n", arg1);
	int r = 0;
	r += 0x10; // A
	r += 0x20; // B
	r += 0x40; // X
	r += 0x80; // Y
	r += 0x100; // Start
	r += 0x200; // Select
	return r;
}

jint getApiLevel(jmethodID id, va_list args) {
	return SDK_INT;
}

jboolean expansionIsValid(jmethodID id, va_list args) {
    return JNI_TRUE;
}

jboolean hasStartButton(jmethodID id, va_list args) {
    return JNI_TRUE;
}

jboolean playGameIsSignedIn(jmethodID id, va_list args) {
    return JNI_FALSE;
}

jboolean hasJoyStickMethods(jmethodID id, va_list args) {
    return JNI_TRUE;
}

jboolean isJoyStick(jmethodID id, va_list args) {
    return JNI_TRUE;
}

jobject getExpansionPath(jmethodID id, va_list args) {
	JavaDynArray * ret = jda_alloc(strlen("ux0:data/soulcalibur") + 1, FIELD_TYPE_BYTE);
    char *arr = ret->array;
	strcpy(arr, "ux0:data/soulcalibur");
    return (jobject)ret;
}

jobject getVersionName(jmethodID id, va_list args) {
	JavaDynArray * ret = jda_alloc(4, FIELD_TYPE_BYTE);
    char *arr = ret->array;
	strcpy(arr, "1.0");
    return (jobject)ret;
}

jobject getDataPath(jmethodID id, va_list args) {
	JavaDynArray * ret = jda_alloc(strlen("ux0:data/soulcalibur") + 1, FIELD_TYPE_BYTE);
    char *arr = ret->array;
	strcpy(arr, "ux0:data/soulcalibur");
    return (jobject)ret;
}

jobject getPubData(jmethodID id, va_list args) {
	JavaDynArray * ret = jda_alloc(strlen("Port by Rinnegatamante") + 1, FIELD_TYPE_BYTE);
    char *arr = ret->array;
	strcpy(arr, "Port by Rinnegatamante");
    return (jobject)ret;
}

jobject getPubLink(jmethodID id, va_list args) {
	JavaDynArray * ret = jda_alloc(strlen("https://vitadb.rinnegatamante.it") + 1, FIELD_TYPE_BYTE);
    char *arr = ret->array;
	strcpy(arr, "https://vitadb.rinnegatamante.it");
    return (jobject)ret;
}

jobject getLocale(jmethodID id, va_list args) {
	JavaDynArray * ret = jda_alloc(3, FIELD_TYPE_BYTE);
    char *arr = ret->array;
	strcpy(arr, "en");
    return (jobject)ret;
}

MethodsBoolean methodsBoolean[] = {
	{ 4, expansionIsValid }, 
	{ 10, hasJoyStickMethods }, 
	{ 17, hasStartButton },
	{ 20, isJoyStick },
	{ 24, playGameIsSignedIn },
};

MethodsByte methodsByte[] = {};
MethodsChar methodsChar[] = {};
MethodsDouble methodsDouble[] = {};
MethodsFloat methodsFloat[] = {};
MethodsLong methodsLong[] = {};
MethodsShort methodsShort[] = {};

MethodsInt methodsInt[] = {
	{ 3, getLicenseResult }, 
	{ 9, getTouchScreenNum },
	{ 11, getApiLevel },
	{ 13, getPadNum },
	{ 21, getButtonList },
	{ 22, getConfirmResult },
};

MethodsObject methodsObject[] = {
	{ 5, getExpansionPath }, 
	{ 8, getLocale },
	{ 12, getDataPath },
	{ 18, getVersionName },
	{ 25, getPubData },
	{ 26, getPubLink },
};

MethodsVoid methodsVoid[] = {
	{ 1, stringCatcher }, 
	{ 2, dummy }, // licenseCheck
	{ 6, dummy }, // gameHelperReady
	{ 7, dummy }, // playGameBoot
	{ 14, dummy }, // openPubData
	{ 15, dummy }, // closePubData
	{ 16, dummy }, // timeStampRequest
	{ 19, dummy }, // confirmFinish
	{ 23, dummy }, // disposeConfirmDialog
	{ 27, dummy }, // playGameSignInReq
	{ 28, dummy }, // webToMoreApp
};

/*
 * JNI Fields
*/

// System-wide constant that applications sometimes request
// https://developer.android.com/reference/android/content/Context.html#WINDOW_SERVICE
char WINDOW_SERVICE[] = "window";

NameToFieldID nameToFieldId[] = {
	{ 0, "WINDOW_SERVICE", FIELD_TYPE_OBJECT }, 
	{ 1, "SDK_INT", FIELD_TYPE_INT },
};

FieldsBoolean fieldsBoolean[] = {};
FieldsByte fieldsByte[] = {};
FieldsChar fieldsChar[] = {};
FieldsDouble fieldsDouble[] = {};
FieldsFloat fieldsFloat[] = {};
FieldsInt fieldsInt[] = {
	{ 1, SDK_INT },
};
FieldsObject fieldsObject[] = {
	{ 0, WINDOW_SERVICE },
};
FieldsLong fieldsLong[] = {};
FieldsShort fieldsShort[] = {};

__FALSOJNI_IMPL_CONTAINER_SIZES
