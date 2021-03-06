线程池原理以及创建 C++实现

本文给出了一个通用的线程池框架，该框架将与线程执行相关的任务进行了高层次的抽象，使之与具体的执行任务无关。另外该线程池具有动态伸缩性，它能根据执行任务的轻重自动调整线程池中线程的数量。

目前的大多数网络服务器，包括Web服务器、Email服务器以及数据库服务器等都具有一个共同点，就是单位时间内必须处理数目巨大的连接请求，但处理时间却相对较短。
传统多线程方案中我们采用的服务器模型则是一旦接受到请求之后，即创建一个新的线程，由该线程执行任务。任务执行完毕后，线程退出，这就是是“即时创建，即时销毁”的策略。尽管与创建进程相比，创建线程的时间已经大大的缩短，但是如果提交给线程的任务是执行时间较短，而且执行次数极其频繁，那么服务器将处于不停的创建线程，销毁线程的状态。

除此之外，线程池能够减少创建的线程个数。通常线程池所允许的并发线程是有上界的，如果同时需要并发的线程数超过上界，那么一部分线程将会等待。线程池采用预创建的技术，在应用程序启动之后，将立即创建一定数量的线程(N1)，放入空闲队列中。这些线程都是处于阻塞（Suspended）状态，不消耗CPU，但占用较小的内存空间。当任务到来后，缓冲池选择一个空闲线程，把任务传入此线程中运行。当N1个线程都在处理任务后，缓冲池自动创建一定数量的新线程，用于处理更多的任务。在任务执行完毕后线程也不退出，而是继续保持在池中等待下一次的任务。当系统比较空闲时，大部分线程都一直处于暂停状态，线程池自动销毁一部分线程，回收系统资源。

## 构建线程池框架

-   一般线程池都必须具备下面几个组成部分：
    线程池管理器:用于创建并管理线程池
-   工作线程: 线程池中实际执行的线程
-   任务接口: 尽管线程池大多数情况下是用来支持网络服务器，但是我们将线程执行的任务抽象出来，形成任务接口，从而是的线程池与具体的任务无关。
-   任务队列:线程池的概念具体到实现则可能是队列，链表之类的数据结构，其中保存执行线程。

我们实现的通用线程池框架由五个重要部分组成CThreadManage，CThreadPool，CThread，CJob，CWorkerThread，除此之外框架中还包括线程同步使用的类CThreadMutex和CCondition。
CJob是所有的任务的基类，其提供一个接口Run，所有的任务类都必须从该类继承，同时实现Run方法。该方法中实现具体的任务逻辑。

-   CThread是Linux中线程的包装，其封装了Linux线程最经常使用的属性和方法，它也是一个抽象类，是所有线程类的基类，具有一个接口Run。
-   CWorkerThread是实际被调度和执行的线程类，其从CThread继承而来，实现了CThread中的Run方法。
-   CThreadPool是线程池类，其负责保存线程，释放线程以及调度线程。
-   CThreadManage是线程池与用户的直接接口，其屏蔽了内部的具体实现。
-   CThreadMutex用于线程之间的互斥。
-   CCondition则是条件变量的封装，用于线程之间的同步。

## 系统组件

### CThreadManager

接口定义如下

```cpp
class CThreadManage
{
private:
    CThreadPool*    m_Pool;
    int          m_NumOfThread;
protected:
public:
    void     SetParallelNum(int num);
    CThreadManage();
    CThreadManage(int num);
    virtual ~CThreadManage();

    void    Run(CJob* job,void* jobdata);
    void    TerminateAll(void);
};
```

其中m_Pool指向实际的线程池；m_NumOfThread是初始创建时候允许创建的并发的线程个数。另外Run和TerminateAll方法也非常简单，只是简单的调用CThreadPool的一些相关方法而已。

实现方法：

```cpp
CThreadManage::CThreadManage(){
    m_NumOfThread = 10;
    m_Pool = new CThreadPool(m_NumOfThread);
}
CThreadManage::CThreadManage(int num){
    m_NumOfThread = num;
    m_Pool = new CThreadPool(m_NumOfThread);
}
CThreadManage::~CThreadManage(){
    if(NULL != m_Pool)
    delete m_Pool;
}
void CThreadManage::SetParallelNum(int num){
    m_NumOfThread = num;
}
void CThreadManage::Run(CJob* job,void* jobdata){
    m_Pool->Run(job,jobdata);
}
void CThreadManage::TerminateAll(void){
    m_Pool->TerminateAll();
}
```

