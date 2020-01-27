#include <jni.h>
#include <string>
#include<android/log.h>
#include <pthread.h>
#include <unistd.h> // sleep 的头文件

#define TAG "jni-exception-handle" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型

/**
 * 测试捕获java抛出的异常
 *
 * @param env
 * @param instance
 */
void javaThrowException(JNIEnv* env, jobject instance) {
    LOGD("jni javaThrowException");
    jclass cls = env -> GetObjectClass(instance);
    jmethodID method = env -> GetMethodID(cls, "throwException", "()V");
    LOGD("jni 调用java异常接口");
    env -> CallVoidMethod(instance, method);
    LOGD("jni 调用java异常接口结束");

    /** ExceptionCheck判断是否存在异常 **/
//    if (env -> ExceptionCheck()) {
//        env -> DeleteLocalRef(cls);
//        env -> ExceptionDescribe();
//        env -> ExceptionClear();
//        LOGD("jni 清除异常");
//    }

/** ExceptionOccurred判断是否存在异常 **/
    if(env -> ExceptionOccurred()) {
        env -> DeleteLocalRef(cls);
        env -> ExceptionDescribe();
        env -> ExceptionClear();
        LOGD("jni 清除异常");
    }

}
/**
 * 测试jni主动抛出异常
 * @param env
 * @param instance
 */
void nativeThrowException(JNIEnv* env, jobject instance) {
    LOGD("jni nativeThrowException");
    jclass exception = env -> FindClass("java/lang/NullPointerException");
    if (exception != NULL) {
        LOGD("jni nativeThrowException throw");
        env -> ThrowNew(exception, "jni 主动抛出抛出异常");
    }
    env -> DeleteLocalRef(exception);//释放局部引用
    LOGD("jni nativeThrowException end");
}

/**
 * 发生了异常，只允许调用资源释放和异常检查相关jni接口，其它接口会导致崩溃
 *
 * @param env
 * @param instance
 */
void callLimitInterface(JNIEnv* env, jobject instance) {
    LOGD("jni callLimitInterface");
    jclass cls = env -> GetObjectClass(instance);
    jmethodID method = env -> GetMethodID(cls, "throwException", "()V");
    LOGD("jni 调用java异常接口");
    env -> CallVoidMethod(instance, method);
    LOGD("jni 调用java异常接口结束");
    env -> GetObjectClass(instance);
    LOGD("jni end");
}


static JavaVM *javaVM = NULL;
jobject globalObject;
void* newThread(void *) {
    LOGD("启动线程");
    sleep(10);//休眠10秒
    LOGD("休眠结束");
    JNIEnv *env = NULL;
    // 将当前线程添加到 Java 虚拟机上
    if (javaVM -> AttachCurrentThread(&env, NULL) == 0) {
        //jclass beenCls = (jclass)env -> FindClass("java/lang/String");
        jclass cls = env -> GetObjectClass(globalObject);
        jmethodID method = env -> GetMethodID(cls, "throwException", "()V");
        LOGD("jni 线程调用java异常接口");
        env -> CallVoidMethod(globalObject, method);
        LOGD("jni 线程调用java异常接口结束");
        // 从 Java 虚拟机上分离当前线程
        javaVM -> DetachCurrentThread();
        LOGD("线程处理结束");
    } else {
        LOGD("AttachCurrentThread error");
    }
    return NULL;
}

/**
 *
 *
 * @param env
 * @param instance
 */

void test(JNIEnv* env, jobject instance) {
    LOGD("jni test");
//    pthread_t handles; //线程句柄
//    int result = pthread_create(&handles, NULL, newThread, NULL);
//    if (result != 0) {
//        LOGD("create thread failed");
//    } else {
//        LOGD("create thread success");
//    }
//    globalObject = env -> NewGlobalRef(instance);
//    jclass cls = env -> GetObjectClass(instance);
//    jmethodID method = env -> GetMethodID(cls, "throwException", "()V");
//    LOGD("jni 调用java异常接口");
//    env -> CallVoidMethod(instance, method);
//    LOGD("jni 调用java异常接口结束");


/************Throw APId的应用**********************************/
/*
    jclass cls = env -> GetObjectClass(instance);
    jmethodID method = env -> GetMethodID(cls, "throwException", "()V");
    LOGD("jni 调用java异常接口");
    env -> CallVoidMethod(instance, method);
    LOGD("jni 调用java异常接口结束");

    jthrowable throwable = env -> ExceptionOccurred();
    if(throwable != NULL) {
        LOGD("jni 异常处理");
        //先清空异常
        env -> ExceptionClear();
        //....处理相关事务
        jclass cls2 = env -> GetObjectClass(instance);
        env -> ExceptionDescribe();
        env -> Throw(throwable);//再次抛出异常
    }
    env -> DeleteLocalRef(cls);
    LOGD("jni end");
 打印出：
    jni test
    jni 调用java异常接口
    java start throw exception
    jni 调用java异常接口结束
    jni 异常处理
    jni end
    java catch exception:测试抛出异常
    */


/************FatalError APId的应用**********************************/
/*
    env ->FatalError("jni 致命错误");
    LOGD("jni end"); // FatalError会立即停止运行，这句话不会被执行到

    打印出:
    art/runtime/jni_internal.cc:769] JNI FatalError called: jni 致命错误
    art/runtime/runtime.cc:290] Runtime aborting...
    ...
*/
}

static JNINativeMethod jniNativeMethods[] = {
        {"javaThrowException", "()V", (void*)javaThrowException},
        {"nativeThrowException", "()V", (void*)nativeThrowException},
        {"test", "()V", (void*)test},
        {"callLimitInterface", "()V", (void*)callLimitInterface}
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGD("JNI_OnLoad");
    JNIEnv* jniEnv = NULL;
    if (vm ->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6) != JNI_OK) {
        LOGD("JNI_OnLoad GetEnv error!");
        return JNI_ERR;
    }
    assert(jniEnv != NULL);
    const char* className  = "com/yjz/ndk/sample/fragment/function/ExceptionHandleFragment";
    jclass clazz = jniEnv -> FindClass(className);
    if (clazz == NULL) {
        LOGD("JNI_OnLoad FindClass error!");
        return JNI_ERR;
    }

    jint result = jniEnv -> RegisterNatives(clazz, jniNativeMethods, sizeof(jniNativeMethods)/ sizeof(jniNativeMethods[0]));
    if (result != JNI_OK) {
        LOGD("JNI_OnLoad RegisterNatives error!");
        return JNI_ERR;
    }
    javaVM = vm;
    //返回jni 的版本
    return JNI_VERSION_1_6;
}
