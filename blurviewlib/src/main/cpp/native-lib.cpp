#include <jni.h>
#include <string>
#include<android/log.h>
#include <android/bitmap.h>  //引入头文件还需要在cmake中引入jnigraphics
#include "blur.h"
#define TAG "yy"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
/**
 * <android/bitmap.h>提供了函数接口定义
 * jnigraphics对应具体的实现，具体怎么确定那个jni和哪个so，可以参照官方文档介绍
 * https://developer.android.google.cn/ndk/guides/stable_apis
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_ndksample_blurviewlib_BlurView_blur(JNIEnv *env, jobject instance, jobject bitmap, jint r) {
    LOGD("jni blur");

    //获取图片信息
    AndroidBitmapInfo bitmapInfo;
    int result = AndroidBitmap_getInfo(env, bitmap, &bitmapInfo);
    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGD("get bitmap error!!!");
        return;
    }

    //获取图片像素点地址
    void* addrPtr;
    result = AndroidBitmap_lockPixels(env, bitmap, &addrPtr);
    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGD("lockPixels error!!!");
        return;
    }
    //对图片进行高斯滤波达到模糊的效果
    int width = bitmapInfo.width;
    int height = bitmapInfo.height;
    if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ) {
        addrPtr = blur_ARGB_8888((int *)addrPtr, width, height, r);
    } else if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        addrPtr = blur_RGB_565((short *)addrPtr, width, height, r);
    } else {
        LOGD("bitmap format error!!!");
    }
    //对应lock
    AndroidBitmap_unlockPixels(env, bitmap);
}