### CThread

CThread 类实现了对Linux中线程操作的封装，它是所有线程的基类，也是一个抽象类，提供了一个抽象接口Run，所有的CThread都必须实现该Run方法。CThread的定义如下所示：

```cpp
class CThread
{
private:
    int          m_ErrCode;
    Semaphore    m_ThreadSemaphore;  //the inner semaphore, which is used to realize
    unsigned     long m_ThreadID;  
    bool         m_Detach;       //The thread is detached
    bool         m_CreateSuspended;  //if suspend after creating
    char*        m_ThreadName;
    ThreadState m_ThreadState;      //the state of the thread
protected:
    void     SetErrcode(int errcode){m_ErrCode = errcode;}
    static void* ThreadFunction(void*);
public:
    CThread();
    CThread(bool createsuspended,bool detach);
    virtual ~CThread();
    virtual void Run(void) = 0;
    void     SetThreadState(ThreadState state){m_ThreadState = state;}

    bool     Terminate(void);    //Terminate the threa
    bool     Start(void);        //Start to execute the thread
    void     Exit(void);
    bool     Wakeup(void);
  
    ThreadState  GetThreadState(void){return m_ThreadState;}
    int      GetLastError(void){return m_ErrCode;}
    void     SetThreadName(char* thrname){strcpy(m_ThreadName,thrname);}
    char*    GetThreadName(void){return m_ThreadName;}
    int      GetThreadID(void){return m_ThreadID;}

    bool     SetPriority(int priority);
    int      GetPriority(void);
    int      GetConcurrency(void);
    void     SetConcurrency(int num);
    bool     Detach(void);
    bool     Join(void);
    bool     Yield(void);
    int      Self(void);
};
```

线程的状态可以分为四种，空闲、忙碌、挂起、终止(包括正常退出和非正常退出)。由于目前Linux线程库不支持挂起操作，因此，我们的此处的挂起操作类似于暂停。如果线程创建后不想立即执行任务，那么我们可以将其“暂停”，如果需要运行，则唤醒。有一点必须注意的是，一旦线程开始执行任务，将不能被挂起，其将一直执行任务至完毕。
线程类的相关操作均十分简单。线程的执行入口是从Start()函数开始，其将调用函数ThreadFunction，ThreadFunction再调用实际的Run函数，执行实际的任务。

### CThreadPool

CThreadPool是线程的承载容器，一般可以将其实现为堆栈、单向队列或者双向队列。在我们的系统中我们使用STL Vector对线程进行保存。CThreadPool的实现代码如下：

定义接口

```cpp
class CThreadPool
{
friend class CWorkerThread;
private:
    unsigned int m_MaxNum;   //the max thread num that can create at the same time
    unsigned int m_AvailLow; //The min num of idle thread that shoule kept
    unsigned int m_AvailHigh;    //The max num of idle thread that kept at the same time
    unsigned int m_AvailNum; //the normal thread num of idle num;
    unsigned int m_InitNum;  //Normal thread num;
protected:
    CWorkerThread* GetIdleThread(void);

    void    AppendToIdleList(CWorkerThread* jobthread);
    void    MoveToBusyList(CWorkerThread* idlethread);
    void    MoveToIdleList(CWorkerThread* busythread);

    void    DeleteIdleThread(int num);
    void    CreateIdleThread(int num);
public:
    CThreadMutex m_BusyMutex;    //when visit busy list,use m_BusyMutex to lock and unlock
    CThreadMutex m_IdleMutex;    //when visit idle list,use m_IdleMutex to lock and unlock
    CThreadMutex m_JobMutex; //when visit job list,use m_JobMutex to lock and unlock
    CThreadMutex m_VarMutex;

    CCondition       m_BusyCond; //m_BusyCond is used to sync busy thread list
    CCondition       m_IdleCond; //m_IdleCond is used to sync idle thread list
    CCondition       m_IdleJobCond;  //m_JobCond is used to sync job list
    CCondition       m_MaxNumCond;

    vector<CWorkerThread*>   m_ThreadList;
    vector<CWorkerThread*>   m_BusyList;     //Thread List
    vector<CWorkerThread*>   m_IdleList; //Idle List

    CThreadPool();
    CThreadPool(int initnum);
    virtual ~CThreadPool();

    void    SetMaxNum(int maxnum){m_MaxNum = maxnum;}
    int     GetMaxNum(void){return m_MaxNum;}
    void    SetAvailLowNum(int minnum){m_AvailLow = minnum;}
    int     GetAvailLowNum(void){return m_AvailLow;}
    void    SetAvailHighNum(int highnum){m_AvailHigh = highnum;}
    int     GetAvailHighNum(void){return m_AvailHigh;}
    int     GetActualAvailNum(void){return m_AvailNum;}
    int     GetAllNum(void){return m_ThreadList.size();}
    int     GetBusyNum(void){return m_BusyList.size();}
    void    SetInitNum(int initnum){m_InitNum = initnum;}
    int     GetInitNum(void){return m_InitNum;}
 
    void    TerminateAll(void);
    void    Run(CJob* job,void* jobdata);
};
```

