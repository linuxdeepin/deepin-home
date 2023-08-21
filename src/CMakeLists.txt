# Application
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")
    add_definitions(-DLOCALLIBPATH="${LIBRARY_OUTPUT_PATH}")
endif()

add_subdirectory(api)
add_subdirectory(main)
add_subdirectory(preloadplugin)
add_subdirectory(maincomponentplugin)
