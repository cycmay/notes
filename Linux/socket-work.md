# linux 下 socket 通信机制

在Linux中一切都是文件，socket 也被认为是文件的一种，两台计算机之间的通信，实际上就是两个socket文件的相互读写。

## 创建socket

使用头文件<sys/socket.h> 的socket()函数来创建套接字：

函数原型：`int socket(int af, int type, int protocol);`

1.  af 地址符 Address Family，即IP地址类型，常用：AF_INET, AF_INET6. （INET 是Internet的缩写）AF_INET: IPv4地址，AF_INET6: IPv6地址。
2.  type 数据传输方式，常用：SOCK_STREAM, SOCK_DGRAM
3.  protocol 传输协议， 常用：IPPROTO_TCP, IPPROTO_UDP. 分别代表TCP传输协议和UDP传输协议。使用0表示系统自动推演协议。
4.  返回值为int 表示**文件描述符**， File Descriptor，Linux系统中创建的文件都有一个int型的编号。

示例：
`int tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // TCP套接字`
`int udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // UDP套接字`

## 绑定端口与建立连接

bind() 函数将套接字与特定额IP地址和端口绑定起来，这样流经该IP地址对应端口的数据才会交给套接字处理

connect() 函数建立客户端的连接

bind函数原型：

`int bind(int sock, struct sockaddr *addr, socklen_t addrlen);	//Linux bind()`

1.  sock socket文件描述符
2.  addr sockaddr结构体变量的指针
3.  addrlen addr变量大小 可有sizeof() 计算得出

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



struct sockaddr_in :

```c
struct sockaddr_in
{
	sa_family_t		sin_family;	// 地址族 即地址类型 AF address family
	uint16_t			sin_port;		// 16位的端口号
  struct in_addr sin_addr;	// 32位IP地址
  char 					sin_zero[8];	//不使用
}
```

1.  sin_port 端口号，uint16_t 长度为两字节，理论上端口号取值范围0~65536，0~1023一般有操作系统占用分配给特定的服务程序。端口号要用htons()转换

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

## 监听状态

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

## 接收发送数据

套接字文件和普通文件一样，在c语言中，可以直接通过write()写入，read()读取。

服务端在accept()生成的套接字中写入数据，客户端就能读取到，这样就是完成了通信。

wirte() 函数原型:

`ssize_t write(int fd, const void *buf, size_t nbytes);`

1.  fd 写入的socket文件描述符
2.  buf 要写入数据的缓冲区地址
3.  nbytes 要写入数据的字节数
4.  size_t 是通过 typedef 声明的 unsigned int 类型；ssize_t 在 "size_t" 前面加了一个"s"，代表 signed，即 ssize_t 是通过 typedef 声明的 signed int 类型。
5.  返回值 write() 函数会将缓冲区 buf 中的 nbytes 个字节写入文件 fd，成功则返回写入的字节数，失败则返回 -1。

read() 函数原型：

`ssize_t read(int fd, void *buf, size_t nbytes);`

1.  buf 为要接收数据的缓冲区地址
2.  nbytes 为要读取的数据的字节数
3.  其他参数参照write() 函数
4.  返回值 read() 函数会从 fd 文件中读取 nbytes 个字节并保存到缓冲区 buf，成功则返回读取到的字节数（但遇到文件结尾则返回0），失败则返回 -1。

发送与接收

`int send(int sock, const char *buf, int len, int flags);`

1.  flags 为发送数据时的选项 最后的 flags 参数一般设置为 0 或 NULL

`int recv(int sock, char *buf, int len, int flags);`

## C语言简单实现socket通信

分为服务器端和客户端，服务器端监听端口发来的请求，收到后向客户端发送一个Hello World,客户机负责发送消息并打印收到的Hello World.

服务器步骤：建立socket,绑定socket和地址信息，开启监听，收到请求后发送数据。

客户端步骤：建立socket,连接服务器端，接收并打印服务器给的数据。

（下面代码中所有方法返回-1表示失败）

server:

