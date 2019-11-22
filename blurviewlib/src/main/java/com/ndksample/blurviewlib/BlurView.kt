package com.ndksample.blurviewlib

import android.graphics.Bitmap

/**
 *
 * @author Yjz
 */
object BlurView {
    init {
        System.loadLibrary("native-lib")
    }

    /**
     * 对传入的BitMap进行模糊处理(高斯滤波实现模糊图像)
     *
     * @param r 高斯滤波虚化程度
     */
    external fun blur(bitmap: Bitmap, r : Int)
}