# boost-mutex

## 一、Boost锁**


mutex：
boost提供的mutex有6种：
boost::mutex      唯一锁，同时只能由一个线程访问，不分读写
boost::try_mutex
boost::timed_mutex
boost::recursive_mutex          读写锁
boost::recursive_try_mutex
boost::recursive_timed_mutex



**一、 下面仅对boost::mutex进行分析。**


mutex类是一个CriticalSection（临界区）封装类，它在构造函数中新建一个临界区并InitializeCriticalSection，然后用一个成员变量
void* m_mutex;
来保存该临界区结构。
除此之外，mutex还提供了do_lock、do_unlock等方法，这些方法分别调用EnterCriticalSection、 LeaveCriticalSection来修改成员变量m_mutex（CRITICAL_SECTION结构指针）的状态，但这些方法都是private的，以防止我们直接对mutex进行锁操作，所有的锁操作都必须通过mutex的友元类detail::thread::lock_ops<mutex>来完成，比较有意思的是，lock_ops的所有方法：lock、unlock、trylock等都是static的，如lock_ops<Mutex>::lock的实现：

```cpp
template <typename Mutex>
class lock_ops : private noncopyable
 {
 public:
    static void lock(Mutex& m)
      {
          m.do_lock();
     }
}
```



boost::thread的设计者为什么会这么设计呢？我想大概是：

1、boost::thread的设计者不希望被我们直接操作mutex，改变其状态，所以mutex的所有方法都是private的（除了构造函数，析构函数）。

2、虽然我们可以**通过lock_ops来修改mutex的状态**，如：

```cpp
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/detail/lock.hpp>
 
int main()
{
     boost::mutex mt;
    //mt.do_lock();        // Error! Can not access private member!
 
    boost::detail::thread::lock_ops<boost::mutex>::lock(mt);
 
    return 0;
}
```

但是，这是不推荐的，因为mutex、scoped_lock、condition、barrier是一套完整的类系，它们是相互协同工作的，像上面这么操作没有办法与后面的几个类协同工作。

scoped_lock：智能锁

​       上面说过，不应该直接用lock_ops来操作mutex对象，那么，应该用什么呢？答案就是scoped_lock。与存在多种mutex一样，存在多种与mutex对应的scoped_lock：

scoped_lock

scoped_try_lock

scoped_timed_lock

这里我们只讨论scoped_lock。

scoped_lock是定义在namespace boost::detail::thread下的，为了方便我们使用（也为了方便设计者），mutex使用了下面的typedef：
typedef detail::thread::scoped_lock<mutex> scoped_lock;
这样我们就可以通过：
boost::mutex::scoped_lock
来使用scoped_lock类模板了。
由于scoped_lock的作用：仅在于对mutex加锁/解锁（即使mutex EnterCriticalSection/LeaveCriticalSection），因此，它的接口也很简单，除了构造函数外，仅有lock/unlock/locked（判断是否已加锁），及类型转换操作符void*，一般我们不需要显式调用这些方法，因为scoped_lock的构造函数是这样定义的：

```cpp
explicit scoped_lock(Mutex& mx, bool initially_locked=true)
    : m_mutex(mx), m_locked(false)
{
    if (initially_locked) lock();
}
```

注：m_mutex是一个mutex的引用。

因此，当我们不指定initially_locked参数构造一个scoped_lock对象时，scoped_lock会自动对所绑定的mutex加锁，而析构函数会检查是否加锁，若已加锁，则解锁；当然，有些情况下，我们可能不需要构造时自动加锁，这样就需要自己调用lock方法。后面的condition、barrier也会调用scoped_lock的lock、unlock方法来实现部分方法。

正因为scoped_lock具有可在构造时加锁，析构时解锁的特性，我们经常会使用局部变量来实现对mutex的独占访问。

