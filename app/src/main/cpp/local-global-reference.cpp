#include <jni.h>
#include <string>
#include<android/log.h>
#include <android/bitmap.h>  //引入头文件还需要在cmake中引入jnigraphics
#include <pthread.h>
#include <unistd.h> // sleep 的头文件
#define TAG "jni"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__)

//char* jstringToChar(JNIEnv* env, jstring jstr) {
//    char* rtn = NULL;
//    jclass clsstring = env->FindClass("java/lang/String");
//    jstring strencode = env->NewStringUTF("GB2312");
//    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
//    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
//    jsize alen = env->GetArrayLength(barr);
//    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
//    if (alen > 0) {
//        rtn = (char*) malloc(alen + 1);
//        memcpy(rtn, ba, alen);
//        rtn[alen] = 0;
//    }
//    env->ReleaseByteArrayElements(barr, ba, 0);
//    env->DeleteLocalRef(clsstring);
//    env->DeleteLocalRef(strencode);
//    env->DeleteLocalRef((jobject)mid);
//    return rtn;
//}
//
//jobjectArray globalArray = NULL;
//void createMuchLocalRef(JNIEnv* env, jobject instance) {
//    LOGD("createMuchLocalRef");
//    if (globalArray == NULL) {
//        // 创建一个字符串数组
//        jclass clsString = env -> FindClass("java/lang/String");
//        // 获取String的构造方法
//        jmethodID  mid_string_init = env -> GetMethodID(clsString, "<init>", "()V");
//        globalArray = (jobjectArray)(env -> NewGlobalRef(env -> NewObjectArray(1000, clsString, env -> NewObject(clsString, mid_string_init))));
//
//        for (int i = 0; i < env -> GetArrayLength(globalArray); ++i) {
//            LOGD("create LocalRef %d", i);
//            jstring js = env -> NewStringUTF("index %d");
//            env -> SetObjectArrayElement(globalArray, i, env -> NewGlobalRef(js));
//            env -> DeleteLocalRef(js); //使用完释放就不会导致出现异常JNI ERROR (app bug): local reference table overflow (max=512)
//        }
//    } else {
//        LOGD("read cache");
//        for (int i = 0; i < env -> GetArrayLength(globalArray); ++i) {
//            char* cc = jstringToChar(env, (jstring)(env ->GetObjectArrayElement(globalArray, i)));
//            LOGD("read cache %d", cc);
//        }
//    }
//
//    LOGD("create LocalRef end");
//}

void createJstring(JNIEnv* env, int i) {
    jstring js = env -> NewStringUTF("index %d");
   // env -> DeleteLocalRef(js);//使用完释放就不会导致出现异常JNI ERROR (app bug): local reference table overflow (max=512)
}

