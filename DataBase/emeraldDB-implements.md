# emeraldDB implements

## enveriments

mkdir emeralddb
  176  ls
  177  cd emeralddb/
  179  mkdir boost
  180  mkdir src
  181  touch build_boost.sh
  182  touch GUN-AGPL-3.0.txt
  183  touch build.sh
  189  sudo apt-get install g++
  190  gcc -v
  191  sudo apt-get install gdb
  193  sudo apt-get install auto-conf
  194  sudo apt-get install autoconf
  195  sudo apt-get install libtool
  196  sudo apt-get install make
  197  sudo apt-get install x11-apps
  198  sudo apt-get install unzio
  199  sudo apt-get install unzip

  204  cd ~/temp/
  206  cd emeralddb_build/
  207  ll
  208  tar -zxvf boost_1_72_0.tar.gz
  209  ls
  210  unzip bson-cpp-master.zip
  211  ls
  212  tar -zxvf eclipse-SDK-4.2.1-linux-gtk-x86_64.tar.gz
  213  ls
  214  tar -zxvf apache-log4j-2.13.0-bin.tar.gz
  215  ls
  216  cd boost_1_72_0/
  217  ls
  218  cp -R * ~/emeralddb/boost/
  219  cd ~/emeralddb/
  224  chmod 755 build_boost.sh
  231  ./build_boost.sh
  236  find ./ -name bjam
  237  vim build_boost.sh
  238  ./build_boost.sh
  239  vim build_boost.sh
  240  sudo apt-get install gcc-multilib
  241  ./build_boost.sh
  242  sudo apt-get install gcc-multilib g++-multilib
  243  ./build_boost.sh
  255  mkdir jdk_linux64
  275  cd ~/temp/
  276  ls
  277  chmod 755 jdk-6u45-linux-x64.bin
  278  ./jdk-6u45-linux-x64.bin
  279  ls
  280  cd jdk1.6.0_45/
  281  ls
  282  mkdir -p ~/emeralddb/java
  283  mv ~/emeralddb/jdk_linux64/ ~/emeralddb/java
  284  cp -R * ~/emeralddb/java/jdk_linux64/
  287  cd ~/emeralddb/
  289  cd src
  292  mkdir bson
  293  midor client
  294  mkdir client
  295  mkdir dms
  296  mkdir driver
  297  mkdir include
  298  mkdir ixm
  299  mkdir mov
  301  ls
  302  mkdir msg
  303  mkdir oss
  304  mkdir pd
  305  mkdir pmd
  306  mkdir rtn
  310  cd ~/temp/emeralddb_build/bson-cpp-master/
  312  ls
  313  cp -R src ~/emeralddb/src/bson/
  314  cp -R GUN* ~/emeralddb/src/bson/
  315  cp -R GNU* ~/emeralddb/src/bson/
  316  cp -R APACHE-2.0.txt ~/emeralddb/src/bson/
  317  cp -R lib ~/emeralddb/src/bson/src/
  318  ls
  319  cd ~/emeralddb/src/bson/
  320  ls
  321  ls src/
  322  cd src/
  323  ls
  324  cd util/
  325  ls
  326  vim time_support.h
  327  cd ..
  328  cd lib/
  329  ls
  330  vim base64.h
  331  mv md5.c md5.cpp
  332  vi nonce.cpp
  333  cd ../
  334  ls
  335  cd ..
  336  ls
  337  cd ..
  338  ks
  339  ls
  340  cd ..
  341  ls
  342  chmod 755 build
  343  chmod 755 build.sh
  344  vim build
  345  vim build.sh
  346  cd src/
  347  ls
  348  cd emeralddb/
  349  ls
  350  cd src/
  351  ls
  352  cd pmd/
  353  ls
  354  vim pmdMain.cpp
  355  vim ~/.vimrc



在项目 emeralddb 目录下运行 build.sh ，emralddb 可执行文件编译连接成功，执行后输出hello world。

环境基本搭建完成 保存快照



## linux 下 socket 通信机制

在Linux中一切都是文件，socket 也被认为是文件的一种，两台计算机之间的通信，实际上就是两个socket文件的相互读写。

### 创建socket

使用头文件<sys/socket.h> 的socket()函数来创建套接字：

函数原型：`int socket(int af, int type, int protocol);`

1. af 地址符 Address Family，即IP地址类型，常用：AF_INET, AF_INET6. （INET 是Internet的缩写）AF_INET: IPv4地址，AF_INET6: IPv6地址。
2. type 数据传输方式，常用：SOCK_STREAM, SOCK_DGRAM
3. protocol 传输协议， 常用：IPPROTO_TCP, IPPROTO_UDP. 分别代表TCP传输协议和UDP传输协议。使用0表示系统自动推演协议。
4. 返回值为int 表示**文件描述符**， File Descriptor，Linux系统中创建的文件都有一个int型的编号。

示例：
`int tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // TCP套接字`
`int udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // UDP套接字`

### 绑定端口与建立连接

bind() 函数将套接字与特定额IP地址和端口绑定起来，这样流经该IP地址对应端口的数据才会交给套接字处理

connect() 函数建立客户端的连接

bind函数原型：

`int bind(int sock, struct sockaddr *addr, socklen_t addrlen);	//Linux bind()`

1. sock socket文件描述符
2. addr sockaddr结构体变量的指针
3. addrlen addr变量大小 可有sizeof() 计算得出

代码示例：

```c
// 创建套接字
int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

// 创建sockaddr_in结构体变量
struct sockaddr_in serv_addr;
memset(&serv_addr, 0, sizeof(serv_addr));	// 每个字节0
serv_addr.sin_family = AF_INET	// 使用ipv4类型地址
serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");	// 服务器ip地址
serv_addr.sin_port = htons(1234);	// 端口

// 将套接字和IP、端口绑定
// 这里将struct sockaddr_in 类型强制转换成sockaddr类型
bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
```



struct sockadd_in :

```c
struct sockaddr_in
{
	sa_family_t		sin_family;	// 地址族 即地址类型 AF address family
	uint16_t			sin_port;		// 16位的端口号
  struct in_addr sin_addr;	// 32位IP地址
  char 					sin_zero[8];	//不使用
}
```

1. sin_port 端口号，uint16_t 长度为两字节，理论上端口号取值范围0~65536，0~1023一般有操作系统占用分配给特定的服务程序。端口号要用htons()转换

struct in_addr:

```c
strcut in_addr
{
	// in_addr_t 在头文件<netinet/in.h>	中定义，等价于unsigned long，长度为4个字节
	in_addr_t 	s_addr;	// 32位IP地址
}
```

s_addr 为长整型，故传入的参数字符串型要使用inet_addr()函数转换。

struct sockaddr:

```c
struct sockaddr
{
	sa_family_t sin_family;
	char 				sa_data[14];	//IP地址和端口号
}
```

可以发现，sockaddr的长度等于sockaddr_in的长度，sockaddr内部成员sa_data保存的是ip地址和端口号，并没有函数对sockaddr结构体的ip和port分开输入，于是转换成对sockaddr_in的定义，并强制将其转换为sockaddr. 同时，sockaddr_in6 表示IPv6地址。

struct sockaddr_in6:

```c
struct sockaddr_in6 
{ 
    sa_family_t sin6_family;  //(2)地址类型，取值为AF_INET6
    in_port_t sin6_port;  //(2)16位端口号
    uint32_t sin6_flowinfo;  //(4)IPv6流信息
    struct in6_addr sin6_addr;  //(4)具体的IPv6地址
    uint32_t sin6_scope_id;  //(4)接口范围ID
};
```

connect() 函数

connect()函数用来创建连接，函数原型：

`int connetc(int sock, struck sockaddr *serv_addr, socklen_t addrlen);`

参数说明与bind()函数相同。

### 监听状态

服务端程序使用bind()绑定套接字与ip、端口后，需要使用listen()函数使套接字进入被动监听状态，随后accept()函数就可以响应客户端请求。

监听函数

listen()函数可以使套接字进入被动监听状态，函数原型：
`int listen(int sock, int backlog);	//linux`

backlog 为请求队列最大长度，被动监听是套接字在没有请求时进入睡眠状态，接收到请求时被唤醒。

