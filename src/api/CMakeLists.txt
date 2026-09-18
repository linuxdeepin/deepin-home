cmake_minimum_required(VERSION 3.2)
project(API)
add_library(${PROJECT_NAME}
        api.h
        api.cpp
        getFeedbackParam.h
)
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} QtDeepinHomeAPI)
