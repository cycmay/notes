# boost::program_options 用法详解



## **简介**

program options是一系列pair<name,value>组成的选项列表,它允许程序通过命令行或配置文件来读取这些参数选项.

## **主要组件**

program_options的使用主要通过下面三个组件完成：

| 组件名                          | 作用                         |
| ------------------------------- | ---------------------------- |
| options_description(选项描述器) | 描述当前的程序定义了哪些选项 |
| parse_command_line(选项分析器)  | 解析由命令行输入的参数       |
| variables_map(选项存储器)       | 容器,用于存储解析后的选项    |

## **代码流程**

1.  构造option_description对象和variables_map对象
2.  add_options()->向option_description对象添加选项
3.  parse_command_line()->将命令行输入的参数解析出来
4.  store()->将解析出的选项存储至variables_map中
5.  notify()->通知variables_map去更新所有的外部变量
6.  count()->检测某个选项是否被输入
7.  operator[]->取出选项的值

## **示例一**

下面的代码是boost::program_options的一个简单的用法示例.
该示例中指定了两个选项,分别是–-help和–-filename.

//linux系统下,编译选项需加上 -lboost_program_options

```c++
# include <iostream>

# include <string>

# include <boost/program_options.hpp>

namespace  bpo = boost::program_options;

int main(int argc, char const *argv[])
{
 //步骤一: 构造选项描述器和选项存储器
 //选项描述器,其参数为该描述器的名字
 bpo::options_description opts("all options"); 
 //选项存储器,继承自map容器
 bpo::variables_map vm;
 //步骤二: 为选项描述器增加选项
 //其参数依次为: key, value的类型，该选项的描述
 opts.add_options()  
 ("filename", bpo::value<std::string>(), "the file name which want to be found")
 ("help", "this is a program to find a specified file");

 //步骤三: 先对命令行输入的参数做解析,而后将其存入选项存储器
 //如果输入了未定义的选项，程序会抛出异常，所以对解析代码要用try-catch块包围
 try{
     //parse_command_line()对输入的选项做解析
     //store()将解析后的结果存入选项存储器
     bpo::store(bpo::parse_command_line(argc, argv, opts), vm);
 }
 catch(...){
     std::cout << "输入的参数中存在未定义的选项！\n";
     return 0;
 }

 //步骤四: 参数解析完毕，处理实际信息
 //count()检测该选项是否被输入
 if(vm.count("help") ){//若参数中有help选项
     //options_description对象支持流输出， 会自动打印所有的选项信息
     std::cout << opts << std::endl;   
 }
 if(vm.count("filename") ){
     //variables_map(选项存储器)是std::map的派生类,可以像关联容器一样使用,
     //通过operator[]来取出其中的元素.但其内部的元素类型value_type是boost::any,
     //用来存储不确定类型的参数值,必须通过模板成员函数as<type>()做类型转换后,
     //才能获取其具体值.
     std::cout << "find " << vm["filename"].as<std::string>() << std::endl;
 }
 if(vm.empty() ){
     std::cout << "no options found \n";
 }
 return 0;
}
```

在编译后(`g++ -I../boost -L../boost/stage/lib ./test.cpp -o test -lboost_program_options`)
输入

./test --help

则其输出为:

all options:
--filename arg        the file name which want to be found
--help                this is a program to find a specified file

输入

./test --filename sd

则其输出为

find sd

若不指定选项,即输入

./test

则输出为

no options found

## 示例二

下面的这个示例主要用来说明外部变量，参数默认值以及一个选项对应多个值的情况
这段代码主要有四个选项：

–-apple ： 苹果的数量
–-orange：橘子的数量
–-address：水果的生产地，可指定多个生产地
–-help： 打印帮助信息

///////////////////////////////////////////
//计算橘子和苹果的总数量，可以指定多个生产地    //
//编译选项加上 -lboost_program_options     //
///////////////////////////////////////////

```c++
# include <iostream>  

# include <vector>  

# include <string>  

# include <boost/program_options.hpp>  

namespace bpo = boost::program_options;  

int main(int argc, char const *argv[])  
{  
 //外部变量，用于保存获取的参数值  
 int apple_num = 0, orange_num = 0;  
 std::vector<std::string> addr;  
 bpo::options_description opt("all options");  
 opt.add_options()  
 //指定该参数的默认值 
 // "apple,a" : 指定选项的全写形式为 --apple, 简写形式为 -a
 //value<type>(ptr) : ptr为该选项对应的外部变量的地址, 当该选项被解析后, 
 //可通过下面的notify()函数将选项的值赋给该外部变量,该变量的值会自动更新
 //defaut_value(num) : num为该选项的默认值, 若命令行中未输入该选项, 则该选项的值取为num
 ("apple,a", bpo::value<int>(&apple_num)->default_value(10), "苹果的数量")  
 ("orange,o", bpo::value<int>(&orange_num)->default_value(20), "橘子的数量")  
 //该参数的实际类型为vector,所以命令行中输入的值可以是多个,
 //multitoken()的作用就是告诉编译器,该选项可接受多个值  
 ("address", bpo::value<std::vector<std::string> >()->multitoken(), "生产地")  
 ("help", "计算苹果和橘子的总数量");  

 bpo::variables_map vm;  

 try{  
     bpo::store(parse_command_line(argc, argv, opt), vm);  
 }  
 catch(...){  
     std::cout << "输入的参数中存在未定义的选项！\n";  
     return 0;  
 }  
 //参数解析完成后，通知variables_map去更新所有的外部变量
 //这句话执行后, 会使得apple_num和orange_num的值自动更新为选项指定的值   
 bpo::notify(vm);  

 if(vm.count("help") ){  
     std::cout << opt << std::endl;  
     return 0;  
 }  
 if(vm.count("address") ){  
     std::cout << "生产地为：";  
     //遍历选项值  
     for(auto& str : vm["address"].as<std::vector<std::string> >() )  
         std::cout << str << " ";  
     std::cout << std::endl;   
 }  
 std::cout << "苹果的数量:" << apple_num << std::endl;  
 std::cout << "橘子的数量:" << orange_num << std::endl;  
 std::cout << "总数量数量:" << orange_num + apple_num << std::endl;  
 return 0;  
}
```

输入

./a.out –-help

输出

all options:
-a [ –apple ] arg (=10) 苹果的数量
-o [ –orange ] arg (=20) 橘子的数量
–address arg 生产地
–help 计算苹果和橘子的总数量

指定苹果和橘子的数量:

./a.out –-apple=8 –-orange=20

其输出为:

苹果的数量:8
橘子的数量:20
总数量数量:28

仅指定橘子的数量,不指定苹果的数量:

./a.out –-orange=20

其输出为:

苹果的数量:10
橘子的数量:20
总数量数量:30

可以看到,由于没有输入苹果的数量,所以输出的苹果的数量为我们指定的默认值

指定一个生产地:

./a.out –-apple=8 –-orange=20 –-address=山东

输出:

生产地为：山东
苹果的数量:8
橘子的数量:20
总数量数量:28

指定多个生产地:

./a.out –-apple=8 –-orange=20 –-address=山东 陕西

输出为

生产地为：山东 陕西
苹果的数量:8
橘子的数量:20
总数量数量:28

简写形式的输入:

./a.out -a 8 -o 20 –-address=山东

输出:

生产地为：山东
苹果的数量:8
橘子的数量:20
总数量数量:28