void createMuchLocalRef(JNIEnv* env, jobject instance) {
    LOGD("createMuchLocalRef");
    jint len = 512;
/***局部变量溢出示例，  EnsureLocalCapacity可以用来判断是否可以创建足够数量的局部引用 **/
/*
    if (env -> EnsureLocalCapacity(len) < 0) {
        //EnsureLocalCapacity可以用来判断是否可以创建足够数量的局部引用,足够返回0,不够返负数并抛出OutOfMemoryError
        //根据需要进行捕获
        LOGD("当前无法创建%d个局部引用", len);
        return;
    }

    for (int i = 0; i < 513; ++i) {
        LOGD("create LocalRef %d", i);
        jint size = 20;
       // createJstring(env, i);   //封装成函数同样也算局部引用个数
        jstring js = env -> NewStringUTF("index %d");
        // env -> DeleteLocalRef(js); 使用完释放就不会导致出现异常JNI ERROR (app bug): local reference table overflow (max=512)
    }
*/

/*** 在PushLocalFrame和PopLocalFrame示例***/

    for (int i = 0; i < 513; ++i) {
        LOGD("create LocalRef %d", i);
        jint size = 10;
        //创建一个局部帧，传参指明大概在PushLocalFrame和PopLocalFrame范围内可能需要创建多个数量的局部引用
        //能够创建成果返回0，否则返回负数并抛出错误outOfMemoryError
        if (env -> PushLocalFrame(size) < 0) {
            //抛出OutOfMemoryError
            return;
        }
         createJstring(env, i);   //在PushLocalFrame和PopLocalFrame范围内，封装成函数同样也算能被释放
        jstring js = env -> NewStringUTF("index %d");
        env -> PopLocalFrame(NULL);//PopLocalFrame之后，该局部堆栈帧的局部引用都会被释放掉，不用手动每一个调用释放
    }


/**基础数据类型、jmethodID、jfieldID都不需要释放，jobject以及其子类jclass、jstring、jarray最好手动调用DeleteLocalRef释放*******/
/*    for (int i = 0; i < 513; ++i) {
        LOGD("create LocalRefff2 %d", i);
        //基础数据类型、jmethodID、jfieldID不需要释放
        jint size = 10 + i;
        jboolean b = i/2 == 0 ? true : false;
        jlong l = 1 + i;
        jdouble dd = 2 + i;
        jfloat  ff = 3 + i;
        jbyte bb = 0x01;
        //以上基础数据类型不会造成局部变量表数量过大而导演outOfMemoryError
        jintArray intArray = env -> NewIntArray(0);
        jbooleanArray booleanArray = env -> NewBooleanArray(1);
        jbyteArray  byteArray = env -> NewByteArray(2);
        env -> DeleteLocalRef(intArray);
        env -> DeleteLocalRef(booleanArray);
        env -> DeleteLocalRef(byteArray);
        //array类型的都需要主动释放,其它类型的array都一样，没再试验

        //jobject以及其子类jstring,jclass,jarray都需要手动释放
        //jstring需要主动释放
        jstring js = env -> NewStringUTF("12312");
        env -> DeleteLocalRef(js);
        //env -> ReleaseStringChars(js, NULL);
        //jclass需要主动释放
        jclass jclazz = env -> FindClass("java/lang/String");
        //jmethodID不需要,jfield也不需要
        jmethodID  constructorMethod = env -> GetMethodID(jclazz, "<init>", "()V");
        //jobject需要主动释放
        jobject object = env -> NewObject(jclazz, constructorMethod);
        env -> DeleteLocalRef(jclazz);
        env -> DeleteLocalRef(object);
    }
*/
    for (int i = 0; i < 51300; ++i) {
        LOGD("create LocalReff %d", i);
        jstring js = env -> NewStringUTF("index %d");
        env -> NewWeakGlobalRef(js);
        env -> DeleteLocalRef(js);
    }
    LOGD("create LocalRef end");
}



jobject localBeenCache = NULL;
void cacheJavaLocal(JNIEnv* env, jobject instance, jobject been) {
    LOGD("cacheJavaLocal been address = %#x", &been);
    jobject tempBeen;
    if (localBeenCache != NULL) {
        LOGD("get from cache %#x", &localBeenCache);
        tempBeen = localBeenCache;
    } else {
        LOGD("first cache been");
        //将java传入的变量缓存起来
        tempBeen = been;
        localBeenCache = been;
    }
    //获取对应类
    jclass cls = env->GetObjectClass(tempBeen);
    //获取getName()函数
    jmethodID  getNameMethod = env -> GetMethodID(cls, "getName", "()Ljava/lang/String;");
    //调用getName函数
    jstring name = (jstring)(env -> CallObjectMethod(tempBeen, getNameMethod));
    //jstring 转char*再打印，否则抛异常
    const char* namePoint = env -> GetStringUTFChars(name , 0);
    LOGD("get Name = %s", namePoint);
}