```c
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
 
#define PORT 1500//端口号 
#define BACKLOG 5/*最大监听数*/ 
 
int main(){
	int sockfd,new_fd;/*socket句柄和建立连接后的句柄*/
	struct sockaddr_in my_addr;/*本方地址信息结构体，下面有具体的属性赋值*/
	struct sockaddr_in their_addr;/*对方地址信息*/
	int sin_size;
 
	sockfd=socket(AF_INET,SOCK_STREAM,0);//建立socket 
	if(sockfd==-1){
		printf("socket failed:%d",errno);
		return -1;
	}
	my_addr.sin_family=AF_INET;/*该属性表示接收本机或其他机器传输*/
	my_addr.sin_port=htons(PORT);/*端口号*/
	my_addr.sin_addr.s_addr=htonl(INADDR_ANY);/*IP，括号内容表示本机IP*/
	bzero(&(my_addr.sin_zero),8);/*将其他属性置0*/
	if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr))<0){//绑定地址结构体和socket
		printf("bind error");
		return -1;
	}
    listen(sockfd,BACKLOG);//开启监听 ，第二个参数是最大监听数 
    while(1){
        sin_size=sizeof(struct sockaddr_in);
        new_fd=accept(sockfd,(struct sockaddr*)&their_addr,&sin_size);//在这里阻塞知道接收到消息，参数分别是socket句柄，接收到的地址信息以及大小 
        if(new_fd==-1){
            printf("receive failed");
        } else{
            printf("receive success");
            send(new_fd,"Hello World!",12,0);//发送内容，参数分别是连接句柄，内容，大小，其他信息（设为0即可） 
        }
    }
    return 0;
} 
```

client:

```c
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
 
 
#define DEST_PORT 1500//目标地址端口号 
#define DEST_IP "127.0.0.1"/*目标地址IP，这里设为本机*/ 
#define MAX_DATA 100//接收到的数据最大程度 
 
int main(){
	int sockfd,new_fd;/*cocket句柄和接受到连接后的句柄 */
	struct sockaddr_in dest_addr;/*目标地址信息*/
	char buf[MAX_DATA];//储存接收数据 
 
	sockfd=socket(AF_INET,SOCK_STREAM,0);/*建立socket*/
	if(sockfd==-1){
		printf("socket failed:%d",errno);
	}
 
 
	//参数意义见上面服务器端 
	dest_addr.sin_family=AF_INET;
 	dest_addr.sin_port=htons(DEST_PORT);
	dest_addr.sin_addr.s_addr=inet_addr(DEST_IP);
	bzero(&(dest_addr.sin_zero),8);
	
	if(connect(sockfd,(struct sockaddr*)&dest_addr,sizeof(struct sockaddr))==-1){//连接方法，传入句柄，目标地址和大小 
		printf("connect failed:%d",errno);//失败时可以打印errno 
	} else{
		printf("connect success");
		recv(sockfd,buf,MAX_DATA,0);//将接收数据打入buf，参数分别是句柄，储存处，最大长度，其他信息（设为0即可）。 
		printf("Received:%s",buf);
	}
	close(sockfd);//关闭socket 
	return 0;
} 
```

# 几个问题总结

## SO_REUSEADDR 设置选项

1、一般来说，一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用。

​    SO_REUSEADDR用于对TCP套接字处于TIME_WAIT状态下的socket，才可以重复绑定使用。server程序总是应该在调用bind()之前设置SO_REUSEADDR套接字选项。TCP，先调用close()的一方会进入TIME_WAIT状态

2、SO_REUSEADDR和SO_REUSEPORT

**SO_REUSEADDR提供如下四个功能：**

​    SO_REUSEADDR允许启动一个监听服务器并捆绑其众所周知端口，即使以前建立的将此端口用做他们的本地端口的连接仍存在。这通常是重启监听服务器时出现，若不设置此选项，则bind时将出错。

​    SO_REUSEADDR允许在同一端口上启动同一服务器的多个实例，只要每个实例捆绑一个不同的本地IP地址即可。对于TCP，我们根本不可能启动捆绑相同IP地址和相同端口号的多个服务器。

​    SO_REUSEADDR允许单个进程捆绑同一端口到多个套接口上，只要每个捆绑指定不同的本地IP地址即可。这一般不用于TCP服务器。

​    SO_REUSEADDR允许完全重复的捆绑：当一个IP地址和端口绑定到某个套接口上时，还允许此IP地址和端口捆绑到另一个套接口上。一般来说，这个特性仅在支持多播的系统上才有，而且只对UDP套接口而言（TCP不支持多播）。

**SO_REUSEPORT选项有如下语义：**

​    此选项允许完全重复捆绑，但仅在想捆绑相同IP地址和端口的套接口都指定了此套接口选项才行。