具体实现

```cpp
CThreadPool::CThreadPool()
{
    m_MaxNum = 50;
    m_AvailLow = 5;
    m_InitNum=m_AvailNum = 10 ; 
    m_AvailHigh = 20;

    m_BusyList.clear();
    m_IdleList.clear();
    for(int i=0;i<m_InitNum;i++){
    CWorkerThread* thr = new CWorkerThread();
    thr->SetThreadPool(this);
    AppendToIdleList(thr);
    thr->Start();
    }
}

CThreadPool::CThreadPool(int initnum)
{
    assert(initnum>0 && initnum<=30);
    m_MaxNum   = 30;
    m_AvailLow = initnum-10>0?initnum-10:3;
    m_InitNum=m_AvailNum = initnum ; 
    m_AvailHigh = initnum+10;

    m_BusyList.clear();
    m_IdleList.clear();
    for(int i=0;i<m_InitNum;i++){
    CWorkerThread* thr = new CWorkerThread();
    AppendToIdleList(thr);
    thr->SetThreadPool(this);
    thr->Start();       //begin the thread,the thread wait for job
    }
}

CThreadPool::~CThreadPool()
{
   TerminateAll();
}

void CThreadPool::TerminateAll()
{
    for(int i=0;i < m_ThreadList.size();i++) {
    CWorkerThread* thr = m_ThreadList[i];
    thr->Join();
    }
    return;
}

CWorkerThread* CThreadPool::GetIdleThread(void)
{
    while(m_IdleList.size() ==0 )
    m_IdleCond.Wait();
  
    m_IdleMutex.Lock();
    if(m_IdleList.size() > 0 )
    {
    CWorkerThread* thr = (CWorkerThread*)m_IdleList.front();
    printf("Get Idle thread %dn",thr->GetThreadID());
    m_IdleMutex.Unlock();
    return thr;
    }
    m_IdleMutex.Unlock();

    return NULL;
}

//add an idle thread to idle list
void CThreadPool::AppendToIdleList(CWorkerThread* jobthread)
{
    m_IdleMutex.Lock();
    m_IdleList.push_back(jobthread);
    m_ThreadList.push_back(jobthread);
    m_IdleMutex.Unlock();
}

//move and idle thread to busy thread
void CThreadPool::MoveToBusyList(CWorkerThread* idlethread)
{
    m_BusyMutex.Lock();
    m_BusyList.push_back(idlethread);
    m_AvailNum--;
    m_BusyMutex.Unlock();
 
    m_IdleMutex.Lock();
    vector<CWorkerThread*>::iterator pos;
    pos = find(m_IdleList.begin(),m_IdleList.end(),idlethread);
    if(pos !=m_IdleList.end())
    m_IdleList.erase(pos);
    m_IdleMutex.Unlock();
}

void CThreadPool::MoveToIdleList(CWorkerThread* busythread)
{
    m_IdleMutex.Lock();
    m_IdleList.push_back(busythread);
    m_AvailNum++;
    m_IdleMutex.Unlock();

    m_BusyMutex.Lock();
    vector<CWorkerThread*>::iterator pos;
    pos = find(m_BusyList.begin(),m_BusyList.end(),busythread);
    if(pos!=m_BusyList.end())
    m_BusyList.erase(pos);
    m_BusyMutex.Unlock();

    m_IdleCond.Signal();
    m_MaxNumCond.Signal();
}

//create num idle thread and put them to idlelist
void CThreadPool::CreateIdleThread(int num)
{
    for(int i=0;i<num;i++){
    CWorkerThread* thr = new CWorkerThread();
    thr->SetThreadPool(this);
    AppendToIdleList(thr);
    m_VarMutex.Lock();
    m_AvailNum++;
    m_VarMutex.Unlock();
    thr->Start();       //begin the thread,the thread wait for job
    }
}

void CThreadPool::DeleteIdleThread(int num)
{
    printf("Enter into CThreadPool::DeleteIdleThreadn");
    m_IdleMutex.Lock();
    printf("Delete Num is %dn",num);
    for(int i=0;i<num;i++){
    CWorkerThread* thr;
    if(m_IdleList.size() > 0 ){
            thr = (CWorkerThread*)m_IdleList.front();
            printf("Get Idle thread %dn",thr->GetThreadID());
    }

    vector<CWorkerThread*>::iterator pos;
    pos = find(m_IdleList.begin(),m_IdleList.end(),thr);
    if(pos!=m_IdleList.end())
        m_IdleList.erase(pos);
    m_AvailNum--;
    printf("The idle thread available num:%d n",m_AvailNum);
    printf("The idlelist              num:%d n",m_IdleList.size());
    }
    m_IdleMutex.Unlock();
}
void CThreadPool::Run(CJob* job,void* jobdata)
{
    assert(job!=NULL);
  
    //if the busy thread num adds to m_MaxNum,so we should wait
    if(GetBusyNum() == m_MaxNum)
        m_MaxNumCond.Wait();

    if(m_IdleList.size()<m_AvailLow)
    {
    if(GetAllNum()+m_InitNum-m_IdleList.size() < m_MaxNum )
        CreateIdleThread(m_InitNum-m_IdleList.size());
    else
        CreateIdleThread(m_MaxNum-GetAllNum());
    }

    CWorkerThread*  idlethr = GetIdleThread();
    if(idlethr !=NULL)
    {
    idlethr->m_WorkMutex.Lock();
    MoveToBusyList(idlethr);
    idlethr->SetThreadPool(this);
    job->SetWorkThread(idlethr);
    printf("Job is set to thread %d n",idlethr->GetThreadID());
    idlethr->SetJob(job,jobdata);
    }
}
```

