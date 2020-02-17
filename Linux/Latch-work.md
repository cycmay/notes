# Linux 下 锁 

## 基础知识

### 进程和线程

进程（process）是指在系统中正在运行的一个应用程序，是系统资源分配的基本单位，在内存 中有其完备的数据空间和代码空间，拥有完整的虚拟空间地址。一个进程所拥有的数据和变量只 属于它自己。

线程（thread）是进程内相对独立的可执行单元，所以也被称为轻量进程（lightweight processes ）；是操作系统进行任务调度的基本单元。它与父进程的其它线程共享该进程所拥有的全部代码 空间和全局变量，但拥有独立的堆栈（即局部变量对于线程来说是私有的）。

#### 线程和进程的区别

线程是进程的一部分，所以线程有的时候被称为是轻量级进程。
一个没有线程的进程是可以被看作单线程的，如果一个进程内拥有多个线程， 进程的执行过程不是一条线（线程）的，而是多条线（线程）共同完成的。
系统在运行的时候会为每个进程分配不同的内存区域，但是不会为线程分配内存（线程所使 用的资源是它所属的进程的资源），线程组只能共享资源。那就是说，除了CPU之外（线程在运 行的时候要占用CPU资源），计算机内部的软硬件资源的分配与线程无关，线程只能共享它所属 进程的资源。
与进程的控制表PCB相似，线程也有自己的控制表TCB，但是TCB中所保存的线程状态比PCB表 中少很多
进程是系统所有资源分配时候的一个基本单位，拥有一个完整的虚拟空间地址，并不依赖线 程而独立存在。
更加详细的介绍，请参考这里。

#### 线程的优点

由于以下原因，行业内广泛地在编程库和操作系统中实现线程：

减少内存占用量。创建另一个线程的内存消耗量被限制为线程所需要的堆栈加上线程管理器需 要的一些簿记内存。
不需要采用先进的技术来访问服务器全局数据。如果数据有可能由另一个同时运行的线程修改， 则要做的一切就是使用互斥体 保护相关段。
创建线程所需要的时间大大小于创建进程所需要的时间，原因是不必复制堆部分（它可能很大）。
在线程之间进行环境切换时，内核在调度器中花的时间比在过程之间进行切换花的时间少。 这给负担很重的服务器留下了更多的cpu时间处理工作。

#### 线程的缺点

尽管线程在现代计算机中极具重要性，它们却有很多缺点：

编程错误代价惨重。如果一个线程崩溃，会使得整个服务器停止。一个坏线程可能会毁坏全局 数据，导致其他线程无法工作
容易产生编程错误。程序员必须不断考虑其他一些线程可能正在做引起麻烦的事情，以及如何 避免这种麻烦。需要采用额外的防范方法编制程序。
线程服务器在同步漏洞方面声名狼藉，这些漏洞几乎无法在测试中进行复制，却在生产期间很 不合时宜地出现。这类漏洞发生几率之所以如此高，是由于使用共享地址空间，这会产生更高程 度的线程交互。
有时候互斥体争用难以控制。如果在同一时间有太多的线程想得到相同的互斥体，就会导致过 度的环境切换，大量的CPU时间就会花在内核调度器上，因而几乎没有时间执行工作。
32位系统限制为每个线程使用4G地址空间。由于所有的线程都共享相同的地址空间，理论上整 个服务器就被限制为使用4G RAM,即便实际上有更多的物理RAM也不例外。实际使用时，地址空间 会在一个小得多的限值下开始变得非常拥挤。
拥挤的32位地址空间会带来另一个问题。每个线程都需要一些堆栈空间，在分配了堆栈后， 即便不使用所分配的大部分空间，服务器也会为其保留地址空间。每个新堆栈都会减少用于堆的 潜在空间。因此，即使有足够的物理内存，也不可能同时使用大型缓冲区，同时使用大量并发线 程，以及同时为每个线程提供足够的堆栈空间。

#### 进程的优点

线程的缺点与使用多进程的优点相对应：

编程错误并不致命。尽管有可能发生，但一个坏分支服务器进程并不容易中断整个服务器。
编程错误发生的可能性小得多。在大多数时候，程序员只需要考虑一个线程的执行，而不用受 可能的并发侵入者的打扰。
飘忽不定的漏洞少得多。如果漏洞出现一次，通常非常容易复制。由于各个分支进程有自己的 地址空间，它们之间并没有太多的交互。
在32位系统中，地址空间用完的问题并不严重。

#### 进程的缺点