当套接字正在处理某一请求时，新的请求会被放入缓冲区并排队，等待上一个请求处理完毕后，新的请求队列队首才会被处理。将backlog自定义设置为10-20都可以，或者设置为SOMAXCONN，意思为有操作系统自行决定队列长度。当队列满时，新的请求会被报错ECONNREFUSED

接收请求

当服务端处于监听状态后，即可通过accept()函数接收客户端请求，函数原型：

`int accept(int sock, struct sockaddr *addr, socktlen_t addrlen);`

参数说明参照上文listen()函数和connetc()函数

函数返回值为新的套接字，之后的两者之间的通信则使用这个新生成的套接字。

accept()函数会阻塞当前进程，直到新的请求。

### 接收发送数据

套接字文件和普通文件一样，在c语言中，可以直接通过write()写入，read()读取。

服务端在accept()生成的套接字中写入数据，客户端就能读取到，这样就是完成了通信。

wirte() 函数原型:

`ssize_t write(int fd, const void *buf, size_t nbytes);`

1. fd 写入的socket文件描述符
2. buf 要写入数据的缓冲区地址
3. nbytes 要写入数据的字节数
4. size_t 是通过 typedef 声明的 unsigned int 类型；ssize_t 在 "size_t" 前面加了一个"s"，代表 signed，即 ssize_t 是通过 typedef 声明的 signed int 类型。
5. 返回值 write() 函数会将缓冲区 buf 中的 nbytes 个字节写入文件 fd，成功则返回写入的字节数，失败则返回 -1。

read() 函数原型：

`ssize_t read(int fd, void *buf, size_t nbytes);`

1. buf 为要接收数据的缓冲区地址
2. nbytes 为要读取的数据的字节数
3. 其他参数参照write() 函数
4. 返回值 read() 函数会从 fd 文件中读取 nbytes 个字节并保存到缓冲区 buf，成功则返回读取到的字节数（但遇到文件结尾则返回0），失败则返回 -1。

发送与接收

`int send(int sock, const char *buf, int len, int flags);`

1. flags 为发送数据时的选项 最后的 flags 参数一般设置为 0 或 NULL

`int recv(int sock, char *buf, int len, int flags);`



### 代码注释

------



```c
_sockAddress.sin_addr.s_addr = htonl(INADDR_ANY);
_sockAddress.sin_port = htons(port);
```

htonl 将主机字节类型转换为网络字节类型 l表示hostlong 主机为长整型

htons s表示短整型

参数INADDR_ANY 表示创建的socket接收任何地址的信息。

参考：

> ## Name
>
> htonl, htons, ntohl, ntohs - convert values between host and network byte order
>
> ## Synopsis
>
> ```
> #include <arpa/inet.h>
> uint32_t htonl(uint32_t hostlong);
> uint16_t htons(uint16_t hostshort);
> uint32_t ntohl(uint32_t netlong);
> uint16_t ntohs(uint16_t netshort);
> ```
>
> ## Description
>
> The **htonl**() function converts the unsigned integer *hostlong* from host byte order to network byte order.
>
> The **htons**() function converts the unsigned short integer *hostshort* from host byte order to network byte order.
>
> The **ntohl**() function converts the unsigned integer *netlong* from network byte order to host byte order.
>
> The **ntohs**() function converts the unsigned short integer *netshort* from network byte order to host byte order.
>
> On the i386 the host byte order is Least Significant Byte first, whereas the network byte order, as used on the Internet, is Most Significant Byte first.
>
> ## Conforming to
>
> POSIX.1-2001.
>
> Some systems require the inclusion of *<netinet/in.h>* instead of *<arpa/inet.h>*.

> If the (sin_addr.s_addr) field is set to the constant INADDR_ANY, as defined in netinet/in.h, the caller is requesting that the socket be bound to all network interfaces on the host.

------



```c
if( (hp=gethostbyname(pHostName)) )
		_sockAddress.sin_addr.s_addr = *((int *)hp->h_addr_list[0]);
else
		_sockAddress.sin_addr.s_addr = inet_addr(pHostName);
```

gethostbyname(name) 通过传入字符串类型或者说是指针来查询到hostent结构体。或者转化为二进制格式的网络字节类型。s_addr 设置socket的主机地址。

> ```c
> #include <netdb.h>
> struct hostent *gethostbyname(const char *name);
> 
> The hostent structure is defined in <netdb.h> as follows:
> 
> struct hostent {
>   char  *h_name;            /* official name of host */
>   char **h_aliases;         /* alias list */
>   int    h_addrtype;        /* host address type */
>   int    h_length;          /* length of address */
>   char **h_addr_list;       /* list of addresses */
> }
> #define h_addr h_addr_list[0] /* for backward compatibility */
> 
> ```
>
> The gethostbyname() function returns a structure of type hostent for the given host name.  Here name is either a hostname or an IPv4 address in standard dot notation (as for inet_addr(3)).  If name is an IPv4 address, no lookup is performed and gethostbyname() simply copies name into the h_name field and its struct in_addr equivalent  into the h_addr_list[0] field of the returned hostent structure.  If  name doesn't end in a dot and the environment variable HOSTALIASES is  set, the alias file pointed to by HOSTALIASES will first be searched for name (see hostname(7) for the file format).  The current domain and its parents are searched unless name ends in a dot.

> ```c
> #include <sys/socket.h>
> #include <netinet/in.h>
> #include <arpa/inet.h>
> 
> in_addr_t inet_addr(const char *cp);
> ```
>
> The inet_addr() function converts the Internet host address cp from IPv4 numbers-and-dots notation into binary data in network byte order.  If the input is invalid, INADDR_NONE (usually -1) is returned.  Use of this function is problematic because -1 is a valid address (255.255.255.255).  Avoid its use in favor of inet_aton(),
> inet_pton(3), or getaddrinfo(3), which provide a cleaner way to indicate error return.

------



```c
rc = getsockname(_fd, (sockaddr*)&_sockAddress, &_addressLen);
rc = getpeername(_fd, (sockaddr *)&_peerAddress, &_peerAddressLen);
```

get socket name.  get peer name

_fd 为已知socket的描述符，将新分配内存空间的 _sockAddress 作为引用参数传入，可以将socket的主机地址保存到该内存空间，同理对peerAddress。

参考：

