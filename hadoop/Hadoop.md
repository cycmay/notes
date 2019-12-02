# Hadoop 实验

## 环境搭建

### vmware  主机网络配置

克隆三个虚拟机 centos

三个虚拟机网络配置为NAT模式 与宿主机组成局域网 VMwareNet8 

`192.168.129.2 #虚拟机宿主机ip地址设置`

宿主机配置 Windows10：

在无线网络连接处右键选择“属性”，选择“共享”选项卡，下拉菜单选择VMnet8。

此时，宿主机可以通过:
`ssh bicycle@192.168.129.4` 
remote 到任意一个虚拟机进行操作

## Linux 配置

### 修改hostname 主机名

`$ sudo vim /etc/hostname`

在三个主机上分别将其修改为centos_master centos_node1 centos_node2

### 修改网卡配置

查看系统网卡信息 确认网卡配置文件

`$ ifconfig`

`$ sudo vim /etc/sysconfig/network-scripts/ifcfg-ens32` 进行修改

```ini
TYPE=Ethernet                            
PROXY_METHOD=none                        
BROWSER_ONLY=no                          
BOOTPROTO=static        # 静态地址                 
IPADDR=192.168.129.3    # 3、4、5 三个主机                 
NETMASK=255.255.255.0       # netmask    
GATEWAY=192.168.129.1       # gateway             
DEFROUTE=yes                             
IPV4_FAILURE_FATAL=no                    
IPV6INIT=yes                             
IPV6_AUTOCONF=yes                        
IPV6_DEFROUTE=yes                        
IPV6_FAILURE_FATAL=no                    
IPV6_ADDR_GEN_MODE=stable-privacy        
NAME=ens32                               
UUID=1d1425a5-315f-491a-b937-f380c745cdd9
DEVICE=ens32                             
ONBOOT=yes                               
HOSTNAME=centos_master                   
DNS1=8.8.8.8                  # dns 服务器           
DNS2=8.8.8.4                             
```

### 修改虚拟机的hosts文件 

修改hosts文件的目的是映射ipv4为人可读的主机名

在master主机上修改：

`$ sudo vim /etc/hosts`

修改成如下：

```
192.168.129.3 centosmaster
192.168.129.4 centosnode1
192.168.129.5 centosnode2
```

检验是否成功：

`$ ping centosnode1`

若成功echo信息：

`64 bytes from centos_node1 (192.168.129.3): icmp_seq=1 ttl=64 time=0.423 ms`

### ssh免密通信

生成ssh-key：
`$ ssh-keygen -t rsa`

生成私钥文件和公钥文件（centos_master_sshkey.pub）

传递公钥可以使用 ssh-copy-id 指令：

   `$ ssh-copy-id -i ~/.ssh/centos_master_sshkey.pub bicycle@192.168.129.4`

其中 ssh-copy-id的说明如下：

   `$ ssh-copy-id [-f] [-n] [-i [公钥文件路径]] [-p 端口号] [-o  ssh 选项] [user@]hostname`

此操作会将本地主机的公钥的内容添加到远程主机指定用户home 路径下的.ssh/authorized_keys. node1 .ssh/authorized_keys 中，表示将公钥传递给node1. 

接下来传给node2. 并分别在node1和node2主机上进行以上操作。

Hadoop 启动时要求 ssh免密登录本机，即执行ssh localhost 不需要密码，将本机centos_master_sshkey.pub 加入到authorized_keys 文件中 ：

`$ cat ~/.ssh/centos_master_sshkey.pub >> ~/.ssh/authorized_keys   `

配置.ssh/config:

```ini
Host *      ServerAliveInterval 60
Host localhost		# 本机地址
        HostName  centosmaster
        User bicycle
        IdentityFile    ~/.ssh/centos_master_sshkey
Host centosmaster
        HostName  centosmaster
        User bicycle
        IdentityFile    ~/.ssh/centos_master_sshkey
Host centosnode1
        HostName  centosnode1
        User bicycle
        IdentityFile    ~/.ssh/centos_master_sshkey

Host centosnode2
        HostName  centosnode2
        User bicycle
        IdentityFile    ~/.ssh/centos_master_sshkey
```

检测ssh

`$ ssh bicycle@localhost`

`$ ssh bicycle@centosnode1`

