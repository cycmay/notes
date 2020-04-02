# VSCode Cmake 调试C/C++

对于编写C/C++的环境，我们至少需要有**文本编辑器**、**C/C++编译器**，最好还能有**C/C++调试器**。

`VSCode`本质上是一个**文本编辑器**，但是它有丰富的插件生态(extensions)，通过插件我们可以对C/C++程序进行调试。而且，它拥有可自定义的任务系统(task.json)，通过任务，可以封装一些操作，化繁为简。

对于**编译器**和**调试器**，一般来讲，这两个东西是成双成对的，由`gcc`和`g++`编译的程序，使用`gdb`进行调试，由`clang`编译的程序使用`lldb`进行调试，由`msvc`编译的程序用`msvc`进行调试。除了`msvc`之外，其他的编译器调试器都是可以跨平台(Windows上通过`Mingw`实现)。 这几种**编译器**和**调试器**占据了大部分市场，所以实际上也没得选，根据平台，`Linux`下`GNU`的`gcc`、`g++`和`gdb`是主流，`macOS`下，`clang`+`lldb`是主流，使用`GNU`的也不能算少。`Windows`平台下比较复杂，主要有两种，一种是通过`Mingw`使用`gcc`的，另一种则是使用微软出品的*宇宙最强IDE*`Visual Studio`。

**全平台的一致C/C++开发体验**，不同**编译器**、**调试器**的使用方式并不相同。**调试器**的问题，`VSCode`通过`C/C++`插件已经解决了。而**编译器**的问题，需要一个通用的编译构建工具，通过`CMake`生成`Makefile`并编译。

梳理一下需要用到的东西。

1.  **文本编辑器**：Visual Studio Code。
2.  **C/C++编译器**：gcc/g++(Linux)、clang(macOS)、msvc(Windows)。
3.  **C/C++调试器**：gdb(Linux)、lldb(macOS)、msvc(Windows)。
4.  **构建工具**：CMake、Make。

------

## VSCode 

搜索安装扩展`C/C++`和`CMake`。

1.  `C/C++`。这个扩展的作用是C/C++代码提示和高亮、调试和代码文件关联跳转。
2.  `CMake`。这个扩展的作用是CMake语法提示和高亮。

## 编译器和调试器

### **Linux**

1.  **Debian**系列(Debian/Ubuntu)

    ```sh
    sudo apt install gcc
    sudo apt install g++
    sudo apt install gdb
    sudo apt install make
    ```

2.  **Redhat**系列(RHEL/CentOS/Fedora)

    ```sh
    sudo yum install gcc
    sudo yum install g++
    sudo yum install gdb
    sudo yum install make
    ```

### **macOS**

```sh
xcode-select --install
```

## CMake

### **Linux**

1.  **Debian**系列(Debian/Ubuntu)

    ```sh
    sudo apt install cmake
    ```

2.  **Redhat**系列(RHEL/CentOS/Fedora)

    ```sh
    sudo yum install cmake
    ```

### **macOS**

