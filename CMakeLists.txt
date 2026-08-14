cmake_minimum_required(VERSION 3.13)

project(deepin-home VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick LinguistTools)
find_package(Dtk6Declarative REQUIRED)

add_definitions(-DAPP_VERSION="${APP_VERSION}")
add_subdirectory(src)
add_subdirectory(3rdparty/apiserver/client)