## Java & Hadoop 软件安装 

### Java 

如果系统没有安装 OpenJDK，可以通过 yum 包管理器来安装:

​    `$ yum install java-1.8.0-openjdk  java-1.8.0-openjdk-devel  #安装 openjdk1.8.0`  

 默认安装位置为 /usr/lib/jvm/java-1.8.0 

 接着需要配置一下 JAVA_HOME 环境变量，为了方便，直接在 ~/.bashrc 中进行设置，最后一行添加：

​    `export JAVA_HOME=/usr/lib/jvm/java-1.8.0`

使环境变量生效：

   `$ source ~/.bashrc    # 使变量设置生效`

检查Java环境

`$ echo $JAVA_HOME  # 检验变量值`

`$ java -version`

`$ javac -version`

`$ JAVA_HOME/bin/java -version # 与直接执行 java -version 一样`

### Hadoop

下载地址：[Hadoop - bit源](http://mirror.bit.edu.cn/apache/hadoop/common/)

下载完成后，将 Hadoop 解压到 /usr/local/ 中：

`$ tar -zxf ~/下载/hadoop-2.6.5.tar.gz -C /usr/local # 解压到/usr/local目录中`

`$ cd /usr/local/       # 切换当前目录为 /usr/local 目录`

`$ mv ./hadoop-2.6.5/ ./hadoop   # 将文件夹名改为hadoop`

`$ chown -R bicycle:bicycle./hadoop   # 修改文件权限，bicycle 是当前用户名`

Hadoop 解压后即可使用，输入如下命令来检查 Hadoop 是否可用，成功则会显示 Hadoop 版本信息：

`$ cd /usr/local/hadoop     # 切换当前目录为 /usr/local/hadoop 目录`

`$ ./bin/hadoop version     # 查看 Hadoop 的版本信息`

或者直接输入 `hadoop version` 命令也可以查看。

系统环境变量，修改 ~/.bashrc :

```bash
export HADOOP_HOME=/usr/local/hadoop
export PATH=$PATH:$JAVA_HOME/bin:$HADOOP_HOME/sbin:$HADOOP_HOME/bin
export CLASSPATH=$($HADOOP_HOME/bin/hadoop classpath):$CLASSPATH
```

重新加载环境变量，在命令行中执行如下:

`$ source ~/.bashrc`

## Hadoop 配置

### Hadoop环境变量文件

hadoop-env.sh是Hadoop的环境变量配置文件，需要在该配置文件中指定JAVA_HOME的路径，Hadoop启动的hdfs、yarn会生成一个pid文件，默认存放在/tmp目录下，但是Linux重新启动后tmp会清空，而且tmp所有者为root。所以更改Hadoop 的pid为 hadoop 软件目录下的pid目录里。

`$ mkdir -P /usr/local/hadoop/pid`

`$ cd /usr/local/hadoop/etc/hadoop`
`$ sudo vim hadoop-env.sh` 

``` bash
# The java implementation to use. By default, this environment

# variable is REQUIRED on ALL platforms except OS X!

export JAVA_HOME=/usr/lib/jvm/java-1.8.0 # 修改此处

```

```bash
# Where pid files are stored.  /tmp by default.
export HADOOP_PID_DIR=/usr/local/hadoop/pid
```



### 修改Hadoop的核心配置文件

core-site.xml是Hadoop的核心配置文件，里面可以配置HDFS（Hadoop分布式文件系统）的NameNode的地址和数据存储目录

`$ sudo vim core-site.xml`

```xml
<configuration>
  <property>
      <name>fs.default.name</name>
      <value>hdfs://centosmaster:9000</value>
  </property>
<!-- 指定hadoop存储数据的目录 -->
  <property>
    <name>hadoop.tmp.dir</name>
    <!-- 手动创建tmp文件目录 -->
    <value>/bigdata/hadoop/data</value>
  </property>
</configuration>
```

### 修改HDFS的配置文件

hdfs-site.xml是HDFS的配置文件，由于当前在一台机器上配置的Hadoop伪分布式，所以这里修改HDFS的副本为1即数据只保存一份

`$ sudo vim hdfs-site.xml`

```xml
<configuration>
    <property>
        <name>dfs.replication</name>
        <value>3</value>
    </property>
    <property>
      <name>dfs.namenode.http-address</name>
      <value>centosmaster:50070</value>
    </property>
</configuration>
```

### 修改MapReduce的配置文件

mapred-site.xml里面可以配置MapReduce框架运行在YARN资源调度系统上

`$ sudo vim mapred-site.xml`

```xml
<configuration>
    <!-- 指定MapReduce运行在YARN上 -->
    <property>
        <name>mapreduce.framework.name</name>
        <value>yarn</value>
    </property>
  <property>
        <name>mapreduce.jobhistory.address</name>
        <value>centosmaster:10020</value>
    </property>
    <property>
        <name>mapreduce.jobhistory.webapp.address</name>
        <value>centosmaster:19888</value>
    </property>
</configuration>
```

### 修改YRAR的配置文件

yarn-site.xml是配置YARN的相关配置的文件

`$ sudo vim yarn-site.xml`

```xml
<configuration>
    <!-- 分别指定ResouceManager的地址 -->
    <property>
       <name>yarn.resourcemanager.hostname</name>
       <value>centosmaster</value>
    </property>
    <!-- 分别指定MapReduce的方式 -->
    <property>
       <name>yarn.nodemanager.aux-services</name>
       <value>mapreduce_shuffle</value>
    </property>
      <property>
        <name>yarn.nodemanager.aux-services.mapreduce.shuffle.class</name>
        <value>org.apache.hadoop.mapred.ShuffleHandler</value>
    </property>
    <property>
        <name>yarn.resourcemanager.address</name>
        <value>centosmaster:8032</value>
    </property>
    <property>
        <name>yarn.resourcemanager.scheduler.address</name>
        <value>centosmaster:8030</value>
    </property>
    <property>
        <name>yarn.resourcemanager.resource-tracker.address</name>
        <value>centosmaster:8031</value>
    </property>
    <property>
        <name>yarn.resourcemanager.admin.address</name>
        <value>centosmaster:8033</value>

    </property>
    <property>
        <name>yarn.resourcemanager.webapp.address</name>
        <value>centosmaster:8088</value>
    </property>
</configuration>
```



## 初始化Hadoop HDFS文件系统

### 修改Hadoop系统权限 

为了安全起见，将Hadoop软件所有者设置为bicycle，同时Hadoop使用的 /bigdata 文件所有为bicycle

`$ sudo chown -R bicycle:bicycle /usr/local/hadoop`
`$ sudo chown -R bicycle:bicycle /bigdata`

hadoop格式化时需要用户免密码登录本机，即执行`$ ssh localhost`，就需要生成默认的sshkey：
`#在.ssh/config中增加localhost`

```ini
Host localhost
        HostName  centosmaster
        User bicycle
        IdentityFile    ~/.ssh/centos_master_sshkey
```

### 格式化文件系统 

修改完配置文件后哦就可以初始化HDFS文件系统了，在命令行中执行下面的命令

`$ hdfs namenode -format`

```
#结果如下 表示成功格式化
2019-11-30 22:20:28,281 INFO common.Storage: Storage directory /bigdata/hadoop/data/dfs/name has been successfully formatted.
```

### 启动 HDFS

执行：

`$ start-dfs.sh`

out:

```
Starting namenodes on [centosmaster]
centosmaster: Warning: Permanently added 'centosmaster' (ECDSA) to the list of known hosts.
Starting datanodes
Starting secondary namenodes [centos_master]
```

执行 jps查看工作进程

`$ jps`

out:

```
12019 DataNode
11909 NameNode
12216 SecondaryNameNode
12348 Jps
```

### 启动 Yarn

执行：

`$ start-yarn.sh`

out:

```
Starting resourcemanager
Starting nodemanagers
```

执行jps查看工作进程

out:

```
12482 ResourceManager #yarn new
12019 DataNode
11909 NameNode
12216 SecondaryNameNode
12922 Jps
12573 NodeManager	# yarn new
```

### 测试Hadoop环境

关闭防火墙 CentOS 7的防火墙服务为firewalld，不再使用IPtables了

`$ sudo systemctl stop firewalld      #关闭防火墙服务网`
`$ sudo systemctl disable firewalld   #设置防火墙服务开机不启动`

1. 打开Hadoop HDFS的管理页面

   使用浏览器访问hdfs管理界面：

   192.168.129.3:50070

2. 打开Hadoop YARN的管理页面

   使用浏览器访问yarn管理界面：

   192.168.129.3:8088

停止hdfs 和 yarn 服务：

`$ hdfs-stop.sh && yarn-stop.sh`

## 集群分发Hadoop到slave

### 创建workers

在master结点 Hadoop目录下 ./etc/ 新增worker文件，并写入所有结点

`$ touch ./etc/worker`

`$ cat ./etc/worker`

out echo：

```
centosmaster
centosnode1
centosnode2
```

### 分发Hadoop

使用scp命令复制matser的 Hadoop软件

`$ scp -r /usr/local/hadoop bicycle@centosnode1:/home/bicycle/share`

在centosnode1结点配置Hadoop需要的系统环境变量，也可以直接从master拷贝~/.bashrc到node1

在centosnode1移动share目录下的hadoop软件到 /usr/local/

`$ mv /home/bicycle/share/hadoop /usr/local/ `

在结点创建Hadoop需要的交换文件目录，并更换用户和组

`$sudo mkdir -p /bigdata/hadoop/data && sudo chown -R biycle:bicycle /bigdata/hadoop/data`

### 启动集群Hadoop系统

在master结点：
`$ start-all.sh`

测试输入jps，在master结点和slave结点分别显示：

```
# master
21154 NameNode
21474 SecondaryNameNode
21815 NodeManager
21257 DataNode
21705 ResourceManager
23465 Jps
```

```
# slave
8113 Jps
7706 NodeManager
7598 DataNode
```



## 运行Mapreduce 程序

以score为例 求平均成绩

### 创建 input 目录

input是Hadoop文件系统输入的数据，执行下面的命令

`$ hdfs dfs -mkdir -p input/score`
```
要从HDFS中删除文件，可以使用以下命令：
hadoop fs -rm -r -skipTrash /path_to_file/file_name
要从HDFS中删除文件夹，可以使用以下命令：
hadoop fs -rm -r -skipTrash /folder_name
```

### 编译mapreduce程序

简单的java Hadoop MapReduce程序(计算平均成绩)从打包到提交及运行

程序源码：

```java
import java.io.IOException;
import java.util.Iterator;
import java.util.StringTokenizer;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;
public class Score {
    public static class Map extends
            Mapper<LongWritable, Text, Text, IntWritable> {
        // 实现map函数
        public void map(LongWritable key, Text value, Context context)
                throws IOException, InterruptedException {
            // 将输入的纯文本文件的数据转化成String
            String line = value.toString();
            // 将输入的数据首先按行进行分割
            StringTokenizer tokenizerArticle = new StringTokenizer(line, "\n");
            // 分别对每一行进行处理
            while (tokenizerArticle.hasMoreElements()) {
                // 每行按空格划分
                StringTokenizer tokenizerLine = new StringTokenizer(tokenizerArticle.nextToken());
                String strName = tokenizerLine.nextToken();// 学生姓名部分
                String strScore = tokenizerLine.nextToken();// 成绩部分
                Text name = new Text(strName);
                int scoreInt = Integer.parseInt(strScore);
                // 输出姓名和成绩
                context.write(name, new IntWritable(scoreInt));
            }
        }
    }
 
 
 
    public static class Reduce extends
            Reducer<Text, IntWritable, Text, IntWritable> {
        // 实现reduce函数
        public void reduce(Text key, Iterable<IntWritable> values,
                Context context) throws IOException, InterruptedException {
            int sum = 0;
            int count = 0;
            Iterator<IntWritable> iterator = values.iterator();
            while (iterator.hasNext()) {
                sum += iterator.next().get();// 计算总分
                count++;// 统计总的科目数
            }
            int average = (int) sum / count;// 计算平均成绩
            context.write(key, new IntWritable(average));
        }
    }
    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        // "localhost:9000" 需要根据实际情况设置一下
        conf.set("mapred.job.tracker", "localhost:9000");
        // 一个hdfs文件系统中的 输入目录 及 输出目录
        String[] ioArgs = new String[] { "input/score", "output" };
        String[] otherArgs = new GenericOptionsParser(conf, ioArgs).getRemainingArgs();
        if (otherArgs.length != 2) {
            System.err.println("Usage: Score Average <in> <out>");
            System.exit(2);
        }
 
        Job job = new Job(conf, "Score Average");
        job.setJarByClass(Score.class);
        // 设置Map、Combine和Reduce处理类
        job.setMapperClass(Map.class);
        job.setCombinerClass(Reduce.class);
        job.setReducerClass(Reduce.class);
        // 设置输出类型
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);
        // 将输入的数据集分割成小数据块splites，提供一个RecordReder的实现
        job.setInputFormatClass(TextInputFormat.class);
        // 提供一个RecordWriter的实现，负责数据输出
        job.setOutputFormatClass(TextOutputFormat.class);
        // 设置输入和输出目录
        FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
        FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}
```

编译

命令

`javac Score.java`

如果出现如下依赖错误:

```java
 $ javac Score.java 
Score.java:4: error: package org.apache.hadoop.conf does not exist
import org.apache.hadoop.conf.Configuration;
                             ^
Score.java:5: error: package org.apache.hadoop.fs does not exist
import org.apache.hadoop.fs.Path;
                           ^
Score.java:6: error: package org.apache.hadoop.io does not exist
import org.apache.hadoop.io.IntWritable;
                           ^
Score.java:7: error: package org.apache.hadoop.io does not exist
import org.apache.hadoop.io.LongWritable;
                           ^
Score.java:8: error: package org.apache.hadoop.io does not exist
import org.apache.hadoop.io.Text;
```

尝试修改环境变量CLASSPATH

`$ vim ~/.bashrc` 

### 创建可执行的 jar 包

打包jar文件时，可以使用java -jar命令，具体如下：

`$ jar -cvf Score.jar -C Score/ .`

-c表示创建新的jar文件，后面是要打包的class文件的路径.

创建数据集 datasource

```
# 三个文件
chinese.txt
english.txt
math.txt
$ cat chinese.txt 
Zhao 98
Qian 9
Sun 67
Li 23
$ cat english.txt 
Zhao 93
Qian 42
Sun 87
Li 54
$ cat math.txt 
Zhao 38
Qian 45
Sun 23
Li 43
```

jar 包生成后的目录结构

+-- hadoopcase
|   +-- Score
|    |    +-- Score.class 
|    |    +-- Score$Map.class
|    |    +-- Score$Reduce.class
|    |    +-- Score.java
|   +-- Score.jar
|   +-- datasource
|    |    +-- chinese.txt
|    |    +-- english.txt
|    |    +-- math.txt

### 使用Hadoop运行jar包

将数据集上传到HDFS文件系统

`$ hdfs dfs -put ./datasource/*.txt input/score`

查看文件系统input目录

`$ hadoop fs -ls input/score`

output:

```
Found 3 items
-rw-r--r--   1 bicycle supergroup         28 2019-12-01 04:38 input/score/chinese.txt
-rw-r--r--   1 bicycle supergroup         29 2019-12-01 04:38 input/score/english.txt
-rw-r--r--   1 bicycle supergroup         29 2019-12-01 04:38 input/score/math.txt
```

运行指令：

`$ hadoop jar Score.jar Score input/score output`

错误: 找不到或无法加载主类org.apache.hadoop.mapreduce.v2.app.MRAppMaster
解决办法：

在命令行下输入如下命令，并将返回的地址复制。

`$ hadoop classpath`

编辑yarn-site.xml

添加如下内容

    <property>
        <name>yarn.application.classpath</name>
        <value>输入刚才返回的Hadoop classpath路径</value>
    </property>

### 查看输出结果

生成的文件目录

`$ hdfs dfs -ls output`

output:

```
Found 2 items
-rw-r--r--   1 bicycle supergroup          0 2019-12-01 06:28 output/_SUCCESS
-rw-r--r--   1 bicycle supergroup         37 2019-12-01 06:28 output/part-r-00000
```

程序运行结果：

`$ hdfs dfs -cat output/part-r-00000`

output:

```
2019-12-01 06:30:06,705 INFO sasl.SaslDataTransferClient: SASL encryption trust check: localHostTrusted = false, remoteHostTrusted = false
Li      40
Qian    32
Sun     59
Zhao    65
zhao    98
```








