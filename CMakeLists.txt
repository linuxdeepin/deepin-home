cmake_minimum_required(VERSION 3.13)

project(deepin-home VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)
find_package(DtkDeclarative REQUIRED)

# 旧版本Dtk不支持使用cmake find_package 获取版本号， 使用 PkgConfig 来获取版本号
find_package(PkgConfig REQUIRED)
pkg_check_modules(DtkDeclarativeModule REQUIRED dtkdeclarative)
# DtkDeclarative 在 5.6.3 更改了插件的路径和使用方式 See: https://github.com/linuxdeepin/dtkdeclarative/commit/a9701858155b7118bfb53b6f241e35b9f6c0f15e
if(${DtkDeclarativeModule_VERSION} VERSION_LESS "5.6.3")
  set(DTK_OLD_VERSION True)
endif()

add_definitions(-DAPP_VERSION="${APP_VERSION}")
add_subdirectory(src)
add_subdirectory(3rdparty/apiserver/client)

set(ENV{QT_SELECT} qt5)
execute_process(COMMAND bash "-c" "lrelease translations/**/*"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
