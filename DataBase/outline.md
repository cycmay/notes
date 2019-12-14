# C++数据库引擎开发入门教程

通过 15 次课，约 30 个课时，搭建起一个 NoSQL数据库，让大家明白真实的数据库的工作原理，并在最后编写出一个基于Java的驱动。

本课程为原先《数据库引擎开发》课程第二版改版，增加了大家较为感兴趣的“SQL优化器开发”部分，课程名也改成《数据库引擎与 SQL 优化器开发》。SQL 优化器设计是任何数据库的核心技术，加入这部分内容后，本课程所研发的数据库已经超出原有范畴，迥然变身成当前最时尚的 NewSQL 数据库！

中国IT行业起步较晚，加上国情和民族性格的原因，大部分人更乐于使用外国人现成的软件（大部分还是盗版），开发点小应用，系统级别的软件几乎没有胆量去挑战。在大多数国人的心目中，象大型操作系统，大型数据库系统这样的软件，就是个禁区。对于性格容易自卑，技术学习没有恒心和耐力，遇难则退的国人，根本从来没有想象过有一天，自己也有能力去挑战。君不见，PUB 上的数据库爱好者们，对于各种 Oracle 内幕，比如块构造，隐藏参数等津津乐道，对于调整好一条 SQL 使之在甲骨文的优化器下能高性能运转具有巨大的满足感成功感，仿佛自己掌握了天下最有价值的真理，驾驭了天下最有难度的技术。但对于设计，编写出这个数据库的人来说，他们看到此情此景，只好躲在一边偷偷哂笑。用数据库的称为大师，那么自己写出一个数据库的又该称为什么呢？

我们具有聪明的头脑，缺乏的是高瞻远瞩的眼光和迎难而上的勇气！现在，我们设计了一个新的课程，参与者将有机会学习怎样用C++写出一个完整的数据库软件！而这个课程几乎是免费的！课程不光培养技术，更加培养勇气！

《数据库引擎与 SQL 优化器开发》课程概述：该课程通过 15 次课，大约 30 课时介绍创建一个简易分布式文档型数据库

## 目标

创建一个Linux下简易分布式文档型数据库

语言：C++

目标代码量：1-3万行

## 数据库支持

- 多线程
- 记录的增删查
- 单条记录的原子性操作
- 持久化磁盘存储
- 分布式存储
- BSON的记录格式
- 全部查询或者_id精准查询
- _id字段的散列索引
- 不支持：
- 多层[数据结构](http://c.biancheng.net/data_structure/)（比如表空间、表等概念）
- 事务
- 高可用
- 范围查询
- 在线增删节点
- 除_id以外字段的检索
- 数据修改
- 锁
- 持久性
- SQL查询

## 学习扩展

- 深入理解NoSQL数据库的内部工作原理
- 实现一个可存取数据的分布式数据存储
- 理解文档式NoSQL数据库的适用场景
- 了解关系型数据库与非关系型的针对特性与各自的实现机制
- 暂时还没有列入本课程的内容：
- 实现关系型数据库的ACID
- 实现商用型非关系型数据库
- 保证数据的高可用性与稳定性
- SQL语法解析

课程内容：

- 第一课：NoSQL 数据库简介与环境配置
- 第二课：建立数据库连接
- 第三课：阀锁，配置，日志等
- 第四课：数据库引擎底层搭建
- 第五课：消息封装
- 第六课：数据的持久化存放
- 第七课：数据的存储结构
- 第八课：数据的插入，读取与删除
- 第九课：散列索引
- 第十课：数据库理论
- 第十一课：SQL 解析和优化
- 第十二课：Java 驱动
- 第十三课：监控、性能调优和测试
- 第十四课：Java 应用
- 第十五课：总结

## 预期收获

1. 知道怎样去设计和实现一个数据库，从而获得加入数据库软件厂商的研发团队的机会。
2. 通过课程极度熟悉 C++ 编程，特别是 [socket](http://c.biancheng.net/socket/) 编程和内存管理，这是晋升 C++ 高手的必经之路，C 语言的能力属于内功级，可以增加 IT 人的底气，无论从事系统级软件，还是底层开发，嵌入式开发，移动开发，客户端开发都能得心应手！具备源码级挑战能力的高手深受各大企业的欢迎，金饭碗指日可待！
3. 加深对数据库的理解，即使只是数据库用家也能从中获益。

# CMU15-445 教程

推荐[CMU15-445/645: Intro to Database Systems](https://link.zhihu.com/?target=http%3A//15445.courses.cs.cmu.edu/fall2017/)，个人觉得是学习数据库的一个不错的起点，特别是如果你不想自己解析SQL的话。

这门课的讲义和视频质量都很不错，教材使用的是《Database System Concepts(6th)》，project是利用SQLite的[The Virtual Table](https://link.zhihu.com/?target=https%3A//www.sqlite.org/vtab.html)机制，做一个存储后端，SQL部分就不用自己解析，实现完之后是这样用的：

启动SQLite，加载虚拟表扩展：

```text
sqlite> .load ./lib/libvtable.dylib
```

创建虚拟表：

```text
sqlite> CREATE VIRTUAL TABLE foo USING vtable('a int, b varchar(13)','foo_pk a')
```

查询：

```text
sqlite> INSERT INTO foo values(1,'hello');
sqlite> SELECT * FROM foo ORDER BY a;
a           b         
----------  ----------
1           hello   
```

project源代码提供了SQLite以及和SQLite交互的部分，自己可以专心于存储后端、ACID事务、Logging&Recovery等。使用的语言是C++，不过4个lab有点虎头蛇尾，并发控制基于tuple级的strict 2PL，但其实在访问tuple时，会锁tuple所在的内存page，logging&recovery也过于简化。不过整体来说还是相当不错的，lab代码量不大，比较适合有编程经验但没有数据库概念的人。

可惜project源代码的tar包被作者给删了....

> UPDATE: 2018-03-06
>
> We appreciate your interest in the project for 15-445/645.
>
> We removed the project file after the semester. There were some bugs in it
> and we plan on redoing the system for Fall 2018.
>
> You are more than welcome to work on the DBMS in CMU's Advanced DB class:
> [http://15721.courses.cs.cmu.edu](https://link.zhihu.com/?target=http%3A//15721.courses.cs.cmu.edu)
>
> -- Andy

感兴趣的可以clone这个[repository](https://link.zhihu.com/?target=https%3A//github.com/xingdl2007/cmu15-445)然后check out到project的第一个commit，或者私信要原始tar包。