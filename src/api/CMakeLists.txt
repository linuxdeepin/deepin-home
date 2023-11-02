cmake_minimum_required(VERSION 3.2)
project(API)
add_library(${PROJECT_NAME}
        api.h
        api.cpp
        diskCacheShare.h
        diskCacheShare.cpp
        getFeedbackParam.h
)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} QtDeepinHomeAPI)