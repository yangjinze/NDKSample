package com.yjz.ndk.sample.fragment.function

import android.os.Bundle
import android.view.View
import android.view.LayoutInflater
import android.view.ViewGroup
import com.yjz.ndk.sample.R
import com.yjz.ndk.sample.been.Been
import com.yjz.ndk.sample.fragment.BaseFragment
import kotlinx.android.synthetic.main.fragment_exception_handle.*
import kotlinx.android.synthetic.main.fragment_global_local_ref.*
import java.lang.Error
import kotlin.concurrent.thread


/**
 * 异常处理
 *
 * @author Yjz
 */
class ExceptionHandleFragment : BaseFragment() {

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        log.d("onCreateView")
        return inflater.inflate(R.layout.fragment_exception_handle,container,false)
    }

    override fun onStart() {
        super.onStart()
        btn_java_exception.setOnClickListener {
            try {
                javaThrowException()
            } catch (exception : Exception) {
                log.d("java catch exception:" + exception.localizedMessage)
            }
        }
        btn_native_exception.setOnClickListener {
            try {
                nativeThrowException()
            } catch (exception : Exception) {
                log.d("java catch exception:" + exception.localizedMessage)
            }
        }
        btn_call_limit_interface.setOnClickListener {
            try {
                callLimitInterface()
            } catch (exception : Exception) {
                log.d("java catch exception:" + exception.localizedMessage)
            }catch (exception : Throwable) {
                log.d("java catch exception:" + exception.localizedMessage)
            }
        }
        btn_test.setOnClickListener {
            try {
                test()
            } catch (exception : Exception) {
                log.d("java catch exception:" + exception.localizedMessage)
            } catch (exception : Throwable) {
                log.d("java catch Throwable:" + exception.localizedMessage)
            }
        }
    }

    /**
     * javaThrowException中回调这个方法
     */
    fun throwException() {
        log.d("java start throw exception")
        throw NullPointerException("测试抛出异常")
        log.d("java end throw exception") // java throw异常就立马停止往下执行，所以这边执行不到
    }

    fun throwException2() {
        log.d("java start throw exception2")
        throw IllegalArgumentException("测试抛出异常2")
        log.d("java end throw exception2") // java throw异常就立马停止往下执行，所以这边执行不到
    }

    external fun javaThrowException()
    external fun nativeThrowException()
    external fun callLimitInterface()
    external fun test()
    companion object {
        init {
            System.loadLibrary("exception-handle-lib")
        }
    }
}