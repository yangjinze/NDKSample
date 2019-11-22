package com.yjz.ndk.sample.fragment.project

import android.graphics.BitmapFactory
import android.os.Bundle
import android.view.View
import android.view.LayoutInflater
import android.view.ViewGroup
import com.ndksample.blurviewlib.BlurView
import com.yjz.ndk.sample.R
import com.yjz.ndk.sample.fragment.BaseFragment
import kotlinx.android.synthetic.main.fragment_blurview.*


/**
 * 模糊图片
 *
 * @author Yjz
 */
class BlurViewFragment : BaseFragment() {

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        log.d("onCreateView")
        return inflater.inflate(R.layout.fragment_blurview,container,false)
    }

    override fun onStart() {
        super.onStart()
        log.d("onStart")
        val bitmap = BitmapFactory.decodeResource(resources, R.mipmap.ic_launcher)
        img_old.setImageBitmap(bitmap.copy(bitmap.config, true))
        BlurView.blur(bitmap,13)
        img_new.setImageBitmap(bitmap)
    }
}