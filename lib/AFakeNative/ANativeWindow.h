#ifndef ANDROID_NATIVE_WINDOW_H
#define ANDROID_NATIVE_WINDOW_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ANativeWindow;
typedef struct ANativeWindow ANativeWindow;


/**
 * [Non-Standard]: Create new ANativeWindow object
 */
ANativeWindow * ANativeWindow_create();

/**
 * Return the current width in pixels of the window surface.  Returns a
 * negative value on error.
 */
int32_t ANativeWindow_getWidth(ANativeWindow* window);

/**
 * Return the current height in pixels of the window surface.  Returns a
 * negative value on error.
 */
int32_t ANativeWindow_getHeight(ANativeWindow* window);

/**
 * Return the current pixel format of the window surface.  Returns a
 * negative value on error.
 */
int32_t ANativeWindow_getFormat(ANativeWindow* window);

/**
 * Change the format and size of the window buffers.
 *
 * The width and height control the number of pixels in the buffers, not the
 * dimensions of the window on screen.  If these are different than the
 * window's physical size, then it buffer will be scaled to match that size
 * when compositing it to the screen.
 *
 * For all of these parameters, if 0 is supplied then the window's base
 * value will come back in force.
 *
 * width and height must be either both zero or both non-zero.
 *
 */
int32_t ANativeWindow_setBuffersGeometry(ANativeWindow* window,
                                         int32_t width, int32_t height, int32_t format);

#ifdef __cplusplus
};
#endif

#endif // ANDROID_NATIVE_WINDOW_H
