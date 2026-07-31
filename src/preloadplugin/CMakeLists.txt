set(PRELOAD_LIB_NAME ${CMAKE_PROJECT_NAME}-preload)
set(PRELOAD_SRC
    preloadplugin.cpp
    )

# Qt5 uses qtquick_compiler_add_resources for AOT compilation,
# Qt6 uses qt_add_resources
if(QT_VERSION_MAJOR EQUAL 5)
    find_package(Qt5QuickCompiler REQUIRED)
    qtquick_compiler_add_resources(PRELOAD_RCC_SOURCES preload.qrc)
else()
    qt_add_resources(PRELOAD_RCC_SOURCES preload.qrc)
endif()

include_directories(src/preloadplugin)
add_library(${PRELOAD_LIB_NAME} SHARED
    ${PRELOAD_SRC}
    ${PRELOAD_RCC_SOURCES}
    )
target_link_libraries(${PRELOAD_LIB_NAME}
    Qt${QT_VERSION_MAJOR}::Quick
    ${DtkDeclarative_LIBRARIES}
    )
install(TARGETS ${PRELOAD_LIB_NAME} DESTINATION ${DTK_QML_APP_PLUGIN_PATH})