​    如果被捆绑的IP地址是一个多播地址，则SO_REUSEADDR和SO_REUSEPORT等效。

**使用这两个套接口选项的建议：**

​    在所有TCP服务器中，在调用bind之前设置SO_REUSEADDR套接口选项；

当编写一个同一时刻在同一主机上可运行多次的多播应用程序时，设置SO_REUSEADDR选项，并将本组的多播地址作为本地IP地址捆绑。

​    **if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,**

   **(const void \*)&nOptval , sizeof(int)) < 0) ...**

**附**

​    Q:编写 TCP/SOCK_STREAM 服务程序时，SO_REUSEADDR到底什么意思？

​    A:这个套接字选项通知内核，如果端口忙，但TCP状态位于 TIME_WAIT ，可以重用端口。如果端口忙，而TCP状态位于其他状态，重用端口时依旧得到一个错误信息，指明"地址已经使用中"。如果你的服务程序停止后想立即重启，而新套接字依旧使用同一端口，此时SO_REUSEADDR 选项非常有用。必须意识到，此时任何非期望数据到达，都可能导致服务程序反应混乱，不过这只是一种可能，事实上很不可能。

​    一个套接字由相关五元组构成，协议、本地地址、本地端口、远程地址、远程端口。SO_REUSEADDR 仅仅表示可以重用本地本地地址、本地端口，整个相关五元组还是唯一确定的。所以，重启后的服务程序有可能收到非期望数据。必须慎重使用SO_REUSEADDR 选项。

------



## Select()函数

select()函数用来检查套接字描述符(sockets descriptors)是否已准备好读/写，提供了一种同时检查多个套接字的方法。

所需头文件：
sys/time.h

