package com.yjz.ndk.sample.fragment.function

import android.os.Bundle
import android.view.View
import android.view.LayoutInflater
import android.view.ViewGroup
import com.yjz.ndk.sample.R
import com.yjz.ndk.sample.fragment.BaseFragment
import kotlinx.android.synthetic.main.fragment_static_dynamic_register.*

/**
 * 静态与动态注册
 *
     静态注册:
    1.需要通过javah指令生成头文件(当然也可以手动书写对应的jni方法，只是容易出错)；
    2.jni方法的名称很长，具有格式要求，需要手动书写或者修改容易出错；
    3.一旦java类发生类型、包名等修过，或者声明的native方法增删改的话，需要修改的地方会比较多;
    3.初次调用的时候，需要根据jni函数名称通过FindSymbol(具体实现在Runctime.c)来找到对应的方法来建立联系，方法数多时，效率不高；

    动态注册(推荐使用):
    1.利用结构体 JNINativeMethod 数组记录 java 方法与 JNI 函数的对应关系；
    2.移植或者增删改方便，大部分可以通过修改这个JNINativeMethod 数组中的记录来实现；
    3.第一次调用的效率也比较高，因为已经提前绑定好关系了；
 * @author Yjz
 */
class StaticAndDynamicRegisterFragment : BaseFragment() {

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        log.d("onCreateView")
        return inflater.inflate(R.layout.fragment_static_dynamic_register,container,false)
    }

    override fun onStart() {
        super.onStart()
        log.d("onStart")
        /**
         * 静态注册示例调用
         * 打印出：
         *   D/jni: staticRegister jni
         *   D/jni: getStringFromJni jni 3
         *   D/StaticAndDynamicRegisterFragment: getStringFromJni result = jni return end
         */
        btn_static.setOnClickListener { staticRegisterJni() } //TODO 在onCreateView中时，btn_static为null

        /**
         * 动态注册示例调用
         * 打印出:
         *  D/jni: JNI_OnLoad
         *  D/jni: dynamicRegisterFunc
         *  D/jni: getInt b
         *  D/StaticAndDynamicRegisterFragment: getIntFromJni result = 0
         */
        btn_dynamic.setOnClickListener { dynamicRegisterJni() }
    }

    fun staticRegisterJni() {
        //可查看jni层打印的日志
        staticRegister()
        val result = getStringFromJni(3)
        log.d("getStringFromJni result = $result")
    }

    fun dynamicRegisterJni() {
        dynamicRegister()
        val result = getIntFromJni(true)
        log.d("getIntFromJni result = $result")
    }
    /**
     * 以下2个方法为静态注册
     *
     * 静态注册需要在cpp文件中，结合external函数所在的完整类路径和函数名，出入参定义
     *
     */
    external fun staticRegister()
    external fun getStringFromJni(type : Int) : String

    /**
     * 动态注册
     *
     * 动态注册需要在Jni中重写JNI_OnLoad方法
     */
    external fun dynamicRegister()
    external fun getIntFromJni(isSuccess : Boolean) : Int

    companion object {
        init {
            System.loadLibrary("function-lib")
        }
    }
}