在CThreadPool中存在两个链表，一个是空闲链表，一个是忙碌链表。Idle链表中存放所有的空闲进程，当线程执行任务时候，其状态变为忙碌状态，同时从空闲链表中删除，并移至忙碌链表中。

在CThreadPool的构造函数中，将创建m_InitNum个线程，创建之后即调用AppendToIdleList放入Idle链表中，由于目前没有任务分发给这些线程，因此线程执行Start后将自己挂起。
事实上，线程池中容纳的线程数目并不是一成不变的，其会根据执行负载进行自动伸缩。为此在CThreadPool中设定四个变量：
m_InitNum：初始创建时线程池中的线程的个数。
m_MaxNum:当前线程池中所允许并发存在的线程的最大数目。
m_AvailLow:当前线程池中所允许存在的空闲线程的最小数目，如果空闲数目低于该值，表明负载可能过重，此时有必要增加空闲线程池的数目。实现中我们总是将线程调整为m_InitNum个。
m_AvailHigh：当前线程池中所允许的空闲的线程的最大数目，如果空闲数目高于该值，表明当前负载可能较轻，此时将删除多余的空闲线程，删除后调整数也为m_InitNum个。
m_AvailNum：目前线程池中实际存在的线程的个数，其值介于m_AvailHigh和m_AvailLow之间。如果线程的个数始终维持在m_AvailLow和m_AvailHigh之间，则线程既不需要创建，也不需要删除，保持平衡状态。因此如何设定m_AvailLow和m_AvailHigh的值，使得线程池最大可能的保持平衡态，是线程池设计必须考虑的问题。
线程池在接受到新的任务之后，线程池首先要检查是否有足够的空闲池可用。检查分为三个步骤：
(1)检查当前处于忙碌状态的线程是否达到了设定的最大值m_MaxNum，如果达到了，表明目前没有空闲线程可用，而且也不能创建新的线程，因此必须等待直到有线程执行完毕返回到空闲队列中。
(2)如果当前的空闲线程数目小于我们设定的最小的空闲数目m_AvailLow，则我们必须创建新的线程，默认情况下，创建后的线程数目应该为m_InitNum，因此创建的线程数目应该为( 当前空闲线程数与m_InitNum);但是有一种特殊情况必须考虑，就是现有的线程总数加上创建后的线程数可能超过m_MaxNum，因此我们必须对线程的创建区别对待。
    if(GetAllNum()+m_InitNum-m_IdleList.size() < m_MaxNum )
        CreateIdleThread(m_InitNum-m_IdleList.size());
    else
        CreateIdleThread(m_MaxNum-GetAllNum());