内存利用不够好。当子进程发生分支时，会不必要地复制大型内存程序段。
需要采用特殊技术实现进程数据共享。（IPC）
创建进程比创建线程需要更多的内核系统开销。对性能的一个很大的打击是需要 复制父进程的 数据段。不过，Linux 在这方面的手段是执行所谓的copy-on-write 。除非子进程或父进程修改了 父进程页，否则并不真正复制父进程页。在此之前，父子进程使用相同的页。
进程之间的环境切换比线程之间的环境切换更为耗时，因为内核需要切换页，文件描述符表 及其他额外的内容信息。留给服务器执行实际工作的时间减少。
以上关于进程和线程的优缺点，都来自《深入理解mysql核心技术》

### 线程API

进程原语和线程原语的比较：

| 进程原语 |      线程原语       |             描述             |
| :------: | :-----------------: | :--------------------------: |
|   fork   |   pthread_create    |        创建新的控制流        |
|   exit   |    pthread_exit     |      从现有的控制流退出      |
| waitpid  |    pthread_join     |    从控制流中得到退出状态    |
|  atexit  | pthread_cancle_push | 注册在退出控制流时调用的函数 |
|  getpid  |    pthread_self     |        获取控制流的ID        |
|  abort   |   pthread_cancle    |    请求控制流的非正常退出    |

## 互斥锁

### 互斥锁简介

1）间接相互制约关系（互斥）

若某一进程要求使用某种资源，而该资源正好被另一进程使用，并且该资源不允许两个进程同时使用，那么该进程只好等待已占有的资源的进程释放资源后再使用。这种制约关系可以用“进程-资源-进程”的形式表示。例如，打印机资源，进程互斥经典问题中生产者-生产者问题。

2）直接相互制约关系（同步）

某一进程若收不到另一进程提供的必要信息就不能继续运行下去，表明了两个进程之间在某些点上要交换信息，相互交流运行情况。这种制约关系的进本形式是“进程-进程”。例如生产者与消费者问题，生产者生产产品并放入缓冲池，消费者从缓冲池取走产品进行消费。这两者就是同步关系。

区分互斥和同步只需记住，**同类进程即为互斥关系**，不同类进程即为同步关系。

临界资源：同时只允许一个进程使用的资源。

临界区：进程中用于访问临界资源的代码段，又称临界段。

每个进程的临界区代码可以不同，临界区代码由于要访问临界资源，因此要在进入临界区之前进行检查，至于每个进程对临界资源进行怎样的操作，这和临界资源及互斥同步管理是无关的。

### 互斥锁创建

有两种方法创建互斥锁，静态方式和动态方式。POSIX定义了一个宏PTHREAD_MUTEX_INITIALIZER 来静态初始化互斥锁，方法如下：

`pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;`在LinuxThreads实现中， pthread_mutex_t是一个结构，而PTHREAD_MUTEX_INITIALIZER则是一个结构常量。

动态方式是采用pthread_mutex_init()函数来初始化互斥锁，API定义如下：

` int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr)`
其中mutexattr用于指定互斥锁属性（见下），如果为NULL则使用缺省属性。 pthread_mutex_destroy ()用于注销一个互斥锁，API定义如下：
` int pthread_mutex_destroy(pthread_mutex_t *mutex)`

### 锁操作

锁操作主要包括加锁pthread_mutex_lock()、解锁pthread_mutex_unlock()和测试加锁 pthread_mutex_trylock()三个，不论哪种类型的锁，都不可能被两个不同的线程同时得到， 而必须等待解锁。对于普通锁和适应锁类型，解锁者可以是同进程内任何线程； 而检错锁则必须由加锁者解锁才有效，否则返回EPERM；对于嵌套锁，文档和实现要求必须由 加锁者解锁，但实验结果表明并没有这种限制，这个不同目前还没有得到解释。在同一进程中 的线程，如果加锁后没有解锁，则任何其他线程都无法再获得锁。

`int pthread_mutex_lock(pthread_mutex_t *mutex)`
`int pthread_mutex_unlock(pthread_mutex_t *mutex)`
`int pthread_mutex_trylock(pthread_mutex_t *mutex)`
pthread_mutex_trylock() 语义与pthread_mutex_lock()类似，不同的是在锁已经被占据时返回 EBUSY而不是挂起等待。

### 读者写者问题

