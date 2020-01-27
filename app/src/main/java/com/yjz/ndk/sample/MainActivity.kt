package com.yjz.ndk.sample

import android.annotation.SuppressLint
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Color
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.view.View
import android.widget.ImageView
import com.ndksample.blurviewlib.BlurView
import com.yjz.ndk.sample.activity.FunctionActivity
import com.yjz.ndk.sample.activity.ProjectActivity
import com.yjz.ndk.sample.util.BytesUtil
import com.yjz.ndk.sample.util.MyLog
import java.io.File
import java.io.FileOutputStream
import java.io.IOException
import android.content.ComponentName
import android.text.TextUtils
import android.R.attr.name
import android.content.Context
import android.content.pm.ResolveInfo
import android.content.pm.PackageManager
import com.yjz.ndk.sample.util.CrashHandler


class MainActivity : AppCompatActivity() {

    private val log = MyLog(this::class.simpleName)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        log.d("onCreate")
        setContentView(R.layout.activity_main)
        Thread.setDefaultUncaughtExceptionHandler(CrashHandler(applicationContext))
    }

    override fun onStart() {
        super.onStart()
        log.d("onStart")
    }

    override fun onPause() {
        super.onPause()
        log.d("onPause")
    }

    override fun onDestroy() {
        super.onDestroy()
        log.d("onDestroy")
    }

    fun functionDemo(view: View) {
        startActivity(Intent(this, FunctionActivity::class.java))
    }

    fun projectDemo(view: View) {
        startActivity(Intent(this, ProjectActivity::class.java))
        //startActivity(getAppOpenIntentByPackageName(this, "com.tongtongsuo.app"))
    }

    fun getAppOpenIntentByPackageName(context: Context, packageName: String): Intent? {
        //Activity完整名
        var mainAct: String? = null
        //根据包名寻找
        val pkgMag = context.getPackageManager()
        val intent = Intent(Intent.ACTION_MAIN)
        intent.addCategory(Intent.CATEGORY_LAUNCHER)
        intent.flags = Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED or Intent.FLAG_ACTIVITY_NEW_TASK


        @SuppressLint("WrongConstant") val list = pkgMag.queryIntentActivities(
            intent,
            PackageManager.GET_ACTIVITIES
        )
        for (i in list.indices) {
            val info = list.get(i)
            if (info.activityInfo.packageName == packageName) {
                mainAct = info.activityInfo.name
                break
            }
        }
        if (TextUtils.isEmpty(mainAct)) {
            return null
        }
        intent.component = ComponentName(packageName, mainAct!!)
        return intent
    }
}