如果创建后总数不超过m_MaxNum，则创建后的线程为m_InitNum；如果超过了，则只创建( m_MaxNum-当前线程总数 )个。
(3)调用GetIdleThread方法查找空闲线程。如果当前没有空闲线程，则挂起；否则将任务指派给该线程，同时将其移入忙碌队列。
当线程执行完毕后，其会调用MoveToIdleList方法移入空闲链表中，其中还调用m_IdleCond.Signal()方法，唤醒GetIdleThread()中可能阻塞的线程。

### CWorkerThread

CWorkerThread是CThread的派生类，是事实上的工作线程。在CThreadPool的构造函数中，我们创建了一定数量的CWorkerThread。一旦这些线程创建完毕，我们将调用Start()启动该线程。Start方法最终会调用Run方法。Run方法是个无限循环的过程。在没有接受到实际的任务的时候，m_Job为NULL，此时线程将调用Wait方法进行等待，从而处于挂起状态。一旦线程池将具体的任务分发给该线程，其将被唤醒，从而通知线程从挂起的地方继续执行。

其定义的接口声明

```cpp
class CWorkerThread:public CThread
{
private:
    CThreadPool*  m_ThreadPool;
    CJob*    m_Job;
    void*    m_JobData;
  
    CThreadMutex m_VarMutex;
    bool      m_IsEnd;
protected:
public:
    CCondition   m_JobCond;
    CThreadMutex m_WorkMutex;
    CWorkerThread();
    virtual ~CWorkerThread();
    void Run();
    void    SetJob(CJob* job,void* jobdata);
    CJob*   GetJob(void){return m_Job;}
    void    SetThreadPool(CThreadPool* thrpool);
    CThreadPool* GetThreadPool(void){return m_ThreadPool;}
};
```

具体定义

```cpp
CWorkerThread::CWorkerThread()
{
    m_Job = NULL;
    m_JobData = NULL;
    m_ThreadPool = NULL;
    m_IsEnd = false;
}
CWorkerThread::~CWorkerThread()
{
    if(NULL != m_Job)
    delete m_Job;
    if(m_ThreadPool != NULL)
    delete m_ThreadPool;
}

void CWorkerThread::Run()
{
    SetThreadState(THREAD_RUNNING);
    for(;;)
    {
    while(m_Job == NULL)
        m_JobCond.Wait();

    m_Job->Run(m_JobData);
    m_Job->SetWorkThread(NULL);
    m_Job = NULL;
    m_ThreadPool->MoveToIdleList(this);
    if(m_ThreadPool->m_IdleList.size() > m_ThreadPool->GetAvailHighNum())
    {
m_ThreadPool->DeleteIdleThread(m_ThreadPool->m_IdleList.size()-m_T
hreadPool->GetInitNum());
    }
    m_WorkMutex.Unlock();
    }
}
void CWorkerThread::SetJob(CJob* job,void* jobdata)
{
    m_VarMutex.Lock();
    m_Job = job;
    m_JobData = jobdata;
    job->SetWorkThread(this);
    m_VarMutex.Unlock();
    m_JobCond.Signal();
}
void CWorkerThread::SetThreadPool(CThreadPool* thrpool)
{
    m_VarMutex.Lock();
    m_ThreadPool = thrpool;
    m_VarMutex.Unlock();
}
```

当线程执行任务之前首先必须判断空闲线程的数目是否低于m_AvailLow，如果低于，则必须创建足够的空闲线程，使其数目达到m_InitNum个，然后将调用MoveToBusyList()移出空闲队列，移入忙碌队列。当任务执行完毕后，其又调用MoveToIdleList()移出忙碌队列，移入空闲队列，等待新的任务。
除了Run方法之外，CWorkerThread中另外一个重要的方法就是SetJob，该方法将实际的任务赋值给线程。当没有任何执行任务即m_Job为NULL的时候，线程将调用m_JobCond.Wait进行等待。一旦Job被赋值给线程，其将调用m_JobCond.Signal方法唤醒该线程。由于m_JobCond属于线程内部的变量，每个线程都维持一个m_JobCond，只有得到任务的线程才被唤醒，没有得到任务的将继续等待。无论一个线程何时被唤醒，其都将从等待的地方继续执行m_Job->Run(m_JobData)，这是线程执行实际任务的地方。
在线程执行给定Job期间，我们必须防止另外一个Job又赋给该线程，因此在赋值之前，通过m_VarMutex进行锁定， Job执行期间，其于的Job将不能关联到该线程；任务执行完毕，我们调用m_VarMutex.Unlock()进行解锁，此时，线程又可以接受新的执行任务。
在线程执行任务结束后返回空闲队列前，我们还需要判断当前空闲队列中的线程是否高于m_AvailHigh个。如果超过m_AvailHigh，则必须从其中删除(m_ThreadPool->m_IdleList.size()-m_ThreadPool->GetInitNum())个线程，使线程数目保持在m_InitNum个。

