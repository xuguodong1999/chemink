cmake_minimum_required(VERSION 3.15)
project(ChemInk-Project)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (MSVC)
    add_compile_options($<$<C_COMPILER_ID:MSVC>:/utf-8>)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/utf-8>)
endif ()

# 配置Qt
if (MSVC)   # TODO: windows下修改到Qt库的cmake文件夹位置
    # set(Qt5_DIR C:/Qt/5.14.2/msvc2017_64/lib/cmake/Qt5)
    set(Qt5_DIR C:/Qt/5.15.1/msvc2019_64/lib/cmake/Qt5)
elseif (APPLE)
    set(Qt5_DIR /Users/xgd/Qt/5.14.2/clang_64/lib/cmake/Qt5)
elseif (UNIX)       # TODO: linux下修改到Qt库的cmake文件夹位置
    set(Qt5_DIR /home/xgd/Qt/5.15.0/gcc_64/lib/cmake/Qt5)
endif ()
find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 配置 OpenCV
if (UNIX)
    set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/lib/opencv/lib/cmake/opencv4)
elseif (MSVC)
    set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/lib/opencv/x64/vc16/staticlib)
endif ()
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# 配置 OpenBabel
set(OPENBABEL2_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/openbabel/include/openbabel3)
if (MSVC)
    if (CMAKE_BUILD_TYPE)
        if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
            set(OPENBABEL2_LIBRARIES ${CMAKE_SOURCE_DIR}/lib/openbabel/bin/openbabel-3d.lib)
        else ()
            set(OPENBABEL2_LIBRARIES ${CMAKE_SOURCE_DIR}/lib/openbabel/bin/openbabel-3.lib)
        endif ()
    else ()
        set(OPENBABEL2_LIBRARIES ${CMAKE_SOURCE_DIR}/lib/openbabel/bin/openbabel-3.lib)
    endif ()
elseif (APPLE)
    set(OPENBABEL2_LIBRARIES ${CMAKE_SOURCE_DIR}/lib/openbabel/lib/libopenbabel.dylib)
elseif (UNIX)
    set(OPENBABEL2_LIBRARIES ${CMAKE_SOURCE_DIR}/lib/openbabel/lib/libopenbabel.so)
endif ()
include_directories(${OPENBABEL2_INCLUDE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${SRC_DIR})

set(CMAKE_DEBUG_POSTFIX d)

add_subdirectory(chemink)