```cpp
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>
 
boost::mutex io_mutex;
void count()    // worker function
{
    for (int i = 0; i < 10; ++i)
   {
      boost::mutex::scoped_lock lock(io_mutex);
      std::cout << i << std::endl;
     }
 }
 
 int main(int argc, char* argv[])
 {
   boost::thread thrd1(&count);
   boost::thread thrd2(&count);
   thrd1.join();
   thrd2.join();
   return 0;
}
```



在每次输出信息时，为了防止整个输出过程被其它线程打乱，通过对io_mutex加锁（进入临界区），从而保证了输出的正确性。

在使用 scoped_lock时，我们有时候需要使用全局锁（定义一个全局mutex，当需要独占访问全局资源时，以该全局mutex为参数构造一个 scoped_lock对象即可。全局mutex可以是全局变量，也可以是类的静态方法等），有时候则需要使用对象锁（将mutex定义成类的成员变量），应该根据需要进行合理选择。

Java的synchronized可用于对方法加锁，对代码段加锁，对对象加锁，对类加锁（仍然是对象级的），这几种加锁方式都可以通过上面讲的对象锁来模拟；相反，在Java中实现全局锁好像有点麻烦，必须将请求封装到类中，以转换成上面的四种 synchronized形式之一。

condition

condition的接口如下：

```cpp
class condition : private boost::noncopyable   // Exposition only
{
public:
   // construct/copy/destruct
    condition();
   ~condition();
   // notification
   void notify_one();
   void notify_all();
 
   // waiting
    template<typename ScopedLock> void wait(ScopedLock&);
    template<typename ScopedLock, typename Pred> void wait(ScopedLock&, Pred);
    template<typename ScopedLock>
      bool timed_wait(ScopedLock&, const boost::xtime&);
   template<typename ScopedLock, typename Pred>
      bool timed_wait(ScopedLock&, Pred);
};
```

其中wait 用于等待某个condition的发生，而timed_wait则提供具有超时的wait功能，notify_one用于唤醒一个等待该condition发生的线程，notify_all则用于唤醒所有等待该condition发生的线程。

由于condition的语义相对较为复杂，它的实现也是整个boost::thread库中最复杂的（对Windows版本而言，对支持pthread的版本而言，由于pthread已经提供了pthread_cond_t，使得condition实现起来也十分简单），下面对wait和notify_one进行简要分析。

condition内部包含了一个condition_impl对象，由该对象执行来处理实际的wait、notify_one...等操作

## **二、临界资源保护（并行程序设计过程中）**

  从事软件研发工作有近两年的时间了，从自己的感觉来说，系统软件，特别是内核软件开发的难点在于并发程序设计，或者从更深层次的角度来讲应该是并行程序设计（多核程序设计）。并行程序设计的难点在于临界资源的保护。通常各并行的线程或者是进程之间都会存在共享的临界资源，如果这些临界资源处理不当，那么小则程序运行出错；大则系统崩溃。所以，我个人认为只要将临界资源处理好，那么并行程序设计就不是问题了。

​       下面结合自己这一段时间的程序设计“经验”，对并行程序设计过程中所应该注意的问题和一些方法做初步小结。

**1、 避免临界资源，减少临界资源的数量**

并行程序设计中，临界资源越多，程序设计将会越复杂，所以在程序设计之初，需要考虑临界资源的数量，尽可能的减少临界资源。另一方面，一个资源在不同的时间点会呈现出不同的特点。在某些情况下其可能为临界资源；在某些情况其表现为非临界资源。例如，操作需要创建一个设备，该设备需要添加到共享资源区中，为了避免临界资源的产生，可以先将创建设备的所有信息都初始化完毕之后，最后将设备添加到共享资源区，这样在一定程度上避免了临界资源，可以简化创建设备的过程，提高系统性能。所以，从这一点上我们可以得出一个结论，在处理临界资源时，很多时候可以将资源从总的资源池中取出来，让被访问的临界资源成为一个非临界资源，在短时间内为一个上下文独享，这样可以简化设计，提高效率。但是，并不是所有的应有都允许这样的操作，然而，总的原则应该是不变的，那就是尽可能的减少临界资源的数量，减少并发程序的依赖关系，这是从“根”上简化并发程序的设计与实现。

