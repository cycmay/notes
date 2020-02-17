emeraldDB implements

## enveriments

在项目 emeralddb 目录下运行 build.sh ，emralddb 可执行文件编译连接成功，执行后输出hello world。

环境基本搭建完成 保存快照

```bash
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
```



## Socket通信

创建一个类ossSocket, 该类实现socket通信的几个步骤，包括初始化socket，绑定监听bind/listen，发送接收send/recv等。

### ossSocket类

#### 私有成员变量

1.  int _fd, socket的描述符。一个ossSocket类维持一个sock，它有一个Unix文件描述符提供给用户进行程序操作。
2.  socketlen_t  _addressLen，主机地址长度。
3.  socketlen_t  _peerAddressLen, 客户机地址长度。
4.  struct sockaddr_in _sockAddress，主机地址结构体。struct sockaddr_in 是Unix封装好的IPv4类型地址结构。
5.  struct sockaddr_in _peerAddress, 客户机地址结构体。
6.  bool _init，sock对象是否已经被初始化。对应sock对象初始化函数initSocket()。
7.  int _timeout，超时设置。

#### 构造函数

**ossSocket()，构造一个全新的套接字**

```c
ossSocket::ossSocket()
{
    _init = false;
    _fd = 0;
    _timeout = 0;
    memset(&_sockAddress, 0, sizeof(sockaddr_in));
    memset(&_peerAddress, 0, sizeof(sockaddr_in));
    _peerAddressLen = sizeof(_peerAddress);
    _addressLen = sizeof(_sockAddress);
}
```

将初始化状态\_init设置为false，文件描述符\_fd设置为0，超时\_timeout设置为0，地址结构体\_sockAddress和\_peerAddress内存置0，地址长度\_peerAddressLen和\_addressLen内存置0.

**ossSocket(unsigned int port, int timeout)，构造服务端用于监听的socket**

```c
ossSocket::ossSocket(unsigned int port, int timeout)
{
    _init = false;
    _fd = 0;
    _timeout = timeout;
    memset(&_sockAddress, 0, sizeof(sockaddr_in));
    memset(&_peerAddress, 0, sizeof(sockaddr_in));
    _peerAddressLen = sizeof(_peerAddress);
    _sockAddress.sin_family = AF_INET;
    _sockAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    _sockAddress.sin_port = htons(port);
    _addressLen = sizeof(_sockAddress);
}
```

服务端的socket要初始化本地的socket。

部分参数参考上述。这里的服务端socket设置：

\_sockAddress.sin_family 通信协议族为 AF\_INET(IPV4).

 \_sockAddress.sin\_addr.s\_addr 绑定socket宿主机IP地址。htonl 将主机字节类型转换为网络字节类型 l表示hostlong 主机为长整型,htons s表示短整型。 INADDR_ANY 为表示创建的socket接收任何地址的信息。If the (sin_addr.s_addr) field is set to the constant INADDR_ANY, as defined in netinet/in.h, the caller is requesting that the socket be bound to all network interfaces on the host.

_sockAddress.sin_port = htons(port) 转换int型的port为短整型。

**ossSocket(const char \*pHostName, unsigned int port, int timeout) 构造客户端的套接字**

```c
ossSocket::ossSocket(const char *pHostName, unsigned int port, int timeout)
{
    struct hostent *hp;
    _init = false;
    _timeout = timeout;
    _fd = 0;
    memset(&_sockAddress, 0, sizeof(sockaddr_in));
    memset(&_peerAddress, 0, sizeof(sockaddr_in));
    _peerAddressLen = sizeof(_peerAddress);
    _sockAddress.sin_family = AF_INET;
    // 解析对方的机型
    if( (hp=gethostbyname(pHostName)) )
    {
        _sockAddress.sin_addr.s_addr = *((int *)hp->h_addr_list[0]);
    }else
    {
        _sockAddress.sin_addr.s_addr = inet_addr(pHostName);
    }
    _sockAddress.sin_port = htons(port);
    _addressLen = sizeof(_sockAddress);
}
```

客户端的套接字socket要解析服务端的地址信息，一般传入的是服务端ip地址或者hostname(若host文件保存对方hostname)，所以这里使用struct hostent以及函数gethostbyname()来解析对方的地址；若解析失败，使用inet_addr()解析传进来的点分十进制ip地址。

**ossSocket(int *sock, int timeout)，从一个已经存在的socket创建新的ossSocket对象**

```c
ossSocket::ossSocket(int *sock, int timeout)
{
    int rc = EDB_OK;
    _fd = *sock;
    // 已经存在的socket表示已经初始化过
    _init = true;
    _timeout = timeout;
    _addressLen = sizeof(_sockAddress);
    memset(&_peerAddress, 0 , sizeof(sockaddr_in));
    _peerAddressLen = sizeof(_peerAddress);
    rc = getsockname(_fd, (sockaddr*)&_sockAddress, &_addressLen);
    if  (rc)
    {
        PD_LOG(PDERROR, "Failed to get sock name, error = %d", 
                SOCKET_GETLASTERROR);
        _init = false;
    }else
    {
        rc = getpeername(_fd, (sockaddr *)&_peerAddress, &_peerAddressLen);
        PD_RC_CHECK(rc, PDERROR, "Failed to get peer name, error = %d",
                    SOCKET_GETLASTERROR);
    }
  done:
  	return;
  error:
  	goto done;
}
```

一般来说，服务端的socket在监听状态下，会有accept()函数，以阻塞进程的方式等待接收客户端的请求，接收到请求后返回一个新的socket描述符，用于对socket的信息读写操作。所以，这里封装新的socket描述符。

get socket name.  get peer name，_fd 为已知socket的描述符，将新分配内存空间的 _sockAddress 作为引用参数传入，可以将socket的主机地址保存到该内存空间，同理对peerAddress。

#### 初始化socket

`int initSocket()`，该函数是对socket()函数的封装

```c
int ossSocket::initSocket()
{
    int rc = EDB_OK;
    if(_init)
    {
        goto done;
    }
    memset(&_peerAddress, 0, sizeof(sockaddr_in));
    _peerAddressLen = sizeof(_peerAddress);
    _fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(-1 == _fd)
    {
        PD_RC_CHECK(EDB_NETWORK, PDERROR, "Failed to initialize socket, error = %d",
                SOCKET_GETLASTERROR);
    }
    _init = true;
    // set timeout
    setTimeout(_timeout);

done :
    return rc;
error :
    goto done;
}
```

首先判断是否已经初始化，若已经初始化直接goto done. 使用系统提供的socket()函数，传入协议族，TCP传输协议，并将生成的socket描述符赋值给对象的成员变量_fd. 最后设置超时时间。

#### 设置服务端地址

```c
void ossSocket::setAddress(const char *pHostName, unsigned int port)
{
    struct hostent *hp;
    memset(&_sockAddress, 0, sizeof(sockaddr_in));
    memset(&_peerAddress, 0, sizeof(sockaddr_in));
    _peerAddressLen = sizeof(_peerAddress);
    _sockAddress.sin_family = AF_INET;  // IPV4
    //获取hostent结构体
    if( (hp=gethostbyname(pHostName)) )
        _sockAddress.sin_addr.s_addr = *((int *)hp->h_addr_list[0]);
    else
        _sockAddress.sin_addr.s_addr = inet_addr(pHostName);

    _sockAddress.sin_port = htons(port);
    _addressLen = sizeof(_sockAddress);
}
```

一般该函数使给客户端socket调用，用来绑定请求服务的服务器地址。

函数结构与构造客户端ossSocket对象类似。

#### 发送队列linger

```c
int ossSocket::setSocketLi(int lOnOff, int linger)
{
    int rc = EDB_OK;
    struct linger _linger;
    _linger.l_onoff = lOnOff;
    _linger.l_linger = linger;
    rc = setsockopt(_fd, SOL_SOCKET, SO_LINGER,
                    (const char *)&_linger, sizeof(_linger));
    return rc;
}
```

当调用closesocket关闭套接字时，SO_LINGER将决定系统如何处理残存在套接字发送队列中的数据。处理方式无非两种：丢弃或者将数据继续发送至对端，优雅关闭连接。事实上，SO_LINGER并不被推荐使用，大多数情况下我们推荐使用默认的关闭方式。

#### 绑定监听

bing_listen()，绑定ossSocket对象成员变量\_sockAddress提供的socket地址信息，以及\_addressLen地址长度。

```c
int ossSocket::bind_listen()
{
    int rc = EDB_OK;
    int temp = 1;
    rc = setsockopt(_fd, SOL_SOCKET,
                    SO_REUSEADDR, (char *)&temp, sizeof(int));
    if(rc)
    {
        PD_LOG(PDWARNING, "Failed to setsocktopt SO_REUSEADDR, error = %d",
                SOCKET_GETLASTERROR);
    }
    rc = setSocketLi(1,30);
    if (rc)
    {   
        PD_LOG(PDWARNING, "Failed to setsockopt SO_LINGER, error = %d", 
                SOCKET_GETLASTERROR);
    }
    rc = ::bind(_fd, (struct sockaddr*)&_sockAddress, _addressLen);
    if(rc)
    {
        PD_RC_CHECK(EDB_NETWORK, PDERROR, "Failed to bind socket, error = %d", 
                SOCKET_GETLASTERROR);
    }
    rc = listen(_fd, SOMAXCONN);
    if(rc)
    {   
        PD_RC_CHECK(EDB_NETWORK, PDERROR, "Failed to listen socket, error = %d",
                SOCKET_GETLASTERROR);
    }

done:
    return rc;
error:
    close();
    goto done;
}
```

`setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&temp, sizeof(int));`

编写 TCP/SOCK_STREAM 服务程序时，SO_REUSEADDR 这个套接字选项通知内核，如果端口忙，但TCP状态位于 TIME_WAIT ，可以重用端口。如果端口忙，而TCP状态位于其他状态，重用端口时依旧得到一个错误信息，指明"地址已经使用中"。如果你的服务程序停止后想立即重启，而新套接字依旧使用同一端口，此时SO_REUSEADDR 选项非常有用。其实一般来说不使用。

`setSocketLi(1,30);` 

设置socketLinger，阻塞进程知道队列消息全部处理完，缓存队列的处理等待时间30ms。

`::bind(_fd, (struct sockaddr*)&_sockAddress, _addressLen);`

绑定socket地址信息

`listen(_fd, SOMAXCONN)`

进入被动监听状态，函数原型`int listen(int sock, int backlog);	//linux`。backlog 为请求队列最大长度，被动监听是套接字在没有请求时进入睡眠状态，接收到请求时被唤醒。

#### 接收请求

accept(), 接收请求的函数是客户发送请求过来后，服务端进行响应的函数。期间阻塞进程，直到客户发送请求。

