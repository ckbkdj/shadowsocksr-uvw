# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------
# Copyright © 2011-2015, RedJack, LLC.
# All rights reserved.
#
# Please see the COPYING file in this distribution for license details.
# ----------------------------------------------------------------------

cmake_minimum_required(VERSION 3.2)
set(PROJECT_NAME shadowsocksr-uvw)
project(${PROJECT_NAME})

set(VERSION 0.0.1)
# Detect linux
if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()
if(WIN32)
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        if(CMAKE_CL_64)
            include(${CMAKE_SOURCE_DIR}/libs/x64-windows/scripts/buildsystems/vcpkg.cmake)
        else()
            include(${CMAKE_SOURCE_DIR}/libs/x86-windows/scripts/buildsystems/vcpkg.cmake)
        endif()
    endif()
endif()

set(WITH_CRYPTO_LIBRARY "openssl" CACHE STRING "build with the given crypto library, TYPE=openssl|polarssl|mbedtls default=openssl")
set(SSR_UVW_WITH_QT 1 CACHE BOOL "build with qt")
option(USE_SYSTEM_SODIUM "use system sodium" OFF)
option(USE_SYSTEM_LIBUV "use system libuv" OFF)
option(STATIC_LINK_LIBUV "static link libuv" ON)
option(STATIC_LINK_SODIUM "static link libsodium" ON)
if(NOT USE_SYSTEM_SODIUM AND NOT STATIC_LINK_SODIUM)
    message(FATAL_ERROR "Not support dynamic linking libsodium without using system libsodium!")
endif()
if(NOT USE_SYSTEM_LIBUV AND NOT STATIC_LINK_LIBUV)
    message(FATAL_ERROR "Not support dynamic linking libuv without using system libuv!")
endif()


if(NOT USE_SYSTEM_SODIUM)
    configure_file(cmake/souium.cmake ${CMAKE_CURRENT_SOURCE_DIR}/libsodium/CMakeLists.txt COPYONLY)
    if(NOT STATIC_LINK_SODIUM)
        add_definitions(-DSODIUM_BUILD_SHARED_LIBS)
    endif()
    add_subdirectory(libsodium)
    set(sodium_LIBRARIES sodium::sodium)
elseif(USE_SYSTEM_SODIUM AND STATIC_LINK_SODIUM)
    add_definitions(-Dsodium_USE_STATIC_LIBS)
endif()
if(NOT USE_SYSTEM_LIBUV)
    set(LibUV_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} libuv/include)
    add_subdirectory(libuv)
    if(STATIC_LINK_LIBUV)
        add_library(uv::uv-static ALIAS uv_a)
        set_target_properties(uv_a PROPERTIES POSITION_INDEPENDENT_CODE 1)
        set(LibUV_LIBRARIES uv::uv-static)
    else()
        add_library(uv::uv-shared ALIAS uv)
        set_target_properties(uv PROPERTIES POSITION_INDEPENDENT_CODE 1)
        set(LibUV_LIBRARIES uv::uv-shared)
    endif()
endif()
include ( cmake/dist.cmake )
include ( configure )
configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )


include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

add_subdirectory(src)
