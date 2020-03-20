# strcpy和strdup比较和详解

函数和功能描述：

①extern char *strdup(char *s);

头文件：string.h

功能: 将串拷贝到新建的位置处

说 明：strdup不是标准的c函数。strdup()在内部调用了malloc()为变量分配内存，不需要使用返回的字符串时，需要用free()释放相应的内存空间，否则会造成内存泄漏。
返回值：返回一个指针,指向为复制字符串分配的空间;如果分配空间失败,则返回NULL值。

 ```c
 // strdup.c
#include <syslib.h>
#include <string.h>
int main() 
{ 
	char *s="Golden Global View";
	char *d;
 	d=strdup(s);
	printf("%s",d);
	return 0;
}
 ```

②char *strcpy(char* dest, const char *src);

头文件：#include <string.h>和 #include <stdio.h>

功能：把从src地址开始且含有NULL结束符的字符串复制到以dest开始的地址空间

说明：strcpy是标准的C语言标准库函数。src和dest所指内存区域不可以重叠且dest必须有足够的空间来容纳src的字符串。

返回值：返回指向dest的指针。

 举例：

```cpp
 // strcpy.c     
 #include <syslib.h>
 # include <string.h>     
 main()
 {        
	char *s="Golden Global View";
	char d[20]; 
	printf("%s",d);       

	return 0;   
  }
```

总结：

1.strdup不是标准的c函数，strcpy是标准的c函数，使用时注意场合。
2.strdup可以直接把要复制的内容复制给没有初始化的指针，因为它会自动分配空间给目的指针，strcpy的目的指针一定是已经分配内存的指针。
3.strdup用完要free()函数释放内存，否则内存泄露 。
4.使用strcpy必须事先确定src大小，可以先strlen判断src的大小，之后为dest申请空间，之后再strcpy就不会有问题了。

------

# C语言mmap()函数：建立内存映射



头文件：#include <unistd.h>    #include <sys/mman.h>

定义函数：void *mmap(void *start, size_t length, int prot, int flags, int fd, off_t offsize);

函数说明：mmap()用来将某个文件内容映射到内存中，对该内存区域的存取即是直接对该文件内容的读写。

参数说明：

| 参数   | 说明                                                         |
| ------ | ------------------------------------------------------------ |
| start  | 指向欲对应的内存起始地址，通常设为NULL，代表让系统自动选定地址，对应成功后该地址会返回。 |
| length | 代表将文件中多大的部分对应到内存。                           |
| prot   | 代表映射区域的保护方式，有下列组合：PROT_EXEC  映射区域可被执行；PROT_READ  映射区域可被读取；PROT_WRITE  映射区域可被写入；PROT_NONE  映射区域不能存取。 |
| flags  | 会影响映射区域的各种特性：MAP_FIXED  如果参数 start 所指的地址无法成功建立映射时，则放弃映射，不对地址做修正。通常不鼓励用此旗标。MAP_SHARED  对应射区域的写入数据会复制回文件内，而且允许其他映射该文件的进程共享。MAP_PRIVATE  对应射区域的写入操作会产生一个映射文件的复制，即私人的"写入时复制" (copy on write)对此区域作的任何修改都不会写回原来的文件内容。MAP_ANONYMOUS  建立匿名映射，此时会忽略参数fd，不涉及文件，而且映射区域无法和其他进程共享。MAP_DENYWRITE  只允许对应射区域的写入操作，其他对文件直接写入的操作将会被拒绝。MAP_LOCKED  将映射区域锁定住，这表示该区域不会被置换(swap)。 在调用mmap()时必须要指定MAP_SHARED 或MAP_PRIVATE。 |
| fd     | open()返回的文件描述词，代表欲映射到内存的文件。             |
| offset | 文件映射的偏移量，通常设置为0，代表从文件最前方开始对应，offset必须是分页大小的整数倍。 |

返回值：若映射成功则返回映射区的内存起始地址，否则返回MAP_FAILED(-1)，错误原因存于errno 中。

错误代码：

-   EBADF  参数fd 不是有效的文件描述词。
-   EACCES  存取权限有误。如果是MAP_PRIVATE 情况下文件必须可读，使用MAP_SHARED 则要有PROT_WRITE 以及该文件要能写入。
-   EINVAL  参数start、length 或offset 有一个不合法。
-   EAGAIN  文件被锁住，或是有太多内存被锁住。
-   ENOMEM  内存不足。