```c
int ossSocket::accept(int *sock, struct sockaddr *addr, socklen_t *addrlen,
                      int timeout)
{
    int rc = EDB_OK;
    int maxFD = _fd;
    struct timeval maxSelectTime;
    fd_set fds;
    maxSelectTime.tv_sec = timeout/1000000;
    maxSelectTime.tv_usec = timeout%1000000;
    while(true)
    {
        FD_ZERO(&fds);
        FD_SET(_fd, &fds);
        rc = select(maxFD+1, &fds, NULL, NULL,
                    timeout>=0?&maxSelectTime:NULL);
        // 0 means timeout
        if(0==rc)
        {
            rc = EDB_TIMEOUT;
            goto done;
        }
        // if < 0 , something wrong
        if(0>rc)
        {
            rc = SOCKET_GETLASTERROR;
            if(EINTR==rc)
            {
                continue;
            }
            PD_RC_CHECK(EDB_NETWORK, PDERROR, 
                        "Failed to select from socket, error = %d", rc);
        }
        if(FD_ISSET(_fd, &fds))
        {
            break;
        }
    }
    rc = EDB_OK;
    *sock = ::accept(_fd, addr, addrlen);
    if(-1 == *sock)
    {
        PD_RC_CHECK(EDB_NETWORK, PDERROR, 
                    "Failed to accept socket, error = %d", SOCKET_GETLASTERROR);
    }
done:
    return rc;
error:
    goto done;
}
```

select()函数用来检查套接字描述符(sockets descriptors)是否已准备好读/写，提供了一种同时检查多个套接字的方法。

