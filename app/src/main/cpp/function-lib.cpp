#include <jni.h>
#include <string>
#include<android/log.h>
#include <android/bitmap.h>  //引入头文件还需要在cmake中引入jnigraphics

#define TAG "jni" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型

extern "C"
JNIEXPORT void JNICALL
Java_com_yjz_ndk_sample_fragment_function_StaticAndDynamicRegisterFragment_staticRegister(JNIEnv *env,
                                                                                          jobject instance) {
    LOGD("staticRegister jni");
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_yjz_ndk_sample_fragment_function_StaticAndDynamicRegisterFragment_getStringFromJni(JNIEnv *env,
                                                                                            jobject instance,
                                                                                            jint type) {
    LOGD("getStringFromJni jni %d", type);
    //return env->NewStringUTF("jni return " + type);//TODO 怎么实现类似java中各种类型的字符串拼接
    return env->NewStringUTF("jni return end");
}

/**
 * 动态调用示例
 *
 * 1.java层声明external方法dynamicRegister、getIntFromJni
 * 2.native层声音要被调用的方法dynamicRegisterFunc、getInt
 * 3.创建JNINativeMethod函数列表数组建立java和native2者之间函数的联系
 * 4.native层重写JNI_OnLoad方法
 * 5.在JNI_OnLoad中通过RegisterNatives进行动态注册
 */
void dynamicRegisterFunc() {
    LOGD("dynamicRegisterFunc");
}

int getInt(JNIEnv *env,jobject instance,jboolean isSuccess) {
    //LOGD("getInt %b", isSuccess); //TODO 怎么方便打印jboolean为true或者false
    LOGD("getInt");
    return isSuccess == true ? 1 : 0;
}

//3.创建JNINativeMethod函数列表数组建立java和native2者之间函数的联系
static JNINativeMethod jniNativeMethods[] = {
        //查看定义可以知道JNINativeMethod结构体入参为:
        //java函数名、函数签名、native层函数指针
        {"dynamicRegister", "()V", (void*)dynamicRegisterFunc},
        {"getIntFromJni", "(Z)I", (void*)getInt}
};
//4.native层重写JNI_OnLoad方法
/**
 * so 被初次加载的时候会被调用一次
 * @param vm
 * @param reserved
 * @return
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGD("JNI_OnLoad");
    //5.在JNI_OnLoad中通过RegisterNatives进行动态注册

    //a.首先从vm获取jnienv对象
    JNIEnv* jniEnv = NULL;
    //TODO reinterpret_cast,const_cast,static_cast,dynamic_cast 显式类型转换
    if (vm ->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6) != JNI_OK) {
        LOGD("JNI_OnLoad GetEnv error!");
        return JNI_ERR;
    }
    assert(jniEnv != NULL);
    //b.首先需要指明external方法所在的java类,通过FindClass 方法来找到对应的类
    const char* className  = "com/yjz/ndk/sample/fragment/function/StaticAndDynamicRegisterFragment";
    jclass clazz = jniEnv -> FindClass(className);
    if (clazz == NULL) {
        LOGD("JNI_OnLoad FindClass error!");
        return JNI_ERR;
    }
    //c.通过RegisterNatives进行动态注册
    jint result = jniEnv -> RegisterNatives(clazz, jniNativeMethods, sizeof(jniNativeMethods)/ sizeof(jniNativeMethods[0]));
    if (result != JNI_OK) {
        LOGD("JNI_OnLoad RegisterNatives error!");
        return JNI_ERR;
    }
    //返回jni 的版本
    return JNI_VERSION_1_6;
}