```
#include <sys/socket.h>

int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

```
getsockname() returns the current address to which the socket sockfd is bound, in the buffer pointed to by addr.  The addrlen argument should be initialized to indicate the amount of space (in bytes) pointed to by addr.  On return it contains the actual size of the socket address. The returned address is truncated if the buffer provided is too small; in this case, addrlen will return a value greater than was supplied to the call.
On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.
```

```
getpeername() returns the address of the peer connected to the socket sockfd, in the buffer pointed to by addr.  The addrlen argument should be initialized to indicate the amount of space pointed to by addr.  On return it contains the actual size of the name returned (in bytes).  The name is truncated if the buffer provided is too small. The returned address is truncated if the buffer provided is too small; in this case, addrlen will return a value greater than was supplied to the call.
```

------

setTimeout

```
struct timeval tv;
tv.tv_sec = seconds;
tv.tv_usec = 0;
// windows take milliseconds as parameter
// linux takes timeval as input
rc = setsockopt(_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
```

参考：Name

> getsockopt, setsockopt - get and set options on sockets
>
> ## Synopsis
>
> ```
> #include <sys/types.h>          /* See NOTES */
> #include <sys/socket.h>
> int getsockopt(int sockfd, int level, int optname,               void *optval,
> socklen_t *optlen);int setsockopt(int sockfd, int level, int optname,
>              const void *optval, socklen_t optlen);
> ```
>
> ## Description
>
> getsockopt() and setsockopt() manipulate options for the socket referred to by the file descriptor sockfd. Options may exist at multiple protocol levels; they are always present at the uppermost socket level.
>
> When manipulating socket options, the level at which the option resides and the name of the option must be specified. To manipulate options at the sockets API level, *level* is specified as **SOL_SOCKET**. To manipulate options at any other level the protocol number of the appropriate protocol controlling the option is supplied. For example, to indicate that an option is to be interpreted by the **TCP** protocol, *level* should be set to the protocol number of **TCP**; see ***getprotoent**(3)*.
>
> The arguments *optval* and *optlen* are used to access option values for **setsockopt**(). For **getsockopt**() they identify a buffer in which the value for the requested **option**(s) are to be returned. For **getsockopt**(), *optlen* is a value-result argument, initially containing the size of the buffer pointed to by *optval*, and modified on return to indicate the actual size of the value returned. If no option value is to be supplied or returned, *optval* may be NULL.
>
> *Optname* and any specified options are passed uninterpreted to the appropriate protocol module for interpretation. The include file *<sys/socket.h>* contains definitions for socket level options, described below. Options at other protocol levels vary in format and name; consult the appropriate entries in section 4 of the manual.
>
> Most socket-level options utilize an *int* argument for *optval*. For **setsockopt**(), the argument should be nonzero to enable a boolean option, or zero if the option is to be disabled.
>
> For a description of the available socket options see ***socket**(7)* and the appropriate protocol man pages.
>
> ## Return Value
>
> On success, zero is returned. On error, -1 is returned, and errno is set appropriately.

------



```c
struct linger _linger;
_linger.l_onoff = lOnOff;
_linger.l_linger = linger;
rc = setsockopt(_fd, SOL_SOCKET, SO_LINGER, (const char *)&_linger, sizeof(_linger));
```

参考：

当调用closesocket关闭套接字时，SO_LINGER将决定系统如何处理残存在套接字发送队列中的数据。处理方式无非两种：丢弃或者将数据继续发送至对端，优雅关闭连接。事实上，SO_LINGER并不被推荐使用，大多数情况下我们推荐使用默认的关闭方式（即下方表格中的第一种情况）。

下方代码段显示linger结构语法，表格为不同参数情况下的套接字行为。

typedef struct linger { 
![img](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)      u_short l_onoff;    //开关，零或者非零 
![img](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)      u_short l_linger;   //优雅关闭最长时限 
![img](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)    } linger; 



| l_onoff | l_linger | closesocket行为                                              | 发送队列                                       | 底层行为                                                     |
| ------- | -------- | ------------------------------------------------------------ | ---------------------------------------------- | ------------------------------------------------------------ |
| 零      | 忽略     | 立即返回。                                                   | 保持直至发送完成。                             | 系统接管套接字并保证将数据发送至对端。                       |
| 非零    | 零       | 立即返回。                                                   | 立即放弃。                                     | 直接发送RST包，自身立即复位，不用经过2MSL状态。对端收到复位错误号。 |
| 非零    | 非零     | 阻塞直到l_linger时间超时或数据发送完成。(套接字必须设置为阻塞zhuan) | 在超时时间段内保持尝试发送，若超时则立即放弃。 | 超时则同第二种情况，若发送完成则皆大欢喜。                   |

------



```
struct timeval maxSelectTime;
fd_set fds;
maxSelectTime.tv_sec = timeout / 1000000;
maxSelectTime.tv_usec = timeout % 1000000;
```

参考：

The <sys/time.h> header defines the  timeval structure that includes at least the following members:

```
/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */
struct timeval {
	long	tv_sec;		/* seconds */
	long	tv_usec;	/* and microseconds */
};
```

------

```
FD_ZERO(&fds);
FD_SET(_fd, &fds);
	rc = select(maxFD + 1, NULL, &fds, NULL,
							timeout>=0?&maxSelectTime:NULL);
```

参考：

> ## Name
>
> select, pselect, FD_CLR, FD_ISSET, FD_SET, FD_ZERO - synchronous I/O multiplexing
>
> ## Synopsis
>
> ```
> /* According to POSIX.1-2001 */
> #include <sys/select.h>
> 
> /* According to earlier standards */
> #include <sys/time.h>
> #include <sys/types.h>
> #include <unistd.h>
> 
> int select(int nfds, fd_set *readfds, fd_set *writefds,
>            fd_set *exceptfds, struct timeval *timeout);
> 
> void FD_CLR(int fd, fd_set *set);
> int  FD_ISSET(int fd, fd_set *set);
> void FD_SET(int fd, fd_set *set);
> void FD_ZERO(fd_set *set);
> 
> #include <sys/select.h>
> 
> int pselect(int nfds, fd_set *readfds, fd_set *writefds,
>             fd_set *exceptfds, const struct timespec *timeout,
>             const sigset_t *sigmask);
> ```
>
> - **pselect**(): _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
>
> ## Description
>
> **select**() and **pselect**() allow a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation (e.g., input possible). A file descriptor is considered ready if it is possible to perform the corresponding I/O operation (e.g., **read**(2)) without blocking.
>
> The operation of **select**() and **pselect**() is identical, other than these three differences:
>
> - (i)**select**() uses a timeout that is a *struct timeval* (with seconds and microseconds), while **pselect**() uses a *struct timespec* (with seconds and nanoseconds).(ii)**select**() may update the *timeout* argument to indicate how much time was left. **pselect**() does not change this argument.(iii)**select**() has no *sigmask* argument, and behaves as **pselect**() called with NULL *sigmask*.
> - Three independent sets of file descriptors are watched. Those listed in *readfds* will be watched to see if characters become available for reading (more precisely, to see if a read will not block; in particular, a file descriptor is also ready on end-of-file), those in *writefds* will be watched to see if a write will not block, and those in *exceptfds* will be watched for exceptions. On exit, the sets are modified in place to indicate which file descriptors actually changed status. Each of the three file descriptor sets may be specified as NULL if no file descriptors are to be watched for the corresponding class of events.Four macros are provided to manipulate the sets. **FD_ZERO**() clears a set. **FD_SET**() and **FD_CLR**() respectively add and remove a given file descriptor from a set. **FD_ISSET**() tests to see if a file descriptor is part of the set; this is useful after **select**() returns.*nfds* is the highest-numbered file descriptor in any of the three sets, plus 1.The *timeout* argument specifies the minimum interval that **select**() should block waiting for a file descriptor to become ready. (This interval will be rounded up to the system clock granularity, and kernel scheduling delays mean that the blocking interval may overrun by a small amount.) If both fields of the *timeval* structure are zero, then **select**() returns immediately. (This is useful for polling.) If *timeout* is NULL (no timeout), **select**() can block indefinitely.*sigmask* is a pointer to a signal mask (see **sigprocmask**(2)); if it is not NULL, then **pselect**() first replaces the current signal mask by the one pointed to by *sigmask*, then does the "select" function, and then restores the original signal mask.Other than the difference in the precision of the *timeout* argument, the following **pselect**() call:`ready = pselect(nfds, &readfds, &writefds, &exceptfds,                 timeout, &sigmask);`is equivalent to *atomically* executing the following calls:` sigset_t origmask;  pthread_sigmask(SIG_SETMASK, &sigmask, &origmask);  ready = select(nfds, &readfds, &writefds, &exceptfds, timeout);  pthread_sigmask(SIG_SETMASK, &origmask, NULL);`The reason that **pselect**() is needed is that if one wants to wait for either a signal or for a file descriptor to become ready, then an atomic test is needed to prevent race conditions. (Suppose the signal handler sets a global flag and returns. Then a test of this global flag followed by a call of **select**() could hang indefinitely if the signal arrived just after the test but just before the call. By contrast, **pselect**() allows one to first block signals, handle the signals that have come in, then call **pselect**() with the desired *sigmask*, avoiding the race.)
>
> **The timeout**
>
> - The time structures involved are defined in *<sys/time.h>* and look like
>
>   `struct timeval {     long    tv_sec;         /* seconds */     long    tv_usec;        /* microseconds */ };`
>
> - and
>
>   `struct timespec {     long    tv_sec;         /* seconds */     long    tv_nsec;        /* nanoseconds */ };`
>
> - (However, see below on the POSIX.1-2001 versions.)Some code calls **select**() with all three sets empty, *nfds* zero, and a non-NULL *timeout* as a fairly portable way to sleep with subsecond precision.On Linux, **select**() modifies *timeout* to reflect the amount of time not slept; most other implementations do not do this. (POSIX.1-2001 permits either behavior.) This causes problems both when Linux code which reads *timeout* is ported to other operating systems, and when code is ported to Linux that reuses a *struct timeval* for multiple **select**()s in a loop without reinitializing it. Consider *timeout* to be undefined after **select**() returns.
>
> ## Return Value
>
> On success, **select**() and **pselect**() return the number of file descriptors contained in the three returned descriptor sets (that is, the total number of bits that are set in *readfds*, *writefds*, *exceptfds*) which may be zero if the timeout expires before anything interesting happens. On error, -1 is returned, and *errno* is set appropriately; the sets and *timeout* become undefined, so do not rely on their contents after an error.

## 锁机制

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
        pthread_mutex_lock(&rd);
        readCount++;
        if( readCount == 1)
        {
            pthread_mutex_lock(&wr);
        }
        pthread_mutex_unlock(&rd);
 
        printf("reader %d is reading\n", id);
        sleep( rand() % 3);
 
        pthread_mutex_lock(&rd);
        readCount--;
        if (readCount == 0)
        {
            pthread_mutex_unlock(&wr);
        }
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

### 读写锁

读写锁适合于对数据结构的读次数比写次数多得多的情况.因为,读模式锁定时可以共享,以写 模式锁住时意味着独占,所以读写锁又叫共享-独占锁.

初始化和销毁:

```c
include <pthread.h>
pthread_rwlock_t _lock;	// 声明一个读写锁
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const
    pthread_rwlockattr_t *restrict attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
```


成功则返回0,出错则返回错误编号. 同互斥量以上,在释放读写锁占用的内存之前,需要先通过 pthread_rwlock_destroy对读写锁进行清理工作, 释放由init分配的资源.

读和写:

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

解决读者写者问题

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

### 代码注释

ossXLatch	互斥锁

ossSLatch	读写锁

## 文件操作

### 文件描述符

   1、对于内核而言，所有打开文件都由文件描述符引用。文件描述符是一个非负整数。当打开一个现存文件或者创建一个新文件时，内核向进程返回一个文件描述符。当读写一个文件时，用open和creat返回的文件描述符标识该文件，将其作为参数传递给read和write。

按照惯例，UNIX shell使用文件描述符0与进程的标准输入相结合，文件描述符1与标准输出相结合，文件描述符2与标准错误输出相结合。STDIN_FILENO、STDOUT_FILENO、STDERR_FILENO这几个宏代替了0、1、2这几个魔数。

​    2、文件描述符，这个数字在一个进程中表示一个特定含义，当我们open一个文件时，操作系统在内存中构建了一些数据结构来表示这个动态文件，然后返回给应用程序一个数字作为文件描述符，这个数字就和我们内存中维护的这个动态文件的这些数据结构绑定上了，以后我们应用程序如果要操作这个动态文件，只需要用这个文件描述符区分。

​    3、文件描述符的作用域就是当前进程，出了这个进程文件描述符就没有意义了。

open函数打开文件，打开成功返回一个文件描述符，打开失败，返回-1。

### 代码注释

------



```c
class ossPrimitiveFileOp
{
    public:
    		// 在linux中文件描述符为int型
        typedef int handleType;
    private:
    		// 定义一个文件操作对象的句柄 即为文件描述符
        handleType  _fileHandle;
```

定义私有成员变量_fileHandle, 是文件操作对象操纵具体文件的文件描述符。

```c
private:
        // 构造方法的两种方式 禁止从一个对象生成另外一个对象
        ossPrimitiveFileOp(const ossPrimitiveFileOp &){};
        const ossPrimitiveFileOp &operator=(const ossPrimitiveFileOp &);
        bool _bIsStdout;
```

------

```c
int Open
   (
      const char * pFilePath,
      unsigned int options = OSS_PRIMITIVE_FILE_OP_OPEN_ALWAYS
   ) ;
// optoins->打开方式 reading only or writing only etc.
```

open64() 与open()类似 ，是linux提供文件操作API的扩展方法，

open(), open64() open a file 

SYNOPSIS

```
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int open(const char **pathname*, int *oflag*,...);
int open64(const char **pathname*, int *oflag*,...);
```

DESCRIPTION

The `open()` function establishes the connection between a file and a file descriptor. It creates an open file description that refers to a file and a file descriptor that refers to the open file description. The file descriptor is used by other I/O functions to refer to that file.

The `open()` function returns a file descriptor for the named file that is the lowest file descriptor not currently open for that process. The open file description is new, and therefore the file descriptor does not share it with any other process in the system. The `FD_CLOEXEC` file descriptor flag associated with the new file descriptor is cleared.

The `open64()` function is a part of the large file extensions, and is equivalent to calling `open()` with the `O_LARGEFILE` flag.

The file offset used to mark the current position within the file is set to the beginning of the file.

The file status flags and file access modes of the open file description are set according to the value of *oflag*. The value of *oflag* is the bitwise inclusive-OR of values from the following lists.

Applications must specify exactly one of the following three values (file access modes) in the value of *oflag*:

- `O_RDONLY` 

  Open for reading only.

- `O_WRONLY` 

  Open for writing only.

- `O_RDWR` 

  Open for both reading and writing.

当我们附带了权限后，打开的文件就只能按照这种权限来操作。以上这三个常数中应当只指定一 个。下列常数是可选择的：     

​        O_CREAT 若文件不存在则创建它。使用此选项时，需要同时说明第三个参数mode，用其说明该新文件的存取许可权限。

​        O_EXCL 如果同时指定了OCREAT，而文件已经存在，则出错。这可测试一个文件是

否存在，如果不存在则创建此文件成为一个原子操作。

​        O_APPEND 每次写时都加到文件的尾端。

​        O_TRUNC 属性去打开文件时，如果这个文件中本来是有内容的，而且为只读或只写成功打开，则将其长度截短为0。

------

```c

   do
   {
      _fileHandle = oss_open( pFilePath, mode, 0644 ) ;
   } while (( -1 == _fileHandle ) && ( EINTR == errno )) ;
```

尝试打开指定路径*pFilePath的文件，

**errno**

NAME  

```
       errno - number of last error
```

SYNOPSIS      

```
       #include <errno.h>
```

DESCRIPTION      

```
       The <errno.h> header file defines the integer variable errno, which
       is set by system calls and some library functions in the event of an
       error to indicate what went wrong.
```

------

lseek

lseek - reposition read/write file offset

Synopsis

\#include <sys/types.h>

\#include <unistd.h>

**off_t lseek(int** *fd***, off_t** *offset***, int** *whence***);**

Description

The lseek() function repositions the offset of the open file associated with the file descriptor fd to the argument offset according to the directive whence as follows:

- **SEEK_SET**

  The offset is set to *offset* bytes.

- **SEEK_CUR**

  The offset is set to its current location plus *offset* bytes.

- **SEEK_END**

  The offset is set to the size of the file plus *offset* bytes.

The **lseek**() function allows the file offset to be set beyond the end of the file (but this does not change the size of the file). If data is later written at this point, subsequent reads of the data in the gap (a "hole") return null bytes (aq\0aq) until data is actually written into the gap.

------

```c
do
      {
         bytesRead = oss_read( _fileHandle, pBuffer, size ) ;
      } while (( -1 == bytesRead ) && ( EINTR == errno )) ;
      if ( -1 == bytesRead )
      {
         goto err_read ;
      }
```



Name

read - read from a file descriptor

Synopsis

```
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);
```

Description

read() attempts to read up to count bytes from file descriptor fd into the buffer starting at buf.

On files that support seeking, the read operation commences at the current file offset, and the file offset is incremented by the number of bytes read. If the current file offset is at or past the end of file, no bytes are read, and **read**() returns zero.

If *count* is zero, **read**() *may* detect the errors described below. In the absence of any errors, or if **read**() does not check for errors, a **read**() with a *count* of 0 returns zero and has no other effects.

If *count* is greater than **SSIZE_MAX**, the result is unspecified.

Return Value

On success, the number of bytes read is returned (zero indicates end of file), and the file position is advanced by this number. It is not an error if this number is smaller than the number of bytes requested; this may happen for example because fewer bytes are actually available right now (maybe because we were close to end-of-file, or because we are reading from a pipe, or from a terminal), or because read() was interrupted by a signal. On error, -1 is returned, and errno is set appropriately. In this case it is left unspecified whether the file position (if any) changes.

------

`int ossPrimitiveFileOp::Write( const void * pBuffer, size_t size )`

```c
do
      {
         rc = oss_write( _fileHandle, &((char*)pBuffer)[currentSize],
                         size-currentSize ) ;
         if ( rc >= 0 )
            currentSize += rc ;
      } while ((( -1 == rc ) && ( EINTR == errno )) ||
               (( -1 != rc ) && ( currentSize != size ))) ;
```

循环的方法写入文件

NAME        

```
       write - write to a file descriptor
```

SYNOPSIS        

```
       #include <unistd.h>

       ssize_t write(int fd, const void *buf, size_t count);
```

DESCRIPTION        

```
       write() writes up to count bytes from the buffer starting at buf to
       the file referred to by the file descriptor fd.

       The number of bytes written may be less than count if, for example,
       there is insufficient space on the underlying physical medium, or the
       RLIMIT_FSIZE resource limit is encountered (see setrlimit(2)), or the
       call was interrupted by a signal handler after having written less
       than count bytes.  (See also pipe(7).)

       For a seekable file (i.e., one to which lseek(2) may be applied, for
       example, a regular file) writing takes place at the file offset, and
       the file offset is incremented by the number of bytes actually
       written.  If the file was open(2)ed with O_APPEND, the file offset is
       first set to the end of the file before writing.  The adjustment of
       the file offset and the write operation are performed as an atomic
       step.

       POSIX requires that a read(2) that can be proved to occur after a
       write() has returned will return the new data.  Note that not all
       filesystems are POSIX conforming.

       According to POSIX.1, if count is greater than SSIZE_MAX, the result
       is implementation-defined; see NOTES for the upper limit on Linux.
```

RETURN VALUE        

```
       On success, the number of bytes written is returned.  On error, -1 is
       returned, and errno is set to indicate the cause of the error.
```

 

------

```c
int ossPrimitiveFileOp::fWrite( const char * format, ... )
{
   int rc = 0 ;
   va_list ap ;
   char buf[OSS_PRIMITIVE_FILE_OP_FWRITE_BUF_SIZE] = { 0 } ;

   va_start( ap, format ) ;
   vsnprintf( buf, sizeof( buf ), format, ap ) ;
   va_end( ap ) ;

   rc = Write( buf ) ;

   return rc ;
}
```

格式化输入，*format为格式化字符串形式。

`(...)`参数使用va_list 读取，保存在ap变量中，va_start(ap, format)表示序列化开始使用，vsnprintf(buf, sizeof(buf), format, ap)表示将按照字符个数(sizeof(buf))将格式化的字符串赋值给buf. 最后以va_end(ap) 结束序列化。

 Description

The C library macro **void va_start(va_list ap, last_arg)** initializes **ap** variable to be used with the **va_arg** and **va_end** macros. The **last_arg** is the last known fixed argument being passed to the function i.e. the argument before the ellipsis.

This macro must be called before using **va_arg** and **va_end**.

Declaration

Following is the declaration for va_start() macro.

```
void va_start(va_list ap, last_arg);
```

Parameters

- **ap** − This is the object of **va_list** and it will hold the information needed to retrieve the additional arguments with **va_arg**.
- **last_arg** − This is the last known fixed argument being passed to the function.

Example

The following example shows the usage of va_start() macro.

```
#include<stdarg.h>
#include<stdio.h>

int sum(int, ...);

int main(void) {
   printf("Sum of 10, 20 and 30 = %d\n",  sum(3, 10, 20, 30) );
   printf("Sum of 4, 20, 25 and 30 = %d\n",  sum(4, 4, 20, 25, 30) );

   return 0;
}

int sum(int num_args, ...) {
   int val = 0;
   va_list ap;
   int i;

   va_start(ap, num_args);
   for(i = 0; i < num_args; i++) {
      val += va_arg(ap, int);
   }
   va_end(ap);
 
   return val;
}
```

Let us compile and run the above program to produce the following result −

```
Sum of 10, 20 and 30 = 60
Sum of 4, 20, 25 and 30 = 79
```

**vsprintf**

```
int vsprintf (char * s, const char * format, va_list arg );
```

Write formatted data from variable argument list to string

Composes a string with the same text that would be printed if *format* was used on [printf](http://www.cplusplus.com/printf), but using the elements in the variable argument list identified by *arg* instead of additional function arguments and storing the resulting content as a *C string* in the buffer pointed by *s*.

Internally, the function retrieves arguments from the list identified by *arg* as if [va_arg](http://www.cplusplus.com/va_arg) was used on it, and thus the state of *arg* is likely to be altered by the call.

In any case, *arg* should have been initialized by [va_start](http://www.cplusplus.com/va_start) at some point before the call, and it is expected to be released by [va_end](http://www.cplusplus.com/va_end) at some point after the call.

Parameters

- s

  Pointer to a buffer where the resulting C-string is stored. The buffer should be large enough to contain the resulting string.

- format

  C string that contains a format string that follows the same specifications as *format* in [printf](http://www.cplusplus.com/printf) (see [printf](http://www.cplusplus.com/printf) for details).

- arg

  A value identifying a variable arguments list initialized with [va_start](http://www.cplusplus.com/va_start). [va_list](http://www.cplusplus.com/va_list) is a special type defined in <cstdarg>.

  ------

  `int ossPrimitiveFileOp::getSize( offsetType * const pFileSize )`

```c
oss_struct_stat buf       = { 0 } ;
if ( -1 == oss_fstat( _fileHandle, &buf ) )
   {
      rc = errno ;
      goto err_exit ;
   }

   *pFileSize = buf.st_size ;
```

oss_fstat 函数原型：Gets information about an open file.

```
#define oss_fstat          fstat64

```

SYNOPSIS

```
#include <sys/types.h>
#include <sys/stat.h>
int fstat(int *fildes*, struct stat **buf*);
int lstat(const char **pathname*, struct stat **buf*);
int stat(const char **pathname*, struct stat **buf*);
int fstat64(int *fildes*, struct stat64 **buf*);
int lstat64(const char **pathname*, struct stat64 **buf*);
int stat64(const char **pathname*, struct stat64 **buf*);
```

DESCRIPTION

The `stat()` function gets information about the named file and writes it to the area that *buf* points to. `stat()` updates any time-related fields before writing into the `stat` structure. The system must be able to search all directories leading to the file; however, read, write, or execute permission of the file is not required.

The `fstat()` function is identical to `stat()`, except that the file whose information is retrieved is specified by file descriptor rather than file name.

The `lstat()` function has the same effect as `stat()`, except when the specified path refers to a symbolic link. In that case, `lstat()` returns information about the link, while `stat()` returns information about the file the link references.

The `fstat64()`, `lstat64()`, and `stat64()` functions are identical to the `fstat()`, `lstat()` and `stat()` functions except that they take a pointer to `struct stat64` instead of `struct stat`. This permits applications to determine the size of a file that is larger than 2 gigabyes.

PARAMETERS

- *pathname* 

  Points to a path name that names a file. The calling process must have at least search permissions on *pathname*.

- *fildes* 

  Is a file descriptor referring to a file for which status is returned.

- *buf* 

  Points to a `stat` or `stat64` structure where status information about the file is to be placed.

RETURN VALUES

If successful, these functions return a value of zero. 

## 日志管理

在代码中嵌入log代码信息，主要记录下列信息： 

（1）记录系统运行异常信息。 

（2）记录系统运行状态信息。

（3）记录系统运行性能指标。  

### 全局工作

枚举类型的PDLEVEL. 从高到低代表的6种日志级别

```c
enum PDLEVEL
{
    PDSEVERE    =0,
    PDERROR,
    PDEVENT,
    PDWARNING,
    PDINFO,
    PDDEBUG
};
```

日志格式化输出形式：

```c
const static char *PD_LOG_HEADER_FORMAT="%04d-%02d-%02d-%02d:%02d:%02d:%06d\
Level:%s" OSS_NEWLINE"PID:%-37dTID:%d" OSS_NEWLINE"Funtion:%-32sLine:%d"\
 OSS_NEWLINE"File:%s" OSS_NEWLINE"Message:" OSS_NEWLINE"%s" OSS_NEWLINE OSS_NEWLINE;
```

日志的环境下全局变量设置：

```c
PDLEVEL _curPDLevel = PD_DFT_DIAGLEVEL;    // _curPDLevel 当前日志级别
char _pdDiagLogPath [OSS_MAX_PATHSIZE+1] = {0};	// _pdDiagLogPath 日志保存默认路径

ossXLatch _pdLogMutex;	// 互斥锁 保证日志文件在线程的安全性
ossPrimitiveFileOp _pdLogFile;	// 文件操作对象 用于保存日志
```

宏定义日志函数：

```c
#define PD_LOG(level, fmt, ...)                                             \
    do{                                                                     \
        if(_curPDLevel >= level)                                            \
        {                                                                   \                                         
            pdLog(level, __func__, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \   
        }                                                                   \
    }while(0)

#define PD_CHECK(cond, retCode, gotoLabel, level, fmt, ...)                 \
    do{                                                                     \
        if(!(cond))                                                         \
        {                                                                   \
            rc = (retCode);                                                 \
            PD_LOG((level), fmt, ##__VA_ARGS__);                            \
            goto gotoLabel;                                                 \
        }                                                                   \
    }while(0)

#define PD_RC_CHECK(rc, level, fmt, ...)                                    \
    do{                                                                     \
        PD_CHECK((EDB_OK==rc), (rc), error, (level), fmt, ##__VA_ARGS__);   \
    }while(0)

```

引用：

> `__VA_ARGS__` is only for macros; variadic functions are rather different. You need to use [`va_start`](http://www.cplusplus.com/reference/clibrary/cstdarg/va_start/), `va_arg` and `va_end` from `stdarg.h` to handle them.
>
> First, your function needs at least one named parameter, e.g.:
>
> ```c
> void COMMON_Print(const char* fmt, ...)
> ```
>
> Then you can define a `va_list` and use `va_start` to set it:
>
> ```c
> va_list args;
> va_start(args, fmt);
> // your code here
> va_end(args);
> ```
>
> Now you can use `args` to access the arguments; calling `va_arg(args, TYPE)` will return the next variadic argument, so you can just keep calling that until you've read all the arguments.
>
> If you're just trying to call `printf`, there's a printf variant called `vprintf` that takes the `va_list` directly:
>
> ```c
> vprintf(fmt, args);
> ```
>
> There is no easy way to call one variadic function from another; you need something like `vprintf` that takes the whole `va_list`

### 日志生成函数

```c
// log
void pdLog(PDLEVEL level, const char *func, const char *file, unsigned int line,
            const char *format, ...)
```

参数说明：

1. PDLEVEL level, 日志级别
2. char *func，日志记录函数名称
3. char *file， 日志记录文件名称
4. unsigned int line，源代码行号
5. char *format，格式化输出，其后 … 可以使用va_list 进行格式化输入

时间处理：

```c
struct tm otm;
struct timeval tv;
struct timezone tz;
time_t tt;
gettimeofday(&tv, &tz);
tt = tv.tv_sec;
localtime_r (&tt, &otm);
```

引用：

> **struct tm**
>
> Time structure
>
> Structure containing a calendar date and time broken down into its components.
>
> The structure contains nine members of type int(in any order), which are:
>
> - [C99 (C++11)](javascript:switch1.select(2))
>
> | Member   | Type  | Meaning                   | Range   |
> | -------- | ----- | ------------------------- | ------- |
> | tm_sec   | `int` | seconds after the minute  | `0-60*` |
> | tm_min   | `int` | minutes after the hour    | `0-59`  |
> | tm_hour  | `int` | hours since midnight      | `0-23`  |
> | tm_mday  | `int` | day of the month          | `1-31`  |
> | tm_mon   | `int` | months since January      | `0-11`  |
> | tm_year  | `int` | years since 1900          | ``      |
> | tm_wday  | `int` | days since Sunday         | `0-6`   |
> | tm_yday  | `int` | days since January 1      | `0-365` |
> | tm_isdst | `int` | Daylight Saving Time flag | ``      |
>
> The Daylight Saving Time flag(tm_isdst) is greater than zero if Daylight Saving Time is in effect, zero if Daylight Saving Time is not in effect, and less than zero if the information is not available.
>
> Data Type: **struct timeval**
>
> The `struct timeval` structure represents an elapsed time. It is declared in sys/time.h and has the following members:
>
> - `time_t tv_sec`
>
>   This represents the number of whole seconds of elapsed time.
>
> - `long int tv_usec`
>
>   This is the rest of the elapsed time (a fraction of a second), represented as the number of microseconds. It is always less than one million.
>
> Data Type: **struct timezone**
>
> ```c
> struct timezone {
>   int tz_minuteswest;     /* minutes west of Greenwich */
>   int tz_dsttime;         /* type of DST correction */
>  };
> ```
>
> **gettimeofday(), settimeofday()**
>
> The functions gettimeofday() and settimeofday() can get and set the time as well as a timezone.
>
> 函数原型：
>
> ```c
> #include <sys/time.h>
> 
> int gettimeofday(struct timeval *tv, struct timezone *tz);
> 
> int settimeofday(const struct timeval *tv, const struct timezone *tz);
> ```
>
> **localtime_r**
>
> `struct tm *localtime_r(const time_t *timep, struct tm *result);`
>
> `struct tm *localtime(const time_t *timep);`
>
> The **localtime**() function converts the calendar time *timep* to broken-down time representation, expressed relative to the user's specified timezone. The function acts as if it called **tzset**(3) and sets the external variables *tzname* with information about the current timezone, *timezone* with the difference between Coordinated Universal Time (UTC) and local standard time in seconds, and *daylight* to a nonzero value if daylight savings time rules apply during some part of the year. The return value points to a statically allocated struct which might be overwritten by subsequent calls to any of the date and time functions. The **localtime_r**() function does the same, but stores the data in a user-supplied struct. It need not set *tzname*, *timezone*, and *daylight*.

信息采集：

包括系统时间、格式化后保存到userInfo数组中。

```c
// create user information
    va_start(ap, format);
    vsnprintf(userInfo, PD_LOG_STRINGMAX, format, ap);
    va_end(ap);
snprintf(sysInfo, PD_LOG_STRINGMAX, PD_LOG_HEADER_FORMAT,
            otm.tm_year+1900,
            otm.tm_mon+1,
            otm.tm_mday,
            otm.tm_hour,
            otm.tm_min,
            otm.tm_sec,
            tv.tv_usec,
            PDLEVELSTRING[level],
            getpid(),
            syscall(SYS_gettid),
            func,
            line,
            file,
            userInfo
    );
```



> **snprintf**
>
> ```
> int snprintf ( char * s, size_t n, const char * format, ... );
> ```
>
> Write formatted output to sized buffer
>
> Composes a string with the same text that would be printed if *format* was used on [printf](http://www.cplusplus.com/printf), but instead of being printed, the content is stored as a *C string* in the buffer pointed by *s* (taking *n* as the maximum buffer capacity to fill).
>
> If the resulting string would be longer than *n-1* characters, the remaining characters are discarded and not stored, but counted for the value returned by the function.
>
> A terminating null character is automatically appended after the content written.
>
> After the *format* parameter, the function expects at least as many additional arguments as needed for *format*.
>
> Parameters
>
> - s
>
>   Pointer to a buffer where the resulting C-string is stored. The buffer should have a size of at least *n* characters.
>
> - n
>
>   Maximum number of bytes to be used in the buffer. The generated string has a length of at most `n-1`, leaving space for the additional terminating null character. [size_t](http://www.cplusplus.com/size_t) is an unsigned integral type.
>
> - format
>
>   C string that contains a format string that follows the same specifications as *format* in [printf](http://www.cplusplus.com/printf) (see [printf](http://www.cplusplus.com/printf) for details).
>
> - ... *(additional arguments)*
>
>   Depending on the *format* string, the function may expect a sequence of additional arguments, each containing a value to be used to replace a *format specifier* in the *format* string (or a pointer to a storage location, for `n`). There should be at least as many of these arguments as the number of values specified in the *format specifiers*. Additional arguments are ignored by the function. 
>
> Return Value
>
> The number of characters that would have been written if n had been sufficiently large, not counting the terminating null character. If an encoding error occurs, a negative number is returned.Notice that only when this returned value is non-negative and less than n, the string has been completely written.
>
> **getpid**, **getppid** 
>
> -- get parent or calling process identification
>
> SYNOPSIS
>
> include <unistd.h>
>
>      pid_t
>      getpid(void);
>    
>      pid_t
>      getppid(void);
>
> DESCRIPTION
>      getpid() returns the process ID of the calling process.  The ID is guaranteed to be unique and is useful for constructing temporary file names.
> 	 getppid() returns the process ID of the parent of the calling process.
>
> **gettid**
>
> gettid - get thread identification
>
> SYNOPSIS
>
> ```
> `**#include <sys/types.h>**   **pid_t gettid(void);**  `
> ```
>
> DESCRIPTION
>
> gettid
>
> () returns the thread ID of the current process. This is equal to the process ID (as returned by 
>
> getpid
>
> (2)), unless the process is part of a thread group (created by specifying the CLONE_THREAD flag to the 
>
> clone
>
> (2) system call). All processes in the same thread group have the same PID, but each one has a unique TID.
>
> RETURN VALUE
>
> On success, returns the thread ID of the current process. 
>
> notes:
>
> This is a silly glibc bug. Work around it like this:
>
> ```cpp
> #include <unistd.h>
> #include <sys/syscall.h>
> #define gettid() syscall(SYS_gettid)
> ```
>
> Glibc does not provide a wrapper for this system call; call it using syscall(2).
>
> The thread ID returned by this call is not the same thing as a POSIX thread ID (i.e., the opaque value returned by pthread_self(3)).

### 日志文件写入

函数原型：

static int _pdLogFileWrite(const char **pData*)`

传入参数为字符串指针 pData

```c
int rc = EDB_OK;
size_t dataSize = strlen(pData);
_pdLogMutex.get();
if(!_pdLogFile.isValid())
    {
        // open the file
        rc = _pdLogFileReopen();
        if(rc)
        {
            printf("Failed to open log file, error = %d" OSS_NEWLINE, rc);
            goto error;
        }
	}
// write to the file
    rc = _pdLogFile.Write(pData, dataSize);
    if(rc) 
    {
        printf("Failed to write into log file, error = %d" OSS_NEWLINE, rc);
        goto error;
    }

done:
    _pdLogMutex.release();
    return rc;
error:
    goto done;
```

1. 获取互斥锁，保证文件写入安全
2. 保证日志文件处于有效状态
3. 调用文件操作Write()函数写入日志字符串
4. 写入日志成功则释放锁，否则也释放锁

## 读取命令行或配置文件

### 命令定义

```c
#define PMD_OPTION_HELP             "help"
#define PMD_OPTION_DBPATH           "dbpath"
#define PMD_OPTION_SVCNAME          "svcname"
#define PMD_OPTION_MAXPOOL          "maxpool"
#define PMD_OPTION_LOGPATH          "logpath"
#define PMD_OPTION_CONFPATH         "confpath"

#define PMD_ADD_PARAM_OPTIONS_BEGIN(desc) \
    desc.add_options()

#define PMD_ADD_PRARM_OPTIONS_END ;

#define PMD_COMMANDS_STRING(a,b)    (string(a) + string(b)).c_str()

#define PMD_COMMANDS_OPTIONS \
    (PMD_COMMANDS_STRING(PMD_OPTION_HELP, ",h"), "help") \
    (PMD_COMMANDS_STRING(PMD_OPTION_DBPATH, ",d"), boost::program_options::value<string>(), "database file full path") \
    (PMD_COMMANDS_STRING(PMD_OPTION_SVCNAME, ",s"), boost::program_options::value<string>(), "local service name")  \
    (PMD_COMMANDS_STRING(PMD_OPTION_MAXPOOL, ",m"), boost::program_options::value<string>(), "max pooled agent")  \
    (PMD_COMMANDS_STRING(PMD_OPTION_LOGPATH, ",l"), boost::program_options::value<string>(), "diagnostic log file full path")   \
    (PMD_COMMANDS_STRING(PMD_OPTION_CONFPATH, ",c"), boost::program_options::value<string>(), "configuration file full path")  \

```

在全局宏定义命令名称，方便修改。

应用配置：

```c++
		po::options_description all("Command options");
    po::variables_map vm; // from command line
    po::variables_map vm2; // form configure file

    PMD_ADD_PARAM_OPTIONS_BEGIN(all)
        PMD_COMMANDS_OPTIONS
    PMD_ADD_PRARM_OPTIONS_END
    rc = readCmd(argc, argv, all, vm);
		
		rc = importVM(vm2);
		rc = importVM(vm);
```

这里先应用配置文件参数，后应用命令行参数，表示命令行设置优先。

### 读取命令行函数

函数原型：

```c
int pmdOptions::readCmd(int argc, char **argv,
                    po::options_description &desc,
                    po::variables_map &vm)
```

参数说明：

1. argc, 参数个数
2. char **argv, 参数列表
3. po::options_description &desc, 参数描述信息
4. po::variables_map &vm, 参数选项存储

### 读取配置文件参数

函数原型：

```c
int pmdOptions::readConfigureFile(const char *path,
                            po::options_description &desc,
                            po::variables_map &vm)
```

参数说明：

1. const char \*path, 配置文件路径 . notes：const char\* path, pat为指向字符串的指针，指针指向的内容不可变。

```c
// conf -> path
try
    {
        po::store(po::parse_config_file<char>(conf, desc, true), vm);
        po::notify(vm);
    }
    catch(po::reading_file)
    {
        std::cerr << "Failed to open config file: "
                    << (std::string) conf << std::endl
                    << "Using default settings"<<std::endl;
        rc = EDB_IO;
        goto error;
    }
    catch(po::unknown_option &e)
    {
        std::cerr << "Unkown config element: "
                    << e.get_option_name() << std::endl;
        rc = EDB_INVALIDARG;
        goto error;
    }
    catch(po::invalid_option_value &e)
    {
        std::cerr << (std::string) "Invalid config element: "
                    << e.get_option_name() << std::endl;
        rc = EDB_INVALIDARG;
        goto error;
    }
```



> parse_config_file 函数说明：
>
> Parse a config file.
>
> Read from given stream.
>
> ```c
> basic_parsed_options<charT>
> parse_config_file(const char* filename, const options_description&,
>                bool allow_unregistered = false);
> ```
>
> 

### 解析命令参数

```c
try
    {
        po::store(po::command_line_parser(argc, argv).options(
            desc).allow_unregistered().run(), vm);
        po::notify(vm);
    }
    catch(po::unknown_option &e)
    {
        std::cerr << "Unknown arguments: "
                    << e.get_option_name() << std::endl;
        rc = EDB_INVALIDARG;
        goto error;
    }
    catch(po::invalid_option_value &e)
    {
        std::cerr << "Invalid arguments: "
                    << e.get_option_name() << std::endl;
        rc = EDB_INVALIDARG;
        goto error; 
    }
    catch(po::error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        rc = EDB_INVALIDARG;
        goto error;
    }
```

notes:

Specifies that unregistered options are allowed and should be passed though. For each command like token that looks like an option but does not contain a recognized name, an instance of basic_option<charT> will be added to result, with 'unrecognized' field set to 'true'. It's possible to collect all unrecognized options with the 'collect_unrecognized' funciton.

### 存储命令参数到全局变量

函数原型：

`int pmdOptions::importVM(const po::variables_map &vm, bool isDefault)`

参数说明：

1. vm，variables_map类型变量，存储store()函数解析后的参数。
2. isDefault，是否取默认值

```c
const char *p = NULL; // p的地址为const
// conf path
    if(vm.count(PMD_OPTION_CONFPATH))
    {
        p = vm[PMD_OPTION_CONFPATH].as<string>().c_str();
        strncpy(_confPath, p,OSS_MAX_PATHSIZE);
    }
    else if(isDefault)
    {
        strcpy(_confPath, "./" CONFFILENAME);
    }
```

### boost::program_options 用法详解

> **boost::program_options 用法详解**
> **简介**
> program options是一系列pair<name,value>组成的选项列表,它允许程序通过命令行或配置文件来读取这些参数选项.
>
> **主要组件**
> program_options的使用主要通过下面三个组件完成：
>
> | 组件名                          | 作用                         |
> | ------------------------------- | ---------------------------- |
> | options_description(选项描述器) | 描述当前的程序定义了哪些选项 |
> | parse_command_line(选项分析器)  | 解析由命令行输入的参数       |
> | variables_map(选项存储器)       | 容器,用于存储解析后的选项    |
>
> **代码流程**
>
> 1. 构造option_description对象和variables_map对象
> 2. add_options()->向option_description对象添加选项
> 3. parse_command_line()->将命令行输入的参数解析出来
> 4. store()->将解析出的选项存储至variables_map中
> 5. notify()->通知variables_map去更新所有的外部变量
> 6. count()->检测某个选项是否被输入
> 7. operator[]->取出选项的值
>
> **示例一**
> 下面的代码是boost::program_options的一个简单的用法示例.
> 该示例中指定了两个选项,分别是–-help和–-filename.
>
> //linux系统下,编译选项需加上 -lboost_program_options
>
> ```c++
> # include <iostream>
> 
> # include <string>
> 
> # include <boost/program_options.hpp>
> 
> namespace  bpo = boost::program_options;
> 
> int main(int argc, char const *argv[])
> {
>     //步骤一: 构造选项描述器和选项存储器
>     //选项描述器,其参数为该描述器的名字
>     bpo::options_description opts("all options"); 
>     //选项存储器,继承自map容器
>     bpo::variables_map vm;
>     //步骤二: 为选项描述器增加选项
>     //其参数依次为: key, value的类型，该选项的描述
>     opts.add_options()  
>     ("filename", bpo::value<std::string>(), "the file name which want to be found")
>     ("help", "this is a program to find a specified file");
> 
>     //步骤三: 先对命令行输入的参数做解析,而后将其存入选项存储器
>     //如果输入了未定义的选项，程序会抛出异常，所以对解析代码要用try-catch块包围
>     try{
>         //parse_command_line()对输入的选项做解析
>         //store()将解析后的结果存入选项存储器
>         bpo::store(bpo::parse_command_line(argc, argv, opts), vm);
>     }
>     catch(...){
>         std::cout << "输入的参数中存在未定义的选项！\n";
>         return 0;
>     }
> 
>     //步骤四: 参数解析完毕，处理实际信息
>     //count()检测该选项是否被输入
>     if(vm.count("help") ){//若参数中有help选项
>         //options_description对象支持流输出， 会自动打印所有的选项信息
>         std::cout << opts << std::endl;   
>     }
>     if(vm.count("filename") ){
>         //variables_map(选项存储器)是std::map的派生类,可以像关联容器一样使用,
>         //通过operator[]来取出其中的元素.但其内部的元素类型value_type是boost::any,
>         //用来存储不确定类型的参数值,必须通过模板成员函数as<type>()做类型转换后,
>         //才能获取其具体值.
>         std::cout << "find " << vm["filename"].as<std::string>() << std::endl;
>     }
>     if(vm.empty() ){
>         std::cout << "no options found \n";
>     }
>     return 0;
> }
> ```
> 
>
> 在编译后(`g++ -I../boost -L../boost/stage/lib ./test.cpp -o test -lboost_program_options`)
> 输入
>
> ./test --help
>
> 则其输出为:
>
> all options:
>   --filename arg        the file name which want to be found
>   --help                this is a program to find a specified file
>
> 输入
>
> ./test --filename sd
>
> 则其输出为
>
> find sd
>
> 若不指定选项,即输入
>
> ./test
>
> 则输出为
>
> no options found
>
> 示例二
> 下面的这个示例主要用来说明外部变量，参数默认值以及一个选项对应多个值的情况
> 这段代码主要有四个选项：
>
> –-apple ： 苹果的数量
> –-orange：橘子的数量
> –-address：水果的生产地，可指定多个生产地
> –-help： 打印帮助信息
>
> ///////////////////////////////////////////
> //计算橘子和苹果的总数量，可以指定多个生产地    //
> //编译选项加上 -lboost_program_options     //
> ///////////////////////////////////////////
>
> ```c++
> # include <iostream>  
> 
> # include <vector>  
> 
> # include <string>  
> 
> # include <boost/program_options.hpp>  
> 
> namespace bpo = boost::program_options;  
> 
> int main(int argc, char const *argv[])  
> {  
>     //外部变量，用于保存获取的参数值  
>     int apple_num = 0, orange_num = 0;  
>     std::vector<std::string> addr;  
>     bpo::options_description opt("all options");  
>     opt.add_options()  
>     //指定该参数的默认值 
>     // "apple,a" : 指定选项的全写形式为 --apple, 简写形式为 -a
>     //value<type>(ptr) : ptr为该选项对应的外部变量的地址, 当该选项被解析后, 
>     //可通过下面的notify()函数将选项的值赋给该外部变量,该变量的值会自动更新
>     //defaut_value(num) : num为该选项的默认值, 若命令行中未输入该选项, 则该选项的值取为num
>     ("apple,a", bpo::value<int>(&apple_num)->default_value(10), "苹果的数量")  
>     ("orange,o", bpo::value<int>(&orange_num)->default_value(20), "橘子的数量")  
>     //该参数的实际类型为vector,所以命令行中输入的值可以是多个,
>     //multitoken()的作用就是告诉编译器,该选项可接受多个值  
>     ("address", bpo::value<std::vector<std::string> >()->multitoken(), "生产地")  
>     ("help", "计算苹果和橘子的总数量");  
> 
>     bpo::variables_map vm;  
> 
>     try{  
>         bpo::store(parse_command_line(argc, argv, opt), vm);  
>     }  
>     catch(...){  
>         std::cout << "输入的参数中存在未定义的选项！\n";  
>         return 0;  
>     }  
>     //参数解析完成后，通知variables_map去更新所有的外部变量
>     //这句话执行后, 会使得apple_num和orange_num的值自动更新为选项指定的值   
>     bpo::notify(vm);  
> 
>     if(vm.count("help") ){  
>         std::cout << opt << std::endl;  
>         return 0;  
>     }  
>     if(vm.count("address") ){  
>         std::cout << "生产地为：";  
>         //遍历选项值  
>         for(auto& str : vm["address"].as<std::vector<std::string> >() )  
>             std::cout << str << " ";  
>         std::cout << std::endl;   
>     }  
>     std::cout << "苹果的数量:" << apple_num << std::endl;  
>     std::cout << "橘子的数量:" << orange_num << std::endl;  
>     std::cout << "总数量数量:" << orange_num + apple_num << std::endl;  
>     return 0;  
> }
> ```
>
> 输入
>
> ./a.out –-help
>
> 输出
>
> all options:
> -a [ –apple ] arg (=10) 苹果的数量
> -o [ –orange ] arg (=20) 橘子的数量
> –address arg 生产地
> –help 计算苹果和橘子的总数量
>
> 指定苹果和橘子的数量:
>
> ./a.out –-apple=8 –-orange=20
>
> 其输出为:
>
> 苹果的数量:8
> 橘子的数量:20
> 总数量数量:28
>
> 仅指定橘子的数量,不指定苹果的数量:
>
> ./a.out –-orange=20
>
> 其输出为:
>
> 苹果的数量:10
> 橘子的数量:20
> 总数量数量:30
>
> 可以看到,由于没有输入苹果的数量,所以输出的苹果的数量为我们指定的默认值
>
> 指定一个生产地:
>
> ./a.out –-apple=8 –-orange=20 –-address=山东
>
> 输出:
>
> 生产地为：山东
> 苹果的数量:8
> 橘子的数量:20
> 总数量数量:28
>
> 指定多个生产地:
>
> ./a.out –-apple=8 –-orange=20 –-address=山东 陕西
>
> 输出为
>
> 生产地为：山东 陕西
> 苹果的数量:8
> 橘子的数量:20
> 总数量数量:28
>
> 简写形式的输入:
>
> ./a.out -a 8 -o 20 –-address=山东
>
> 输出:
>
> 生产地为：山东
> 苹果的数量:8
> 橘子的数量:20
> 总数量数量:28
>



