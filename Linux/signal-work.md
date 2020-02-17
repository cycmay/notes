# 信号机制

## 信号本质

软中断信号（signal，又简称为信号）用来通知进程发生了异步事件。在软件层次上是对中断机制的一种模拟，在原理上，一个进程收到一个信号与处理器收到一个中断请求可以说是一样的。信号是进程间通信机制中唯一的异步通信机制，一个进程不必通过任何操作来等待信号的到达，事实上，进程也不知道信号到底什么时候到达。进程之间可以互相通过系统调用kill发送软中断信号。内核也可以因为内部事件而给进程发送信号，通知进程发生了某个事件。信号机制除了基本通知功能外，还可以传递附加信息。

收到信号的进程对各种信号有不同的处理方法。处理方法可以分为三类：

第一种是类似中断的处理程序，对于需要处理的信号，进程可以指定处理函数，由该函数来处理。

第二种方法是，忽略某个信号，对该信号不做任何处理，就象未发生过一样。

第三种方法是，对该信号的处理保留系统的默认值，这种缺省操作，对大部分的信号的缺省操作是使得进程终止。进程通过系统调用signal来指定进程对某个信号的处理行为。



对于一个完整的信号生命周期(从信号发送到相应的处理函数执行完毕)来说，可以分为三个阶段：

信号诞生

## 

信号在进程中注册

信号的执行和注销

>   Linux支持的信号列表如下。很多信号是与机器的体系结构相关的
>
>   信号值 默认处理动作 发出信号的原因
>
>   SIGHUP 1 A 终端挂起或者控制进程终止
>
>   SIGINT 2 A 键盘中断（如break键被按下）
>
>   SIGQUIT 3 C 键盘的退出键被按下
>
>   SIGILL 4 C 非法指令
>
>   SIGABRT 6 C 由abort(3)发出的退出指令
>
>   SIGFPE 8 C 浮点异常
>
>   SIGKILL 9 AEF Kill信号
>
>   SIGSEGV 11 C 无效的内存引用
>
>   SIGPIPE 13 A 管道破裂: 写一个没有读端口的管道
>
>   SIGALRM 14 A 由alarm(2)发出的信号
>
>   SIGTERM 15 A 终止信号
>
>   SIGUSR1 30,10,16 A 用户自定义信号1
>
>   SIGUSR2 31,12,17 A 用户自定义信号2
>
>   SIGCHLD 20,17,18 B 子进程结束信号
>
>   SIGCONT 19,18,25 进程继续（曾被停止的进程）
>
>   SIGSTOP 17,19,23 DEF 终止进程
>
>   SIGTSTP 18,20,24 D 控制终端（tty）上按下停止键
>
>   SIGTTIN 21,21,26 D 后台进程企图从控制终端读
>
>   SIGTTOU 22,22,27 D 后台进程企图从控制终端写

## 信号的安装

如果进程要处理某一信号，那么就要在进程中安装该信号。安装信号主要用来确定信号值及进程针对该信号值的动作之间的映射关系，即进程将要处理哪个信号；该信号被传递给进程时，将执行何种操作。

linux主要有两个函数实现信号的安装：signal()、sigaction()。其中signal()只有两个参数，不支持信号传递信息，主要是用于前32种非实时信号的安装；而sigaction()是较新的函数（由两个系统调用实现：sys_signal以及sys_rt_sigaction），有三个参数，支持信号传递信息，主要用来与 sigqueue() 系统调用配合使用，当然，sigaction()同样支持非实时信号的安装。sigaction()优于signal()主要体现在支持信号带有参数。

## sigaction() 

\#include <signal.h>

int sigaction(int signum,const struct sigaction *act,struct sigaction *oldact));

sigaction函数用于改变进程接收到特定信号后的行为。该函数的第一个参数为信号的值，可以为除SIGKILL及SIGSTOP外的任何一个特定有效的信号（为这两个信号定义自己的处理函数，将导致信号安装错误）。第二个参数是指向结构sigaction的一个实例的指针，在结构sigaction的实例中，指定了对特定信号的处理，可以为空，进程会以缺省方式对信号处理；第三个参数oldact指向的对象用来保存返回的原来对相应信号的处理，可指定oldact为NULL。如果把第二、第三个参数都设为NULL，那么该函数可用于检查信号的有效性。

第二个参数最为重要，其中包含了对指定信号的处理、信号所传递的信息、信号处理函数执行过程中应屏蔽掉哪些信号等等。

sigaction结构定义如下：

```c
struct sigaction
{
    union
    {
        __sighandler_t _sa_handler;
        void (*_sa_sigaction)(int,struct siginfo *, void *)；
    }_u
 
     sigset_t sa_mask；
    unsigned long sa_flags；
}
```

1、联合数据结构中的两个元素_sa_handler以及*_sa_sigaction指定信号关联函数，即用户指定的信号处理函数。除了可以是用户自定义的处理函数外，还可以为SIG_DFL(采用缺省的处理方式)，也可以为SIG_IGN（忽略信号）。

2、由_sa_sigaction是指定的信号处理函数带有三个参数，是为实时信号而设的（当然同样支持非实时信号），它指定一个3参数信号处理函数。第一个参数为信号值，第三个参数没有使用，第二个参数是指向siginfo_t结构的指针，结构中包含信号携带的数据值

前面在讨论系统调用sigqueue发送信号时，sigqueue的第三个参数就是sigval联合数据结构，当调用sigqueue时，该数据结构中的数据就将拷贝到信号处理函数的第二个参数中。这样，在发送信号同时，就可以让信号传递一些附加信息。信号可以传递信息对程序开发是非常有意义的。

3、sa_mask指定在信号处理程序执行过程中，哪些信号应当被阻塞。缺省情况下当前信号本身被阻塞，防止信号的嵌套发送，除非指定SA_NODEFER或者SA_NOMASK标志位。

注：请注意sa_mask指定的信号阻塞的前提条件，是在由sigaction（）安装信号的处理函数执行过程中由sa_mask指定的信号才被阻塞。

4、sa_flags中包含了许多标志位，包括刚刚提到的SA_NODEFER及SA_NOMASK标志位。另一个比较重要的标志位是SA_SIGINFO，当设定了该标志位时，表示信号附带的参数可以被传递到信号处理函数中，因此，应该为sigaction结构中的sa_sigaction指定处理函数，而不应该为sa_handler指定信号处理函数，否则，设置该标志变得毫无意义。即使为sa_sigaction指定了信号处理函数，如果不设置SA_SIGINFO，信号处理函数同样不能得到信号传递过来的数据，在信号处理函数中对这些信息的访问都将导致段错误（Segmentation fault）。

## 信号应用实例

```c
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
void new_op(int,siginfo_t*,void*);
int main(int argc,char**argv)
{
    struct sigaction act;
    int sig;
    sig=atoi(argv[1]);
    sigemptyset(&act.sa_mask);
    act.sa_flags=SA_SIGINFO;
    act.sa_sigaction=new_op;
    if(sigaction(sig,&act,NULL) < 0)
    {
        printf("install sigal error\n");
    }
    while(1)
    {
        sleep(2);
        printf("wait for the signal\n");
    }
}
void new_op(int signum,siginfo_t *info,void *myact)

{
		printf("receive signal %d", signum);
		sleep(5);
}
```

后台编译运行

g++ test.cpp -o test

./test signo &	# signo为处理信号 将被注册到信号