package com.yjz.ndk.sample.util;

import android.content.Context;

/**
 * 异常崩溃日志捕获
 */
public class CrashHandler implements Thread.UncaughtExceptionHandler {
    private final MyLog logger = new MyLog("CrashHandler");
    Context appContext;
    public CrashHandler(Context appContext){
        this.appContext=appContext;
    }
    @Override
    public void uncaughtException(Thread thread, Throwable ex) {
        ex.printStackTrace();
        logger.e("CrashHandler:" + ex.getLocalizedMessage());
        System.exit(0);
    }
}
