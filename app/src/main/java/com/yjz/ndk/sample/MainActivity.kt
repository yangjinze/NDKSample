package com.yjz.ndk.sample

import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import com.ndksample.blurviewlib.BlurView
import com.yjz.ndk.sample.activity.ProjectActivity
import com.yjz.ndk.sample.util.MyLog
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    private val log = MyLog(this::class.simpleName)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }


    fun projectDemo(view: View) {
        startActivity(Intent(this, ProjectActivity::class.java))
    }
}