### CJob

CJob类相对简单，其封装了任务的基本的属性和方法，其中最重要的是Run方法

```cpp
class CJob
{
private:
    int      m_JobNo;        //The num was assigned to the job
    char*    m_JobName;      //The job name
    CThread  *m_pWorkThread;     //The thread associated with the job
public:
    CJob( void );
    virtual ~CJob();
      
    int      GetJobNo(void) const { return m_JobNo; }
    void     SetJobNo(int jobno){ m_JobNo = jobno;}
    char*    GetJobName(void) const { return m_JobName; }
    void     SetJobName(char* jobname);
    CThread *GetWorkThread(void){ return m_pWorkThread; }
    void     SetWorkThread ( CThread *pWorkThread ){
        m_pWorkThread = pWorkThread;
    }
    virtual void Run ( void *ptr ) = 0;
};
```

具体实现定义

```cpp
CJob::CJob(void)
:m_pWorkThread(NULL)
,m_JobNo(0)
,m_JobName(NULL)
{
}
CJob::~CJob(){
    if(NULL != m_JobName)
    free(m_JobName);
}
void CJob::SetJobName(char* jobname)
{
    if(NULL !=m_JobName)    {
        free(m_JobName);
        m_JobName = NULL;
    }
    if(NULL !=jobname)    {
        m_JobName = (char*)malloc(strlen(jobname)+1);
        strcpy(m_JobName,jobname);
    }
}
```

## 线程池使用示例

至此我们给出了一个简单的与具体任务无关的线程池框架。使用该框架非常的简单，我们所需要的做的就是派生CJob类，将需要完成的任务实现在Run方法中。然后将该Job交由CThreadManage去执行。下面我们给出一个简单的示例程序

```cpp
class CXJob:public CJob
{
public:
    CXJob(){i=0;}
    ~CXJob(){}
    void Run(void* jobdata)    {
        printf("The Job comes from CXJOB\n");
        sleep(2);
    }
};

class CYJob:public CJob
{
public:
    CYJob(){i=0;}
    ~CYJob(){}
    void Run(void* jobdata)    {
        printf("The Job comes from CYJob\n");
    }
};

main()
{
    CThreadManage* manage = new CThreadManage(10);
    for(int i=0;i<40;i++)
    {
        CXJob*   job = new CXJob();
        manage->Run(job,NULL);
    }
    sleep(2);
    CYJob* job = new CYJob();
    manage->Run(job,NULL);
    manage->TerminateAll();
}
```

CXJob和CYJob都是从Job类继承而来，其都实现了Run接口。CXJob只是简单的打印一句”The Job comes from CXJob”，CYJob也只打印”The Job comes from CYJob”，然后均休眠2秒钟。在主程序中我们初始创建10个工作线程。然后分别执行40次CXJob和一次CYJob。

线程池适合场合
事实上，线程池并不是万能的。它有其特定的使用场合。线程池致力于减少线程本身的开销对应用所产生的影响，这是有前提的，前提就是线程本身开销与线程执行任务相比不可忽略。如果线程本身的开销相对于线程任务执行开销而言是可以忽略不计的，那么此时线程池所带来的好处是不明显的，比如对于FTP服务器以及Telnet服务器，通常传送文件的时间较长，开销较大，那么此时，我们采用线程池未必是理想的方法，我们可以选择“即时创建，即时销毁”的策略。
总之线程池通常适合下面的几个场合：
(1)  单位时间内处理任务频繁而且任务处理时间短
(2)  对实时性要求较高。如果接受到任务后在创建线程，可能满足不了实时要求，因此必须采用线程池进行预创建。
(3)  必须经常面对高突发性事件，比如Web服务器，如果有足球转播，则服务器将产生巨大的冲击。此时如果采取传统方法，则必须不停的大量产生线程，销毁线程。此时采用动态线程池可以避免这种情况的发生。