定义：
int select(int fd_max, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

当参数中文件描述符集合fds内发生变化时，函数返回变化的描述符个数；-1：发生错误，并将所有描述符集合清0，可通过errno输出错误详情；0：超时。每次调用完select()函数后需要将文件描述符集合清空并重新设置，也就是设置的文件描述符集合是一次性使用的。原因是调用完select()后文件描述符集合可能发生改变。

```c
FD_ZERO(fd_set *fd);             /* 清空该组文件描述符集合 */
FD_CLR(int fd,fd_set *fd);       /* 清除该组文件描述符集合中的指定文件描述符 */
FD_ISSET(int fd,fd_set *fd);     /* 测试指定的文件描述符是否在该文件描述符集合中 */
FD_SET(int fd,fd_set *fd);       /* 向该文件描述符集合中添加文件描述符 */
```

设置select参数READ若检测到文件描述符\_fd发生变化后，集合内保存的文件描述符_fd, 表明文件准备读取就绪，则调用accept()函数来接收客户端请求，即读取并处理数据。accept()函数返回值为新的套接字，之后的两者之间的通信则使用这个新生成的套接字。

#### 客户端连接

connect()函数是客户端连接到服务端的socket调用的。

```c
int ossSocket::connect()
{
    int rc = EDB_OK;
    rc = ::connect(_fd, (struct sockaddr *)&_sockAddress, _addressLen);
    if(rc)
    {
        PD_RC_CHECK(EDB_NETWORK, PDERROR, 
                    "Failed to connect, error = %d", SOCKET_GETLASTERROR);
    }
    rc = getsockname(_fd, (sockaddr*)&_sockAddress, &_addressLen);
    if (rc)
    {
        PD_RC_CHECK(EDB_NETWORK, PDERROR,
                    "Failed to get local address, error = %d", rc);
    }
    // get peer address
    rc = getpeername(_fd, (sockaddr*)&_peerAddress, &_peerAddressLen);
    if(rc)
    {
        PD_RC_CHECK(EDB_NETWORK, PDERROR,
                    "Failed to get peer address, error = %d", rc);
    }

done:
    return rc;
error:
    goto done;
}
```

ossSocket类对connect()函数封装，将连接后的双方socketAddress结构体保存到私有成员变量，\_sockAddress/\_peerAddress.

#### 发送和接收

**send() 发送消息**

```c
int ossSocket::send(const char *pMsg, int len,
                    int timeout, int flags)
{
    int rc = EDB_OK;
    int maxFD = _fd;
    struct timeval maxSelectTime;
    fd_set fds;

    maxSelectTime.tv_sec = timeout / 1000000;
    maxSelectTime.tv_usec = timeout % 1000000;
    // if len==0 then let's just return
    if(0==len)
    {
        return EDB_OK;
    }
    // wait loop until socket is ready
    while(true)
    {
        // 清空
        FD_ZERO(&fds);
        // add
        FD_SET(_fd, &fds);
        rc = select(maxFD + 1, NULL, &fds, NULL,
                    timeout>=0?&maxSelectTime:NULL);
        if(0==rc)
        {
            // timeout
            rc = EDB_TIMEOUT;
            goto done;
        }
        // if rf < 0, someting wrong
        if(0>rc)
        {
            rc = SOCKET_GETLASTERROR;
            // failed due to interrupt, let's continue
            if(EINTR==rc)
            {
                continue;
            }
            PD_RC_CHECK(EDB_NETWORK, PDERROR, "Failed to select from socket, error = %d",
                    SOCKET_GETLASTERROR);
        }
        // FD_ISSET() tests to see if a file descriptor is part of the set; 
        if(FD_ISSET(_fd, &fds))
        {
            break;
        }
    }
    // 发送消息
    while(len>0)
    {
        // MSG_NOSIGNAL: Requests not to send SIGPIPE on errors on stream oriented sockets
        // when the other end breaks the connetction. The EPIPE error is still returned
        rc = ::send(_fd, pMsg, len, MSG_NOSIGNAL|flags);
        if (-1 == rc)
        {
            PD_RC_CHECK(EDB_NETWORK, PDERROR, "Failed to send, error = %d", 
                        SOCKET_GETLASTERROR);
        }
        len -=rc;
        pMsg += rc;
    }
    rc = EDB_OK;

done:
    return rc;
error:
    goto done;
}

```

select()函数设置文件集合可写性，当准备好后进行socket写入操作(send)。

这里对sned函数进行了二次封装，保证发送的健壮性。发送len长度的消息，发送可能截断一部分，send()函数返回完成的字节数，len减去这个字节数，消息指针移动到字节数后一个位置，重新发送，重复上述步骤直到len==0.

**recv()接收函数**

```c
#define MAX_RECV_PETRIES 5
int _ossSocket::recv(char *pMsg, int len,
                    int timeout, int flags)
{
    int rc = EDB_OK;
    int retries = 0;
    int maxFD = _fd;
    struct timeval maxSelectTime;
    fd_set fds;

    if(0==len)
        return EDB_OK;
    maxSelectTime.tv_sec = timeout/1000000;
    maxSelectTime.tv_usec = timeout%1000000;
    while(true)
    {
        FD_ZERO(&fds);
        FD_SET(_fd, &fds);
        rc = select(maxFD+1, &fds, NULL, NULL,
                    timeout>=0?&maxSelectTime:NULL);
        // 0 means timeout
        if(0==rc)
        {
            rc = EDB_TIMEOUT;
            goto done;
        }
        // if < 0 , something wrong
        if(0>rc)
        {
            rc = SOCKET_GETLASTERROR;
            if(EINTR==rc)
            {
                continue;
            }
            PD_RC_CHECK(EDB_NETWORK, PDERROR, 
                        "Failed to select from socket, error = %d", 
                        rc);
        }
        if(FD_ISSET(_fd, &fds))
        {
            break;
        }
    }
    while(len > 0 )
    {
        rc = ::recv(_fd, pMsg, len, MSG_NOSIGNAL|flags);
        if(rc > 0)
        {
            if(flags & MSG_PEEK)
            {
                goto done;
            }
            len -= rc;
            pMsg += rc;
        }
        else if(rc==0)
        {
            PD_RC_CHECK(EDB_NETWORK_CLOSE, PDINFO, 
                        "Peer unexcepted shutdown");
        }
        else
        {
            rc = SOCKET_GETLASTERROR;
            if((EAGAIN==rc||EWOULDBLOCK==rc) &&
                _timeout>0)
            {
                PD_RC_CHECK(EDB_NETWORK, PDERROR, 
                            "Recv() timeout, error = %d", rc);
            }
            if((EINTR == rc) && (retries<MAX_RECV_PETRIES))
            {
                retries++;
                continue;
            }
            PD_RC_CHECK(EDB_NETWORK, PDERROR, 
                        "Recv() Failed, error = %d", rc);
        }
    }
    rc = EDB_OK;
done:
    return rc;
error:
    goto done;
}
```

select()函数部分，与send()函数类似，唯一差别在于设置文件描述符集合的可读性，当文件_fd准备好可读时，进行socket读操作(revc）

recv()函数同样进行封装，保证接收的健壮性。

recv的原型是ssize_t recv(int s, void *buf, size_t len, int flags); 通常flags都设置为0，此时recv函数读取tcp buffer中的数据到buf中，并从tcp buffer中移除已读取的数据。把flags设置为MSG_PEEK，仅把tcp buffer中的数据读取到buf中，并不把已读取的数据从tcp buffer中移除，再次调用recv仍然可以读到刚才读到的数据。

程序从socket中读取数据，若flags设置了MSG_PEEK，意味着读取操作不移除socket内存空间数据，此时仅读取定长len的数据到pMsg；读取文件返回值<0报错，若为中断错误进行重新读取，retries次数超过定值最后报错，写入日志。

#### 关闭socket文件

close()函数封装

```c
void ossSocket::close()
{
    if(_init)
    {
        int i=0;
        i = ::close(_fd);
        if(i<0)
            i = -1;
        _init = false;
    }
}
```

最后将初始化状态置false.

#### 禁用Nagle算法 心跳检测

```c
int _ossSocket::disableNagle ()
{
   int rc = EDB_OK ;
   int temp = 1 ;
   rc = setsockopt ( _fd, IPPROTO_TCP, TCP_NODELAY, (char *) &temp,
                     sizeof ( int ) ) ;
   if ( rc )
   {
      PD_LOG ( PDWARNING, "Failed to setsockopt, rc = %d", SOCKET_GETLASTERROR ) ;
   }

   rc = setsockopt ( _fd, SOL_SOCKET, SO_KEEPALIVE, (char *) &temp,
                     sizeof ( int ) ) ;
   if ( rc )
   {
      PD_LOG ( PDWARNING, "Failed to setsockopt, rc = %d", SOCKET_GETLASTERROR ) ;
   }
   return rc ;
}
```

Nagle算法用于将小包封装打包发送，禁用该算法保证低延迟。SO_KEEPALIVE选项在检测到对方未响应后主动关闭TCP连接。

#### 辅助函数

```c
unsigned int _ossSocket::_getPort(sockaddr_in *addr)
{
    return ntohs(addr->sin_port);
}

int _ossSocket::_getAddress(sockaddr_in *addr, char *pAddress, unsigned int length)
{
    int rc = EDB_OK;
    length = length < NI_MAXHOST ? length : NI_MAXHOST;
    rc = getnameinfo((struct sockaddr*)addr, sizeof(sockaddr),  pAddress, length,
                    NULL, 0, NI_NUMERICHOST);
    if(rc)
    {
        PD_RC_CHECK(EDB_NETWORK, PDERROR,
                    "Failed to getnameinfo, error = %d", rc);
    }
done:
    return rc;
error:
    goto done;
}

unsigned int _ossSocket::getLocalPort()
{
    return _getPort(&_sockAddress);
}

unsigned int _ossSocket::getPeerPort()
{
    return _getPort(&_peerAddress);
}

int _ossSocket::getLocalAddress(char * pAddress, unsigned int length)
{
    return _getAddress(&_sockAddress, pAddress, length);
}

int _ossSocket::getPeerAddress(char * pAddress, unsigned int length)
{
    return _getAddress(&_peerAddress, pAddress, length);
}

```

各个函数功能显而易见。

### 客户端socket应用设计

**edb class 私有成员变量**

```c
ossSocket       _sock;
```

_sock 是客户端创建并初始化的socket文件对象。

**connect 命令连接服务端socket**

```c
int ConnectCommand::execute(ossSocket &sock, std::vector<std::string> &argVec)
{
    int ret = EDB_OK;
    _address = argVec[0];
    _port = atoi(argVec[1].c_str());
    sock.close();
    sock.setAddress(_address.c_str(), _port);
    ret = sock.initSocket();
    if(ret)
    {
        return getError(EDB_SOCK_INIT_FAILED);
    }
    ret = sock.connect();
    if(ret)
    {
        return getError(EDB_SOCK_CONNECT_FAILED);
    }
    sock.disableNagle();
    return ret;
}
```

这是一条执行命令的类ConnectCommand的成员函数execute()，暂时只关注它实现的功能—连接到服务端socket。argVec是解析好的用户传入命令，每个元素代表一个参数。第一个参数为目标主机地址赋值给私有成员变量\_address，第二个参数为目标主机端口号赋值给私有成员变量\_port. 

先关闭sock，再根据上述地址和端口号设置socket绑定地址，然后初始化socket，并调用connect()函数进行连接socket，最后禁用Nagle算法函数。上述几个步骤进行的同时，设置异常。

**sendOrder()函数** 客户端发送数据

```c
int ICommand::sendOrder(ossSocket &sock, int opCode)
{
    int ret = EDB_OK;
    memset(_sendBuf, 0, SEND_BUF_SIZE);
    char * pSendBuf = _sendBuf;
    const char *pStr = "hello world";
    *(int *)pSendBuf = strlen(pStr) + 1 + sizeof(int);
    memcpy(&pSendBuf[sizeof(int)], pStr, strlen(pStr)+1);
    ret = sock.send(pSendBuf, *(int *)pSendBuf);
}
```

这是一个测试类型的函数，用于验证是否能够通信。ossSocket &sock传入edb类的私有成员变量\_sock；\_sendBuf是ICommand的私有成员变量，用于存储发送的数据；*(int *)pSendBuf = — 是指向字符串数组的指针pSendBuf的数组长度，计算得= 11 + 1 + 4  = 16; 由于\_sendBuf前四个字节存储消息字节数，故从sizeof(int )开始存储消息。最后调用ossSocket对象sock的send函数进行消息发送。

### 服务端socket连接测试设计

```c
int pmdTcpListenerEntryPoint()
{
    int rc = EDB_OK;
    int port = 12551;
    ossSocket sock(port);
    rc = sock.initSocket();
    if(rc)
    {
        printf("Failed to initialize socket, rc = %d", rc);
        goto error;
    }
    rc = sock.bind_listen();
    if(rc)
    {
        printf("Failed to bid/listen socket, rc = %d", rc);
        goto error;
    }
    // master loop for tcp listener
    while(true)
    {
        int s;
        rc = sock.accept(&s, NULL, NULL);
        // if we dont get anything from period of time, lets loop
        if(EDB_TIMEOUT == rc)
        {
            rc = EDB_OK;
            continue;
        }
        char buffer[1024];
        int size;
        ossSocket sock1(&s);
        sock1.disableNagle();
        do
        {
            rc = sock1.recv((char*)&size, 4);
            if(rc && rc!=EDB_TIMEOUT)
            {
                printf("Failed to receive size, rc = %d", rc);
                goto error;
            }
        }while(EDB_TIMEOUT == rc);
        do
        {
            rc = sock1.recv(&buffer[0], size-sizeof(int));
            if(rc && rc!=EDB_TIMEOUT)
            {
                printf("Failed to receive buffer, rc = %d", rc);
                goto error;
            }
        }while(EDB_TIMEOUT == rc);
        printf("%s\n", buffer);
        sock1.close();
    }

error:
    switch (rc)
    {
    case EDB_SYS:
        printf("system error occured");
        break;
    
    default:
        printf("internal error");
    }
    goto done;
done:
    return rc;
}
```

设置socket监听端口为port = 12551

使用ossSocket(port)的重载函数构造服务器端socket对象，并调用initSocket()函数进行初始化，最后绑定监听

进入迭代循环，一直监听端口是否有客户端访问，若有访问请求，则在accept()函数得到新的socket文件描述符s，调用ossSocket(int *sock)的构造函数构造socket对象sock1，先接收消息的字节数，再传入字节数接收消息内容。最后关闭socket.

## 锁机制

两个锁类对互斥锁、读写锁进行封装

### ossXLatch类 互斥锁

私有成员变量：

pthread_mutex_t _lock; 互斥锁

成员函数

```c
public:
        ossXLatch()
        {
            pthread_mutex_init(&_lock, 0);
        }
        ~ossXLatch()
        {
            pthread_mutex_destroy(&_lock);
        }
        void get()
        {
            pthread_mutex_lock(&_lock);
        }
        void release()
        {
            pthread_mutex_unlock(&_lock);
        }
        bool try_get()
        {
            return (pthread_mutex_trylock(&_lock)==0);
        }
```

### ossSLatch类 读写锁

私有成员变量：

pthread_rwlock_t _lock;

成员函数：

```c
				ossSLatch()
        {
            pthread_rwlock_init(&_lock, 0);
        }
        ~ossSLatch()
        {
            pthread_rwlock_destroy(&_lock);
        }
        void get()
        {
            pthread_rwlock_wrlock(&_lock);
        }
        void release()
        {
            pthread_rwlock_unlock(&_lock);
        }
        bool try_get()
        {
            return (pthread_rwlock_trywrlock(&_lock)==0);
        }
        void get_shared()
        {
            pthread_rwlock_rdlock(&_lock);
        }
        void release_shared()
        {
            pthread_rwlock_unlock(&_lock);
        }
        bool try_get_shared()
        {
            return (pthread_rwlock_tryrdlock(&_lock)==0);
        }
```

加锁的封装暂时不关注。

## 文件操作

### ossPrimitiveFileOp 类 文件操作封装

文件操作创建一个ossPrimitiveFileOp类，类内包含句柄\_fileHandle, 是否设置标准输出\_bisStdout等私有成员变量；类内公有成员函数 Open()/Close() 打开文件/关闭文件，Read()/Write() 读/写数据，seekToOffset() 跳转文件游标位置等。

#### 私有成员与构造函数

定义私有成员变量_fileHandle, 是文件操作对象操纵具体文件的文件描述符。

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

声明无法直接生成对象的的构造函数

```c
private:
        // 构造方法的两种方式 禁止从一个对象生成另外一个对象
				// 将()表达式写为私有成员 用户将不能调用
        ossPrimitiveFileOp(const ossPrimitiveFileOp &){};
				// 重载‘=’运算符号为私有成员，用户不能调用
        const ossPrimitiveFileOp &operator=(const ossPrimitiveFileOp &);
				// 是否为标准输出
        bool _bIsStdout;
```

#### 构造函数

```c
ossPrimitiveFileOp::ossPrimitiveFileOp()
{
    _fileHandle = OSS_INVALID_HANDLE_FD_VALUE;
    _bIsStdout  = false;
}
```

OSS_INVALID_HANDLE_FD_VALUE 为定义的宏 `#define OSS_INVALID_HANDLE_FD_VALUE (-1)` -1的文件描述符表示非法。

#### Open() 打开文件函数

函数声明：


```c
int Open
   (
      const char * pFilePath,
      unsigned int options = OSS_PRIMITIVE_FILE_OP_OPEN_ALWAYS
   ) ;
// optoins->打开方式 reading only or writing only etc.
```

函数定义：

```c
int ossPrimitiveFileOp::Open(const char * pFilePath, unsigned int options)
{
    int rc = 0;
    int mode = O_RDWR;

    if(options & OSS_PRIMITIVE_FILE_OP_READ_ONLY)
    {
        mode = O_RDONLY;
    }
    else if(options & OSS_PRIMITIVE_FILE_OP_WRITE_ONLY)
    {
        mode = O_WRONLY;
    }
    
    if(options & OSS_PRIMITIVE_FILE_OP_OPEN_EXISTING)
    {

    }
    else if(options & OSS_PRIMITIVE_FILE_OP_OPEN_ALWAYS)
    {
        mode |= O_CREAT;
    }
    if(options & OSS_PRIMITIVE_FILE_OP_OPEN_TRUNC)
    {
        mode |= O_TRUNC;
    }
    
    do
    {
        _fileHandle = oss_open(pFilePath, mode, 0644);
    }while((-1==_fileHandle)&&(EINTR == errno));
    if(_fileHandle <= OSS_INVALID_HANDLE_FD_VALUE)
    {
        rc = errno;
        goto exit;
    }

exit:
    return rc;

}

```

选项options的宏定义：

```c
#define OSS_PRIMITIVE_FILE_OP_READ_ONLY         (((unsigned int)1) << 1)
#define OSS_PRIMITIVE_FILE_OP_WRITE_ONLY        (((unsigned int)1) << 2)
#define OSS_PRIMITIVE_FILE_OP_OPEN_EXISTING     (((unsigned int)1) << 3)
#define OSS_PRIMITIVE_FILE_OP_OPEN_ALWAYS       (((unsigned int)1) << 4)
#define OSS_PRIMITIVE_FILE_OP_OPEN_TRUNC        (((unsigned int)1) << 5)
```

传入的options参数通过位的与操作，判断打开文件时选择的打开方式：O_RDONLY or O_WRONLY or O_RDWR。详见文件IO。

oss_open 是对open64的宏定义，open64()函数 原型：`int open64(const char *pathname, int oflag,...);` 

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

The file status flags and file access modes of the open file description are set according to the value of *oflag*. 

#### Close() 关闭文件函数

```c
void ossPrimitiveFileOp::Close()
{
    if(isValid()&&(!_bIsStdout))
    {
        oss_close(_fileHandle);
        _fileHandle = OSS_INVALID_HANDLE_FD_VALUE;
    }
}
```

oss_close 是Unix文件操作close的宏定义，close函数原型：`int close(int fd);`在调用关闭函数之前，判断文件是否非法以及是否是标准输出(标准输出不能关闭)，调用close函数后，将对象的成员变量_fileHandle设置为非法状态。

#### Read() 读文件

函数原型：`Read(const size_t size, void * const pBuffer,  int * const pBytesRead)`

size_t size, 要读取的字节数 正数size_t.

void * const pBuffer, 指向任意类型存储空间的不可变指针，指向的是读取数据完成后数据保存的内存空间。

int * const pBytesRead, 指向int的不可变指针，指向的是读取数据完成后读取到的数据长度保存的内存空间。

```c
int ossPrimitiveFileOp::Read(const size_t size,
                             void * const pBuffer,
                             int * const pBytesRead)
{
    int retval = 0;
    ssize_t bytesRead = 0;
    if(isValid())
    {
        do
        {
            bytesRead = oss_read(_fileHandle, pBuffer, size);
        } while ((-1 == bytesRead) && (EINTR == errno));
        if(-1 == bytesRead)
        {
            goto err_read;
        }
    }
    else
    {
        goto err_read;
    }
    if(pBytesRead)
    {
        *pBytesRead = bytesRead;
    }
exit:
    return retval;
err_read:
    *pBytesRead = 0;
    retval = errno;
    goto exit;
}
```

oss_read 是Unix文件操作read的宏定义，read函数原型 `ssize_t read(int fd, void *buf, size_t, n);` read读取失败返回-1，进入异常处理：读取到的数据长度设置为0，返回错误代码

Write() 写入文件

函数原型：`ossPrimitiveFileOp::Write(const void *pBuffer, size_t size)`

const void *pBuffer, 写入的数据存储空间。

size_t size, 正数的写入的数据存储空间字节数。

```c
int ossPrimitiveFileOp::Write(const void *pBuffer, size_t size)
{
    int rc = 0;
    size_t currentSize = 0;
    if(0==size)
    {
        size = strlen((char *)pBuffer);
    }

    if(isValid())
    {
        do
        {
            rc = oss_write(_fileHandle, &((char *)pBuffer)[currentSize], 
                            size-currentSize);
            if(rc > 0)
                currentSize += rc;
        }while(((-1 == rc) && (EINTR == errno)) ||
                ((-1 != rc) && (currentSize != size)));
        
        if(-1 == rc)
        {
            rc = errno;
            goto exit;
        }
        rc = 0;
    }
exit:
    return rc;

}

```

oss_write 是对Unix 文件操作write的宏定义，其函数原型：`ssize_t write(int fd, const void *buf, size_t n);`这里设置了currentSize标记已经读取到的字节个数。下次迭代传入的写入字节数变为size-currentSize.

#### 文件游标跳转seek

**seekToEnd() 跳转到文件末尾**

```c
void ossPrimitiveFileOp::seekToEnd(void)
{
    oss_lseek(_fileHandle, 0, SEEK_END);
}
```

oss_lseek 是Unix文件操作lseek64的宏定义，seek函数原型：`off_t lseek(int filedes, off_t offset, int whence);` whence 是跳转标记，as follows:

-   **SEEK_SET**

    The offset is set to *offset* bytes.

-   **SEEK_CUR**

    The offset is set to its current location plus *offset* bytes.

-   **SEEK_END**

    The offset is set to the size of the file plus *offset* bytes.

**getCurrentOffset() 获取当前游标偏移量**

```c
offsetType ossPrimitiveFileOp::getCurrentOffset() const
{
    return oss_lseek(_fileHandle, 0, SEEK_CUR);
}
```

**seekToOffset() 跳转到指定偏移量的位置**

```
void ossPrimitiveFileOp::seekToOffset(offsetType offset)
{
    if((oss_off_t)-1 != offset)
    {
        oss_lseek(_fileHandle, offset, SEEK_SET);
    }
}
```

#### getSize() 获取文件大小

```c
int ossPrimitiveFileOp::getSize(offsetType *const pFileSize)
{
    int rc = 0;
    oss_struct_stat buf = {0};

    if(-1 == oss_fstat(_fileHandle, &buf))
    {
        rc = errno;
        goto err_exit;
    }
    *pFileSize = buf.st_size;

exit:
    return rc;
err_exit:
    *pFileSize = 0;
    goto exit;
}
```

oss_fstat是对Unix文件操作fstat64的宏定义，其函数原型：`int fstat64(int fildes, struct stat64 *buf);`传入参数buf为struct stat64结构体stat类型，where status information about the file is to be placed. offsetType *const fFileSize 是指向off64_t类型的不可变指针，用于存储查询到的文件大小：buf.st_size.

>   **errno**
>
>   NAME  
>
>   ```
>          errno - number of last error
>   ```
>
>   SYNOPSIS      
>
>   ```
>          #include <errno.h>
>   ```
>
>   DESCRIPTION      
>
>   ```
>          The <errno.h> header file defines the integer variable errno, which
>          is set by system calls and some library functions in the event of an
>          error to indicate what went wrong.
>   ```
>



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

缺省参数格式化读取 va_start

>    Description
>
>   The C library macro **void va_start(va_list ap, last_arg)** initializes **ap** variable to be used with the **va_arg** and **va_end** macros. The **last_arg** is the last known fixed argument being passed to the function i.e. the argument before the ellipsis.
>
>   This macro must be called before using **va_arg** and **va_end**.
>
>   Declaration
>
>   Following is the declaration for va_start() macro.
>
>   ```
>   void va_start(va_list ap, last_arg);
>   ```
>
>   Parameters
>
>   - **ap** − This is the object of **va_list** and it will hold the information needed to retrieve the additional arguments with **va_arg**.
>   - **last_arg** − This is the last known fixed argument being passed to the function.
>
>   Example
>
>   The following example shows the usage of va_start() macro.
>
>   ```c
>   #include<stdarg.h>
>   #include<stdio.h>
>   
>   int sum(int, ...);
>   
>   int main(void) {
>      printf("Sum of 10, 20 and 30 = %d\n",  sum(3, 10, 20, 30) );
>      printf("Sum of 4, 20, 25 and 30 = %d\n",  sum(4, 4, 20, 25, 30) );
>   
>      return 0;
>   }
>   
>   int sum(int num_args, ...) {
>      int val = 0;
>      va_list ap;
>      int i;
>   
>      va_start(ap, num_args);
>      for(i = 0; i < num_args; i++) {
>         val += va_arg(ap, int);
>      }
>      va_end(ap);
>    
>      return val;
>   }
>   ```
>
>   Let us compile and run the above program to produce the following result −
>
>   ```
>   Sum of 10, 20 and 30 = 60
>   Sum of 4, 20, 25 and 30 = 79
>   ```
>
>   **vsprintf**
>
>   ```
>   int vsprintf (char * s, const char * format, va_list arg );
>   ```
>
>   Write formatted data from variable argument list to string
>
>   Composes a string with the same text that would be printed if *format* was used on [printf](http://www.cplusplus.com/printf), but using the elements in the variable argument list identified by *arg* instead of additional function arguments and storing the resulting content as a *C string* in the buffer pointed by *s*.
>
>   Internally, the function retrieves arguments from the list identified by *arg* as if [va_arg](http://www.cplusplus.com/va_arg) was used on it, and thus the state of *arg* is likely to be altered by the call.
>
>   In any case, *arg* should have been initialized by [va_start](http://www.cplusplus.com/va_start) at some point before the call, and it is expected to be released by [va_end](http://www.cplusplus.com/va_end) at some point after the call.
>
>   Parameters
>
>   - s
>
>     Pointer to a buffer where the resulting C-string is stored. The buffer should be large enough to contain the resulting string.
>
>   - format
>
>     C string that contains a format string that follows the same specifications as *format* in [printf](http://www.cplusplus.com/printf) (see [printf](http://www.cplusplus.com/printf) for details).
>
>   - arg
>
>     A value identifying a variable arguments list initialized with [va_start](http://www.cplusplus.com/va_start). [va_list](http://www.cplusplus.com/va_list) is a special type defined in <cstdarg>.
>
>     
>

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

## 客户端设计Client

### 主程序edb

#### Edb类

```c
class Edb
{
    public:
        Edb() {};
        ~Edb() {};
    
    public:
        void start(void);
    
    protected:
        void prompt(void);
    
    private:
        void        split(const std::string &text, char delim, std::vector<std::string> &result);
        char*       readLine(char *p, int length);
        int         readInput(const char* pPrompt, int numIndent);
    
    private:
        ossSocket       _sock;
        CommandFactory  _cmdFactory;
        char            _cmdBuffer[CMD_BUFFER_SIZE];
};
```

公有成员函数：

-   start() 启动客户端主程序
-   prompt() 人机交互窗口，用于提示信息，并将命令输入机器

私有成员函数：

-   split() 解析一段字符串
-   readLine() 读取用户输入的一行
-   readInput() 读取用户输入，并进行处理

私有成员变量：

-   ossSocket _sock, 客户端socket对象，用于socket消息传递
-   CommandFactory \_cmdFactory, 命令工厂类，对程序内置命令进行初始化，并存储到类的成员变量\_cmdMap中，\_cmdMap主要用来映射命令字符串与相应命令类
-   char \_cmdBuffer[], 存储用户输入 

#### start()启动

```c
void Edb::start(void)
{
    std::cout<< "Welcome to EmeraldDB shell!" << std::endl;
    std::cout<< "edb help for help, Ctrl+c or quit to exit " << std::endl;
    while(0==gQuit)
    {
        prompt();
    }
}
```

屏幕打印基本信息，进入循环。设置全局变量`int gQuit = 0;`当gQuit不变时客户端保持在人机交互窗口，用户输入退出命令时gQuit置1，程序结束。

#### 读取输入信息

**读取一行**

```c
char *Edb::readLine(char *p, int length)
{
    int len = 0;
    char ch;
    while((ch=getchar()) != NEW_LINE)
    {
        switch (ch)
        {
        case BACK_SLANT:
            break;
        default:
            p[len] = ch;
            len++;
        }
        continue;
    }
    len = strlen(p);
    p[len] = 0;
    return p;
}
```

宏定义NEW_LINE -> '\n', BACK_SLANT -> '\\\\'. 读取标准输入，直到遇到换行符或者反斜杠(表示下文继续这一行)

**读取用户输入的完整信息**

```c
int Edb::readInput(const char *pPrompt, int numIndent)
{
    memset(_cmdBuffer, 0, CMD_BUFFER_SIZE);
    // print tab
    for(int i=0; i<numIndent; ++i)
    {
        std::cout << TAB;
    }
    // print "edb>    "
    std::cout << pPrompt << ">  ";
    // read a line from cmd
    readLine(_cmdBuffer, CMD_BUFFER_SIZE-1);
    int curBufLen = strlen(_cmdBuffer);
    // "\" means continue
    while(_cmdBuffer[curBufLen-1] == BACK_SLANT
        && (CMD_BUFFER_SIZE-curBufLen)>0)
    {
        for(int i=0; i<numIndent; ++i)
        {
            std::cout<< TAB;
        }
        std::cout<<"> ";
        readLine(&_cmdBuffer[curBufLen-1], CMD_BUFFER_SIZE-curBufLen);
    }
    curBufLen = strlen(_cmdBuffer);
    for(int i=0; i < curBufLen; ++i)
    {
        if(_cmdBuffer[i] == TAB)
        {
            _cmdBuffer[i]=SPACE;
        }
    }
    return EDB_OK;
}

```

与只读一行信息不同的是，读取完整的信息时readInput()函数对用户操作进行了界面美化，即每一步输出 ">",并且允许用户输入多行，使用反斜杠"\\"进行标记。首先读取第一行，若第一行行尾是"\\"则继续读取下一行。

**命令解析 delim切割**

```c
void Edb::split(const std::string &text, char delim, std::vector<std::string> &result)
{
    size_t strLen = text.length();
    size_t first = 0;
    size_t pos = 0;
    for(first = 0; first<strLen; first=pos+1)
    {
        pos = first;
        while(text[pos]!=delim && pos<strLen)
        {
            pos++;
        }
        // truncate str
        std::string str = text.substr(first, pos-first);
        result.push_back(str);
    }
    return;
}
```

对传入的字符串text以delim为单位进行切割，使用字符串的substr(start, len)函数切割，切割后的每个元素str保存在vector\<string\> 类型参数的result中.

**prompt()交互窗口**

```c
void Edb::prompt(void)
{
    int ret = EDB_OK;
    ret = readInput("edb", 0);
    if(ret)
    {
        return;
    }
    // Input string
    std::string textInput = _cmdBuffer;
    // split the input sentence
    std::vector<std::string> textVec;
    split(textInput, SPACE, textVec);
    int count = 0;
    std::string cmd = "";
    std::vector<std::string> optionVec;

    std::vector<std::string>::iterator iter = textVec.begin();
    // handle different command here.
    ICommand * pCmd = NULL;
    for(; iter!=textVec.end(); ++iter)
    {
        std::string str = *iter;
        if(0==count)
        {
            cmd = str;
            count ++;
        }
        else
        {
            optionVec.push_back(str);
        }
    }
    pCmd = _cmdFactory.getCommandProcesser(cmd.c_str());
    if(NULL!=pCmd)
    {
        pCmd->execute(_sock, optionVec);
    }
}
```

首先调用readInput()函数，并传入提示信息'edb', 接收用户输入到\_cmdBuffer存储空间,  这里拷贝\_cmdBuffer字符串到新的string对象textInput来进行命令切割,切割完的字符串保存到vector \<string\>对象textVec, 接下来遍历该vector，读取到cmd命令字符串；获取到命令代码后调用\_cmdFactory类的getCommandProcesser()方法，参数为命令代码字符串，该方法返回指向ICommand类的指针，方法内是遍历\_cmdMap, 一个map类型的容器，存储命令字符串与命令类的映射，每次遍历到的命令字符串存放到vector；vector存放的第一个元素是cmd命令字符串，传入客户端socket对象和optionVec执行命令，即调用ICommand类的子类的execute()方法。

### 命令类

#### Icommand 类

ICommand作为基类，提供给几个命令类继承，重写execute()函数

```c++
class ICommand
{
    typedef int(*OnMsgBuild)(char ** ppBuffer, int * pBufferSize,   \
                bson::BSONObj &obj);
    public:
        virtual int execute(ossSocket &sock, std::vector<std::string> &argVec);
        int         getError(int code);
    
    protected:
        int         recvReply(ossSocket &sock);
        int         sendOrder(ossSocket &sock, OnMsgBuild onMsgBuild);
        int         sendOrder(ossSocket &sock, int opCode);
    
    protected:
        virtual int handleReply()   {return EDB_OK;}
    
    protected:
        char _recvBuf[RECV_BUF_SIZE];
        char _sendBuf[SEND_BUF_SIZE];
        std::string _jsonString;
};
```

ICommand类有两个公有函数 execute()和getError(), 前者基于建立的socket对象发送给服务端消息或执行本地化命令，后者是返回错误码。保护函数: recvReply() 接收服务端发来的数据，sendOrder() 发送命令到服务端程序。

recvReply()函数

```c++
// receive a message that replyed
int ICommand::recvReply(ossSocket &sock)
{
    // define message data length
    int length = 0;
    int ret = EDB_OK;
    // fill receive buffer with 0.
    memset(_recvBuf, 0, RECV_BUF_SIZE);
    if(!sock.isConnected())
    {
        return getError(EDB_SOCK_NOT_CONNECT);
    }
    while(1)
    {
        // receive data from the server.first receive the length of the data
        ret = sock.recv(_recvBuf, sizeof(int));
        if(EDB_TIMEOUT == ret)
        {
            continue;
        }
        else if(EDB_NETWORK_CLOSE == ret)
        {
            return getError(EDB_SOCK_REMOTE_CLOSED);
        }
        else
        {
            break;
        }
    }
    // get the value of length
    length = *(int *)_recvBuf;
    if(length > RECV_BUF_SIZE)
    {
        return getError(EDB_RECV_DATA_LENGTH_ERROR);
    }

    // receive data from the server.second receive the last data
    while(1)
    {
        ret = sock.recv(&_recvBuf[sizeof(int)], length-sizeof(int));
        if(ret == EDB_TIMEOUT)
        {
            continue;
        }
        else if(EDB_NETWORK_CLOSE == ret)
        {
            return getError(EDB_SOCK_REMOTE_CLOSED);
        }
        else
        {
            break;
        }
    }
    return ret;
} 
```

先接收数据的长度，再接收数据。接收方法使用ossSocket类的recv方法，健壮地读消息，返回错误码后调用getError()方法输出错误消息。

**sendOrder() 发送命令**

```c++
int ICommand::sendOrder(ossSocket &sock, int opCode)
{
    int ret = EDB_OK;
    memset(_sendBuf, 0, SEND_BUF_SIZE);
    char * pSendBuf = _sendBuf;
    const char *pStr = "hello world";
    *(int *)pSendBuf = strlen(pStr) + 1 + sizeof(int);
    memcpy(&pSendBuf[sizeof(int)], pStr, strlen(pStr)+1);
    ret = sock.send(pSendBuf, *(int *)pSendBuf);
}
```

测试使用，发送消息为helloworld，

#### ConnectCommand 连接命令

公有继承ICommand类，并重写execute() 函数. 连接命令是用来实现socket客户端与服务端的连接.

std::string _address,存储 用户传入的ip地址，字符串格式。

int _port，存储用户传入的port

类的声明：

```c++
class ConnectCommand: public ICommand
{
    public:
        int execute(ossSocket &sock, std::vector<std::string> &argVec);
    
    private:
        std::string _address;
        int         _port;
};
```

类的实现：

**execute()命令执行函数**

传入的参数为ossSocket对象 sock/vector\<std::string\> argVec命令容器建立该socket的连接。socket建立连接执行ossSocket对象的几个命令：先close()关闭文件描述符对应的socket，再setAddress(add, port)设置socket监听的地址和端口，然后initSocket()初始化客户端socket并创建socket文件，初始化成功后调用连接函数connect()，本地的客户端socket文件就可以和服务端socket建立连接，最后设置disableNagle()函数，禁止小包打包成大包再发送。

```c++
/****************************Connect Command**********************************/
int ConnectCommand::execute(ossSocket &sock, std::vector<std::string> &argVec)
{
    int ret = EDB_OK;
    _address = argVec[0];
    _port = atoi(argVec[1].c_str());
    sock.close();
    sock.setAddress(_address.c_str(), _port);
    ret = sock.initSocket();
    if(ret)
    {
        return getError(EDB_SOCK_INIT_FAILED);
    }
    ret = sock.connect();
    if(ret)
    {
        return getError(EDB_SOCK_CONNECT_FAILED);
    }
    sock.disableNagle();
    return ret;
}

```

#### help命令

用户输入help后屏幕输出提示信息

```c++
int HelpCommand::execute(ossSocket &sock, std::vector<std::string> &argVec)
{
    int ret = EDB_OK;
    printf("List of classes of commands:\n\n");
    printf("%s [server] [port]-- connecting emeralddb server\n", COMMAND_CONNECT);
    printf("%s -- sending a insert command to emeralddb server\n", COMMAND_INSERT);
    printf("%s -- sending a query command to emeralddb server\n", COMMAND_QUERY);
    printf("%s -- sending a delete command to emeralddb server\n", COMMAND_DELETE);
    printf("%s [number]-- sending a test command to emeralddb server\n", COMMAND_TEST);
    printf("%s -- providing current number of record inserting\n", COMMAND_SNAPSHOT);
    printf("%s -- quitting command\n\n", COMMAND_QUIT);
    printf("Type \"help\" command for help\n");
    return ret;
}
```

#### quit命令

离开客户端命令quit

```c++
/****************************Quit Command**********************************/
int QuitCommand::handleReply()
{
    int ret = EDB_OK;
    // gQuit = 1;
    return ret;
}

int QuitCommand::execute(ossSocket &sock, std::vector<std::string> &argVec)
{
    int ret = EDB_OK;
    if(!sock.isConnected())
    {
        return getError(EDB_SOCK_NOT_CONNECT);
    }
    ret = sendOrder(sock, 0);
    // sock.close();
    ret = handleReply();
    return ret;
}

```



### CommandFactory 类

该类将各种基于ICommand类派生的命令类，如ConnectCommand类，将该类与命令字符串映射，并存储到容器 std::map\<std::string, ICommand*\>  \_cmdMap 中。CommandFactory类有一个公有函数getCommandProcesser()，该函数传入命令字符串，遍历上述\_cmdMap找到命令类，并返回指针。

类的定义

```c++
#define COMMAND_BEGIN void CommandFactory::addCommand() {
#define COMMAND_END }
#define COMMAND_ADD(cmdName,cmdClass) {                         \
   ICommand* pObj = new cmdClass();                             \
   std::string str = cmdName;                                   \
   _cmdMap.insert(COMMAND_MAP::value_type(str,pObj));           \
   } 

class CommandFactory
{
    typedef std::map<std::string, ICommand*> COMMAND_MAP;
    public:
        CommandFactory();
        ~CommandFactory(){}
        void addCommand();
        ICommand * getCommandProcesser(const char * pcmd);
    private:
        COMMAND_MAP _cmdMap;
};
```

getCommandProcesser()

```c++
ICommand *CommandFactory::getCommandProcesser(const char* pcmd)
{
    ICommand * pProcessor = NULL;
    do{
        COMMAND_MAP::iterator iter;
        iter = _cmdMap.find(pcmd);
        if(iter != _cmdMap.end())
        {
            pProcessor = iter->second;
        }
    }while(0);
    return pProcessor;
}
```

利用宏定义来格式化命令类

```

COMMAND_BEGIN   
COMMAND_ADD(COMMAND_CONNECT,ConnectCommand)
COMMAND_ADD(COMMAND_QUIT,QuitCommand)
COMMAND_ADD(COMMAND_HELP,HelpCommand)
COMMAND_END
```

这里不再推荐这种方式 太难读了，也不好调试。

## 核调度设计

### KRCB

对于emeralddb数据库系统，应该有一个主模块用于管理数据库当前状态，以及诸如数据库文件存储位置、日志文件存储位置等系统基本配置，以及获取配置的方法。

#### 核状态

数据库的状态如下表，不同状态应有不同的操作方法

| 状态名   | 变量名          | 代码 |
| -------- | --------------- | ---- |
| 正常状态 | EDB_DB_NORMAL   | 0    |
| 关机状态 | EDB_DB_SHUTDOWN | 1    |
| 病态     | EDB_DB_PANIC    | 2    |

#### 类 EDB_KRCB

class EDB_KRCB, 

成员变量：

```c++
char            _dataFilePath[OSS_MAX_PATHSIZE+1];	// 数据存储文件
char            _logFilePath[OSS_MAX_PATHSIZE+1];	// 日志文件
int             _maxPool;							// 线程池大小
char            _svcName[NI_MAXSERV+1];				// 服务端名称
EDB_DB_STATUS   _dbStatus;							// 数据库状态
pmdEDUMgr       _eduMgr;							// EDU线程管理单元
```

类的构造函数，char 数组全部设置为0，_dbStatus设置为EDB_DB_NORMAL，int 类型变量设置为0。

成员函数：

```c++
// get edumgr
inline pmdEDUMgr *getEDUMgr()
{
    return &_eduMgr;
} 

// get database status
inline EDB_DB_STATUS getDBStatus()
{
    return _dbStatus;
}
// setup database status
inline void setDBStatus(EDB_DB_STATUS status)
{
	_dbStatus = status;
}
```

类似的有：`const char *getDataFilePath()`，`const char *getLogFilePath()`，`const char *getSvcName()`, `int getMaxPool()`, `setDBStatus(EDB_DB_STATUS status)`, `setDataFilePath(const char *pPath)`, `setLogFilePath(const char *pPath)`,。

并设置全局的函数：

```C++
inline EDB_KRCB *pmdGetKRCB()
{
    return &pmd_krcb;
}
```

用于获取内核控制块KRCB。

#### 类初始化

内核控制块有一个

```C++
// setup from pmdOptions
int init(pmdOptions *options);
```

初始化函数

```c++
int EDB_KRCB::init(pmdOptions *options)
{
    setDBStatus(EDB_DB_NORMAL);
    setDataFilePath(options->getDBPath());
    setLogFilePath(options->getLogPath());
    strncpy(_pdDiagLogPath, getLogFilePath(), sizeof(_pdDiagLogPath));
    setSvcName(options->getServiceName());
    setMaxPool(options->getMaxPool());
    return EDB_OK;
}
```

内核控制块初始化时，读取命令行或者配置文件的配置参数，例如DBpath，LogPath等。

### EDU 线程单元

数据库维护一个线程池，线程池内的每个线程称为EDU，每个线程由一个EDUMgr进行管理(创建、销毁、回调线程池等操作)。多线程的作用是在同一时间提供给多个客户端多路访问数据库，

```c++
enum EDU_STATUS
{
    PMD_EDU_CREATING = 0,
    PMD_EDU_RUNNING,
    PMD_EDU_WAITING,
    PMD_EDU_IDLE,
    PMD_EDU_DESTROY,
    PMD_EDU_UNKNOWN,
    PMD_EDU_STATUS_MAXIMUM = PMD_EDU_UNKNOWN
};
```

EDU状态表

| 状态(EDU_STATUS) | 说明             |
| ---------------- | ---------------- |
| PMD_EDU_CREATING | 线程正在创建     |
| PMD_EDU_RUNNING  | 线程正在运行     |
| PMD_EDU_WAITING  | 等待调用         |
| PMD_EDU_IDLE     | 线程回调入线程池 |
| PMD_EDU_DESTROY  | 销毁             |
| PMD_EDU_UNKNOWN  | 未知             |

对于UNIX下系统线程操作，使用boost提供的thread类。

EDU种类：

| 类型(EDU_TYPES)      | 说明         |
| -------------------- | ------------ |
| EDU_TYPE_TCPLISTENER | TCP通信线程  |
| EDU_TYPE_AGENT       | 主程代理分发 |
| EDU_TYPE_UNKNOWN     | 未知         |



### EDUCB 线程单元控制块

EDU作为线程操作的封装，不但具有一个核控制的ID，同时拥有一个控制块CB(pmdEDUCB类)来进行线程控制。

CB的私有成员：

```C++
private:
        EDUID           _id;	// 创建的EDU id
        EDU_TYPES       _type;	// EDU 类型
        EDU_STATUS      _status;  // EDU 状态
        pmdEDUMgr       *_mgr;		// 内核manager，用于内核调度该EDU
        bool            _isForced;	// 是否强制停止
        bool            _isDisconnected;	// 失去连接
        ossQueue<pmdEDUEvent>   _queue;		// 该EDU拥有一个事件队列，处理内核分发的事务
```

CB的构造函数，首先要获取内核创建的线程manager—pmdEDUMgr, 再传入要创建的EDU类型— EDU_TYPES。

```cpp
pmdEDUCB::pmdEDUCB ( pmdEDUMgr *mgr, EDU_TYPES type ):
    _type(type),
    _mgr(mgr),
    _status(PMD_EDU_CREATING),
    _id(0),
    _isForced(false),
    _isDisconnected(false)
{
}
```

内联函数：

getID(), 获取该CB控制块控制的EDU的ID。

postEvent(), 将要处理的EDUEvent post到CB的私有成员变量_queue队列（存储待处理任务）。

waitEvent(), 在\_queue队列中获取一个待处理任务，注意到这里的wait，就是当_queue队列为空时。waitEvent()函数传入一个millsec作为事件post等待时间。CB执行waitEvent()方法时，EDU状态应该设置为WAITTING或者本身处于IDLE。当传入的millsec参数不合法(小于0)，队列\_queue 使用wait_and_pop()方法pop出一个Event，即等待队列不空时pop；当传入的参数合法，队列\_queue执行time\_wait\_and_pop() 方法pop Event。

其他的get/set系列函数顾名思义。

```C++
// 控制块
class pmdEDUCB
{
    public:
        pmdEDUCB(pmdEDUMgr *mgr, EDU_TYPES type);
        
        inline EDUID getID()
        {
            return _id;
        }
        inline void postEvent(pmdEDUEvent const &data)
        {
            _queue.push(data);
        }

        bool waitEvent(pmdEDUEvent &data, long long millsec)
        {
            // if millsec is not 0, that means we want timeout
            bool waitMsg = false;
            if(PMD_EDU_IDLE != _status)
            {
                _status = PMD_EDU_WAITING;
            }
            if(0>millsec)
            {
                _queue.wait_and_pop(data);
                waitMsg = true;
            }
            else
            {
                waitMsg = _queue.time_wait_and_pop(data, millsec);
            }
            if(waitMsg)
            {
                if(data._eventType == PMD_EDU_EVENT_TERM)
                {
                    _isDisconnected = true;
                }
                else
                {
                    _status = PMD_EDU_RUNNING;
                }
            }
            return waitMsg;
        }
        inline void force()
        {
            _isForced = true;
        }
        inline void disconnect()
        {
            _isDisconnected = true;
        }
        inline EDU_TYPES getType()
        {
            return _type;
        }
        inline EDU_STATUS getStatus()
        {
            return _status;
        }
        inline void setType(EDU_TYPES type)
        {
            _type = type;
        }
        inline void setID(EDUID id)
        {
            _id = id;
        }
        inline void setStatus(EDU_STATUS status)
        {
            _status = status;
        }
        inline bool isForced()
        {
            return _isForced;
        }
        inline pmdEDUMgr *getEDUMgr()
        {
            return _mgr;
        }
    
    private:
        EDUID           _id;
        EDU_TYPES       _type;
        EDU_STATUS      _status;
        pmdEDUMgr       *_mgr;
        bool            _isForced;
        bool            _isDisconnected;
        ossQueue<pmdEDUEvent>   _queue;
};
```

### EDUMgr 线程单元管理

线程的所有操作由控制块CB完成，给控制块下达指令的则是核内唯一的管理线程EDUMgr，它维护一个线程池，线程池分为\_runQueue(正在运行的线程队列)和\_idleQueue(等待运行的线程队列)。EDUMgr可以获取线程池的状态信息，例如EDU队列大小、系统EDU、创建EDU、销毁EDU、激活EDU等。

#### 类的成员变量

```c++
private:
        std::map<EDUID, pmdEDUCB *> _runQueue;
        std::map<EDUID, pmdEDUCB *> _idleQueue;
        std::map<unsigned int, EDUID> _tid_eduid_map;

        ossSLatch _mutex;
        // increamental-only EDU id
        // 64 bit is big enough for most
        EDUID _EDUID;
        // list of system EDUS
        std::map<EDU_TYPES, EDUID> _mapSystemEDUS;
        // no new requests are allowed
        bool _isQuiesced;
        bool _isDestroyed;

```

`std::map<EDUID, pmdEDUCB *> _runQueue`, 这是EDUMgr类的成员设计的一个map容器，用于存储处于RUNNING状态的EDU，映射关系为线程单元ID到线程单元控制块的指针。

`std::map<EDUID, pmdEDUCB *> _idleQueue`，功能与\_runQueue区别在于存储处于IDLE状态的EDU。

`std::map<unsigned int, EDUID> _tid_eduid_map`, 线程池的EDU在创建时会被系统分配一个`thread_id`, 设计一个map容器用于存储tid与EDUID的映射关系，方便系统使用tid找到EDB系统中对应的EDUID。

`ossSLatch _mutex`，多线程的程序开发中，对于临界资源的访问要保证读取数据的干净，所以在读数据时加锁。

`EDUID _EDUID`, 线程单元管理员Mgr维护一个递增的`_EDUID`,新建的EDU使用这个`_EDUID` 。

` std::map<EDU_TYPES, EDUID> _mapSystemEDUS`, 在EDB系统中有些EDU线程单元是要为系统运行服务的，将其定义为SystemEDUS，映射关系为EDU类型到线程单元ID。

`bool _isQuiesced`, 布尔变量表示系统是否暂停。

`bool _isDestroyed`, 布尔变量表示EDUMgr是否销毁。

#### 类的成员方法

`size():unsigned int`, 获取线程池大小。

线程管理单元类内的线程池作为公共资源被客户端的线程访问，需要使用共享锁(用于读数据多)`ossSLatch _mutex`。读取完数据后释放锁。

```cpp
unsigned int size()
{
    unsigned int num = 0;
    _mutex.get_shared();    // 拿到共享锁
    num = (unsigned int)_runQueue.size() + 
        (unsigned int)_idleQueue.size();
    _mutex.release_shared();
    return num;
}
```

与`size()`方法类似的方法还有：`sizeRun()`, `sizeIdle()`, `sizeSystem()`.

##### EDU线程执行方法入口

新建的EDU线程单元对应一个需要执行的任务函数，需要把这个函数入口提供给EDU。

获取指定TYPE类型的方法：

在EDB系统中，将EDU分为不同的TYPE，每种TYPE执行的任务函数不同。

本系统中设计一个数据结构`_eduEntryInfo`, 结构体内有三个成员：EDU类型 type，EDU注册结构 regResult， EDU的入口函数 entryFunc。静态常量entry[]里保存不同TYPE的`_eduEntryInfo`, 在外部调用`getEntryFuncByType()`时就可以根据传入的type参数来获取对应的入口函数。

这里返回的是函数指针，为了表示方便以及便于阅读，重新定义类型 `pmdEntryPoint`

```cpp
// 函数指针
typedef int (*pmdEntryPoint) (pmdEDUCB*, void *);

struct _eduEntryInfo
{
   EDU_TYPES     type ;
   int           regResult ;
   pmdEntryPoint entryFunc ;
};

#define ON_EDUTYPE_TO_ENTRY1(type, system, entry, desp) \
    {type, registerEDUName(type, desp, system), entry}

pmdEntryPoint getEntryFuncByType(EDU_TYPES type)
{
    pmdEntryPoint rt = NULL;
    static const _eduEntryInfo entry[] = {
        ON_EDUTYPE_TO_ENTRY1(EDU_TYPE_AGENT, false,
                            pmdAgentEntryPoint,
                            "Agent"),
        ON_EDUTYPE_TO_ENTRY1(EDU_TYPE_TCPLISTENER, true,
                            pmdTcpListenerEntryPoint,
                            "TCPListener"),
        ON_EDUTYPE_TO_ENTRY1(EDU_TYPE_MAXIMUM, false,
                            NULL,
                            "Unkown")
    };

    static const unsigned int number = sizeof(entry)/sizeof(_eduEntryInfo);

    unsigned int index = 0;
    for(;index < number; ++index)
    {
        if(entry[index].type == type)
        {
            rt = entry[index].entryFunc;
            goto done;
        }
    }

done:
    return rt;
error:
    goto done;
}
```

线程执行函数入口设计：

布尔变量`eduDestroyed`表示当前线程单元EDU是否还在执行过程中，整个入口函数就是在死循环中进行，死循环结束的标志则是当前EDU是否要被销毁。

利用当前EDU的控制块来接收外部发送的事件，控制块内有一个事件队列，控制块的waitEvent()函数就是一直等待事件队列能够pop出一个事件来进行处理。

在EDU执行过程中遇到控制块的isForced()函数后，表示外部强行终止该EDU。

接收到事件后判断事件类型:

RESUME: 表示线程重新被启用。设置当前EDU处于暂停状态(调用waitEDU函数)，根据线程单元控制块CB的EDU_TYPE获取EDU的执行入口函数，如果没有得到入口函数，系统down。

TERM：表示结束EDU，置isForced为true。

执行入口函数，并传入参数CB和事件Event对应的数据`_Data`.

继续等待下一个事件Event。

若接收到事件内参数`_release`为true，表示释放数据内存空间。

```cpp
int pmdEDUEntryPoint(EDU_TYPES type, pmdEDUCB *cb, void *arg)
{
    int rc = EDB_OK;
    EDUID eduID = cb->getID();
    pmdEDUMgr *eduMgr = cb->getEDUMgr();
    pmdEDUEvent event;
    bool eduDestroyed = false;
    bool isForced = false;
    // main loop
    while(!eduDestroyed)
    {
        type = cb->getType();
        // wait for 1000 milliseconds for events
        if(!cb->waitEvent(event, 1000))
        {
            if(cb->isForced())
            {
                PD_LOG(PDEVENT, "EDU %lld is forced", eduID);
                isForced = true;
            }
            else
            {
                continue;
            }
        }
        if(!isForced && PMD_EDU_EVENT_RESUME == event._eventType)
        {
            // set EDU status to wait
            eduMgr->waitEDU(eduID);
            // run the main function
            pmdEntryPoint entryFunc = getEntryFuncByType(type);
            if(!entryFunc)
            {
                PD_LOG(PDERROR, "EDU %lld type %d entry point func is NULL",
                    eduID, type);
                EDB_SHUTDOWN_DB
                rc = EDB_SYS;
            }
            else
            {
                rc = entryFunc(cb, event._Data);
            }
            // sanity check
            if(EDB_IS_DB_UP)
            {
                // system EDU should never exit when db is still runnig
                if(isSystemEDU(cb->getType()))
                {
                    PD_LOG(PDSEVERE, "System EDU: %lld, type %s, exits with %d",
                            eduID, getEDUName(type), rc);
                    EDB_SHUTDOWN_DB
                }
                else if(rc)
                {
                    PD_LOG(PDWARNING, "EDU %lld, type %s, exits with %d",
                            eduID, getEDUName(type), rc);
                }
            }
            eduMgr->waitEDU(eduID);
        }
        else if(!isForced && PMD_EDU_EVENT_TERM != event._eventType)
        {
            PD_LOG(PDERROR, "Receive the wrong event %d in EDU %lld, type %s",
                    event._eventType, eduID, getEDUName(type));
            rc = EDB_SYS;
        }
        else if(isForced && PMD_EDU_EVENT_TERM == event._eventType)
        {
            PD_LOG(PDEVENT, "EDU %lld, type %s is forced", eduID, type);
            isForced = true;
        }
        // release the event data
        if(!isForced && event._Data && event._release)
        {
            free(event._Data);
            event.reset();
        }
        rc = eduMgr->returnEDU(eduID, isForced, &eduDestroyed);
        if(rc)
        {
            PD_LOG(PDERROR, "Invalid EDU status for EDU: %lld, type %s",
                    eduID, getEDUName(type));
        }
        PD_LOG(PDDEBUG, "Terminating threads for EDU: %lld, type %s",
                    eduID, getEDUName(type));
    }
return 0;
}
```



##### 创建新线程EDU

创建线程属于管理者EDU的私有成员方法，传入参数为线程类型、缺省参数、生成的EDU的id指针。

首先要判断当前系统是否暂停，暂停状态下直接停止操作。

判断能否根据type能够获取到线程单元入口函数。

创建EDU的控制块CB, `new pmdEDUCB ( this, type ) `.

利用控制块CB设置当前正在创建的EDU状态为CREATING。

pmdEDUMgr维护着一个自递增的`_EDUID`，最新的`_EDUID`是没有被分配EDU的，要检查`_runQueue`和`_idleQueue`两个线程队列是否占用了这个`_EDUID`.

`_EDUID`检查完毕后，给控制核CB分配EDU的ID，即调用CB的`setID()`方法。并将该EDU保存到`_runQueue`线程队列，意味着EDU开始运行。上述两个步骤读取并修改了线程队列，所以在前后应该申请锁并释放锁。

使用`boost::thread`创建新的线程，传入的参数有线程执行函数`pmdEDUEntryPoint`以及执行函数的参数；若在这一过程中出错，则删除`_runQueue`队列中登记的id。

最后postEvent(RESUME)表示启动上述创建的线程执行任务。

```cpp
int pmdEDUMgr::_createNewEDU ( EDU_TYPES type, void* arg, EDUID *eduid )
{
   int rc               = EDB_OK ;
   unsigned int probe   = 0 ;
   pmdEDUCB *cb         = NULL ;
   EDUID myEDUID        = 0 ;
   if ( isQuiesced () )
   {
      rc = EDB_QUIESCED ;
      goto done ;
   }

   if ( !getEntryFuncByType ( type ) )
   {
      PD_LOG ( PDERROR, "The edu[type:%d] not exist or function is null", type ) ;
      rc = EDB_INVALIDARG ;
      probe = 30 ;
      goto error ;
   }
   // 在内存不足时，new (std::nothrow)并不抛出异常，而是将指针置NULL。 
   cb = new(std::nothrow) pmdEDUCB ( this, type ) ;
   EDB_VALIDATE_GOTOERROR ( cb, EDB_OOM,
            "Out of memory to create agent control block" )
   // set to creating status
   cb->setStatus ( PMD_EDU_CREATING ) ;

   /***********CRITICAL SECTION*********************/
   _mutex.get () ;
   // if the EDU exist in runqueue
   if ( _runQueue.end() != _runQueue.find ( _EDUID )  )
   {
      _mutex.release () ;
      rc = EDB_SYS ;
      probe = 10 ;
      goto error ;
   }
   // if the EDU exist in idle queue
   if ( _idleQueue.end() != _idleQueue.find ( _EDUID )  )
   {
      _mutex.release () ;
      rc = EDB_SYS ;
      probe = 15 ;
      goto error ;
   }
   // assign EDU id and increment global EDUID
   cb->setID ( _EDUID ) ;
   if ( eduid )
      *eduid = _EDUID ;
   // place cb into runqueue
   _runQueue [ _EDUID ] = ( pmdEDUCB* ) cb ;
   myEDUID = _EDUID ;
   ++_EDUID ;
   _mutex.release () ;
   /***********END CRITICAL SECTION****************/

   // create a new thread here, pass agent CB and other arguments
   try
   {
      boost::thread agentThread ( pmdEDUEntryPoint,
                                  type, cb, arg ) ;
      // detach the agent so that he's all on his own
      // we only track based on CB
      agentThread.detach () ;
   }
   catch ( std::exception e )
   {
      // if we failed to create thread, make sure to clean runqueue
      _runQueue.erase ( myEDUID ) ;
      rc = EDB_SYS ;
      probe = 20 ;
      goto error ;
   }

   //The edu is create, need post a resum event
   cb->postEvent( pmdEDUEvent( PMD_EDU_EVENT_RESUME, false, arg ) ) ;

done :
   return rc ;
error :
   // clean out memory if it's allocated
   if ( cb )
      delete cb ;
   PD_LOG ( PDERROR, "Failed to create new agent, probe = %d", probe ) ;
   goto done ;
}
```

##### 启动新线程EDU

启动指定EDU类型(`EDU_TYPE`)的一个新线程EDU单元。并返回线程ID

由于要访问线程队列，所以获取锁。首先判断线程池是否存在空闲的线程，即`_idleQueue`队列中是否存在怠性线程，若存在则获取线程ID以及线程控制块CB，并把移除`_idleQueue`队列，根据传入的参数type设置新建的线程类型，设置新建的线程状态为WAITING，注册到`_runQueue`队列，释放锁，最后发送事件RESUME，表示线程启动；若线程池不存在空闲的线程，释放锁，调用私有函数`_createNewEDU()`，从系统新建一个EDU。

```
// get an EDU from idle pool, if idle is empty, create new one
int pmdEDUMgr::startEDU(EDU_TYPES type, void *arg, EDUID *eduid)
{
    int rc = EDB_OK;
    EDUID eduID = 0;
    pmdEDUCB * eduCB = NULL;
    std::map<EDUID, pmdEDUCB *>::iterator it;
    if(isQuiesced())
    {
        rc = EDB_QUIESCED;
        goto done;
    }
    // get exclusive latch, we don't latch the entire function,
    // in order to avoid creating new thread while holding latch
    _mutex.get();
    // if there is any pooled EDU?
    // or is the request type can be pooled
    // 判断是不是可以从线程池拿
    if(true == _idleQueue.empty() || !isPoolable(type))
    {
        // note that EDU types other than "agent" shouldn't be pooled at all
        // release latch before calling creat NEW EDU;
        _mutex.release();
        rc = _createNewEDU(type, arg, eduid);
        if(EDB_OK == rc)
            goto done;
        goto error;
    }

    // if we can find something in idle queue, let's get the first of it
    for(it = _idleQueue.begin();
            (_idleQueue.end() != it) && 
            (PMD_EDU_IDLE != (*it).second->getStatus());
                ++it)
        ;
    //  if everything in idleQueue are in DESTROY status, we still need to create
    // a new EDU
    if(_idleQueue.end() == it)
    {
        // create a new
        // release latch before calling createNewEDU
        _mutex.release();
        rc = _createNewEDU(type, arg, eduid);
        if(EDB_OK == rc)
            goto done;
        goto error;
    }

    // now "it" is pointing to an idle EDU
    // note that all EDUs in the idleQueue should be AGENT type
    eduID = (*it).first;
    eduCB = (*it).second;
    _idleQueue.erase(eduID);
    EDB_ASSERT(isPoolable(type), "must be agent.");

    // switch agent type for the EDU(call different agent entry point)
    eduCB->setType(type);
    eduCB->setStatus(PMD_EDU_WAITING);
    _runQueue[eduID] = eduCB;
    *eduid = eduID;
    _mutex.release();
    // the EDU start, need post a resume event
    eduCB->postEvent(pmdEDUEvent(PMD_EDU_EVENT_RESUME, false, arg));

done:
    return rc;
error:
    goto done;
}
```

##### 暂停线程EDU

##### 销毁线程EDU



## 服务端监听程序

作为服务端监听程序，提供给客户端建立连接的socket通信服务。

设置一个宏：`PMD_TCPLISTENER_RETRY` 作为程序重启最大次数，启动过程中可能遇到的问题有：socket连接超时和代理线程启动失败。

由主程序核KRCB获取连接参数：svcname，port。

根据port创建ossSocket对象，服务端创建的socket不必提供主机地址，只需提供port。随后初始化ossSocket对象，并将其处于绑定监听状态，等待客户端socket接入。

激活监听程序的线程，即调用控制块CB的activateEDU()函数，使线程EDU处于RUNNING状态。

在数据库运行过程中保持执行：

服务端socket执行accept()函数，并传入描述符s以记录该函数新生成的socket描述符。

若接收过程中返回EDB_TIMEOUT错误，重新开始。

若数据库已停止运行，停掉程序，goto  done。

启动Agent代理线程，处理客户端发送来的命令，并返回data。

上述过程中出错后记录日志，并及时关闭新生成的socket。

```cpp
int pmdTcpListenerEntryPoint(pmdEDUCB *cb, void *arg)
{
    int rc                  = EDB_OK;
    pmdEDUMgr * eduMgr      = cb->getEDUMgr();
    EDUID eduID             = cb->getID();
    unsigned int retry      = 0;
    EDUID agentEDU          = PMD_INVALID_EDUID;
    char                    svcName[OSS_MAX_SERVICENAME + 1];

    while(retry <= PMD_TCPLISTENER_RETRY && !EDB_IS_DB_DOWN)
    {
        retry++;

        strcpy(svcName, pmdGetKRCB()->getSvcName() );
        PD_LOG(PDEVENT, "Listening on port_test: %s\n", svcName);

        int port = 0;
        int len = strlen(svcName);
        for (int i = 0; i < len; ++i)
        {
            if(svcName[i] >= '0' && svcName[i] <= '9')
            {
                port = port*10;
                port += int(svcName[i]-'0');
            }
            else
            {
                PD_LOG(PDERROR, "service name error!");
            }
        }

        ossSocket sock(port);
        rc = sock.initSocket();
        EDB_VALIDATE_GOTOERROR(EDB_OK==rc, rc, "Failed initialize socket");

        rc = sock.bind_listen();
        EDB_VALIDATE_GOTOERROR(EDB_OK==rc, rc, "Failed to bind_listen socket");

        // once bind is successful, set the state of EDU to RUNNING
        if(EDB_OK != (rc = eduMgr->activateEDU(eduID)) )
        {
            goto error;
        }
        // master loop for tcp listener
        while(!EDB_IS_DB_DOWN)
        {
            int s;
            // accept() default timeout=10ms, 
            rc = sock.accept(&s, NULL, NULL);
            // get nothing for a period of time, loop
            if(EDB_TIMEOUT == rc)
            {
                rc = EDB_OK;
                continue;
            }
            // receive error due to database down
            if(rc && EDB_IS_DB_DOWN)
            {
                rc = EDB_OK;
                goto done;
            }
            else if(rc)
            {
                // error, restart socket
                PD_LOG(PDERROR, "Failed to accept socket in TCP listener");
                PD_LOG(PDEVENT, "Restarting socket to listen");
                break;
            }

            void *pData = NULL;
            *((int *) &pData) = s;

            // 启动代理线程 用于处理客户端发送的请求
            rc = eduMgr->startEDU(EDU_TYPE_AGENT, pData, &agentEDU);
            if(rc)
            {
                if(rc == EDB_QUIESCED)
                {
                    // we cannot start EDU due to quiesced
                    PD_LOG(PDWARNING, "reject new connection due to quiesced database");

                }
                else
                {
                    PD_LOG(PDERROR, "Failed to start EDU agent");
                }
                // close remote connection if we cannot create new thread
                ossSocket newsock(&s);
                newsock.close();
                continue;
            }
        }
        if(EDB_OK != (rc = eduMgr->waitEDU(eduID)) )
        {
            goto error;
        }
    }   // while(retry <= PMD_TCPLISTENER_RETRY)

done:
    return rc;
error:
    switch(rc)
    {
        case EDB_SYS:
            PD_LOG(PDSEVERE, "System error occured");
            break;
        default:
            PD_LOG(PDSEVERE, "Internal error");
    }
    goto done;
}
```

## 代理程序处理命令

客户端发送到服务端的消息是按照本系统设定的协议格式化的，socket在接收到消息后，系统申请一块缓冲区存放接收到的数据内容，有代理程序处理这些内容，一是识别命令并调用对应命令的函数方法以执行，二是将发送的命令按照协议写入缓冲区返回replyData。

代理程序依然使用EDU单元控制，入口函数为pmdAgentEntryPoint()。

首先申请一块char类型，大小为receiveBufferSize（EDB系统宏定义为4096bytes）的内存空间，用于存储接收到的数据内容，其指针为pReceiveBuffer；再申请一块char类型，大小为resultBufferSize (EDB系统宏定义为一个消息包数据结构MsgReply占用的字节数)的内存空间，用于存储将要返回给客户端的数据内容。申请存储空间失败报EDB_OOM内存溢出的错误。

设置一个标记disconnect，表示程序是否遇到终止命令。在未终止期间，执行：

服务端接收消息：先接收数据长度，再接收数据内容。

判断是否系统终止该程序，若是则退出；否则接收数据长度。

执行socket对象的recv()函数，并传入消息缓冲区指针，即pReceiveBuffer，用于存储消息内容。

接收到数据长度后判断是否合法以及申请的receiveBufferSize是否足够，不足则重新申请足够大小的内存空间，并赋予指针pReceiveBuffer，最后修改新申请空间的前int位表示的数据包长度。

接收数据内容，并激活该代理程序的线程状态activateEDU()。

调用代理线程的处理函数pmdProcessAgentRequest，处理接收到的命令。

若处理完消息后discount标记为false，发送缓冲区待发送的返回消息，并从头开始执行本函数；否则退出，出错后报错。

处理消息的函数pmdProcessAgentRequest（）：

接收的参数：

```cpp
char *pReceiveBuffer,	// 接收到数据后存储消息的内存空间指针
int packetSize,			// 接收到的数据字节长度
char **ppResultBuffer,	// 指向一个指向结果存储空间的指针的指针
int *pResultBufferSize, // 存储空间大小的指针
bool *disconnect,		// 是否断开代理程序线程的标记
pmdEDUCB *cb			// 代理程序控制块
```



```cpp
int pmdRecv(char *pBuffer, int recvSize, ossSocket *sock, pmdEDUCB *cb)
{
    int rc = EDB_OK;
    EDB_ASSERT(sock, "Soket is NULL");
    EDB_ASSERT(cb, "cb is NULL");
    while(true)
    {
        if(cb -> isForced())
        {
            rc = EDB_APP_FORCED;
            goto done;
        }
        rc = sock->recv(pBuffer, recvSize);
        if(EDB_TIMEOUT == rc)
        {
            continue;
        }
        goto done;
    }
done:
    return rc;
error:
    goto done;
}
```



