cmake_minimum_required(VERSION 3.13.1)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug, Release, or MinSizeRel." FORCE)
    message(STATUS "CMAKE_BUILD_TYPE not set, defaulting to Release.")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/cmake")

if(NOT BUILD_NUMBER)
    set(BUILD_NUMBER 0)
endif()
set(PIPC_VERSION 1.0.${BUILD_NUMBER})

set(PACKAGE_NAME pip2cmake)
project(${PACKAGE_NAME} VERSION "${PIPC_VERSION}" LANGUAGES CXX C)

message(STATUS "Generator .............. ${CMAKE_GENERATOR}")
message(STATUS "Build Type ............. ${CMAKE_BUILD_TYPE}")

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/usr)

include(ExternalProject)

ExternalProject_Add(pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml.git
    GIT_TAG v1.10
    GIT_SHALLOW 1
    BUILD_IN_SOURCE 0
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=MinSizeRel
        -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_INSTALL_PREFIX}/include)
link_directories(${CMAKE_INSTALL_PREFIX}/lib)

add_executable(pip2cmake main.cpp pip.cpp)
add_dependencies(pip2cmake pugixml)

if(UNIX)
    target_link_libraries(pip2cmake -lpugixml)
elseif(MSVC)
    target_link_libraries(pip2cmake pugixml.lib)
endif()

