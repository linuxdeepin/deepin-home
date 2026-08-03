set(PRELOAD_LIB_NAME ${CMAKE_PROJECT_NAME}-preload)
set(PRELOAD_SRC
    preloadplugin.cpp
    )
include_directories(src/preloadplugin)
add_library(${PRELOAD_LIB_NAME} SHARED
    ${PRELOAD_SRC}
    preload.qrc
    )
target_link_libraries(${PRELOAD_LIB_NAME}
    Qt6::Quick
    ${Dtk6Declarative_LIBRARIES}
    )
install(TARGETS ${PRELOAD_LIB_NAME} DESTINATION ${DTK_QML_APP_PLUGIN_PATH})