读者————写者问题是一个用信号量实现的经典进程同步问题。在系统中，一个数据集(如文件或 记录) 被几个并发进程共享，这些线程分两类，一部分只要求进行读操作，称之为“读者”； 另一类要求写或修改操作，我们称之为“写者“。一般而言，对一个数据集，为了保证数据的完整 性、正确性，允许多个读者进程同时访问，但是不允许一个写者进程同其它任何一个进程（读者 或者写者）同时访问，而这类问题就称之为"读者-写者"问题。

读者优先的算法在操作系统相关的书籍中都有介绍，这是一种最简单的解决办法： 当没有写进 程正在访问共享数据集时，读进程可以进入访问，否则必须等待。而读者优先的算法存在"饿死 写者"线程的问题：只要有读者不断到来，写者就要持久地等待，直到所有的读者都读完且没有 新的读者到来时写者才能写数据集。而在很多情况下我们需要避免"饿死写者"，故而采用写者优 先算法：

在写者优先算法中，我们要实现的目标是：

1.要让读者与写者之间、以及写者与写者之问要互斥地访同数据集； 2.在无写进程到来时各读者可同时访问数据集； 3.在读者和写者都等待时访问时写者优先．

在实现写者优先时，增加一个互斥量，用于写者优先。当有写者来时，就不在允许读者去读取数据， 等待正在读数据的读者完成以后开始写数据，以此实现写者优先。

```c
/*使用了多线程 编译命令为： g++ reader_writer.c -l pthread -o a.out */
#include <pthread.h>
#include <signal.h>
#include "apue.h"
#define N 5 //No. of reader
#define M 5 //No. of reading and writing
 
pthread_mutex_t rd = PTHREAD_MUTEX_INITIALIZER; // it's mean reader can reading
pthread_mutex_t wr = PTHREAD_MUTEX_INITIALIZER; //it's mean writer can writing
 
int readCount = 0;
 
void* reader(void *arg)
{
    int n = M;
    int id = (int)arg;
    while (n--)
    {
        sleep( rand() % 3);
        // 该读者要读数据 首先获取读者锁 线程共享资源进行写入操作时要加锁避免竞争
        pthread_mutex_lock(&rd);
        readCount++;
        if( readCount == 1)
        {
            // 当有读者想要读取资源时，第一个读者在读数据之前要加锁写锁，后续读者不需要再加写锁
            // 加写锁是为了写者优先写入数据
            pthread_mutex_lock(&wr);
        }
        // 共享资源readCount操作完后解锁读锁
        pthread_mutex_unlock(&rd);
        // 各读者可以同时读取数据
        printf("reader %d is reading\n", id);
        sleep( rand() % 3);
 		// 共享资源操作前加锁
        pthread_mutex_lock(&rd);
        readCount--;
        if (readCount == 0)
        {
            // 当没有读者读取数据时解锁写着锁
            pthread_mutex_unlock(&wr);
        }
        // 共享资源readCount操作完后解锁读锁
        pthread_mutex_unlock(&rd);
        printf("reader %d is leaving\n", id);
    }
    printf("----reader %d has done----\n", (int)arg);
}
 
void* writer(void *arg)
{
    int n = M;
    while (n--)
    {
        sleep( rand() % 3);
        // 写操作加写锁
        pthread_mutex_lock(&wr);
        printf("\twriter is writing\n");
        sleep( rand() % 3);
        pthread_mutex_unlock(&wr);
        printf("\twriter is leaving\n");
    }
    printf("----writer has done----\n");
 
}
 
 
int main(int argc, const char *argv[])
{
    int err;
    pthread_t tid[N], writerTid;
    int i;
 
 
    err = pthread_create(&writerTid, NULL, writer, (void *)NULL);
    if (err != 0)
    {
        err_quit("can't create process for writer");
    }
 
    for (i = 0; i < N; i++)
    {
        err = pthread_create(&tid[i], NULL, reader, (void *)(i+1));
        if (err != 0)
        {
            err_quit("can't create process for reader");
        }
    }
    pause();
    return 0;
}
```

如何体现的写者优先？

在共享数据读写操作之前，读者写者都有可能随时对资源进行访问，既不能限制读者读数据，又不能限制写者写入数据。当某个读者在写者访问之前已经开始读取数据，写者就要等待这个或多个读者访问完数据，当所有读者读取完后，写者再写入数据；当写者快于各个读者之前有写入数据需求，写者可以持续写完数据，随后读者才有机会读取数据。