定义：
int select(int fd_max, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
fd_max：传入的监视文件描述符集合中最大的文件描述符数值 + 1，因为select是从0开始一直遍历到数值最大的标识符。
*readfds：文件描述符集合，检查该组文件描述符的可读性。
*writefds：文件描述符集合，检查该组文件描述符的可写性。
*exceptfds：文件描述符集合，检查该组文件描述符的异常条件。
*timeout： 时间结构体

```c
struct timeval {
    time_t tv_sec;     /* 秒 */
    time_t tv_usec;    /* 微秒 */
};
```

*timeout的值为NULL，则将select()函数置为阻塞状态，当监视的文件描述符集合中的某一个描述符发生变化才会返回结果并向下执行。
*timeout的值等于0，则将select()函数置为非阻塞状态，执行select()后立即返回，无论文件描述符是否发生变化。
*timeout的值大于0，则将select()函数的超时时间设为这个值，在超时时间内阻塞，超时后返回结果。
文件描述符集合设置：

```c
FD_ZERO(fd_set *fd);             /* 清空该组文件描述符集合 */
FD_CLR(int fd,fd_set *fd);       /* 清除该组文件描述符集合中的指定文件描述符 */
FD_ISSET(int fd,fd_set *fd);     /* 测试指定的文件描述符是否在该文件描述符集合中 */
FD_SET(int fd,fd_set *fd);       /* 向该文件描述符集合中添加文件描述符 */
```

返回值：

-1：发生错误，并将所有描述符集合清0，可通过errno输出错误详情。
0：超时。
正数：表示集合中文件描述符的数量，集合也会被修改以显示哪一个文件描述符已准备就绪。
注意：
每次调用完select()函数后需要将文件描述符集合清空并重新设置，也就是设置的文件描述符集合是一次性使用的。原因是调用完select()后文件描述符集合可能发生改变。

Select在Socket编程中还是比较重要的，可是对于初学Socket的人来说都不太爱用Select写程序，他们只是习惯写诸如connect、accept、recv或recvfrom这样的阻塞程序（所谓阻塞方式block，顾名思义，就是进程或是线程执行到这些函数时必须等待某个事件的发生，如果事件没有发生，进程或线程就被阻塞，函数不能立即返回）。可是使用Select就可以完成非阻塞（所谓非阻塞方式non-block，就是进程或线程执行此函数时不必非要等待事件的发生，一旦执行肯定返回，以返回值的不同来反映函数的执行情况，如果事件发生则与阻塞方式相同，若事件没有发生则返回一个代码来告知事件未发生，而进程或线程继续执行，所以效率较高）方式工作的程序，它能够监视我们需要监视的文件描述符的变化情况——读写或是异常。

struct fd_set可以理解为一个集合，这个集合中存放的是文件描述符(filedescriptor)，即文件句柄，这可以是我们所说的普通意义的文件，当然Unix下任何设备、管道、FIFO等都是文件形式，全部包括在内，所以毫无疑问一个socket就是一个文件，socket句柄就是一个文件描述符。fd\_set集合可以通过一些宏由人为来操作，比如清空集合FD\_ZERO(fd\_set)，将一个给定的文件描述符加入集合之中FD\_SET(int ,fd\_set)，将一个给定的文件描述符从集合中删除FD\_CLR(int,fd\_set)，检查集合中指定的文件描述符是否可以读写FD\_ISSET(int ,fd\_set )。

具体解释select的参数：
int maxfdp是一个整数值，是指集合中所有文件描述符的范围，即所有文件描述符的最大值加1，不能错！在Windows中这个参数的值无所谓，可以设置不正确。

fd_set * readfds是指向fd_set结构的指针，这个集合中应该包括文件描述符，我们是要监视这些文件描述符的读变化的，即我们关心是否可以从这些文件中读取数据了，如果这个集合中有一个文件可读，select就会返回一个大于0的值，表示有文件可读，如果没有可读的文件，则根据timeout参数再判断是否超时，若超出timeout的时间，select返回0，若发生错误返回负值。可以传入NULL值，表示不关心任何文件的读变化。

fd_set * writefds是指向fd_set结构的指针，这个集合中应该包括文件描述符，我们是要监视这些文件描述符的写变化的，即我们关心是否可以向这些文件中写入数据了，如果这个集合中有一个文件可写，select就会返回一个大于0的值，表示有文件可写，如果没有可写的文件，则根据timeout参数再判断是否超时，若超出timeout的时间，select返回0，若发生错误返回负值。可以传入NULL值，表示不关心任何文件的写变化。

fd_set * errorfds同上面两个参数的意图，用来监视文件错误异常。

struct timeval * timeout是select的超时时间，这个参数至关重要，它可以使select处于三种状态，第一，若将NULL以形参传入，即不传入时间结构，就是将select置于阻塞状态，一定等到监视文件描述符集合中某个文件描述符发生变化为止；第二，若将时间值设为0秒0毫秒，就变成一个纯粹的非阻塞函数，不管文件描述符是否有变化，都立刻返回继续执行，文件无变化返回0，有变化返回一个正值；第三，timeout的值大于0，这就是等待的超时时间，即select在timeout时间内阻塞，超时时间之内有事件到来就返回了，否则在超时后不管怎样一定返回，返回值同上述。

示例：

```c
#include <sys/time.h>
#include <stdio.h>
#include <fcntl.h>

int main()
{
    int fd_key, ret;
    char value;
    fd_set readfd;
    struct timeval timeout;

    fd_key = open("/dev/tty", O_RDONLY);
    timeout.tv_sec=1;
    timeout.tv_usec=0;

    while(1){
        FD_ZERO(&readfd);                       /* 清空文件描述符集合 */
        FD_SET(fd_key, &readfd);                /* 添加文件描述符到集合 */

        ret = select(fd_key + 1, &readfd, NULL, NULL, &timeout);
        if(FD_ISSET(fd_key, &readfd)){          /* 测试fd_key是否在描述符集合中 */
            read(fd_key, &value, 1);  
            if('\n' == value){
                continue;
            }
            printf("ret = %d, fd_key = %d, value = %c\n", ret, fd_key, value);
        }
    }
}
```

##禁用 Nagle算法

使用setsockopt TCP_NODELAY禁用 Nagle算法

  Nagle算法用于对缓冲区内的一定数量的消息进行自动连接。该处理过程(称为Nagling)，通过减少必须发送的封包的数量，提高了网络应用 程序系统的效率。（Nagle虽然解决了小封包问题，但也导致了较高的不可预测的延迟，同时降低了吞吐量。）

vc下面socket编程，使用阻塞方式的时候，会自动使用Nagle算法，如：当pc不断发送32Bytes的数据的时候，会将这些包合并起来一起发送。如果另一头使用的时候一个tcp包一条命令处理的话，会出问题的。


在vc里，可以屏蔽掉该算法：

```c
const char chOpt=1;   
int   nErr=setsockopt(   m_socket,   IPPROTO_TCP,   TCP_NODELAY,   &chOpt,   sizeof(char));   
if(nErr==-1)   
{   
 TRACE(_T("setsockopt()   error\n"),WSAGetLastError());   
 return ;   
}  
```

## SO_KEEPALIVE选项

SO_KEEPALIVE 保持连接检测对方主机是否崩溃，避免（服务器）永远阻塞于TCP连接的输入。设置该选项后，如果2小时内在此套接口的任一方向都没有数据交换，TCP就自 动给对方 发一个保持存活探测分节(keepalive probe)。这是一个对方必须响应的TCP分节.它会导致以下三种情况：对方接收一切正常：以期望的ACK响应。2小时后，TCP将发出另一个探测分 节。对方已崩溃且已重新启动：以RST响应。套接口的待处理错误被置为ECONNRESET，套接 口本身则被关闭。对方无任何响应：源自berkeley的TCP发送另外8个探测分节，相隔75秒一个，试图得到一个响应。在发出第一个探测分节11分钟 15秒后若仍无响应就放弃。套接口的待处理错误被置为ETIMEOUT，套接口本身则被关闭。如ICMP错误是“host unreachable(主机不可达)”，说明对方主机并没有崩溃，但是不可达，这种情况下待处理错误被置为 EHOSTUNREACH。

根据上面的介绍我们可以知道对端以一种非优雅的方式断开连接的时候，我们可以设置SO_KEEPALIVE属性使得我们在2小时以后发现对方的TCP连接是否依然存在。   

keepAlive = 1;

Setsockopt(listenfd, SOL_SOCKET, SO_KEEPALIVE, (void*)&keepAlive, sizeof(keepAlive));

如果我们不能接受如此之长的等待时间，从TCP-Keepalive-HOWTO上可以知道一共有两种方式可以设置，一种是修改内核关于网络方面的 配置参数，另外一种就是SOL_TCP字段的TCP_KEEPIDLE， TCP_KEEPINTVL， TCP_KEEPCNT三个选项。

# 引用：

参考：

>   ## Name
>
>   htonl, htons, ntohl, ntohs - convert values between host and network byte order
>
>   ## Synopsis
>
>   ```
>   #include <arpa/inet.h>
>   uint32_t htonl(uint32_t hostlong);
>   uint16_t htons(uint16_t hostshort);
>   uint32_t ntohl(uint32_t netlong);
>   uint16_t ntohs(uint16_t netshort);
>   ```
>
>   ## Description
>
>   The **htonl**() function converts the unsigned integer *hostlong* from host byte order to network byte order.
>
>   The **htons**() function converts the unsigned short integer *hostshort* from host byte order to network byte order.
>
>   The **ntohl**() function converts the unsigned integer *netlong* from network byte order to host byte order.
>
>   The **ntohs**() function converts the unsigned short integer *netshort* from network byte order to host byte order.
>
>   On the i386 the host byte order is Least Significant Byte first, whereas the network byte order, as used on the Internet, is Most Significant Byte first.
>
>   ## Conforming to
>
>   POSIX.1-2001.
>
>   Some systems require the inclusion of *<netinet/in.h>* instead of *<arpa/inet.h>*.

>   If the (sin_addr.s_addr) field is set to the constant INADDR_ANY, as defined in netinet/in.h, the caller is requesting that the socket be bound to all network interfaces on the host.

------

>   
>
>   ```c
>   if( (hp=gethostbyname(pHostName)) )
>   		_sockAddress.sin_addr.s_addr = *((int *)hp->h_addr_list[0]);
>   else
>   		_sockAddress.sin_addr.s_addr = inet_addr(pHostName);
>   ```
>
>   gethostbyname(name) 通过传入字符串类型或者说是指针来查询到hostent结构体。或者转化为二进制格式的网络字节类型。s_addr 设置socket的主机地址。

>   ```c
>   #include <netdb.h>
>   struct hostent *gethostbyname(const char *name);
>   
>   The hostent structure is defined in <netdb.h> as follows:
>   
>   struct hostent {
>   char  *h_name;            /* official name of host */
>   char **h_aliases;         /* alias list */
>   int    h_addrtype;        /* host address type */
>   int    h_length;          /* length of address */
>   char **h_addr_list;       /* list of addresses */
>   }
>   #define h_addr h_addr_list[0] /* for backward compatibility */
>   
>   ```
>
>   The gethostbyname() function returns a structure of type hostent for the given host name.  Here name is either a hostname or an IPv4 address in standard dot notation (as for inet_addr(3)).  If name is an IPv4 address, no lookup is performed and gethostbyname() simply copies name into the h_name field and its struct in_addr equivalent  into the h_addr_list[0] field of the returned hostent structure.  If  name doesn't end in a dot and the environment variable HOSTALIASES is  set, the alias file pointed to by HOSTALIASES will first be searched for name (see hostname(7) for the file format).  The current domain and its parents are searched unless name ends in a dot.

>   ```c
>   #include <sys/socket.h>
>   #include <netinet/in.h>
>   #include <arpa/inet.h>
>   
>   in_addr_t inet_addr(const char *cp);
>   ```
>
>   The inet_addr() function converts the Internet host address cp from IPv4 numbers-and-dots notation into binary data in network byte order.  If the input is invalid, INADDR_NONE (usually -1) is returned.  Use of this function is problematic because -1 is a valid address (255.255.255.255).  Avoid its use in favor of inet_aton(),
>   inet_pton(3), or getaddrinfo(3), which provide a cleaner way to indicate error return.

------

>   
>
>   ```c
>   rc = getsockname(_fd, (sockaddr*)&_sockAddress, &_addressLen);
>   rc = getpeername(_fd, (sockaddr *)&_peerAddress, &_peerAddressLen);
>   ```
>
>   get socket name.  get peer name
>
>   _fd 为已知socket的描述符，将新分配内存空间的 _sockAddress 作为引用参数传入，可以将socket的主机地址保存到该内存空间，同理对peerAddress。
>
>   参考：
>
>   ```
>   #include <sys/socket.h>
>   
>   int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
>   ```
>
>   ```
>   getsockname() returns the current address to which the socket sockfd is bound, in the buffer pointed to by addr.  The addrlen argument should be initialized to indicate the amount of space (in bytes) pointed to by addr.  On return it contains the actual size of the socket address. The returned address is truncated if the buffer provided is too small; in this case, addrlen will return a value greater than was supplied to the call.
>   On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.
>   ```
>
>   ```
>   getpeername() returns the address of the peer connected to the socket sockfd, in the buffer pointed to by addr.  The addrlen argument should be initialized to indicate the amount of space pointed to by addr.  On return it contains the actual size of the name returned (in bytes).  The name is truncated if the buffer provided is too small. The returned address is truncated if the buffer provided is too small; in this case, addrlen will return a value greater than was supplied to the call.
>   ```
>
>   ------
>
>   setTimeout
>
>   ```
>   struct timeval tv;
>   tv.tv_sec = seconds;
>   tv.tv_usec = 0;
>   // windows take milliseconds as parameter
>   // linux takes timeval as input
>   rc = setsockopt(_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
>   ```
>
>   参考：Name

>   getsockopt, setsockopt - get and set options on sockets
>
>   ## Synopsis
>
>   ```
>   #include <sys/types.h>          /* See NOTES */
>   #include <sys/socket.h>
>   int getsockopt(int sockfd, int level, int optname,               void *optval,
>   socklen_t *optlen);int setsockopt(int sockfd, int level, int optname,
>             const void *optval, socklen_t optlen);
>   ```
>
>   ## Description
>
>   getsockopt() and setsockopt() manipulate options for the socket referred to by the file descriptor sockfd. Options may exist at multiple protocol levels; they are always present at the uppermost socket level.
>
>   When manipulating socket options, the level at which the option resides and the name of the option must be specified. To manipulate options at the sockets API level, *level* is specified as **SOL_SOCKET**. To manipulate options at any other level the protocol number of the appropriate protocol controlling the option is supplied. For example, to indicate that an option is to be interpreted by the **TCP** protocol, *level* should be set to the protocol number of **TCP**; see ***getprotoent**(3)*.
>
>   The arguments *optval* and *optlen* are used to access option values for **setsockopt**(). For **getsockopt**() they identify a buffer in which the value for the requested **option**(s) are to be returned. For **getsockopt**(), *optlen* is a value-result argument, initially containing the size of the buffer pointed to by *optval*, and modified on return to indicate the actual size of the value returned. If no option value is to be supplied or returned, *optval* may be NULL.
>
>   *Optname* and any specified options are passed uninterpreted to the appropriate protocol module for interpretation. The include file *<sys/socket.h>* contains definitions for socket level options, described below. Options at other protocol levels vary in format and name; consult the appropriate entries in section 4 of the manual.
>
>   Most socket-level options utilize an *int* argument for *optval*. For **setsockopt**(), the argument should be nonzero to enable a boolean option, or zero if the option is to be disabled.
>
>   For a description of the available socket options see ***socket**(7)* and the appropriate protocol man pages.
>
>   ## Return Value
>
>   On success, zero is returned. On error, -1 is returned, and errno is set appropriately.

------

>   
>
>   ```c
>   struct linger _linger;
>   _linger.l_onoff = lOnOff;
>   _linger.l_linger = linger;
>   rc = setsockopt(_fd, SOL_SOCKET, SO_LINGER, (const char *)&_linger, sizeof(_linger));
>   ```
>
>   参考：
>
>   当调用closesocket关闭套接字时，SO_LINGER将决定系统如何处理残存在套接字发送队列中的数据。处理方式无非两种：丢弃或者将数据继续发送至对端，优雅关闭连接。事实上，SO_LINGER并不被推荐使用，大多数情况下我们推荐使用默认的关闭方式（即下方表格中的第一种情况）。
>
>   下方代码段显示linger结构语法，表格为不同参数情况下的套接字行为。
>
>   typedef struct linger { 
>   ![img](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)      u_short l_onoff;    //开关，零或者非零 
>   ![img](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)      u_short l_linger;   //优雅关闭最长时限 
>   ![img](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)    } linger; 
>
>   
>
>   | l_onoff | l_linger | closesocket行为                                              | 发送队列                                       | 底层行为                                                     |
>   | ------- | -------- | ------------------------------------------------------------ | ---------------------------------------------- | ------------------------------------------------------------ |
>   | 零      | 忽略     | 立即返回。                                                   | 保持直至发送完成。                             | 系统接管套接字并保证将数据发送至对端。                       |
>   | 非零    | 零       | 立即返回。                                                   | 立即放弃。                                     | 直接发送RST包，自身立即复位，不用经过2MSL状态。对端收到复位错误号。 |
>   | 非零    | 非零     | 阻塞直到l_linger时间超时或数据发送完成。(套接字必须设置为阻塞zhuan) | 在超时时间段内保持尝试发送，若超时则立即放弃。 | 超时则同第二种情况，若发送完成则皆大欢喜。                   |
>
>   ------
>
>   
>
>   ```
>   struct timeval maxSelectTime;
>   fd_set fds;
>   maxSelectTime.tv_sec = timeout / 1000000;
>   maxSelectTime.tv_usec = timeout % 1000000;
>   ```
>
>   参考：
>
>   The <sys/time.h> header defines the  timeval structure that includes at least the following members:
>
>   ```
>   /*
>    * Structure returned by gettimeofday(2) system call,
>    * and used in other calls.
>    */
>   struct timeval {
>   	long	tv_sec;		/* seconds */
>   	long	tv_usec;	/* and microseconds */
>   };
>   ```
>
>   ------
>
>   ```
>   FD_ZERO(&fds);
>   FD_SET(_fd, &fds);
>   	rc = select(maxFD + 1, NULL, &fds, NULL,
>   							timeout>=0?&maxSelectTime:NULL);
>   ```
>
>   参考：

>   ## Name
>
>   select, pselect, FD_CLR, FD_ISSET, FD_SET, FD_ZERO - synchronous I/O multiplexing
>
>   ## Synopsis
>
>   ```
>   /* According to POSIX.1-2001 */
>   #include <sys/select.h>
>   
>   /* According to earlier standards */
>   #include <sys/time.h>
>   #include <sys/types.h>
>   #include <unistd.h>
>   
>   int select(int nfds, fd_set *readfds, fd_set *writefds,
>           fd_set *exceptfds, struct timeval *timeout);
>   
>   void FD_CLR(int fd, fd_set *set);
>   int  FD_ISSET(int fd, fd_set *set);
>   void FD_SET(int fd, fd_set *set);
>   void FD_ZERO(fd_set *set);
>   
>   #include <sys/select.h>
>   
>   int pselect(int nfds, fd_set *readfds, fd_set *writefds,
>            fd_set *exceptfds, const struct timespec *timeout,
>            const sigset_t *sigmask);
>   ```
>
>   -   **pselect**(): _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
>
>   ## Description
>
>   **select**() and **pselect**() allow a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation (e.g., input possible). A file descriptor is considered ready if it is possible to perform the corresponding I/O operation (e.g., **read**(2)) without blocking.
>
>   The operation of **select**() and **pselect**() is identical, other than these three differences:
>
>   -   (i)**select**() uses a timeout that is a *struct timeval* (with seconds and microseconds), while **pselect**() uses a *struct timespec* (with seconds and nanoseconds).(ii)**select**() may update the *timeout* argument to indicate how much time was left. **pselect**() does not change this argument.(iii)**select**() has no *sigmask* argument, and behaves as **pselect**() called with NULL *sigmask*.
>   -   Three independent sets of file descriptors are watched. Those listed in *readfds* will be watched to see if characters become available for reading (more precisely, to see if a read will not block; in particular, a file descriptor is also ready on end-of-file), those in *writefds* will be watched to see if a write will not block, and those in *exceptfds* will be watched for exceptions. On exit, the sets are modified in place to indicate which file descriptors actually changed status. Each of the three file descriptor sets may be specified as NULL if no file descriptors are to be watched for the corresponding class of events.Four macros are provided to manipulate the sets. **FD_ZERO**() clears a set. **FD_SET**() and **FD_CLR**() respectively add and remove a given file descriptor from a set. **FD_ISSET**() tests to see if a file descriptor is part of the set; this is useful after **select**() returns.*nfds* is the highest-numbered file descriptor in any of the three sets, plus 1.The *timeout* argument specifies the minimum interval that **select**() should block waiting for a file descriptor to become ready. (This interval will be rounded up to the system clock granularity, and kernel scheduling delays mean that the blocking interval may overrun by a small amount.) If both fields of the *timeval* structure are zero, then **select**() returns immediately. (This is useful for polling.) If *timeout* is NULL (no timeout), **select**() can block indefinitely.*sigmask* is a pointer to a signal mask (see **sigprocmask**(2)); if it is not NULL, then **pselect**() first replaces the current signal mask by the one pointed to by *sigmask*, then does the "select" function, and then restores the original signal mask.Other than the difference in the precision of the *timeout* argument, the following **pselect**() call:`ready = pselect(nfds, &readfds, &writefds, &exceptfds,                 timeout, &sigmask);`is equivalent to *atomically* executing the following calls:` sigset_t origmask;  pthread_sigmask(SIG_SETMASK, &sigmask, &origmask);  ready = select(nfds, &readfds, &writefds, &exceptfds, timeout);  pthread_sigmask(SIG_SETMASK, &origmask, NULL);`The reason that **pselect**() is needed is that if one wants to wait for either a signal or for a file descriptor to become ready, then an atomic test is needed to prevent race conditions. (Suppose the signal handler sets a global flag and returns. Then a test of this global flag followed by a call of **select**() could hang indefinitely if the signal arrived just after the test but just before the call. By contrast, **pselect**() allows one to first block signals, handle the signals that have come in, then call **pselect**() with the desired *sigmask*, avoiding the race.)
>
>   **The timeout**
>
>   -   The time structures involved are defined in *<sys/time.h>* and look like
>
>   `struct timeval {     long    tv_sec;         /* seconds */     long    tv_usec;        /* microseconds */ };`
>
>   -   and
>
>   `struct timespec {     long    tv_sec;         /* seconds */     long    tv_nsec;        /* nanoseconds */ };`
>
>   -   (However, see below on the POSIX.1-2001 versions.)Some code calls **select**() with all three sets empty, *nfds* zero, and a non-NULL *timeout* as a fairly portable way to sleep with subsecond precision.On Linux, **select**() modifies *timeout* to reflect the amount of time not slept; most other implementations do not do this. (POSIX.1-2001 permits either behavior.) This causes problems both when Linux code which reads *timeout* is ported to other operating systems, and when code is ported to Linux that reuses a *struct timeval* for multiple **select**()s in a loop without reinitializing it. Consider *timeout* to be undefined after **select**() returns.
>
>   ## Return Value
>
>   On success, **select**() and **pselect**() return the number of file descriptors contained in the three returned descriptor sets (that is, the total number of bits that are set in *readfds*, *writefds*, *exceptfds*) which may be zero if the timeout expires before anything interesting happens. On error, -1 is returned, and *errno* is set appropriately; the sets and *timeout* become undefined, so do not rely on their contents after an error.