/**
 * 缓存native层创建的局部变量
 *
 * native层创建的局部变量只在该本地方法内有效，本地方法执行完毕之后会被自动释放。
 * 这种缓存法是错误的
 *
 * 这边尝试缓存FindClass找到的jclass,下次调用直接使用，但这种做法会导致抛出异常，并且无法被try catch：
  D/jni: cacheNativeLocalFunc
  D/jni: get from cahce

 E/art: JNI ERROR (app bug): accessed stale local reference 0x100019 (index 6 in a table of size 6)
 ...
 E/art: 0xb8a08320 SpaceTypeMallocSpace begin=0x12c00000,end=0x12ee2000,limit=0x22c00000,size=2MB,capacity=96MB,non_growth_limit_capacity=256MB,name="main rosalloc space"]
 ...
 e A/art: art/runtime/check_jni.cc:65] JNI DETECTED ERROR IN APPLICATION: jclass is an invalid local reference: 0x100019 (0xdead4321)  //大意就是调用是一个无效的jclass本地应用导致了应用异常退出
 in call to GetMethodID from void com.yjz.ndk.sample.fragment.function.GlobalAndLocalReferenceFragment.cacheNativeLocalFunc(com.yjz.ndk.sample.been.Been)
 A/art: art/runtime/check_jni.cc:65] "main" prio=5 tid=1 Runnable
...
A/art: art/runtime/runtime.cc:290] Runtime aborting...
A/art: art/runtime/runtime.cc:290] Aborting thread:
A/art: art/runtime/runtime.cc:290] "main" prio=5 tid=1 Native
...
 * @param env
 * @param instance
 * @param been
 */
jclass beenCls;
//static jclass beenCls; 一样会崩溃
void cacheNativeLocal(JNIEnv* env, jobject instance, jobject been) {
    LOGD("cacheNativeLocalFunc");
    if (beenCls != NULL) {
        LOGD("get from cahce");
    } else {
        //获取对应类
        beenCls = env->GetObjectClass(been);
    }
    //获取getName()函数，第二次调用的使用使用了错误缓存的jclass进行调用而发生异常
    jmethodID  getNameMethod = env -> GetMethodID(beenCls, "getName", "()Ljava/lang/String;");
    //调用getName函数
    jstring name = (jstring)(env -> CallObjectMethod(been, getNameMethod));
    const char* namePoint = env -> GetStringUTFChars(name , 0);
    LOGD("get Name = %s", namePoint);
}

jobject globalBeenObj;
void cacheJavaGlobal(JNIEnv* env, jobject instance, jobject been) {
    LOGD("globalFunc");
    if (globalBeenObj != NULL) {
        LOGD("get from cahce");
    } else {
        //获取对应类 , 并通过NewGlobalRef持有全局引用
        globalBeenObj = env -> NewGlobalRef(been);
    }
    jclass cls = env -> GetObjectClass(globalBeenObj);
    //获取getName()函数
    jmethodID  getNameMethod = env -> GetMethodID(cls, "getName", "()Ljava/lang/String;");
    //调用getName函数
    jstring name = (jstring)(env -> CallObjectMethod(globalBeenObj, getNameMethod));
    const char* namePoint = env -> GetStringUTFChars(name , 0);
    LOGD("get Name = %s", namePoint);
    env -> ReleaseStringUTFChars(name, namePoint);
}

jclass globalBeenCls;
jmethodID getNameMethod;
void cacheNativeGlobal(JNIEnv* env, jobject instance, jobject been) {
    LOGD("cacheNativeGlobal");
    if (globalBeenCls != NULL) {
        LOGD("get jclass from cache");
    } else {
        //获取对应类 , 并通过NewGlobalRef持有全局引用
        jclass cls = env -> GetObjectClass(been);
        globalBeenCls = (jclass)env -> NewGlobalRef(cls);
    }
    if (getNameMethod != NULL) {
        LOGD("get jmethodID from cache");
    } else {
        //获取getName()函数
        getNameMethod = env -> GetMethodID(globalBeenCls, "getName", "()Ljava/lang/String;");
    }
    //调用getName函数
    jstring name = (jstring)(env -> CallObjectMethod(been, getNameMethod));
    const char* namePoint = env -> GetStringUTFChars(name , 0);
    env -> DeleteLocalRef(name);
    LOGD("get Name = %s", namePoint);

    /**不再使用全局引用的时候手动释放，顺便置为NULL,否则可能下次又访问到该对象，if(globalBeenCls != NULL)成功而直接访问被释放的对象会导致程序崩溃**/
    /*
    env -> DeleteGlobalRef(globalBeenCls);
    globalBeenCls = NULL;
    */
}

