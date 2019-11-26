package com.yjz.ndk.sample.activity

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.view.View
import android.support.v4.app.Fragment
import android.view.KeyEvent
import com.yjz.ndk.sample.R
import com.yjz.ndk.sample.fragment.function.GlobalAndLocalReferenceFragment
import com.yjz.ndk.sample.fragment.function.StaticAndDynamicRegisterFragment
import com.yjz.ndk.sample.fragment.project.BlurViewFragment
import com.yjz.ndk.sample.util.MyLog
import kotlinx.android.synthetic.main.activity_project.*

/**
 *
 * @author Yjz
 */
class FunctionActivity : AppCompatActivity() {

    private val log = MyLog(this::class.simpleName)
    private var curFragment: Fragment? = null
    private val fragmentManager = supportFragmentManager

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_function)

    }

    override fun onKeyUp(keyCode: Int, event: KeyEvent?): Boolean {
        log.d("onKeyUp")
        when (keyCode) {
            KeyEvent.KEYCODE_BACK -> back().also { log.d("KEYCODE_BACK") }
        }
        return super.onKeyUp(keyCode, event)
    }

    /**
     * 静态与动态注册
     */
    fun staticAndDynamicRegister(view : View) {
        changeFragment(StaticAndDynamicRegisterFragment())
    }

    /**
     * 全局与局部变量
     */
    fun globalAndLocalReference(view : View) {
        changeFragment(GlobalAndLocalReferenceFragment())
    }

    private fun changeFragment(fragment: Fragment) {
        //实例化碎片管理器对象
        val ft = fragmentManager.beginTransaction()
        //选择fragment替换的部分
        ft.replace(R.id.frameLayout, fragment)
        ft.commit()
        btn_layout.visibility = View.GONE
        curFragment = fragment
    }

    private fun back() {
        curFragment ?: return
        val ft = fragmentManager.beginTransaction()
        ft.remove(curFragment!!)
        curFragment = null
        btn_layout.visibility = View.VISIBLE
    }
}