安装CMake需要用到[brew](https://links.jianshu.com/go?to=https%3A%2F%2Fbrew.sh%2F)，首先安装`brew`。

```sh
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
brew install cmake
```

## 例程

1.  创建C/C++项目文件夹，这里命名为`CStart`，并用VSCode打开该文件夹。
     新建下列文件结构

```tree
CStart
│—— CMakeLists.txt
│—— main.c
|—— .vscode
│—— │—— launch.json // 启动调试器
│—— │—— tasks.json // vscode集成任务管理，可以执行多种任务，主要用来make
```

2.  使用CMake构建C/C++项目
     **CMakeLists.txt** 项目cmake配置文件。关于CMakeLists.txt的编写，展开来讲有点长，可以到网上搜索相关教程。这是一个最简单的版本。通过这个文件，**统一全平台下的项目管理与构建配置**。

    ```cmake
    cmake_minimum_required (VERSION 2.8) #最低要求的CMake版本
    project(CStart) # 项目名称
    file(GLOB SRC_FILE *.c) # 建立变量SRC_FILE为目录下.c文件列表
    add_executable (${PROJECT_NAME} ${SRC_FILE}) # 要求编译可执行文件
    ```

    **main.c** 简单的HelloWorld代码

    ```c
    #include <stdio.h>
    
    int main(void) {
        printf("Hello world\n");
        return 0;
    }
    ```

3.  VSCode配置编译任务与调试对象
     在配置的时候会用到一些vscode的变量，用`${}`包裹起来的那些。
     `${workspaceFolder}`是当前工作空间（或vscode所打开根文件夹）在操作系统中绝对路径
     `${workspaceFolderBasename}`是当前工作空间（或vscode所打开根文件夹）的名称

    **tasks.json** 这是`VSCode`任务的配置文件，通过配置它可以快速执行各种命令。这里我们利用它来配置**编译构建流程**。我们要执行的任务为**建立build文件夹，在build文件夹中使用CMake生成并编译**。通过这个任务配置，**统一全平台下的程序编译命令**。

    ```jsonc
    {
        // See https://go.microsoft.com/fwlink/?LinkId=733558
        // for the documentation about the tasks.json format
        "version": "2.0.0",
        "tasks": [
            { // 在根文件夹中执行创建文件夹build的命令
                // 除windows系统外执行的命令为`mkdir -p build`
                // windows系统是在powershell中执行命令`mkdir -Force build`
                "label": "build_dir",
                "command": "mkdir",
                "type": "shell",
                "args": [
                    "-p",
                    "build"
                ],
                "windows": {
                    "options": {
                        "shell": {
                            "executable": "powershell.exe"
                        }
                    },
                    "args": [
                        "-Force",
                        "build"
                    ],
                }
            },
            { // 在build文件夹中调用cmake进行项目配置
                // 除windows系统外执行的命令为`cmake -DCMAKE_BUILD_TYPE=<Debug|Release|RelWithDebInfo|MinSizeRel> ../`
                // windows系统是在visual stuido的环境中执行命令`cmake -DCMAKE_BUILD_TYPE=<Debug|Release|RelWithDebInfo|MinSizeRel>  ../ -G "CodeBlocks - NMake Makefiles"`
                "label": "cmake",
                "type": "shell",
                "command": "cmake",
                "args": [
                    "-DCMAKE_BUILD_TYPE=${input:CMAKE_BUILD_TYPE}",
                    "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON", // 生成compile_commands.json 供c/c++扩展提示使用
                    "../"
                ],
                "options": {
                    "cwd": "${workspaceFolder}/build",
                },
                "windows": {
                    "args": [
                        "-DCMAKE_BUILD_TYPE=${input:CMAKE_BUILD_TYPE}",
                        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
                        "../",
                        "-G",
                        "\"CodeBlocks - NMake Makefiles\""
                    ],
                    "options": {
                        "shell": {
                            // "executable": "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\vcvarsall.bat",
                            // 需要根据安装的vs版本调用vs工具命令提示符
                            "executable": "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat",
                            "args": [
                                "${input:PLATFORM}", //指定平台
                                "-vcvars_ver=${input:vcvars_ver}", //指定vc环境版本
                                "&&"
                            ]
                        }
                    },
                },
                "dependsOn": [
                    "build_dir" // 在task `build_dir` 后执行该task
                ]
            },
            { // 在build文件夹中调用cmake编译构建debug程序
                // 执行的命令为`cmake --build ./ --target all --`
                //  windows系统如上需要在visual stuido的环境中执行命令
                "label": "build",
                "group": "build",
                "type": "shell",
                "command": "cmake",
                "args": [
                    "--build",
                    "./",
                    "--target",
                    "all",
                    "--"
                ],
                "options": {
                    "cwd": "${workspaceFolder}/build",
                },
                "problemMatcher": "$gcc",
                "windows": {
                    "options": {
                        "shell": {
                            // "executable": "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\vcvarsall.bat",
                            "executable": "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat",
                            "args": [
                                "${input:PLATFORM}",
                                "-vcvars_ver=${input:vcvars_ver}",
                                "&&"
                            ]
                        }
                    },
                    "problemMatcher": "$msCompile"
                },
                "dependsOn": [
                    "cmake" // 在task `cmake` 后执行该task
                ]
            }
        ],
        "inputs": [
            {
                "id": "CMAKE_BUILD_TYPE",
                "type": "pickString",
                "description": "What CMAKE_BUILD_TYPE do you want to create?",
                "options": [
                    "Debug",
                    "Release",
                    "RelWithDebInfo",
                    "MinSizeRel",
                ],
                "default": "Debug"
            },
            {
                "id": "PLATFORM",
                "type": "pickString",
                "description": "What PLATFORM do you want to create?",
                "options": [
                    "x86",
                    "amd64",
                    "arm",
                    "x86_arm",
                    "x86_amd64",
                    "amd64_x86",
                    "amd64_arm",
                ],
                "default": "amd64"
            },
            {
                "id": "vcvars_ver",
                "type": "pickString",
                "description": "What vcvars_ver do you want to create?",
                "options": [
                    "14.2", // 2019
                    "14.1", // 2017
                    "14.0", // 2015
                ],
                "default": "14.2"
            }
        ]
    }
    ```

    **launch.json** 这是`VSCode`运行调试的配置文件。**全平台统一的调试体验**就靠它了。依赖于VSCode的C/C++扩展。这里需要告诉VSCode你的**C/C++程序在哪**，以及**运行参数**，**工作目录**等，**用哪个调试器调试**。

    ```jsonc
    {
        // Use IntelliSense to learn about possible attributes.
        // Hover to view descriptions of existing attributes.
        // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
        "version": "0.2.0",
        "configurations": [
            {
                "name": "Launch Debug", //名称
                "type": "cppdbg", //调试类型，除使用msvc进行调试外，均为该类型
                "request": "launch",
                "program": "${workspaceFolder}/bin/${workspaceFolderBasename}", //指定C/C++程序位置
                "args": [], //指定运行参数
                "stopAtEntry": false, // 设为true时程序将暂停在程序入口处，相当于在main上打断点
                "cwd": "${workspaceFolder}", //指定工作目录
                "preLaunchTask": "build", //在调试前会先调用build_debug这个task编译构建程序
                "environment": [], // 环境变量
                "externalConsole": false, // 为true时使用单独的cmd窗口，与其它IDE一致；18年10月后设为false可调用VSC内置终端
                "osx": { //macOS的特定配置
                    // "miDebuggerPath": "/Applications/Xcode.app/Contents/ Developer/usr/bin/lldb-mi", //修改使用的lldb-mi，一般不需要修改
                    "MIMode": "lldb" //指定使用lldb进行调试
                },
                "linux": { //linux的特定配置
                    "MIMode": "gdb", //指定使用gdb调试
                    "setupCommands": [
                        {
                            "description": "Enable pretty-printing for gdb",
                            "text": "-enable-pretty-printing",
                            "ignoreFailures": true
                        }
                    ]
                },
                // "windows": { //windows的特定配置
                //     "type": "cppvsdbg", //指定使用msvc进行调试
                //     "program": "${workspaceFolder}/build/${workspaceFolderBasename}.exe", //指定C/C++程序位置
                // }
            }
        ]
    }
    ```

    加入断点，左键单击即可，右键可以设置条件断点等。

    按下`F5`，`VSCode`开始执行`launch.json`的命令，在执行前会先运行`preLaunchTask`，编译C代码。

## CMake Settings

### **1. 指令说明**

**target_include_directories()**：指定**目标**包含的头文件路径。[官方文档](https://link.zhihu.com/?target=https%3A//cmake.org/cmake/help/v3.15/command/target_include_directories.html%3Fhighlight%3Dtarget_include_directories)

**target_link_libraries()**：指定**目标**链接的库。[官方文档](https://link.zhihu.com/?target=https%3A//cmake.org/cmake/help/v3.15/command/target_link_libraries.html%3Fhighlight%3Dtarget_link_libraries)

**target_compile_options()**：指定**目标**的编译选项。[官方文档](https://link.zhihu.com/?target=https%3A//cmake.org/cmake/help/v3.15/command/target_compile_options.html%23command%3Atarget_compile_options)

**目标** 由 *add_library()* 或 *add_executable()* 生成。

这三个指令类似，这里以 **target_include_directories()** 为例进行讲解。

### **2. 指令讲解**

**测试工程目录结构：**

```bash
cmake-test/                 工程主目录，main.c 调用 libhello-world.so
├── CMakeLists.txt
├── hello-world             生成 libhello-world.so，调用 libhello.so 和 libworld.so
│   ├── CMakeLists.txt
│   ├── hello               生成 libhello.so 
│   │   ├── CMakeLists.txt
│   │   ├── hello.c
│   │   └── hello.h         libhello.so 对外的头文件
│   ├── hello_world.c
│   ├── hello_world.h       libhello-world.so 对外的头文件
│   └── world               生成 libworld.so
│       ├── CMakeLists.txt
│       ├── world.c
│       └── world.h         libworld.so 对外的头文件
└── main.c
```

**调用关系：**

```bash
                                 ├────libhello.so
可执行文件────libhello-world.so
                                 ├────libworld.so
```

**关键字用法说明：**

**PRIVATE**：私有的。生成 libhello-world.so时，只在 hello_world.c 中包含了 hello.h，libhello-world.so **对外**的头文件——hello_world.h 中不包含 hello.h。而且 main.c 不会调用 hello.c 中的函数，或者说 main.c 不知道 hello.c 的存在，那么在 hello-world/CMakeLists.txt 中应该写入：

```cmake
target_link_libraries(hello-world PRIVATE hello)
target_include_directories(hello-world PRIVATE hello)
```

**INTERFACE**：接口。生成 libhello-world.so 时，只在libhello-world.so **对外**的头文件——hello_world.h 中包含 了 hello.h， hello_world.c 中不包含 hello.h，即 libhello-world.so 不使用 libhello.so 提供的功能，只使用 hello.h 中的某些信息，比如结构体。但是 main.c 需要使用 libhello.so 中的功能。那么在 hello-world/CMakeLists.txt 中应该写入：

```cmake
target_link_libraries(hello-world INTERFACE hello)
target_include_directories(hello-world INTERFACE hello)
```

**PUBLIC**：公开的。**PUBLIC = PRIVATE + INTERFACE**。生成 libhello-world.so 时，在 hello_world.c 和 hello_world.h 中都包含了 hello.h。并且 main.c 中也需要使用 libhello.so 提供的功能。那么在 hello-world/CMakeLists.txt 中应该写入：

```cmake
target_link_libraries(hello-world PUBLIC hello)
target_include_directories(hello-world PUBLIC hello)
```

实际上，这三个关键字指定的是目标文件依赖项的使用**范围（scope）**或者一种**传递（propagate）**。[官方说明](https://link.zhihu.com/?target=https%3A//cmake.org/cmake/help/v3.15/manual/cmake-buildsystem.7.html%23transitive-usage-requirements)

可执行文件依赖 libhello-world.so， libhello-world.so 依赖 libhello.so 和 libworld.so。

1.  main.c 不使用 libhello.so 的任何功能，因此 libhello-world.so 不需要将其依赖—— libhello.so 传递给 main.c，hello-world/CMakeLists.txt 中使用 PRIVATE 关键字；
2.  main.c 使用 libhello.so 的功能，但是libhello-world.so 不使用，hello-world/CMakeLists.txt 中使用 INTERFACE 关键字；
3.  main.c 和 libhello-world.so 都使用 libhello.so 的功能，hello-world/CMakeLists.txt 中使用 PUBLIC 关键字；

### **3. include_directories(dir)**

`target_include_directories()` 的功能完全可以使用 `include_directories()` 实现。但是我还是建议使用 `target_include_directories()`。为什么？保持清晰！

`include_directories(header-dir)` 是一个全局包含，向下传递。什么意思呢？就是说如果某个目录的 CMakeLists.txt 中使用了该指令，其下所有的子目录默认也包含了`header-dir` 目录。

上述例子中，如果在顶层的 cmake-test/CMakeLists.txt 中加入：

```cmake
include_directories(hello-world)
include_directories(hello-world/hello)
include_directories(hello-world/world)
```

那么整个工程的源文件在编译时**都**会增加：

```bash
-I hello-world -I hello-world/hello -I hello-world/world ...
```

各级子目录中无需使用 `target_include_directories()` 或者 `include_directories()`了。如果此时查看详细的编译过程（`make VERBOSE=1`）就会发现编译过程是一大坨，很不舒服。

当然了，在**最终子目录**的 CMakeLists.txt 文件中，使用 `include_directories()` 和 `target_include_directories()` 的效果是相同的。

### **4. 目录划分**

每一个目录都是一个模块，目录内部应将对外和对内的头文件进行区分，由模块的调用者决定模块是否被传递（PRIVATE，INTERFACE，PUBLIC）。



---





```cmake
#CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)
#项目名称
SET(PROJECT_NAME DataStruct)
project(${PROJECT_NAME})
#the version number
# no use

#允许gdb调试
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


#添加C++11支持及其他选项
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs -Wno-deprecated")

#set execute path
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set library path
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#生成项目的可执行文件
add_executable(${PROJECT_NAME}
    src/datastruct.cpp 
)
#为可执行文件添加源文件cpp
target_sources(${PROJECT_NAME}
    PRIVATE
    src/utils.cpp
    test/queue_test.cpp test/tree_test.cpp
)
#添加头文件目录
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/test/include
)
#编译选项
target_compile_options(${PROJECT_NAME}
    PRIVATE
)
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
)



```