**2、 函数的设计需要考虑应用上下文环境**

并行程序中的函数设计并不仅仅是函数功能的封装，算法的封装，还需要考虑函数的应用上下文，也就是设计实现的函数将在什么环境下被调用。这一点非常重要，如果处理不好，那么很容易出现死锁、系统崩溃等现象。

函数设计需要考虑应用上下文，这一点一个非常重要的原因在于并行程序，特别是内核程序中的上下文特点存在很大的区别。例如，Linux内核上下文分为普通的用户进程上下文、软中断上下文、中断上下文。在用户进程上下文中，函数的限制条件不是很强烈，能够睡眠的函数都可以运行，但是在中断上下文限制条件很强烈，睡眠函数是不能运行的，所以，函数设计需要考虑应用上下文环境。

​       除了上述原因之外，另一个非常重要的原因在于加锁的问题，而这个问题可能更容易在设计实现过程中被忽视。函数的运行上下文一定要考虑加锁情况，例如一个函数在持有spinlock资源锁的条件下调用一个可能引起睡眠的函数，那么系统肯定崩溃；另外，一个函数在持有锁A的时候再次调用可能访问锁A的函数，那么系统肯定死锁。总的来说，上述这些问题的根源都在于函数调用时没有考虑运行上下文的加锁情况，导致错误加锁或者乱序加锁。

​       因此，在并行程序设计过程中，设计一个函数需要考虑一下，这个函数是为哪个上下文写的？调用这个函数存在哪些限制？这是在普通函数设计之上提出的更高要求。



**3、 临界资源的保护需要考虑读写性、竞争上下文**

临界资源的保护需要考虑对临界资源访问的读写性，如果访问临界资源的多个上下文存在读操作，那么访问临界资源的锁可以被优化。通常可以采用读锁对临界资源进行读访问。另外，在临界资源访问时一定要考虑竞争上下文，如果竞争上下文为中断上下文，那么需要考虑加锁时间与可睡眠性，通常在Linux系统中采用Spinlock对其进行保护；如果竞争上下文为普通的进程上下文，那么保护的方法将简单的多。

临界资源保护时，锁的设计非常重要，通常在设计实现过程中会遇到大锁、小锁的抉择。大锁的设计实现简单，竞争点可以分析的非常清晰，但是程序效率将会大打折扣；小锁的设计实现复杂，竞争点的分析、考虑将会变得复杂，程序实现效率将会大大提高。我个人认为，在设计临界资源保护时，首先需要分析清楚竞争上下文，根据竞争上下文对资源访问的竞争点分析结果，设计合理的锁资源。尽可能在不太影响性能的前提下（锁不能成为系统的性能瓶颈），设计大锁资源。在后继的性能优化过程中，如果有必要再将锁资源进行必要细化。

​       并行程序设计中，临界资源的访问是程序设计的一大难点，一个好的程序设计人员，一定需要将程序的功能模块切分好，程序的运行上下文及上下文之间的关系设计好，临界资源及资源访问的锁设计好。只有这样设计的程序才能具备一个完美的框架，只有拥有完美框架的程序才有可能成为一个非常出色的程序。

​       总的而言，在涉及临界资源访问时，设计开发人员需要问一下自己：访问时这个资源是否为临界资源？这个资源将在何种上下文中运行？资源的竞争点有哪些？对资源封装的函数是否可以在该上下文中应用？不管怎么样，并行程序设计时，一定要清楚，共享的资源并不是在一个上下文中引用，需要对它进行合理保护。

​       这是我对临界资源保护的一点小结，不是很全面，都是一些个人理解，写下文字记录一下，希望大家批评。