cmake_minimum_required(VERSION 3.2)

project(QtDeepinHomeAPI)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Gui)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(${PROJECT_NAME}
  DHHandlers_BBSURLRequest.h
  DHHandlers_BBSURLResponse.h
  DHHandlers_ClientBBSTokenRequest.h
  DHHandlers_ClientBBSTokenResponse.h
  DHHandlers_ClientLoginRequest.h
  DHHandlers_ClientLoginResponse.h
  DHHandlers_ClientMessagesResponse.h
  DHHandlers_ClientUserInfoResponse.h
  DHHandlers_LanguageCodeResponse.h
  DHHandlers_LoginConfigResponse.h
  DHHandlers_NodeSelectResponse.h
  DHHandlers_PublicStatResponse.h
  DHHandlers_PublicTopicsResponse.h
  DHHandlers_Topic.h
  DHClientApi.h
  DHUserApi.h
  DHHelpers.h
  DHHttpRequest.h
  DHObject.h
  DHEnum.h
  DHHttpFileElement.h
  DHServerConfiguration.h
  DHServerVariable.h
  DHOauth.h
  DHHandlers_BBSURLRequest.cpp
  DHHandlers_BBSURLResponse.cpp
  DHHandlers_ClientBBSTokenRequest.cpp
  DHHandlers_ClientBBSTokenResponse.cpp
  DHHandlers_ClientLoginRequest.cpp
  DHHandlers_ClientLoginResponse.cpp
  DHHandlers_ClientMessagesResponse.cpp
  DHHandlers_ClientUserInfoResponse.cpp
  DHHandlers_LanguageCodeResponse.cpp
  DHHandlers_LoginConfigResponse.cpp
  DHHandlers_NodeSelectResponse.cpp
  DHHandlers_PublicStatResponse.cpp
  DHHandlers_PublicTopicsResponse.cpp
  DHHandlers_Topic.cpp
  DHClientApi.cpp
  DHUserApi.cpp
  DHHelpers.cpp
  DHHttpRequest.cpp
  DHHttpFileElement.cpp
  DHOauth.cpp
  )

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Gui

)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
)

install(
    FILES ${HEADER_FILES}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
    EXPORT ${PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