static JavaVM *javaVM = NULL;
jobject weakBeen;
void* newThread(void *) {
    LOGD("启动线程");
    sleep(10);//休眠10秒
    LOGD("休眠结束");
    JNIEnv *env = NULL;
    // 将当前线程添加到 Java 虚拟机上
    if (javaVM -> AttachCurrentThread(&env, NULL) == 0) {
        LOGD("准备使用被回收的弱引用");
//        if (env -> IsSameObject(weakBeen, NULL)) { //通过这种方法可以判断是否被回收了，避免出现异常
//            LOGD("弱引用已被回收");
//            javaVM -> DetachCurrentThread();
//            return NULL;
//        }
        jclass beenCls = (jclass)env -> NewGlobalRef(env -> GetObjectClass(weakBeen));
        // 从 Java 虚拟机上分离当前线程
        javaVM -> DetachCurrentThread();
    }
    return NULL;
}

void globalWeakFunc(JNIEnv* env, jobject instance, jobject been) {
    LOGD("globalWeakFunc");
    if (weakBeen == NULL) {
        LOGD("cache weak been");
        weakBeen = env -> NewWeakGlobalRef(been);
    } else {
        LOGD("read weak been from cache");
    }
    jclass beenCls = (jclass)env -> NewGlobalRef(env -> GetObjectClass(weakBeen));
    //获取getName()函数
    jmethodID  getNameMethod = env -> GetMethodID(beenCls, "getName", "()Ljava/lang/String;");
    //调用getName函数
    jstring name = (jstring)(env -> CallObjectMethod(weakBeen, getNameMethod));
    const char* namePoint = env -> GetStringUTFChars(name , 0);
    env -> DeleteLocalRef(name);
    LOGD("get Name = %s", namePoint);

    pthread_t handles; //线程句柄
    int result = pthread_create(&handles, NULL, newThread, NULL);
    if (result != 0) {
        LOGD("create thread failed");
    } else {
        LOGD("create thread success");
    }
}

static JNINativeMethod jniNativeMethods[] = {
        {"createMuchLocalRef", "()V", (void*)createMuchLocalRef},
        {"cacheJavaLocal", "(Lcom/yjz/ndk/sample/been/Been;)V", (void*)cacheJavaLocal},
        {"cacheNativeLocal", "(Lcom/yjz/ndk/sample/been/Been;)V", (void*)cacheNativeLocal},
        {"cacheJavaGlobal", "(Lcom/yjz/ndk/sample/been/Been;)V", (void*)cacheJavaGlobal},
        {"cacheNativeGlobal", "(Lcom/yjz/ndk/sample/been/Been;)V", (void*)cacheNativeGlobal},
        {"globalWeakFunc", "(Lcom/yjz/ndk/sample/been/Been;)V", (void*)globalWeakFunc}
};


//4.native层重写JNI_OnLoad方法
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGD("local_global_ref JNI_OnLoad");

    JNIEnv* jniEnv = NULL;
    //TODO reinterpret_cast,const_cast,static_cast,dynamic_cast 显式类型转换
    if (vm ->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6) != JNI_OK) {
        LOGD("JNI_OnLoad GetEnv error!");
        return JNI_ERR;
    }
    assert(jniEnv != NULL);

    const char* className  = "com/yjz/ndk/sample/fragment/function/GlobalAndLocalReferenceFragment";
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
