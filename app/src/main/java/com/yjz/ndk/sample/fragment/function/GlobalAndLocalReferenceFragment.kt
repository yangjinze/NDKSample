package com.yjz.ndk.sample.fragment.function

import android.os.Bundle
import android.view.View
import android.view.LayoutInflater
import android.view.ViewGroup
import com.yjz.ndk.sample.R
import com.yjz.ndk.sample.been.Been
import com.yjz.ndk.sample.fragment.BaseFragment
import kotlinx.android.synthetic.main.fragment_global_local_ref.*
import java.lang.Error
import kotlin.concurrent.thread


/**
 * 全局与本地引用
 *
 * 局部引用:
局部引用可以通过NewLocalRef和FindClass、NewObject、GetObjectClass和NewCharArray等方法进行创建，一般是在方法中使用并且值在创建它的方法内有效，
它会增加引用计数从而阻止GC回收所引用的对象，但是一旦出了这个方法就变得无效了。也就是说它的生命周期只在创建该局部引用的方法内部。
在用完局部引用之后可以选择不手动释放而是在本地方法执行完之后由JVM自动释放，还可以手动调用DeleteLocalRef释放。但是一般最好手动在刚用完之后就立刻释放，
因为JNI会将创建的局部引用都存储在一个局部引用表中，如果这个表超过了最大容量限制（Android上的JNI局部引用表最大数量是512个），就会造成局部引用表溢出，
从而导致程序崩溃。并且有可能在你申请并使用完局部引用之后的操作还需要比较大的空间，这时候如果不即使释放有可能导致OOM。
在使用局部引用的时候还需要注意的是局部引用不能跨线程使用，只在创建它的线程有效。不要试图在一个线程中创建局部引用并存储到全局引用中，然后在另外一个线程中使用。
 * @author Yjz
 */
class GlobalAndLocalReferenceFragment : BaseFragment() {

    val bbb = Been("test been", 1)
    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        log.d("onCreateView")
        return inflater.inflate(R.layout.fragment_global_local_ref,container,false)
    }

    override fun onStart() {
        super.onStart()

        btn_create_much_local.setOnClickListener {
            try {
                createMuchLocalRef()
            } catch (exception : Exception) {
                log.d("捕获到异常")
                exception.printStackTrace()
            } catch (error : Error) {
                log.d("捕获到error")
                error.printStackTrace()
            }
        }
        /**
         * 打印出：
         *  jni: cacheJavaLocal been address = 0xe716bf40
            jni: first cache been
            jni: get Name = test been
            jni: cacheJavaLocal been address = 0xe716bf40
            jni: get from cache 0x98415098
            jni: get Name = test been2

            不通过NewGlobalRef而直接缓存局部变量的是错误的，也许这局部变量所指向的地址内容已经被改变了，
            或者说这局部变量已经被gc回收了，再次访问可能会出现各种异常情况的发生。

            像是本例，原意可能是要达到每次都能打印出缓存的对象"test benn"，但结果确不是，因为该内存地址存放的对象改变了，
            所以访问该地址内容的时候，获取到的是新的对象数据。也有可能此时该对象地址写入的是不同类型的新数据，则会导致未知异常的发生。
         */
        //缓存Java层传入的局部引用
        btn_cache_java_local.setOnClickListener {
            var beenKT : Been? = Been("test been", 1)
            cacheJavaLocal(beenKT)
            beenKT = null
            System.gc()
            cacheJavaLocal(Been("test been2", 2))
           // cacheJavaLocal(null)
        }
        //缓存native层创建的局部引用
        btn_cache_native_local.setOnClickListener {
            try {
                cacheNativeLocal(Been("test been", 1))
                //native层访问无效的本地引用抛出的异常无法被捕获
                cacheNativeLocal(Been("test been2", 2))
            } catch (exception : Exception) {
                exception.printStackTrace()
            } catch (throws : Throwable) {
                throws.printStackTrace()
            }
        }

        /**
         * 将java层传入的对象缓存为全局引用
         *
         * 打印：
         * D/jni: globalFunc
           D/jni: get Name = test been
           D/jni: globalFunc
           D/jni: get from cahce
           D/jni: get Name = test been
         */
        btn_cache_java_global.setOnClickListener {
            cacheJavaGlobal(Been("test been", 1))
            cacheJavaGlobal(Been("test been2", 2))
        }

        /**
         * 在native创建引用并缓存为全局引用
         *
         */
        btn_cache_native_global.setOnClickListener {
            cacheNativeGlobal(Been("test been", 1))
            cacheNativeGlobal(Been("test been2", 2))
        }
        btn_global_weak.setOnClickListener {
            globalWeakFunc(Been("test been", 1))
            thread {
                //System.gc()
                log.d("gc end")
            }
        }
    }

    external fun createMuchLocalRef()
    external fun cacheJavaLocal(been : Been?)
    external fun cacheNativeLocal(been : Been)
    external fun cacheJavaGlobal(been : Been)
    external fun cacheNativeGlobal(been : Been)
    external fun globalWeakFunc(been : Been)

    companion object {
        init {
            System.loadLibrary("local-global-reference-lib")
        }
    }
}