cmake_minimum_required(VERSION 3.6)
project(libmaia VERSION 1.0.0)

option(BUILD_EXAMPLE "Build example" OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5Xml CONFIG REQUIRED)

set(maia_headers
    maia/maiaFault.h
    maia/maiaObject.h
    maia/maiaXmlRpcClient.h
    maia/maiaXmlRpcServerConnection.h
    maia/maiaXmlRpcServer.h
    )

set(maia_sources
    maia/maiaFault.cpp
    maia/maiaObject.cpp
    maia/maiaXmlRpcClient.cpp
    maia/maiaXmlRpcServerConnection.cpp
    maia/maiaXmlRpcServer.cpp
    )

add_library(maia ${maia_sources} ${maia_headers})
target_include_directories(maia PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>)
target_link_libraries(maia Qt5::Network Qt5::Xml)

if (${BUILD_EXAMPLE})
    add_subdirectory(examples)
endif()

# Install stuff

set(include_install_dir "include")
set(lib_install_dir "lib")
set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(config_export_name "${PROJECT_NAME}Config")
set(targets_export_name "${PROJECT_NAME}Targets")
set(project_config "${CMAKE_CURRENT_BINARY_DIR}/${config_export_name}.cmake")
set(version_config "${CMAKE_CURRENT_BINARY_DIR}/${config_export_name}Version.cmake")
set(namespace "${PROJECT_NAME}::")

include (CMakePackageConfigHelpers)
configure_package_config_file(
    "config.cmake.in"
    ${project_config}
    INSTALL_DESTINATION ${config_install_dir}
)
write_basic_package_version_file(
    ${version_config}
    VERSION ${VERSION}
    COMPATIBILITY SameMajorVersion
)

add_library(${namespace}maia ALIAS maia)

install(TARGETS maia
    EXPORT ${targets_export_name}
    INCLUDES DESTINATION "${include_install_dir}"
    LIBRARY DESTINATION ${lib_install_dir}
    ARCHIVE DESTINATION ${lib_install_dir}
    PUBLIC_HEADER DESTINATION ${include_install_dir}
)

install(EXPORT ${targets_export_name}
    NAMESPACE ${namespace}
    DESTINATION ${config_install_dir})
install(FILES ${project_config} ${version_config}
    DESTINATION ${config_install_dir})

install(FILES ${maia_headers} DESTINATION include/maia)
