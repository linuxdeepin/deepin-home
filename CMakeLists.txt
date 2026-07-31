cmake_minimum_required(VERSION 3.13)

project(deepin-home VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

# Auto-detect Qt version (tries Qt6 first, falls back to Qt5)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
message(STATUS "Found Qt version: ${QT_VERSION_MAJOR}")

# Map to DTK version (Qt6->DTK6, Qt5->DTK5)
if(QT_VERSION_MAJOR MATCHES 6)
    set(DTK_VERSION_MAJOR 6)
else()
    set(DTK_VERSION_MAJOR "")
endif()
message(STATUS "Build with DTK: ${DTK_VERSION_MAJOR}")

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick DBus)
find_package(Dtk${DTK_VERSION_MAJOR}Declarative REQUIRED)

add_definitions(-DAPP_VERSION="${APP_VERSION}")
add_subdirectory(src)
add_subdirectory(3rdparty/apiserver/client)

set(ENV{QT_SELECT} qt${QT_VERSION_MAJOR})
execute_process(COMMAND bash "-c" "lrelease translations/**/*"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
