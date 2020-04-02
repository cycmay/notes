#CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)
#项目名称
SET(PROJECT_NAME DEMOS)
project(${PROJECT_NAME})
#the version number
# no use

#允许gdb调试
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


#添加C++11支持及其他选项
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g") # -ftest-coverage -fprofile-arcs -Wno-deprecated")

#set execute path
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set library path
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# cpp 文件目录
aux_source_directory(
    src
    test
)
#生成项目的可执行文件
add_executable(${PROJECT_NAME}
    src/DEMOS.cpp
)
#为可执行文件添加源文件cpp
target_sources(${PROJECT_NAME}
    PRIVATE
    test/producer-consumer_test.cpp
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


