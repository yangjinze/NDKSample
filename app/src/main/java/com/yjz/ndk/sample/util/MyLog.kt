package com.yjz.ndk.sample.util

import android.util.Log

/**
 *
 * @author Yjz
 */
class MyLog(private val TAG : String?) {

    fun d(msg : String) {
        Log.d(TAG, msg)
    }

    fun e(msg : String) {
        Log.e(TAG, msg)
    }
}