对应的，设置一个全局变量readCount作为当前读取资源的读者数，第一个读者请求访问资源时，首先对全局变量readCount作为线程共享资源进行加锁(读锁)，并标记readCount自增，该读者是第一个读者访问资源时，要等待写者写入，即获取写锁。这里判定第一个读者是因为后续的读者在已有写锁后没必要再申请写锁，但是需要在访问资源前标记readCount++。对readCount操作完成后，要释放读锁。接下来读者自由读取资源。读取完后，申请读锁，操作readCount—，表示读者完成读取操作，当readCount==0时表示所有读者均读取资源完毕，释放写锁，表示写者可以再申请写锁进行写入资源操作。最后释放因操作readCount申请的读锁。相比较读者来说，写者仅需要在写入操作之前申请写锁，写入完资源后释放写锁。

## 读写锁

读写锁适合于对数据结构的读次数比写次数多得多的情况.因为,读模式锁定时可以共享,以写 模式锁住时意味着独占,所以**读写锁**又叫**共享-独占锁.**

### 初始化和销毁

```c
include <pthread.h>
pthread_rwlock_t _lock;	// 声明一个读写锁
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const
    pthread_rwlockattr_t *restrict attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
```

成功则返回0,出错则返回错误编号. 同互斥量以上,在释放读写锁占用的内存之前,需要先通过 pthread_rwlock_destroy对读写锁进行清理工作, 释放由init分配的资源.

### 读和写

```c
include <pthread.h>

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
```

成功则返回0,出错则返回错误编号.这3个函数分别实现获取读锁,获取写锁和释放锁的操作.获 取锁的两个函数是阻塞操作,同样,非阻塞的函数为:

```c
include <pthread.h>

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
```

成功则返回0,出错则返回错误编号.非阻塞的获取锁操作,如果可以获取则返回0,否则返回 错误的EBUSY.

### 解决读者写者问题

```c
#include <pthread.h>
#include <signal.h>
#include "apue.h"
#define N 5 //No. of reader
#define M 5 //No. of reading and writing
 
pthread_rwlock_t lock; //it's mean writer can writing
 
int readCount = 0;
 
void* reader(void *arg)
{
    int n = M;
    int id = (int)arg;
    while (n--)
    {
        sleep( rand() % 3);
        pthread_rwlock_rdlock(&lock);
        printf("reader %d is reading\n", id);
        sleep( rand() % 3);
 
        pthread_rwlock_unlock(&lock);
        printf("reader %d is leaving\n", id);
    }
    printf("----reader %d has done----\n", (int)arg);
}
 
void* writer(void *arg)
{
    int n = M;
    while (n--)
    {
        sleep( rand() % 3);
        pthread_rwlock_wrlock(&lock);
        printf("\twriter is writing\n");
        sleep( rand() % 3);
        pthread_rwlock_unlock(&lock);
        printf("\twriter is leaving\n");
    }
    printf("----writer has done----\n");
}
 
int main(int argc, const char *argv[])
{
    int err;
    pthread_t tid[N], writerTid;
    int i;
 
    err = pthread_create(&writerTid, NULL, writer, (void *)NULL);
    if (err != 0)
    {
        err_quit("can't create process for writer");
 
    }
 
    pthread_rwlock_init(&lock, NULL);
    for (i = 0; i < N; i++)
    {
        err = pthread_create(&tid[i], NULL, reader, (void *)(i+1));
        if (err != 0)
        {
            err_quit("can't create process for reader");
        }
    }
    pause();
    pthread_rwlock_destroy(&lock);
    return 0;
}
```

## boost mutex

boost::mutex提供了跨平台的锁操作，不允许多个线程同时访问共享资源，从而确保共享资源不被脏写。在本文中仅仅是介绍简单的两种锁，最高效的锁boost::mutex和区域锁boost::mutex::scoped_lock

boost::mutex例子

```c++
#include <boost/thread/mutex.hpp>

boost::mutex m_mutexAccessServiceManager;

void CSettingCenter::ClearPlatformServiceInfoCache()

{

	m_mutexAccessServiceManager.lock();

	m_mapAccessServiceManager.clear();

	m_mutexAccessServiceManager.unlock();

}
```

区域锁boost::mutex::scoped_lock顾名思义就是在作用域内有效，当离开作用域自动释放锁，传递参数是锁。区域锁就是把锁封装到一个对象里面。锁的初始化放到构造函数，锁的释放放到析构函数。这样当锁离开作用域时，

析构函数会自动释放锁。即使运行时抛出异常，由于析构函数仍然会自动运行，所以锁仍然能自动释放。一个典型的区域锁

```C++
void test()

{

	boost::mutex::scoped_lock lock(m_mutexAccessServiceManager);

	m_mapAccessServiceManager.clear();

}


```

应用于有大量的return返回的代码，避免出现死锁的问题