范例：利用mmap()来读取/etc/passwd 文件内容。

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
main(){
    int fd;
    void *start;
    struct stat sb;
    fd = open("/etc/passwd", O_RDONLY); /*打开/etc/passwd */
    fstat(fd, &sb); /* 取得文件大小 */
    start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(start == MAP_FAILED) /* 判断是否映射成功 */
        return;
    printf("%s", start); munmap(start, sb.st_size); /* 解除映射 */
    close(fd);
}
```

执行结果：

root : x : 0 : root : /root : /bin/bash

bin : x : 1 : 1 : bin : /bin :

daemon : x : 2 : 2 :daemon : /sbin

adm : x : 3 : 4 : adm : /var/adm :

lp : x :4 :7 : lp : /var/spool/lpd :

sync : x : 5 : 0 : sync : /sbin : bin/sync :

shutdown : x : 6 : 0 : shutdown : /sbin : /sbin/shutdown

halt : x : 7 : 0 : halt : /sbin : /sbin/halt

mail : x : 8 : 12 : mail : /var/spool/mail :

news : x :9 :13 : news : /var/spool/news :

uucp : x :10 :14 : uucp : /var/spool/uucp :

operator : x : 11 : 0 :operator : /root:

games : x : 12 :100 : games :/usr/games:

gopher : x : 13 : 30 : gopher : /usr/lib/gopher-data:

ftp : x : 14 : 50 : FTP User : /home/ftp:

nobody : x :99: 99: Nobody : /:

xfs :x :100 :101 : X Font Server : /etc/xll/fs : /bin/false

gdm : x : 42 :42 : : /home/gdm: /bin/bash

kids : x : 500 :500 :/home/kids : /bin/bash

# C语言munmap()函数：解除内存映射

头文件：#include <unistd.h>       #include <sys/mman.h>

定义函数：int munmap(void *start, size_t length);

函数说明：munmap()用来取消参数start 所指的映射内存起始地址，参数length 则是欲取消的内存大小。当进程结束或利用exec 相关函数来执行其他程序时，映射内存会自动解除，但关闭对应的文件描述词时不会解除映射。

返回值：如果解除映射成功则返回0，否则返回－1。错误原因存于errno 中错误代码EINVAL参数 start 或length 不合法。

# equal_range用法

equal_range是C++ STL中的一种二分查找的算法，试图在已排序的[first,last)中寻找value，它返回一对迭代器i和j，其中i是在不破坏次序的前提下，value可插入的第一个位置（亦即lower_bound），j则是在不破坏次序的前提下，value可插入的最后一个位置（亦即upper_bound），因此，[i,j)内的每个元素都等同于value，而且[i,j)是[first,last)之中符合此一性质的最大子区间

   如果以稍许不同的角度来思考equal_range,我们可把它想成是[first,last)内"与value等同"之所有元素形成的区间A，由于[fist,last)有序（sorted），所以我们知道"与value等同"之所有元素一定都相邻，于是，算法lower_bound返回区间A的第一个迭代器，算法upper_bound返回区间A的最后一个元素的下一个位置，算法equal_range则是以pair的形式将两者都返回
   即使[fist,last)并未含有"与value等同"之任何元素，以上叙述仍然合理，这种情况下，"与value等同"之所有元素形成的，其实是一个空区间，在不破坏次序的情况下，只有一个位置可以插入value，而equal_range所返回的pair，其第一和第二（都是迭代器）皆指向该位置。

```cpp
// map::equal_elements
#include <iostream>
#include <map>
using namespace std;

int main ()
{
  map<char,int> mymap;
  pair<map<char,int>::iterator,map<char,int>::iterator> ret;

  mymap['a']=10;
  mymap['b']=20;
  mymap['c']=30;

  ret = mymap.equal_range('b');

  cout << "lower bound points to: ";
  cout << ret.first->first << " => " << ret.first->second << endl;

  cout << "upper bound points to: ";
  cout << ret.second->first << " => " << ret.second->second << endl;

  return 0;
}

```

运行结果：

1.  lower bound points to: 'b' => 20
2.  upper bound points to: 'c' => 30

------

1.  sdf
2.  dsfd