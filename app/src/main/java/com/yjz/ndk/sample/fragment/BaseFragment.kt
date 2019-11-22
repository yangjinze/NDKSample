package com.yjz.ndk.sample.fragment

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.support.v4.app.Fragment
import android.view.View
import android.view.LayoutInflater
import android.view.ViewGroup
import com.ndksample.blurviewlib.BlurView
import com.yjz.ndk.sample.R
import com.yjz.ndk.sample.util.MyLog
import kotlinx.android.synthetic.main.fragment_blurview.*


/**
 *
 *
 * @author Yjz
 */
open class BaseFragment : Fragment() {
    protected val log = MyLog(this::class.simpleName)
}