cmake_minimum_required(VERSION 3.13)

project(deepin-home VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(QT_VERSION_MAJOR 6)
set(DTK_VERSION_MAJOR 6)

find_package(QT NAMES Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)
find_package(Dtk${DTK_VERSION_MAJOR}Declarative REQUIRED)

# 旧版本Dtk不支持使用cmake find_package 获取版本号， 使用 PkgConfig 来获取版本号
find_package(PkgConfig REQUIRED)
pkg_check_modules(Dtk${DTK_VERSION_MAJOR}DeclarativeModule REQUIRED dtk${DTK_VERSION_MAJOR}declarative)

add_definitions(-DAPP_VERSION="${APP_VERSION}")
add_subdirectory(src)
add_subdirectory(3rdparty/apiserver/client)

execute_process(COMMAND bash "-c" "/usr/lib/qt${QT_VERSION_MAJOR}/bin/lrelease translations/**/*"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})