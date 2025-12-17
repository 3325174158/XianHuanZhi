cmake_minimum_required(VERSION 3.10)
project(XianHuanZhi VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 可执行文件
add_executable(XianHuanZhi
    main.cpp
    XianHuanZhi.cpp
    ItemManager.cpp
    XianHuanZhiSystem.cpp
    AdminManager.cpp
)

# 头文件包含路径
target_include_directories(XianHuanZhi PRIVATE .)